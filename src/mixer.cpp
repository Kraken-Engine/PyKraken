#include "Mixer.hpp"

#include <pybind11/native_enum.h>

#include <algorithm>
#include <limits>
#include <stdexcept>

#include "Log.hpp"

namespace kn::mixer
{
constexpr uint8_t MAX_TRACKS = 64;
constexpr uint8_t MAX_POLYPHONY = 32;

enum class TrackUsage : uint8_t
{
    None = 0,
    Sample = 1,
    Stream = 2,
};

struct TrackInfo
{
    MIX_Track* track = nullptr;
    MIX_Audio* audio = nullptr;  // currently assigned audio (if any)
    AudioPriority priority = AudioPriority::SFX;
    uint64_t started_seq = 0;
    TrackUsage usage = TrackUsage::None;
};

static MIX_Mixer* _mixer = nullptr;
static TrackInfo _tracks[MAX_TRACKS] = {};
static uint64_t _playSeq = 1;

static uint8_t _countPlayingInstances(MIX_Audio* audio);
static int _trackIndex(const TrackInfo* trackInfo);
static void _clearTrackAssignment(TrackInfo& trackInfo);
static TrackInfo* _acquireTrack(AudioPriority priority, TrackUsage usage, bool canSteal);

static Sint64 _secondsToMs(double seconds);
static SDL_PropertiesID _buildPlayOptions(bool looping, double fadeInSeconds);
static Sint64 _fadeOutFramesForTrack(MIX_Track* track, double fadeOutSeconds);

std::shared_ptr<Sample> loadSample(const std::string& path, const bool predecode)
{
    MIX_Audio* audio = MIX_LoadAudio(_mixer, path.c_str(), predecode);
    if (audio == nullptr)
    {
        throw std::runtime_error(
            std::string("Failed to load sample '") + path + "': " + SDL_GetError()
        );
    }

    return std::shared_ptr<Sample>(new Sample(audio));
}

std::shared_ptr<Stream> loadStream(const std::string& path, const bool predecode)
{
    MIX_Audio* audio = MIX_LoadAudio(_mixer, path.c_str(), predecode);
    if (audio == nullptr)
    {
        throw std::runtime_error(
            std::string("Failed to load stream '") + path + "': " + SDL_GetError()
        );
    }

    return std::shared_ptr<Stream>(new Stream(audio));
}

void setMasterVolume(float volume)
{
    volume = std::clamp(volume, 0.0f, 1.0f);
    MIX_SetMixerGain(_mixer, volume);
}

float getMasterVolume()
{
    return MIX_GetMixerGain(_mixer);
}

Audio::Audio(MIX_Audio* sdlAudio)
    : m_audio(sdlAudio)
{
}

Audio::~Audio()
{
    if (m_audio != nullptr)
    {
        MIX_DestroyAudio(m_audio);
        m_audio = nullptr;
    }
}

float Audio::getVolume() const
{
    return m_volume;
}

Sample::Sample(MIX_Audio* sdlAudio)
    : Audio(sdlAudio)
{
}

void Sample::setMaxPolyphony(uint8_t maxPolyphony)
{
    maxPolyphony = std::clamp(maxPolyphony, uint8_t(1), MAX_POLYPHONY);
    m_maxPolyphony = maxPolyphony;
}

uint8_t Sample::getMaxPolyphony() const
{
    return m_maxPolyphony;
}

void Sample::setVolume(float volume)
{
    m_volume = std::clamp(volume, 0.0f, 1.0f);

    if (_mixer == nullptr || m_audio == nullptr)
        return;

    for (auto& trackInfo : _tracks)
    {
        if (!trackInfo.track)
            continue;
        if (trackInfo.audio != m_audio)
            continue;
        if (trackInfo.usage != TrackUsage::Sample)
            continue;
        if (!MIX_TrackPlaying(trackInfo.track))
            continue;

        MIX_SetTrackGain(trackInfo.track, m_volume);
    }
}

bool Sample::isPlaying() const
{
    if (_mixer == nullptr || m_audio == nullptr)
        return false;

    for (const auto& trackInfo : _tracks)
    {
        if (!trackInfo.track || trackInfo.audio != m_audio)
            continue;
        if (trackInfo.usage != TrackUsage::Sample)
            continue;

        if (MIX_TrackPlaying(trackInfo.track))
            return true;
    }

    return false;
}

void Sample::play(const double fadeInSeconds)
{
    if (_mixer == nullptr)
        throw std::runtime_error("Mixer not initialized");
    if (m_audio == nullptr)
        throw std::runtime_error("Audio not loaded");

    if (_countPlayingInstances(m_audio) >= m_maxPolyphony)
        return;

    TrackInfo* trackInfo = _acquireTrack(priority, TrackUsage::Sample, canSteal);
    if (trackInfo == nullptr)
        return;

    if (MIX_TrackPlaying(trackInfo->track))
    {
        if (!MIX_StopTrack(trackInfo->track, 0))
            throw std::runtime_error(std::string("Failed to stop track: ") + SDL_GetError());
    }

    if (!MIX_SetTrackAudio(trackInfo->track, m_audio))
        throw std::runtime_error(std::string("Failed to set track audio: ") + SDL_GetError());

    const SDL_PropertiesID options = _buildPlayOptions(m_looping, fadeInSeconds);
    if (!MIX_PlayTrack(trackInfo->track, options))
    {
        if (options != 0)
            SDL_DestroyProperties(options);
        throw std::runtime_error(std::string("Failed to play track: ") + SDL_GetError());
    }
    if (options != 0)
        SDL_DestroyProperties(options);

    trackInfo->audio = m_audio;
    trackInfo->priority = priority;
    trackInfo->started_seq = _playSeq++;
    trackInfo->usage = TrackUsage::Sample;

    // Apply per-audio volume to this new instance.
    if (trackInfo->track)
        MIX_SetTrackGain(trackInfo->track, m_volume);
}

void Sample::stop(const double fadeOutSeconds)
{
    if (_mixer == nullptr)
        throw std::runtime_error("Mixer not initialized");
    if (m_audio == nullptr)
        throw std::runtime_error("Audio not loaded");

    for (auto& trackInfo : _tracks)
    {
        if (!trackInfo.track || trackInfo.audio != m_audio)
            continue;
        if (trackInfo.usage != TrackUsage::Sample)
            continue;

        const Sint64 fadeFrames = _fadeOutFramesForTrack(trackInfo.track, fadeOutSeconds);
        if (!MIX_StopTrack(trackInfo.track, fadeFrames))
            throw std::runtime_error(std::string("Failed to stop track: ") + SDL_GetError());
    }
}

Stream::Stream(MIX_Audio* sdlAudio)
    : Audio(sdlAudio)
{
}

Stream::~Stream()
{
    if (_mixer != nullptr && m_trackIndex >= 0 && m_trackIndex < MAX_TRACKS)
    {
        TrackInfo& trackInfo = _tracks[m_trackIndex];
        if (trackInfo.track)
        {
            (void)MIX_StopTrack(trackInfo.track, 0);
            _clearTrackAssignment(trackInfo);
        }
    }
    m_trackIndex = -1;

    // m_audio is destroyed by Audio base class destructor
}

void Stream::setLooping(const bool looping)
{
    m_looping = looping;

    // Best-effort: update currently playing instance too.
    if (_mixer == nullptr || m_audio == nullptr)
        return;
    if (m_trackIndex < 0 || m_trackIndex >= MAX_TRACKS)
        return;

    TrackInfo& trackInfo = _tracks[m_trackIndex];
    if (!trackInfo.track)
        return;
    if (trackInfo.usage != TrackUsage::Stream)
        return;

    const int loops = m_looping ? -1 : 0;
    MIX_SetTrackLoops(trackInfo.track, loops);
}

bool Stream::getLooping() const
{
    return m_looping;
}

void Stream::setVolume(float volume)
{
    m_volume = std::clamp(volume, 0.0f, 1.0f);

    if (_mixer == nullptr || m_audio == nullptr)
        return;
    if (m_trackIndex < 0 || m_trackIndex >= MAX_TRACKS)
        return;

    TrackInfo& trackInfo = _tracks[m_trackIndex];
    if (!trackInfo.track)
        return;
    if (trackInfo.usage != TrackUsage::Stream)
        return;
    if (!MIX_TrackPlaying(trackInfo.track))
        return;

    MIX_SetTrackGain(trackInfo.track, m_volume);
}

bool Stream::isPlaying() const
{
    if (_mixer == nullptr || m_audio == nullptr)
        return false;
    if (m_trackIndex < 0 || m_trackIndex >= MAX_TRACKS)
        return false;

    const TrackInfo& trackInfo = _tracks[m_trackIndex];
    if (!trackInfo.track || trackInfo.usage != TrackUsage::Stream)
        return false;

    return MIX_TrackPlaying(trackInfo.track);
}

double Stream::getPlaybackPos() const
{
    if (_mixer == nullptr || m_audio == nullptr)
        return 0.0;

    if (m_trackIndex >= 0 && m_trackIndex < MAX_TRACKS)
    {
        const TrackInfo& trackInfo = _tracks[m_trackIndex];
        if (trackInfo.track && trackInfo.usage == TrackUsage::Stream &&
            MIX_TrackPlaying(trackInfo.track))
        {
            const Sint64 frames = MIX_GetTrackPlaybackPosition(trackInfo.track);
            if (frames >= 0)
            {
                const Sint64 ms = MIX_TrackFramesToMS(trackInfo.track, frames);
                if (ms >= 0)
                    return static_cast<double>(ms) / 1000.0;
            }
        }
    }

    const Sint64 ms = MIX_AudioFramesToMS(m_audio, m_savedFrames);
    if (ms < 0)
        return 0.0;

    return static_cast<double>(ms) / 1000.0;
}

void Stream::play(const double fadeInSeconds)
{
    if (_mixer == nullptr)
        throw std::runtime_error("Mixer not initialized");
    if (m_audio == nullptr)
        throw std::runtime_error("Audio not loaded");

    stop(0.0);
    m_savedFrames = 0;

    TrackInfo* trackInfo = _acquireTrack(priority, TrackUsage::Stream, canSteal);
    if (trackInfo == nullptr)
        return;

    const int idx = _trackIndex(trackInfo);
    if (idx < 0)
        return;
    m_trackIndex = idx;

    if (MIX_TrackPlaying(trackInfo->track))
    {
        if (!MIX_StopTrack(trackInfo->track, 0))
            throw std::runtime_error(std::string("Failed to stop track: ") + SDL_GetError());
    }

    if (!MIX_SetTrackAudio(trackInfo->track, m_audio))
        throw std::runtime_error(std::string("Failed to set track audio: ") + SDL_GetError());

    const SDL_PropertiesID options = _buildPlayOptions(m_looping, fadeInSeconds);
    if (!MIX_PlayTrack(trackInfo->track, options))
    {
        if (options != 0)
            SDL_DestroyProperties(options);
        throw std::runtime_error(std::string("Failed to play track: ") + SDL_GetError());
    }
    if (options != 0)
        SDL_DestroyProperties(options);

    trackInfo->audio = m_audio;
    trackInfo->priority = priority;
    trackInfo->started_seq = _playSeq++;
    trackInfo->usage = TrackUsage::Stream;

    MIX_SetTrackGain(trackInfo->track, m_volume);
}

void Stream::pause()
{
    if (_mixer == nullptr)
        throw std::runtime_error("Mixer not initialized");
    if (m_audio == nullptr)
        throw std::runtime_error("Audio not loaded");

    if (m_trackIndex < 0 || m_trackIndex >= MAX_TRACKS)
        return;

    TrackInfo& trackInfo = _tracks[m_trackIndex];
    if (!trackInfo.track || trackInfo.usage != TrackUsage::Stream)
        return;

    if (!MIX_TrackPlaying(trackInfo.track))
        return;

    const Sint64 frames = MIX_GetTrackPlaybackPosition(trackInfo.track);
    if (frames >= 0)
        m_savedFrames = frames;

    // Requirement: paused streams should be removed from the track.
    if (!MIX_StopTrack(trackInfo.track, 0))
        throw std::runtime_error(std::string("Failed to stop track: ") + SDL_GetError());

    _clearTrackAssignment(trackInfo);
    m_trackIndex = -1;
}

void Stream::resume(const double fadeInSeconds)
{
    if (_mixer == nullptr)
        throw std::runtime_error("Mixer not initialized");
    if (m_audio == nullptr)
        throw std::runtime_error("Audio not loaded");

    if (isPlaying())
        return;

    TrackInfo* trackInfo = _acquireTrack(priority, TrackUsage::Stream, canSteal);
    if (trackInfo == nullptr)
        return;

    const int idx = _trackIndex(trackInfo);
    if (idx < 0)
        return;
    m_trackIndex = idx;

    if (MIX_TrackPlaying(trackInfo->track))
    {
        if (!MIX_StopTrack(trackInfo->track, 0))
            throw std::runtime_error(std::string("Failed to stop track: ") + SDL_GetError());
    }

    if (!MIX_SetTrackAudio(trackInfo->track, m_audio))
        throw std::runtime_error(std::string("Failed to set track audio: ") + SDL_GetError());

    const SDL_PropertiesID options = _buildPlayOptions(m_looping, fadeInSeconds);
    if (!MIX_PlayTrack(trackInfo->track, options))
    {
        if (options != 0)
            SDL_DestroyProperties(options);
        throw std::runtime_error(std::string("Failed to play track: ") + SDL_GetError());
    }
    if (options != 0)
        SDL_DestroyProperties(options);

    if (m_savedFrames > 0)
    {
        if (!MIX_SetTrackPlaybackPosition(trackInfo->track, m_savedFrames))
            throw std::runtime_error(std::string("Failed to seek track: ") + SDL_GetError());
    }

    trackInfo->audio = m_audio;
    trackInfo->priority = priority;
    trackInfo->started_seq = _playSeq++;
    trackInfo->usage = TrackUsage::Stream;

    MIX_SetTrackGain(trackInfo->track, m_volume);
}

void Stream::stop(const double fadeOutSeconds)
{
    if (_mixer == nullptr)
        throw std::runtime_error("Mixer not initialized");
    if (m_audio == nullptr)
        throw std::runtime_error("Audio not loaded");

    if (m_trackIndex < 0 || m_trackIndex >= MAX_TRACKS)
    {
        m_savedFrames = 0;
        return;
    }

    TrackInfo& trackInfo = _tracks[m_trackIndex];
    if (!trackInfo.track || trackInfo.usage != TrackUsage::Stream)
    {
        m_trackIndex = -1;
        m_savedFrames = 0;
        return;
    }

    const Sint64 fadeFrames = _fadeOutFramesForTrack(trackInfo.track, fadeOutSeconds);
    if (!MIX_StopTrack(trackInfo.track, fadeFrames))
        throw std::runtime_error(std::string("Failed to stop track: ") + SDL_GetError());

    _clearTrackAssignment(trackInfo);
    m_trackIndex = -1;
    m_savedFrames = 0;
}

void Stream::seek(const double seconds)
{
    if (_mixer == nullptr)
        throw std::runtime_error("Mixer not initialized");
    if (m_audio == nullptr)
        throw std::runtime_error("Audio not loaded");

    const Sint64 ms = _secondsToMs(seconds);

    if (m_trackIndex >= 0 && m_trackIndex < MAX_TRACKS)
    {
        TrackInfo& trackInfo = _tracks[m_trackIndex];
        if (trackInfo.track && trackInfo.usage == TrackUsage::Stream &&
            MIX_TrackPlaying(trackInfo.track))
        {
            const Sint64 frames = MIX_TrackMSToFrames(trackInfo.track, ms);
            if (frames < 0)
                throw std::runtime_error(
                    std::string("Failed to convert ms->frames: ") + SDL_GetError()
                );

            if (!MIX_SetTrackPlaybackPosition(trackInfo.track, frames))
                throw std::runtime_error(std::string("Failed to seek track: ") + SDL_GetError());

            m_savedFrames = frames;
            return;
        }
    }

    const Sint64 frames = MIX_AudioMSToFrames(m_audio, ms);
    if (frames < 0)
        throw std::runtime_error(std::string("Failed to convert ms->frames: ") + SDL_GetError());

    m_savedFrames = frames;
}

uint8_t _countPlayingInstances(MIX_Audio* audio)
{
    uint8_t count = 0;
    for (const auto& trackInfo : _tracks)
        if (trackInfo.track && trackInfo.audio == audio && MIX_TrackPlaying(trackInfo.track))
            ++count;
    return count;
}

int _trackIndex(const TrackInfo* trackInfo)
{
    if (!trackInfo)
        return -1;
    for (int i = 0; i < MAX_TRACKS; ++i)
        if (&_tracks[i] == trackInfo)
            return i;
    return -1;
}

void _clearTrackAssignment(TrackInfo& trackInfo)
{
    trackInfo.audio = nullptr;
    trackInfo.priority = AudioPriority::SFX;
    trackInfo.started_seq = 0;
    trackInfo.usage = TrackUsage::None;
}

TrackInfo* _acquireTrack(AudioPriority priority, TrackUsage usage, bool canSteal)
{
    TrackInfo* stealCandidate = nullptr;

    for (auto& trackInfo : _tracks)
    {
        if (!trackInfo.track)
            continue;

        // 1. Try finding a completely idle track.
        if (!MIX_TrackPlaying(trackInfo.track))
            return &trackInfo;

        if (!canSteal)
            continue;

        // 2. Streams never steal from other streams.
        if (usage == TrackUsage::Stream && trackInfo.usage == TrackUsage::Stream)
            continue;

        // 3. Compare priorities.
        // Steal if the target has LOWER priority,
        // or SAME priority if it's an older sample.
        if (trackInfo.priority < priority)
        {
            if (!stealCandidate || trackInfo.priority < stealCandidate->priority ||
                (trackInfo.priority == stealCandidate->priority &&
                 trackInfo.started_seq < stealCandidate->started_seq))
            {
                stealCandidate = &trackInfo;
            }
        }
        else if (trackInfo.priority == priority && trackInfo.usage != TrackUsage::Stream)
        {
            // Within same priority, steal the oldest one.
            if (!stealCandidate || trackInfo.started_seq < stealCandidate->started_seq)
            {
                stealCandidate = &trackInfo;
            }
        }
    }

    return stealCandidate;
}

Sint64 _secondsToMs(double seconds)
{
    if (seconds <= 0.0)
        return 0;

    const double ms = seconds * 1000.0;
    const auto maxMs = static_cast<double>(std::numeric_limits<Sint64>::max());
    if (ms >= maxMs)
        return std::numeric_limits<Sint64>::max();

    return static_cast<Sint64>(ms);
}

SDL_PropertiesID _buildPlayOptions(const bool looping, const double fadeInSeconds)
{
    const Sint64 fadeInMs = _secondsToMs(fadeInSeconds);
    const Sint64 loops = looping ? -1 : 0;

    if (fadeInMs <= 0 && loops == 0)
        return 0;

    const SDL_PropertiesID options = SDL_CreateProperties();
    if (options == 0)
        throw std::runtime_error(std::string("Failed to create SDL properties: ") + SDL_GetError());

    if (loops != 0)
        SDL_SetNumberProperty(options, MIX_PROP_PLAY_LOOPS_NUMBER, loops);
    if (fadeInMs > 0)
        SDL_SetNumberProperty(options, MIX_PROP_PLAY_FADE_IN_MILLISECONDS_NUMBER, fadeInMs);

    return options;
}

Sint64 _fadeOutFramesForTrack(MIX_Track* track, const double fadeOutSeconds)
{
    if (fadeOutSeconds <= 0.0)
        return 0;

    const Sint64 fadeOutMs = _secondsToMs(fadeOutSeconds);
    const Sint64 frames = MIX_TrackMSToFrames(track, fadeOutMs);
    if (frames < 0)
        return 0;

    return frames;
}

void _init()
{
    if (!MIX_Init())
    {
        throw std::runtime_error(std::string("Failed to initialize SDL_mixer: ") + SDL_GetError());
    }

    kn::log::info(
        "SDL_mixer version: {}.{}.{}", SDL_MIXER_MAJOR_VERSION, SDL_MIXER_MINOR_VERSION,
        SDL_MIXER_MICRO_VERSION
    );

    _mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
    if (_mixer == nullptr)
    {
        throw std::runtime_error(std::string("Failed to create mixer device: ") + SDL_GetError());
    }

    const SDL_PropertiesID mixerProps = MIX_GetMixerProperties(_mixer);
    const auto deviceNumber = SDL_GetNumberProperty(mixerProps, MIX_PROP_MIXER_DEVICE_NUMBER, 0);
    kn::log::info("Using mixer device number: {}", deviceNumber);

    for (uint8_t i = 0; i < MAX_TRACKS; ++i)
    {
        _tracks[i].track = MIX_CreateTrack(_mixer);
        if (_tracks[i].track == nullptr)
        {
            throw std::runtime_error(
                std::string("Failed to create mixer track ") + std::to_string(i) + ": " +
                SDL_GetError()
            );
        }
    }

    kn::log::info("Initialized mixer with {} tracks.", MAX_TRACKS);
}

void _quit()
{
    if (_mixer != nullptr)
    {
        MIX_DestroyMixer(_mixer);
        _mixer = nullptr;
    }
}

class PyAudio : public Audio, public py::trampoline_self_life_support
{
  public:
    using Audio::Audio;

    void setVolume(float volume) override
    {
        PYBIND11_OVERRIDE_PURE(void, Audio, setVolume, volume);
    }
    bool isPlaying() const override
    {
        PYBIND11_OVERRIDE_PURE(bool, Audio, isPlaying);
    }
    void play(double fadeInSeconds) override
    {
        PYBIND11_OVERRIDE_PURE(void, Audio, play, fadeInSeconds);
    }
    void stop(double fadeOutSeconds) override
    {
        PYBIND11_OVERRIDE_PURE(void, Audio, stop, fadeOutSeconds);
    }
};

void _bind(py::module_& module)
{
    auto subMixer = module.def_submodule("mixer", R"doc(
        Sound mixer and audio management system.

        The mixer manages a pool of 64 mixer tracks/voices for playing
        :class:`Sample` (short polyphonic sounds) and :class:`Stream`
        (long music files). It handles automatic track acquisition and
        priority-based sound stealing when the track pool is exhausted.
    )doc");

    py::native_enum<AudioPriority>(subMixer, "AudioPriority", "enum.IntEnum", R"doc(
        Priority levels used for track acquisition.

        Used to determine which sounds to interrupt ('steal') when the 64-track
        limit is reached. Higher priority sounds are more protected from being stolen.
    )doc")
        .value("MUSIC", AudioPriority::Music, "Highest priority level.")
        .value("UI", AudioPriority::UI, "Medium priority level.")
        .value("SFX", AudioPriority::SFX, "Standard priority level.")
        .finalize();

    py::classh<Audio, PyAudio>(subMixer, "Audio", R"doc(
        Abstract base class for all audio resources.

        Common interface for local volume and playback status. Local volume
        is multiplied by the mixer's master volume. Both default to 1.0.

        Attributes:
            volume (float): Local volume multiplier (0.0 to 1.0). Defaults to 1.0.
            playing (bool): (Read-only) Whether the audio is currently playing on any track.

        Methods:
            play(fade_in=0.0): Start audio playback.
            stop(fade_out=0.0): Stop all instances of this audio resource.
    )doc")
        .def_property("volume", &Audio::getVolume, &Audio::setVolume, "Volume scalar (0.0 to 1.0).")
        .def_property_readonly("playing", &Audio::isPlaying, "True if currently playing.")

        .def("play", &Audio::play, py::arg("fade_in") = 0.0, R"doc(
            Start audio playback.

            Args:
                fade_in (float): Fade in duration in seconds. Defaults to 0.0.
        )doc")
        .def("stop", &Audio::stop, py::arg("fade_out") = 0.0, R"doc(
            Stop all playing instances of this audio.

            Args:
                fade_out (float): Fade out duration in seconds. Defaults to 0.0.
        )doc");

    py::classh<Sample, Audio>(subMixer, "Sample", R"doc(
        A sound effect sample loaded entirely into memory.

        Samples support polyphony (multiple simultaneous instances). If tracks
        are full, samples attempt to steal tracks from lower-priority or older sounds.

        Attributes:
            priority (AudioPriority): Acquisition priority level. Defaults to SFX.
            can_steal (bool): Whether this sound can interrupt others to acquire a
                track. Defaults to True.
            max_polyphony (int): Maximum simultaneous instances of this specific
                sample (Range 1-32). Defaults to 1.
    )doc")
        .def_readwrite("priority", &Sample::priority, "Acquisition priority level.")
        .def_readwrite("can_steal", &Sample::canSteal, "Whether can interrupt others to acquire a track.")
        .def_property(
            "max_polyphony", &Sample::getMaxPolyphony, &Sample::setMaxPolyphony,
            "Max simultaneous instances of sample (1-32)."
        );

    py::classh<Stream, Audio>(subMixer, "Stream", R"doc(
        A streaming audio resource intended for long music files.

        Streams occupy exactly one track while active. They are protected and
        will not be stolen by incoming :class:`Sample` requests.

        Attributes:
            playback_pos (float): (Read-only) Current playback position in seconds.
            looping (bool): Whether the stream should loop when it reaches the end.

        Methods:
            pause(): Pause playback, preserving position.
            resume(fade_in=0.0): Resume playback from a paused state.
            seek(seconds): Jump to a specific time in the audio file.
    )doc")
        .def_property_readonly(
            "playback_pos", &Stream::getPlaybackPos,
            R"doc(Current position in seconds. 0.0 if stopped/never played, paused position if paused.)doc"
        )
        .def_property(
            "looping", &Stream::getLooping, &Stream::setLooping,
            R"doc(Whether the stream should loop when it reaches the end.)doc"
        )

        .def("pause", &Stream::pause, R"doc(
            Pause playback. Releases the hardware track but preserves position.
        )doc")
        .def("resume", &Stream::resume, py::arg("fade_in") = 0.0, R"doc(
            Resume playback from a paused state.

            Args:
                fade_in (float): Duration in seconds to fade back in. Defaults to 0.0.
        )doc")
        .def("seek", &Stream::seek, py::arg("seconds"), R"doc(
            Jump to a specific time in the audio file.

            Args:
                seconds (float): Target position in seconds from the start.
        )doc");

    subMixer.def("load_sample", &loadSample, py::arg("path"), py::arg("predecode") = true, R"doc(
        Load an audio sample (SFX) from disk.

        Args:
            path (str): File path to load.
            predecode (bool): Whether to decode into memory now. Defaults to True.

        Returns:
            Sample: The loaded audio object.
    )doc");

    subMixer.def("load_stream", &loadStream, py::arg("path"), py::arg("predecode") = false, R"doc(
        Load an audio stream (Music) from disk.

        Args:
            path (str): File path to load.
            predecode (bool): Whether to decode into memory now. Defaults to False.

        Returns:
            Stream: The loaded audio object.
    )doc");

    subMixer.def("set_master_volume", &setMasterVolume, py::arg("volume"), R"doc(
        Set the global mixer gain.

        This affects all playing samples and streams. Individual audio volume
        is multiplied by this value. Default is 1.0.

        Args:
            volume (float): Master volume scalar (0.0 to 1.0).
    )doc");

    subMixer.def("get_master_volume", &getMasterVolume, R"doc(
        Get the current global mixer gain.

        Returns:
            float: Master volume (0.0 to 1.0).
    )doc");
}
}  // namespace kn::mixer
