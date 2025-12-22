#pragma once

#include <SDL3/SDL.h>
#include <pybind11/pybind11.h>

#include <string_view>

namespace py = pybind11;

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

void _bind(py::module_& module);

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
    Color operator/(double scalar) const;
};

constexpr Color BLACK = {0, 0, 0};
constexpr Color WHITE = {255, 255, 255};
constexpr Color RED = {255, 0, 0};
constexpr Color GREEN = {0, 255, 0};
constexpr Color BLUE = {0, 0, 255};
constexpr Color YELLOW = {255, 255, 0};
constexpr Color MAGENTA = {255, 0, 255};
constexpr Color CYAN = {0, 255, 255};
constexpr Color GRAY = {128, 128, 128};
constexpr Color DARK_GRAY = {64, 64, 64};
constexpr Color LIGHT_GRAY = {192, 192, 192};
constexpr Color ORANGE = {255, 165, 0};
constexpr Color BROWN = {139, 69, 19};
constexpr Color PINK = {255, 192, 203};
constexpr Color PURPLE = {128, 0, 128};
constexpr Color NAVY = {0, 0, 128};
constexpr Color TEAL = {0, 128, 128};
constexpr Color OLIVE = {128, 128, 0};
constexpr Color MAROON = {128, 0, 0};
}  // namespace kn
