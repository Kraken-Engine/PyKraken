#include "Time.hpp"
#include <SDL3/SDL.h>

namespace kn::time
{
void _bind(py::module_& module)
{
    py::class_<Clock>(module, "Clock", R"doc(
A clock for tracking frame time and controlling framerate.

The Clock class is used to measure time between frames and optionally
limit the framerate of your application. It's essential for creating
smooth, frame-rate independent animations and game loops.
    )doc")
        .def(py::init<>(), R"doc(
Create a new Clock instance.

The clock starts measuring time immediately upon creation.
        )doc")
        .def("tick", &Clock::tick, py::arg("frame_rate") = 0, R"doc(
Get the time since the last frame and optionally cap the framerate.

This method should be called once per frame in your main loop. It returns
the time elapsed since the last call to tick(), which can be used for
frame-rate independent animations.

Args:
    frame_rate (int, optional): Maximum framerate to enforce. If 0, no limit is applied.
                               Defaults to 0 (unlimited).

Returns:
    float: The time elapsed since the last tick() call, in seconds.
        )doc")
        .def("get_fps", &Clock::getFPS, R"doc(
Get the current frames per second of the program.

Returns:
    int: The current FPS based on the last frame time.
        )doc");

    auto subTime = module.def_submodule("time", "Time related functions");

    subTime.def("get_elapsed_time", &getElapsedTime, R"doc(
Get the elapsed time since the program started.

Returns:
    float: The total elapsed time since program start, in seconds.
        )doc");
    subTime.def("delay", &delay, py::arg("milliseconds"), R"doc(
Delay the program execution for the specified duration.

This function pauses execution for the given number of milliseconds.
Useful for simple timing control, though using Clock.tick() is generally
preferred for frame rate control.

Args:
    milliseconds (int): The number of milliseconds to delay.
        )doc");
}

Clock::Clock() : m_lastTick(SDL_GetTicksNS()) {}

double Clock::tick(const uint16_t frameRate)
{
    uint64_t now = SDL_GetTicksNS();
    uint64_t frameTime = now - m_lastTick;

    if (frameRate > 0)
    {
        const uint64_t targetFrameTimeNS = SDL_NS_PER_SECOND / frameRate;
        if (frameTime < targetFrameTimeNS)
        {
            SDL_DelayNS(targetFrameTimeNS - frameTime);
            now = SDL_GetTicksNS();
            frameTime = now - m_lastTick;
        }
    }

    m_lastTick = now;
    double delta = static_cast<double>(frameTime) / SDL_NS_PER_SECOND;
    m_fps = static_cast<uint64_t>(1.0 / delta);

    return delta;
}

uint64_t Clock::getFPS() const { return m_fps; }

double getElapsedTime() { return SDL_GetTicksNS() / SDL_NS_PER_SECOND; }

void delay(const uint64_t ms) { SDL_DelayNS(SDL_MS_TO_NS(ms)); }
} // namespace kn::time
