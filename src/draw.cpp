#include "Draw.hpp"

#include <gfx/SDL3_gfxPrimitives.h>
#include <pybind11/stl.h>

#include <mapbox/earcut.hpp>

#include "Camera.hpp"
#include "Circle.hpp"
#include "Color.hpp"
#include "Line.hpp"
#include "Math.hpp"
#include "Polygon.hpp"
#include "Rect.hpp"
#include "Renderer.hpp"

namespace mapbox
{
namespace util
{
template <>
struct nth<0, kn::Vec2>
{
    inline static float get(const kn::Vec2& v)
    {
        return v.x;
    }
};
template <>
struct nth<1, kn::Vec2>
{
    inline static float get(const kn::Vec2& v)
    {
        return v.y;
    }
};
}  // namespace util
}  // namespace mapbox

namespace kn::draw
{
void _circleFilled(SDL_Renderer* r, const Circle& circle, const Color& color, int numSegments);
void _circleOutline(
    SDL_Renderer* r, const Circle& circle, const Color& color, double thickness, int numSegments
);

void _polygonFilled(SDL_Renderer* r, const Polygon& polygon, const Color& color);

void circle(const Circle& circle, const Color& color, const double thickness, const int numSegments)
{
    SDL_Renderer* rend = renderer::_get();
    if (!rend)
        throw std::runtime_error("Renderer not yet initialized");

    if (circle.radius < 1.0 || color.a == 0)
        return;

    const Vec2 cameraPos = camera::getActivePos();
    const Vec2 targetRes = renderer::getTargetResolution();

    const Vec2 center = circle.pos - cameraPos;
    if (center.x + circle.radius < 0.0 || center.y + circle.radius < 0.0 ||
        center.x - circle.radius >= targetRes.x || center.y - circle.radius >= targetRes.y)
    {
        return;
    }

    Circle cameraCircle = circle;
    cameraCircle.pos = center;

    if (thickness <= 0.0 || thickness >= circle.radius)
        _circleFilled(rend, cameraCircle, color, numSegments);
    else
        _circleOutline(rend, cameraCircle, color, thickness, numSegments);
}

void circles(
    const std::vector<Circle>& circles, const Color& color, const double thickness,
    const int numSegments
)
{
    SDL_Renderer* rend = renderer::_get();
    if (!rend)
        throw std::runtime_error("Renderer not yet initialized");

    if (circles.empty() || color.a == 0)
        return;

    const Vec2 cameraPos = camera::getActivePos();
    const Vec2 targetRes = renderer::getTargetResolution();

    for (const Circle& circle : circles)
    {
        if (circle.radius < 1.0)
            continue;

        const Vec2 center = circle.pos - cameraPos;
        if (center.x + circle.radius < 0.0 || center.y + circle.radius < 0.0 ||
            center.x - circle.radius >= targetRes.x || center.y - circle.radius >= targetRes.y)
        {
            continue;
        }

        Circle cameraCircle = circle;
        cameraCircle.pos = center;

        if (thickness <= 0.0 || thickness >= circle.radius)
            _circleFilled(rend, cameraCircle, color, numSegments);
        else
            _circleOutline(rend, cameraCircle, color, thickness, numSegments);
    }
}

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
        if (!SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a))
            throw std::runtime_error("Failed to set draw color: " + std::string(SDL_GetError()));

        if (!SDL_RenderLine(rend, x1, y1, x2, y2))
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

    const Vec2 cameraPos = camera::getActivePos();
    if (size == 1)
    {
        if (!SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a))
            throw std::runtime_error("Failed to set draw color: " + std::string(SDL_GetError()));

        const auto [x, y] = static_cast<SDL_FPoint>(polygon.points.at(0) - cameraPos);
        if (!SDL_RenderPoint(rend, x, y))
            throw std::runtime_error("Failed to render point: " + std::string(SDL_GetError()));

        return;
    }
    if (size == 2)
    {
        const auto [x1, y1] = static_cast<SDL_FPoint>(polygon.points.at(0) - cameraPos);
        const auto [x2, y2] = static_cast<SDL_FPoint>(polygon.points.at(1) - cameraPos);

        if (!SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a))
            throw std::runtime_error("Failed to set draw color: " + std::string(SDL_GetError()));

        if (!SDL_RenderLine(rend, x1, y1, x2, y2))
            throw std::runtime_error("Failed to render line: " + std::string(SDL_GetError()));

        return;
    }

    Polygon cameraPolygon = polygon;
    cameraPolygon.translate(-cameraPos);

    // Just draw lines if not filled
    if (!filled)
    {
        if (!SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a))
            throw std::runtime_error("Failed to set draw color: " + std::string(SDL_GetError()));

        std::vector<SDL_FPoint> points;
        points.reserve(cameraPolygon.points.size() + 1);
        for (const auto& p : cameraPolygon.points)
            points.push_back(static_cast<SDL_FPoint>(p));
        points.push_back(points.front());

        if (!SDL_RenderLines(rend, points.data(), static_cast<int>(points.size())))
        {
            throw std::runtime_error(
                "Failed to render polygon outline: " + std::string(SDL_GetError())
            );
        }

        return;
    }

    _polygonFilled(rend, cameraPolygon, color);
}

void polygons(const std::vector<Polygon>& polygons, const Color& color, const bool filled)
{
    SDL_Renderer* rend = renderer::_get();
    if (!rend)
        throw std::runtime_error("Renderer not yet initialized");

    if (color.a == 0)
        return;

    const Vec2 cameraPos = camera::getActivePos();

    for (const Polygon& polygon : polygons)
    {
        const size_t size = polygon.points.size();
        if (size == 0)
            continue;
        if (size == 1)
        {
            if (!SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a))
                throw std::runtime_error(
                    "Failed to set draw color: " + std::string(SDL_GetError())
                );

            const auto [x, y] = static_cast<SDL_FPoint>(polygon.points.at(0) - cameraPos);
            if (!SDL_RenderPoint(rend, x, y))
                throw std::runtime_error("Failed to render point: " + std::string(SDL_GetError()));

            continue;
        }
        if (size == 2)
        {
            const auto [x1, y1] = static_cast<SDL_FPoint>(polygon.points.at(0) - cameraPos);
            const auto [x2, y2] = static_cast<SDL_FPoint>(polygon.points.at(1) - cameraPos);

            if (!SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a))
                throw std::runtime_error(
                    "Failed to set draw color: " + std::string(SDL_GetError())
                );

            if (!SDL_RenderLine(rend, x1, y1, x2, y2))
                throw std::runtime_error("Failed to render line: " + std::string(SDL_GetError()));

            continue;
        }

        Polygon cameraPolygon = polygon;
        cameraPolygon.translate(-cameraPos);

        if (!filled)
        {
            if (!SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a))
                throw std::runtime_error(
                    "Failed to set draw color: " + std::string(SDL_GetError())
                );

            std::vector<SDL_FPoint> points;
            points.reserve(cameraPolygon.points.size() + 1);
            for (const auto& p : cameraPolygon.points)
                points.push_back(static_cast<SDL_FPoint>(p));
            points.push_back(points.front());

            if (!SDL_RenderLines(rend, points.data(), static_cast<int>(points.size())))
            {
                throw std::runtime_error(
                    "Failed to render polygon outline: " + std::string(SDL_GetError())
                );
            }

            continue;
        }

        _polygonFilled(rend, cameraPolygon, color);
    }
}

void _polygonFilled(SDL_Renderer* r, const Polygon& polygon, const Color& color)
{
    std::vector<std::vector<kn::Vec2>> vertices;
    vertices.push_back(polygon.points);

    std::vector<uint32_t> indices = mapbox::earcut<uint32_t>(vertices);

    const auto fColor = static_cast<SDL_FColor>(color);
    std::vector<SDL_Vertex> sdlVertices;
    sdlVertices.reserve(polygon.points.size());

    for (const auto& point : polygon.points)
    {
        sdlVertices.push_back({static_cast<SDL_FPoint>(point), fColor, {}});
    }

    if (!SDL_RenderGeometry(
            r, nullptr, sdlVertices.data(), static_cast<int>(sdlVertices.size()),
            reinterpret_cast<const int*>(indices.data()), static_cast<int>(indices.size())
        ))
    {
        throw std::runtime_error(
            std::string("Failed to render polygon geometry: ") + SDL_GetError()
        );
    }
}

void _circleFilled(SDL_Renderer* r, const Circle& circle, const Color& color, const int numSegments)
{
    std::vector<SDL_Vertex> vertices;
    vertices.reserve(numSegments + 2);

    const auto fColor = static_cast<SDL_FColor>(color);

    // Center point
    vertices.push_back({static_cast<SDL_FPoint>(circle.pos), fColor, {0.0f, 0.0f}});

    // Edge points
    for (int i = 0; i <= numSegments; ++i)
    {
        auto theta = 2.0 * M_PI * static_cast<double>(i) / static_cast<double>(numSegments);
        Vec2 p{
            circle.pos.x + circle.radius * cos(theta),
            circle.pos.y + circle.radius * sin(theta),
        };
        vertices.push_back({static_cast<SDL_FPoint>(p), fColor, {}});
    }

    // Create triangle fan
    std::vector<int> indices;
    indices.reserve(numSegments * 3);
    for (int i = 1; i <= numSegments; ++i)
    {
        indices.push_back(0);      // Center
        indices.push_back(i);      // Current edge
        indices.push_back(i + 1);  // Next edge
    }

    if (!SDL_RenderGeometry(
            r, nullptr, vertices.data(), static_cast<int>(vertices.size()), indices.data(),
            static_cast<int>(indices.size())
        ))
    {
        throw std::runtime_error(
            std::string("Failed to render circle geometry: ") + SDL_GetError()
        );
    }
}

void _circleOutline(
    SDL_Renderer* r, const Circle& circle, const Color& color, const double thickness,
    const int numSegments
)
{
    const double innerRadius = circle.radius - thickness;
    const auto fColor = static_cast<SDL_FColor>(color);

    // We need 2 vertices per segment (inner and outer)
    std::vector<SDL_Vertex> vertices;
    vertices.reserve((numSegments + 1) * 2);

    for (int i = 0; i <= numSegments; ++i)
    {
        auto theta = 2.0 * M_PI * static_cast<double>(i) / static_cast<double>(numSegments);
        double cosT = cos(theta);
        double sinT = sin(theta);

        // Outer vertex
        Vec2 outerPoint{
            circle.pos.x + circle.radius * cosT,
            circle.pos.y + circle.radius * sinT,
        };
        vertices.push_back({static_cast<SDL_FPoint>(outerPoint), fColor, {}});

        // Inner vertex
        Vec2 innerPoint{
            circle.pos.x + innerRadius * cosT,
            circle.pos.y + innerRadius * sinT,
        };
        vertices.push_back({static_cast<SDL_FPoint>(innerPoint), fColor, {}});
    }

    // Indices for the triangles (forming a quad between each segment)
    std::vector<int> indices;
    indices.reserve(numSegments * 6);

    for (int i = 0; i < numSegments; ++i)
    {
        int topL = i * 2;
        int botL = i * 2 + 1;
        int topR = (i + 1) * 2;
        int botR = (i + 1) * 2 + 1;

        // Triangle 1
        indices.push_back(topL);
        indices.push_back(topR);
        indices.push_back(botL);

        // Triangle 2
        indices.push_back(topR);
        indices.push_back(botR);
        indices.push_back(botL);
    }

    if (!SDL_RenderGeometry(
            r, nullptr, vertices.data(), static_cast<int>(vertices.size()), indices.data(),
            static_cast<int>(indices.size())
        ))
    {
        throw std::runtime_error(std::string("Failed to render circle outline: ") + SDL_GetError());
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
        py::arg("num_segments") = 36, R"doc(
Draw a circle to the renderer.

Args:
    circle (Circle): The circle to draw.
    color (Color): The color of the circle.
    thickness (float, optional): The line thickness. If <= 0 or >= radius, draws filled circle. Defaults to 0 (filled).
    num_segments (int, optional): Number of segments to approximate the circle.
                                  Higher values yield smoother circles. Defaults to 36.
    )doc"
    );

    subDraw.def(
        "circles", &circles, py::arg("circles"), py::arg("color"), py::arg("thickness") = 0,
        py::arg("num_segments") = 36, R"doc(
Draw an array of circles in bulk to the renderer.

Args:
    circles (Sequence[Circle]): The circles to draw in bulk.
    color (Color): The color of the circles.
    thickness (float, optional): The line thickness. If <= 0 or >= radius, draws filled circle. Defaults to 0 (filled).
    num_segments (int, optional): Number of segments to approximate each circle.
                                  Higher values yield smoother circles. Defaults to 36.
    )doc"
    );

    subDraw.def(
        "ellipse", &ellipse, py::arg("bounds"), py::arg("color"), py::arg("filled") = false, R"doc(
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
        "polygon", &polygon, py::arg("polygon"), py::arg("color"), py::arg("filled") = true,
        R"doc(
Draw a polygon to the renderer.

Args:
    polygon (Polygon): The polygon to draw.
    color (Color): The color of the polygon.
    filled (bool, optional): Whether to draw a filled polygon or just the outline.
                             Defaults to False (outline). Works with both convex and concave polygons.
    )doc"
    );

    subDraw.def(
        "polygons", &polygons, py::arg("polygons"), py::arg("color"), py::arg("filled") = true,
        R"doc(
Draw an array of polygons in bulk to the renderer.

Args:
    polygons (Sequence[Polygon]): The polygons to draw in bulk.
    color (Color): The color of the polygons.
    filled (bool, optional): Whether to draw filled polygons or just the outlines.
                             Defaults to True (filled). Works with both convex and concave polygons.
    )doc"
    );
}
}  // namespace kn::draw
