#pragma once

#include <stdexcept>
#include <string>

#include "kraken/animation/AnimationController.hpp"
#include "kraken/animation/Ease.hpp"
#include "kraken/animation/Orchestrator.hpp"
#include "kraken/audio/Mixer.hpp"
#include "kraken/core/Constants.hpp"
#include "kraken/core/Log.hpp"
#include "kraken/core/Time.hpp"
#include "kraken/core/_globals.hpp"
#include "kraken/geometry/Capsule.hpp"
#include "kraken/geometry/Circle.hpp"
#include "kraken/geometry/Collision.hpp"
#include "kraken/geometry/Line.hpp"
#include "kraken/geometry/Polygon.hpp"
#include "kraken/geometry/Rect.hpp"
#include "kraken/graphics/Camera.hpp"
#include "kraken/graphics/Color.hpp"
#include "kraken/graphics/Draw.hpp"
#include "kraken/graphics/Font.hpp"
#include "kraken/graphics/Mask.hpp"
#include "kraken/graphics/PixelArray.hpp"
#include "kraken/graphics/Renderer.hpp"
#include "kraken/graphics/Shaders.hpp"
#include "kraken/graphics/Text.hpp"
#include "kraken/graphics/Texture.hpp"
#include "kraken/graphics/Viewport.hpp"
#include "kraken/graphics/Window.hpp"
#include "kraken/input/Event.hpp"
#include "kraken/input/Gamepad.hpp"
#include "kraken/input/Input.hpp"
#include "kraken/input/Key.hpp"
#include "kraken/input/Mouse.hpp"
#include "kraken/math/Math.hpp"
#include "kraken/math/Transform.hpp"
#include "kraken/physics/World.hpp"
#include "kraken/physics/bodies/Body.hpp"
#include "kraken/physics/bodies/CharacterBody.hpp"
#include "kraken/physics/bodies/RigidBody.hpp"
#include "kraken/physics/bodies/StaticBody.hpp"
#include "kraken/physics/joints/DistanceJoint.hpp"
#include "kraken/physics/joints/FilterJoint.hpp"
#include "kraken/physics/joints/Joint.hpp"
#include "kraken/physics/joints/MotorJoint.hpp"
#include "kraken/physics/joints/MouseJoint.hpp"
#include "kraken/physics/joints/PrismaticJoint.hpp"
#include "kraken/physics/joints/RevoluteJoint.hpp"
#include "kraken/physics/joints/WeldJoint.hpp"
#include "kraken/physics/joints/WheelJoint.hpp"
#include "kraken/tilemap/TileMap.hpp"
#include "kraken/ui/UI.hpp"

#define KN_VERSION_MAJOR 1
#define KN_VERSION_MINOR 7
#define KN_VERSION_PATCH 4

namespace kn
{
namespace detail
{
inline constexpr const char* getPlatform()
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

inline constexpr const char* getArchitecture()
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
}  // namespace detail

inline void init(const bool debug = false)
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

inline void quit()
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
