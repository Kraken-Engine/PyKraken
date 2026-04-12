// clang-format off
#include "opaque_types.hpp"
// clang-format on

#include "KrakenEngine.hpp"

NB_MODULE(_pykraken, m)
{
    using namespace nb::literals;

    m.def("init", &kn::init, "debug"_a = false, R"doc(
Initialize the Kraken engine subsystems.

Args:
    debug (bool): When True, enables logging outputs.

Raises:
    RuntimeError: If SDL initialization fails.
    )doc");

    m.def("quit", &kn::quit, R"doc(
Tear down the Kraken engine subsystems.
    )doc");

    // These are ordered based on dependencies
    kn::color::_bind(m);
    kn::constants::_bind(m);
    kn::math::_bind(m);
    kn::transform::_bind(m);
    kn::rect::_bind(m);
    kn::pixel_array::_bind(m);
    kn::texture::_bind(m);
    kn::polygon::_bind(m);
    kn::camera::_bind(m);
    kn::line::_bind(m);
    kn::circle::_bind(m);
    kn::capsule::_bind(m);
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
    kn::ui::_bind(m);
    kn::tilemap::_bind(m);
    kn::physics::_bind(m);
    kn::shader_state::_bind(m);
    kn::viewport::_bind(m);
}
