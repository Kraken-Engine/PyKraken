#include "Camera.hpp"
#include "Circle.hpp"
#include "Color.hpp"
#include "Constants.hpp"
#include "Ease.hpp"
#include "Event.hpp"
#include "Gamepad.hpp"
#include "Input.hpp"
#include "Key.hpp"
#include "Line.hpp"
#include "Math.hpp"
#include "Mouse.hpp"
#include "Rect.hpp"
#include "Renderer.hpp"
#include "Surface.hpp"
#include "Texture.hpp"
#include "Time.hpp"
#include "Transform.hpp"
#include "Window.hpp"

PYBIND11_MODULE(_core, m)
{
    m.def("init", &init, R"doc(
Initialize the Kraken Engine.

This sets up internal systems and must be called before using any other features.
    )doc");

    m.def("quit", &quit, R"doc(
Shut down the Kraken Engine and clean up resources.

Call this once you're done using the engine to avoid memory leaks.
    )doc");

    constants::_bind(m);
    math::_bind(m);
    rect::_bind(m);

    camera::_bind(m);
    circle::_bind(m);
    color::_bind(m);
    ease::_bind(m);
    event::_bind(m);
    gamepad::_bind(m);
    input::_bind(m);
    key::_bind(m);
    line::_bind(m);
    mouse::_bind(m);
    renderer::_bind(m);
    surface::_bind(m);
    texture::_bind(m);
    kn::time::_bind(m);
    transform::_bind(m);
    window::_bind(m);
}
