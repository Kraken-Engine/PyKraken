#include "kraken/input/Input.hpp"

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>

#include <algorithm>
#include <unordered_map>

#include "bindings/python/bindings.hpp"
#include "kraken/input/Gamepad.hpp"
#include "kraken/input/Key.hpp"
#include "kraken/input/Mouse.hpp"
#include "kraken/math/Math.hpp"

namespace kn::input
{
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
}  // namespace kn::input
