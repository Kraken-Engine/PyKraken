#pragma once

#include "Math.hpp"
#include <pybind11/pybind11.h>

namespace py = pybind11;

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
    Circle(const Vec2& center, double radius);
    ~Circle() = default;

    [[nodiscard]] double getArea() const;

    [[nodiscard]] double getCircumference() const;

    [[nodiscard]] bool collidePoint(const Vec2& point) const;

    [[nodiscard]] bool collideCircle(const Circle& circle) const;

    [[nodiscard]] bool collideRect(const Rect& rect) const;

    [[nodiscard]] bool collideLine(const Line& line) const;

    [[nodiscard]] bool contains(const Circle& circle) const;
    [[nodiscard]] bool contains(const Rect& rect) const;
    [[nodiscard]] bool contains(const Line& line) const;

    [[nodiscard]] Rect asRect() const;

    [[nodiscard]] Circle copy() const;

    bool operator==(const Circle& other) const;
    bool operator!=(const Circle& other) const;
};

namespace circle
{
void _bind(const py::module_& module);
} // namespace circle
} // namespace kn
