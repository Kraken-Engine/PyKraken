#include "kraken/geometry/Circle.hpp"

#include <nanobind/make_iterator.h>
#include <nanobind/nanobind.h>
#include <nanobind/operators.h>

#include "bindings/python/bindings.hpp"
#include "kraken/geometry/Line.hpp"
#include "kraken/geometry/Rect.hpp"

namespace kn
{
namespace circle
{
void _bind(nb::module_& module)
{
    using namespace nb::literals;

    nb::class_<Circle>(module, "Circle", nb::pooled(KRAKEN_PYTHON_POOL_CAPACITY), R"doc(
Represents a circle shape with position and radius.

Supports collision detection with points, rectangles, other circles, and lines.
    )doc")

        .def(nb::init<>(), R"doc(
Create a circle with default position (0, 0) and radius 0.
        )doc")
        .def(nb::init<double>(), "radius"_a, R"doc(
Create a circle with a specified radius at the default position (0, 0).

Args:
    radius (float): The radius of the circle.
        )doc")
        .def(nb::init<const Vec2&, double>(), "pos"_a, "radius"_a, R"doc(
Create a circle at a given position and radius.

Args:
    pos (Vec2): Center position of the circle.
    radius (float): Radius of the circle.
        )doc")
        .def(
            nb::init<double, double, double>(), "x"_a, "y"_a, "radius"_a,
            R"doc(
Create a circle at given x and y coordinates with a specified radius.

Args:
    x (float): X coordinate of the circle's center.
    y (float): Y coordinate of the circle's center.
    radius (float): Radius of the circle.
        )doc"
        )

        .def_rw("pos", &Circle::pos, R"doc(
The center position of the circle as a Vec2.
        )doc")
        .def_rw("radius", &Circle::radius, R"doc(
The radius of the circle.
        )doc")
        .def_prop_rw("diameter", &Circle::getDiameter, &Circle::setDiameter, R"doc(
Get or set the diameter of the circle.
        )doc")
        .def_prop_rw("left", &Circle::getLeft, &Circle::setLeft, R"doc(
Get or set the x coordinate of the leftmost point of the circle.
        )doc")
        .def_prop_rw("right", &Circle::getRight, &Circle::setRight, R"doc(
Get or set the x coordinate of the rightmost point of the circle.
        )doc")
        .def_prop_rw("top", &Circle::getTop, &Circle::setTop, R"doc(
Get or set the y coordinate of the topmost point of the circle.
        )doc")
        .def_prop_rw("bottom", &Circle::getBottom, &Circle::setBottom, R"doc(
Get or set the y coordinate of the bottommost point of the circle.
        )doc")

        .def_prop_ro("area", &Circle::getArea, R"doc(
Get the area of the circle.
        )doc")
        .def_prop_ro("circumference", &Circle::getCircumference, R"doc(
Get the circumference of the circle.
        )doc")

        .def("as_rect", &Circle::asRect, R"doc(
Return the smallest rectangle that fully contains the circle.
        )doc")
        .def("copy", &Circle::copy, R"doc(
Return a copy of the circle.
        )doc")

        .def(
            "__iter__",
            [](const Circle& circle) -> nb::iterator
            {
                static double data[3];
                data[0] = circle.pos.x;
                data[1] = circle.pos.y;
                data[2] = circle.radius;
                return nb::make_iterator(nb::type<Circle>(), "iterator", data, data + 3);
            },
            nb::keep_alive<0, 1>()
        )

        .def(
            "__getitem__",
            [](const Circle& circle, const size_t i) -> double
            {
                switch (i)
                {
                case 0:
                    return circle.pos.x;
                case 1:
                    return circle.pos.y;
                case 2:
                    return circle.radius;
                default:
                    throw nb::index_error("Index out of range");
                }
            },
            "index"_a
        )

        .def("__len__", [](const Circle&) -> int { return 3; })

        .def(nb::self == nb::self)
        .def(nb::self != nb::self);
}
}  // namespace circle
}  // namespace kn
