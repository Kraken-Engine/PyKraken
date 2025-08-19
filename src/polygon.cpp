#include "Polygon.hpp"
#include "Math.hpp"

#include <pybind11/stl.h>

namespace polygon
{
void _bind(py::module_& module)
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

        .def(py::init(
                 [](const py::sequence& pointsSeq) -> Polygon
                 {
                     std::vector<Vec2> points;
                     for (auto item : pointsSeq)
                     {
                         if (py::isinstance<Vec2>(item))
                         {
                             points.push_back(item.cast<Vec2>());
                         }
                         else if (py::isinstance<py::sequence>(item))
                         {
                             py::sequence point = item.cast<py::sequence>();
                             if (point.size() != 2)
                                 throw std::invalid_argument(
                                     "Each point must be a 2-element sequence");
                             points.emplace_back(point[0].cast<double>(), point[1].cast<double>());
                         }
                         else
                         {
                             throw std::invalid_argument(
                                 "Points must be Vec2 objects or 2-element sequences");
                         }
                     }
                     return {points};
                 }),
             py::arg("points"), R"doc(
Create a polygon from a sequence of points.

Args:
    points: A sequence of Vec2 objects or 2-element sequences [[x, y], ...].

Raises:
    ValueError: If points are not properly formatted.
        )doc")

        .def(
            "__iter__", [](const Polygon& polygon) -> py::iterator
            { return py::make_iterator(polygon.points.begin(), polygon.points.end()); },
            py::keep_alive<0, 1>(), R"doc(
Return an iterator over the polygon's points.
        )doc")

        .def(
            "__getitem__",
            [](const Polygon& polygon, size_t i) -> Vec2
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
        )doc");

    py::implicitly_convertible<py::sequence, Polygon>();
}
} // namespace polygon

Polygon::Polygon(const std::vector<Vec2>& points) : points(points) {}

Polygon Polygon::copy() const { return {points}; }
