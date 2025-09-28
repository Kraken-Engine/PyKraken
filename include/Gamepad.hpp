#pragma once

#include <SDL3/SDL.h>
#include <pybind11/pybind11.h>
#include <unordered_map>

namespace py = pybind11;

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
void _bind(py::module_& module);

bool isPressed(SDL_GamepadButton button, int slot = 0);

bool isJustPressed(SDL_GamepadButton button, int slot = 0);

bool isJustReleased(SDL_GamepadButton button, int slot = 0);

Vec2 getLeftStick(int slot = 0);

Vec2 getRightStick(int slot = 0);

double getLeftTrigger(int slot = 0);

double getRightTrigger(int slot = 0);

void setDeadZone(float deadZone, int slot = 0);

float getDeadZone(int slot = 0);

std::vector<int> getConnectedSlots();

void _clearStates();

void _handleEvents(const SDL_Event& sdlEvent, const Event& e);
} // namespace gamepad
} // namespace kn
