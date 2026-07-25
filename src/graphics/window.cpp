#include "kraken/graphics/Window.hpp"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <stdexcept>

#include "graphics/assets/kraken_icon.h"
#include "kraken/animation/AnimationController.hpp"
#include "kraken/animation/Ease.hpp"
#include "kraken/animation/Orchestrator.hpp"
#include "kraken/audio/Mixer.hpp"
#include "kraken/core/Log.hpp"
#include "kraken/core/Time.hpp"
#include "kraken/graphics/Draw.hpp"
#include "kraken/graphics/Font.hpp"
#include "kraken/graphics/Renderer.hpp"
#include "kraken/graphics/Text.hpp"
#include "kraken/math/Math.hpp"
#include "kraken/physics/World.hpp"

namespace kn
{
namespace window
{
static SDL_Window* _window = nullptr;
static bool _isOpen = false;
static int _scale = 1;
static bool _handleClose = true;

SDL_Window* _get()
{
    return _window;
}

bool _handlesClose()
{
    return _handleClose;
}

void create(const std::string& title, const int width, const int height, const bool handleClose)
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
    draw::_init(renderer::_get());

    log::info("SDL version: {}.{}.{}", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_MICRO_VERSION);
    log::info(
        "SDL_image version: {}.{}.{}", SDL_IMAGE_MAJOR_VERSION, SDL_IMAGE_MINOR_VERSION,
        SDL_IMAGE_MICRO_VERSION
    );
    font::_init();
    text::_init();

    _handleClose = handleClose;
}

bool isOpen()
{
    time::_tick();
    animation_controller::_tick();
    ease::_tick();
    orchestrator::_tick();
    physics::_tick();

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

void setIcon(const std::filesystem::path& path)
{
    if (!_window)
        throw std::runtime_error("Window not initialized");

    SDL_Surface* iconSurface = IMG_Load(path.string().c_str());
    if (!iconSurface)
        throw std::runtime_error("Failed to load icon: " + path.string());

    SDL_SetWindowIcon(_window, iconSurface);
    SDL_DestroySurface(iconSurface);
}

void saveScreenshot(const std::filesystem::path& filePath)
{
    if (!_window)
        throw std::runtime_error("Window not initialized");

    SDL_Surface* shotSurface = SDL_RenderReadPixels(renderer::_get(), nullptr);
    if (!shotSurface)
        throw std::runtime_error("Failed to read pixels: " + std::string(SDL_GetError()));

    if (!IMG_SavePNG(shotSurface, filePath.string().c_str()))
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

}  // namespace window
}  // namespace kn
