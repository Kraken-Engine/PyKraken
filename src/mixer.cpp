#define MINIAUDIO_IMPLEMENTATION

#include "Mixer.hpp"

#include <algorithm>
#include <atomic>
#include <stdexcept>

#include "Log.hpp"

const char* getBackendName(ma_backend backend);

namespace kn
{
static ma_engine _engine;
static std::vector<std::weak_ptr<Audio>> _audioInstances;
static std::mutex _instancesMutex;
static std::vector<AudioStream*> _audioStreamInstances;
static std::mutex _streamsMutex;
static std::atomic<bool> _engineShutdown{false};

static bool hasSupportedExtension(const std::string& path)
{
    auto extPos = path.find_last_of('.');
    if (extPos == std::string::npos)
        return false;

    std::string ext = path.substr(extPos + 1);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    return ext == "wav" || ext == "mp3" || ext == "flac";
}

Audio::Audio(const std::string& path, const float volume) : m_path(path), m_volume(volume)
{
    if (!hasSupportedExtension(path))
        throw std::invalid_argument("Unsupported audio format: " + path);

    if (ma_sound_init_from_file(&_engine, m_path.c_str(), m_flags, nullptr, nullptr, &m_proto) !=
        MA_SUCCESS)
        throw std::runtime_error("Failed to decode: " + path);
}

Audio::~Audio()
{
    // Only clean up if engine hasn't been shut down
    if (!_engineShutdown.load(std::memory_order_acquire))
    {
        // Only clean up if engine still exists (not already cleaned by _quit)
        if (m_proto.engineNode.pEngine != nullptr)
        {
            stop();
            {
                std::lock_guard g(m_mutex);
                for (const auto& v : m_voices)
                {
                    ma_sound_set_end_callback(&v->snd, nullptr, nullptr);
                    ma_sound_uninit(&v->snd);
                }
                m_voices.clear();
            }
            ma_sound_uninit(&m_proto);
        }

        // Unregister this instance
        std::lock_guard g(_instancesMutex);
        std::erase_if(_audioInstances,
                      [&](auto& w) { return w.expired() || w.lock().get() == this; });
    }
}

void Audio::play(const int fadeInMs, const bool loop)
{
    auto v = std::make_unique<Voice>();

    if (ma_sound_init_copy(&_engine, &m_proto, m_flags, nullptr, &v->snd) != MA_SUCCESS)
        throw std::runtime_error("ma_sound_init_copy() failed");

    ma_sound_set_looping(&v->snd, loop ? MA_TRUE : MA_FALSE);
    ma_sound_set_volume(&v->snd, m_volume);

    if (fadeInMs > 0)
    {
        const ma_uint64 frames = msToFrames(fadeInMs);
        ma_sound_set_fade_in_pcm_frames(&v->snd, 0.0f, m_volume, frames);
    }

    // Mark for GC on end.
    ma_sound_set_end_callback(
        &v->snd, [](void* user, ma_sound*)
        { static_cast<Voice*>(user)->done.store(true, std::memory_order_relaxed); }, v.get());

    ma_sound_start(&v->snd);

    std::lock_guard g(m_mutex);
    m_voices.push_back(std::move(v));
}

void Audio::stop(const int fadeOutMs)
{
    std::lock_guard g(m_mutex);
    for (const auto& v : m_voices)
        doFadeStop(v->snd, m_volume, fadeOutMs);
}

void Audio::setVolume(const float volume)
{
    m_volume = volume;
    std::lock_guard g(m_mutex);
    for (const auto& v : m_voices)
        ma_sound_set_volume(&v->snd, volume);
}

float Audio::getVolume() const
{
    return m_volume;
}

void Audio::cleanup()
{
    std::lock_guard g(m_mutex);
    std::erase_if(m_voices,
                  [&](const std::unique_ptr<Voice>& v)
                  {
                      if (v->done.load(std::memory_order_relaxed) && !ma_sound_is_playing(&v->snd))
                      {
                          ma_sound_set_end_callback(&v->snd, nullptr, nullptr);
                          ma_sound_uninit(&v->snd);
                          return true;
                      }
                      return false;
                  });
}

ma_uint64 Audio::msToFrames(const int ms)
{
    return ma_engine_get_sample_rate(&_engine) * static_cast<ma_uint64>(ms) / 1000;
}

void Audio::doFadeStop(ma_sound& snd, const float currentVol, const int fadeOutMs)
{
    if (fadeOutMs <= 0)
    {
        ma_sound_stop(&snd);
        return;
    }

    const ma_uint64 fadeFrames = msToFrames(fadeOutMs);
    ma_sound_set_fade_in_pcm_frames(&snd, currentVol, 0.0f, fadeFrames);

    // Schedule the stop for exactly when the fade hits 0.
    const ma_uint64 now = ma_engine_get_time_in_pcm_frames(&_engine);
    ma_sound_set_stop_time_in_pcm_frames(&snd, now + fadeFrames);
}

AudioStream::AudioStream(const std::string& filePath, const float volume) : m_volume(volume)
{
    if (!hasSupportedExtension(filePath))
        throw std::invalid_argument("Unsupported audio format: " + filePath);

    if (ma_sound_init_from_file(&_engine, filePath.c_str(), m_flags, nullptr, nullptr, &m_snd) !=
        MA_SUCCESS)
        throw std::runtime_error("ma_sound_init_from_file(STREAM) failed: " + filePath);

    ma_sound_set_volume(&m_snd, volume);

    // Register this stream for cleanup
    std::lock_guard g(_streamsMutex);
    _audioStreamInstances.push_back(this);
}

AudioStream::~AudioStream()
{
    // Only clean up if engine hasn't been shut down
    if (!_engineShutdown.load(std::memory_order_acquire))
    {
        // Unregister this stream
        {
            std::lock_guard g(_streamsMutex);
            auto it = std::find(_audioStreamInstances.begin(), _audioStreamInstances.end(), this);
            if (it != _audioStreamInstances.end())
            {
                _audioStreamInstances.erase(it);
            }
        }

        // Only uninit if engine still exists (not already cleaned by _quit)
        if (m_snd.engineNode.pEngine != nullptr)
        {
            ma_sound_uninit(&m_snd);
        }
    }
}

void AudioStream::play(const int fadeInMs, const bool loop, const float startTimeSeconds)
{
    startTimeSeconds > 0.0f ? seek(startTimeSeconds) : rewind();

    ma_sound_set_looping(&m_snd, loop ? MA_TRUE : MA_FALSE);
    if (fadeInMs > 0)
    {
        const ma_uint64 frames = msToFrames(fadeInMs);
        ma_sound_set_fade_in_pcm_frames(&m_snd, 0.0f, m_volume, frames);
    }
    else
    {
        ma_sound_set_volume(&m_snd, m_volume);
    }
    ma_sound_start(&m_snd);
}

void AudioStream::stop(const int fadeOutMs)
{
    if (fadeOutMs <= 0)
    {
        ma_sound_stop(&m_snd);
        return;
    }

    const float cur = ma_sound_get_volume(&m_snd);
    const ma_uint64 frames = msToFrames(fadeOutMs);
    ma_sound_set_fade_in_pcm_frames(&m_snd, cur, 0.0f, frames);
    const ma_uint64 now = ma_engine_get_time_in_pcm_frames(&_engine);
    ma_sound_set_stop_time_in_pcm_frames(&m_snd, now + frames);
}

void AudioStream::pause()
{
    ma_sound_stop(&m_snd);
}

void AudioStream::resume()
{
    ma_sound_start(&m_snd);
}

void AudioStream::rewind()
{
    ma_sound_seek_to_pcm_frame(&m_snd, 0);
}

void AudioStream::seek(float timeSeconds)
{
    // Clamp to positive values only to prevent bugs
    if (timeSeconds < 0.0f)
        timeSeconds = 0.0f;

    const ma_uint32 sampleRate = ma_engine_get_sample_rate(&_engine);
    const ma_uint64 framePosition = static_cast<ma_uint64>(timeSeconds * sampleRate);
    ma_sound_seek_to_pcm_frame(&m_snd, framePosition);
}

float AudioStream::getCurrentTime()
{
    ma_uint64 currentFrame = ma_sound_get_time_in_pcm_frames(&m_snd);
    const ma_uint32 sampleRate = ma_engine_get_sample_rate(&_engine);

    // Get the total length to handle looping properly
    ma_uint64 lengthInFrames = 0;
    ma_result result = ma_sound_get_length_in_pcm_frames(&m_snd, &lengthInFrames);
    if (result == MA_SUCCESS && lengthInFrames > 0)
        currentFrame %= lengthInFrames;

    return static_cast<float>(currentFrame) / static_cast<float>(sampleRate);
}

void AudioStream::setVolume(const float volume)
{
    m_volume = volume;
    ma_sound_set_volume(&m_snd, volume);
}

float AudioStream::getVolume() const
{
    return m_volume;
}

void AudioStream::setLooping(const bool loop)
{
    ma_sound_set_looping(&m_snd, loop ? MA_TRUE : MA_FALSE);
}

ma_uint64 AudioStream::msToFrames(const int ms)
{
    return ma_engine_get_sample_rate(&_engine) * static_cast<ma_uint64>(ms) / 1000;
}

namespace mixer
{
void _init()
{
    if (ma_engine_init(nullptr, &_engine) != MA_SUCCESS)
        throw std::runtime_error("Audio engine init failed");

    ma_engine_listener_set_enabled(&_engine, 0, MA_FALSE);

    kn::log::info("Miniaudio version: {}", MA_VERSION_STRING);
    kn::log::info("Audio device: {}", _engine.pDevice->playback.name);
    kn::log::info("Audio sample rate: {} Hz", _engine.pDevice->sampleRate);
    kn::log::info("Audio channels: {}", _engine.pDevice->playback.channels);
}

void _quit()
{
    // Mark as shutting down first to prevent _tick() from running
    _engineShutdown.store(true, std::memory_order_release);

    // Stop the engine to halt all audio processing threads
    // This prevents any race conditions with audio callbacks
    ma_engine_stop(&_engine);

    // Clean up all AudioStream instances
    {
        std::lock_guard g(_streamsMutex);
        for (AudioStream* stream : _audioStreamInstances)
        {
            if (stream->m_snd.engineNode.pEngine != nullptr)
            {
                ma_sound_uninit(&stream->m_snd);
                stream->m_snd.engineNode.pEngine = nullptr;
            }
        }
        _audioStreamInstances.clear();
    }

    // Clean up all Audio instances
    // Collect the audio instances first, then release the mutex before cleanup
    std::vector<std::shared_ptr<Audio>> audioToClean;
    {
        std::lock_guard g(_instancesMutex);
        for (auto& weak : _audioInstances)
        {
            if (auto audio = weak.lock())
            {
                audioToClean.push_back(audio);
            }
        }
        _audioInstances.clear();
    }

    // Now clean up audio instances without holding _instancesMutex (avoid deadlock)
    for (auto& audio : audioToClean)
    {
        // Stop all sounds (this will acquire audio->m_mutex internally)
        audio->stop(0);

        // Clean up voices
        {
            std::lock_guard audioLock(audio->m_mutex);
            for (const auto& v : audio->m_voices)
            {
                ma_sound_set_end_callback(&v->snd, nullptr, nullptr);
                ma_sound_uninit(&v->snd);
            }
            audio->m_voices.clear();
        }

        // Clean up proto sound
        if (audio->m_proto.engineNode.pEngine != nullptr)
        {
            ma_sound_uninit(&audio->m_proto);
            audio->m_proto.engineNode.pEngine = nullptr;
        }
    }

    // Finally uninit the engine
    ma_engine_uninit(&_engine);
}

void _tick()
{
    // Don't tick if engine is shutting down or has shut down
    if (_engineShutdown.load(std::memory_order_acquire))
        return;

    std::vector<std::shared_ptr<Audio>> work;
    {
        std::lock_guard lk(_instancesMutex);
        // collect live instances and compact away expired ones
        work.reserve(_audioInstances.size());
        auto it = _audioInstances.begin();
        while (it != _audioInstances.end())
        {
            if (auto sp = it->lock())
            {
                work.push_back(std::move(sp));
                ++it;
            }
            else
            {
                it = _audioInstances.erase(it);  // drop expired
            }
        }

        if (_audioInstances.capacity() > _audioInstances.size() * 2)
            _audioInstances.shrink_to_fit();
    }
    for (const auto& a : work)
        a->cleanup();  // no global lock held; lifetime pinned
}

void _bind(const py::module_& module)
{
    // ------------ Audio -------------
    py::classh<Audio>(module, "Audio", R"doc(
A decoded audio object that supports multiple simultaneous playbacks.

Audio objects decode the entire file into memory for low-latency playback. They support
multiple concurrent playbacks of the same sound. Use this for short sound effects that may need to overlap.
    )doc")
        .def(py::init(
                 [](const std::string& path, float volume) -> std::shared_ptr<Audio>
                 {
                     auto sp = std::make_shared<Audio>(path, volume);
                     {
                         std::lock_guard g(_instancesMutex);
                         _audioInstances.push_back(sp);
                     }
                     return sp;
                 }),
             py::arg("file_path"), py::arg("volume") = 1.0f,
             R"doc(
Create an Audio object from a file path with optional volume.

Args:
    file_path (str): Path to the audio file to load.
    volume (float, optional): Initial volume level (0.0 to 1.0+). Defaults to 1.0.

Raises:
    RuntimeError: If the audio file cannot be loaded or decoded.
        )doc")

        .def_property("volume", &Audio::getVolume, &Audio::setVolume, R"doc(
The volume level for new and existing playbacks.

Setting this property affects all currently playing voices and sets the default
volume for future playbacks. Volume can exceed 1.0 for amplification.

Type:
    float: Volume level (0.0 = silent, 1.0 = original volume, >1.0 = amplified).
        )doc")

        .def("play", &Audio::play, py::arg("fade_in_ms") = 0, py::arg("loop") = false,
             R"doc(
Play the audio with optional fade-in time and loop setting.

Creates a new voice for playback, allowing multiple simultaneous plays of the same audio.
Each play instance is independent and can have different fade and loop settings.

Args:
    fade_in_ms (int, optional): Fade-in duration in milliseconds. Defaults to 0.
    loop (bool, optional): Whether to loop the audio continuously. Defaults to False.

Raises:
    RuntimeError: If audio playback initialization fails.
        )doc")
        .def("stop", &Audio::stop, py::arg("fade_out_ms") = 0, R"doc(
Stop all active playbacks of this audio.

Stops all currently playing voices associated with this Audio object. If a fade-out
time is specified, all voices will fade out over that duration before stopping.

Args:
    fade_out_ms (int, optional): Fade-out duration in milliseconds. Defaults to 0.
        )doc");

    // ------------ AudioStream -------------

    py::classh<AudioStream>(module, "AudioStream", R"doc(
A streaming audio object for single-instance playback of large audio files.

AudioStream objects stream audio data from disk during playback, using minimal memory.
They support only one playback instance at a time, making them ideal for background
music, long audio tracks, or when memory usage is a concern.
    )doc")
        .def(py::init<const std::string&, float>(), py::arg("file_path"), py::arg("volume") = 1.0f,
             R"doc(
Create an AudioStream object from a file path with optional volume.

Args:
    file_path (str): Path to the audio file to stream.
    volume (float, optional): Initial volume level (0.0 to 1.0+). Defaults to 1.0.

Raises:
    RuntimeError: If the audio file cannot be opened for streaming.
        )doc")

        .def_property("volume", &AudioStream::getVolume, &AudioStream::setVolume, R"doc(
The volume level of the audio stream.

Volume can exceed 1.0 for amplification.

Type:
    float: Volume level (0.0 = silent, 1.0 = original volume, >1.0 = amplified).
    )doc")
        .def_property_readonly("current_time", &AudioStream::getCurrentTime, R"doc(
The current playback time position in seconds.
    )doc")

        .def("play", &AudioStream::play, py::arg("fade_in_ms") = 0, py::arg("loop") = false,
             py::arg("start_time_seconds") = 0.0f,
             R"doc(
Play the audio stream with optional fade-in time, loop setting, and start position.

Starts playback from the specified time position. If the stream is already
playing, it will restart from the specified position.

Args:
    fade_in_ms (int, optional): Fade-in duration in milliseconds. Defaults to 0.
    loop (bool, optional): Whether to loop the audio continuously. Defaults to False.
    start_time_seconds (float, optional): Time position in seconds to start playback from. Defaults to 0.0.
        )doc")
        .def("stop", &AudioStream::stop, py::arg("fade_out_ms") = 0, R"doc(
Stop the audio stream playback.

Args:
    fade_out_ms (int, optional): Fade-out duration in milliseconds. If 0, stops immediately.
                              If > 0, fades out over the specified duration. Defaults to 0.
        )doc")
        .def("pause", &AudioStream::pause, R"doc(
Pause the audio stream playback.

The stream position is preserved and can be resumed with resume().
        )doc")
        .def("resume", &AudioStream::resume, R"doc(
Resume paused audio stream playback.

Continues playback from the current stream position.
        )doc")
        .def("rewind", &AudioStream::rewind, R"doc(
Rewind the audio stream to the beginning.

Sets the playback position back to the start of the audio file. Does not affect
the current play state (playing/paused).
        )doc")
        .def("seek", &AudioStream::seek, py::arg("time_seconds"), R"doc(
Seek to a specific time position in the audio stream.

Sets the playback position to the specified time in seconds. Does not affect
the current play state (playing/paused).

Args:
    time_seconds (float): The time position in seconds to seek to.
        )doc")
        .def("set_looping", &AudioStream::setLooping, py::arg("loop"), R"doc(
Set whether the audio stream loops continuously.

Args:
    loop (bool): True to enable looping, False to disable.
        )doc");
}
}  // namespace mixer
}  // namespace kn

const char* getBackendName(ma_backend backend)
{
    switch (backend)
    {
        case ma_backend_wasapi:
            return "WASAPI";
        case ma_backend_dsound:
            return "DirectSound";
        case ma_backend_winmm:
            return "WinMM";
        case ma_backend_coreaudio:
            return "CoreAudio";
        case ma_backend_alsa:
            return "ALSA";
        case ma_backend_pulseaudio:
            return "PulseAudio";
        case ma_backend_jack:
            return "JACK";
        case ma_backend_oss:
            return "OSS";
        case ma_backend_aaudio:
            return "AAudio";
        case ma_backend_opensl:
            return "OpenSL ES";
        case ma_backend_webaudio:
            return "WebAudio";
        case ma_backend_null:
            return "Null";
        default:
            return "Unknown";
    }
}
