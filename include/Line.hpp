#pragma once

#include <pybind11/pybind11.h>

class Line;

namespace py = pybind11;

class Vec2;

namespace line
{
void _bind(py::module_& module);

Line move(const Line& line, const Vec2& offset);
} // namespace line

class Line
{
  public:
    double ax, ay, bx, by;

    Line();
    Line(double ax, double ay, double bx, double by);
    Line(double ax, double ay, const Vec2& b);
    Line(const Vec2& a, double bx, double by);
    Line(const Vec2& a, const Vec2& b);
    ~Line() = default;

    double getLength() const;

    Vec2 getA() const;
    void setA(const Vec2& pos);
    Vec2 getB() const;
    void setB(const Vec2& pos);

    void move(const Vec2& offset);

    Line* copy() const;

    bool operator==(const Line& other) const;
    bool operator!=(const Line& other) const;
};
