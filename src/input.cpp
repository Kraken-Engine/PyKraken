#include "Input.hpp"

#ifdef KRAKEN_ENABLE_PYTHON
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#endif  // KRAKEN_ENABLE_PYTHON

#include <algorithm>
#include <unordered_map>

#include "Gamepad.hpp"
#include "Key.hpp"
#include "Math.hpp"
#include "Mouse.hpp"

template <class... Ts>
struct overloaded : Ts...
{
    using Ts::operator()...;
};

template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

namespace kn
{
InputAction::InputAction(const Scancode scan)
    : data(scan)
{
}

InputAction::InputAction(const Keycode key)
    : data(key)
{
}

InputAction::InputAction(const MouseButton mButton)
    : data(mButton)
{
}

InputAction::InputAction(const GamepadButton cButton, const int slot)
    : data(cButton),
      padSlot(slot)
{
}

InputAction::InputAction(const GamepadAxis axis, const bool isPositive, const int slot)
    : data(std::make_pair(axis, isPositive)),
      padSlot(slot)
{
}

namespace input
{
static std::unordered_map<std::string, std::vector<InputAction>> _inputBindings;

void bind(const std::string& name, const std::vector<InputAction>& actions)
{
    if (name.empty())
        throw std::invalid_argument("Input name cannot be empty.");

    _inputBindings[name] = actions;
}

void unbind(const std::string& name)
{
    _inputBindings.erase(name);
}

// Returns the maximum activation strength [0, 1] across all bindings for a given action.
// Digital inputs (keys, buttons) contribute 1.0 when active.
// Analog inputs contribute their absolute deflection in the matching direction.
static double getStrength(const std::string& name)
{
    const auto it = _inputBindings.find(name);
    if (it == _inputBindings.end())
        return 0.0;

    const Vec2 leftStick = gamepad::getLeftStick();
    const Vec2 rightStick = gamepad::getRightStick();

    double strength = 0.0;

    for (const auto& action : it->second)
    {
        std::visit(
            overloaded{
                [&](const Scancode scan)
                {
                    if (key::isPressed(scan))
                        strength = std::max(strength, 1.0);
                },
                [&](const Keycode key)
                {
                    if (key::isPressed(key))
                        strength = std::max(strength, 1.0);
                },
                [&](const MouseButton mButton)
                {
                    if (mouse::isPressed(mButton))
                        strength = std::max(strength, 1.0);
                },
                [&](const GamepadButton cButton)
                {
                    if (gamepad::isPressed(cButton, action.padSlot))
                        strength = std::max(strength, 1.0);
                },
                [&](const std::pair<GamepadAxis, bool>& axisPair)
                {
                    auto [axis, isPositive] = axisPair;

                    auto process = [&](const GamepadAxis a, const double value)
                    {
                        if (axis == a && ((isPositive && value > 0) || (!isPositive && value < 0)))
                            strength = std::max(strength, std::abs(value));
                    };

                    process(GamepadAxis::LeftX, leftStick.x);
                    process(GamepadAxis::LeftY, leftStick.y);
                    process(GamepadAxis::RightX, rightStick.x);
                    process(GamepadAxis::RightY, rightStick.y);
                },
            },
            action.data
        );
    }

    return strength;
}

Vec2 getDirection(
    const std::string& up, const std::string& right, const std::string& down,
    const std::string& left
)
{
    Vec2 direction{
        getStrength(right) - getStrength(left),
        getStrength(down) - getStrength(up),
    };

    if (direction.getLengthSquared() > 1.0)
        direction.normalize();

    return direction;
}

double getAxis(const std::string& negative, const std::string& positive)
{
    return std::clamp(getStrength(positive) - getStrength(negative), -1.0, 1.0);
}

bool isPressed(const std::string& name)
{
    const auto it = _inputBindings.find(name);
    if (it == _inputBindings.end())
        return false;

    return std::ranges::any_of(
        it->second,
        [](const InputAction& action)
        {
            return std::visit(
                overloaded{
                    [](const Scancode scan) -> bool { return key::isPressed(scan); },
                    [](const Keycode key) -> bool { return key::isPressed(key); },
                    [](const MouseButton mButton) -> bool { return mouse::isPressed(mButton); },
                    [&](const GamepadButton cButton) -> bool
                    { return gamepad::isPressed(cButton, action.padSlot); },
                    [](const std::pair<GamepadAxis, bool>&) -> bool { return false; },
                },
                action.data
            );
        }
    );
}

bool isJustPressed(const std::string& name)
{
    const auto it = _inputBindings.find(name);
    if (it == _inputBindings.end())
        return false;

    return std::ranges::any_of(
        it->second,
        [](const InputAction& action)
        {
            return std::visit(
                overloaded{
                    [](const Scancode scan) -> bool { return key::isJustPressed(scan); },
                    [](const Keycode key) -> bool { return key::isJustPressed(key); },
                    [](const MouseButton mButton) -> bool { return mouse::isJustPressed(mButton); },
                    [&](const GamepadButton cButton) -> bool
                    { return gamepad::isJustPressed(cButton, action.padSlot); },
                    [](const std::pair<GamepadAxis, bool>&) -> bool { return false; },
                },
                action.data
            );
        }
    );
}

bool isJustReleased(const std::string& name)
{
    const auto it = _inputBindings.find(name);
    if (it == _inputBindings.end())
        return false;

    return std::ranges::any_of(
        it->second,
        [](const InputAction& action)
        {
            return std::visit(
                overloaded{
                    [](const Scancode scan) -> bool { return key::isJustReleased(scan); },
                    [](const Keycode key) -> bool { return key::isJustReleased(key); },
                    [](const MouseButton mButton) -> bool
                    { return mouse::isJustReleased(mButton); },
                    [&](const GamepadButton cButton) -> bool
                    { return gamepad::isJustReleased(cButton, action.padSlot); },
                    [](const std::pair<GamepadAxis, bool>&) -> bool { return false; },
                },
                action.data
            );
        }
    );
}

#ifdef KRAKEN_ENABLE_PYTHON
void _bind(nb::module_& module)
{
    using namespace nb::literals;

    nb::class_<InputAction>(module, "InputAction", R"doc(
Represents a single input trigger such as a key, mouse button, or gamepad control.
    )doc")

        .def(nb::init<Scancode>(), "scancode"_a, R"doc(
Create an input action from a scancode.

Args:
    scancode (Scancode): Keyboard scancode.
        )doc")

        .def(nb::init<Keycode>(), "keycode"_a, R"doc(
Create an input action from a keycode.

Args:
    keycode (Keycode): Keyboard keycode.
        )doc")

        .def(nb::init<MouseButton>(), "mouse_button"_a, R"doc(
Create an input action from a mouse button.

Args:
    mouse_button (MouseButton): Mouse button code.
        )doc")

        .def(
            nb::init<GamepadButton, int>(), "gamepad_button"_a, "slot"_a = 0,
            R"doc(
Create an input action from a gamepad button.

Args:
    gamepad_button (GamepadButton): Gamepad button code.
    slot (int, optional): Gamepad slot (default is 0).
        )doc"
        )

        .def(
            nb::init<GamepadAxis, bool, int>(), "gamepad_axis"_a, "is_positive"_a, "slot"_a = 0,
            R"doc(
Create an input action from a gamepad axis direction.

Args:
    gamepad_axis (GamepadAxis): Gamepad axis code.
    is_positive (bool): True for positive direction, False for negative.
    slot (int, optional): Gamepad slot (default is 0).
        )doc"
        );

    auto subInput = module.def_submodule("input", "Input handling and action binding");

    subInput.def("bind", &bind, "name"_a, "actions"_a, R"doc(
    Bind a name to a list of InputActions.

    Args:
        name (str): The identifier for this binding (e.g. "jump").
        actions (Sequence[InputAction]): One or more InputActions to bind.
            )doc");

    subInput.def("unbind", &unbind, "name"_a, R"doc(
Unbind a previously registered input name.

Args:
    name (str): The binding name to remove.
        )doc");

    subInput.def("get_direction", &getDirection, "up"_a, "right"_a, "down"_a, "left"_a, R"doc(
Get a directional vector based on named input actions.

This is typically used for WASD-style or D-pad movement.

Args:
    up (str): Name of action for upward movement.
    right (str): Name of action for rightward movement.
    down (str): Name of action for downward movement.
    left (str): Name of action for leftward movement.

Returns:
    Vec2: A normalized vector representing the intended direction.
        )doc");

    subInput.def("get_axis", &getAxis, "negative"_a, "positive"_a, R"doc(
Get a 1D axis value based on two opposing input actions.

Args:
    negative (str): Name of the negative direction action (e.g. "left").
    positive (str): Name of the positive direction action (e.g. "right").

Returns:
    float: Value in range [-1.0, 1.0] based on input.
        )doc");

    subInput.def("is_pressed", &isPressed, "name"_a, R"doc(
Check if the given action is currently being held.

Args:
    name (str): The name of the bound input.

Returns:
    bool: True if any action bound to the name is pressed.
        )doc");

    subInput.def("is_just_pressed", &isJustPressed, "name"_a, R"doc(
Check if the given action was just pressed this frame.

Args:
    name (str): The name of the bound input.

Returns:
    bool: True if pressed this frame only.
        )doc");

    subInput.def("is_just_released", &isJustReleased, "name"_a, R"doc(
Check if the given action was just released this frame.

Args:
    name (str): The name of the bound input.

Returns:
    bool: True if released this frame only.
        )doc");
}
#endif  // KRAKEN_ENABLE_PYTHON

}  // namespace input
}  // namespace kn
