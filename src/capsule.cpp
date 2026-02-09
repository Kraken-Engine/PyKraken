#include "Capsule.hpp"

#include <algorithm>

#include "Rect.hpp"

namespace kn
{
Capsule::Capsule(const Vec2& p1, const Vec2& p2, const double radius)
    : p1(p1),
      p2(p2),
      radius(radius)
{
}

Capsule::Capsule(
    const double x1, const double y1, const double x2, const double y2, const double radius
)
    : p1(x1, y1),
      p2(x2, y2),
      radius(radius)
{
}

Rect Capsule::asRect() const
{
    const double minX = std::min(p1.x, p2.x) - radius;
    const double minY = std::min(p1.y, p2.y) - radius;
    const double maxX = std::max(p1.x, p2.x) + radius;
    const double maxY = std::max(p1.y, p2.y) + radius;

    return {minX, minY, maxX - minX, maxY - minY};
}

Capsule Capsule::copy() const
{
    return {p1, p2, radius};
}

bool Capsule::operator==(const Capsule& other) const
{
    return p1 == other.p1 && p2 == other.p2 && radius == other.radius;
}

bool Capsule::operator!=(const Capsule& other) const
{
    return !(*this == other);
}

namespace capsule
{
void _bind(const py::module_& module)
{
    py::classh<Capsule>(module, "Capsule", R"doc(
Represents a capsule shape with two points and a radius.
    )doc")
        .def(py::init<>(), R"doc(
Create a capsule with default values.
        )doc")
        .def(
            py::init<const Vec2&, const Vec2&, double>(), py::arg("p1"), py::arg("p2"),
            py::arg("radius"), R"doc(
Create a capsule from two points and a radius.

Args:
    p1 (Vec2): The first point.
    p2 (Vec2): The second point.
    radius (float): The radius of the capsule.
        )doc"
        )
        .def(
            py::init<double, double, double, double, double>(), py::arg("x1"), py::arg("y1"),
            py::arg("x2"), py::arg("y2"), py::arg("radius"), R"doc(
Create a capsule from coordinates and a radius.

Args:
    x1 (float): The x coordinate of the first point.
    y1 (float): The y coordinate of the first point.
    x2 (float): The x coordinate of the second point.
    y2 (float): The y coordinate of the second point.
    radius (float): The radius of the capsule.
        )doc"
        )
        .def_readwrite("p1", &Capsule::p1, "The first point.")
        .def_readwrite("p2", &Capsule::p2, "The second point.")
        .def_readwrite("radius", &Capsule::radius, "The radius.")
        .def("as_rect", &Capsule::asRect, R"doc(
Get the axis-aligned bounding box of the capsule.

Returns:
    Rect: The bounding box.
        )doc")
        .def("copy", &Capsule::copy, R"doc(
Create a copy of the capsule.

Returns:
    Capsule: The copy.
        )doc")
        .def("__copy__", &Capsule::copy)
        .def("__deepcopy__", [](const Capsule& self, py::dict) { return self.copy(); })
        .def("__eq__", &Capsule::operator==)
        .def("__ne__", &Capsule::operator!=)
        .def(
            "__repr__",
            [](const Capsule& self)
            {
                return "Capsule(p1=" + std::to_string(self.p1.x) + ", " + std::to_string(self.p1.y) +
                       ", p2=" + std::to_string(self.p2.x) + ", " + std::to_string(self.p2.y) +
                       ", radius=" + std::to_string(self.radius) + ")";
            }
        );
}
}  // namespace capsule
}  // namespace kn
