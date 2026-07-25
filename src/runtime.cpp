#include "kraken/Runtime.hpp"

#include <SDL3/SDL.h>

#include <stdexcept>
#include <string>

#include "kraken/audio/Mixer.hpp"
#include "kraken/core/Log.hpp"
#include "kraken/graphics/Draw.hpp"
#include "kraken/graphics/Font.hpp"
#include "kraken/graphics/Renderer.hpp"
#include "kraken/graphics/Shaders.hpp"
#include "kraken/graphics/Text.hpp"
#include "kraken/graphics/Window.hpp"

namespace kn
{
void init(const bool debug)
{
    if (debug)
        kn::log::enable();

    // Log platform and architecture at startup for diagnostics.
    kn::log::info("Platform: {} ({})", detail::getPlatform(), detail::getArchitecture());
    kn::log::info("Kraken Engine v{}.{}.{}", KN_VERSION_MAJOR, KN_VERSION_MINOR, KN_VERSION_PATCH);

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
        throw std::runtime_error(std::string("Failed to initialize SDL: ") + SDL_GetError());

    kn::mixer::_init();
}

void quit()
{
    kn::log::disable();

    // Text objects must be destroyed first (they reference fonts and text engine).
    kn::text::_quit();

    // Fonts must be destroyed and TTF shut down (after text is cleaned up).
    kn::font::_quit();

    // Shader states must be destroyed before renderer/GPU device.
    kn::shaders::_quit();

    // Mixer is independent.
    kn::mixer::_quit();

    // Invalidate cached draw renderer before renderer destruction.
    kn::draw::_init(nullptr);

    // Renderer must be destroyed before window.
    kn::renderer::_quit();

    // Window cleanup.
    kn::window::_quit();

    if (SDL_WasInit(0))
        SDL_Quit();
}
}  // namespace kn
