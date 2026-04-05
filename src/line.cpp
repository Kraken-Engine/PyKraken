#include "Line.hpp"

#include <nanobind/make_iterator.h>
#include <nanobind/operators.h>

#include "Math.hpp"

namespace kn
{
Line::Line()
    : ax(0.0),
      ay(0.0),
      bx(0.0),
      by(0.0)
{
}

Line::Line(const double ax, const double ay, const double bx, const double by)
    : ax(ax),
      ay(ay),
      bx(bx),
      by(by)
{
}

Line::Line(const double ax, const double ay, const Vec2& b)
    : ax(ax),
      ay(ay),
      bx(b.x),
      by(b.y)
{
}

Line::Line(const Vec2& a, const double bx, const double by)
    : ax(a.x),
      ay(a.y),
      bx(bx),
      by(by)
{
}

Line::Line(const Vec2& a, const Vec2& b)
    : ax(a.x),
      ay(a.y),
      bx(b.x),
      by(b.y)
{
}

double Line::getLength() const
{
    const double dx = bx - ax;
    const double dy = by - ay;
    return sqrt(dx * dx + dy * dy);
}

Vec2 Line::getA() const
{
    return {ax, ay};
}

void Line::setA(const Vec2& pos)
{
    ax = pos.x;
    ay = pos.y;
}

Vec2 Line::getB() const
{
    return {bx, by};
}

void Line::setB(const Vec2& pos)
{
    bx = pos.x;
    by = pos.y;
}

void Line::move(const Vec2& offset)
{
    ax += offset.x;
    ay += offset.y;
    bx += offset.x;
    by += offset.y;
}

Line Line::moved(const Vec2& offset) const
{
    Line l = *this;
    l.move(offset);
    return l;
}

Vec2 Line::getMidpoint() const
{
    return {(ax + bx) / 2.0, (ay + by) / 2.0};
}

Line Line::getPerpendicular() const
{
    const double dx = bx - ax;
    const double dy = by - ay;
    return {-dy, dx, -dy, dx};
}

double Line::getAngle() const
{
    return atan2(by - ay, bx - ax);
}

Vec2 Line::getClosestPoint(const Vec2& point) const
{
    const double dx = bx - ax;
    const double dy = by - ay;
    const double lengthSquared = dx * dx + dy * dy;

    if (lengthSquared == 0.0)
        return {ax, ay};  // Line is a point

    const double t = ((point.x - ax) * dx + (point.y - ay) * dy) / lengthSquared;
    const double clampedT = std::max(0.0, std::min(1.0, t));

    return {ax + clampedT * dx, ay + clampedT * dy};
}

Line Line::copy() const
{
    return {ax, ay, bx, by};
}

bool Line::operator==(const Line& other) const
{
    return ax == other.ax && ay == other.ay && bx == other.bx && by == other.by;
}

bool Line::operator!=(const Line& other) const
{
    return !(*this == other);
}

namespace line
{
void _bind(nb::module_& module)
{
    using namespace nb::literals;

    nb::class_<Line>(module, "Line", R"doc(
A 2D line segment defined by two points.
You can access or modify points using `.a`, `.b`, or directly via `.ax`, `.ay`, `.bx`, `.by`.
    )doc")
        .def(nb::init(), R"doc(
Create a default line with all values set to 0.
        )doc")
        .def(nb::init<double, double, double, double>(), "ax"_a, "ay"_a, "bx"_a, "by"_a, R"doc(
Create a line from two coordinate points.

Args:
    ax (float): X-coordinate of point A.
    ay (float): Y-coordinate of point A.
    bx (float): X-coordinate of point B.
    by (float): Y-coordinate of point B.
        )doc")
        .def(
            nb::init<double, double, const Vec2&>(), "ax"_a, "ay"_a, "b"_a,
            R"doc(
Create a line from A coordinates and a Vec2 B point.

Args:
    ax (float): X-coordinate of point A.
    ay (float): Y-coordinate of point A.
    b (Vec2): Point B.
        )doc"
        )
        .def(
            nb::init<const Vec2&, double, double>(), "a"_a, "bx"_a, "by"_a,
            R"doc(
Create a line from a Vec2 A point and B coordinates.

Args:
    a (Vec2): Point A.
    bx (float): X-coordinate of point B.
    by (float): Y-coordinate of point B.
        )doc"
        )
        .def(nb::init<const Vec2&, const Vec2&>(), "a"_a, "b"_a, R"doc(
Create a line from two Vec2 points.

Args:
    a (Vec2): Point A.
    b (Vec2): Point B.
        )doc")

        .def_prop_rw("a", &Line::getA, &Line::setA, R"doc(
Get or set point A as a tuple or Vec2.
        )doc")
        .def_prop_rw("b", &Line::getB, &Line::setB, R"doc(
Get or set point B as a tuple or Vec2.
        )doc")

        .def_rw("ax", &Line::ax, "X-coordinate of point A.")
        .def_rw("ay", &Line::ay, "Y-coordinate of point A.")
        .def_rw("bx", &Line::bx, "X-coordinate of point B.")
        .def_rw("by", &Line::by, "Y-coordinate of point B.")

        .def_prop_ro("length", &Line::getLength, R"doc(
The Euclidean length of the line segment.
        )doc")

        .def("copy", &Line::copy, R"doc(
Return a copy of this line.
        )doc")
        .def("move", &Line::move, "offset"_a, R"doc(
Move this line by a Vec2 offset.

Args:
    offset (Vec2): The amount to move.
        )doc")
        .def("moved", &Line::moved, "offset"_a, R"doc(
Return a new line moved by a Vec2 offset.

Args:
    offset (Vec2): The amount to move.
        )doc")
        .def("get_midpoint", &Line::getMidpoint, R"doc(
Get the midpoint of the line segment.

Returns:
    Vec2: The midpoint as a Vec2.
        )doc")
        .def("get_perpendicular", &Line::getPerpendicular, R"doc(
Get a perpendicular bisector line from this line.

Returns:
    Line: A new perpendicular line.
        )doc")
        .def("get_angle", &Line::getAngle, R"doc(
Get the angle of the line in radians.

Returns:
    float: The angle in radians.
        )doc")
        .def("get_closest_point", &Line::getClosestPoint, "point"_a, R"doc(
Get the closest point on the line to a given point.

Args:
    point (Vec2): The point to find the closest point to.

Returns:
    Vec2: The closest point on the line as a Vec2.
        )doc")

        .def(
            "__iter__", [](const Line& self) -> nb::iterator
            { return nb::make_iterator(nb::type<Line>(), "iterator", &self.ax, &self.ax + 4); },
            nb::keep_alive<0, 1>()
        )
        .def(
            "__getitem__",
            [](const Line& self, const size_t i) -> double
            {
                switch (i)
                {
                case 0:
                    return self.ax;
                case 1:
                    return self.ay;
                case 2:
                    return self.bx;
                case 3:
                    return self.by;
                default:
                    throw nb::index_error("Index out of range");
                }
            },
            "index"_a
        )
        .def("__len__", [](const Line&) -> int { return 4; })
        .def(nb::self == nb::self)
        .def(nb::self != nb::self);
}
}  // namespace line
}  // namespace kn
