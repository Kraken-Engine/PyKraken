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
class Vec2;
struct Event;

namespace mouse
{
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

void _handleEvents(const SDL_Event& sdlEvent, Event& e);

#ifdef KRAKEN_ENABLE_PYTHON
void _bind(nb::module_& module);
#endif  // KRAKEN_ENABLE_PYTHON
}  // namespace mouse
}  // namespace kn
