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
    double h = 0;
    double s = 0;
    double v = 0;
    double a = 1.0;
};

void _bind(py::module_& module);

Color fromHex(std::string_view hex);

Color fromHSV(const HSV& hsv);

Color lerp(const Color& a, const Color& b, double t);

Color invert(const Color& color);

Color grayscale(const Color& color);
} // namespace color

struct Color
{
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 255;

    explicit operator SDL_Color() const { return {r, g, b, a}; }

    explicit operator SDL_FColor() const
    {
        return {static_cast<float>(r) / 255, static_cast<float>(g) / 255,
                static_cast<float>(b) / 255, static_cast<float>(a) / 255};
    }

    explicit operator uint32_t() const
    {
        return static_cast<uint32_t>(a) << 24 | static_cast<uint32_t>(b) << 16 |
               static_cast<uint32_t>(g) << 8 | static_cast<uint32_t>(r);
    }

    [[nodiscard]] std::string toHex() const;

    void fromHex(std::string_view hex);

    [[nodiscard]] color::HSV toHSV() const;

    void fromHSV(const color::HSV& hsv);
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
} // namespace kn
