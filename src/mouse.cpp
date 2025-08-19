#include "Mouse.hpp"
#include "Camera.hpp"
#include "Event.hpp"
#include "Math.hpp"
#include "Window.hpp"
#include "_globals.hpp"

constexpr size_t MOUSE_BUTTON_COUNT = 5;
static bool _mousePressed[MOUSE_BUTTON_COUNT];
static bool _mouseReleased[MOUSE_BUTTON_COUNT];

namespace mouse
{
void _bind(pybind11::module_& module)
{
    auto subMouse = module.def_submodule("mouse", "Mouse related functions");

    subMouse.def("get_pos", &mouse::getPos, R"doc(
Get the current position of the mouse cursor.

Returns:
    tuple[float, float]: The current mouse position as (x, y) coordinates.
    )doc");
    subMouse.def("get_rel", &mouse::getRel, R"doc(
Get the relative mouse movement since the last frame.

Returns:
    tuple[float, float]: The relative movement of the mouse as (dx, dy).
    )doc");
    subMouse.def("is_pressed", &mouse::isPressed, py::arg("button"), R"doc(
Check if a mouse button is currently pressed.

Args:
    button (MouseButton): The mouse button to check (e.g., kn.MOUSE_LEFT).

Returns:
    bool: True if the button is currently pressed.
    )doc");
    subMouse.def("is_just_pressed", &mouse::isJustPressed, py::arg("button"), R"doc(
Check if a mouse button was pressed this frame.

Args:
    button (MouseButton): The mouse button to check.

Returns:
    bool: True if the button was just pressed.
    )doc");
    subMouse.def("is_just_released", &mouse::isJustReleased, py::arg("button"), R"doc(
Check if a mouse button was released this frame.

Args:
    button (MouseButton): The mouse button to check.

Returns:
    bool: True if the button was just released.
    )doc");
    subMouse.def("lock", &mouse::lock, R"doc(
Lock the mouse to the center of the window.

Useful for first-person controls where you want to capture mouse movement
without letting the cursor leave the window area.
    )doc");
    subMouse.def("unlock", &mouse::unlock, R"doc(
Unlock the mouse from the window, allowing it to move freely.
    )doc");
    subMouse.def("is_locked", &mouse::isLocked, R"doc(
Check if the mouse is currently locked to the window.

Returns:
    bool: True if the mouse is locked.
    )doc");
    subMouse.def("hide", &mouse::hide, R"doc(
Hide the mouse cursor from view.

The cursor will be invisible but mouse input will still be tracked.
    )doc");
    subMouse.def("show", &mouse::show, R"doc(
Show the mouse cursor if it was hidden.
    )doc");
    subMouse.def("is_hidden", &mouse::isHidden, R"doc(
Check if the mouse cursor is currently hidden.

Returns:
    bool: True if the cursor is hidden.
    )doc");
}

Vec2 getPos()
{
    float x, y;
    SDL_GetMouseState(&x, &y);
    auto pos = Vec2{x, y} / window::getScale();
    return pos + camera::getActivePos();
}

Vec2 getRel()
{
    float dx, dy;
    SDL_GetRelativeMouseState(&dx, &dy);
    float scale = window::getScale();
    return {dx / scale, dy / scale};
}

bool isPressed(knMouseButton button)
{
    return SDL_GetMouseState(nullptr, nullptr) & static_cast<uint32_t>(button);
}

bool isJustPressed(knMouseButton button) { return _mousePressed[static_cast<size_t>(button) - 1]; }

bool isJustReleased(knMouseButton button)
{
    return _mouseReleased[static_cast<size_t>(button) - 1];
}

void lock() { SDL_SetWindowRelativeMouseMode(window::get(), true); }

void unlock() { SDL_SetWindowRelativeMouseMode(window::get(), false); }

bool isLocked() { return SDL_GetWindowRelativeMouseMode(window::get()); }

void hide() { SDL_HideCursor(); }

void show() { SDL_ShowCursor(); }

bool isHidden() { return !SDL_CursorVisible(); }

void _clearStates()
{
    std::fill(std::begin(_mousePressed), std::end(_mousePressed), false);
    std::fill(std::begin(_mouseReleased), std::end(_mouseReleased), false);
}

void _handleEvents(const SDL_Event& sdle, event::knEvent& e)
{
    switch (sdle.type)
    {
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP:
        if (sdle.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
            _mousePressed[sdle.button.button - 1] = true;
        else if (sdle.type == SDL_EVENT_MOUSE_BUTTON_UP)
            _mouseReleased[sdle.button.button - 1] = true;
        e.data["button"] = py::cast(static_cast<knMouseButton>(sdle.button.button));
        break;
    }
}
} // namespace mouse
