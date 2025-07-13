#pragma once

#include <SDL3/SDL.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;

enum class knMouseButton : uint8_t;

class Vec2;

namespace event
{
struct knEvent;
}

namespace mouse
{
void _bind(py::module_& module);

py::tuple getPos();

py::tuple getRel();

bool isPressed(knMouseButton button);

bool isJustPressed(knMouseButton button);

bool isJustReleased(knMouseButton button);

void lock();

void unlock();

bool isLocked();

void hide();

void show();

bool isHidden();

void _clearStates();

void _handleEvents(const SDL_Event& sdle, event::knEvent& e);
} // namespace mouse