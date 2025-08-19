#pragma once

#include <chrono>
#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace kn::time
{
void _bind(py::module_& module);

double getDelta();

void setCap(uint16_t frameRate);

double getFPS();

double getElapsed();

void delay(uint64_t ms);

void _tick();
} // namespace kn::time

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
