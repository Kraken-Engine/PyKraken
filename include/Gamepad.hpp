#pragma once

#include <SDL3/SDL.h>
#include <pybind11/pybind11.h>
#include <unordered_map>

namespace py = pybind11;

class Vec2;

namespace event
{
struct knEvent;
}

struct GamepadState
{
    SDL_Gamepad* pad = nullptr;
    float deadzone = 0.1f;
    std::unordered_map<SDL_GamepadButton, bool> justPressed;
    std::unordered_map<SDL_GamepadButton, bool> justReleased;
};

namespace gamepad
{
void _bind(py::module_& module);

bool isPressed(SDL_GamepadButton button, int slot = 0);

bool isJustPressed(SDL_GamepadButton button, int slot = 0);

bool isJustReleased(SDL_GamepadButton button, int slot = 0);

py::tuple getLeftStick(int slot = 0);

py::tuple getRightStick(int slot = 0);

double getLeftTrigger(int slot = 0);

double getRightTrigger(int slot = 0);

void setDeadzone(float deadZone, int slot = 0);

float getDeadzone(int slot = 0);

std::vector<int> getConnectedSlots();

void _clearStates();

void _handleEvents(const SDL_Event& sdle, event::knEvent& e);
} // namespace gamepad
