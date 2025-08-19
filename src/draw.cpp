#include "Draw.hpp"
#include "Camera.hpp"
#include "Circle.hpp"
#include "Color.hpp"
#include "Line.hpp"
#include "Math.hpp"
#include "Polygon.hpp"
#include "Rect.hpp"
#include "Renderer.hpp"

#include <gfx/SDL3_gfxPrimitives.h>
#include <pybind11/stl.h>
#include <set>

static void _circleThin(SDL_Renderer* renderer, Vec2 center, int radius, const Color& color);
static void _circle(SDL_Renderer* renderer, Vec2 center, int radius, const Color& color,
                    int thickness);
static Uint64 packPoint(int x, int y);

namespace draw
{
void _bind(py::module_& module)
{
    auto subDraw = module.def_submodule("draw", "Functions for drawing shape objects");

    subDraw.def("point", &point, py::arg("point"), py::arg("color"), R"doc(
Draw a single point to the renderer.

Args:
    point (Vec2): The position of the point.
    color (Color): The color of the point.

Raises:
    RuntimeError: If point rendering fails.
    )doc");

    subDraw.def("points", &points, py::arg("points"), py::arg("color"), R"doc(
Batch draw an array of points to the renderer.

Args:
    points (Sequence[Vec2]): The points to batch draw.
    color (Color): The color of the points.

Raises:
    RuntimeError: If point rendering fails.
    )doc");

    subDraw.def("points_from_ndarray", &pointsFromNDarray, py::arg("points"), py::arg("color"),
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
    )doc");

    subDraw.def("circle", &circle, py::arg("circle"), py::arg("color"), py::arg("thickness") = 0,
                R"doc(
Draw a circle to the renderer.

Args:
    circle (Circle): The circle to draw.
    color (Color): The color of the circle.
    thickness (int, optional): The line thickness. If 0 or >= radius, draws filled circle.
                              Defaults to 0 (filled).
    )doc");

    subDraw.def("line", &line, py::arg("line"), py::arg("color"), py::arg("thickness") = 1,
                R"doc(
Draw a line to the renderer.

Args:
    line (Line): The line to draw.
    color (Color): The color of the line.
    thickness (int, optional): The line thickness in pixels. Defaults to 1.
    )doc");

    subDraw.def("rect", &rect, py::arg("rect"), py::arg("color"), py::arg("thickness") = 0,
                R"doc(
Draw a rectangle to the renderer.

Args:
    rect (Rect): The rectangle to draw.
    color (Color): The color of the rectangle.
    thickness (int, optional): The border thickness. If 0 or >= half width/height, draws filled rectangle. Defaults to 0 (filled).
    )doc");

    subDraw.def("rects", &rects, py::arg("rects"), py::arg("color"), py::arg("thickness") = 0,
                R"doc(
Batch draw an array of rectangles to the renderer.

Args:
    rects (Sequence[Rect]): The rectangles to batch draw.
    color (Color): The color of the rectangles.
    thickness (int, optional): The border thickness of the rectangles. If 0 or >= half width/height, draws filled rectangles. Defaults to 0 (filled).
    )doc");

    subDraw.def("polygon", &polygon, py::arg("polygon"), py::arg("color"),
                py::arg("filled") = false,
                R"doc(
Draw a polygon to the renderer.

Args:
    polygon (Polygon): The polygon to draw.
    color (Color): The color of the polygon.
    filled (bool, optional): Whether to draw a filled polygon or just the outline.
                             Defaults to False (outline). Works with both convex and concave polygons.
    )doc");
}

void point(const Vec2& point, const Color& color)
{
    SDL_Renderer* rend = renderer::get();
    SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a);

    SDL_FPoint sdlPoint = point - camera::getActivePos();
    if (!SDL_RenderPoint(rend, sdlPoint.x, sdlPoint.y))
        throw std::runtime_error("Failed to render point: " + std::string(SDL_GetError()));
}

void points(const std::vector<Vec2>& points, const Color& color)
{
    if (points.empty())
        return;

    SDL_Renderer* rend = renderer::get();

    SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a);

    std::vector<SDL_FPoint> sdlPoints;
    sdlPoints.reserve(points.size());

    const Vec2 cameraPos = camera::getActivePos();
    const Vec2 max = renderer::getResolution() - cameraPos;
    const Vec2 min = -cameraPos;
    for (const Vec2& point : points)
    {
        const Vec2 pos = point - cameraPos;
        if (min < pos && pos < max)
            sdlPoints.emplace_back(pos);
    }

    if (!SDL_RenderPoints(rend, sdlPoints.data(), static_cast<int>(sdlPoints.size())))
        throw std::runtime_error("Failed to render points: " + std::string(SDL_GetError()));
}

// Accept a NumPy ndarray with shape (N,2) and dtype float64 for the fastest path.
void pointsFromNDarray(py::array_t<double, py::array::c_style | py::array::forcecast> arr,
                       const Color& color)
{
    auto info = arr.request();
    if (info.ndim != 2 || info.shape[1] != 2)
        throw std::invalid_argument("Expected array shape (N,2)");

    const auto n = static_cast<size_t>(info.shape[0]);
    if (n == 0)
        return;

    const double* data = static_cast<double*>(info.ptr);

    SDL_Renderer* rend = renderer::get();
    SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a);

    std::vector<SDL_FPoint> sdlPoints;
    sdlPoints.reserve(n);

    const Vec2 res = renderer::getResolution();
    const Vec2 cameraPos = camera::getActivePos();
    const Vec2 zero;

    for (size_t i = 0; i < n; ++i)
    {
        Vec2 pos = {data[i * 2 + 0], data[i * 2 + 1]};
        pos -= cameraPos;
        if (pos > zero && pos < res)
            sdlPoints.emplace_back(pos);
    }

    if (!SDL_RenderPoints(rend, sdlPoints.data(), static_cast<int>(sdlPoints.size())))
        throw std::runtime_error("Failed to render points: " + std::string(SDL_GetError()));
}

void circle(const Circle& circle, const Color& color, const int thickness)
{
    if (circle.radius < 1)
        return;

    SDL_Renderer* rend = renderer::get();
    SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a);

    if (thickness == 1)
        _circleThin(rend, circle.pos - camera::getActivePos(), circle.radius, color);
    else
        _circle(rend, circle.pos - camera::getActivePos(), circle.radius, color, thickness);
}

void line(const Line& line, const Color& color, const int thickness)
{
    const Vec2 cameraPos = camera::getActivePos();
    const auto x1 = static_cast<Sint16>(line.ax - cameraPos.x);
    const auto y1 = static_cast<Sint16>(line.ay - cameraPos.y);
    const auto x2 = static_cast<Sint16>(line.bx - cameraPos.x);
    const auto y2 = static_cast<Sint16>(line.by - cameraPos.y);

    if (thickness <= 1)
        lineRGBA(renderer::get(), x1, y1, x2, y2, color.r, color.g, color.b, color.a);
    else
        thickLineRGBA(renderer::get(), x1, y1, x2, y2, thickness, color.r, color.g, color.b,
                      color.a);
}

void rect(Rect rect, const Color& color, const int thickness)
{
    SDL_Renderer* rend = renderer::get();
    SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a);

    const Vec2 cameraPos = camera::getActivePos();
    rect.x -= cameraPos.x;
    rect.y -= cameraPos.y;
    SDL_FRect sdlRect = rect;

    const auto halfWidth = static_cast<int>(rect.w / 2.0);
    const auto halfHeight = static_cast<int>(rect.h / 2.0);
    if (thickness <= 0 || thickness > halfWidth || thickness > halfHeight)
    {
        SDL_RenderFillRect(rend, &sdlRect);
        return;
    }

    SDL_RenderRect(rend, &sdlRect);
    for (int i = 1; i < thickness; i++)
    {
        rect.inflate({-2, -2});
        sdlRect = rect;
        SDL_RenderRect(rend, &sdlRect);
    }
}

void rects(const std::vector<Rect>& rects, const Color& color, const int thickness)
{
    if (rects.empty())
        return;

    SDL_Renderer* rend = renderer::get();
    SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a);

    const Vec2 cameraPos = camera::getActivePos();

    // Convert to SDL_FRect array with camera offset
    std::vector<SDL_FRect> sdlRects;
    sdlRects.reserve(rects.size());

    for (const Rect& rect : rects)
    {
        SDL_FRect sdlRect;
        sdlRect.x = rect.x - cameraPos.x;
        sdlRect.y = rect.y - cameraPos.y;
        sdlRect.w = rect.w;
        sdlRect.h = rect.h;
        sdlRects.push_back(sdlRect);
    }

    // For filled rectangles or thick outlines, use batch fill
    if (thickness <= 0)
        SDL_RenderFillRects(rend, sdlRects.data(), static_cast<int>(sdlRects.size()));
    else
    {
        // For outlined rectangles, use batch outline
        SDL_RenderRects(rend, sdlRects.data(), static_cast<int>(sdlRects.size()));

        // For thick outlines, draw additional nested rectangles
        if (thickness > 1)
        {
            for (int i = 1; i < thickness; i++)
            {
                std::vector<SDL_FRect> innerRects;
                innerRects.reserve(rects.size());

                for (const Rect& rect : rects)
                {
                    const auto halfWidth = static_cast<int>(rect.w / 2.0);
                    const auto halfHeight = static_cast<int>(rect.h / 2.0);

                    // Skip if thickness would exceed rectangle dimensions
                    if (i >= halfWidth || i >= halfHeight)
                        continue;

                    SDL_FRect innerRect;
                    innerRect.x = rect.x - cameraPos.x + i;
                    innerRect.y = rect.y - cameraPos.y + i;
                    innerRect.w = rect.w - (2 * i);
                    innerRect.h = rect.h - (2 * i);
                    innerRects.push_back(innerRect);
                }

                if (!innerRects.empty())
                    SDL_RenderRects(rend, innerRects.data(), static_cast<int>(innerRects.size()));
            }
        }
    }
}

void polygon(const Polygon& polygon, const Color& color, const bool filled)
{
    const size_t size = polygon.points.size();
    if (size == 0)
        return;
    if (size == 1)
    {
        point(polygon.points.at(0), color);
        return;
    }
    if (size == 2)
    {
        line({polygon.points.at(0), polygon.points.at(1)}, color);
        return;
    }

    const Vec2 cameraPos = camera::getActivePos();

    std::vector<Sint16> vx(size);
    std::vector<Sint16> vy(size);
    for (size_t i = 0; i < size; ++i)
    {
        vx[i] = static_cast<Sint16>(polygon.points.at(i).x - cameraPos.x);
        vy[i] = static_cast<Sint16>(polygon.points.at(i).y - cameraPos.y);
    }

    if (filled)
        filledPolygonRGBA(renderer::get(), vx.data(), vy.data(), static_cast<int>(size), color.r,
                          color.g, color.b, color.a);
    else
        polygonRGBA(renderer::get(), vx.data(), vy.data(), static_cast<int>(size), color.r, color.g,
                    color.b, color.a);
}
} // namespace draw

void _circleThin(SDL_Renderer* renderer, Vec2 center, const int radius, const Color& color)
{
    int f = 1 - radius;
    int ddF_x = 0;
    int ddF_y = -2 * radius;
    int x = 0;
    int y = radius;

    std::set<Uint64> pointSet;

    center -= camera::getActivePos();
    auto emit = [&](int dx, int dy) { pointSet.insert(packPoint(center.x + dx, center.y + dy)); };

    while (x <= y)
    {
        emit(x, y);
        emit(-x, y);
        emit(x, -y);
        emit(-x, -y);
        emit(y, x);
        emit(-y, x);
        emit(y, -x);
        emit(-y, -x);

        if (f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x + 1;
    }

    // Convert to SDL_FPoint vector
    std::vector<SDL_FPoint> points;
    points.reserve(pointSet.size());
    for (auto packed : pointSet)
    {
        int x = int(int32_t(packed >> 32)) - 32768;
        int y = int(int32_t(packed & 0xFFFFFFFF)) - 32768;
        points.push_back(SDL_FPoint{float(x), float(y)});
    }

    SDL_RenderPoints(renderer, points.data(), static_cast<int>(points.size()));
}

void _circle(SDL_Renderer* renderer, Vec2 center, const int radius, const Color& color,
             const int thickness)
{
    const int innerRadius = (thickness <= 0 || thickness >= radius) ? -1 : radius - thickness;
    center -= camera::getActivePos();

    auto hline = [&](int x1, int y, int x2)
    {
        SDL_RenderLine(renderer, static_cast<float>(x1), static_cast<float>(y),
                       static_cast<float>(x2), static_cast<float>(y));
    };

    auto drawCircleSpan = [&](int r, std::unordered_map<int, std::pair<int, int>>& bounds)
    {
        int x = 0;
        int y = r;
        int d = 3 - 2 * r;

        while (x <= y)
        {
            auto update = [&](int y_offset, int xval)
            {
                int yPos = static_cast<int>(center.y) + y_offset;
                int minX = static_cast<int>(center.x) - xval;
                int maxX = static_cast<int>(center.x) + xval;
                bounds[yPos].first = std::min(bounds[yPos].first, minX);
                bounds[yPos].second = std::max(bounds[yPos].second, maxX);
            };

            for (int sign : {-1, 1})
            {
                update(sign * y, x);
                update(sign * x, y);
            }

            if (d < 0)
                d += 4 * x + 6;
            else
            {
                d += 4 * (x - y) + 10;
                y--;
            }
            x++;
        }
    };

    std::unordered_map<int, std::pair<int, int>> outerBounds;
    std::unordered_map<int, std::pair<int, int>> innerBounds;

    // Initialize bounds with max/mins
    for (int i = -radius; i <= radius; ++i)
    {
        outerBounds[static_cast<int>(center.y) + i] = {INT_MAX, INT_MIN};
        innerBounds[static_cast<int>(center.y) + i] = {INT_MAX, INT_MIN};
    }

    drawCircleSpan(radius, outerBounds);
    drawCircleSpan(innerRadius, innerBounds);

    for (const auto& [y, outer] : outerBounds)
    {
        const auto& inner = innerBounds[y];

        if (inner.first == INT_MAX || inner.second == INT_MIN)
            // No inner circle on this line → full span
            hline(outer.first, y, outer.second);
        else
        {
            // Left ring
            if (outer.first < inner.first)
                hline(outer.first, y, inner.first - 1);
            // Right ring
            if (outer.second > inner.second)
                hline(inner.second + 1, y, outer.second);
        }
    }
}

Uint64 packPoint(const int x, const int y)
{
    return (uint64_t(uint32_t(x + 32768)) << 32) | uint32_t(y + 32768);
}
