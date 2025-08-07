#pragma once

#include <chrono>
#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace kn::time
{
void _bind(py::module_& module);

class Clock
{
  public:
    Clock();
    ~Clock() = default;

    double tick(uint16_t frameRate);

    uint64_t getFPS() const;

  private:
    uint64_t m_lastTick;
    uint64_t m_fps = 0;
};

class Timer
{
  public:
    explicit Timer(double duration);
    ~Timer() = default;

    void start();

    void pause();

    void resume();

    void reset();

    bool isDone() const;

    double timeRemaining() const;

    double elapsedTime() const;

    double progress() const;

  private:
    double m_duration;
    bool m_started = false;
    bool m_paused = false;
    double m_elapsedPausedTime = 0.0;

    std::chrono::steady_clock::time_point m_startTime;
    std::chrono::steady_clock::time_point m_pauseTime;
};

double getElapsed();

void delay(uint64_t ms);
} // namespace kn::time
