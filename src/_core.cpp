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
#include "Mask.hpp"
#include "Math.hpp"
#include "Mixer.hpp"
#include "Mouse.hpp"
#include "PixelArray.hpp"
#include "Polygon.hpp"
#include "Rect.hpp"
#include "Renderer.hpp"
#include "ShaderState.hpp"
#include "Text.hpp"
#include "Texture.hpp"
#include "TileMap.hpp"
#include "Time.hpp"
#include "Transform.hpp"
#include "Window.hpp"

PYBIND11_MODULE(_core, m)
{
    m.def("init", &kn::init, R"doc(
Initialize the Kraken Engine.

This sets up internal systems and must be called before using any other features.
    )doc");

    m.def("quit", &kn::quit, R"doc(
Shut down the Kraken Engine and clean up resources.

Call this once you're done using the engine to avoid memory leaks.
    )doc");

    // These are ordered based on dependencies
    kn::color::_bind(m);
    kn::constants::_bind(m);
    kn::math::_bind(m);
    kn::rect::_bind(m);
    kn::pixel_array::_bind(m);
    kn::texture::_bind(m);
    kn::polygon::_bind(m);
    kn::camera::_bind(m);
    kn::line::_bind(m);
    kn::circle::_bind(m);
    kn::collision::_bind(m);
    kn::ease::_bind(m);
    kn::event::_bind(m);
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
    kn::transform::_bind(m);
    kn::window::_bind(m);
    kn::draw::_bind(m);
    kn::animation_controller::_bind(m);
    kn::tile_map::_bind(m);
    kn::shader_state::_bind(m);
}
