#include "kraken/geometry/Capsule.hpp"

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>
#include <nanobind/stl/string.h>

#include <algorithm>
#include <string>

#include "bindings/python/bindings.hpp"
#include "kraken/geometry/Rect.hpp"

namespace kn
{
namespace capsule
{
void _bind(nb::module_& module)
{
    using namespace nb::literals;

    nb::class_<Capsule>(module, "Capsule", nb::pooled(KRAKEN_PYTHON_POOL_CAPACITY), R"doc(
Represents a capsule shape with two points and a radius.
    )doc")
        .def(nb::init<>(), R"doc(
Create a capsule with default values.
        )doc")
        .def(nb::init<const Vec2&, const Vec2&, double>(), "p1"_a, "p2"_a, "radius"_a, R"doc(
Create a capsule from two points and a radius.

Args:
    p1 (Vec2): The first point.
    p2 (Vec2): The second point.
    radius (float): The radius of the capsule.
        )doc")
        .def(
            nb::init<double, double, double, double, double>(), "x1"_a, "y1"_a, "x2"_a, "y2"_a,
            "radius"_a, R"doc(
Create a capsule from coordinates and a radius.

Args:
    x1 (float): The x coordinate of the first point.
    y1 (float): The y coordinate of the first point.
    x2 (float): The x coordinate of the second point.
    y2 (float): The y coordinate of the second point.
    radius (float): The radius of the capsule.
        )doc"
        )
        .def_rw("p1", &Capsule::p1, "The first point.")
        .def_rw("p2", &Capsule::p2, "The second point.")
        .def_rw("radius", &Capsule::radius, "The radius.")
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
        .def("__deepcopy__", [](const Capsule& self, nb::dict) { return self.copy(); })
        .def(nb::self == nb::self)
        .def(nb::self != nb::self)
        .def(
            "__repr__",
            [](const Capsule& self) -> std::string
            {
                return "Capsule(p1=" + std::to_string(self.p1.x) + ", " +
                       std::to_string(self.p1.y) + ", p2=" + std::to_string(self.p2.x) + ", " +
                       std::to_string(self.p2.y) + ", radius=" + std::to_string(self.radius) + ")";
            }
        );
}
}  // namespace capsule
}  // namespace kn
