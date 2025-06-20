#pragma once

#include <SDL3/SDL.h>
#include <cstdint>
#include <pybind11/pybind11.h>
#include <string_view>

namespace py = pybind11;

struct Color
{
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 255;

    operator SDL_Color() const { return {r, g, b, a}; }
    operator SDL_FColor() const { return {r / 255.f, g / 255.f, b / 255.f, a / 255.f}; }

    std::string toHex() const;

    void fromHex(std::string_view hex);

    bool _isValid() const;
};

namespace color
{
void _bind(py::module_& module);

Color fromHex(std::string_view hex);

std::string toHex(const Color& color);

Color fromHSV(float h, float s, float v, float a = 1.0f);

Color _fromSeq(const py::sequence& seq);

Color lerp(const Color& a, const Color& b, double t);

Color invert(const Color& color);

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
} // namespace color
