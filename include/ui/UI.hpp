#pragma once

#ifdef KRAKEN_ENABLE_PYTHON
#include <nanobind/nanobind.h>
#endif  // KRAKEN_ENABLE_PYTHON

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "Color.hpp"
#include "Font.hpp"
#include "Math.hpp"
#include "Rect.hpp"
#include "Texture.hpp"

#ifdef KRAKEN_ENABLE_PYTHON
namespace nb = nanobind;
#endif  // KRAKEN_ENABLE_PYTHON

namespace kn::ui
{
enum class Direction
{
    Horizontal,
    Vertical,
    Stack
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

    // Layout
    Vec2 offset{0.0, 0.0};

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

class Context
{
  public:
    Context() = default;
    ~Context();
    Context(const Context&) = delete;
    Context& operator=(const Context&) = delete;
    Context(Context&& other) noexcept
        : m_active(other.m_active)
    {
        other.m_active = false;
    }
    Context& operator=(Context&& other) noexcept
    {
        m_active = other.m_active;
        other.m_active = false;
        return *this;
    }

    void exit();

  private:
    bool m_active = true;
};

class RootContext
{
  public:
    RootContext(
        const Rect& bounds, Direction dir = Direction::Vertical, Align align = Align::Start,
        Align justify = Align::Start
    );
    ~RootContext();
    RootContext(const RootContext&) = delete;
    RootContext& operator=(const RootContext&) = delete;
    RootContext(RootContext&& other) noexcept
        : m_active(other.m_active)
    {
        other.m_active = false;
    }
    RootContext& operator=(RootContext&& other) noexcept
    {
        m_active = other.m_active;
        other.m_active = false;
        return *this;
    }

    void exit();

  private:
    bool m_active = true;
};

RootContext root(
    const Rect& bounds, Direction dir = Direction::Vertical, Align align = Align::Start,
    Align justify = Align::Start
);

Context row(
    const std::optional<Style>& style = std::nullopt, double gap = 0.0, double padding = 0.0,
    Align align = Align::Start, Align justify = Align::Start
);

Context column(
    const std::optional<Style>& style = std::nullopt, double gap = 0.0, double padding = 0.0,
    Align align = Align::Start, Align justify = Align::Start
);

Context stack(
    const std::optional<Style>& style = std::nullopt, double padding = 0.0,
    Align align = Align::Start, Align justify = Align::Start
);

bool button(const std::string& text, const Style& style);
void label(const std::string& text, const Style& style);
void panel(const Style& style);

#ifdef KRAKEN_ENABLE_PYTHON
void _bind(nb::module_& m);
#endif  // KRAKEN_ENABLE_PYTHON

}  // namespace kn::ui
