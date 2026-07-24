#include "kraken/input/Mouse.hpp"

#include <nanobind/nanobind.h>

#include <algorithm>

#include "bindings/python/bindings.hpp"
#include "kraken/core/_globals.hpp"
#include "kraken/graphics/Camera.hpp"
#include "kraken/graphics/Renderer.hpp"
#include "kraken/graphics/Window.hpp"
#include "kraken/input/Event.hpp"
#include "kraken/math/Math.hpp"

namespace kn::mouse
{
void _bind(nb::module_& module)
{
    using namespace nb::literals;

    auto subMouse = module.def_submodule("mouse", "Mouse related functions");

    subMouse.def("get_pos", &getPos, R"doc(
Get the current position of the mouse cursor.

Returns:
    Vec2: The current mouse position as (x, y) coordinates.
    )doc");
    subMouse.def("get_rel", &getRel, R"doc(
Get the relative mouse movement since the last frame.

Returns:
    Vec2: The relative movement of the mouse as (dx, dy).
    )doc");
    subMouse.def("is_pressed", &isPressed, "button"_a, R"doc(
Check if a mouse button is currently pressed.

Args:
    button (MouseButton): The mouse button to check (e.g., kn.MOUSE_LEFT).

Returns:
    bool: True if the button is currently pressed.
    )doc");
    subMouse.def("is_just_pressed", &isJustPressed, "button"_a, R"doc(
Check if a mouse button was pressed this frame.

Args:
    button (MouseButton): The mouse button to check.

Returns:
    bool: True if the button was just pressed.
    )doc");
    subMouse.def("is_just_released", &isJustReleased, "button"_a, R"doc(
Check if a mouse button was released this frame.

Args:
    button (MouseButton): The mouse button to check.

Returns:
    bool: True if the button was just released.
    )doc");
    subMouse.def("lock", &lock, R"doc(
Lock the mouse to the center of the window.

Useful for first-person controls where you want to capture mouse movement
without letting the cursor leave the window area.
    )doc");
    subMouse.def("unlock", &unlock, R"doc(
Unlock the mouse from the window, allowing it to move freely.
    )doc");
    subMouse.def("is_locked", &isLocked, R"doc(
Check if the mouse is currently locked to the window.

Returns:
    bool: True if the mouse is locked.
    )doc");
    subMouse.def("hide", &hide, R"doc(
Hide the mouse cursor from view.

The cursor will be invisible but mouse input will still be tracked.
    )doc");
    subMouse.def("show", &show, R"doc(
Show the mouse cursor if it was hidden.
    )doc");
    subMouse.def("is_hidden", &isHidden, R"doc(
Check if the mouse cursor is currently hidden.

Returns:
    bool: True if the cursor is hidden.
    )doc");
}
}  // namespace kn::mouse
