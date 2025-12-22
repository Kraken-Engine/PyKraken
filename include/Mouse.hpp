#pragma once

#include <SDL3/SDL.h>
#include <pybind11/pybind11.h>

#include "_globals.hpp"

namespace py = pybind11;

namespace kn
{
class Vec2;
struct Event;

namespace mouse
{
void _bind(py::module_& module);

Vec2 getPos();

Vec2 getRel();

bool isPressed(MouseButton button);

bool isJustPressed(MouseButton button);

bool isJustReleased(MouseButton button);

void lock();

void unlock();

bool isLocked();

void hide();

void show();

bool isHidden();

void _clearStates();

void _handleEvents(const SDL_Event& sdlEvent, const Event& e);
}  // namespace mouse
}  // namespace kn
