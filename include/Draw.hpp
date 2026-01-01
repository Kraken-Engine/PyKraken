#pragma once

#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace kn
{
class Circle;
class Vec2;
class Line;
struct Color;
class Polygon;
class Rect;

namespace draw
{
void _bind(py::module_& module);

void circle(const Circle& circle, const Color& color, int thickness = 0);

void ellipse(Rect bounds, const Color& color, bool filled = false);

void point(Vec2 point, const Color& color);
void points(const std::vector<Vec2>& points, const Color& color);
void pointsFromNDArray(
    const py::array_t<double, py::array::c_style | py::array::forcecast>& arr, const Color& color
);

void line(const Line& line, const Color& color, int thickness = 1);
// void drawLines(const std::vector<Line>& lines, const Color& color, int thickness = 1);

void rect(Rect rect, const Color& color, int thickness = 0);
void rects(const std::vector<Rect>& rects, const Color& color, int thickness = 0);

void polygon(const Polygon& polygon, const Color& color, bool filled = false);
// void drawPolygons(const std::vector<Polygon>& polygons, const Color& color, bool filled = false);
}  // namespace draw
}  // namespace kn
