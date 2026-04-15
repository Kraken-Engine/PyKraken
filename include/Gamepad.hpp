#pragma once

#include <SDL3/SDL.h>
#ifdef KRAKEN_ENABLE_PYTHON
#include <nanobind/nanobind.h>
#endif  // KRAKEN_ENABLE_PYTHON

#include <unordered_map>
#include <vector>

#include "_globals.hpp"

#ifdef KRAKEN_ENABLE_PYTHON
namespace nb = nanobind;
#endif  // KRAKEN_ENABLE_PYTHON

namespace kn
{
struct Event;
class Vec2;

struct GamepadState
{
    SDL_Gamepad* pad = nullptr;
    float deadZone = 0.1f;
    std::unordered_map<GamepadButton, bool> justPressed;
    std::unordered_map<GamepadButton, bool> justReleased;
};

namespace gamepad
{
bool isPressed(GamepadButton button, int slot = 0);

bool isJustPressed(GamepadButton button, int slot = 0);

bool isJustReleased(GamepadButton button, int slot = 0);

Vec2 getLeftStick(int slot = 0);

Vec2 getRightStick(int slot = 0);

double getLeftTrigger(int slot = 0);

double getRightTrigger(int slot = 0);

void setDeadZone(float deadZone, int slot = 0);

float getDeadZone(int slot = 0);

const std::vector<int> getConnectedSlots();

GamepadType getType(int slot = 0);

void _clearStates();

void _handleEvents(const SDL_Event& sdlEvent, Event& e);

#ifdef KRAKEN_ENABLE_PYTHON
void _bind(nb::module_& module);
#endif  // KRAKEN_ENABLE_PYTHON
}  // namespace gamepad
}  // namespace kn
