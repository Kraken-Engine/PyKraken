#include "Draw.hpp"

#include <gfx/SDL3_gfxPrimitives.h>
#include <pybind11/stl_bind.h>

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

void point(Vec2 point, const Color& color)
{
    SDL_Renderer* rend = renderer::_get();
    if (!rend)
        throw std::runtime_error("Renderer not yet initialized");

    if (color.a == 0)
        return;

    const Vec2 targetRes = renderer::getTargetResolution();
    point -= camera::getActivePos();
    if (point.x < 0.0 || point.y < 0.0 || point.x >= targetRes.x || point.y >= targetRes.y)
        return;

    if (!SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a))
        throw std::runtime_error("Failed to set draw color: " + std::string(SDL_GetError()));

    if (const auto [x, y] = static_cast<SDL_FPoint>(point); !SDL_RenderPoint(rend, x, y))
        throw std::runtime_error("Failed to render point: " + std::string(SDL_GetError()));
}

void points(const std::vector<Vec2>& points, const Color& color)
{
    SDL_Renderer* rend = renderer::_get();
    if (!rend)
        throw std::runtime_error("Renderer not yet initialized");

    if (points.empty() || color.a == 0)
        return;

    if (!SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a))
        throw std::runtime_error("Failed to set draw color: " + std::string(SDL_GetError()));

    std::vector<SDL_FPoint> sdlPoints;
    sdlPoints.reserve(points.size());

    const Vec2 cameraPos = camera::getActivePos();
    const Vec2 targetRes = renderer::getTargetResolution();
    for (Vec2 point : points)
    {
        point -= cameraPos;
        if (point.x >= 0.0 && point.y >= 0.0 && point.x < targetRes.x && point.y < targetRes.y)
            sdlPoints.push_back(std::move(static_cast<SDL_FPoint>(point)));
    }

    if (!SDL_RenderPoints(rend, sdlPoints.data(), static_cast<int>(sdlPoints.size())))
        throw std::runtime_error("Failed to render points: " + std::string(SDL_GetError()));
}

// Accept a NumPy ndarray with shape (N,2) and dtype float64 for the fastest path.
void pointsFromNDArray(
    const py::array_t<double, py::array::c_style | py::array::forcecast>& arr, const Color& color
)
{
    SDL_Renderer* rend = renderer::_get();
    if (!rend)
        throw std::runtime_error("Renderer not yet initialized");

    if (color.a == 0)
        return;

    const auto info = arr.request();
    if (info.ndim != 2 || info.shape[1] != 2)
        throw std::invalid_argument("Expected array shape (N,2)");

    const auto n = static_cast<size_t>(info.shape[0]);
    if (n == 0)
        return;

    if (!SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a))
        throw std::runtime_error("Failed to set draw color: " + std::string(SDL_GetError()));

    std::vector<SDL_FPoint> sdlPoints;
    sdlPoints.reserve(n);

    const auto* data = static_cast<double*>(info.ptr);
    const Vec2 targetRes = renderer::getTargetResolution();
    const Vec2 cameraPos = camera::getActivePos();
    for (size_t i = 0; i < n; ++i)
    {
        Vec2 pos = {data[i * 2], data[i * 2 + 1]};
        pos -= cameraPos;
        if (pos.x > 0.0 && pos.y > 0.0 && pos.x < targetRes.x && pos.y < targetRes.y)
            sdlPoints.push_back(std::move(static_cast<SDL_FPoint>(pos)));
    }

    if (!SDL_RenderPoints(rend, sdlPoints.data(), static_cast<int>(sdlPoints.size())))
        throw std::runtime_error("Failed to render points: " + std::string(SDL_GetError()));
}

void circle(const Circle& circle, const Color& color, const int thickness)
{
    SDL_Renderer* rend = renderer::_get();
    if (!rend)
        throw std::runtime_error("Renderer not yet initialized");

    if (circle.radius < 1.0 || color.a == 0)
        return;

    if (!SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a))
        throw std::runtime_error("Failed to set draw color: " + std::string(SDL_GetError()));

    if (thickness == 1)
    {
        _circleThin(rend, circle.pos - camera::getActivePos(), static_cast<int>(circle.radius));
    }
    else
    {
        _circle(
            rend, circle.pos - camera::getActivePos(), static_cast<int>(circle.radius), thickness
        );
    }
}

void ellipse(Rect bounds, const Color& color, const bool filled)
{
    SDL_Renderer* rend = renderer::_get();
    if (!rend)
        throw std::runtime_error("Renderer not yet initialized");

    if (bounds.w < 1 || bounds.h < 1)
        return;
    if (color.a == 0)
        return;

    bounds.setTopLeft(bounds.getTopLeft() - camera::getActivePos());

    if (filled)
    {
        if (!filledEllipseColor(
                rend, static_cast<Sint16>(bounds.x + bounds.w / 2.0),
                static_cast<Sint16>(bounds.y + bounds.h / 2.0), static_cast<Sint16>(bounds.w / 2.0),
                static_cast<Sint16>(bounds.h / 2.0), static_cast<Uint32>(color)
            ))
            throw std::runtime_error(
                "Failed to render filled ellipse: " + std::string(SDL_GetError())
            );
    }
    else
    {
        if (!ellipseColor(
                rend, static_cast<Sint16>(bounds.x + bounds.w / 2.0),
                static_cast<Sint16>(bounds.y + bounds.h / 2.0), static_cast<Sint16>(bounds.w / 2.0),
                static_cast<Sint16>(bounds.h / 2.0), static_cast<Uint32>(color)
            ))
        {
            throw std::runtime_error("Failed to render ellipse: " + std::string(SDL_GetError()));
        }
    }
}

void line(const Line& line, const Color& color, const int thickness)
{
    SDL_Renderer* rend = renderer::_get();
    if (!rend)
        throw std::runtime_error("Renderer not yet initialized");

    if (color.a == 0)
        return;

    const Vec2 cameraPos = camera::getActivePos();
    const auto x1 = static_cast<Sint16>(line.ax - cameraPos.x);
    const auto y1 = static_cast<Sint16>(line.ay - cameraPos.y);
    const auto x2 = static_cast<Sint16>(line.bx - cameraPos.x);
    const auto y2 = static_cast<Sint16>(line.by - cameraPos.y);

    if (thickness <= 1)
    {
        if (!lineColor(rend, x1, y1, x2, y2, static_cast<Uint32>(color)))
            throw std::runtime_error("Failed to render line: " + std::string(SDL_GetError()));
    }
    else
    {
        if (!thickLineColor(rend, x1, y1, x2, y2, thickness, static_cast<Uint32>(color)))
            throw std::runtime_error("Failed to render thick line: " + std::string(SDL_GetError()));
    }
}

void rect(Rect rect, const Color& color, const int thickness)
{
    SDL_Renderer* rend = renderer::_get();
    if (!rend)
        throw std::runtime_error("Renderer not yet initialized");

    if (color.a == 0)
        return;

    if (!SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a))
        throw std::runtime_error("Failed to set draw color: " + std::string(SDL_GetError()));

    rect.setTopLeft(rect.getTopLeft() - camera::getActivePos());
    auto sdlRect = static_cast<SDL_FRect>(rect);

    if (thickness <= 0 || thickness > rect.w / 2.0 || thickness > rect.h / 2.0)
    {
        if (!SDL_RenderFillRect(rend, &sdlRect))
            throw std::runtime_error(
                "Failed to render filled rectangle: " + std::string(SDL_GetError())
            );
        return;
    }

    if (!SDL_RenderRect(rend, &sdlRect))
        throw std::runtime_error("Failed to render rectangle: " + std::string(SDL_GetError()));

    for (int i = 1; i < thickness; i++)
    {
        rect.inflate({-2, -2});
        sdlRect = static_cast<SDL_FRect>(rect);
        if (!SDL_RenderRect(rend, &sdlRect))
            throw std::runtime_error("Failed to render rectangle: " + std::string(SDL_GetError()));
    }
}

void rects(const std::vector<Rect>& rects, const Color& color, const int thickness)
{
    SDL_Renderer* rend = renderer::_get();
    if (!rend)
        throw std::runtime_error("Renderer not yet initialized");

    if (color.a == 0)
        return;
    if (rects.empty())
        return;

    if (!SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a))
        throw std::runtime_error("Failed to set draw color: " + std::string(SDL_GetError()));

    const Vec2 cameraPos = camera::getActivePos();

    // Convert to SDL_FRect array with camera offset
    std::vector<SDL_FRect> sdlRects;
    sdlRects.reserve(rects.size());
    for (const Rect& rect : rects)
    {
        const SDL_FRect sdlRect{
            static_cast<float>(rect.x - cameraPos.x),
            static_cast<float>(rect.y - cameraPos.y),
            static_cast<float>(rect.w),
            static_cast<float>(rect.h),
        };
        sdlRects.push_back(std::move(sdlRect));
    }

    // For filled rectangles or thick outlines, use batch fill
    if (thickness <= 0)
    {
        if (!SDL_RenderFillRects(rend, sdlRects.data(), static_cast<int>(sdlRects.size())))
            throw std::runtime_error(
                "Failed to render filled rectangles: " + std::string(SDL_GetError())
            );
        return;
    }

    if (thickness == 1)
    {
        if (!SDL_RenderRects(rend, sdlRects.data(), static_cast<int>(sdlRects.size())))
            throw std::runtime_error("Failed to render rectangles: " + std::string(SDL_GetError()));
        return;
    }

    for (int i = 1; i < thickness; i++)
    {
        std::vector<SDL_FRect> innerRects;
        innerRects.reserve(rects.size());

        for (const Rect& rect : rects)
        {
            // Skip if thickness exceeds rectangle dimensions
            if (i >= rect.w / 2.0 || i >= rect.h / 2.0)
                continue;

            const SDL_FRect innerRect = {
                static_cast<float>(rect.x - cameraPos.x + i),
                static_cast<float>(rect.y - cameraPos.y + i),
                static_cast<float>(rect.w - 2 * i),
                static_cast<float>(rect.h - 2 * i),
            };
            innerRects.push_back(std::move(innerRect));
        }

        if (innerRects.empty())
            break;

        if (!SDL_RenderRects(rend, innerRects.data(), static_cast<int>(innerRects.size())))
            throw std::runtime_error("Failed to render rectangles: " + std::string(SDL_GetError()));
    }
}

void polygon(const Polygon& polygon, const Color& color, const bool filled)
{
    SDL_Renderer* rend = renderer::_get();
    if (!rend)
        throw std::runtime_error("Renderer not yet initialized");

    if (color.a == 0)
        return;

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

    std::vector<Sint16> vx(size), vy(size);
    for (size_t i = 0; i < size; ++i)
    {
        const auto& p = polygon.points[i];
        vx[i] = static_cast<Sint16>(p.x - cameraPos.x);
        vy[i] = static_cast<Sint16>(p.y - cameraPos.y);
    }

    using PolyFn = bool (*)(SDL_Renderer*, const Sint16*, const Sint16*, int, Uint32);
    PolyFn drawFn = filled ? filledPolygonColor : polygonColor;

    if (!drawFn(rend, vx.data(), vy.data(), static_cast<int>(size), static_cast<Uint32>(color)))
        throw std::runtime_error(std::string("Failed to render polygon: ") + SDL_GetError());
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
            packPoint(static_cast<int>(center.x) + dx, static_cast<int>(center.y) + dy)
        );
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
        const auto px = static_cast<int>(packed >> 32) - 32768;
        const auto py = static_cast<int>(packed & 0xFFFFFFFF) - 32768;
        points.emplace_back(static_cast<float>(px), static_cast<float>(py));
    }

    if (!SDL_RenderPoints(renderer, points.data(), static_cast<int>(points.size())))
        throw std::runtime_error("Failed to render circle points: " + std::string(SDL_GetError()));
}

void _circle(SDL_Renderer* renderer, const Vec2& center, const int radius, const int thickness)
{
    const int innerRadius = thickness <= 0 || thickness >= radius ? -1 : radius - thickness;

    auto hLine = [&](const int x1, const int y, const int x2)
    {
        if (!SDL_RenderLine(
                renderer, static_cast<float>(x1), static_cast<float>(y), static_cast<float>(x2),
                static_cast<float>(y)
            ))
        {
            throw std::runtime_error("Failed to render line: " + std::string(SDL_GetError()));
        }
    };

    using LineSpan = std::pair<int, int>;
    using BoundsMap = std::unordered_map<int, LineSpan>;

    auto drawCircleSpan = [&](const int r, BoundsMap& bounds)
    {
        int x = 0;
        int y = r;
        int d = 3 - 2 * r;

        while (x <= y)
        {
            auto update = [&](const int yOffset, const int xVal)
            {
                const auto yPos = static_cast<int>(center.y) + yOffset;
                const auto minX = static_cast<int>(center.x) - xVal;
                const auto maxX = static_cast<int>(center.x) + xVal;
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

    BoundsMap outerBounds, innerBounds;

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
        {
            // No inner circle on this line → full span
            hLine(outer.first, y, outer.second);
        }
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

    subDraw.def(
        "points_from_ndarray", &pointsFromNDArray, py::arg("points"), py::arg("color"),
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
        "circle", &circle, py::arg("circle"), py::arg("color"), py::arg("thickness") = 0,
        R"doc(
Draw a circle to the renderer.

Args:
    circle (Circle): The circle to draw.
    color (Color): The color of the circle.
    thickness (int, optional): The line thickness. If 0 or >= radius, draws filled circle.
                              Defaults to 0 (filled).
    )doc"
    );

    subDraw.def(
        "ellipse", &ellipse, py::arg("bounds"), py::arg("color"), py::arg("filled") = false,
        R"doc(
Draw an ellipse to the renderer.

Args:
    bounds (Rect): The bounding box of the ellipse.
    color (Color): The color of the ellipse.
    filled (bool, optional): Whether to draw a filled ellipse or just the outline.
                             Defaults to False (outline).
    )doc"
    );

    subDraw.def(
        "line", &line, py::arg("line"), py::arg("color"), py::arg("thickness") = 1,
        R"doc(
Draw a line to the renderer.

Args:
    line (Line): The line to draw.
    color (Color): The color of the line.
    thickness (int, optional): The line thickness in pixels. Defaults to 1.
    )doc"
    );

    subDraw.def(
        "rect", &rect, py::arg("rect"), py::arg("color"), py::arg("thickness") = 0,
        R"doc(
Draw a rectangle to the renderer.

Args:
    rect (Rect): The rectangle to draw.
    color (Color): The color of the rectangle.
    thickness (int, optional): The border thickness. If 0 or >= half width/height, draws filled rectangle. Defaults to 0 (filled).
    )doc"
    );

    subDraw.def(
        "rects", &rects, py::arg("rects"), py::arg("color"), py::arg("thickness") = 0,
        R"doc(
Batch draw an array of rectangles to the renderer.

Args:
    rects (Sequence[Rect]): The rectangles to batch draw.
    color (Color): The color of the rectangles.
    thickness (int, optional): The border thickness of the rectangles. If 0 or >= half width/height, draws filled rectangles. Defaults to 0 (filled).
    )doc"
    );

    subDraw.def(
        "polygon", &polygon, py::arg("polygon"), py::arg("color"), py::arg("filled") = false,
        R"doc(
Draw a polygon to the renderer.

Args:
    polygon (Polygon): The polygon to draw.
    color (Color): The color of the polygon.
    filled (bool, optional): Whether to draw a filled polygon or just the outline.
                             Defaults to False (outline). Works with both convex and concave polygons.
    )doc"
    );
}
}  // namespace kn::draw

Uint64 packPoint(const int x, const int y)
{
    return static_cast<uint64_t>(static_cast<uint32_t>(x + 32768)) << 32 |
           static_cast<uint32_t>(y + 32768);
}
