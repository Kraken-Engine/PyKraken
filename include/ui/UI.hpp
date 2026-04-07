#pragma once

#include <nanobind/nanobind.h>

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "Color.hpp"
#include "Math.hpp"
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
    std::optional<Color> backgroundColor;
    const Texture* texture = nullptr;
    Rect slice;  // 9-slice parameters: x=left, y=top, w=right, h=bottom

    double padding = 0.0;
    double gap = 0.0;
    double margin = 0.0;

    std::optional<double> width;
    std::optional<double> height;
};

class Node
{
  public:
    Node() = default;
    virtual ~Node() = default;

    Style style;
    Rect bounds;
    std::vector<std::unique_ptr<Node>> children;

    // Layout properties
    Direction direction = Direction::Vertical;
    Align align = Align::Start;
    Align justify = Align::Start;

    virtual void render() = 0;
    virtual bool handleInput(const Vec2& mousePos, bool clicked) = 0;
};

class Box : public Node
{
  public:
    void render() override;
    bool handleInput(const Vec2& mousePos, bool clicked) override;
};

void begin(const Rect& rootBounds);
void end();

bool button(const std::string& text, const Style& style);
void label(const std::string& text, const Style& style);
void image(const Texture& tex, const Rect& slice, const Style& style);

void _bind(nb::module_& m);
}  // namespace kn::ui
