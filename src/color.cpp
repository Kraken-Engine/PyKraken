#include "Color.hpp"

#ifdef KRAKEN_ENABLE_PYTHON
#include <nanobind/make_iterator.h>
#include <nanobind/operators.h>
#include <nanobind/stl/string.h>
#endif  // KRAKEN_ENABLE_PYTHON

#include <algorithm>
#include <iomanip>
#include <sstream>

namespace kn
{
void Color::fromHex(const std::string_view hex)
{
    *this = color::fromHex(hex);
}

std::string Color::toHex() const
{
    std::stringstream ss;

    ss << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << static_cast<int>(r)
       << std::setw(2) << static_cast<int>(g) << std::setw(2) << static_cast<int>(b) << std::setw(2)
       << static_cast<int>(a);

    return "#" + ss.str();
}

void Color::fromHSV(const color::HSV& hsv)
{
    *this = color::fromHSV(hsv);
}

color::HSV Color::toHSV() const
{
    double rNorm = r / 255.0;
    double gNorm = g / 255.0;
    double bNorm = b / 255.0;

    const double maxVal = std::max({rNorm, gNorm, bNorm});
    const double minVal = std::min({rNorm, gNorm, bNorm});
    const double delta = maxVal - minVal;

    double h, s;
    const double v = maxVal;

    if (delta < 0.00001f)
    {
        h = 0;  // Undefined hue
        s = 0;
    }
    else
    {
        s = delta / maxVal;

        if (maxVal == rNorm)
            h = (gNorm - bNorm) / delta + (gNorm < bNorm ? 6.0 : 0.0);
        else if (maxVal == gNorm)
            h = (bNorm - rNorm) / delta + 2.0;
        else
            h = (rNorm - gNorm) / delta + 4.0;

        h *= 60.0;  // Convert to degrees
    }

    return {h, s, v, a / 255.0};
}

Color::operator SDL_Color() const
{
    return {r, g, b, a};
}

Color::operator SDL_FColor() const
{
    return {
        static_cast<float>(r) / 255.f, static_cast<float>(g) / 255.f, static_cast<float>(b) / 255.f,
        static_cast<float>(a) / 255.f
    };
}

Color::operator uint32_t() const
{
    return static_cast<uint32_t>(a) << 24 | static_cast<uint32_t>(b) << 16 |
           static_cast<uint32_t>(g) << 8 | static_cast<uint32_t>(r);
}

bool Color::operator==(const Color& other) const
{
    return r == other.r && g == other.g && b == other.b && a == other.a;
}

bool Color::operator!=(const Color& other) const
{
    return !(*this == other);
}

Color Color::operator-() const
{
    return color::invert(*this);
}

Color Color::operator*(const double scalar) const
{
    // Negative scalar inverts the color
    if (scalar < 0.0)
    {
        Color inverted = color::invert(*this);
        return inverted * std::abs(scalar);
    }

    // Clamp results to valid range [0, 255], preserve alpha
    auto clamp = [](double value) -> uint8_t
    { return static_cast<uint8_t>(std::clamp(value, 0.0, 255.0)); };

    return {clamp(r * scalar), clamp(g * scalar), clamp(b * scalar), a};
}

Color& Color::operator*=(const double scalar)
{
    *this = *this * scalar;
    return *this;
}

Color operator*(const double scalar, const Color& color)
{
    return color * scalar;
}

Color Color::operator/(const double scalar) const
{
    if (scalar == 0.0)
        throw std::invalid_argument("Cannot divide by zero");

    // Negative scalar inverts the color
    if (scalar < 0.0)
    {
        Color inverted = color::invert(*this);
        return inverted / std::abs(scalar);
    }

    // Clamp results to valid range [0, 255], preserve alpha
    auto clamp = [](double value) -> uint8_t
    { return static_cast<uint8_t>(std::clamp(value, 0.0, 255.0)); };

    return {clamp(r / scalar), clamp(g / scalar), clamp(b / scalar), a};
}

Color& Color::operator/=(const double scalar)
{
    *this = *this / scalar;
    return *this;
}

const Color Color::BLACK = {0, 0, 0};
const Color Color::WHITE = {255, 255, 255};
const Color Color::RED = {255, 0, 0};
const Color Color::GREEN = {0, 255, 0};
const Color Color::BLUE = {0, 0, 255};
const Color Color::YELLOW = {255, 255, 0};
const Color Color::MAGENTA = {255, 0, 255};
const Color Color::CYAN = {0, 255, 255};
const Color Color::GRAY = {128, 128, 128};
const Color Color::DARK_GRAY = {64, 64, 64};
const Color Color::LIGHT_GRAY = {192, 192, 192};
const Color Color::ORANGE = {255, 165, 0};
const Color Color::BROWN = {139, 69, 19};
const Color Color::PINK = {255, 192, 203};
const Color Color::PURPLE = {128, 0, 128};
const Color Color::NAVY = {0, 0, 128};
const Color Color::TEAL = {0, 128, 128};
const Color(Color::OLIVE) = {128, 128, 0};
const Color(Color::MAROON) = {128, 0, 0};

namespace color
{
Color fromHex(std::string_view hex)
{
    if (hex.empty())
        throw std::invalid_argument("Hex string cannot be empty");

    if (hex[0] == '#')
        hex.remove_prefix(1);

    auto hexToByte = [](const std::string_view str) -> uint8_t
    {
        uint32_t byte;
        std::stringstream ss;
        ss << std::hex << str;
        ss >> byte;
        return static_cast<uint8_t>(byte);
    };

    if (hex.length() == 6)
    {
        // RRGGBB
        return {
            hexToByte(hex.substr(0, 2)), hexToByte(hex.substr(2, 2)), hexToByte(hex.substr(4, 2)),
            255
        };
    }

    if (hex.length() == 8)
    {
        // RRGGBBAA
        return {
            hexToByte(hex.substr(0, 2)), hexToByte(hex.substr(2, 2)), hexToByte(hex.substr(4, 2)),
            hexToByte(hex.substr(6, 2))
        };
    }

    if (hex.length() == 3)
    {
        // RGB → duplicate each
        return {
            hexToByte(std::string(2, hex[0])), hexToByte(std::string(2, hex[1])),
            hexToByte(std::string(2, hex[2])), 255
        };
    }

    if (hex.length() == 4)
    {
        // RGBA → duplicate each
        return {
            hexToByte(std::string(2, hex[0])), hexToByte(std::string(2, hex[1])),
            hexToByte(std::string(2, hex[2])), hexToByte(std::string(2, hex[3]))
        };
    }

    throw std::invalid_argument("Invalid hex string format");
}

Color fromHSV(const HSV& hsv)
{
    if (hsv.s < 0.0 || hsv.s > 1.0 || hsv.v < 0.0 || hsv.v > 1.0 || hsv.a < 0.0 || hsv.a > 1.0)
        throw std::invalid_argument("Saturation, value, and alpha must be in the range [0, 1]");
    if (hsv.h < 0.0 || hsv.h >= 360.0)
        throw std::invalid_argument("Hue must be in the range [0, 360)");

    const double c = hsv.v * hsv.s;
    const double x = c * (1.0 - std::fabs(fmod(hsv.h / 60.0, 2.0) - 1.0));
    const double m = hsv.v - c;

    double r, g, b;

    if (hsv.h < 60.0)
    {
        r = c;
        g = x;
        b = 0.0;
    }
    else if (hsv.h < 120.0)
    {
        r = x;
        g = c;
        b = 0.0;
    }
    else if (hsv.h < 180.0)
    {
        r = 0.0;
        g = c;
        b = x;
    }
    else if (hsv.h < 240.0)
    {
        r = 0.0;
        g = x;
        b = c;
    }
    else if (hsv.h < 300.0)
    {
        r = x;
        g = 0.0;
        b = c;
    }
    else
    {
        r = c;
        g = 0.0;
        b = x;
    }

    return {
        static_cast<uint8_t>((r + m) * 255.0), static_cast<uint8_t>((g + m) * 255.0),
        static_cast<uint8_t>((b + m) * 255.0), static_cast<uint8_t>(hsv.a * 255.0)
    };
}

Color lerp(const Color& a, const Color& b, const double t)
{
    return {
        static_cast<uint8_t>(a.r + (b.r - a.r) * t), static_cast<uint8_t>(a.g + (b.g - a.g) * t),
        static_cast<uint8_t>(a.b + (b.b - a.b) * t), static_cast<uint8_t>(a.a + (b.a - a.a) * t)
    };
}

Color invert(const Color& color)
{
    return {
        static_cast<uint8_t>(255 - color.r), static_cast<uint8_t>(255 - color.g),
        static_cast<uint8_t>(255 - color.b), color.a
    };
}

Color grayscale(const Color& color)
{
    const auto gray = static_cast<uint8_t>(0.299 * color.r + 0.587 * color.g + 0.114 * color.b);
    return {gray, gray, gray, color.a};
}

#ifdef KRAKEN_ENABLE_PYTHON
void _bind(nb::module_& module)
{
    using namespace nb::literals;

    nb::class_<Color>(module, "Color", R"doc(
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
#endif  // KRAKEN_ENABLE_PYTHON

}  // namespace color
}  // namespace kn
