#pragma once

#include <pybind11/pybind11.h>

class Line;

namespace py = pybind11;

namespace math
{
class Vec2;
}

namespace line
{
void _bind(py::module_& module);

Line move(const Line& line, const math::Vec2& offset);
} // namespace line

class Line
{
  public:
    double ax, ay, bx, by;

    Line();
    Line(double ax, double ay, double bx, double by);
    Line(double ax, double ay, const math::Vec2& b);
    Line(const math::Vec2& a, double bx, double by);
    Line(const math::Vec2& a, const math::Vec2& b);
    ~Line() = default;

    double getLength() const;

    math::Vec2 getA() const;

    math::Vec2 getB() const;

    void move(const math::Vec2& offset);

    Line copy() const;

    bool operator==(const Line& other) const;
    bool operator!=(const Line& other) const;
};
