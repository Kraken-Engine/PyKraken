#include "kraken/graphics/Text.hpp"

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

#include <algorithm>
#include <cmath>
#include <mutex>
#include <stdexcept>
#include <vector>

#include "bindings/python/bindings.hpp"
#include "kraken/core/Log.hpp"
#include "kraken/geometry/Rect.hpp"
#include "kraken/graphics/Camera.hpp"
#include "kraken/graphics/Font.hpp"
#include "kraken/graphics/Renderer.hpp"

namespace kn::text
{
void _bind(nb::module_& module)
{
    using namespace nb::literals;

    nb::class_<Text>(module, "Text", R"doc(
A text object for rendering text to the active renderer.

This class handles the rendered text instance. You must provide a Font object
when creating a Text instance.

Note:
    A window/renderer must be created before using text. Typically you should
    call kn.window.create(...) first, which initializes the text engine.
    )doc")
        .def(
            nb::init<const Font&, const std::string&>(), "font"_a, "text"_a = "",
            R"doc(
Create a Text object. The font object must remain in-scope for the lifetime of the Text.

Args:
    font (Font): The font to use for rendering this text.
    text (str): The initial text string (optional, defaults to empty).

Raises:
    RuntimeError: If text creation fails.
    )doc"
        )

        .def_rw("shadow_color", &Text::shadowColor, R"doc(
Get or set the shadow color for the text.
        )doc")
        .def_rw("shadow_offset", &Text::shadowOffset, R"doc(
Get or set the shadow offset for the text.
        )doc")

        .def_prop_rw("wrap_width", &Text::getWrapWidth, &Text::setWrapWidth, R"doc(
Get or set the wrap width in pixels for text wrapping.

Set to 0 to disable wrapping. Negative values are clamped to 0.
        )doc")
        .def_prop_rw("text", &Text::getText, &Text::setText, R"doc(
Get or set the text string to be rendered.
        )doc")
        .def_prop_rw("color", &Text::getColor, &Text::setColor, R"doc(
Get or set the color of the rendered text.
        )doc")
        .def_prop_ro("size", &Text::getSize, R"doc(
Get the size (width, height) of the current text as a Vec2.

Returns:
    Vec2: The text dimensions.
        )doc")
        .def_prop_ro("width", &Text::getWidth, R"doc(
Get the width in pixels of the current text.

Returns:
    int: The text width.
        )doc")
        .def_prop_ro("height", &Text::getHeight, R"doc(
Get the height in pixels of the current text.

Returns:
    int: The text height.
        )doc")

        .def("draw", &Text::draw, "pos"_a = Vec2{}, "anchor"_a = Anchor::TOP_LEFT, R"doc(
Draw the text to the renderer at the specified position with alignment.
A shadow is drawn if shadow_color.a > 0 and shadow_offset is not (0, 0).

Args:
    pos (Vec2 | None): The position in pixels. Defaults to (0, 0).
    anchor (Vec2 | None): The anchor point for alignment (0.0-1.0). Defaults to top left (0, 0).

Raises:
    RuntimeError: If the renderer is not initialized or text drawing fails.
    RuntimeError: If the text font is not set or has gone out of scope.
        )doc")
        .def("set_font", &Text::setFont, "font"_a, R"doc(
Set the font to use for rendering this text.

Args:
    font (Font): The font to use.
        )doc")
        .def("get_rect", &Text::getRect, R"doc(
Get the bounding rectangle of the current text.

Returns:
    Rect: A rectangle with x=0, y=0, and width/height of the text.
    )doc");
}
}  // namespace kn::text
