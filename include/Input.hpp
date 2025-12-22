#pragma once

#include <SDL3/SDL.h>
#include <pybind11/pybind11.h>

#include <variant>

#include "_globals.hpp"

namespace py = pybind11;

namespace kn
{
class Vec2;

struct InputAction
{
    using InputData = std::variant<
        SDL_Scancode, Keycode, MouseButton, SDL_GamepadButton, std::pair<SDL_GamepadAxis, bool>>;
    InputData data;
    int padSlot = 0;

    explicit InputAction(SDL_Scancode scan);
    explicit InputAction(Keycode key);
    explicit InputAction(MouseButton mButton);
    explicit InputAction(SDL_GamepadButton cButton, int slot = 0);
    InputAction(SDL_GamepadAxis axis, bool isPositive, int slot = 0);
};

namespace input
{
void _bind(py::module_& module);

void bind(const std::string& name, const std::vector<InputAction>& actions);

void unbind(const std::string& name);

Vec2 getDirection(
    const std::string& up = "", const std::string& right = "", const std::string& down = "",
    const std::string& left = ""
);

double getAxis(const std::string& negative = "", const std::string& positive = "");

bool isPressed(const std::string& name);

bool isJustPressed(const std::string& name);

bool isJustReleased(const std::string& name);
}  // namespace input
}  // namespace kn
