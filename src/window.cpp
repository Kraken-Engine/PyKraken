#include "Window.hpp"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <stdexcept>

#include "AnimationController.hpp"
#include "Font.hpp"
#include "Log.hpp"
#include "Math.hpp"
#include "Mixer.hpp"
#include "Orchestrator.hpp"
#include "Renderer.hpp"
#include "ShaderState.hpp"
#include "Text.hpp"
#include "Time.hpp"
#include "misc/kraken_icon.h"

namespace kn
{
namespace window
{
static SDL_Window* _window = nullptr;
static bool _isOpen = false;
static int _scale = 1;

SDL_Window* _get()
{
    return _window;
}

void create(const std::string& title, const int width, const int height)
{
    if (_window)
        throw std::runtime_error("Window already created");

    if (title.empty())
        throw std::invalid_argument("Title cannot be empty");
    if (title.size() > 255)
        throw std::invalid_argument("Title cannot exceed 255 characters");

    if (width <= 0 || height <= 0)
        throw std::invalid_argument("Window size values must be greater than 0");

    _window = SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_RESIZABLE);
    if (!_window)
        throw std::runtime_error(SDL_GetError());

    SDL_IOStream* iconStream = SDL_IOFromMem(kraken_icon_png, kraken_icon_png_len);
    if (!iconStream)
        throw std::runtime_error("Failed to create icon stream: " + std::string(SDL_GetError()));

    SDL_Surface* iconSurf = IMG_Load_IO(iconStream, true);
    if (!iconSurf)
        throw std::runtime_error("Failed to load window icon: " + std::string(SDL_GetError()));

    SDL_SetWindowIcon(_window, iconSurf);
    SDL_DestroySurface(iconSurf);

    _isOpen = true;

    renderer::_init(_window, width, height);

    log::info("SDL version: {}.{}.{}", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_MICRO_VERSION);
    log::info(
        "SDL_image version: {}.{}.{}", SDL_IMAGE_MAJOR_VERSION, SDL_IMAGE_MINOR_VERSION,
        SDL_IMAGE_MICRO_VERSION
    );
    font::_init();
    text::_init();
}

bool isOpen()
{
    time::_tick();

    mixer::_tick();
    animation_controller::_tick();
    orchestrator::_tick();

    return _isOpen;
}

void close()
{
    _isOpen = false;
}

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

    if (!SDL_SetWindowFullscreen(_window, fullscreen))
        throw std::runtime_error(std::string("Failed to set fullscreen mode: ") + SDL_GetError());
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

    subWindow.def("create", &create, py::arg("title"), py::arg("width"), py::arg("height"), R"doc(
Create a window with the requested title and resolution.

Args:
    title (str): Non-empty title no longer than 255 characters.
    width (int): The window width, must be positive.
    height (int): The window height, must be positive.

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
    RuntimeError: If the window is not initialized or fullscreen mode cannot be changed.
    )doc");

    subWindow.def("is_fullscreen", &isFullscreen, R"doc(
Check if the window is in fullscreen mode.

Returns:
    bool: True if the window is currently in fullscreen mode.

Raises:
    RuntimeError: If the window is not initialized.
    )doc");

    subWindow.def(
        "get_size", &getSize,
        R"doc(
Get the current size of the window.

Returns:
    tuple[float, float]: The window size as (width, height).

Raises:
    RuntimeError: If the window is not initialized.
    )doc"
    );

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
}  // namespace window
}  // namespace kn
