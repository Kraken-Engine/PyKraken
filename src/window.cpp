#include "Window.hpp"
#include "Math.hpp"

#include <SDL3/SDL.h>
#include <stdexcept>

static SDL_Window* _window = nullptr;
static bool _isOpen = false;

namespace window
{
void _bind(pybind11::module_& module)
{
    auto subWindow = module.def_submodule("window", "Window related functions");
    subWindow.def("create", &window::create, py::arg("title"), py::arg("size"),
                  py::arg("scaled") = false, R"doc(
Create a window with specified title and size.

Args:
    title (str): The window title. Must be non-empty and <= 255 characters.
    size (Vec2): The window size as (width, height). Ignored if scaled=True.
    scaled (bool, optional): If True, creates a fullscreen window using the 
                            display's usable bounds. Defaults to False.

Raises:
    RuntimeError: If a window already exists or window creation fails.
    ValueError: If title is empty, exceeds 255 characters, or size values are <= 0.
    )doc");
    subWindow.def("is_open", &window::isOpen, R"doc(
Check if the window is open.

Returns:
    bool: True if the window is open and active.
    )doc");
    subWindow.def("close", &window::close, R"doc(
Close the window.

Marks the window as closed, typically used to signal the main loop to exit.
This doesn't destroy the window immediately but sets the close flag.
    )doc");
    subWindow.def("set_fullscreen", &window::setFullscreen, py::arg("fullscreen"), R"doc(
Set the fullscreen mode of the window.

Args:
    fullscreen (bool): True to enable fullscreen mode, False for windowed mode.

Raises:
    RuntimeError: If the window is not initialized.
    )doc");
    subWindow.def("is_fullscreen", &window::isFullscreen, R"doc(
Check if the window is in fullscreen mode.

Returns:
    bool: True if the window is currently in fullscreen mode.

Raises:
    RuntimeError: If the window is not initialized.
    )doc");
    subWindow.def(
        "get_size",
        []() -> py::tuple
        {
            Vec2 winSize = getSize();
            return py::make_tuple(winSize.x, winSize.y);
        },
        R"doc(
Get the current size of the window.

Returns:
    tuple[float, float]: The window size as (width, height).

Raises:
    RuntimeError: If the window is not initialized.
    )doc");
    subWindow.def("get_title", &window::getTitle, R"doc(
Get the current title of the window.

Returns:
    str: The current window title.

Raises:
    RuntimeError: If the window is not initialized.
    )doc");
    subWindow.def("set_title", &window::setTitle, py::arg("title"), R"doc(
Set the title of the window.

Args:
    title (str): The new window title. Must be non-empty and <= 255 characters.

Raises:
    RuntimeError: If the window is not initialized or title setting fails.
    ValueError: If title is empty or exceeds 255 characters.
    )doc");
}

SDL_Window* getWindow() { return _window; }

void create(const std::string& title, const Vec2& size, const bool scaled)
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

        winW = usableBounds.w;
        winH = usableBounds.h;
    }
    else
    {
        winW = static_cast<int>(size.x);
        winH = static_cast<int>(size.y);

        if (winW <= 0 || winH <= 0)
            throw std::invalid_argument("Window size values must be greater than 0");
    }

    _window = SDL_CreateWindow(title.c_str(), winW, winH, 0);
    if (!_window)
        throw std::runtime_error(SDL_GetError());

    _isOpen = true;
}

bool isOpen() { return _isOpen; }

void close() { _isOpen = false; }

Vec2 getSize()
{
    if (!_window)
        throw std::runtime_error("Window not initialized");

    int w, h;
    SDL_GetWindowSize(_window, &w, &h);

    return {w, h};
}

void setFullscreen(bool fullscreen)
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

    return std::string(title);
}
} // namespace window

void init()
{
    if (_window)
        return;

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
        throw std::runtime_error(SDL_GetError());
}

void quit()
{
    if (_window)
        SDL_DestroyWindow(_window);

    _window = nullptr;

    SDL_Quit();
}
