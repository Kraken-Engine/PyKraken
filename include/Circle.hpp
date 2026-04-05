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

    [[nodiscard]] double getDiameter() const;
    void setDiameter(double diameter);

    [[nodiscard]] Rect asRect() const;

    [[nodiscard]] double getLeft() const;
    [[nodiscard]] double getRight() const;
    [[nodiscard]] double getTop() const;
    [[nodiscard]] double getBottom() const;

    void setLeft(double left);
    void setRight(double right);
    void setTop(double top);
    void setBottom(double bottom);

    [[nodiscard]] Circle copy() const;

    bool operator==(const Circle& other) const;
    bool operator!=(const Circle& other) const;
};

namespace circle
{
void _bind(const nb::module_& module);
}  // namespace circle
}  // namespace kn
