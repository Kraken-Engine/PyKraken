#include "kraken/graphics/Color.hpp"

#include <nanobind/make_iterator.h>
#include <nanobind/nanobind.h>
#include <nanobind/operators.h>
#include <nanobind/stl/string.h>

#include <algorithm>
#include <iomanip>
#include <sstream>

#include "bindings/python/bindings.hpp"

namespace kn::color
{
void _bind(nb::module_& module)
{
    using namespace nb::literals;

    nb::class_<Color>(module, "Color", nb::pooled(KRAKEN_PYTHON_POOL_CAPACITY), R"doc(
Represents an RGBA color.

Each channel (r, g, b, a) is an 8-bit unsigned integer.
    )doc")

        .def(nb::init(), R"doc(
Create a Color with default values (0, 0, 0, 255).
        )doc")
        .def(
            nb::init<uint8_t, uint8_t, uint8_t, uint8_t>(), "r"_a, "g"_a, "b"_a, "a"_a = 255,
            R"doc(
Create a Color from RGBA components.

Args:
    r (int): Red value [0-255].
    g (int): Green value [0-255].
    b (int): Blue value [0-255].
    a (int, optional): Alpha value [0-255]. Defaults to 255.
            )doc"
        )
        .def(
            "__init__", [](Color* self, const std::string& hex) -> void
            { new (self) Color(fromHex(hex)); }, "hex"_a, R"doc(
Create a Color from a hex string.

Args:
    hex (str): Hex color string (with or without '#' prefix).
            )doc"
        )

        .def_rw("r", &Color::r, R"doc(
Red channel value.

Type: int
Range: 0-255 (8-bit unsigned integer)
        )doc")
        .def_rw("g", &Color::g, R"doc(
Green channel value.

Type: int
Range: 0-255 (8-bit unsigned integer)
        )doc")
        .def_rw("b", &Color::b, R"doc(
Blue channel value.

Type: int
Range: 0-255 (8-bit unsigned integer)
        )doc")
        .def_rw("a", &Color::a, R"doc(
Alpha (transparency) channel value.

Type: int
Range: 0-255 (8-bit unsigned integer)
Note: 0 = fully transparent, 255 = fully opaque
        )doc")

        .def_prop_rw(
            "hex", &Color::toHex, [](Color& self, const std::string& hex) { self.fromHex(hex); },
            R"doc(
Get or set the color as a hex string.

When getting, returns an 8-digit hex string in the format "#RRGGBBAA".
When setting, accepts various hex formats (see from_hex for details).

Example:
    color.hex = "#FF00FF"     # Set to magenta
    print(color.hex)          # Returns "#FF00FFFF"
        )doc"
        )
        .def_prop_rw(
            "hsv",
            [](const Color& self) -> nb::typed<nb::tuple, double, double, double, double>
            {
                const auto [h, s, v, a] = self.toHSV();
                return nb::make_tuple(h, s, v, a);
            },
            [](Color& self, const nb::sequence& hsvSeq)
            {
                const size_t seqSize = nb::len(hsvSeq);
                if (seqSize < 3 || seqSize > 4)
                    throw std::invalid_argument("HSV tuple must have 3 or 4 elements.");

                const auto h = nb::cast<double>(hsvSeq[0]);
                const auto s = nb::cast<double>(hsvSeq[1]);
                const auto v = nb::cast<double>(hsvSeq[2]);
                const auto a = seqSize == 4 ? nb::cast<double>(hsvSeq[3]) : 1.0;

                self.fromHSV({h, s, v, a});
            },
            R"doc(
Get or set the color as an HSV tuple.

When getting, returns a tuple of (hue, saturation, value, alpha).
When setting, accepts a tuple of 3 or 4 values.

Values:
    hue (float): Hue angle in degrees (0-360)
    saturation (float): Saturation level (0-1)
    value (float): Brightness/value level (0-1)
    alpha (float): Alpha transparency (0-1), optional

Example:
    color.hsv = (120, 1.0, 1.0)        # Pure green
    color.hsv = (240, 0.5, 0.8, 0.9)   # Light blue with transparency
    h, s, v, a = color.hsv              # Get HSV values
        )doc"
        )
        .def(
            "copy", [](const Color& self) -> Color { return {self.r, self.g, self.b, self.a}; },
            R"doc(
Create a copy of the color.

Returns:
    Color: A new Color object with the same RGBA values.
        )doc"
        )

        .def(
            "__str__",
            [](const Color& c) -> std::string
            {
                return "(" + std::to_string(c.r) + ", " + std::to_string(c.g) + ", " +
                       std::to_string(c.b) + ", " + std::to_string(c.a) + ")";
            }
        )

        .def(
            "__repr__",
            [](const Color& c) -> std::string
            {
                return "Color(" + std::to_string(c.r) + ", " + std::to_string(c.g) + ", " +
                       std::to_string(c.b) + ", " + std::to_string(c.a) + ")";
            }
        )

        .def(
            "__iter__", [](const Color& c) -> nb::iterator
            { return nb::make_iterator(nb::type<Color>(), "iterator", &c.r, &c.r + 4); },
            nb::keep_alive<0, 1>()
        )

        .def(
            "__getitem__",
            [](const Color& c, const size_t i) -> int
            {
                if (i >= 4)
                    throw nb::index_error();
                return *(&c.r + i);
            },
            "index"_a
        )

        .def(
            "__setitem__",
            [](Color& c, const size_t i, const uint8_t value) -> void
            {
                if (i >= 4)
                    throw nb::index_error();
                *(&c.r + i) = value;
            },
            "index"_a, "value"_a
        )

        .def("__len__", [](const Color&) -> int { return 4; })

        .def(nb::self == nb::self)
        .def(nb::self != nb::self)

        .def(-nb::self)

        .def(nb::self * double())
        .def(double() * nb::self)
        .def(nb::self *= double(), nb::rv_policy::none)
        .def(nb::self / double())
        .def(nb::self /= double(), nb::rv_policy::none)

        .def_prop_ro_static(
            "BLACK", [](const nb::object&) { return Color::BLACK; }, "(0, 0, 0, 255)"
        )
        .def_prop_ro_static(
            "WHITE", [](const nb::object&) { return Color::WHITE; }, "(255, 255, 255, 255)"
        )
        .def_prop_ro_static(
            "RED", [](const nb::object&) { return Color::RED; }, "(255, 0, 0, 255)"
        )
        .def_prop_ro_static(
            "GREEN", [](const nb::object&) { return Color::GREEN; }, "(0, 255, 0, 255)"
        )
        .def_prop_ro_static(
            "BLUE", [](const nb::object&) { return Color::BLUE; }, "(0, 0, 255, 255)"
        )
        .def_prop_ro_static(
            "YELLOW", [](const nb::object&) { return Color::YELLOW; }, "(255, 255, 0, 255)"
        )
        .def_prop_ro_static(
            "MAGENTA", [](const nb::object&) { return Color::MAGENTA; }, "(255, 0, 255, 255)"
        )
        .def_prop_ro_static(
            "CYAN", [](const nb::object&) { return Color::CYAN; }, "(0, 255, 255, 255)"
        )
        .def_prop_ro_static(
            "GRAY", [](const nb::object&) { return Color::GRAY; }, "(128, 128, 128, 255)"
        )
        .def_prop_ro_static(
            "DARK_GRAY", [](const nb::object&) { return Color::DARK_GRAY; }, "(64, 64, 64, 255)"
        )
        .def_prop_ro_static(
            "LIGHT_GRAY", [](const nb::object&) { return Color::LIGHT_GRAY; },
            "(192, 192, 192, 255)"
        )
        .def_prop_ro_static(
            "ORANGE", [](const nb::object&) { return Color::ORANGE; }, "(255, 165, 0, 255)"
        )
        .def_prop_ro_static(
            "BROWN", [](const nb::object&) { return Color::BROWN; }, "(165, 42, 42, 255)"
        )
        .def_prop_ro_static(
            "PINK", [](const nb::object&) { return Color::PINK; }, "(255, 192, 203, 255)"
        )
        .def_prop_ro_static(
            "PURPLE", [](const nb::object&) { return Color::PURPLE; }, "(128, 0, 128, 255)"
        )
        .def_prop_ro_static(
            "NAVY", [](const nb::object&) { return Color::NAVY; }, "(0, 0, 128, 255)"
        )
        .def_prop_ro_static(
            "TEAL", [](const nb::object&) { return Color::TEAL; }, "(0, 128, 128, 255)"
        )
        .def_prop_ro_static(
            "OLIVE", [](const nb::object&) { return Color::OLIVE; }, "(128, 128, 0, 255)"
        )
        .def_prop_ro_static(
            "MAROON", [](const nb::object&) { return Color::MAROON; }, "(128, 0, 0, 255)"
        );

    auto subColor = module.def_submodule("color", R"doc(
Color utility functions and predefined color constants.

This module provides functions for color manipulation and conversion,
as well as commonly used color constants for convenience.
    )doc");

    subColor.def("from_hex", [](const std::string& hex) { return fromHex(hex); }, "hex"_a, R"doc(
Create a Color from a hex string.

Supports multiple hex formats:
- "#RRGGBB" - 6-digit hex with full opacity
- "#RRGGBBAA" - 8-digit hex with alpha
- "#RGB" - 3-digit hex (each digit duplicated)
- "#RGBA" - 4-digit hex with alpha (each digit duplicated)

Args:
    hex (str): Hex color string (with or without '#' prefix).

Returns:
    Color: New Color object from the hex string.

Examples:
    from_hex("#FF00FF")      # Magenta, full opacity
    from_hex("#FF00FF80")    # Magenta, 50% opacity
    from_hex("#F0F")         # Same as "#FF00FF"
    from_hex("RGB")          # Without '#' prefix
        )doc");

    subColor.def(
        "from_hsv", [](const double h, const double s, const double v, const double a)
        { return fromHSV({h, s, v, a}); }, "h"_a, "s"_a, "v"_a, "a"_a = 1.0, R"doc(
Create a Color from HSV(A) values.

Args:
    h (float): Hue angle (0-360).
    s (float): Saturation (0-1).
    v (float): Value/brightness (0-1).
    a (float, optional): Alpha (0-1). Defaults to 1.0.
        )doc"
    );

    subColor.def("lerp", &lerp, "a"_a, "b"_a, "t"_a, R"doc(
Linearly interpolate between two colors.

Performs component-wise linear interpolation between start and end colors.
All RGBA channels are interpolated independently.

Args:
    a (Color): Start color (when t=0.0).
    b (Color): End color (when t=1.0).
    t (float): Blend factor. Values outside [0,1] will extrapolate.

Returns:
    Color: New interpolated color.

Examples:
    lerp(Color.RED, Color.BLUE, 0.5)    # Purple (halfway between red and blue)
    lerp(Color.BLACK, Color.WHITE, 0.25) # Dark gray
        )doc");

    subColor.def("invert", &invert, "color"_a, R"doc(
Return the inverse of a color by flipping RGB channels.

The alpha channel is preserved unchanged.

Args:
    color (Color): The color to invert.

Returns:
    Color: New Color with inverted RGB values (255 - original value).

Example:
    invert(Color(255, 0, 128, 200))  # Returns Color(0, 255, 127, 200)
        )doc");

    subColor.def("grayscale", &grayscale, "color"_a, R"doc(
Convert a color to grayscale.

Args:
    color (Color): The color to convert.

Returns:
    Color: New Color object representing the grayscale version.

Example:
    grayscale(Color(255, 0, 0))  # Returns Color(76, 76, 76, 255)
        )doc");
}
}  // namespace kn::color
