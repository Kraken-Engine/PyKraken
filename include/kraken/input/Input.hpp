#pragma once

#include <SDL3/SDL.h>

#include <string>
#include <variant>
#include <vector>

#include "kraken/core/_globals.hpp"

namespace kn
{
class Vec2;

class InputAction
{
  public:
    using InputData =
        std::variant<Scancode, Keycode, MouseButton, GamepadButton, std::pair<GamepadAxis, bool>>;

    explicit InputAction(Scancode scan);
    explicit InputAction(Keycode key);
    explicit InputAction(MouseButton mButton);
    explicit InputAction(GamepadButton cButton, int slot = 0);
    InputAction(GamepadAxis axis, bool isPositive, int slot = 0);

    int getPadSlot() const;
    InputData getData() const;

  private:
    InputData data;
    int padSlot = 0;
};

namespace input
{

// Internal event-poll lifecycle; queries never consume state.
void _beginFrame();
void _handleEvent(const SDL_Event& event);
void _reset();

void bind(const std::string& name, const std::vector<InputAction>& actions);

void unbind(const std::string& name);

Vec2 getDirection(
    const std::string& up = "", const std::string& right = "", const std::string& down = "",
    const std::string& left = ""
);

double getAxis(const std::string& negative = "", const std::string& positive = "");

// Digital state from the latest event::poll(). Queries do not consume edges.
// Both edges can be true in one poll; analog axis bindings are excluded.
bool isPressed(const std::string& name);

bool isJustPressed(const std::string& name);

bool isJustReleased(const std::string& name);
}  // namespace input
}  // namespace kn
