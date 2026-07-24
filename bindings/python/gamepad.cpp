#include "kraken/input/Gamepad.hpp"

#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>

#include <array>
#include <optional>
#include <ranges>

#include "bindings/python/bindings.hpp"
#include "kraken/input/Event.hpp"
#include "kraken/math/Math.hpp"

namespace kn::gamepad
{
void _bind(nb::module_& module)
{
    using namespace nb::literals;

    auto subGamepad = module.def_submodule("gamepad", "Gamepad input handling functions");

    subGamepad.def("is_pressed", &isPressed, "button"_a, "slot"_a = 0, R"doc(
Check if a gamepad button is currently being held down.

Args:
    button (GamepadButton): The button code.
    slot (int, optional): Gamepad slot ID (default is 0).

Returns:
    bool: True if the button is pressed.
    )doc");

    subGamepad.def(
        "is_just_pressed", &isJustPressed, "button"_a, "slot"_a = 0,
        R"doc(
Check if a gamepad button was pressed during this frame.

Args:
    button (GamepadButton): The button code.
    slot (int, optional): Gamepad slot ID (default is 0).

Returns:
    bool: True if the button was just pressed.
    )doc"
    );

    subGamepad.def(
        "is_just_released", &isJustReleased, "button"_a, "slot"_a = 0,
        R"doc(
Check if a gamepad button was released during this frame.

Args:
    button (GamepadButton): The button code.
    slot (int, optional): Gamepad slot ID (default is 0).

Returns:
    bool: True if the button was just released.
    )doc"
    );

    subGamepad.def("get_left_stick", &getLeftStick, "slot"_a = 0, R"doc(
Get the left analog stick position.

Args:
    slot (int, optional): Gamepad slot ID (default is 0).

Returns:
    Vec2: A vector of stick input normalized to [-1, 1], or (0, 0) if inside dead zone.
    )doc");

    subGamepad.def("get_right_stick", &getRightStick, "slot"_a = 0, R"doc(
Get the right analog stick position.

Args:
    slot (int, optional): Gamepad slot ID (default is 0).

Returns:
    Vec2: A vector of stick input normalized to [-1, 1], or (0, 0) if inside dead zone.
    )doc");

    subGamepad.def("get_left_trigger", &getLeftTrigger, "slot"_a = 0, R"doc(
Get the left trigger's current pressure value.

Args:
    slot (int, optional): Gamepad slot ID (default is 0).

Returns:
    float: Trigger value in range [0.0, 1.0].
    )doc");

    subGamepad.def("get_right_trigger", &getRightTrigger, "slot"_a = 0, R"doc(
Get the right trigger's current pressure value.

Args:
    slot (int, optional): Gamepad slot ID (default is 0).

Returns:
    float: Trigger value in range [0.0, 1.0].
    )doc");

    subGamepad.def("set_deadzone", &setDeadZone, "deadzone"_a, "slot"_a = 0, R"doc(
Set the dead zone threshold for a gamepad's analog sticks.

Args:
    deadzone (float): Value from 0.0 to 1.0 where movement is ignored.
    slot (int, optional): Gamepad slot ID (default is 0).
    )doc");

    subGamepad.def("get_deadzone", &getDeadZone, "slot"_a = 0, R"doc(
Get the current dead zone value for a gamepad's analog sticks.

Args:
    slot (int, optional): Gamepad slot ID (default is 0).

Returns:
    float: Deadzone threshold.
    )doc");

    subGamepad.def("get_connected_slots", &getConnectedSlots, R"doc(
Get a list of connected gamepad slot indices.

Returns:
    list[int]: A list of slot IDs with active gamepads.
    )doc");

    subGamepad.def("get_type", &getType, "slot"_a = 0, R"doc(
Get the type of gamepad connected in a given slot.

Args:
    slot (int, optional): Gamepad slot ID (default is 0).

Returns:
    GamepadType: An enum value representing the gamepad type, or Unknown if no gamepad is connected.
    )doc");
}
}  // namespace kn::gamepad
