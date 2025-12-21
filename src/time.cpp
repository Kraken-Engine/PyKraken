#include "Time.hpp"

#include <SDL3/SDL.h>

#include <algorithm>
#include <limits>

namespace kn
{
static uint64_t _lastTick = 0;
static double _fps = 0.0;
static uint16_t _frameTarget = 0;
static double _delta = 0.0;
static double _maxDelta = std::numeric_limits<double>::infinity();
static double _scale = 1.0;

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
    m_elapsedPausedTime = 0.0;  // Reset accumulated pause time when starting
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
        const auto now = std::chrono::steady_clock::now();
        const auto pauseDuration = now - m_pauseTime;
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

    const auto now = std::chrono::steady_clock::now();
    const std::chrono::duration<double> totalElapsed = now - m_startTime;

    double effectiveElapsed;
    if (m_paused)
    {
        // If currently paused, don't count time since pause started
        const std::chrono::duration<double> pausedDuration = now - m_pauseTime;
        effectiveElapsed = totalElapsed.count() - m_elapsedPausedTime - pausedDuration.count();
    }
    else
    {
        // Not currently paused, just subtract accumulated pause time
        effectiveElapsed = totalElapsed.count() - m_elapsedPausedTime;
    }

    return effectiveElapsed >= m_duration;
}

double Timer::timeRemaining() const
{
    if (!m_started)
        return m_duration;

    const auto now = std::chrono::steady_clock::now();
    const std::chrono::duration<double> totalElapsed = now - m_startTime;

    double effectiveElapsed;
    if (m_paused)
    {
        // If currently paused, don't count time since pause started
        const std::chrono::duration<double> pausedDuration = now - m_pauseTime;
        effectiveElapsed = totalElapsed.count() - m_elapsedPausedTime - pausedDuration.count();
    }
    else
    {
        // Not currently paused, just subtract accumulated pause time
        effectiveElapsed = totalElapsed.count() - m_elapsedPausedTime;
    }

    const double remaining = m_duration - effectiveElapsed;
    return remaining > 0.0 ? remaining : 0.0;
}

double Timer::elapsedTime() const
{
    if (!m_started)
        return 0.0;

    const auto now = std::chrono::steady_clock::now();
    const std::chrono::duration<double> totalElapsed = now - m_startTime;

    double effectiveElapsed;
    if (m_paused)
    {
        // If currently paused, don't count time since pause started
        const std::chrono::duration<double> pausedDuration = now - m_pauseTime;
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

    const double elapsed = elapsedTime();
    return std::min(elapsed / m_duration, 1.0);
}

namespace time
{
double getDelta()
{
    return _delta;
}

void setMaxDelta(const double maxDelta)
{
    if (maxDelta <= 0.0)
        throw std::invalid_argument("Max delta must be greater than 0.0");
    _maxDelta = maxDelta;
}

double getFPS()
{
    return _fps;
}

void setTarget(const uint16_t frameRate)
{
    _frameTarget = frameRate;
}

double getElapsed()
{
    return static_cast<double>(SDL_GetTicksNS()) / SDL_NS_PER_SECOND;
}

void delay(const uint64_t ms)
{
    SDL_Delay(ms);
}

void setScale(double scale)
{
    if (scale < 0.0)
        scale = 0.0;
    _scale = scale;
}

double getScale()
{
    return _scale;
}

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

    if (_frameTarget > 0)
    {
        const uint64_t targetFrameTimeNS = SDL_NS_PER_SECOND / _frameTarget;
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
    _fps = _delta > 0.0 ? 1.0 / _delta : 0.0;

    // Cap delta
    if (_delta > _maxDelta)
        _delta = _maxDelta;

    _delta *= _scale;
}

void _bind(py::module_& module)
{
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

    subTime.def("set_max_delta", &setMaxDelta, py::arg("max_delta"), R"doc(
Set the maximum allowed delta time between frames.

Args:
    max_delta (float): Maximum delta time in seconds (> 0.0).
                       Use this to avoid large deltas during frame drops or pauses
                       that could destabilize physics or animations.
        )doc");

    subTime.def("get_fps", &getFPS, R"doc(
Get the current frames per second of the program.

Returns:
    float: The current FPS based on the last frame time.
        )doc");

    subTime.def("set_target", &setTarget, py::arg("frame_rate"), R"doc(
Set the target framerate for the application.

Args:
    frame_rate (int): Target framerate to enforce. Values <= 0 disable frame rate limiting.
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

    subTime.def("set_scale", &setScale, py::arg("scale"), R"doc(
Set the global time scale factor.

Args:
    scale (float): The time scale factor. Values < 0.0 are clamped to 0.0.
                   A scale of 1.0 represents normal time, 0.5 is half speed,
                   and 2.0 is double speed.
        )doc");

    subTime.def("get_scale", &getScale, R"doc(
Get the current global time scale factor.

Returns:
    float: The current time scale factor.
        )doc");

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
    )doc");
}
}  // namespace time
}  // namespace kn
