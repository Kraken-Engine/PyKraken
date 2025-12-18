#include "AnimationController.hpp"
#include "Camera.hpp"
#include "Circle.hpp"
#include "Collision.hpp"
#include "Color.hpp"
#include "Constants.hpp"
#include "Draw.hpp"
#include "Ease.hpp"
#include "Event.hpp"
#include "Font.hpp"
#include "Gamepad.hpp"
#include "Input.hpp"
#include "Key.hpp"
#include "Line.hpp"
#include "Log.hpp"
#include "Mask.hpp"
#include "Math.hpp"
#include "Mixer.hpp"
#include "Mouse.hpp"
#include "Orchestrator.hpp"
#include "PixelArray.hpp"
#include "Polygon.hpp"
#include "Rect.hpp"
#include "Renderer.hpp"
#include "ShaderState.hpp"
#include "Sprite.hpp"
#include "Text.hpp"
#include "Texture.hpp"
#include "TileMap.hpp"
#include "Time.hpp"
#include "Viewport.hpp"
#include "Window.hpp"

constexpr int KRAKEN_MAJOR_VERSION = 1;
constexpr int KRAKEN_MINOR_VERSION = 4;
constexpr int KRAKEN_MICRO_VERSION = 0;

constexpr const char* getPlatform();
constexpr const char* getArchitecture();

static void init(const bool debug = false)
{
    if (debug)
        kn::log::_init();

    kn::log::info("Kraken Engine v{}.{}.{}", KRAKEN_MAJOR_VERSION, KRAKEN_MINOR_VERSION,
                  KRAKEN_MICRO_VERSION);

    // log platform and architecture
    kn::log::info("Platform: {} ({})", getPlatform(), getArchitecture());

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
        throw std::runtime_error(std::string("Failed to initialize SDL: ") + SDL_GetError());

    kn::mixer::_init();
}

static void quit()
{
    // Text objects must be destroyed first (they reference fonts and text engine)
    kn::text::_quit();

    // Fonts must be destroyed and TTF shut down (after text is cleaned up)
    kn::font::_quit();

    // Shader states must be destroyed before renderer/GPU device
    kn::shader_state::_quit();

    // Mixer is independent
    kn::mixer::_quit();

    // Renderer must be destroyed before window
    kn::renderer::_quit();

    // Window cleanup
    kn::window::_quit();

    if (SDL_WasInit(0))
        SDL_Quit();
}

PYBIND11_MODULE(_core, m)
{
    m.def("init", &init, py::arg("debug") = false, R"doc(
Initialize the Kraken engine subsystems.

Args:
    debug (bool): When True, enables logging outputs.

Raises:
    RuntimeError: If SDL initialization fails.
    )doc");
    m.def("quit", &quit, R"doc(
Tear down the Kraken engine subsystems.
    )doc");

    // These are ordered based on dependencies
    kn::color::_bind(m);
    kn::constants::_bind(m);
    kn::math::_bind(m);
    kn::rect::_bind(m);
    kn::pixel_array::_bind(m);
    kn::texture::_bind(m);
    kn::sprite::_bind(m);
    kn::polygon::_bind(m);
    kn::camera::_bind(m);
    kn::line::_bind(m);
    kn::circle::_bind(m);
    kn::collision::_bind(m);
    kn::ease::_bind(m);
    kn::event::_bind(m);
    kn::log::_bind(m);
    kn::font::_bind(m);
    kn::text::_bind(m);
    kn::gamepad::_bind(m);
    kn::input::_bind(m);
    kn::key::_bind(m);
    kn::mask::_bind(m);
    kn::mixer::_bind(m);
    kn::mouse::_bind(m);
    kn::renderer::_bind(m);
    kn::time::_bind(m);
    kn::window::_bind(m);
    kn::draw::_bind(m);
    kn::animation_controller::_bind(m);
    kn::orchestrator::_bind(m);
    kn::tile_map::_bind(m);
    kn::shader_state::_bind(m);
    kn::viewport::_bind(m);
}

constexpr const char* getPlatform()
{
#if defined(_WIN32)
    return "Windows";
#elif defined(__APPLE__)
    return "macOS";
#elif defined(__linux__)
    return "Linux";
#elif defined(__unix__)
    return "Unix";
#else
    return "Unknown OS";
#endif
}

constexpr const char* getArchitecture()
{
#if defined(_M_X64) || defined(__x86_64__)
    return "x64";
#elif defined(_M_IX86) || defined(__i386__) || defined(__i686__)
    return "x86";
#elif defined(_M_ARM64) || defined(__aarch64__)
    return "ARM64";
#elif defined(_M_ARM) || defined(__arm__)
    return "ARM";
#elif defined(__ppc64__) || defined(__PPC64__)
    return "PowerPC64";
#elif defined(__ppc__) || defined(__PPC__)
    return "PowerPC";
#else
    return "Unknown Architecture";
#endif
}
