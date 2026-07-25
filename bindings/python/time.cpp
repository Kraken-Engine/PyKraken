#include "kraken/core/Time.hpp"

#include <SDL3/SDL.h>
#include <nanobind/nanobind.h>

#include <algorithm>
#include <limits>

#include "bindings/python/bindings.hpp"

namespace kn::time
{
void _bind(nb::module_& module)
{
    using namespace nb::literals;

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

    subTime.def("set_max_delta", &setMaxDelta, "max_delta"_a, R"doc(
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

    subTime.def("set_target", &setTarget, "frame_rate"_a, R"doc(
Set the target framerate for the application.

Args:
    frame_rate (int): Target framerate to enforce. Values <= 0 disable frame rate limiting.
        )doc");

    subTime.def("get_elapsed", &getElapsed, R"doc(
Get the elapsed time since the program started.

Returns:
    float: The total elapsed time since program start, in seconds.
        )doc");

    subTime.def("delay", &delay, "milliseconds"_a, R"doc(
Delay the program execution for the specified duration.

This function pauses execution for the given number of milliseconds.
Useful for simple timing control, though using time.set_cap() is generally
preferred for precise frame rate control with nanosecond accuracy.

Args:
    milliseconds (int): The number of milliseconds to delay.
        )doc");

    subTime.def("set_scale", &setScale, "scale"_a, R"doc(
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

    nb::class_<Timer>(module, "Timer", R"doc(
A timer for tracking countdown durations with pause/resume functionality.

The Timer class provides a simple countdown timer that can be started, paused,
and resumed. It's useful for implementing time-based game mechanics like
cooldowns, temporary effects, or timed events.
    )doc")
        .def(nb::init<double>(), "duration"_a, R"doc(
Create a new Timer instance with the specified duration.

Args:
    duration (float): The countdown duration in seconds. Must be greater than 0.

Raises:
    RuntimeError: If duration is less than or equal to 0.
    )doc")

        .def_prop_rw("duration", &Timer::getDuration, &Timer::setDuration, R"doc(
Get or set the duration of the timer as a float greater than 0.
        )doc")

        .def_prop_ro("done", &Timer::isDone, R"doc(
bool: True if the timer has finished counting down, False otherwise.

A timer is considered done when the elapsed time since start (excluding
paused time) equals or exceeds the specified duration.
    )doc")
        .def_prop_ro("time_remaining", &Timer::timeRemaining, R"doc(
float: The remaining time in seconds before the timer completes.

Returns the full duration if the timer hasn't been started, or 0.0 if
the timer has already finished.
    )doc")
        .def_prop_ro("elapsed_time", &Timer::elapsedTime, R"doc(
float: The time elapsed since the timer was started, in seconds.

Returns 0.0 if the timer hasn't been started. This includes time spent
while paused, giving you the total wall-clock time since start().
    )doc")
        .def_prop_ro("progress", &Timer::progress, R"doc(
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
    )doc")
        .def("restart", &Timer::restart, R"doc(
Restart the timer countdown.

This is a convenience method that combines reset() and start() into one call.
    )doc");
}
}  // namespace kn::time
