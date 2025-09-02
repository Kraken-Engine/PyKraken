#pragma once

#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include <miniaudio/miniaudio.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace kn
{
namespace mixer
{
void _bind(const py::module_& module);

void _init();

void _quit();

void _tick(); // Internal cleanup function, not exposed to Python
} // namespace mixer

class Audio
{
public:
    explicit Audio(const std::string& path, float volume = 1.f);
    ~Audio();

    void play(int fadeInMs = 0, bool loop = false);

    void stop(int fadeOutMs = 0);

    void setVolume(float volume);

    [[nodiscard]] float getVolume() const;

private:
    static constexpr ma_uint32 m_flags =
        MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_NO_SPATIALIZATION | MA_SOUND_FLAG_NO_PITCH;

    struct Voice
    {
        ma_sound snd;
        std::atomic<bool> done{false};
    };

    std::string m_path;
    ma_sound m_proto{};
    std::mutex m_mutex;
    std::vector<std::unique_ptr<Voice>> m_voices;
    int m_nextId = 0;
    float m_volume;

    static ma_uint64 msToFrames(int ms);

    static void doFadeStop(ma_sound& snd, float currentVol, int fadeOutMs);

    void cleanup(); // Internal cleanup method

    friend void mixer::_tick(); // Allow mixer::_tick to access private cleanup
};

class AudioStream
{
public:
    explicit AudioStream(const std::string& filePath, float volume = 1.f);
    ~AudioStream();

    void play(int fadeInMs = 0, bool loop = false);

    void stop(int fadeOutMs = 0);

    void pause();

    void resume();

    void rewind();

    void setVolume(float volume);

    [[nodiscard]] float getVolume() const;

    void setLooping(bool loop);

private:
    static constexpr ma_uint32 m_flags =
        MA_SOUND_FLAG_STREAM | MA_SOUND_FLAG_NO_SPATIALIZATION | MA_SOUND_FLAG_NO_PITCH;

    ma_sound m_snd{};

    [[nodiscard]] static ma_uint64 msToFrames(int ms);
};
} // namespace kn
