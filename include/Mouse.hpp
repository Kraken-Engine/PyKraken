#pragma once

#include <SDL3/SDL.h>
#include <nanobind/nanobind.h>

#include "_globals.hpp"

namespace nb = nanobind;

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

void _handleEvents(const SDL_Event& sdlEvent, const Event& e);

void _bind(nb::module_& module);
}  // namespace mouse
}  // namespace kn
