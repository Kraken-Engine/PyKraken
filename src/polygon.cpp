#include "Polygon.hpp"

#ifdef KRAKEN_ENABLE_PYTHON
#include <nanobind/make_iterator.h>
#include <nanobind/stl/vector.h>
#endif  // KRAKEN_ENABLE_PYTHON

#include <cmath>
#include <limits>

#include "Math.hpp"
#include "Rect.hpp"
#include "_globals.hpp"

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

namespace kn
{
Polygon::Polygon(const std::vector<Vec2>& points)
    : points(points)
{
}

Polygon::Polygon(const uint32_t n, const double radius, const Vec2& centroid)
{
    if (n == 0)
        return;

    points.reserve(n);
    const double angleStep = 2.0 * M_PI / n;
    for (uint32_t i = 0; i < n; ++i)
    {
        const double angle = i * angleStep;
        points.emplace_back(radius * std::cos(angle), radius * std::sin(angle));
    }

    setCentroid(centroid);
}

Polygon Polygon::copy() const
{
    return Polygon{points};
}

double Polygon::getPerimeter() const
{
    if (points.size() < 2)
        return 0.0;

    double distance = 0.0;
    for (size_t i = 0; i < points.size(); ++i)
    {
        const Vec2& current = points[i];
        const Vec2& next = points[(i + 1) % points.size()];
        distance += current.distanceTo(next);
    }

    return distance;
}

double Polygon::getArea() const
{
    if (points.size() < 3)
        return 0.0;

    double sum = 0.0;
    for (size_t i = 0; i < points.size(); ++i)
    {
        const Vec2& current = points[i];
        const Vec2& next = points[(i + 1) % points.size()];
        sum += math::cross(current, next);
    }

    return std::abs(sum) * 0.5;
}

Vec2 Polygon::getCentroid() const
{
    if (points.empty())
        return {};
    const size_t n = points.size();

    if (n == 1)
        return points[0];
    if (n == 2)
        return {(points[0].x + points[1].x) * 0.5, (points[0].y + points[1].y) * 0.5};

    Vec2 centroid{0.0, 0.0};
    double signedArea = 0.0;

    for (size_t i = 0; i < n; ++i)
    {
        const Vec2& current = points[i];
        const Vec2& next = points[(i + 1) % n];
        const double cross = math::cross(current, next);
        signedArea += cross;
        centroid += (current + next) * cross;
    }

    signedArea *= 0.5;
    if (std::abs(signedArea) < 1e-10)
    {
        Vec2 sum{0.0, 0.0};
        for (const auto& point : points)
            sum += point;
        return sum / n;
    }

    centroid /= (6.0 * signedArea);
    return centroid;
}

void Polygon::setCentroid(const Vec2& centroid)
{
    const Vec2 currentCentroid = getCentroid();
    const Vec2 offset = centroid - currentCentroid;
    move(offset);
}

bool Polygon::isConvex() const
{
    if (points.size() < 3)
        return false;

    bool initialized = false;
    bool positive = false;
    const size_t n = points.size();

    for (size_t i = 0; i < n; ++i)
    {
        const Vec2& p1 = points[i];
        const Vec2& p2 = points[(i + 1) % n];
        const Vec2& p3 = points[(i + 2) % n];

        double cp = math::cross(p2 - p1, p3 - p2);
        if (std::abs(cp) > 1e-10)  // Ignore very small cross products
        {
            if (!initialized)
            {
                positive = cp > 0;
                initialized = true;
            }
            else if (positive != (cp > 0))
            {
                return false;
            }
        }
    }

    return true;
}

bool Polygon::isConcave() const
{
    return !isConvex();
}

Rect Polygon::getRect() const
{
    if (points.empty())
        return {};

    double minX = std::numeric_limits<double>::max();
    double minY = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::lowest();
    double maxY = std::numeric_limits<double>::lowest();

    for (const auto& point : points)
    {
        minX = std::min(minX, point.x);
        minY = std::min(minY, point.y);
        maxX = std::max(maxX, point.x);
        maxY = std::max(maxY, point.y);
    }

    return {minX, minY, maxX - minX, maxY - minY};
}

void Polygon::rotate(double angle)
{
    const Vec2 absPivot = getCentroid();
    for (auto& point : points)
        point = absPivot + (point - absPivot).rotated(angle);
}

Polygon Polygon::rotated(double angle) const
{
    Polygon p = *this;
    p.rotate(angle);
    return p;
}

void Polygon::move(const Vec2& offset)
{
    for (auto& point : points)
        point += offset;
}

Polygon Polygon::moved(const Vec2& offset) const
{
    Polygon p = *this;
    p.move(offset);
    return p;
}

void Polygon::scaleBy(double factor)
{
    const Vec2 absPivot = getCentroid();
    for (auto& point : points)
        point = absPivot + (point - absPivot) * factor;
}

void Polygon::scaleBy(const Vec2& factor)
{
    const Vec2 absPivot = getCentroid();
    for (auto& point : points)
        point = absPivot + (point - absPivot) * factor;
}

Polygon Polygon::scaledBy(double factor) const
{
    Polygon p = *this;
    p.scaleBy(factor);
    return p;
}

Polygon Polygon::scaledBy(const Vec2& factor) const
{
    Polygon p = *this;
    p.scaleBy(factor);
    return p;
}

#ifdef KRAKEN_ENABLE_PYTHON
namespace polygon
{
void _bind(const nb::module_& module)
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
#endif  // KRAKEN_ENABLE_PYTHON

}  // namespace kn
