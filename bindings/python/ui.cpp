#include "kraken/ui/UI.hpp"

#include <nanobind/nanobind.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/string.h>

#include <stack>
#include <unordered_map>
#include <vector>

#include "bindings/python/bindings.hpp"
#include "kraken/core/Log.hpp"
#include "kraken/geometry/Collision.hpp"
#include "kraken/graphics/Camera.hpp"
#include "kraken/graphics/Draw.hpp"
#include "kraken/graphics/Renderer.hpp"
#include "kraken/graphics/Text.hpp"
#include "kraken/input/Input.hpp"
#include "kraken/input/Mouse.hpp"

namespace kn::ui
{
void _bind(nb::module_& m)
{
    using namespace nb::literals;

    auto subUI = m.def_submodule("ui", "A declarative UI and layout submodule.");

    nb::enum_<Direction>(subUI, "Direction")
        .value("HORIZONTAL", Direction::Horizontal, "Layout children horizontally.")
        .value("VERTICAL", Direction::Vertical, "Layout children vertically.");

    nb::enum_<Align>(subUI, "Align")
        .value("START", Align::Start, "Align children to the start of the container.")
        .value("CENTER", Align::Center, "Align children to the center of the container.")
        .value("END", Align::End, "Align children to the end of the container.")
        .value("STRETCH", Align::Stretch, "Stretch children to fill the container.");

    nb::class_<Style>(
        subUI, "Style",
        R"doc(
Container for UI appearance, layout, and sizing settings.

Styles can provide background textures or colors, text settings, spacing,
borders, and optional fixed width and height constraints.
        )doc"
    )
        .def(
            nb::init<
                std::optional<Color>, const Texture*, Rect, Vec2, const Font*, std::optional<Color>,
                double, double, double, int, double, std::optional<Color>, std::optional<double>,
                std::optional<double>>(),
            "background_color"_a = nb::none(), "texture"_a = nb::none(), "slice"_a = Rect{},
            "offset"_a = Vec2{0.0, 0.0}, "font"_a = nb::none(), "text_color"_a = nb::none(),
            "padding"_a = 0.0, "margin"_a = 0.0, "gap"_a = 0.0, "border_width"_a = 0,
            "border_radius"_a = 0.0, "border_color"_a = nb::none(), "width"_a = nb::none(),
            "height"_a = nb::none(), R"doc(
Create a new style definition for UI elements.

Args:
    background_color (Color, optional): The background color of the element if no texture is used.
    texture (Texture, optional): The texture to draw behind the element.
    slice (Rect, optional): The 9-slice margins used when drawing the texture.
    offset (Vec2, optional): The element offset relative to its parent. Defaults to (0, 0).
    font (Font, optional): The font to use for text rendering.
    text_color (Color, optional): The text color.
    padding (float, optional): Inner padding around the content. Defaults to 0.0.
    margin (float, optional): Outer margin around the element. Defaults to 0.0.
    gap (float, optional): Space between child elements. Defaults to 0.0.
    border_width (int, optional): Border thickness. Defaults to 0.
    border_radius (float, optional): Radius used for rounded corners. Defaults to 0.0.
    border_color (Color, optional): Border color.
    width (float, optional): Fixed element width.
    height (float, optional): Fixed element height.
        )doc"
        )
        .def_rw(
            "background_color", &Style::backgroundColor,
            "The background color of the element (if no texture is provided)."
        )
        .def_rw("texture", &Style::texture, "The texture to use for the element.")
        .def_rw("slice", &Style::slice, "The region of the texture to use.")
        .def_rw("offset", &Style::offset, "The offset of the element within its parent.")
        .def_rw("font", &Style::font, "The font to use for text rendering.")
        .def_rw("text_color", &Style::textColor, "The color of the text.")
        .def_rw("padding", &Style::padding, "The padding around the element.")
        .def_rw("margin", &Style::margin, "The margin around the element.")
        .def_rw("gap", &Style::gap, "The gap between child elements.")
        .def_rw("border_width", &Style::borderWidth, "The width of the border.")
        .def_rw("border_radius", &Style::borderRadius, "The radius of the border corners.")
        .def_rw("border_color", &Style::borderColor, "The color of the border.")
        .def_rw("width", &Style::width, "The width of the element.")
        .def_rw("height", &Style::height, "The height of the element.");

    nb::class_<Context>(subUI, "_Context")
        .def("__enter__", [](Context& self) { return &self; })
        .def(
            "__exit__", [](Context& self, nb::handle, nb::handle, nb::handle) { self.exit(); },
            nb::arg("type").none(), nb::arg("value").none(), nb::arg("traceback").none()
        );

    nb::class_<RootContext>(subUI, "_RootContext")
        .def("__enter__", [](RootContext& self) { return &self; })
        .def(
            "__exit__", [](RootContext& self, nb::handle, nb::handle, nb::handle) { self.exit(); },
            nb::arg("type").none(), nb::arg("value").none(), nb::arg("traceback").none()
        );

    subUI.def(
        "root", &root, "bounds"_a, "direction"_a = Direction::Vertical, "align"_a = Align::Start,
        "justify"_a = Align::Start, R"doc(
Create a root UI context for the current frame.

The root defines the available bounds for the UI tree and controls the
default layout direction and alignment. Use it as a context manager.

Args:
    bounds (Rect): The available region for the UI.
    direction (Direction, optional): The root layout direction. Defaults to Vertical.
    align (Align, optional): Cross-axis alignment for root children. Defaults to Start.
    justify (Align, optional): Main-axis justification for root children. Defaults to Start.

Returns:
    _RootContext: A context manager that finalizes the UI when exited.
        )doc"
    );

    subUI.def(
        "row", &row, "style"_a = nb::none(), "gap"_a = 0.0, "padding"_a = 0.0,
        "align"_a = Align::Start, "justify"_a = Align::Start, R"doc(
Create a horizontal container.

Children are laid out left to right unless the active direction or alignment
settings are changed on the returned context.

Args:
    style (Style, optional): Base style for the container.
    gap (float, optional): Space between child elements. Defaults to 0.0.
    padding (float, optional): Inner padding of the container. Defaults to 0.0.
    align (Align, optional): Cross-axis alignment for children. Defaults to Start.
    justify (Align, optional): Main-axis justification for children. Defaults to Start.

Returns:
    _Context: A context manager for the row.
        )doc"
    );

    subUI.def(
        "column", &column, "style"_a = nb::none(), "gap"_a = 0.0, "padding"_a = 0.0,
        "align"_a = Align::Start, "justify"_a = Align::Start, R"doc(
Create a vertical container.

Children are laid out top to bottom unless alignment or spacing overrides are
applied on the returned context.

Args:
    style (Style, optional): Base style for the container.
    gap (float, optional): Space between child elements. Defaults to 0.0.
    padding (float, optional): Inner padding of the container. Defaults to 0.0.
    align (Align, optional): Cross-axis alignment for children. Defaults to Start.
    justify (Align, optional): Main-axis justification for children. Defaults to Start.

Returns:
    _Context: A context manager for the column.
        )doc"
    );

    subUI.def(
        "stack", &stack, "style"_a = nb::none(), "padding"_a = 0.0, "align"_a = Align::Start,
        "justify"_a = Align::Start, R"doc(
Create an overlapping container.

Stack children share the same container space instead of being arranged in a
linear flow.

Args:
    style (Style, optional): Base style for the container.
    padding (float, optional): Inner padding of the container. Defaults to 0.0.
    align (Align, optional): Alignment for children inside the stack. Defaults to Start.
    justify (Align, optional): Justification for children inside the stack. Defaults to Start.

Returns:
    _Context: A context manager for the stack.
        )doc"
    );

    subUI.def("panel", &panel, "style"_a = Style{}, R"doc(
Create a non-interactive container element.

Panels draw their background, texture, and borders using the provided style.
        )doc");

    subUI.def("button", &button, "text"_a, "style"_a = Style{}, R"doc(
Create a clickable text button.

The button is evaluated against the previous frame's bounds and returns True
when it is clicked during the current frame.

Args:
    text (str): The label displayed on the button.
    style (Style, optional): Visual and layout settings for the button.

Returns:
    bool: True if the button was clicked this frame, otherwise False.
        )doc");

    subUI.def("label", &label, "text"_a, "style"_a = Style{}, R"doc(
Create a non-interactive text label.

Args:
    text (str): The text to display.
    style (Style, optional): Visual and layout settings for the label.
        )doc");
}
}  // namespace kn::ui
