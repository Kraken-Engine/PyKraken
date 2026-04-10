#pragma once

#include <SDL3/SDL.h>
#ifdef KRAKEN_ENABLE_PYTHON
#include <nanobind/nanobind.h>
#endif // KRAKEN_ENABLE_PYTHON

#include <unordered_map>
#include <vector>

#ifdef KRAKEN_ENABLE_PYTHON
namespace nb = nanobind;
#endif // KRAKEN_ENABLE_PYTHON

namespace kn
{
struct Event;
class Vec2;

struct GamepadState
{
    SDL_Gamepad* pad = nullptr;
    float deadZone = 0.1f;
    std::unordered_map<SDL_GamepadButton, bool> justPressed;
    std::unordered_map<SDL_GamepadButton, bool> justReleased;
};

namespace gamepad
{
bool isPressed(SDL_GamepadButton button, int slot = 0);

bool isJustPressed(SDL_GamepadButton button, int slot = 0);

bool isJustReleased(SDL_GamepadButton button, int slot = 0);

Vec2 getLeftStick(int slot = 0);

Vec2 getRightStick(int slot = 0);

double getLeftTrigger(int slot = 0);

double getRightTrigger(int slot = 0);

void setDeadZone(float deadZone, int slot = 0);

float getDeadZone(int slot = 0);

const std::vector<int> getConnectedSlots();

void _clearStates();

void _handleEvents(const SDL_Event& sdlEvent, const Event& e);

#ifdef KRAKEN_ENABLE_PYTHON
void _bind(nb::module_& module);
#endif // KRAKEN_ENABLE_PYTHON
}  // namespace gamepad
}  // namespace kn
