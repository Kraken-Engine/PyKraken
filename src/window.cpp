#include "AnimationController.hpp"
#include "Font.hpp"
#include "Log.hpp"
#include "Math.hpp"
#include "Mixer.hpp"
#include "Renderer.hpp"
#include "ShaderState.hpp"
#include "Text.hpp"
#include "Time.hpp"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdexcept>

#include "Window.hpp"
#include "misc/kraken_icon.h"

namespace kn
{
namespace window
{
static SDL_Window* _window = nullptr;
static bool _isOpen = false;
static int _scale = 1;

SDL_Window* _get() { return _window; }

void create(const std::string& title, const Vec2& res, const bool scaled, const bool debug)
{
    if (_window)
        throw std::runtime_error("Window already created");

    if (debug)
        log::_init();

    if (title.empty())
        throw std::invalid_argument("Title cannot be empty");
    if (title.size() > 255)
        throw std::invalid_argument("Title cannot exceed 255 characters");

    int winW;
    int winH;
    if (scaled)
    {
        const SDL_DisplayID primaryDisplay = SDL_GetPrimaryDisplay();
        if (primaryDisplay == 0)
        {
            // Fallback: if we can't get primary display, use non-scaled mode
            winW = static_cast<int>(res.x);
            winH = static_cast<int>(res.y);
            _scale = 1;
        }
        else
        {
            SDL_Rect usableBounds;
            if (!SDL_GetDisplayUsableBounds(primaryDisplay, &usableBounds))
            {
                // Fallback: if we can't get bounds, use non-scaled mode
                winW = static_cast<int>(res.x);
                winH = static_cast<int>(res.y);
                _scale = 1;
            }
            else
            {
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
        }
    }
    else
    {
        winW = static_cast<int>(res.x);
        winH = static_cast<int>(res.y);

        if (winW <= 0 || winH <= 0)
            throw std::invalid_argument("Window resolution values must be greater than 0");
    }

    _window = SDL_CreateWindow(title.c_str(), winW, winH, SDL_WINDOW_RESIZABLE);
    if (!_window)
        throw std::runtime_error(SDL_GetError());

    SDL_IOStream* iconStream = SDL_IOFromMem(kraken_icon_png, kraken_icon_png_len);
    SDL_Surface* iconSurf = IMG_Load_IO(iconStream, true);
    SDL_SetWindowIcon(_window, iconSurf);
    SDL_DestroySurface(iconSurf);

    _isOpen = true;

    renderer::_init(_window, res);
    font::_init();
    text::_init();
}

bool isOpen()
{
    time::_tick();

    mixer::_tick();
    animation_controller::_tick();

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
    {
        log::warn("Attempted to set fullscreen on uninitialized window");
        return;
    }

    if (!SDL_SetWindowFullscreen(_window, fullscreen))
    {
        log::warn("Failed to set fullscreen mode: {}", SDL_GetError());
    }
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

void setIcon(const std::string& path)
{
    if (!_window)
        throw std::runtime_error("Window not initialized");

    SDL_Surface* iconSurface = IMG_Load(path.c_str());
    if (!iconSurface)
        throw std::runtime_error("Failed to load icon: " + path);

    SDL_SetWindowIcon(_window, iconSurface);
    SDL_DestroySurface(iconSurface);
}

void saveScreenshot(const std::string& filePath)
{
    if (!_window)
        throw std::runtime_error("Window not initialized");

    SDL_Surface* shotSurface = SDL_RenderReadPixels(renderer::_get(), nullptr);
    if (!shotSurface)
        throw std::runtime_error("Failed to read pixels: " + std::string(SDL_GetError()));

    if (!IMG_SavePNG(shotSurface, filePath.c_str()))
    {
        SDL_DestroySurface(shotSurface);
        throw std::runtime_error("Failed to save screenshot: " + std::string(SDL_GetError()));
    }

    SDL_DestroySurface(shotSurface);
}

void _quit()
{
    if (_window)
    {
        SDL_DestroyWindow(_window);
        _window = nullptr;
    }
}

void _bind(py::module_& module)
{
    auto subWindow = module.def_submodule("window", "Window related functions");

    subWindow.def("create", &create, py::arg("title"), py::arg("resolution"),
                  py::arg("scaled") = false, py::arg("debug") = false, R"doc(
Create a window with the requested title and resolution.

Args:
    title (str): Non-empty title no longer than 255 characters.
    resolution (Vec2): Target renderer resolution as (width, height).
    scaled (bool): When True, stretches to usable display bounds while maintaining aspect.
    debug (bool): When True, initializes logging outputs.

Raises:
    RuntimeError: If a window already exists or SDL window creation fails.
    ValueError: If the title is invalid or any dimension is non-positive.
    )doc");

    subWindow.def("is_open", &isOpen, R"doc(
Report whether the window is currently open.

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

    subWindow.def("set_icon", &setIcon, py::arg("path"), R"doc(
Set the window icon from an image file.

Args:
    path (str): The file path to the image to use as the icon.

Raises:
    RuntimeError: If the window is not initialized or icon setting fails.
    )doc");

    subWindow.def("save_screenshot", &saveScreenshot, py::arg("path"), R"doc(
Save a screenshot of the current frame to a file.

Args:
    path (str): The path to save the screenshot to.

Raises:
    RuntimeError: If the window is not initialized or the screenshot cannot be saved.
)doc");
}
} // namespace window
} // namespace kn
