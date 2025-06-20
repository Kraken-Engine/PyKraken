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
    subWindow.def("create", &window::create, py::arg("title"), py::arg("scaled") = false,
                  py::arg("size") = py::make_tuple(800, 600),
                  "Create a window with (width, height), optional title, and auto-scaling mode");
    subWindow.def("is_open", &window::isOpen, "Check if the window is open");
    subWindow.def("close", &window::close, "Close the window");
    subWindow.def("set_fullscreen", &window::setFullscreen, py::arg("fullscreen"),
                  "Set the fullscreen mode of the window");
    subWindow.def("is_fullscreen", &window::isFullscreen,
                  "Check if the window is in fullscreen mode");
    subWindow.def("get_size", &window::getSize, "Get the current size of the window");
    subWindow.def("get_title", &window::getTitle, "Get the current title of the window");
    subWindow.def("set_title", &window::setTitle, py::arg("title"), "Set the title of the window");
    subWindow.def("set_window_icon", &window::setTitle, py::arg("window, icon"),
                  "Set the icon of the window");
}

SDL_Window* getWindow() { return _window; }

void create(const std::string& title, const bool scaled, const py::object& sizeObj)
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
        if (py::isinstance<math::Vec2>(sizeObj))
        {
            auto sizeVec = sizeObj.cast<math::Vec2>();
            winW = static_cast<int>(sizeVec.x);
            winH = static_cast<int>(sizeVec.y);
        }
        else if (py::isinstance<py::sequence>(sizeObj))
        {
            auto sizeSeq = sizeObj.cast<py::sequence>();
            if (sizeSeq.size() != 2)
                throw std::invalid_argument("Size sequence must be of length 2");
            winW = sizeSeq[0].cast<int>();
            winH = sizeSeq[1].cast<int>();
        }
        else
            throw std::invalid_argument("Size must be a Vec2 or a sequence of two integers");

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

py::tuple getSize()
{
    if (!_window)
        throw std::runtime_error("Window not initialized");

    int w, h;
    SDL_GetWindowSize(_window, &w, &h);

    return py::make_tuple(w, h);
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
void setWindowIcon(SDL_Window* window, SDL_Surface* icon)
{
    if (!SDL_SetWindowIcon(window, icon))
    {
        throw std::runtime_error(SDL_GetError());
    };
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
