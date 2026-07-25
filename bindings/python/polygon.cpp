#include "kraken/geometry/Polygon.hpp"

#include <nanobind/make_iterator.h>
#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>

#include <cmath>
#include <limits>

#include "bindings/python/bindings.hpp"
#include "kraken/core/_globals.hpp"
#include "kraken/geometry/Rect.hpp"
#include "kraken/math/Math.hpp"

namespace kn
{
namespace polygon
{
void _bind(nb::module_& module)
{
    using namespace nb::literals;

    nb::class_<Polygon>(module, "Polygon", R"doc(
Represents a polygon shape defined by a sequence of points.

A polygon is a closed shape made up of connected line segments. The points define
the vertices of the polygon in order. Supports various geometric operations.
    )doc")
        .def(nb::init(), R"doc(
Create an empty polygon with no points.
        )doc")
        .def(nb::init<const std::vector<Vec2>&>(), "points"_a, R"doc(
Create a polygon from a vector of Vec2 points.

Args:
    points (Sequence[Vec2]): List of Vec2 points defining the polygon vertices.
        )doc")
        .def(
            nb::init<uint32_t, double, const Vec2&>(), "n"_a, "radius"_a, "centroid"_a = Vec2::ZERO,
            R"doc(
Create a regular polygon with n sides inscribed in a circle of the given radius.

Args:
    n (int): The number of sides (vertices) of the regular polygon.
    radius (float): The radius of the circumscribed circle for the regular polygon.
    centroid (Vec2, optional): The center point of the polygon. Defaults to (0, 0).
        )doc"
        )

        .def_rw("points", &Polygon::points, R"doc(
The list of Vec2 points that define the polygon vertices.
        )doc")
        .def_prop_rw("centroid", &Polygon::getCentroid, &Polygon::setCentroid, R"doc(
Get or set the centroid of the polygon.

Returns:
    Vec2: The center point of the polygon.
        )doc")

        .def_prop_ro("perimeter", &Polygon::getPerimeter, R"doc(
Get the perimeter of the polygon.

Returns:
    float: The total distance around the polygon.
        )doc")
        .def_prop_ro("area", &Polygon::getArea, R"doc(
Get the area of the polygon.

Returns:
    float: The area enclosed by the polygon.
        )doc")
        .def_prop_ro("is_convex", &Polygon::isConvex, R"doc(
Check if the polygon is convex.

Returns:
    bool: True if the polygon is convex, False otherwise.
        )doc")
        .def_prop_ro("is_concave", &Polygon::isConcave, R"doc(
Check if the polygon is concave.

Returns:
    bool: True if the polygon is concave, False otherwise.
        )doc")

        .def("get_rect", &Polygon::getRect, R"doc(
Get the axis-aligned bounding rectangle of the polygon.

Returns:
    Rect: The bounding rectangle.
        )doc")
        .def("copy", &Polygon::copy, R"doc(
Return a copy of the polygon.

Returns:
    Polygon: A new polygon with the same points.
        )doc")
        .def("rotate", &Polygon::rotate, "angle"_a, R"doc(
Rotate the polygon around its centroid.

Args:
    angle (float): The rotation angle in radians.
        )doc")
        .def("rotated", &Polygon::rotated, "angle"_a, R"doc(
Return a rotated copy of the polygon.

Args:
    angle (float): The rotation angle in radians.

Returns:
    Polygon: A new polygon that is a rotated version of this polygon.
        )doc")
        .def("move", &Polygon::move, "offset"_a, R"doc(
Move the polygon by an offset.

Args:
    offset (Vec2): The offset to move by.
        )doc")
        .def(
            "scale_by", nb::overload_cast<double>(&Polygon::scaleBy), "factor"_a,
            R"doc(
Scale the polygon uniformly from its centroid.

Args:
    factor (float): The scaling factor.
        )doc"
        )
        .def(
            "scale_by", nb::overload_cast<const Vec2&>(&Polygon::scaleBy), "factor"_a,
            R"doc(
Scale the polygon non-uniformly from its centroid.

Args:
    factor (Vec2): The scaling factors for x and y.
        )doc"
        )
        .def(
            "scaled_by", nb::overload_cast<double>(&Polygon::scaledBy, nb::const_), "factor"_a,
            R"doc(
Return a uniformly scaled copy of the polygon.

Args:
    factor (float): The scaling factor.

Returns:
    Polygon: A new polygon that is a scaled version of this polygon.
        )doc"
        )
        .def(
            "scaled_by", nb::overload_cast<const Vec2&>(&Polygon::scaledBy, nb::const_), "factor"_a,
            R"doc(
Return a non-uniformly scaled copy of the polygon.

Args:
    factor (Vec2): The scaling factors for x and y.

Returns:
    Polygon: A new polygon that is a scaled version of this polygon.
        )doc"
        )

        .def(
            "__iter__",
            [](const Polygon& polygon) -> nb::iterator
            {
                return nb::make_iterator(
                    nb::type<Polygon>(), "iterator", polygon.points.begin(), polygon.points.end()
                );
            },
            nb::keep_alive<0, 1>()
        )
        .def(
            "__getitem__",
            [](const Polygon& polygon, const size_t i) -> Vec2
            {
                if (i >= polygon.points.size())
                    throw nb::index_error("Index out of range");
                return polygon.points[i];
            },
            "index"_a
        )
        .def("__len__", [](const Polygon& polygon) -> size_t { return polygon.points.size(); });
}
}  // namespace polygon
}  // namespace kn
