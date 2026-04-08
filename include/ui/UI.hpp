#pragma once

#include <nanobind/nanobind.h>

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "Color.hpp"
#include "Font.hpp"
#include "Rect.hpp"
#include "Texture.hpp"

namespace nb = nanobind;

namespace kn::ui
{
enum class Direction
{
    Horizontal,
    Vertical
};

enum class Align
{
    Start,
    Center,
    End,
    Stretch
};

struct Style
{
    // Background props
    std::optional<Color> backgroundColor;
    const Texture* texture = nullptr;
    Rect slice;  // 9-slice: x=left, y=top, w=right, h=bottom

    // Text props
    const Font* font = nullptr;
    std::optional<Color> textColor;

    double padding = 0.0;
    double margin = 0.0;
    double gap = 0.0;

    // Border props
    int borderWidth = 0;
    double borderRadius = 0.0;
    std::optional<Color> borderColor;

    std::optional<double> width;
    std::optional<double> height;
};

void begin(const Rect& rootBounds);
void end();

bool button(const std::string& text, const Style& style);
void label(const std::string& text, const Style& style);
void image(const Texture* tex, const Rect& slice, const Style& style);

void _bind(nb::module_& m);
}  // namespace kn::ui
