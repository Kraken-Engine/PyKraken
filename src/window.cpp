#include "Window.hpp"
#include "Color.hpp"
#include "Math.hpp"

#include <SDL3/SDL.h>
#include <stdexcept>

static SDL_Window* _window = nullptr;
static SDL_Renderer* _renderer = nullptr;
static bool _isOpen = false;

namespace window
{
void _bind(pybind11::module_& module)
{
    auto subWindow = module.def_submodule("window", "Window related functions");
    subWindow.def(
        "create",
        [](const py::sequence& res, const py::str& title, py::bool_ scaled)
        {
            if (res.size() != 2)
                throw std::invalid_argument("Resolution must be a tuple of (width, height)");

            const auto width = res[0].cast<double>();
            const auto height = res[1].cast<double>();
            if (width <= 0.0 || height <= 0.0)
                throw std::invalid_argument("Resolution values must be greater than 0");

            create({width, height}, title.cast<std::string>(), scaled);
        },
        py::arg("resolution"), py::arg("title") = "Kraken Engine", py::arg("scaled") = false,
        "Create a window with (width, height), optional title, and auto-scaling mode");
    subWindow.def("is_open", &window::isOpen, "Check if the window is open");
    subWindow.def("close", &window::close, "Close the window");
    subWindow.def(
        "clear",
        [](const py::sequence& color)
        {
            if (py::isinstance<Color>(color))
            {
                clear(color.cast<Color>());
                return;
            }

            if (color.size() != 3 || color.size() != 4)
                throw std::invalid_argument(
                    "Color must be a sequence of (r, g, b) or (r, g, b, a)");

            const auto r = color[0].cast<uint8_t>();
            const auto g = color[1].cast<uint8_t>();
            const auto b = color[2].cast<uint8_t>();

            clear({r, g, b});
        },
        py::arg("color") = py::make_tuple(0, 0, 0),
        "Clear the window with the specified color (r, g, b)");
    subWindow.def("flip", &window::flip, "Flip the render buffer");
    subWindow.def("get_scale", &window::getScale, "Get the current scale of the renderer");
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

SDL_Renderer* getRenderer() { return _renderer; }

void create(const math::Vec2& resolution, const std::string& title, const bool scaled)
{
    if (_window || _renderer)
        throw std::runtime_error("Window or renderer already created");

    const auto resW = static_cast<int>(resolution.x);
    const auto resH = static_cast<int>(resolution.y);

    if (resW <= 0 || resH <= 0)
        throw std::invalid_argument("Resolution values must be greater than 0");

    int winW = resW;
    int winH = resH;
    if (scaled)
    {
        SDL_Rect usableBounds;
        if (!SDL_GetDisplayUsableBounds(SDL_GetPrimaryDisplay(), &usableBounds))
            throw std::runtime_error(SDL_GetError());

        const int scaleX = usableBounds.w / resW;
        const int scaleY = usableBounds.h / resH;
        const int scale = std::max(1, std::min(scaleX, scaleY));

        winW = resW * scale;
        winH = resH * scale;
    }

    SDL_CreateWindowAndRenderer(title.c_str(), winW, winH, 0, &_window, &_renderer);
    if (!_window || !_renderer)
        throw std::runtime_error(SDL_GetError());

    if (scaled)
    {
        if (!SDL_SetRenderLogicalPresentation(_renderer, resW, resH,
                                              SDL_LOGICAL_PRESENTATION_LETTERBOX))
            throw std::runtime_error(SDL_GetError());
    }

    SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);

    _isOpen = true;
}

bool isOpen() { return _isOpen; }

void close() { _isOpen = false; }

void clear(const Color& color)
{
    if (!_renderer)
        throw std::runtime_error("Renderer not initialized");

    if (color.r < 0 || color.r > 255 || color.g < 0 || color.g > 255 || color.b < 0 ||
        color.b > 255)
        throw std::invalid_argument("Color values must be between 0 and 255");

    SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, 255);
    SDL_RenderClear(_renderer);
}

void flip() { SDL_RenderPresent(_renderer); }

py::tuple getSize()
{
    if (!_window)
        throw std::runtime_error("Window not initialized");

    int w, h;
    SDL_GetRenderLogicalPresentation(_renderer, &w, &h, nullptr);
    if (!w || !h)
        SDL_GetWindowSize(_window, &w, &h);

    return py::make_tuple(w, h);
}

float getScale()
{
    if (!_renderer)
        throw std::runtime_error("Renderer not initialized");

    int winScale, renScale;
    SDL_GetWindowSize(_window, &winScale, nullptr);
    SDL_GetRenderLogicalPresentation(_renderer, &renScale, nullptr, nullptr);

    return static_cast<float>(winScale) / static_cast<float>(renScale);
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
    if (_window || _renderer)
        return;

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
        throw std::runtime_error(SDL_GetError());
}

void quit()
{
    if (_renderer)
        SDL_DestroyRenderer(_renderer);
    if (_window)
        SDL_DestroyWindow(_window);

    _renderer = nullptr;
    _window = nullptr;

    SDL_Quit();
}