#include "Circle.hpp"

#include <pybind11/stl_bind.h>

#include "Line.hpp"
#include "Rect.hpp"

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

namespace kn
{
Circle::Circle(const Vec2& center, const double radius)
    : pos(center),
      radius(radius)
{
}

Circle::Circle(const double x, const double y, const double radius)
    : pos({x, y}),
      radius(radius)
{
}

double Circle::getArea() const
{
    return M_PI * radius * radius;
}

double Circle::getCircumference() const
{
    return 2 * M_PI * radius;
}

Rect Circle::asRect() const
{
    Rect rect;
    rect.setSize(Vec2{radius * 2});
    rect.setCenter(pos);
    return rect;
}

Circle Circle::copy() const
{
    return {pos, radius};
}

bool Circle::operator==(const Circle& other) const
{
    return pos == other.pos && radius == other.radius;
}

bool Circle::operator!=(const Circle& other) const
{
    return !(*this == other);
}

namespace circle
{
void _bind(const py::module_& module)
{
    py::classh<Circle>(module, "Circle", R"doc(
Represents a circle shape with position and radius.

Supports collision detection with points, rectangles, other circles, and lines.
    )doc")

        .def(py::init<const Vec2&, double>(), py::arg("pos"), py::arg("radius"), R"doc(
Create a circle at a given position and radius.

Args:
    pos (Vec2): Center position of the circle.
    radius (float): Radius of the circle.
        )doc")
        .def(
            py::init<double, double, double>(), py::arg("x"), py::arg("y"), py::arg("radius"),
            R"doc(
Create a circle at given x and y coordinates with a specified radius.

Args:
    x (float): X coordinate of the circle's center.
    y (float): Y coordinate of the circle's center.
    radius (float): Radius of the circle.
        )doc"
        )

        .def_readwrite("pos", &Circle::pos, R"doc(
The center position of the circle as a Vec2.
        )doc")

        .def_readwrite("radius", &Circle::radius, R"doc(
The radius of the circle.
        )doc")

        .def_property_readonly("area", &Circle::getArea, R"doc(
Return the area of the circle.
        )doc")

        .def_property_readonly("circumference", &Circle::getCircumference, R"doc(
Return the circumference of the circle.
        )doc")

        .def("as_rect", &Circle::asRect, R"doc(
Return the smallest rectangle that fully contains the circle.
        )doc")

        .def("copy", &Circle::copy, R"doc(
Return a copy of the circle.
        )doc")

        .def(
            "__iter__",
            [](const Circle& circle) -> py::iterator
            {
                static double data[3];
                data[0] = circle.pos.x;
                data[1] = circle.pos.y;
                data[2] = circle.radius;
                return py::make_iterator(data, data + 3);
            },
            py::keep_alive<0, 1>()
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
                    throw py::index_error("Index out of range");
                }
            },
            py::arg("index")
        )

        .def("__len__", [](const Circle&) -> int { return 3; })

        .def("__eq__", &Circle::operator==, py::arg("other"))

        .def("__ne__", &Circle::operator!=, py::arg("other"));

    py::bind_vector<std::vector<kn::Circle>>(module, "CircleList");
}
}  // namespace circle
}  // namespace kn
