#pragma once

#include <pybind11/pybind11.h>

#include "Math.hpp"

class Rect;
class Line;

namespace py = pybind11;

namespace circle
{
void _bind(py::module_& module);
}

class Circle
{
  public:
    Vec2 pos;
    double radius = 0.0;

    Circle() = default;
    Circle(const Vec2& center, double radius);
    ~Circle() = default;

    double getArea() const;

    double getCircumference() const;

    bool collidePoint(const Vec2& point) const;

    bool collideCircle(const Circle& circle) const;

    bool collideRect(const Rect& rect) const;

    bool collideLine(const Line& line) const;

    bool contains(const Circle& circle) const;
    bool contains(const Rect& rect) const;
    bool contains(const Line& line) const;

    Rect asRect() const;

    Circle* copy() const;

    bool operator==(const Circle& other) const;
    bool operator!=(const Circle& other) const;
};
