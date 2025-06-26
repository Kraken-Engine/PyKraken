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
                  py::arg("scaled") = false,
                  "Create a window with (width, height), optional title, and auto-scaling mode");
    subWindow.def("is_open", &window::isOpen, "Check if the window is open");
    subWindow.def("close", &window::close, "Close the window");
    subWindow.def("set_fullscreen", &window::setFullscreen, py::arg("fullscreen"),
                  "Set the fullscreen mode of the window");
    subWindow.def("is_fullscreen", &window::isFullscreen,
                  "Check if the window is in fullscreen mode");
    subWindow.def(
        "get_size",
        []() -> py::tuple
        {
            math::Vec2 winSize = getSize();
            return py::make_tuple(winSize.x, winSize.y);
        },
        "Get the current size of the window");
    subWindow.def("get_title", &window::getTitle, "Get the current title of the window");
    subWindow.def("set_title", &window::setTitle, py::arg("title"), "Set the title of the window");
}

SDL_Window* getWindow() { return _window; }

void create(const std::string& title, const math::Vec2& size, const bool scaled)
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

math::Vec2 getSize()
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
