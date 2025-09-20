#pragma once

#include <chrono>
#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace kn
{
class Timer
{
public:
    explicit Timer(double duration);
    ~Timer() = default;

    void start();

    void pause();

    void resume();

    void reset();

    [[nodiscard]] bool isDone() const;

    [[nodiscard]] double timeRemaining() const;

    [[nodiscard]] double elapsedTime() const;

    [[nodiscard]] double progress() const;

private:
    double m_duration;
    bool m_started = false;
    bool m_paused = false;
    double m_elapsedPausedTime = 0.0;

    std::chrono::steady_clock::time_point m_startTime;
    std::chrono::steady_clock::time_point m_pauseTime;
};

namespace time
{
void _bind(py::module_& module);

double getDelta();

void setMaxDelta(double maxDelta);

void setTarget(uint16_t frameRate);

double getFPS();

double getElapsed();

void delay(uint64_t ms);

void setScale(double scale);

double getScale();

void _tick();
} // namespace time
} // namespace kn
