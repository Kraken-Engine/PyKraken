#pragma once

#include <SDL3/SDL.h>
#ifdef KRAKEN_ENABLE_PYTHON
#include <nanobind/nanobind.h>
#endif  // KRAKEN_ENABLE_PYTHON

#include <string_view>

#ifdef KRAKEN_ENABLE_PYTHON
namespace nb = nanobind;
#endif  // KRAKEN_ENABLE_PYTHON

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

Color fromHex(std::string_view hex);

Color fromHSV(const HSV& hsv);

Color lerp(const Color& a, const Color& b, double t);

Color invert(const Color& color);

Color grayscale(const Color& color);

#ifdef KRAKEN_ENABLE_PYTHON
void _bind(nb::module_& module);
#endif  // KRAKEN_ENABLE_PYTHON

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

    static const Color BLACK;
    static const Color WHITE;
    static const Color RED;
    static const Color GREEN;
    static const Color BLUE;
    static const Color YELLOW;
    static const Color MAGENTA;
    static const Color CYAN;
    static const Color GRAY;
    static const Color DARK_GRAY;
    static const Color LIGHT_GRAY;
    static const Color ORANGE;
    static const Color BROWN;
    static const Color PINK;
    static const Color PURPLE;
    static const Color NAVY;
    static const Color TEAL;
    static const Color OLIVE;
    static const Color MAROON;
};

inline constexpr Color Color::BLACK = {0, 0, 0};
inline constexpr Color Color::WHITE = {255, 255, 255};
inline constexpr Color Color::RED = {255, 0, 0};
inline constexpr Color Color::GREEN = {0, 255, 0};
inline constexpr Color Color::BLUE = {0, 0, 255};
inline constexpr Color Color::YELLOW = {255, 255, 0};
inline constexpr Color Color::MAGENTA = {255, 0, 255};
inline constexpr Color Color::CYAN = {0, 255, 255};
inline constexpr Color Color::GRAY = {128, 128, 128};
inline constexpr Color Color::DARK_GRAY = {64, 64, 64};
inline constexpr Color Color::LIGHT_GRAY = {192, 192, 192};
inline constexpr Color Color::ORANGE = {255, 165, 0};
inline constexpr Color Color::BROWN = {139, 69, 19};
inline constexpr Color Color::PINK = {255, 192, 203};
inline constexpr Color Color::PURPLE = {128, 0, 128};
inline constexpr Color Color::NAVY = {0, 0, 128};
inline constexpr Color Color::TEAL = {0, 128, 128};
inline constexpr Color Color::OLIVE = {128, 128, 0};
inline constexpr Color Color::MAROON = {128, 0, 0};

Color operator*(double scalar, const Color& color);
}  // namespace kn
