#pragma once

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace kn
{
class Vec2;

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

    [[nodiscard]] double getLength() const;

    [[nodiscard]] Vec2 getA() const;
    void setA(const Vec2& pos);
    [[nodiscard]] Vec2 getB() const;
    void setB(const Vec2& pos);

    void move(const Vec2& offset);

    [[nodiscard]] Line copy() const;

    bool operator==(const Line& other) const;
    bool operator!=(const Line& other) const;
};

namespace line
{
void _bind(py::module_& module);

Line move(const Line& line, const Vec2& offset);
} // namespace line
} // namespace kn
