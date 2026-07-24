#include "kraken/graphics/Color.hpp"

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

}  // namespace color
}  // namespace kn
