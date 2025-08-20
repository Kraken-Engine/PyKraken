#include "Time.hpp"
#include "Mixer.hpp"
#include <SDL3/SDL.h>

static uint64_t _lastTick = 0;
static double _fps = 0.0;
static uint16_t _frameCap = 0;
static double _timeStep = 1.0 / 60.0;
static double _delta = 0.0;

namespace kn::time
{
void _bind(py::module_& module)
{
    py::classh<Timer>(module, "Timer", R"doc(
A timer for tracking countdown durations with pause/resume functionality.

The Timer class provides a simple countdown timer that can be started, paused,
and resumed. It's useful for implementing time-based game mechanics like
cooldowns, temporary effects, or timed events.
    )doc")
        .def(py::init<double>(), py::arg("duration"), R"doc(
Create a new Timer instance with the specified duration.

Args:
    duration (float): The countdown duration in seconds. Must be greater than 0.

Raises:
    RuntimeError: If duration is less than or equal to 0.
        )doc")
        .def("start", &Timer::start, R"doc(
Start or restart the timer countdown.

This begins the countdown from the full duration. If the timer was previously
started, this will reset it back to the beginning.
        )doc")
        .def("pause", &Timer::pause, R"doc(
Pause the timer countdown.

The timer will stop counting down but retain its current state. Use resume()
to continue the countdown from where it was paused. Has no effect if the
timer is not started or already paused.
        )doc")
        .def("resume", &Timer::resume, R"doc(
Resume a paused timer countdown.

Continues the countdown from where it was paused. Has no effect if the
timer is not started or not currently paused.
        )doc")
        .def("reset", &Timer::reset, R"doc(
Reset the timer to its initial state.

Stops the timer and resets it back to its initial, unstarted state.
The timer can be started again with `start()` after being reset.
        )doc")

        .def_property_readonly("done", &Timer::isDone, R"doc(
bool: True if the timer has finished counting down, False otherwise.

A timer is considered done when the elapsed time since start (excluding
paused time) equals or exceeds the specified duration.
        )doc")
        .def_property_readonly("time_remaining", &Timer::timeRemaining, R"doc(
float: The remaining time in seconds before the timer completes.

Returns the full duration if the timer hasn't been started, or 0.0 if
the timer has already finished.
        )doc")
        .def_property_readonly("elapsed_time", &Timer::elapsedTime, R"doc(
float: The time elapsed since the timer was started, in seconds.

Returns 0.0 if the timer hasn't been started. This includes time spent
while paused, giving you the total wall-clock time since start().
        )doc")
        .def_property_readonly("progress", &Timer::progress, R"doc(
float: The completion progress of the timer as a value between 0.0 and 1.0.

Returns 0.0 if the timer hasn't been started, and 1.0 when the timer
is complete. Useful for progress bars and interpolated animations.
        )doc");

    auto subTime = module.def_submodule("time", "Time related functions");

    subTime.def("get_delta", &getDelta, R"doc(
Get the time elapsed since the last frame in seconds.

For stability, the returned delta is clamped so it will not be
smaller than 1/12 seconds (equivalent to capping at 12 FPS). This prevents
unstable calculations that rely on delta when very small frame times are
measured.

Returns:
    float: The time elapsed since the last frame, in seconds.
        )doc");

    subTime.def("get_fps", &getFPS, R"doc(
Get the current frames per second of the program.

Returns:
    float: The current FPS based on the last frame time.
        )doc");

    subTime.def("set_cap", &setCap, py::arg("frame_rate"), R"doc(
Set the maximum framerate for the application.

Args:
    frame_rate (int): Maximum framerate to enforce. Set to 0 for unlimited.
        )doc");

    subTime.def("get_elapsed", &getElapsed, R"doc(
Get the elapsed time since the program started.

Returns:
    float: The total elapsed time since program start, in seconds.
        )doc");
    subTime.def("delay", &delay, py::arg("milliseconds"), R"doc(
Delay the program execution for the specified duration.

This function pauses execution for the given number of milliseconds.
Useful for simple timing control, though using time.set_cap() is generally
preferred for precise frame rate control with nanosecond accuracy.

Args:
    milliseconds (int): The number of milliseconds to delay.
        )doc");
}

double getDelta() { return _delta; }

double getFPS() { return _fps; }

void setCap(const uint16_t frameRate) { _frameCap = frameRate; }

double getElapsed() { return static_cast<double>(SDL_GetTicksNS()) / SDL_NS_PER_SECOND; }

void delay(const uint64_t ms) { SDL_Delay(ms); }

void _tick()
{
    uint64_t now = SDL_GetTicksNS();

    // Stable first frame
    if (_lastTick == 0)
    {
        _lastTick = now;
        _delta = 0.0;
        _fps = 0;
        return;
    }

    uint64_t frameTime = now - _lastTick;

    if (_frameCap > 0)
    {
        const uint64_t targetFrameTimeNS = SDL_NS_PER_SECOND / _frameCap;
        if (frameTime < targetFrameTimeNS)
        {
            SDL_DelayNS(targetFrameTimeNS - frameTime);
            now = SDL_GetTicksNS();
            frameTime = now - _lastTick;
        }
    }

    _lastTick = now;
    _delta = static_cast<double>(frameTime) / SDL_NS_PER_SECOND;

    // Prevent division by zero and handle very small delta times
    _fps = (_delta > 0.0) ? (1.0 / _delta) : 0.0;

    // Cap delta at 12fps
    if (_fps < 12.0)
        _delta = 1.0 / 12.0;

    // Automatic audio cleanup
    mixer::_tick();
}
} // namespace kn::time

Timer::Timer(const double duration) : m_duration(duration)
{
    if (duration <= 0.0)
        throw std::invalid_argument("Timer duration must be greater than 0");
}

void Timer::start()
{
    m_startTime = std::chrono::steady_clock::now();
    m_started = true;
    m_paused = false;
    m_elapsedPausedTime = 0.0; // Reset accumulated pause time when starting
}

void Timer::pause()
{
    if (m_started && !m_paused)
    {
        m_pauseTime = std::chrono::steady_clock::now();
        m_paused = true;
    }
}

void Timer::resume()
{
    if (m_started && m_paused)
    {
        auto now = std::chrono::steady_clock::now();
        auto pauseDuration = now - m_pauseTime;
        m_elapsedPausedTime += std::chrono::duration<double>(pauseDuration).count();
        m_paused = false;
    }
}

void Timer::reset()
{
    m_started = false;
    m_paused = false;
    m_elapsedPausedTime = 0.0;
}

bool Timer::isDone() const
{
    if (!m_started)
        return false;

    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> totalElapsed = now - m_startTime;

    double effectiveElapsed;
    if (m_paused)
    {
        // If currently paused, don't count time since pause started
        std::chrono::duration<double> pausedDuration = now - m_pauseTime;
        effectiveElapsed = totalElapsed.count() - m_elapsedPausedTime - pausedDuration.count();
    }
    else
    {
        // Not currently paused, just subtract accumulated pause time
        effectiveElapsed = totalElapsed.count() - m_elapsedPausedTime;
    }

    return (effectiveElapsed >= m_duration);
}

double Timer::timeRemaining() const
{
    if (!m_started)
        return m_duration;

    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> totalElapsed = now - m_startTime;

    double effectiveElapsed;
    if (m_paused)
    {
        // If currently paused, don't count time since pause started
        std::chrono::duration<double> pausedDuration = now - m_pauseTime;
        effectiveElapsed = totalElapsed.count() - m_elapsedPausedTime - pausedDuration.count();
    }
    else
    {
        // Not currently paused, just subtract accumulated pause time
        effectiveElapsed = totalElapsed.count() - m_elapsedPausedTime;
    }

    double remaining = m_duration - effectiveElapsed;
    return (remaining > 0.0) ? remaining : 0.0;
}

double Timer::elapsedTime() const
{
    if (!m_started)
        return 0.0;

    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> totalElapsed = now - m_startTime;

    double effectiveElapsed;
    if (m_paused)
    {
        // If currently paused, don't count time since pause started
        std::chrono::duration<double> pausedDuration = now - m_pauseTime;
        effectiveElapsed = totalElapsed.count() - m_elapsedPausedTime - pausedDuration.count();
    }
    else
    {
        // Not currently paused, just subtract accumulated pause time
        effectiveElapsed = totalElapsed.count() - m_elapsedPausedTime;
    }

    return std::max(effectiveElapsed, 0.0);
}

double Timer::progress() const
{
    if (!m_started)
        return 0.0;

    double elapsed = elapsedTime();

    return std::min(elapsed / m_duration, 1.0);
}
