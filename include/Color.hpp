#pragma once

#include <SDL3/SDL.h>
#include <nanobind/nanobind.h>

#include <string_view>

namespace nb = nanobind;

namespace kn
{
struct Color;
namespace color
{
struct HSV
{
    double h = 0.0;
    double s = 0.0;
    double v = 0.0;
    double a = 1.0;
};

void _bind(nb::module_& module);

Color fromHex(std::string_view hex);

Color fromHSV(const HSV& hsv);

Color lerp(const Color& a, const Color& b, double t);

Color invert(const Color& color);

Color grayscale(const Color& color);
}  // namespace color

struct Color
{
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 255;

    [[nodiscard]] std::string toHex() const;
    void fromHex(std::string_view hex);

    [[nodiscard]] color::HSV toHSV() const;
    void fromHSV(const color::HSV& hsv);

    // Conversions to other color formats
    explicit operator SDL_Color() const;
    explicit operator SDL_FColor() const;
    explicit operator uint32_t() const;

    // Equality operators compare all RGBA components
    bool operator==(const Color& other) const;
    bool operator!=(const Color& other) const;

    // Unary negation inverts the color (preserves alpha)
    Color operator-() const;

    // Division and multiplication by scalar between 0.0 and 1.0
    Color operator*(double scalar) const;
    Color& operator*=(double scalar);
    Color operator/(double scalar) const;
    Color& operator/=(double scalar);

    const static Color BLACK;
    const static Color WHITE;
    const static Color RED;
    const static Color GREEN;
    const static Color BLUE;
    const static Color YELLOW;
    const static Color MAGENTA;
    const static Color CYAN;
    const static Color GRAY;
    const static Color DARK_GRAY;
    const static Color LIGHT_GRAY;
    const static Color ORANGE;
    const static Color BROWN;
    const static Color PINK;
    const static Color PURPLE;
    const static Color NAVY;
    const static Color TEAL;
    const static Color OLIVE;
    const static Color MAROON;
};

Color operator*(double scalar, const Color& color);
}  // namespace kn
