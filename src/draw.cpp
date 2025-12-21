#include "Draw.hpp"

#include <gfx/SDL3_gfxPrimitives.h>
#include <pybind11/stl.h>

#include <set>

#include "Camera.hpp"
#include "Circle.hpp"
#include "Color.hpp"
#include "Line.hpp"
#include "Math.hpp"
#include "Polygon.hpp"
#include "Rect.hpp"
#include "Renderer.hpp"

static Uint64 packPoint(int x, int y);

namespace kn::draw
{
static void _circleThin(SDL_Renderer* renderer, const Vec2& center, int radius);
static void _circle(SDL_Renderer* renderer, const Vec2& center, int radius, int thickness);

void point(const Vec2& point, const Color& color)
{
    SDL_Renderer* rend = renderer::_get();
    SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a);

    if (const auto [x, y] = static_cast<SDL_FPoint>(point - camera::getActivePos());
        !SDL_RenderPoint(rend, x, y))
        throw std::runtime_error("Failed to render point: " + std::string(SDL_GetError()));
}

void points(const std::vector<Vec2>& points, const Color& color)
{
    if (points.empty())
        return;

    SDL_Renderer* rend = renderer::_get();

    SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a);

    std::vector<SDL_FPoint> sdlPoints;
    sdlPoints.reserve(points.size());

    const Vec2 cameraPos = camera::getActivePos();
    const Vec2 max = renderer::getTargetResolution() - cameraPos;
    const Vec2 min = -cameraPos;
    for (const Vec2& point : points)
        if (const Vec2 pos = point - cameraPos; min < pos && pos < max)
            sdlPoints.emplace_back(pos);

    if (!SDL_RenderPoints(rend, sdlPoints.data(), static_cast<int>(sdlPoints.size())))
        throw std::runtime_error("Failed to render points: " + std::string(SDL_GetError()));
}

// Accept a NumPy ndarray with shape (N,2) and dtype float64 for the fastest path.
void pointsFromNDArray(const py::array_t<double, py::array::c_style | py::array::forcecast>& arr,
                       const Color& color)
{
    const auto info = arr.request();
    if (info.ndim != 2 || info.shape[1] != 2)
        throw std::invalid_argument("Expected array shape (N,2)");

    const auto n = static_cast<size_t>(info.shape[0]);
    if (n == 0)
        return;

    const double* data = static_cast<double*>(info.ptr);

    SDL_Renderer* rend = renderer::_get();
    SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a);

    std::vector<SDL_FPoint> sdlPoints;
    sdlPoints.reserve(n);

    const Vec2 res = renderer::getTargetResolution();
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

    SDL_Renderer* rend = renderer::_get();
    SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a);

    if (thickness == 1)
        _circleThin(rend, circle.pos - camera::getActivePos(), static_cast<int>(circle.radius));
    else
        _circle(rend, circle.pos - camera::getActivePos(), static_cast<int>(circle.radius),
                thickness);
}

void ellipse(const Rect& bounds, const Color& color, const bool filled)
{
    if (bounds.w < 1 || bounds.h < 1)
        return;

    SDL_Renderer* rend = renderer::_get();
    SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a);

    const Vec2 cameraPos = camera::getActivePos();
    const auto sdlRect = static_cast<SDL_FRect>(
        Rect(bounds.x - cameraPos.x, bounds.y - cameraPos.y, bounds.w, bounds.h));

    if (filled)
        filledEllipseRGBA(rend, static_cast<Sint16>(sdlRect.x + sdlRect.w / 2),
                          static_cast<Sint16>(sdlRect.y + sdlRect.h / 2),
                          static_cast<Sint16>(sdlRect.w / 2), static_cast<Sint16>(sdlRect.h / 2),
                          color.r, color.g, color.b, color.a);
    else
        ellipseRGBA(rend, static_cast<Sint16>(sdlRect.x + sdlRect.w / 2),
                    static_cast<Sint16>(sdlRect.y + sdlRect.h / 2),
                    static_cast<Sint16>(sdlRect.w / 2), static_cast<Sint16>(sdlRect.h / 2), color.r,
                    color.g, color.b, color.a);
}

void line(const Line& line, const Color& color, const int thickness)
{
    const Vec2 cameraPos = camera::getActivePos();
    const auto x1 = static_cast<Sint16>(line.ax - cameraPos.x);
    const auto y1 = static_cast<Sint16>(line.ay - cameraPos.y);
    const auto x2 = static_cast<Sint16>(line.bx - cameraPos.x);
    const auto y2 = static_cast<Sint16>(line.by - cameraPos.y);

    if (thickness <= 1)
        lineRGBA(renderer::_get(), x1, y1, x2, y2, color.r, color.g, color.b, color.a);
    else
        thickLineRGBA(renderer::_get(), x1, y1, x2, y2, thickness, color.r, color.g, color.b,
                      color.a);
}

void rect(Rect rect, const Color& color, const int thickness)
{
    SDL_Renderer* rend = renderer::_get();
    SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a);

    const Vec2 cameraPos = camera::getActivePos();
    rect.x -= cameraPos.x;
    rect.y -= cameraPos.y;
    auto sdlRect = static_cast<SDL_FRect>(rect);

    if (thickness <= 0 || thickness > rect.w / 2.0 || thickness > rect.h / 2.0)
    {
        SDL_RenderFillRect(rend, &sdlRect);
        return;
    }

    SDL_RenderRect(rend, &sdlRect);
    for (int i = 1; i < thickness; i++)
    {
        rect.inflate({-2, -2});
        sdlRect = static_cast<SDL_FRect>(rect);
        SDL_RenderRect(rend, &sdlRect);
    }
}

void rects(const std::vector<Rect>& rects, const Color& color, const int thickness)
{
    if (rects.empty())
        return;

    SDL_Renderer* rend = renderer::_get();
    SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a);

    const Vec2 cameraPos = camera::getActivePos();

    // Convert to SDL_FRect array with camera offset
    std::vector<SDL_FRect> sdlRects;
    sdlRects.reserve(rects.size());

    for (const Rect& rect : rects)
    {
        SDL_FRect sdlRect;
        sdlRect.x = static_cast<float>(rect.x - cameraPos.x);
        sdlRect.y = static_cast<float>(rect.y - cameraPos.y);
        sdlRect.w = static_cast<float>(rect.w);
        sdlRect.h = static_cast<float>(rect.h);
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
                    // Skip if thickness exceeds rectangle dimensions
                    if (i >= rect.w / 2.0 || i >= rect.h / 2.0)
                        continue;

                    SDL_FRect innerRect;
                    innerRect.x = static_cast<float>(rect.x - cameraPos.x + i);
                    innerRect.y = static_cast<float>(rect.y - cameraPos.y + i);
                    innerRect.w = static_cast<float>(rect.w - 2 * i);
                    innerRect.h = static_cast<float>(rect.h - 2 * i);
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
        filledPolygonRGBA(renderer::_get(), vx.data(), vy.data(), static_cast<int>(size), color.r,
                          color.g, color.b, color.a);
    else
        polygonRGBA(renderer::_get(), vx.data(), vy.data(), static_cast<int>(size), color.r,
                    color.g, color.b, color.a);
}

void _circleThin(SDL_Renderer* renderer, const Vec2& center, const int radius)
{
    int f = 1 - radius;
    int ddF_x = 0;
    int ddF_y = -2 * radius;
    int x = 0;
    int y = radius;

    std::set<Uint64> pointSet;

    auto emit = [&](const int dx, const int dy)
    {
        pointSet.insert(
            packPoint(static_cast<int>(center.x) + dx, static_cast<int>(center.y) + dy));
    };

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
    for (const auto packed : pointSet)
    {
        const int px = static_cast<int32_t>(packed >> 32) - 32768;
        const int py = static_cast<int32_t>(packed & 0xFFFFFFFF) - 32768;
        points.push_back(SDL_FPoint{static_cast<float>(px), static_cast<float>(py)});
    }

    SDL_RenderPoints(renderer, points.data(), static_cast<int>(points.size()));
}

void _circle(SDL_Renderer* renderer, const Vec2& center, const int radius, const int thickness)
{
    const int innerRadius = thickness <= 0 || thickness >= radius ? -1 : radius - thickness;

    auto hLine = [&](const int x1, const int y, const int x2)
    {
        SDL_RenderLine(renderer, static_cast<float>(x1), static_cast<float>(y),
                       static_cast<float>(x2), static_cast<float>(y));
    };

    auto drawCircleSpan = [&](const int r, std::unordered_map<int, std::pair<int, int>>& bounds)
    {
        int x = 0;
        int y = r;
        int d = 3 - 2 * r;

        while (x <= y)
        {
            auto update = [&](const int yOffset, const int xVal)
            {
                const int yPos = static_cast<int>(center.y) + yOffset;
                const int minX = static_cast<int>(center.x) - xVal;
                const int maxX = static_cast<int>(center.x) + xVal;
                bounds[yPos].first = std::min(bounds[yPos].first, minX);
                bounds[yPos].second = std::max(bounds[yPos].second, maxX);
            };

            for (const int sign : {-1, 1})
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
        if (const auto& [fst, snd] = innerBounds[y]; fst == INT_MAX || snd == INT_MIN)
            // No inner circle on this line → full span
            hLine(outer.first, y, outer.second);
        else
        {
            // Left ring
            if (outer.first < fst)
                hLine(outer.first, y, fst - 1);
            // Right ring
            if (outer.second > snd)
                hLine(snd + 1, y, outer.second);
        }
    }
}

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

    subDraw.def("points_from_ndarray", &pointsFromNDArray, py::arg("points"), py::arg("color"),
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

    subDraw.def("ellipse", &ellipse, py::arg("bounds"), py::arg("color"), py::arg("filled") = false,
                R"doc(
Draw an ellipse to the renderer.

Args:
    bounds (Rect): The bounding box of the ellipse.
    color (Color): The color of the ellipse.
    filled (bool, optional): Whether to draw a filled ellipse or just the outline.
                             Defaults to False (outline).
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
                py::arg("filled") = false, R"doc(
Draw a polygon to the renderer.

Args:
    polygon (Polygon): The polygon to draw.
    color (Color): The color of the polygon.
    filled (bool, optional): Whether to draw a filled polygon or just the outline.
                             Defaults to False (outline). Works with both convex and concave polygons.
    )doc");
}
}  // namespace kn::draw

Uint64 packPoint(const int x, const int y)
{
    return static_cast<uint64_t>(static_cast<uint32_t>(x + 32768)) << 32 |
           static_cast<uint32_t>(y + 32768);
}
