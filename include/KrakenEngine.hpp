#pragma once

#include <stdexcept>
#include <string>

#include "AnimationController.hpp"
#include "Camera.hpp"
#include "Capsule.hpp"
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
#include "Shaders.hpp"
#include "Text.hpp"
#include "Texture.hpp"
#include "TileMap.hpp"
#include "Time.hpp"
#include "Transform.hpp"
#include "Viewport.hpp"
#include "Window.hpp"
#include "_globals.hpp"
#include "physics/World.hpp"
#include "physics/bodies/Body.hpp"
#include "physics/bodies/CharacterBody.hpp"
#include "physics/bodies/RigidBody.hpp"
#include "physics/bodies/StaticBody.hpp"
#include "physics/joints/DistanceJoint.hpp"
#include "physics/joints/FilterJoint.hpp"
#include "physics/joints/Joint.hpp"
#include "physics/joints/MotorJoint.hpp"
#include "physics/joints/MouseJoint.hpp"
#include "physics/joints/PrismaticJoint.hpp"
#include "physics/joints/RevoluteJoint.hpp"
#include "physics/joints/WeldJoint.hpp"
#include "physics/joints/WheelJoint.hpp"
#include "ui/UI.hpp"

#define KN_VERSION_MAJOR 1
#define KN_VERSION_MINOR 7
#define KN_VERSION_PATCH 1

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
        kn::log::_init();

    // Log platform and architecture at startup for diagnostics.
    kn::log::info("Platform: {} ({})", detail::getPlatform(), detail::getArchitecture());
    kn::log::info("Kraken Engine v{}.{}.{}", KN_VERSION_MAJOR, KN_VERSION_MINOR, KN_VERSION_PATCH);

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
        throw std::runtime_error(std::string("Failed to initialize SDL: ") + SDL_GetError());

    kn::mixer::_init();
}

inline void quit()
{
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
