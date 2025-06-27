#include "Time.hpp"
#include <SDL3/SDL.h>

namespace kn::time
{
void _bind(py::module_& module)
{
    py::class_<Clock>(module, "Clock")
        .def(py::init<>())
        .def("tick", &Clock::tick, py::arg("frame_rate") = 0,
             "Get the time since the last frame in seconds and optionally cap the framerate")
        .def("get_fps", &Clock::getFPS, "Get the current frames per second of the program");

    auto subTime = module.def_submodule("time", "Time related functions");

    subTime.def("get_elapsed_time", &getElapsedTime,
                "Get the elapsed time since the program started, in seconds");
    subTime.def("delay", &delay, "Delay the program by a duration in milliseconds");
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
