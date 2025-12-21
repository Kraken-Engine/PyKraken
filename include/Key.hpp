#pragma once

#include <SDL3/SDL.h>
#include <pybind11/pybind11.h>

#include "_globals.hpp"

namespace py = pybind11;

namespace kn
{
struct Event;
namespace key
{
void _bind(py::module_& module);

void _handleEvents(const SDL_Event& sdlEvent, const Event& e);

void _clearStates();

bool isPressed(SDL_Scancode scancode);

bool isJustPressed(SDL_Scancode scancode);

bool isJustReleased(SDL_Scancode scancode);

bool isPressed(Keycode keycode);

bool isJustPressed(Keycode keycode);

bool isJustReleased(Keycode keycode);
}  // namespace key
}  // namespace kn
