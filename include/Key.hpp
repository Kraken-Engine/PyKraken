#pragma once

#include <SDL3/SDL.h>
#ifdef KRAKEN_ENABLE_PYTHON
#include <nanobind/nanobind.h>
#endif  // KRAKEN_ENABLE_PYTHON

#include "_globals.hpp"

#ifdef KRAKEN_ENABLE_PYTHON
namespace nb = nanobind;
#endif  // KRAKEN_ENABLE_PYTHON

namespace kn
{
struct Event;
namespace key
{
#ifdef KRAKEN_ENABLE_PYTHON
void _bind(nb::module_& module);
#endif  // KRAKEN_ENABLE_PYTHON

void _handleEvents(const SDL_Event& sdlEvent, Event& e);

void _clearStates();

bool isPressed(Scancode scancode);

bool isJustPressed(Scancode scancode);

bool isJustReleased(Scancode scancode);

bool isPressed(Keycode keycode);

bool isJustPressed(Keycode keycode);

bool isJustReleased(Keycode keycode);
}  // namespace key
}  // namespace kn
