#include "kraken/input/Key.hpp"

#include <nanobind/nanobind.h>

#include <algorithm>
#include <unordered_map>

#include "bindings/python/bindings.hpp"
#include "kraken/core/_globals.hpp"
#include "kraken/input/Event.hpp"

namespace kn::key
{
void _bind(nb::module_& module)
{
    using namespace nb::literals;

    auto subKey = module.def_submodule("key", "Keyboard key state checks");

    subKey.def(
        "is_pressed", nb::overload_cast<Scancode>(&isPressed), "scancode"_a,
        R"doc(
Check if a key is currently held down (by scancode).

Args:
    scancode (Scancode): The physical key (e.g., S_w).

Returns:
    bool: True if the key is held.
        )doc"
    );

    subKey.def(
        "is_just_pressed", nb::overload_cast<Scancode>(&isJustPressed), "scancode"_a,
        R"doc(
Check if a key was pressed this frame (by scancode).

Args:
    scancode (Scancode): The physical key.

Returns:
    bool: True if the key was newly pressed.
        )doc"
    );

    subKey.def(
        "is_just_released", nb::overload_cast<Scancode>(&isJustReleased), "scancode"_a,
        R"doc(
Check if a key was released this frame (by scancode).

Args:
    scancode (Scancode): The physical key.

Returns:
    bool: True if the key was newly released.
        )doc"
    );

    subKey.def(
        "is_pressed", nb::overload_cast<Keycode>(&isPressed), "keycode"_a,
        R"doc(
Check if a key is currently held down (by keycode).

Args:
    keycode (Keycode): The symbolic key (e.g., K_SPACE).

Returns:
    bool: True if the key is held.
        )doc"
    );

    subKey.def(
        "is_just_pressed", nb::overload_cast<Keycode>(&isJustPressed), "keycode"_a,
        R"doc(
Check if a key was pressed this frame (by keycode).

Args:
    keycode (Keycode): The symbolic key.

Returns:
    bool: True if the key was newly pressed.
        )doc"
    );

    subKey.def(
        "is_just_released", nb::overload_cast<Keycode>(&isJustReleased), "keycode"_a,
        R"doc(
Check if a key was released this frame (by keycode).

Args:
    keycode (Keycode): The symbolic key.

Returns:
    bool: True if the key was newly released.
        )doc"
    );
}
}  // namespace kn::key
