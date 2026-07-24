#include "kraken/graphics/Draw.hpp"

#include <nanobind/nanobind.h>
#include <nanobind/ndarray.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>

#include <algorithm>
#include <array>
#include <sstream>

#include "bindings/python/bindings.hpp"
#include "geometry/earcut.hpp"
#include "kraken/geometry/Capsule.hpp"
#include "kraken/geometry/Circle.hpp"
#include "kraken/geometry/Line.hpp"
#include "kraken/geometry/Polygon.hpp"
#include "kraken/geometry/Rect.hpp"
#include "kraken/graphics/Camera.hpp"
#include "kraken/graphics/Renderer.hpp"
#include "kraken/graphics/Texture.hpp"

namespace kn::draw
{
// Accept a NumPy ndarray with shape (N,2) and dtype float64 for the fastest path.
void pointsFromNDArray(
    nb::ndarray<const double, nb::ndim<2>, nb::c_contig, nb::device::cpu> arr, const Color& color
)
{
    SDL_Renderer* rend = renderer::_get();
    if (!rend)
        throw std::runtime_error("Renderer not yet initialized");

    if (color.a == 0)
        return;

    if (arr.ndim() != 2 || arr.shape(1) != 2)
        throw std::invalid_argument("Expected array shape (N,2)");

    const auto n = static_cast<size_t>(arr.shape(0));
    if (n == 0)
        return;

    if (!SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a))
        throw std::runtime_error("Failed to set draw color: " + std::string(SDL_GetError()));

    std::vector<SDL_FPoint> sdlPoints;
    sdlPoints.reserve(n);

    const auto* data = arr.data();
    const Vec2 rendRes = renderer::getCurrentResolution();
    for (size_t i = 0; i < n; ++i)
    {
        Vec2 pos = {data[i * 2], data[i * 2 + 1]};
        pos = camera::worldToScreen(pos);
        if (pos.x >= 0.0 && pos.y >= 0.0 && pos.x < rendRes.x && pos.y < rendRes.y)
            sdlPoints.push_back(static_cast<SDL_FPoint>(pos));
    }

    if (!SDL_RenderPoints(rend, sdlPoints.data(), static_cast<int>(sdlPoints.size())))
        throw std::runtime_error("Failed to render points: " + std::string(SDL_GetError()));
}
}  // namespace kn::draw

namespace kn::draw
{
void _bind(nb::module_& module)
{
    using namespace nb::literals;

    nb::class_<Vertex>(
        module, "Vertex", nb::pooled(KRAKEN_PYTHON_POOL_CAPACITY),
        "A vertex with position, color, and texture coordinates."
    )
        .def(
            "__init__",
            [](Vertex* self, const Vec2& position, std::optional<Color> color,
               std::optional<Vec2> texCoord) -> void
            {
                new (self)
                    Vertex{position, color.value_or(Color::WHITE), texCoord.value_or(Vec2{})};
            },
            "position"_a, "color"_a = nb::none(), "tex_coord"_a = nb::none(),
            R"doc(
Create a new Vertex.

Args:
    position (Vec2): The position of the vertex in world space.
    color (Color | None): The color of the vertex. Defaults to White.
    tex_coord (Vec2 | None): The texture coordinate of the vertex. Defaults to (0, 0).
            )doc"
        )
        .def_rw("position", &Vertex::pos, "Position of the vertex in world space.")
        .def_rw("color", &Vertex::color, "Color of the vertex.")
        .def_rw("tex_coord", &Vertex::texCoord, "Texture coordinate of the vertex.")
        .def(
            "__repr__",
            [](const Vertex& self) -> std::string
            {
                std::stringstream ss;
                ss << "Vertex(pos=(" << self.pos.x << ", " << self.pos.y << "), color=("
                   << self.color.r << ", " << self.color.g << ", " << self.color.b << ", "
                   << self.color.a << "), tex_coord=(" << self.texCoord.x << ", " << self.texCoord.y
                   << "))";
                return ss.str();
            }
        );

    auto subDraw = module.def_submodule("draw", "Functions for drawing shape objects");

    subDraw.def("point", &point, "point"_a, "color"_a, R"doc(
Draw a single point to the renderer.

Args:
    point (Vec2): The position of the point.
    color (Color): The color of the point.

Raises:
    RuntimeError: If point rendering fails.
    )doc");

    subDraw.def("points", &points, "points"_a, "color"_a, R"doc(
Batch draw an array of points to the renderer.

Args:
    points (Sequence[Vec2]): The points to batch draw.
    color (Color): The color of the points.

Raises:
    RuntimeError: If point rendering fails.
    )doc");

    subDraw.def(
        "points_from_ndarray", &pointsFromNDArray, "points"_a, "color"_a,
        R"doc(
Batch draw points from a NumPy array.

This fast path accepts a contiguous NumPy array of shape (N,2) (dtype float64) and
reads coordinates directly with minimal overhead. Use this to measure the best-case
zero-copy/buffer-backed path.

Args:
    points (numpy.ndarray): Array with shape (N,2) containing x,y coordinates.
    color (Color): The color of the points.

Raises:
    ValueError: If the array shape is not (N,2).
    RuntimeError: If point rendering fails.
    )doc"
    );

    subDraw.def(
        "circle", &circle, "circle"_a, "color"_a, "thickness"_a = 0, "num_segments"_a = 24, R"doc(
Draw a circle to the renderer.

Args:
    circle (Circle): The circle to draw.
    color (Color): The color of the circle.
    thickness (float, optional): The line thickness. If <= 0 or >= radius, draws filled circle. Defaults to 0 (filled).
    num_segments (int, optional): Number of segments to approximate the circle. Higher values yield smoother circles. Defaults to 24.
    )doc"
    );

    subDraw.def(
        "circles", &circles, "circles"_a, "color"_a, "thickness"_a = 0, "num_segments"_a = 24,
        R"doc(
Draw an array of circles in bulk to the renderer.

Args:
    circles (Sequence[Circle]): The circles to draw in bulk.
    color (Color): The color of the circles.
    thickness (float, optional): The line thickness. If <= 0 or >= radius, draws filled circle. Defaults to 0 (filled).
    num_segments (int, optional): Number of segments to approximate each circle. Higher values yield smoother circles. Defaults to 24.
    )doc"
    );

    subDraw.def(
        "capsule", &capsule, "capsule"_a, "color"_a, "thickness"_a = 0, "num_segments"_a = 24,
        R"doc(
Draw a capsule to the renderer.

Args:
    capsule (Capsule): The capsule to draw.
    color (Color): The color of the capsule.
    thickness (float, optional): The line thickness. If <= 0 or >= radius, draws filled capsule. Defaults to 0 (filled).
    num_segments (int, optional): Number of segments to approximate the capsule ends. Higher values yield smoother capsules. Defaults to 24.
    )doc"
    );

    subDraw.def(
        "capsules", &capsules, "capsules"_a, "color"_a, "thickness"_a = 0, "num_segments"_a = 24,
        R"doc(
Draw an array of capsules in bulk to the renderer.

Args:
    capsules (Sequence[Capsule]): The capsules to draw in bulk.
    color (Color): The color of the capsules.
    thickness (float, optional): The line thickness. If <= 0 or >= radius, draws filled capsules. Defaults to 0 (filled).
    num_segments (int, optional): Number of segments to approximate each capsule end. Higher values yield smoother capsules. Defaults to 24.
    )doc"
    );

    subDraw.def(
        "ellipse", &ellipse, "bounds"_a, "color"_a, "thickness"_a = 0.0, "num_segments"_a = 24,
        R"doc(
Draw an ellipse to the renderer.

Args:
    bounds (Rect): The bounding box of the ellipse.
    color (Color): The color of the ellipse.
    thickness (float, optional): The line thickness. If <= 0 or >= radius, draws filled ellipse. Defaults to 0 (filled).
    num_segments (int, optional): Number of segments to approximate the ellipse. Higher values yield smoother ellipses. Defaults to 24.
    )doc"
    );

    subDraw.def(
        "ellipses", &ellipses, "bounds"_a, "color"_a, "thickness"_a = 0.0, "num_segments"_a = 24,
        R"doc(
Draw an array of ellipses in bulk to the renderer.

Args:
    bounds (Sequence[Rect]): The bounding boxes of the ellipses to draw in bulk.
    color (Color): The color of the ellipses.
    thickness (float, optional): The line thickness. If <= 0 or >= radius, draws filled ellipses. Defaults to 0 (filled).
    num_segments (int, optional): Number of segments to approximate each ellipse. Higher values yield smoother ellipses. Defaults to 24.
    )doc"
    );

    subDraw.def(
        "line", &line, "line"_a, "color"_a, "thickness"_a = 1.0,
        R"doc(
Draw a line to the renderer.

Args:
    line (Line): The line to draw.
    color (Color): The color of the line.
    thickness (float, optional): The line thickness in pixels. Defaults to 1.0.
    )doc"
    );

    subDraw.def(
        "lines", &lines, "lines"_a, "color"_a, "thickness"_a = 1.0,
        R"doc(
Batch draw an array of lines to the renderer.

Args:
    lines (Sequence[Line]): The lines to batch draw.
    color (Color): The color of the lines.
    thickness (float, optional): The line thickness in pixels. Defaults to 1.0.
    )doc"
    );

    subDraw.def(
        "rect", &rect, "rect"_a, "color"_a, "thickness"_a = 0, "border_radius"_a = 0.0,
        "radius_top_left"_a = -1.0, "radius_top_right"_a = -1.0, "radius_bottom_right"_a = -1.0,
        "radius_bottom_left"_a = -1.0,
        R"doc(
Draw a rectangle to the renderer.

Args:
    rect (Rect): The rectangle to draw.
    color (Color): The color of the rectangle.
    thickness (int, optional): The border thickness. If 0 or >= half width/height, draws filled rectangle. Defaults to 0 (filled).
    border_radius (float, optional): Uniform corner radius for all four corners. Defaults to 0.
    radius_top_left (float, optional): Override radius for the top-left corner. -1 to ignore.
    radius_top_right (float, optional): Override radius for the top-right corner. -1 to ignore.
    radius_bottom_right (float, optional): Override radius for the bottom-right corner. -1 to ignore.
    radius_bottom_left (float, optional): Override radius for the bottom-left corner. -1 to ignore.
    )doc"
    );

    subDraw.def(
        "rects", &rects, "rects"_a, "color"_a, "thickness"_a = 0, "border_radius"_a = 0.0,
        "radius_top_left"_a = -1.0, "radius_top_right"_a = -1.0, "radius_bottom_right"_a = -1.0,
        "radius_bottom_left"_a = -1.0,
        R"doc(
Batch draw an array of rectangles to the renderer.

Args:
    rects (Sequence[Rect]): The rectangles to batch draw.
    color (Color): The color of the rectangles.
    thickness (int, optional): The border thickness of the rectangles. If 0 or >= half width/height, draws filled rectangles. Defaults to 0 (filled).
    border_radius (float, optional): Uniform corner radius for all four corners. Defaults to 0.
    radius_top_left (float, optional): Override radius for the top-left corner of all rectangles. -1 to ignore.
    radius_top_right (float, optional): Override radius for the top-right corner of all rectangles. -1 to ignore.
    radius_bottom_right (float, optional): Override radius for the bottom-right corner of all rectangles. -1 to ignore.
    radius_bottom_left (float, optional): Override radius for the bottom-left corner of all rectangles. -1 to ignore.
    )doc"
    );

    subDraw.def(
        "polygon", &polygon, "polygon"_a, "color"_a, "filled"_a = true,
        R"doc(
Draw a polygon to the renderer.

Args:
    polygon (Polygon): The polygon to draw.
    color (Color): The color of the polygon.
    filled (bool, optional): Whether to draw a filled polygon or just the outline. Defaults to True.
    )doc"
    );

    subDraw.def(
        "polygons", &polygons, "polygons"_a, "color"_a, "filled"_a = true,
        R"doc(
Draw an array of polygons in bulk to the renderer.

Args:
    polygons (Sequence[Polygon]): The polygons to draw in bulk.
    color (Color): The color of the polygons.
    filled (bool, optional): Whether to draw filled polygons or just the outlines. Defaults to True (filled).
    )doc"
    );

    subDraw.def(
        "geometry", &geometry, "texture"_a.none(), "vertices"_a, "indices"_a = std::vector<int>{},
        R"doc(
Draw arbitrary geometry using vertices and optional indices.

Args:
    texture (Texture | None): The texture to apply to the geometry. Can be None.
    vertices (Sequence[Vertex]): A list of Vertex objects.
    indices (Sequence[int] | None): A list of indices defining the primitives.
                                   If None or empty, vertices are drawn sequentially.
        )doc"
    );

    subDraw.def(
        "bezier", &bezier, "control_points"_a, "color"_a, "thickness"_a = 1.0,
        "num_segments"_a = 24, R"doc(
Draw a Bezier curve with 3 or 4 control points.

Args:
    control_points (Sequence[Vec2]): The control points (3 for quadratic, 4 for cubic).
    color (Color): The color of the curve.
    thickness (float, optional): The line thickness. Defaults to 1.0.
    num_segments (int, optional): Number of segments to approximate the curve. Defaults to 24.
    )doc"
    );

    subDraw.def(
        "sector", &sector, "circle"_a, "start_angle"_a, "end_angle"_a, "color"_a,
        "thickness"_a = 0.0, "num_segments"_a = 24,
        R"doc(
Draw a circular sector or arc.

Args:
    circle (Circle): The circle defining the sector.
    start_angle (float): The start angle in radians.
    end_angle (float): The end angle in radians.
    color (Color): The color of the sector.
    thickness (float, optional): The line thickness. If <= 0 or >= radius, draws filled sector. Defaults to 0 (filled).
    num_segments (int, optional): Number of segments to approximate the arc. Defaults to 24.
    )doc"
    );

    subDraw.def(
        "polyline", &polyline, "points"_a, "color"_a, "thickness"_a = 1.0, "closed"_a = false,
        R"doc(
Draw connected line segments through a sequence of points.

Args:
    points (Sequence[Vec2]): The vertices of the polyline (must have at least 2).
    color (Color): The color of the polyline.
    thickness (float, optional): The line thickness in pixels. Defaults to 1.0.
    closed (bool, optional): If True, connects the last point back to the first. Defaults to False.
    )doc"
    );
}
}  // namespace kn::draw
