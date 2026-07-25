#include "kraken/input/Mouse.hpp"

#include <algorithm>

#include "kraken/core/_globals.hpp"
#include "kraken/graphics/Camera.hpp"
#include "kraken/graphics/Renderer.hpp"
#include "kraken/graphics/Window.hpp"
#include "kraken/input/Event.hpp"
#include "kraken/math/Math.hpp"

namespace kn::mouse
{
constexpr size_t MOUSE_BUTTON_COUNT = 5;
static bool _mousePressed[MOUSE_BUTTON_COUNT];
static bool _mouseReleased[MOUSE_BUTTON_COUNT];
static float _mouseRelX = 0.0f;
static float _mouseRelY = 0.0f;

namespace
{
size_t _buttonToIndex(const MouseButton button)
{
    return static_cast<size_t>(button) - 1;
}
}  // namespace

Vec2 getPos()
{
    float windowX;
    float windowY;
    SDL_GetMouseState(&windowX, &windowY);

    // Needed for black bars
    float logicalX;
    float logicalY;
    SDL_RenderCoordinatesFromWindow(renderer::_get(), windowX, windowY, &logicalX, &logicalY);

    // Apply scaling only when primary render target is active.
    if (renderer::_primaryActive())
    {
        const Vec2 virtualScale = renderer::getVirtualScale();
        logicalX /= static_cast<float>(virtualScale.x);
        logicalY /= static_cast<float>(virtualScale.y);
    }

    return camera::screenToWorld({logicalX, logicalY});
}

Vec2 getRel()
{
    float x0, y0, x1, y1;

    SDL_Renderer* r = renderer::_get();

    const float dx = _mouseRelX;
    const float dy = _mouseRelY;
    SDL_RenderCoordinatesFromWindow(r, 0.0f, 0.0f, &x0, &y0);
    SDL_RenderCoordinatesFromWindow(r, dx, dy, &x1, &y1);

    float relX = x1 - x0;
    float relY = y1 - y0;

    if (renderer::_primaryActive())
    {
        const Vec2 virtualScale = renderer::getVirtualScale();
        relX /= static_cast<float>(virtualScale.x);
        relY /= static_cast<float>(virtualScale.y);
    }

    return {relX, relY};
}

bool isPressed(MouseButton button)
{
    return SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON_MASK(static_cast<uint32_t>(button));
}

bool isJustPressed(MouseButton button)
{
    return _mousePressed[_buttonToIndex(button)];
}

bool isJustReleased(MouseButton button)
{
    return _mouseReleased[_buttonToIndex(button)];
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
    _mouseRelX = 0.0f;
    _mouseRelY = 0.0f;
}

void _handleEvents(const SDL_Event& sdlEvent, Event& e)
{
    switch (sdlEvent.type)
    {
    case SDL_EVENT_MOUSE_MOTION:
        _mouseRelX += sdlEvent.motion.xrel;
        _mouseRelY += sdlEvent.motion.yrel;
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
