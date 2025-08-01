#include "Color.hpp"

#include <iomanip>
#include <sstream>

namespace color
{
void _bind(py::module_& module)
{
    py::class_<Color>(module, "Color", R"doc(
Represents an RGBA color.

Each channel (r, g, b, a) is an 8-bit unsigned integer.
    )doc")

        .def(py::init(), R"doc(
Create a Color with default values (0, 0, 0, 255).
        )doc")

        .def(py::init<uint8_t, uint8_t, uint8_t, uint8_t>(), py::arg("r"), py::arg("g"),
             py::arg("b"), py::arg("a") = 255, R"doc(
Create a Color from RGBA components.

Args:
    r (int): Red value [0-255].
    g (int): Green value [0-255].
    b (int): Blue value [0-255].
    a (int, optional): Alpha value [0-255]. Defaults to 255.
        )doc")

        .def(py::init(
                 [](const py::object& objparam) -> Color*
                 {
                     if (py::isinstance<py::str>(objparam))
                         return new Color(fromHex(objparam.cast<std::string>()));

                     if (py::isinstance<py::sequence>(objparam))
                     {
                         auto seq = objparam.cast<py::sequence>();
                         if (seq.size() == 3 || seq.size() == 4)
                         {
                             auto* c = new Color{seq[0].cast<uint8_t>(), seq[1].cast<uint8_t>(),
                                                 seq[2].cast<uint8_t>()};
                             c->a = seq.size() == 4 ? seq[3].cast<uint8_t>() : 255;
                             return c;
                         }
                     }

                     throw std::invalid_argument(
                         "Argument must be a hex string or a sequence of 3-4 integers.");
                 }),
             R"doc(
Create a Color from a hex string or a sequence of RGB(A) integers.

Examples:
    Color("#ff00ff")
    Color([255, 0, 255])
    Color((255, 0, 255, 128))
        )doc")

        .def(
            "__str__",
            [](const Color& c) -> std::string
            {
                return "(" + std::to_string(c.r) + ", " + std::to_string(c.g) + ", " +
                       std::to_string(c.b) + ", " + std::to_string(c.a) + ")";
            },
            R"doc(
Return a human-readable string representation.
        )doc")

        .def(
            "__repr__",
            [](const Color& c) -> std::string
            {
                return "Color(" + std::to_string(c.r) + ", " + std::to_string(c.g) + ", " +
                       std::to_string(c.b) + ", " + std::to_string(c.a) + ")";
            },
            R"doc(
Return a string suitable for debugging and recreation.
        )doc")

        .def(
            "__iter__", [](const Color& c) -> py::iterator
            { return py::make_iterator(&c.r, &c.r + 4); }, py::keep_alive<0, 1>(), R"doc(
Return an iterator over (r, g, b, a).
        )doc")

        .def(
            "__getitem__",
            [](const Color& c, size_t i) -> uint8_t
            {
                if (i >= 4)
                    throw py::index_error();
                return *(&c.r + i);
            },
            py::arg("index"), R"doc(
Access color channels by index (0=r, 1=g, 2=b, 3=a).
        )doc")

        .def(
            "__setitem__",
            [](Color& c, size_t i, uint8_t value)
            {
                if (i >= 4)
                    throw py::index_error();
                *(&c.r + i) = value;
            },
            py::arg("index"), py::arg("value"), R"doc(
Set a color channel by index.
        )doc")

        .def(
            "__len__", [](const Color&) { return 4; }, R"doc(
Return the number of channels (always 4).
        )doc")

        .def_readwrite("r", &Color::r, R"doc(
Red channel (0-255).
        )doc")
        .def_readwrite("g", &Color::g, R"doc(
Green channel (0-255).
        )doc")
        .def_readwrite("b", &Color::b, R"doc(
Blue channel (0-255).
        )doc")
        .def_readwrite("a", &Color::a, R"doc(
Alpha channel (0-255).
        )doc")

        .def_property("hex", &Color::toHex, &Color::fromHex, R"doc(
Get or set the color as a hex string (e.g. "#FF00FF" or "#FF00FF80").
        )doc");

    py::implicitly_convertible<py::sequence, Color>();
    py::implicitly_convertible<py::str, Color>();

    auto subColor = module.def_submodule("color", "Color-related functions and constants");

    subColor.def("from_hex", &fromHex, py::arg("hex"), R"doc(
Create a Color from a hex string (e.g. "#FF00FF" or "#FF00FF80").
        )doc");

    subColor.def(
        "to_hex",
        [](const py::object& colorObj) -> std::string
        {
            if (py::isinstance<Color>(colorObj))
                return toHex(colorObj.cast<Color>());

            if (py::isinstance<py::sequence>(colorObj))
            {
                auto seq = colorObj.cast<py::sequence>();
                if (seq.size() == 3 || seq.size() == 4)
                {
                    Color c = {seq[0].cast<uint8_t>(), seq[1].cast<uint8_t>(),
                               seq[2].cast<uint8_t>()};
                    c.a = seq.size() == 4 ? seq[3].cast<uint8_t>() : 255;
                    return toHex(c);
                }
            }

            throw std::invalid_argument("Argument must be a Color or a sequence of 3–4 integers.");
        },
        py::arg("color"), R"doc(
Convert a Color or RGB(A) sequence to a hex string.
        )doc");

    subColor.def("from_hsv", &fromHSV, py::arg("h"), py::arg("s"), py::arg("v"),
                 py::arg("a") = 1.0f, R"doc(
Create a Color from HSV(A) values.

Args:
    h (float): Hue angle (0-360).
    s (float): Saturation (0-1).
    v (float): Value/brightness (0-1).
    a (float, optional): Alpha (0-1). Defaults to 1.0.
        )doc");

    subColor.def("lerp", &lerp, py::arg("a"), py::arg("b"), py::arg("t"), R"doc(
Linearly interpolate between two colors.

Args:
    a (Color): Start color.
    b (Color): End color.
    t (float): Blend factor (0.0 = a, 1.0 = b).
        )doc");

    subColor.def("invert", &invert, py::arg("color"), R"doc(
Return the inverse of a color (flips RGB channels).
        )doc");

    subColor.attr("BLACK") = BLACK;
    subColor.attr("WHITE") = WHITE;
    subColor.attr("RED") = RED;
    subColor.attr("GREEN") = GREEN;
    subColor.attr("BLUE") = BLUE;
    subColor.attr("YELLOW") = YELLOW;
    subColor.attr("MAGENTA") = MAGENTA;
    subColor.attr("CYAN") = CYAN;
    subColor.attr("GRAY") = GRAY;
    subColor.attr("GREY") = GRAY;
    subColor.attr("DARK_GRAY") = DARK_GRAY;
    subColor.attr("DARK_GREY") = DARK_GRAY;
    subColor.attr("LIGHT_GRAY") = LIGHT_GRAY;
    subColor.attr("LIGHT_GREY") = LIGHT_GRAY;
    subColor.attr("ORANGE") = ORANGE;
    subColor.attr("BROWN") = BROWN;
    subColor.attr("PINK") = PINK;
    subColor.attr("PURPLE") = PURPLE;
    subColor.attr("NAVY") = NAVY;
    subColor.attr("TEAL") = TEAL;
    subColor.attr("OLIVE") = OLIVE;
    subColor.attr("MAROON") = MAROON;
}

Color fromHex(std::string_view hex)
{
    if (hex.empty())
        return {0, 0, 0, 255};

    if (hex[0] == '#')
        hex.remove_prefix(1);

    auto hexToByte = [](std::string_view str) -> uint8_t
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
        return {hexToByte(hex.substr(0, 2)), hexToByte(hex.substr(2, 2)),
                hexToByte(hex.substr(4, 2)), 255};
    }
    else if (hex.length() == 8)
    {
        // RRGGBBAA
        return {hexToByte(hex.substr(0, 2)), hexToByte(hex.substr(2, 2)),
                hexToByte(hex.substr(4, 2)), hexToByte(hex.substr(6, 2))};
    }
    else if (hex.length() == 3)
    {
        // RGB → duplicate each
        return {hexToByte(std::string(2, hex[0])), hexToByte(std::string(2, hex[1])),
                hexToByte(std::string(2, hex[2])), 255};
    }
    else if (hex.length() == 4)
    {
        // RGBA → duplicate each
        return {hexToByte(std::string(2, hex[0])), hexToByte(std::string(2, hex[1])),
                hexToByte(std::string(2, hex[2])), hexToByte(std::string(2, hex[3]))};
    }

    // Invalid format fallback
    return {0, 0, 0, 255};
}

std::string toHex(const Color& color)
{
    std::stringstream ss;

    ss << std::hex << std::uppercase << std::setfill('0') << std::setw(2)
       << static_cast<int>(color.r) << std::setw(2) << static_cast<int>(color.g) << std::setw(2)
       << static_cast<int>(color.b) << std::setw(2) << static_cast<int>(color.a);

    return "#" + ss.str();
}

Color fromHSV(const float h, const float s, const float v, const float a)
{
    const float c = v * s;
    const float x = c * (1 - std::fabs(fmod(h / 60.0f, 2) - 1));
    const float m = v - c;

    float r, g, b;

    if (h < 60)
    {
        r = c;
        g = x;
        b = 0;
    }
    else if (h < 120)
    {
        r = x;
        g = c;
        b = 0;
    }
    else if (h < 180)
    {
        r = 0;
        g = c;
        b = x;
    }
    else if (h < 240)
    {
        r = 0;
        g = x;
        b = c;
    }
    else if (h < 300)
    {
        r = x;
        g = 0;
        b = c;
    }
    else
    {
        r = c;
        g = 0;
        b = x;
    }

    return {static_cast<uint8_t>((r + m) * 255), static_cast<uint8_t>((g + m) * 255),
            static_cast<uint8_t>((b + m) * 255), static_cast<uint8_t>(a * 255)};
}

Color lerp(const Color& a, const Color& b, const double t)
{
    return {
        static_cast<uint8_t>(a.r + (b.r - a.r) * t), static_cast<uint8_t>(a.g + (b.g - a.g) * t),
        static_cast<uint8_t>(a.b + (b.b - a.b) * t), static_cast<uint8_t>(a.a + (b.a - a.a) * t)};
}

Color invert(const Color& color)
{
    return {static_cast<uint8_t>(255 - color.r), static_cast<uint8_t>(255 - color.g),
            static_cast<uint8_t>(255 - color.b), color.a};
}
} // namespace color

void Color::fromHex(std::string_view hex)
{
    if (hex.empty())
        return;

    if (hex[0] == '#')
        hex.remove_prefix(1);

    auto hexToByte = [](std::string_view str) -> uint8_t
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
        r = hexToByte(hex.substr(0, 2));
        g = hexToByte(hex.substr(2, 2));
        b = hexToByte(hex.substr(4, 2));
    }
    else if (hex.length() == 8)
    {
        // RRGGBBAA
        r = hexToByte(hex.substr(0, 2));
        g = hexToByte(hex.substr(2, 2));
        b = hexToByte(hex.substr(4, 2));
        a = hexToByte(hex.substr(6, 2));
    }
    else if (hex.length() == 3)
    {
        // RGB → duplicate each
        r = hexToByte(std::string(2, hex[0]));
        g = hexToByte(std::string(2, hex[1]));
        b = hexToByte(std::string(2, hex[2]));
    }
    else if (hex.length() == 4)
    {
        // RGBA → duplicate each
        r = hexToByte(std::string(2, hex[0]));
        g = hexToByte(std::string(2, hex[1]));
        b = hexToByte(std::string(2, hex[2]));
        a = hexToByte(std::string(2, hex[3]));
    }
}

std::string Color::toHex() const
{
    std::stringstream ss;

    ss << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << r << std::setw(2) << g
       << std::setw(2) << b << std::setw(2) << a;

    return "#" + ss.str();
}

bool Color::_isValid() const
{
    return 0 <= r && r <= 255 && 0 <= g && g <= 255 && 0 <= b && b <= 255 && 0 <= a && a <= 255;
}

Color color::_fromSeq(const py::sequence& seq)
{
    if (seq.size() < 3 || seq.size() > 4)
        throw std::invalid_argument("Color sequence must be of length 3 or 4");

    Color color = {seq[0].cast<uint8_t>(), seq[1].cast<uint8_t>(), seq[2].cast<uint8_t>()};
    if (seq.size() == 4)
        color.a = seq[3].cast<uint8_t>();

    return color;
}