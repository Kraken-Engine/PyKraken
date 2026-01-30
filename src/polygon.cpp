#include "Polygon.hpp"

#include <pybind11/stl.h>

#include <cmath>
#include <limits>

#include "Math.hpp"
#include "Rect.hpp"
#include "_globals.hpp"

namespace kn
{
Polygon::Polygon(const std::vector<Vec2>& points)
    : points(points)
{
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
        sum += current.x * next.y - next.x * current.y;
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

    double cx = 0.0;
    double cy = 0.0;
    double signedArea = 0.0;

    for (size_t i = 0; i < n; ++i)
    {
        const Vec2& current = points[i];
        const Vec2& next = points[(i + 1) % n];
        double cross = current.x * next.y - next.x * current.y;
        signedArea += cross;
        cx += (current.x + next.x) * cross;
        cy += (current.y + next.y) * cross;
    }

    signedArea *= 0.5;
    if (std::abs(signedArea) < 1e-10)
    {
        double sumX = 0.0;
        double sumY = 0.0;
        for (const auto& point : points)
        {
            sumX += point.x;
            sumY += point.y;
        }
        return {sumX / n, sumY / n};
    }

    cx /= (6.0 * signedArea);
    cy /= (6.0 * signedArea);

    return {cx, cy};
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

void Polygon::rotate(double angle, const Vec2& pivot)
{
    const Rect bounds = getRect();
    const Vec2 absPivot = bounds.getTopLeft() + bounds.getSize() * pivot;

    const double cosA = std::cos(angle);
    const double sinA = std::sin(angle);

    for (auto& point : points)
    {
        const double dx = point.x - absPivot.x;
        const double dy = point.y - absPivot.y;

        point.x = absPivot.x + dx * cosA - dy * sinA;
        point.y = absPivot.y + dx * sinA + dy * cosA;
    }
}

void Polygon::translate(const Vec2& offset)
{
    for (auto& point : points)
    {
        point.x += offset.x;
        point.y += offset.y;
    }
}

void Polygon::scale(double factor, const Vec2& pivot)
{
    const Rect bounds = getRect();
    const Vec2 absPivot = bounds.getTopLeft() + bounds.getSize() * pivot;

    for (auto& point : points)
    {
        point.x = absPivot.x + (point.x - absPivot.x) * factor;
        point.y = absPivot.y + (point.y - absPivot.y) * factor;
    }
}

void Polygon::scale(const Vec2& factor, const Vec2& pivot)
{
    const Rect bounds = getRect();
    const Vec2 absPivot = bounds.getTopLeft() + bounds.getSize() * pivot;

    for (auto& point : points)
    {
        point.x = absPivot.x + (point.x - absPivot.x) * factor.x;
        point.y = absPivot.y + (point.y - absPivot.y) * factor.y;
    }
}

namespace polygon
{
void _bind(const py::module_& module)
{
    py::classh<Polygon>(module, "Polygon", R"doc(
Represents a polygon shape defined by a sequence of points.

A polygon is a closed shape made up of connected line segments. The points define
the vertices of the polygon in order. Supports various geometric operations.
    )doc")
        .def(py::init(), R"doc(
Create an empty polygon with no points.
        )doc")
        .def(py::init<const std::vector<Vec2>&>(), py::arg("points"), R"doc(
Create a polygon from a vector of Vec2 points.

Args:
    points (Sequence[Vec2]): List of Vec2 points defining the polygon vertices.
        )doc")

        .def_readwrite("points", &Polygon::points, R"doc(
The list of Vec2 points that define the polygon vertices.
        )doc")
        .def_property_readonly("perimeter", &Polygon::getPerimeter, R"doc(
Get the perimeter of the polygon.

Returns:
    float: The total distance around the polygon.
        )doc")
        .def_property_readonly("area", &Polygon::getArea, R"doc(
Get the area of the polygon.

Returns:
    float: The area enclosed by the polygon.
        )doc")
        .def_property_readonly("centroid", &Polygon::getCentroid, R"doc(
Get the centroid of the polygon.

Returns:
    Vec2: The center point of the polygon.
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
        .def(
            "rotate",
            [](Polygon& self, double angle, const py::object& pivot)
            {
                auto p = pivot.is_none() ? Anchor::CENTER : pivot.cast<Vec2>();
                self.rotate(angle, p);
            },
            py::arg("angle"), py::arg("pivot") = py::none(), R"doc(
Rotate the polygon around a pivot point.

Args:
    angle (float): The rotation angle in radians.
    pivot (Vec2, optional): The normalized point relative to the polygon's bounding box to rotate around. Defaults to center (0.5, 0.5).
        )doc"
        )
        .def("translate", &Polygon::translate, py::arg("offset"), R"doc(
Move the polygon by an offset.

Args:
    offset (Vec2): The offset to move by.
        )doc")
        .def(
            "scale",
            [](Polygon& self, double factor, const py::object& pivot)
            {
                auto p = pivot.is_none() ? Anchor::CENTER : pivot.cast<Vec2>();
                self.scale(factor, p);
            },
            py::arg("factor"), py::arg("pivot") = py::none(), R"doc(
Scale the polygon uniformly from a pivot point.

Args:
    factor (float): The scaling factor.
    pivot (Vec2, optional): The normalized point relative to the polygon's bounding box to scale from. Defaults to center (0.5, 0.5).
        )doc"
        )
        .def(
            "scale",
            [](Polygon& self, const Vec2& factor, const py::object& pivot)
            {
                auto p = pivot.is_none() ? Anchor::CENTER : pivot.cast<Vec2>();
                self.scale(factor, p);
            },
            py::arg("factor"), py::arg("pivot") = py::none(), R"doc(
Scale the polygon non-uniformly from a pivot point.

Args:
    factor (Vec2): The scaling factors for x and y.
    pivot (Vec2, optional): The normalized point relative to the polygon's bounding box to scale from. Defaults to center (0.5, 0.5).
        )doc"
        )

        .def(
            "__iter__", [](const Polygon& polygon) -> py::iterator
            { return py::make_iterator(polygon.points.begin(), polygon.points.end()); },
            py::keep_alive<0, 1>()
        )
        .def(
            "__getitem__",
            [](const Polygon& polygon, const size_t i) -> Vec2
            {
                if (i >= polygon.points.size())
                    throw py::index_error("Index out of range");
                return polygon.points[i];
            },
            py::arg("index")
        )
        .def("__len__", [](const Polygon& polygon) -> size_t { return polygon.points.size(); });
}
}  // namespace polygon
}  // namespace kn
