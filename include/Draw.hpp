#pragma once

#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

#include <memory>
#include <vector>

#include "Color.hpp"
#include "Math.hpp"

namespace py = pybind11;

namespace kn
{
class Capsule;
class Circle;
class Line;
class Polygon;
class Rect;
class Texture;

struct Vertex
{
    Vec2 pos;
    Color color;
    Vec2 texCoord;
};

namespace draw
{
void _bind(py::module_& module);

void circle(const Circle& circle, const Color& color, double thickness = 0.0, int numSegments = 24);
void circles(
    const std::vector<Circle>& circles, const Color& color, double thickness = 0.0,
    int numSegments = 24
);

void capsule(const Capsule& capsule, const Color& color, double thickness = 0.0, int numSegments = 24);
void capsules(
    const std::vector<Capsule>& capsules, const Color& color, double thickness = 0.0,
    int numSegments = 24
);

void ellipse(Rect bounds, const Color& color, double thickness = 0.0, int numSegments = 24);
void ellipses(
    const std::vector<Rect>& rects, const Color& color, double thickness = 0.0,
    int numSegments = 24
);

void point(Vec2 point, const Color& color);
void points(const std::vector<Vec2>& points, const Color& color);
void pointsFromNDArray(
    const py::array_t<double, py::array::c_style | py::array::forcecast>& arr, const Color& color
);

void line(Line line, const Color& color, double thickness = 1.0);
void lines(const std::vector<Line>& lines, const Color& color, double thickness = 1.0);

void rect(Rect rect, const Color& color, int thickness = 0);
void rects(const std::vector<Rect>& rects, const Color& color, int thickness = 0);

void polygon(const Polygon& polygon, const Color& color, bool filled = true);
void polygons(const std::vector<Polygon>& polygons, const Color& color, bool filled = true);

void geometry(
    const std::shared_ptr<Texture>& texture, const std::vector<Vertex>& vertices,
    const std::vector<int>& indices = {}
);
}  // namespace draw
}  // namespace kn
