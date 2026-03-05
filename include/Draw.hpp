#pragma once

#include <nanobind/nanobind.h>
#include <nanobind/ndarray.h>

#include <memory>
#include <vector>

#include "Color.hpp"
#include "Math.hpp"

namespace nb = nanobind;

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
void _bind(nb::module_& module);
void _init(SDL_Renderer* renderer);

void circle(const Circle& circle, const Color& color, double thickness = 0.0, int numSegments = 24);
void circles(
    const std::vector<Circle>& circles, const Color& color, double thickness = 0.0,
    int numSegments = 24
);

void capsule(
    const Capsule& capsule, const Color& color, double thickness = 0.0, int numSegments = 24
);
void capsules(
    const std::vector<Capsule>& capsules, const Color& color, double thickness = 0.0,
    int numSegments = 24
);

void ellipse(Rect bounds, const Color& color, double thickness = 0.0, int numSegments = 24);
void ellipses(
    const std::vector<Rect>& bounds, const Color& color, double thickness = 0.0,
    int numSegments = 24
);

void point(Vec2 point, const Color& color);
void points(const std::vector<Vec2>& points, const Color& color);
void pointsFromNDArray(
    nb::ndarray<const double, nb::ndim<2>, nb::c_contig, nb::device::cpu> arr, const Color& color
);

void line(Line line, const Color& color, double thickness = 1.0);
void lines(const std::vector<Line>& lines, const Color& color, double thickness = 1.0);

void rect(
    Rect rect, const Color& color, int thickness = 0, double borderRadius = 0.0,
    double radiusTopLeft = -1.0, double radiusTopRight = -1.0, double radiusBottomRight = -1.0,
    double radiusBottomLeft = -1.0
);
void rects(
    const std::vector<Rect>& rects, const Color& color, int thickness = 0,
    double borderRadius = 0.0, double radiusTopLeft = -1.0, double radiusTopRight = -1.0,
    double radiusBottomRight = -1.0, double radiusBottomLeft = -1.0
);

void polygon(const Polygon& polygon, const Color& color, bool filled = true);
void polygons(const std::vector<Polygon>& polygons, const Color& color, bool filled = true);

void geometry(
    const std::shared_ptr<Texture>& texture, const std::vector<Vertex>& vertices,
    const std::vector<int>& indices = {}
);

void bezier(
    const std::vector<Vec2>& controlPoints, const Color& color, double thickness = 1.0,
    int numSegments = 24
);

void sector(
    const Circle& circle, double startAngle, double endAngle, const Color& color,
    double thickness = 0.0, int numSegments = 24
);

void polyline(
    const std::vector<Vec2>& points, const Color& color, double thickness = 1.0, bool closed = false
);
}  // namespace draw
}  // namespace kn
