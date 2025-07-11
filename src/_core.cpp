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
    // py::options options;
    // options.disable_function_signatures();

    m.def("init", &init, "Initialize Kraken Engine");
    m.def("quit", &quit, "Quit Kraken Engine");

    camera::_bind(m);
    circle::_bind(m);
    color::_bind(m);
    constants::_bind(m);
    ease::_bind(m);
    event::_bind(m);
    gamepad::_bind(m);
    input::_bind(m);
    key::_bind(m);
    line::_bind(m);
    math::_bind(m);
    mouse::_bind(m);
    rect::_bind(m);
    renderer::_bind(m);
    surface::_bind(m);
    texture::_bind(m);
    kn::time::_bind(m);
    transform::_bind(m);
    window::_bind(m);
}
