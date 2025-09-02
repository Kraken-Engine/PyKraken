#include "Math.hpp"
#include "Mixer.hpp"
#include "Renderer.hpp"
#include "Time.hpp"

#include "Window.hpp"
#include <SDL3/SDL.h>
#include <stdexcept>

namespace kn
{
namespace window
{
static SDL_Window* _window = nullptr;
static bool _isOpen = false;
static int _scale = 1;

void _bind(py::module_& module)
{
    auto subWindow = module.def_submodule("window", "Window related functions");

    subWindow.def("create", &create, py::arg("title"), py::arg("resolution"),
                  py::arg("scaled") = false, R"doc(
Create a window with specified title and size.

Args:
    title (str): The window title. Must be non-empty and <= 255 characters.
    resolution (Vec2): The renderer resolution as (width, height).
    scaled (bool, optional): If True, creates a scaled up window using the 
                            display's usable bounds, retaining the resolution's ratio.
                            Defaults to False.

Raises:
    RuntimeError: If a window already exists or window creation fails.
    ValueError: If title is empty, exceeds 255 characters, or size values are <= 0.
    )doc");
    subWindow.def("is_open", &isOpen, R"doc(
Check if the window is open.

Returns:
    bool: True if the window is open and active.
    )doc");
    subWindow.def("close", &close, R"doc(
Close the window.

Marks the window as closed, typically used to signal the main loop to exit.
This doesn't destroy the window immediately but sets the close flag.
    )doc");
    subWindow.def("set_fullscreen", &setFullscreen, py::arg("fullscreen"), R"doc(
Set the fullscreen mode of the window.

Args:
    fullscreen (bool): True to enable fullscreen mode, False for windowed mode.

Raises:
    RuntimeError: If the window is not initialized.
    )doc");
    subWindow.def("is_fullscreen", &isFullscreen, R"doc(
Check if the window is in fullscreen mode.

Returns:
    bool: True if the window is currently in fullscreen mode.

Raises:
    RuntimeError: If the window is not initialized.
    )doc");
    subWindow.def("get_size", &getSize,
                  R"doc(
Get the current size of the window.

Returns:
    tuple[float, float]: The window size as (width, height).

Raises:
    RuntimeError: If the window is not initialized.
    )doc");
    subWindow.def("get_scale", &getScale, R"doc(
Get the scale of the window relative to the renderer resolution.

Returns:
    int: The window's scale

Raises:
    RuntimeError: If the window is not initialized.
    )doc");
    subWindow.def("get_title", &getTitle, R"doc(
Get the current title of the window.

Returns:
    str: The current window title.

Raises:
    RuntimeError: If the window is not initialized.
    )doc");
    subWindow.def("set_title", &setTitle, py::arg("title"), R"doc(
Set the title of the window.

Args:
    title (str): The new window title. Must be non-empty and <= 255 characters.

Raises:
    RuntimeError: If the window is not initialized or title setting fails.
    ValueError: If title is empty or exceeds 255 characters.
    )doc");
}

SDL_Window* _get() { return _window; }

void create(const std::string& title, const Vec2& res, const bool scaled)
{
    if (_window)
        throw std::runtime_error("Window already created");

    if (title.empty())
        throw std::invalid_argument("Title cannot be empty");
    if (title.size() > 255)
        throw std::invalid_argument("Title cannot exceed 255 characters");

    int winW;
    int winH;
    if (scaled)
    {
        SDL_Rect usableBounds;
        if (!SDL_GetDisplayUsableBounds(SDL_GetPrimaryDisplay(), &usableBounds))
            throw std::runtime_error(SDL_GetError());

        // Calculate scale factors for both dimensions
        const double scaleX = usableBounds.w / res.x;
        const double scaleY = usableBounds.h / res.y;

        // Use the smaller scale to maintain an aspect ratio
        const double minScale = scaleX < scaleY ? scaleX : scaleY;
        _scale = static_cast<int>(minScale);
        if (fmod(minScale, 1.0) == 0.0)
            _scale = static_cast<int>(minScale) - 1;

        winW = static_cast<int>(res.x * _scale);
        winH = static_cast<int>(res.y * _scale);
    }
    else
    {
        winW = static_cast<int>(res.x);
        winH = static_cast<int>(res.y);

        if (winW <= 0 || winH <= 0)
            throw std::invalid_argument("Window resolution values must be greater than 0");
    }

    _window = SDL_CreateWindow(title.c_str(), winW, winH, 0);
    if (!_window)
        throw std::runtime_error(SDL_GetError());

    _isOpen = true;

    renderer::_init(_window, res);
}

bool isOpen()
{
    kn::time::_tick();
    return _isOpen;
}

void close() { _isOpen = false; }

Vec2 getSize()
{
    if (!_window)
        throw std::runtime_error("Window not initialized");

    int w, h;
    SDL_GetWindowSize(_window, &w, &h);

    return {w, h};
}

int getScale()
{
    if (!_window)
        throw std::runtime_error("Window not initialized");

    return _scale;
}

void setFullscreen(const bool fullscreen)
{
    if (!_window)
        throw std::runtime_error("Window not initialized");

    SDL_SetWindowFullscreen(_window, fullscreen);
}

bool isFullscreen()
{
    if (!_window)
        throw std::runtime_error("Window not initialized");

    return (SDL_GetWindowFlags(_window) & SDL_WINDOW_FULLSCREEN) == SDL_WINDOW_FULLSCREEN;
}

void setTitle(const std::string& title)
{
    if (!_window)
        throw std::runtime_error("Window not initialized");

    if (title.empty())
        throw std::invalid_argument("Title cannot be empty");

    if (title.size() > 255)
        throw std::invalid_argument("Title cannot exceed 255 characters");

    if (!SDL_SetWindowTitle(_window, title.c_str()))
        throw std::runtime_error(SDL_GetError());
}

std::string getTitle()
{
    if (!_window)
        throw std::runtime_error("Window not initialized");

    const char* title = SDL_GetWindowTitle(_window);

    return {title};
}
} // namespace window

void init()
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
        throw std::runtime_error(SDL_GetError());

    mixer::_init();
}

void quit()
{
    mixer::_quit();
    renderer::_quit();

    if (window::_window)
        SDL_DestroyWindow(window::_window);
    window::_window = nullptr;

    SDL_Quit();
}
} // namespace kn
