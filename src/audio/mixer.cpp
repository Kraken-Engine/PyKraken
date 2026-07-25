#include "kraken/audio/Mixer.hpp"

#include <algorithm>
#include <limits>
#include <stdexcept>

#include "kraken/core/Log.hpp"

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

Sample loadSample(const std::filesystem::path& path, const bool predecode)
{
    MIX_Audio* audio = MIX_LoadAudio(_mixer, path.string().c_str(), predecode);
    if (!audio)
    {
        throw std::runtime_error(
            std::string("Failed to load sample '") + path.string() + "': " + SDL_GetError()
        );
    }

    return Sample(audio);
}

Stream loadStream(const std::filesystem::path& path, const bool predecode)
{
    MIX_Audio* audio = MIX_LoadAudio(_mixer, path.string().c_str(), predecode);
    if (!audio)
    {
        throw std::runtime_error(
            std::string("Failed to load stream '") + path.string() + "': " + SDL_GetError()
        );
    }

    return Stream(audio);
}

void setMasterVolume(float volume)
{
    if (!_mixer)
        throw std::runtime_error("Mixer not initialized");

    volume = std::clamp(volume, 0.0f, 1.0f);
    MIX_SetMixerGain(_mixer, volume);
}

float getMasterVolume()
{
    if (!_mixer)
        throw std::runtime_error("Mixer not initialized");

    return MIX_GetMixerGain(_mixer);
}

Audio::Audio(MIX_Audio* sdlAudio)
    : m_audio(sdlAudio)
{
}

Audio::Audio(Audio&& other) noexcept
    : priority(other.priority),
      canSteal(other.canSteal),
      m_audio(other.m_audio),
      m_looping(other.m_looping),
      m_volume(other.m_volume)
{
    // Steal pointer, nullify original
    other.m_audio = nullptr;
}

Audio& Audio::operator=(Audio&& other) noexcept
{
    if (this != &other)
    {
        // Clean up resource first
        if (m_audio)
            MIX_DestroyAudio(m_audio);

        priority = other.priority;
        canSteal = other.canSteal;
        m_audio = other.m_audio;
        m_looping = other.m_looping;
        m_volume = other.m_volume;

        // Nullify original
        other.m_audio = nullptr;
    }

    return *this;
}

Audio::~Audio()
{
    if (m_audio)
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

Sample::Sample(Sample&& other) noexcept
    : Audio(std::move(other)),  // Explicitly move the base class portion
      m_maxPolyphony(other.m_maxPolyphony)
{
}

Sample& Sample::operator=(Sample&& other) noexcept
{
    if (this != &other)
    {
        Audio::operator=(std::move(other));  // Move base class portion
        m_maxPolyphony = other.m_maxPolyphony;
    }

    return *this;
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

        if (!MIX_SetTrackGain(trackInfo.track, m_volume))
            throw std::runtime_error(std::string("Failed to set track gain: ") + SDL_GetError());
    }
}

bool Sample::isPlaying() const
{
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
    if (_countPlayingInstances(m_audio) >= m_maxPolyphony)
        return;

    TrackInfo* trackInfo = _acquireTrack(priority, TrackUsage::Sample, canSteal);
    if (!trackInfo)
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
    {
        if (!MIX_SetTrackGain(trackInfo->track, m_volume))
            throw std::runtime_error(std::string("Failed to set track gain: ") + SDL_GetError());
    }
}

void Sample::stop(const double fadeOutSeconds)
{
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

Stream::Stream(Stream&& other) noexcept
    : Audio(std::move(other)),  // Explicitly move the base class portion
      m_trackIndex(other.m_trackIndex),
      m_savedFrames(other.m_savedFrames)
{
    // Disconnect temporary from the hardware track
    // so its destructor doesn't stop the music
    other.m_trackIndex = -1;
    other.m_savedFrames = 0;
}

Stream& Stream::operator=(Stream&& other) noexcept
{
    if (this != &other)
    {
        // Clean up existing track assignment
        if (_mixer && m_trackIndex >= 0 && m_trackIndex < MAX_TRACKS)
        {
            TrackInfo& trackInfo = _tracks[m_trackIndex];
            if (trackInfo.track)
            {
                if (!MIX_StopTrack(trackInfo.track, 0))
                    kn::log::error("Failed to stop track: {}", SDL_GetError());
                _clearTrackAssignment(trackInfo);
            }
        }

        Audio::operator=(std::move(other));  // Move base class portion

        m_trackIndex = other.m_trackIndex;
        m_savedFrames = other.m_savedFrames;

        // Disconnect temporary
        other.m_trackIndex = -1;
        other.m_savedFrames = 0;
    }

    return *this;
}

Stream::~Stream()
{
    if (_mixer && m_trackIndex >= 0 && m_trackIndex < MAX_TRACKS)
    {
        TrackInfo& trackInfo = _tracks[m_trackIndex];
        if (trackInfo.track)
        {
            if (!MIX_StopTrack(trackInfo.track, 0))
                kn::log::error("Failed to stop track: {}", SDL_GetError());
            _clearTrackAssignment(trackInfo);
        }
    }
    m_trackIndex = -1;

    // m_audio is destroyed by Audio base class destructor
}

void Stream::setLooping(const bool looping)
{
    m_looping = looping;

    // Try updating currently playing instance too
    if (m_trackIndex < 0 || m_trackIndex >= MAX_TRACKS)
        return;

    TrackInfo& trackInfo = _tracks[m_trackIndex];
    if (!trackInfo.track)
        return;
    if (trackInfo.usage != TrackUsage::Stream)
        return;

    const int loops = m_looping ? -1 : 0;
    if (!MIX_SetTrackLoops(trackInfo.track, loops))
        throw std::runtime_error(std::string("Failed to set track loops: ") + SDL_GetError());
}

bool Stream::getLooping() const
{
    return m_looping;
}

void Stream::setVolume(float volume)
{
    m_volume = std::clamp(volume, 0.0f, 1.0f);

    if (m_trackIndex < 0 || m_trackIndex >= MAX_TRACKS)
        return;

    TrackInfo& trackInfo = _tracks[m_trackIndex];
    if (!trackInfo.track)
        return;
    if (trackInfo.usage != TrackUsage::Stream)
        return;
    if (!MIX_TrackPlaying(trackInfo.track))
        return;

    if (!MIX_SetTrackGain(trackInfo.track, m_volume))
        throw std::runtime_error(std::string("Failed to set track gain: ") + SDL_GetError());
}

bool Stream::isPlaying() const
{
    if (m_trackIndex < 0 || m_trackIndex >= MAX_TRACKS)
        return false;

    const TrackInfo& trackInfo = _tracks[m_trackIndex];
    if (!trackInfo.track || trackInfo.usage != TrackUsage::Stream)
        return false;

    return MIX_TrackPlaying(trackInfo.track);
}

double Stream::getPlaybackPos() const
{
    if (m_trackIndex >= 0 && m_trackIndex < MAX_TRACKS)
    {
        const TrackInfo& trackInfo = _tracks[m_trackIndex];
        const bool isPlaying = MIX_TrackPlaying(trackInfo.track);

        if (trackInfo.track && trackInfo.usage == TrackUsage::Stream && isPlaying)
        {
            const Sint64 frames = MIX_GetTrackPlaybackPosition(trackInfo.track);
            if (frames == -1)
                throw std::runtime_error(
                    std::string("Failed to get track playback position: ") + SDL_GetError()
                );

            const Sint64 ms = MIX_TrackFramesToMS(trackInfo.track, frames);
            if (ms == -1)
                throw std::runtime_error(
                    std::string("Failed to convert track frames to ms: ") + SDL_GetError()
                );

            return static_cast<double>(ms) / 1000.0;
        }
    }

    const Sint64 ms = MIX_AudioFramesToMS(m_audio, m_savedFrames);
    if (ms == -1)
        throw std::runtime_error(
            std::string("Failed to convert audio frames to ms: ") + SDL_GetError()
        );

    return static_cast<double>(ms) / 1000.0;
}

void Stream::play(const double fadeInSeconds)
{
    stop(0.0);
    m_savedFrames = 0;

    TrackInfo* trackInfo = _acquireTrack(priority, TrackUsage::Stream, canSteal);
    if (!trackInfo)
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

    if (!MIX_SetTrackGain(trackInfo->track, m_volume))
        throw std::runtime_error(std::string("Failed to set track gain: ") + SDL_GetError());
}

void Stream::pause()
{
    if (m_trackIndex < 0 || m_trackIndex >= MAX_TRACKS)
        return;

    TrackInfo& trackInfo = _tracks[m_trackIndex];
    if (!trackInfo.track || trackInfo.usage != TrackUsage::Stream)
        return;

    if (!MIX_TrackPlaying(trackInfo.track))
        return;

    const Sint64 frames = MIX_GetTrackPlaybackPosition(trackInfo.track);
    if (frames == -1)
        throw std::runtime_error(
            std::string("Failed to get track playback position: ") + SDL_GetError()
        );

    m_savedFrames = frames;

    // Requirement: paused streams should be removed from the track.
    if (!MIX_StopTrack(trackInfo.track, 0))
        throw std::runtime_error(std::string("Failed to stop track: ") + SDL_GetError());

    _clearTrackAssignment(trackInfo);
    m_trackIndex = -1;
}

void Stream::resume(const double fadeInSeconds)
{
    if (isPlaying())
        return;

    TrackInfo* trackInfo = _acquireTrack(priority, TrackUsage::Stream, canSteal);
    if (!trackInfo)
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

    if (!MIX_SetTrackGain(trackInfo->track, m_volume))
        throw std::runtime_error(std::string("Failed to set track gain: ") + SDL_GetError());
}

void Stream::stop(const double fadeOutSeconds)
{
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
        kn::log::warn("Failed to initialize SDL_mixer: {}. Audio disabled.", SDL_GetError());
        return;
    }

    kn::log::info(
        "SDL_mixer version: {}.{}.{}", SDL_MIXER_MAJOR_VERSION, SDL_MIXER_MINOR_VERSION,
        SDL_MIXER_MICRO_VERSION
    );

    _mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
    if (!_mixer)
    {
        kn::log::warn("Failed to create mixer device: {}. Audio disabled.", SDL_GetError());
        return;
    }

    const SDL_PropertiesID mixerProps = MIX_GetMixerProperties(_mixer);
    const auto deviceNumber = SDL_GetNumberProperty(mixerProps, MIX_PROP_MIXER_DEVICE_NUMBER, 0);
    kn::log::info("Using mixer device number: {}", deviceNumber);

    for (uint8_t i = 0; i < MAX_TRACKS; ++i)
    {
        _tracks[i].track = MIX_CreateTrack(_mixer);
        if (!_tracks[i].track)
        {
            kn::log::warn(
                "Failed to create mixer track {}: {}. Continuing with fewer tracks.",
                std::to_string(i), SDL_GetError()
            );
            _tracks[i].track = nullptr;
            continue;
        }
    }

    int createdTracks = 0;
    for (const auto& t : _tracks)
        if (t.track)
            ++createdTracks;

    kn::log::info("Initialized mixer with {} tracks.", createdTracks);
}

void _quit()
{
    if (_mixer)
    {
        MIX_DestroyMixer(_mixer);
        _mixer = nullptr;
    }
}

}  // namespace kn::mixer
