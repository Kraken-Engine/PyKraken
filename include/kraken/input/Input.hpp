#pragma once

#include <SDL3/SDL.h>

#include <string>
#include <variant>
#include <vector>

#include "kraken/core/_globals.hpp"

namespace kn
{
class Vec2;

struct InputAction
{
    using InputData =
        std::variant<Scancode, Keycode, MouseButton, GamepadButton, std::pair<GamepadAxis, bool>>;
    InputData data;
    int padSlot = 0;

    explicit InputAction(Scancode scan);
    explicit InputAction(Keycode key);
    explicit InputAction(MouseButton mButton);
    explicit InputAction(GamepadButton cButton, int slot = 0);
    InputAction(GamepadAxis axis, bool isPositive, int slot = 0);
};

namespace input
{

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
