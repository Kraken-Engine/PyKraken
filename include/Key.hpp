#pragma once

#include <SDL3/SDL.h>
#ifdef KRAKEN_ENABLE_PYTHON
#include <nanobind/nanobind.h>
#endif // KRAKEN_ENABLE_PYTHON

#include "_globals.hpp"

#ifdef KRAKEN_ENABLE_PYTHON
namespace nb = nanobind;
#endif // KRAKEN_ENABLE_PYTHON

namespace kn
{
struct Event;
namespace key
{
#ifdef KRAKEN_ENABLE_PYTHON
void _bind(nb::module_& module);
#endif // KRAKEN_ENABLE_PYTHON

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
