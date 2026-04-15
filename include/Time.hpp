#pragma once

#ifdef KRAKEN_ENABLE_PYTHON
#include <nanobind/nanobind.h>
#endif  // KRAKEN_ENABLE_PYTHON

#include <chrono>

#ifdef KRAKEN_ENABLE_PYTHON
namespace nb = nanobind;
#endif  // KRAKEN_ENABLE_PYTHON

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

    void restart();

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
#ifdef KRAKEN_ENABLE_PYTHON
void _bind(nb::module_& module);
#endif  // KRAKEN_ENABLE_PYTHON

double getDelta();

void setMaxDelta(double maxDelta);

void setTarget(uint16_t frameRate);

double getFPS();

double getElapsed();

void delay(uint64_t ms);

void setScale(double scale);

double getScale();

void _tick();
}  // namespace time
}  // namespace kn
