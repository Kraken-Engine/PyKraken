#pragma once

#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include <miniaudio/miniaudio.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace mixer
{
void _bind(py::module_& module);

void _init();

void _quit();

void _tick(); // Internal cleanup function, not exposed to Python
} // namespace mixer

class Audio
{
  public:
    Audio(const std::string& path, float volume = 1.f);
    ~Audio();

    void play(int fadeInMs = 0, bool loop = false);

    void stop(int fadeOutMs = 0);

    void setVolume(float volume);

    float getVolume() const;

  private:
    struct Voice
    {
        ma_sound snd;
        std::atomic<bool> done{false};
        float volume = 1.f;
    };

    std::string m_path;
    ma_sound m_proto;
    std::mutex m_mutex;
    std::vector<std::unique_ptr<Voice>> m_voices;
    int m_nextId = 0;
    float m_volume;

    ma_uint64 msToFrames(int ms) const;

    void doFadeStop(ma_sound& snd, float currentVol, int fadeOutMs);

    void cleanup(); // Internal cleanup method

    friend void mixer::_tick(); // Allow mixer::_tick to access private cleanup
};
