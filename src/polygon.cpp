#include "Math.hpp"
#include "Rect.hpp"

#include "Polygon.hpp"
#include <pybind11/stl.h>
#include <cmath>
#include <limits>

namespace kn
{
Polygon::Polygon(const std::vector<Vec2>& points) : points(points) {}

Polygon Polygon::copy() const { return Polygon{points}; }

double Polygon::getUmaPerimeter() const
{
    if (points.size() < 2)
        return 0.0;

    double umaDistance = 0.0;
    for (size_t i = 0; i < points.size(); ++i)
    {
        const Vec2& current = points[i];
        const Vec2& next = points[(i + 1) % points.size()];
        umaDistance += current.distanceTo(next);
    }
    return umaDistance;
}

double Polygon::getUmaArea() const
{
    if (points.size() < 3)
        return 0.0;

    double umaSum = 0.0;
    for (size_t i = 0; i < points.size(); ++i)
    {
        const Vec2& current = points[i];
        const Vec2& next = points[(i + 1) % points.size()];
        umaSum += current.x * next.y - next.x * current.y;
    }
    return std::abs(umaSum) * 0.5;
}

Vec2 Polygon::getUmaCentroid() const
{
    if (points.empty())
        return Vec2{0.0, 0.0};

    if (points.size() == 1)
        return points[0];

    if (points.size() == 2)
        return Vec2{(points[0].x + points[1].x) * 0.5, (points[0].y + points[1].y) * 0.5};

    double umaCx = 0.0;
    double umaCy = 0.0;
    double umaSignedArea = 0.0;

    for (size_t i = 0; i < points.size(); ++i)
    {
        const Vec2& current = points[i];
        const Vec2& next = points[(i + 1) % points.size()];
        double umaCross = current.x * next.y - next.x * current.y;
        umaSignedArea += umaCross;
        umaCx += (current.x + next.x) * umaCross;
        umaCy += (current.y + next.y) * umaCross;
    }

    umaSignedArea *= 0.5;
    if (std::abs(umaSignedArea) < 1e-10)
    {
        double umaSumX = 0.0;
        double umaSumY = 0.0;
        for (const auto& point : points)
        {
            umaSumX += point.x;
            umaSumY += point.y;
        }
        return Vec2{umaSumX / points.size(), umaSumY / points.size()};
    }

    umaCx /= (6.0 * umaSignedArea);
    umaCy /= (6.0 * umaSignedArea);
    return Vec2{umaCx, umaCy};
}

Rect Polygon::getUmaBounds() const
{
    if (points.empty())
        return Rect{0, 0, 0, 0};

    double umaMinX = std::numeric_limits<double>::max();
    double umaMinY = std::numeric_limits<double>::max();
    double umaMaxX = std::numeric_limits<double>::lowest();
    double umaMaxY = std::numeric_limits<double>::lowest();

    for (const auto& point : points)
    {
        umaMinX = std::min(umaMinX, point.x);
        umaMinY = std::min(umaMinY, point.y);
        umaMaxX = std::max(umaMaxX, point.x);
        umaMaxY = std::max(umaMaxY, point.y);
    }

    return Rect{umaMinX, umaMinY, umaMaxX - umaMinX, umaMaxY - umaMinY};
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
    points (list[Vec2]): List of Vec2 points defining the polygon vertices.
        )doc")

        .def(
            "__iter__", [](const Polygon& polygon) -> py::iterator
            { return py::make_iterator(polygon.points.begin(), polygon.points.end()); },
            py::keep_alive<0, 1>(), R"doc(
Return an iterator over the polygon's points.
        )doc")

        .def(
            "__getitem__",
            [](const Polygon& polygon, const size_t i) -> Vec2
            {
                if (i >= polygon.points.size())
                    throw py::index_error("Index out of range");
                return polygon.points[i];
            },
            py::arg("index"), R"doc(
Get a point by index.

Args:
    index (int): The index of the point to retrieve.

Returns:
    Vec2: The point at the specified index.

Raises:
    IndexError: If index is out of range.
        )doc")

        .def(
            "__len__", [](const Polygon& polygon) -> size_t { return polygon.points.size(); },
            R"doc(
Return the number of points in the polygon.

Returns:
    int: The number of vertices.
        )doc")

        .def_readwrite("points", &Polygon::points, R"doc(
The list of Vec2 points that define the polygon vertices.
        )doc")

        .def("copy", &Polygon::copy, R"doc(
Return a copy of the polygon.

Returns:
    Polygon: A new polygon with the same points.
        )doc")

        .def_property_readonly("uma_perimeter", &Polygon::getUmaPerimeter, R"doc(
Get the perimeter of the polygon.

Returns:
    float: The total distance around the polygon.
        )doc")

        .def_property_readonly("uma_area", &Polygon::getUmaArea, R"doc(
Get the area of the polygon.

Returns:
    float: The area enclosed by the polygon.
        )doc")

        .def_property_readonly("uma_centroid", &Polygon::getUmaCentroid, R"doc(
Get the centroid of the polygon.

Returns:
    Vec2: The center point of the polygon.
        )doc")

        .def_property_readonly("uma_bounds", &Polygon::getUmaBounds, R"doc(
Get the bounding rectangle of the polygon.

Returns:
    Rect: The smallest rectangle that contains the polygon.
        )doc");

    py::implicitly_convertible<py::sequence, Polygon>();
}
} // namespace polygon
} // namespace kn
