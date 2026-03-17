#pragma once

#include <nanobind/nanobind.h>

#include "Math.hpp"

namespace nb = nanobind;

namespace kn
{
class Line;
class Rect;

class Circle
{
  public:
    Vec2 pos;
    double radius = 0.0;

    Circle() = default;
    Circle(double radius);
    Circle(const Vec2& center, double radius);
    Circle(double x, double y, double radius);

    ~Circle() = default;

    [[nodiscard]] double getArea() const;

    [[nodiscard]] double getCircumference() const;

    [[nodiscard]] Rect asRect() const;

    [[nodiscard]] Circle copy() const;

    bool operator==(const Circle& other) const;
    bool operator!=(const Circle& other) const;
};

namespace circle
{
void _bind(const nb::module_& module);
}  // namespace circle
}  // namespace kn
