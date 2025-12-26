#include "Mouse.hpp"

#include "Camera.hpp"
#include "Event.hpp"
#include "Math.hpp"
#include "Renderer.hpp"
#include "Window.hpp"
#include "_globals.hpp"

namespace kn::mouse
{
constexpr size_t MOUSE_BUTTON_COUNT = 5;
static bool _mousePressed[MOUSE_BUTTON_COUNT];
static bool _mouseReleased[MOUSE_BUTTON_COUNT];

void _bind(py::module_& module)
{
    auto subMouse = module.def_submodule("mouse", "Mouse related functions");

    subMouse.def("get_pos", &getPos, R"doc(
Get the current position of the mouse cursor.

Returns:
    tuple[float, float]: The current mouse position as (x, y) coordinates.
    )doc");
    subMouse.def("get_rel", &getRel, R"doc(
Get the relative mouse movement since the last frame.

Returns:
    tuple[float, float]: The relative movement of the mouse as (dx, dy).
    )doc");
    subMouse.def("is_pressed", &isPressed, py::arg("button"), R"doc(
Check if a mouse button is currently pressed.

Args:
    button (MouseButton): The mouse button to check (e.g., kn.MOUSE_LEFT).

Returns:
    bool: True if the button is currently pressed.
    )doc");
    subMouse.def("is_just_pressed", &isJustPressed, py::arg("button"), R"doc(
Check if a mouse button was pressed this frame.

Args:
    button (MouseButton): The mouse button to check.

Returns:
    bool: True if the button was just pressed.
    )doc");
    subMouse.def("is_just_released", &isJustReleased, py::arg("button"), R"doc(
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

Vec2 getPos()
{
    float windowX;
    float windowY;
    SDL_GetMouseState(&windowX, &windowY);

    float logicalX;
    float logicalY;
    SDL_RenderCoordinatesFromWindow(renderer::_get(), windowX, windowY, &logicalX, &logicalY);

    return Vec2{logicalX, logicalY} + camera::getActivePos();
}

Vec2 getRel()
{
    float dx, dy;
    SDL_GetRelativeMouseState(&dx, &dy);

    float x0;
    float y0;
    float x1;
    float y1;

    auto* r = renderer::_get();
    SDL_RenderCoordinatesFromWindow(r, 0.0f, 0.0f, &x0, &y0);
    SDL_RenderCoordinatesFromWindow(r, dx, dy, &x1, &y1);

    return {x1 - x0, y1 - y0};
}

bool isPressed(MouseButton button)
{
    return SDL_GetMouseState(nullptr, nullptr) & static_cast<uint32_t>(button);
}

bool isJustPressed(MouseButton button)
{
    return _mousePressed[static_cast<size_t>(button) - 1];
}

bool isJustReleased(MouseButton button)
{
    return _mouseReleased[static_cast<size_t>(button) - 1];
}

void lock()
{
    SDL_SetWindowRelativeMouseMode(window::_get(), true);
}

void unlock()
{
    SDL_SetWindowRelativeMouseMode(window::_get(), false);
}

bool isLocked()
{
    return SDL_GetWindowRelativeMouseMode(window::_get());
}

void hide()
{
    SDL_HideCursor();
}

void show()
{
    SDL_ShowCursor();
}

bool isHidden()
{
    return !SDL_CursorVisible();
}

void _clearStates()
{
    std::ranges::fill(_mousePressed, false);
    std::ranges::fill(_mouseReleased, false);
}

void _handleEvents(const SDL_Event& sdlEvent, const Event& e)
{
    switch (sdlEvent.type)
    {
    case SDL_EVENT_MOUSE_MOTION:
        e.data["which"] = sdlEvent.motion.which;
        e.data["x"] = sdlEvent.motion.x;
        e.data["y"] = sdlEvent.motion.y;
        e.data["xrel"] = sdlEvent.motion.xrel;
        e.data["yrel"] = sdlEvent.motion.yrel;
        e.data["state"] = sdlEvent.motion.state;
        e.data["window_id"] = sdlEvent.motion.windowID;
        break;
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP:
        if (sdlEvent.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
            _mousePressed[sdlEvent.button.button - 1] = true;
        else if (sdlEvent.type == SDL_EVENT_MOUSE_BUTTON_UP)
            _mouseReleased[sdlEvent.button.button - 1] = true;
        e.data["which"] = sdlEvent.button.which;
        e.data["button"] = static_cast<MouseButton>(sdlEvent.button.button);
        e.data["x"] = sdlEvent.button.x;
        e.data["y"] = sdlEvent.button.y;
        e.data["clicks"] = sdlEvent.button.clicks;
        e.data["window_id"] = sdlEvent.button.windowID;
        break;
    case SDL_EVENT_MOUSE_WHEEL:
    {
        const int dir = sdlEvent.wheel.direction == SDL_MOUSEWHEEL_FLIPPED ? -1 : 1;
        e.data["which"] = sdlEvent.wheel.which;
        e.data["x"] = static_cast<float>(dir) * sdlEvent.wheel.x;
        e.data["y"] = static_cast<float>(dir) * sdlEvent.wheel.y;
        e.data["intx"] = dir * sdlEvent.wheel.integer_x;
        e.data["inty"] = dir * sdlEvent.wheel.integer_y;
        e.data["window_id"] = sdlEvent.wheel.windowID;
        e.data["mouse_x"] = sdlEvent.wheel.mouse_x;
        e.data["mouse_y"] = sdlEvent.wheel.mouse_y;
        break;
    }
    case SDL_EVENT_MOUSE_ADDED:
    case SDL_EVENT_MOUSE_REMOVED:
        e.data["which"] = sdlEvent.mdevice.which;
        break;
    default:
        break;
    }
}
}  // namespace kn::mouse
