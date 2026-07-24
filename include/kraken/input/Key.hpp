#pragma once

#include <SDL3/SDL.h>

#include "kraken/core/_globals.hpp"

namespace kn
{
struct Event;
namespace key
{

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
