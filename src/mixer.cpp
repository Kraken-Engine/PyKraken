#define MINIAUDIO_IMPLEMENTATION

#include "Mixer.hpp"

#include <algorithm>
#include <stdexcept>

static ma_engine _engine;
constexpr ma_uint32 _flags =
    MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_NO_SPATIALIZATION | MA_SOUND_FLAG_NO_PITCH;

// Global list of Audio instances for automatic cleanup
static std::vector<Audio*> _audioInstances;
static std::mutex _instancesMutex;

namespace mixer
{
void _bind(pybind11::module_& module)
{
    py::class_<Audio>(module, "Audio")
        .def(py::init<const std::string&, float>(), py::arg("path"), py::arg("volume") = 1.0f,
             "Create an Audio object from a file path with optional volume")

        .def("play", &Audio::play, py::arg("fadeInMs") = 0, py::arg("loop") = false,
             "Play the audio with optional fade-in time and loop setting.")
        .def("stop", &Audio::stop, py::arg("fadeOutMs") = 0, "Stop sound playback")

        .def_property("volume", &Audio::getVolume, &Audio::setVolume);
}

void _init()
{
    if (ma_engine_init(nullptr, &_engine) != MA_SUCCESS)
        throw std::runtime_error("Audio engine init failed");

    ma_engine_listener_set_enabled(&_engine, 0, MA_FALSE);
}

void _quit() { ma_engine_uninit(&_engine); }

void _tick()
{
    // Clean up all audio instances automatically
    std::lock_guard<std::mutex> g(_instancesMutex);
    for (Audio* audio : _audioInstances)
    {
        audio->cleanup();
    }
}
} // namespace mixer

Audio::Audio(const std::string& path, const float volume) : m_path(path), m_volume(volume)
{
    if (ma_sound_init_from_file(&_engine, m_path.c_str(), _flags, nullptr, nullptr, &m_proto) !=
        MA_SUCCESS)
    {
        throw std::runtime_error("ma_sound_init_from_file(DECODE) failed: " + m_path);
    }

    // Register this instance for automatic cleanup
    std::lock_guard<std::mutex> g(_instancesMutex);
    _audioInstances.push_back(this);
}

Audio::~Audio()
{
    stop();
    {
        std::lock_guard<std::mutex> g(m_mutex);
        for (auto& v : m_voices)
        {
            ma_sound_uninit(&v->snd);
        }
        m_voices.clear();
    }
    ma_sound_uninit(&m_proto);

    // Unregister this instance
    std::lock_guard<std::mutex> g(_instancesMutex);
    _audioInstances.erase(std::remove(_audioInstances.begin(), _audioInstances.end(), this),
                          _audioInstances.end());
}

void Audio::play(int fadeInMs, bool loop)
{
    auto v = std::make_unique<Voice>();

    if (ma_sound_init_copy(&_engine, &m_proto, _flags, nullptr, &v->snd) != MA_SUCCESS)
        throw std::runtime_error("ma_sound_init_copy() failed");

    v->volume = m_volume;

    ma_sound_set_looping(&v->snd, loop ? MA_TRUE : MA_FALSE);
    ma_sound_set_volume(&v->snd, m_volume);

    if (fadeInMs > 0)
    {
        ma_uint64 frames = msToFrames(fadeInMs);
        ma_sound_set_fade_in_pcm_frames(&v->snd, 0.0f, m_volume, frames);
    }

    // Mark for GC on end.
    ma_sound_set_end_callback(
        &v->snd, [](void* user, ma_sound*)
        { reinterpret_cast<Voice*>(user)->done.store(true, std::memory_order_relaxed); }, v.get());

    ma_sound_start(&v->snd);

    std::lock_guard<std::mutex> g(m_mutex);
    m_voices.push_back(std::move(v));
}

void Audio::stop(int fadeOutMs)
{
    std::lock_guard<std::mutex> g(m_mutex);
    for (auto& v : m_voices)
    {
        doFadeStop(v->snd, v->volume, fadeOutMs);
    }
}

void Audio::setVolume(float volume)
{
    m_volume = volume;
    std::lock_guard<std::mutex> g(m_mutex);
    for (auto& v : m_voices)
    {
        v->volume = volume;
        ma_sound_set_volume(&v->snd, volume);
    }
}

float Audio::getVolume() const { return m_volume; }

void Audio::cleanup()
{
    std::lock_guard<std::mutex> g(m_mutex);
    m_voices.erase(std::remove_if(m_voices.begin(), m_voices.end(),
                                  [&](const std::unique_ptr<Voice>& v)
                                  {
                                      if (v->done.load(std::memory_order_relaxed) &&
                                          !ma_sound_is_playing(&v->snd))
                                      {
                                          ma_sound_uninit(&v->snd);
                                          return true;
                                      }
                                      return false;
                                  }),
                   m_voices.end());
}

ma_uint64 Audio::msToFrames(int ms) const
{
    return (ma_uint64)((ma_engine_get_sample_rate(&_engine) * (ma_uint64)ms) / 1000);
}

void Audio::doFadeStop(ma_sound& snd, float currentVol, int fadeOutMs)
{
    if (fadeOutMs <= 0)
    {
        ma_sound_stop(&snd);
        return;
    }

    ma_uint64 fadeFrames = msToFrames(fadeOutMs);
    ma_sound_set_fade_in_pcm_frames(&snd, currentVol, 0.0f, fadeFrames);

    // Schedule the stop for exactly when the fade hits 0.
    ma_uint64 now = ma_engine_get_time_in_pcm_frames(&_engine);
    ma_sound_set_stop_time_in_pcm_frames(&snd, now + fadeFrames);
}