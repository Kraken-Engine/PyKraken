#include "Draw.hpp"

#include <pybind11/stl.h>

#include <algorithm>
#include <mapbox/earcut.hpp>
#include <sstream>

#include "Camera.hpp"
#include "Capsule.hpp"
#include "Circle.hpp"
#include "Line.hpp"
#include "Polygon.hpp"
#include "Rect.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

namespace kn::draw
{
SDL_Renderer* rend = nullptr;

// Ellipse helpers
void _ellipseFilled(
    const Vec2& center, double radiusX, double radiusY, const Color& color, int numSegments
);
void _ellipseOutline(
    const Vec2& center, double radiusX, double radiusY, const Color& color, double thickness,
    int numSegments
);

// Capsule helpers
void _capsuleFilled(const Capsule& capsule, const Color& color, int numSegments);
void _capsuleOutline(const Capsule& capsule, const Color& color, double thickness, int numSegments);

// Line helper for thick lines
void _thickLine(const Line& line, const Color& color, double thickness);

// Polygon helper for filled polygons
void _polygonFilled(const Polygon& polygon, const Color& color);

// Helpers for rounded rectangles
Polygon _roundedRectPolygon(const Rect& rect, const std::array<double, 4>& radii);
void _roundedRectOutline(const Rect& rect, const Color& color, const std::array<double, 4>& radii);

std::vector<SDL_FPoint> _ellipsePolyline(
    const Vec2& center, const double radiusX, const double radiusY, const int numSegments
);

std::vector<SDL_FPoint> _capsulePolyline(
    const Vec2& p1, const Vec2& p2, const double radius, const int numSegments
);

void circle(const Circle& circle, const Color& color, const double thickness, const int numSegments)
{
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

    const bool filled = (thickness <= 0.0 || thickness >= circle.radius);
    if (filled)
    {
        _ellipseFilled(center, circle.radius, circle.radius, color, numSegments);
    }
    else
    {
        _ellipseOutline(center, circle.radius, circle.radius, color, thickness, numSegments);
    }
}

void circles(
    const std::vector<Circle>& circles, const Color& color, const double thickness,
    const int numSegments
)
{
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

        if (thickness <= 0.0 || thickness >= circle.radius)
        {
            _ellipseFilled(center, circle.radius, circle.radius, color, numSegments);
        }
        else
        {
            _ellipseOutline(center, circle.radius, circle.radius, color, thickness, numSegments);
        }
    }
}

void capsule(
    const Capsule& capsule, const Color& color, const double thickness, const int numSegments
)
{
    if (!rend)
        throw std::runtime_error("Renderer not yet initialized");

    if (capsule.radius < 1.0 || color.a == 0)
        return;

    const Vec2 cameraPos = camera::getActivePos();
    const Vec2 targetRes = renderer::getTargetResolution();

    const double radius = capsule.radius;
    const Vec2 p1 = capsule.p1 - cameraPos;
    const Vec2 p2 = capsule.p2 - cameraPos;

    const double minX = std::min(p1.x, p2.x) - radius;
    const double minY = std::min(p1.y, p2.y) - radius;
    const double maxX = std::max(p1.x, p2.x) + radius;
    const double maxY = std::max(p1.y, p2.y) + radius;
    if (maxX < 0.0 || maxY < 0.0 || minX >= targetRes.x || minY >= targetRes.y)
        return;

    const bool filled = (thickness <= 0.0 || thickness >= radius);
    if (filled)
        _capsuleFilled(capsule, color, numSegments);
    else
        _capsuleOutline(capsule, color, thickness, numSegments);
}

void capsules(
    const std::vector<Capsule>& capsules, const Color& color, const double thickness,
    const int numSegments
)
{
    if (!rend)
        throw std::runtime_error("Renderer not yet initialized");

    const Vec2 cameraPos = camera::getActivePos();
    const Vec2 targetRes = renderer::getTargetResolution();

    for (const auto& c : capsules)
    {
        if (c.radius < 1.0 || color.a == 0)
            continue;

        const double radius = c.radius;
        const Vec2 p1 = c.p1 - cameraPos;
        const Vec2 p2 = c.p2 - cameraPos;

        const double minX = std::min(p1.x, p2.x) - radius;
        const double minY = std::min(p1.y, p2.y) - radius;
        const double maxX = std::max(p1.x, p2.x) + radius;
        const double maxY = std::max(p1.y, p2.y) + radius;
        if (maxX < 0.0 || maxY < 0.0 || minX >= targetRes.x || minY >= targetRes.y)
            continue;

        const bool filled = (thickness <= 0.0 || thickness >= radius);
        if (filled)
            _capsuleFilled(c, color, numSegments);
        else
            _capsuleOutline(c, color, thickness, numSegments);
    }
}

void point(Vec2 point, const Color& color)
{
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
            sdlPoints.push_back(static_cast<SDL_FPoint>(point));
    }

    if (!SDL_RenderPoints(rend, sdlPoints.data(), static_cast<int>(sdlPoints.size())))
        throw std::runtime_error("Failed to render points: " + std::string(SDL_GetError()));
}

// Accept a NumPy ndarray with shape (N,2) and dtype float64 for the fastest path.
void pointsFromNDArray(
    const py::array_t<double, py::array::c_style | py::array::forcecast>& arr, const Color& color
)
{
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
        if (pos.x >= 0.0 && pos.y >= 0.0 && pos.x < targetRes.x && pos.y < targetRes.y)
            sdlPoints.push_back(static_cast<SDL_FPoint>(pos));
    }

    if (!SDL_RenderPoints(rend, sdlPoints.data(), static_cast<int>(sdlPoints.size())))
        throw std::runtime_error("Failed to render points: " + std::string(SDL_GetError()));
}

void ellipse(Rect bounds, const Color& color, const double thickness, const int numSegments)
{
    if (!rend)
        throw std::runtime_error("Renderer not yet initialized");

    if (bounds.w < 1 || bounds.h < 1 || color.a == 0)
        return;

    const Vec2 center = bounds.getCenter() - camera::getActivePos();
    const double radiusX = bounds.w / 2.0;
    const double radiusY = bounds.h / 2.0;

    const bool filled = (thickness <= 0.0 || (thickness >= radiusX && thickness >= radiusY));
    if (filled)
    {
        _ellipseFilled(center, radiusX, radiusY, color, numSegments);
    }
    else
    {
        _ellipseOutline(center, radiusX, radiusY, color, thickness, numSegments);
    }
}

void ellipses(
    const std::vector<Rect>& bounds, const Color& color, const double thickness,
    const int numSegments
)
{
    if (!rend)
        throw std::runtime_error("Renderer not yet initialized");

    if (bounds.empty() || color.a == 0)
        return;

    const Vec2 cameraPos = camera::getActivePos();
    for (const auto& rect : bounds)
    {
        if (rect.w < 1 || rect.h < 1)
            continue;

        const Vec2 center = rect.getCenter() - cameraPos;
        const double radiusX = rect.w / 2.0;
        const double radiusY = rect.h / 2.0;

        if (thickness <= 0.0 || (thickness >= radiusX && thickness >= radiusY))
        {
            _ellipseFilled(center, radiusX, radiusY, color, numSegments);
        }
        else
        {
            _ellipseOutline(center, radiusX, radiusY, color, thickness, numSegments);
        }
    }
}

void line(Line line, const Color& color, const double thickness)
{
    if (!rend)
        throw std::runtime_error("Renderer not yet initialized");

    if (color.a == 0)
        return;

    const Vec2 cameraPos = camera::getActivePos();
    line.move(-cameraPos);

    if (thickness <= 1.0)
    {
        const auto a = static_cast<SDL_FPoint>(line.getA());
        const auto b = static_cast<SDL_FPoint>(line.getB());
        if (!SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a))
            throw std::runtime_error("Failed to set draw color: " + std::string(SDL_GetError()));

        if (!SDL_RenderLine(rend, a.x, a.y, b.x, b.y))
            throw std::runtime_error("Failed to render line: " + std::string(SDL_GetError()));
    }
    else
    {
        _thickLine(line, color, thickness);
    }
}

void lines(const std::vector<Line>& lines, const Color& color, const double thickness)
{
    if (!rend)
        throw std::runtime_error("Renderer not yet initialized");

    if (lines.empty() || color.a == 0)
        return;

    const Vec2 cameraPos = camera::getActivePos();
    if (thickness <= 1.0)
    {
        if (!SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a))
            throw std::runtime_error("Failed to set draw color: " + std::string(SDL_GetError()));

        for (auto line : lines)
        {
            line.move(-cameraPos);
            const auto a = static_cast<SDL_FPoint>(line.getA());
            const auto b = static_cast<SDL_FPoint>(line.getB());
            if (!SDL_RenderLine(rend, a.x, a.y, b.x, b.y))
                throw std::runtime_error("Failed to render line: " + std::string(SDL_GetError()));
        }
    }
    else
    {
        for (auto line : lines)
        {
            line.move(-cameraPos);
            _thickLine(line, color, thickness);
        }
    }
}

void rect(
    Rect rect, const Color& color, const int thickness, const double borderRadius,
    double radiusTopLeft, double radiusTopRight, double radiusBottomRight, double radiusBottomLeft
)
{
    if (!rend)
        throw std::runtime_error("Renderer not yet initialized");

    if (color.a == 0 || rect.w < 1.0 || rect.h < 1.0)
        return;

    if (!SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a))
        throw std::runtime_error("Failed to set draw color: " + std::string(SDL_GetError()));

    rect.setTopLeft(rect.getTopLeft() - camera::getActivePos());

    const Vec2 targetRes = renderer::getTargetResolution();
    if (rect.getRight() < 0.0 || rect.getBottom() < 0.0 || rect.x >= targetRes.x ||
        rect.y >= targetRes.y)
    {
        return;
    }

    if (radiusTopLeft < 0.0)
        radiusTopLeft = borderRadius;
    if (radiusTopRight < 0.0)
        radiusTopRight = borderRadius;
    if (radiusBottomRight < 0.0)
        radiusBottomRight = borderRadius;
    if (radiusBottomLeft < 0.0)
        radiusBottomLeft = borderRadius;

    // If has any rounded corners, use polygon rendering
    if (radiusTopLeft > 0.0 || radiusTopRight > 0.0 || radiusBottomRight > 0.0 ||
        radiusBottomLeft > 0.0)
    {
        const double maxRadius = std::max(0.0, std::min(rect.w, rect.h) * 0.5);
        const std::array<double, 4> radii = {
            std::clamp(radiusTopLeft, 0.0, maxRadius),
            std::clamp(radiusTopRight, 0.0, maxRadius),
            std::clamp(radiusBottomRight, 0.0, maxRadius),
            std::clamp(radiusBottomLeft, 0.0, maxRadius),
        };

        if (thickness <= 0 || thickness > rect.w / 2.0 || thickness > rect.h / 2.0)
        {
            _polygonFilled(_roundedRectPolygon(rect, radii), color);
            return;
        }

        for (int i = 0; i < thickness; ++i)
        {
            Rect insetRect = rect;
            insetRect.inflate({-2.0 * i, -2.0 * i});
            if (insetRect.w <= 0.0 || insetRect.h <= 0.0)
                break;

            const std::array<double, 4> insetRadii = {
                std::max(0.0, radiusTopLeft - i),
                std::max(0.0, radiusTopRight - i),
                std::max(0.0, radiusBottomRight - i),
                std::max(0.0, radiusBottomLeft - i),
            };
            _roundedRectOutline(insetRect, color, insetRadii);
        }

        return;
    }

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

void rects(
    const std::vector<Rect>& rects, const Color& color, const int thickness,
    const double borderRadius, double radiusTopLeft, double radiusTopRight,
    double radiusBottomRight, double radiusBottomLeft
)
{
    if (!rend)
        throw std::runtime_error("Renderer not yet initialized");

    if (color.a == 0)
        return;
    if (rects.empty())
        return;

    if (radiusTopLeft < 0.0)
        radiusTopLeft = borderRadius;
    if (radiusTopRight < 0.0)
        radiusTopRight = borderRadius;
    if (radiusBottomRight < 0.0)
        radiusBottomRight = borderRadius;
    if (radiusBottomLeft < 0.0)
        radiusBottomLeft = borderRadius;

    if (radiusTopLeft > 0.0 || radiusTopRight > 0.0 || radiusBottomRight > 0.0 ||
        radiusBottomLeft > 0.0)
    {
        for (const Rect& rectValue : rects)
            rect(
                rectValue, color, thickness, borderRadius, radiusTopLeft, radiusTopRight,
                radiusBottomRight, radiusBottomLeft
            );
        return;
    }

    if (!SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a))
        throw std::runtime_error("Failed to set draw color: " + std::string(SDL_GetError()));

    const Vec2 cameraPos = camera::getActivePos();
    const Vec2 targetRes = renderer::getTargetResolution();

    // Convert to SDL_FRect array with camera offset
    std::vector<SDL_FRect> sdlRects;
    sdlRects.reserve(rects.size());
    for (const Rect& rect : rects)
    {
        if (rect.w < 1.0 || rect.h < 1.0)
            continue;

        const double x = rect.x - cameraPos.x;
        const double y = rect.y - cameraPos.y;
        if (x + rect.w < 0.0 || y + rect.h < 0.0 || x >= targetRes.x || y >= targetRes.y)
            continue;

        const SDL_FRect sdlRect{
            static_cast<float>(x),
            static_cast<float>(y),
            static_cast<float>(rect.w),
            static_cast<float>(rect.h),
        };
        sdlRects.push_back(sdlRect);
    }

    if (sdlRects.empty())
        return;

    // For filled rectangles or thick outlines, use batch fill
    if (thickness <= 0)
    {
        if (!SDL_RenderFillRects(rend, sdlRects.data(), static_cast<int>(sdlRects.size())))
            throw std::runtime_error(
                "Failed to render filled rectangles: " + std::string(SDL_GetError())
            );
        return;
    }

    if (!SDL_RenderRects(rend, sdlRects.data(), static_cast<int>(sdlRects.size())))
        throw std::runtime_error("Failed to render rectangles: " + std::string(SDL_GetError()));

    if (thickness == 1)
        return;

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
    if (!rend)
        throw std::runtime_error("Renderer not yet initialized");

    if (color.a == 0)
        return;

    const size_t size = polygon.points.size();
    if (size == 0)
        return;

    // If will be drawn as point or line, set color now
    if ((size <= 2 || !filled) && !SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a))
        throw std::runtime_error("Failed to set draw color: " + std::string(SDL_GetError()));

    const Vec2 cameraPos = camera::getActivePos();
    if (size == 1)
    {
        const auto [x, y] = static_cast<SDL_FPoint>(polygon.points.at(0) - cameraPos);
        if (!SDL_RenderPoint(rend, x, y))
            throw std::runtime_error("Failed to render point: " + std::string(SDL_GetError()));

        return;
    }
    if (size == 2)
    {
        const auto a = static_cast<SDL_FPoint>(polygon.points.at(0) - cameraPos);
        const auto b = static_cast<SDL_FPoint>(polygon.points.at(1) - cameraPos);

        if (!SDL_RenderLine(rend, a.x, a.y, b.x, b.y))
            throw std::runtime_error("Failed to render line: " + std::string(SDL_GetError()));

        return;
    }

    Polygon cameraPolygon = polygon;
    cameraPolygon.translate(-cameraPos);

    // Just draw lines if not filled
    if (!filled)
    {
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

    _polygonFilled(cameraPolygon, color);
}

void polygons(const std::vector<Polygon>& polygons, const Color& color, const bool filled)
{
    if (!rend)
        throw std::runtime_error("Renderer not yet initialized");

    if (color.a == 0)
        return;

    if (!SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a))
        throw std::runtime_error("Failed to set draw color: " + std::string(SDL_GetError()));

    const Vec2 cameraPos = camera::getActivePos();

    for (const Polygon& polygon : polygons)
    {
        const size_t size = polygon.points.size();
        if (size == 0)
            continue;
        if (size == 1)
        {
            const auto [x, y] = static_cast<SDL_FPoint>(polygon.points.at(0) - cameraPos);
            if (!SDL_RenderPoint(rend, x, y))
                throw std::runtime_error("Failed to render point: " + std::string(SDL_GetError()));

            continue;
        }
        if (size == 2)
        {
            const auto a = static_cast<SDL_FPoint>(polygon.points.at(0) - cameraPos);
            const auto b = static_cast<SDL_FPoint>(polygon.points.at(1) - cameraPos);

            if (!SDL_RenderLine(rend, a.x, a.y, b.x, b.y))
                throw std::runtime_error("Failed to render line: " + std::string(SDL_GetError()));

            continue;
        }

        Polygon cameraPolygon = polygon;
        cameraPolygon.translate(-cameraPos);

        if (!filled)
        {
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

        _polygonFilled(cameraPolygon, color);
    }
}

void geometry(
    const std::shared_ptr<Texture>& texture, const std::vector<Vertex>& vertices,
    const std::vector<int>& indices
)
{
    if (!rend)
        throw std::runtime_error("Renderer not yet initialized");

    if (vertices.empty())
        return;

    const Vec2 cameraPos = camera::getActivePos();

    std::vector<SDL_Vertex> sdlVertices;
    sdlVertices.reserve(vertices.size());

    for (const auto& v : vertices)
    {
        const SDL_Vertex vert{
            static_cast<SDL_FPoint>(v.pos - cameraPos),
            static_cast<SDL_FColor>(v.color),
            static_cast<SDL_FPoint>(v.texCoord),
        };
        sdlVertices.push_back(vert);
    }

    if (!SDL_RenderGeometry(
            rend, texture ? texture->getSDL() : nullptr, sdlVertices.data(),
            static_cast<int>(sdlVertices.size()), indices.empty() ? nullptr : indices.data(),
            static_cast<int>(indices.size())
        ))
    {
        throw std::runtime_error("Failed to draw geometry: " + std::string(SDL_GetError()));
    }
}

void bezier(
    const std::vector<Vec2>& controlPoints, const Color& color, const double thickness,
    const int numSegments
)
{
    if (!rend)
        throw std::runtime_error("Renderer not yet initialized");

    if (controlPoints.size() < 3 || controlPoints.size() > 4)
        throw std::invalid_argument("Bezier curve must have 3 or 4 control points");

    if (color.a == 0)
        return;

    std::vector<Vec2> points;
    points.reserve(numSegments + 1);

    const Vec2 cameraPos = camera::getActivePos();

    for (int i = 0; i <= numSegments; ++i)
    {
        const double t = static_cast<double>(i) / static_cast<double>(numSegments);
        Vec2 p;
        const double mt = 1.0 - t;
        if (controlPoints.size() == 3)
        {
            p = controlPoints[0] * (mt * mt) + controlPoints[1] * (2.0 * mt * t) +
                controlPoints[2] * (t * t);
        }
        else  // size == 4
        {
            p = controlPoints[0] * (mt * mt * mt) + controlPoints[1] * (3.0 * mt * mt * t) +
                controlPoints[2] * (3.0 * mt * t * t) + controlPoints[3] * (t * t * t);
        }
        points.push_back(p - cameraPos);
    }

    if (thickness <= 1.0)
    {
        std::vector<SDL_FPoint> sdlPoints;
        sdlPoints.reserve(points.size());
        for (const auto& p : points)
            sdlPoints.push_back(static_cast<SDL_FPoint>(p));

        if (!SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a))
            throw std::runtime_error("Failed to set draw color: " + std::string(SDL_GetError()));

        if (!SDL_RenderLines(rend, sdlPoints.data(), static_cast<int>(sdlPoints.size())))
            throw std::runtime_error(
                "Failed to render bezier lines: " + std::string(SDL_GetError())
            );
    }
    else
    {
        for (size_t i = 0; i < points.size() - 1; ++i)
        {
            _thickLine(Line(points[i], points[i + 1]), color, thickness);
        }
    }
}

void sector(
    const Circle& circle, double startAngle, double endAngle, const Color& color, double thickness,
    int numSegments
)
{
    if (!rend)
        throw std::runtime_error("Renderer not yet initialized");

    if (circle.radius < 1.0 || color.a == 0)
        return;

    const Vec2 cameraPos = camera::getActivePos();
    const Vec2 targetRes = renderer::getTargetResolution();
    const Vec2 center = circle.pos - cameraPos;

    // Basic culling
    if (center.x + circle.radius < 0.0 || center.y + circle.radius < 0.0 ||
        center.x - circle.radius >= targetRes.x || center.y - circle.radius >= targetRes.y)
    {
        return;
    }

    const auto fColor = static_cast<SDL_FColor>(color);
    const int segments = std::max(1, numSegments);

    const auto buildArc = [&](const double radius) -> std::vector<SDL_FPoint>
    {
        std::vector<SDL_FPoint> arc;
        arc.reserve(static_cast<size_t>(segments) + 1);
        for (int i = 0; i <= segments; ++i)
        {
            const double t = static_cast<double>(i) / static_cast<double>(segments);
            const double theta = startAngle + (endAngle - startAngle) * t;
            arc.push_back(static_cast<SDL_FPoint>(center + Vec2(cos(theta), sin(theta)) * radius));
        }
        return arc;
    };

    if (thickness <= 0.0 || thickness >= circle.radius)
    {
        // Filled sector (pie slice)
        std::vector<SDL_Vertex> vertices;
        vertices.reserve(segments + 2);

        // Center point
        vertices.push_back({static_cast<SDL_FPoint>(center), fColor, {}});

        // Edge points
        for (int i = 0; i <= segments; ++i)
        {
            double t = static_cast<double>(i) / static_cast<double>(segments);
            double theta = startAngle + (endAngle - startAngle) * t;
            Vec2 p = center + Vec2(cos(theta), sin(theta)) * circle.radius;
            vertices.push_back({static_cast<SDL_FPoint>(p), fColor, {}});
        }

        std::vector<int> indices;
        indices.reserve(segments * 3);
        for (int i = 1; i <= segments; ++i)
        {
            indices.push_back(0);
            indices.push_back(i);
            indices.push_back(i + 1);
        }

        if (!SDL_RenderGeometry(
                rend, nullptr, vertices.data(), static_cast<int>(vertices.size()), indices.data(),
                static_cast<int>(indices.size())
            ))
            throw std::runtime_error(
                std::string("Failed to render sector geometry: ") + SDL_GetError()
            );
    }
    else
    {
        // Outline arc with thickness
        std::vector<SDL_Vertex> vertices;
        vertices.reserve((segments + 1) * 2);

        for (int i = 0; i <= segments; ++i)
        {
            double t = static_cast<double>(i) / static_cast<double>(segments);
            double theta = startAngle + (endAngle - startAngle) * t;
            double cosT = cos(theta);
            double sinT = sin(theta);

            // Outer vertex
            vertices.push_back({
                static_cast<SDL_FPoint>(center + Vec2(cosT, sinT) * circle.radius),
                fColor,
                {},
            });
            // Inner vertex
            vertices.push_back({
                static_cast<SDL_FPoint>(center + Vec2(cosT, sinT) * (circle.radius - thickness)),
                fColor,
                {},
            });
        }

        std::vector<int> indices;
        indices.reserve(segments * 6);
        for (int i = 0; i < segments; ++i)
        {
            int topL = i * 2;
            int botL = i * 2 + 1;
            int topR = (i + 1) * 2;
            int botR = (i + 1) * 2 + 1;

            indices.push_back(topL);
            indices.push_back(topR);
            indices.push_back(botL);

            indices.push_back(topR);
            indices.push_back(botR);
            indices.push_back(botL);
        }

        if (!SDL_RenderGeometry(
                rend, nullptr, vertices.data(), static_cast<int>(vertices.size()), indices.data(),
                static_cast<int>(indices.size())
            ))
            throw std::runtime_error(
                std::string("Failed to render sector outline: ") + SDL_GetError()
            );
    }
}

void polyline(
    const std::vector<Vec2>& points, const Color& color, const double thickness, const bool closed
)
{
    if (!rend)
        throw std::runtime_error("Renderer not yet initialized");

    if (points.size() < 2 || color.a == 0)
        return;

    const Vec2 cameraPos = camera::getActivePos();

    std::vector<SDL_FPoint> sdlPoints;
    sdlPoints.reserve(points.size() + (closed ? 1 : 0));
    for (const auto& p : points)
        sdlPoints.push_back(static_cast<SDL_FPoint>(p - cameraPos));

    if (closed && !sdlPoints.empty())
        sdlPoints.push_back(sdlPoints.front());

    if (thickness <= 1.0)
    {
        if (!SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a))
            throw std::runtime_error("Failed to set draw color: " + std::string(SDL_GetError()));

        if (!SDL_RenderLines(rend, sdlPoints.data(), static_cast<int>(sdlPoints.size())))
            throw std::runtime_error("Failed to render polyline: " + std::string(SDL_GetError()));
    }
    else
    {
        const size_t count = sdlPoints.size();
        for (size_t i = 0; i < count - 1; ++i)
        {
            const Vec2 a{sdlPoints[i].x, sdlPoints[i].y};
            const Vec2 b{sdlPoints[i + 1].x, sdlPoints[i + 1].y};
            _thickLine(Line(a, b), color, thickness);
        }
    }
}

void _polygonFilled(const Polygon& polygon, const Color& color)
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
            rend, nullptr, sdlVertices.data(), static_cast<int>(sdlVertices.size()),
            reinterpret_cast<const int*>(indices.data()), static_cast<int>(indices.size())
        ))
    {
        throw std::runtime_error(
            std::string("Failed to render polygon geometry: ") + SDL_GetError()
        );
    }
}

void _ellipseFilled(
    const Vec2& center, double radiusX, double radiusY, const Color& color, int numSegments
)
{
    std::vector<SDL_Vertex> vertices;
    vertices.reserve(numSegments + 2);

    const auto fColor = static_cast<SDL_FColor>(color);

    // Center point
    vertices.push_back({static_cast<SDL_FPoint>(center), fColor, {0.0f, 0.0f}});

    // Edge points
    for (int i = 0; i <= numSegments; ++i)
    {
        auto theta = 2.0 * M_PI * static_cast<double>(i) / static_cast<double>(numSegments);
        Vec2 p{
            center.x + radiusX * cos(theta),
            center.y + radiusY * sin(theta),
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
            rend, nullptr, vertices.data(), static_cast<int>(vertices.size()), indices.data(),
            static_cast<int>(indices.size())
        ))
    {
        throw std::runtime_error(
            std::string("Failed to render ellipse geometry: ") + SDL_GetError()
        );
    }
}

void _ellipseOutline(
    const Vec2& center, double radiusX, double radiusY, const Color& color, double thickness,
    int numSegments
)
{
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
            center.x + radiusX * cosT,
            center.y + radiusY * sinT,
        };
        vertices.push_back({static_cast<SDL_FPoint>(outerPoint), fColor, {}});

        // Inner vertex
        Vec2 innerPoint{
            center.x + (radiusX - thickness) * cosT,
            center.y + (radiusY - thickness) * sinT,
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
            rend, nullptr, vertices.data(), static_cast<int>(vertices.size()), indices.data(),
            static_cast<int>(indices.size())
        ))
    {
        throw std::runtime_error(
            std::string("Failed to render ellipse outline: ") + SDL_GetError()
        );
    }
}

void _capsuleFilled(const Capsule& capsule, const Color& color, int numSegments)
{
    const Vec2 cameraPos = camera::getActivePos();
    const Vec2 p1 = capsule.p1 - cameraPos;
    const Vec2 p2 = capsule.p2 - cameraPos;

    const double radius = capsule.radius;
    const auto fColor = static_cast<SDL_FColor>(color);

    const double angle = atan2(p2.y - p1.y, p2.x - p1.x);

    std::vector<Vec2> points;
    points.reserve(numSegments + 2);

    // Semicircle around p1
    for (int i = 0; i <= numSegments / 2; ++i)
    {
        double theta = angle + M_PI / 2.0 + (M_PI * i) / (numSegments / 2.0);
        points.push_back(p1 + Vec2(cos(theta), sin(theta)) * radius);
    }

    // Semicircle around p2
    for (int i = 0; i <= numSegments / 2; ++i)
    {
        double theta = angle - M_PI / 2.0 + (M_PI * i) / (numSegments / 2.0);
        points.push_back(p2 + Vec2(cos(theta), sin(theta)) * radius);
    }

    std::vector<std::vector<Vec2>> polygon = {points};
    std::vector<uint32_t> indices = mapbox::earcut<uint32_t>(polygon);

    std::vector<SDL_Vertex> vertices;
    vertices.reserve(points.size());
    for (const auto& p : points)
        vertices.push_back({static_cast<SDL_FPoint>(p), fColor, {}});

    if (!SDL_RenderGeometry(
            rend, nullptr, vertices.data(), static_cast<int>(vertices.size()),
            reinterpret_cast<const int*>(indices.data()), static_cast<int>(indices.size())
        ))
    {
        throw std::runtime_error(
            std::string("Failed to render capsule geometry: ") + SDL_GetError()
        );
    }
}

void _capsuleOutline(const Capsule& capsule, const Color& color, double thickness, int numSegments)
{
    const Vec2 cameraPos = camera::getActivePos();
    const Vec2 p1 = capsule.p1 - cameraPos;
    const Vec2 p2 = capsule.p2 - cameraPos;

    const double rOuter = capsule.radius;
    const double rInner = capsule.radius - thickness;

    if (rOuter <= 0.0 || rInner <= 0.0 || thickness <= 0.0)
        return;

    const auto fColor = static_cast<SDL_FColor>(color);

    std::vector<SDL_FPoint> outer = _capsulePolyline(p1, p2, rOuter, numSegments);
    std::vector<SDL_FPoint> inner = _capsulePolyline(p1, p2, rInner, numSegments);

    if (outer.size() < 3 || inner.size() != outer.size())
        return;

    const auto ensureClosed = [](std::vector<SDL_FPoint>& pts) -> void
    {
        if (pts.empty())
            return;
        const SDL_FPoint& a = pts.front();
        const SDL_FPoint& b = pts.back();
        if (a.x != b.x || a.y != b.y)
            pts.push_back(a);
    };
    ensureClosed(outer);
    ensureClosed(inner);

    const size_t n = outer.size();
    if (inner.size() != n || n < 4)
        return;

    std::vector<SDL_Vertex> verts;
    verts.reserve(n * 2);
    for (size_t i = 0; i < n; ++i)
    {
        verts.push_back({outer[i], fColor, {}});
        verts.push_back({inner[i], fColor, {}});
    }

    std::vector<int> indices;
    indices.reserve((n - 1) * 6);

    for (size_t i = 0; i < n - 1; ++i)
    {
        const int o0 = static_cast<int>(i * 2);
        const int i0 = o0 + 1;
        const int o1 = static_cast<int>((i + 1) * 2);
        const int i1 = o1 + 1;

        indices.push_back(o0);
        indices.push_back(o1);
        indices.push_back(i0);
        indices.push_back(o1);
        indices.push_back(i1);
        indices.push_back(i0);
    }

    if (!SDL_RenderGeometry(
            rend, nullptr, verts.data(), static_cast<int>(verts.size()), indices.data(),
            static_cast<int>(indices.size())
        ))
    {
        throw std::runtime_error(
            std::string("Failed to render capsule outline: ") + SDL_GetError()
        );
    }
}

void _thickLine(const Line& line, const Color& color, double thickness)
{
    const Vec2 a = line.getA();
    const Vec2 b = line.getB();
    Vec2 dir = b - a;

    double len = dir.getLength();
    if (len < 0.0001)
        return;

    dir /= len;
    const Vec2 norm = {-dir.y, dir.x};
    const Vec2 offset = norm * (thickness * 0.5);

    SDL_Vertex vertices[4];
    const auto fColor = static_cast<SDL_FColor>(color);

    vertices[0] = {static_cast<SDL_FPoint>(a + offset), fColor, {}};
    vertices[1] = {static_cast<SDL_FPoint>(a - offset), fColor, {}};
    vertices[2] = {static_cast<SDL_FPoint>(b + offset), fColor, {}};
    vertices[3] = {static_cast<SDL_FPoint>(b - offset), fColor, {}};

    int indices[6] = {0, 1, 2, 2, 1, 3};

    if (!SDL_RenderGeometry(rend, nullptr, vertices, 4, indices, 6))
    {
        throw std::runtime_error(std::string("Failed to render thick line: ") + SDL_GetError());
    }
}

Polygon _roundedRectPolygon(const Rect& rect, const std::array<double, 4>& radii)
{
    const auto appendArcPoints = [](std::vector<Vec2>& points, const Vec2& center,
                                    const double radius, const double startAngle,
                                    const double endAngle, const int segments) -> void
    {
        if (radius <= 0.0)
            return;

        for (int i = 1; i <= segments; ++i)
        {
            const double t = static_cast<double>(i) / static_cast<double>(segments);
            const double angle = startAngle + (endAngle - startAngle) * t;
            points.push_back(center + Vec2(cos(angle), sin(angle)) * radius);
        }
    };

    const auto [radiusTopLeft, radiusTopRight, radiusBottomRight, radiusBottomLeft] = radii;
    const double maxRadius = std::
        max(std::max(radiusTopLeft, radiusTopRight), std::max(radiusBottomRight, radiusBottomLeft));
    const int cornerSegments =
        std::max(4, std::min(24, static_cast<int>(std::ceil(maxRadius / 3.0))));

    std::vector<Vec2> points;
    points.reserve(static_cast<size_t>(cornerSegments) * 4 + 8);

    points.push_back({rect.x + radiusTopLeft, rect.y});
    points.push_back({rect.x + rect.w - radiusTopRight, rect.y});
    appendArcPoints(
        points, {rect.x + rect.w - radiusTopRight, rect.y + radiusTopRight}, radiusTopRight,
        -M_PI / 2.0, 0.0, cornerSegments
    );

    points.push_back({rect.x + rect.w, rect.y + rect.h - radiusBottomRight});
    appendArcPoints(
        points, {rect.x + rect.w - radiusBottomRight, rect.y + rect.h - radiusBottomRight},
        radiusBottomRight, 0.0, M_PI / 2.0, cornerSegments
    );

    points.push_back({rect.x + radiusBottomLeft, rect.y + rect.h});
    appendArcPoints(
        points, {rect.x + radiusBottomLeft, rect.y + rect.h - radiusBottomLeft}, radiusBottomLeft,
        M_PI / 2.0, M_PI, cornerSegments
    );

    points.push_back({rect.x, rect.y + radiusTopLeft});
    appendArcPoints(
        points, {rect.x + radiusTopLeft, rect.y + radiusTopLeft}, radiusTopLeft, M_PI,
        3.0 * M_PI / 2.0, cornerSegments
    );

    return Polygon(points);
}

void _roundedRectOutline(const Rect& rect, const Color& color, const std::array<double, 4>& radii)
{
    if (!SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a))
        throw std::runtime_error("Failed to set draw color: " + std::string(SDL_GetError()));

    const Polygon polygon = _roundedRectPolygon(rect, radii);
    std::vector<SDL_FPoint> points;
    points.reserve(polygon.points.size() + 1);
    for (const Vec2& point : polygon.points)
        points.push_back(static_cast<SDL_FPoint>(point));

    if (!points.empty())
        points.push_back(points.front());

    if (points.empty())
        return;

    if (!SDL_RenderLines(rend, points.data(), static_cast<int>(points.size())))
        throw std::runtime_error(
            "Failed to render rounded rectangle: " + std::string(SDL_GetError())
        );
}

std::vector<SDL_FPoint> _ellipsePolyline(
    const Vec2& center, const double radiusX, const double radiusY, const int numSegments
)
{
    const int segments = std::max(3, numSegments);
    std::vector<SDL_FPoint> points;
    points.reserve(static_cast<size_t>(segments) + 1);

    for (int i = 0; i <= segments; ++i)
    {
        const double theta = 2.0 * M_PI * static_cast<double>(i) / static_cast<double>(segments);
        points.push_back(
            static_cast<SDL_FPoint>(
                Vec2{center.x + radiusX * cos(theta), center.y + radiusY * sin(theta)}
            )
        );
    }

    return points;
}

std::vector<SDL_FPoint> _capsulePolyline(
    const Vec2& p1, const Vec2& p2, const double radius, const int numSegments
)
{
    const int halfSegments = std::max(2, numSegments / 2);
    const double angle = atan2(p2.y - p1.y, p2.x - p1.x);

    std::vector<SDL_FPoint> points;
    points.reserve(static_cast<size_t>(halfSegments) * 2 + 3);

    for (int i = 0; i <= halfSegments; ++i)
    {
        const double theta = angle + M_PI / 2.0 + (M_PI * i) / static_cast<double>(halfSegments);
        points.push_back(static_cast<SDL_FPoint>(p1 + Vec2(cos(theta), sin(theta)) * radius));
    }

    for (int i = 0; i <= halfSegments; ++i)
    {
        const double theta = angle - M_PI / 2.0 + (M_PI * i) / static_cast<double>(halfSegments);
        points.push_back(static_cast<SDL_FPoint>(p2 + Vec2(cos(theta), sin(theta)) * radius));
    }

    if (!points.empty())
        points.push_back(points.front());

    return points;
}

void _init(SDL_Renderer* renderer)
{
    rend = renderer;
}

void _bind(py::module_& module)
{
    py::classh<Vertex>(module, "Vertex", "A vertex with position, color, and texture coordinates.")
        .def(
            py::init(
                [](const Vec2& position, py::object colorObj, py::object texCoordObj) -> Vertex
                {
                    const auto color = colorObj.is_none() ? WHITE : colorObj.cast<Color>();
                    const auto texCoord = texCoordObj.is_none() ? Vec2::ZERO()
                                                                : texCoordObj.cast<Vec2>();

                    return {position, color, texCoord};
                }
            ),
            py::arg("position"), py::arg("color") = py::none(), py::arg("tex_coord") = py::none(),
            R"doc(
Create a new Vertex.

Args:
    position (Vec2): The position of the vertex in world space.
    color (Color | None): The color of the vertex. Defaults to White.
    tex_coord (Vec2 | None): The texture coordinate of the vertex. Defaults to (0, 0).
            )doc"
        )
        .def_readwrite("position", &Vertex::pos, "Position of the vertex in world space.")
        .def_readwrite("color", &Vertex::color, "Color of the vertex.")
        .def_readwrite("tex_coord", &Vertex::texCoord, "Texture coordinate of the vertex.")
        .def(
            "__repr__",
            [](const Vertex& self)
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
        py::arg("num_segments") = 24, R"doc(
Draw a circle to the renderer.

Args:
    circle (Circle): The circle to draw.
    color (Color): The color of the circle.
    thickness (float, optional): The line thickness. If <= 0 or >= radius, draws filled circle. Defaults to 0 (filled).
    num_segments (int, optional): Number of segments to approximate the circle. Higher values yield smoother circles. Defaults to 24.
    )doc"
    );

    subDraw.def(
        "circles", &circles, py::arg("circles"), py::arg("color"), py::arg("thickness") = 0,
        py::arg("num_segments") = 24, R"doc(
Draw an array of circles in bulk to the renderer.

Args:
    circles (Sequence[Circle]): The circles to draw in bulk.
    color (Color): The color of the circles.
    thickness (float, optional): The line thickness. If <= 0 or >= radius, draws filled circle. Defaults to 0 (filled).
    num_segments (int, optional): Number of segments to approximate each circle. Higher values yield smoother circles. Defaults to 24.
    )doc"
    );

    subDraw.def(
        "capsule", &capsule, py::arg("capsule"), py::arg("color"), py::arg("thickness") = 0,
        py::arg("num_segments") = 24, R"doc(
Draw a capsule to the renderer.

Args:
    capsule (Capsule): The capsule to draw.
    color (Color): The color of the capsule.
    thickness (float, optional): The line thickness. If <= 0 or >= radius, draws filled capsule. Defaults to 0 (filled).
    num_segments (int, optional): Number of segments to approximate the capsule ends. Higher values yield smoother capsules. Defaults to 24.
    )doc"
    );

    subDraw.def(
        "capsules", &capsules, py::arg("capsules"), py::arg("color"), py::arg("thickness") = 0,
        py::arg("num_segments") = 24, R"doc(
Draw an array of capsules in bulk to the renderer.

Args:
    capsules (Sequence[Capsule]): The capsules to draw in bulk.
    color (Color): The color of the capsules.
    thickness (float, optional): The line thickness. If <= 0 or >= radius, draws filled capsules. Defaults to 0 (filled).
    num_segments (int, optional): Number of segments to approximate each capsule end. Higher values yield smoother capsules. Defaults to 24.
    )doc"
    );

    subDraw.def(
        "ellipse", &ellipse, py::arg("bounds"), py::arg("color"), py::arg("thickness") = 0.0,
        py::arg("num_segments") = 24, R"doc(
Draw an ellipse to the renderer.

Args:
    bounds (Rect): The bounding box of the ellipse.
    color (Color): The color of the ellipse.
    thickness (float, optional): The line thickness. If <= 0 or >= radius, draws filled ellipse. Defaults to 0 (filled).
    num_segments (int, optional): Number of segments to approximate the ellipse. Higher values yield smoother ellipses. Defaults to 24.
    )doc"
    );

    subDraw.def(
        "ellipses", &ellipses, py::arg("bounds"), py::arg("color"), py::arg("thickness") = 0.0,
        py::arg("num_segments") = 24, R"doc(
Draw an array of ellipses in bulk to the renderer.

Args:
    bounds (Sequence[Rect]): The bounding boxes of the ellipses to draw in bulk.
    color (Color): The color of the ellipses.
    thickness (float, optional): The line thickness. If <= 0 or >= radius, draws filled ellipses. Defaults to 0 (filled).
    num_segments (int, optional): Number of segments to approximate each ellipse. Higher values yield smoother ellipses. Defaults to 24.
    )doc"
    );

    subDraw.def(
        "line", &line, py::arg("line"), py::arg("color"), py::arg("thickness") = 1.0,
        R"doc(
Draw a line to the renderer.

Args:
    line (Line): The line to draw.
    color (Color): The color of the line.
    thickness (float, optional): The line thickness in pixels. Defaults to 1.0.
    )doc"
    );

    subDraw.def(
        "lines", &lines, py::arg("lines"), py::arg("color"), py::arg("thickness") = 1.0,
        R"doc(
Batch draw an array of lines to the renderer.

Args:
    lines (Sequence[Line]): The lines to batch draw.
    color (Color): The color of the lines.
    thickness (float, optional): The line thickness in pixels. Defaults to 1.0.
    )doc"
    );

    subDraw.def(
        "rect", &rect, py::arg("rect"), py::arg("color"), py::arg("thickness") = 0,
        py::arg("border_radius") = 0.0, py::arg("radius_top_left") = -1.0,
        py::arg("radius_top_right") = -1.0, py::arg("radius_bottom_right") = -1.0,
        py::arg("radius_bottom_left") = -1.0,
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
        "rects", &rects, py::arg("rects"), py::arg("color"), py::arg("thickness") = 0,
        py::arg("border_radius") = 0.0, py::arg("radius_top_left") = -1.0,
        py::arg("radius_top_right") = -1.0, py::arg("radius_bottom_right") = -1.0,
        py::arg("radius_bottom_left") = -1.0,
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
        "polygon", &polygon, py::arg("polygon"), py::arg("color"), py::arg("filled") = true,
        R"doc(
Draw a polygon to the renderer.

Args:
    polygon (Polygon): The polygon to draw.
    color (Color): The color of the polygon.
    filled (bool, optional): Whether to draw a filled polygon or just the outline. Defaults to True.
    )doc"
    );

    subDraw.def(
        "polygons", &polygons, py::arg("polygons"), py::arg("color"), py::arg("filled") = true,
        R"doc(
Draw an array of polygons in bulk to the renderer.

Args:
    polygons (Sequence[Polygon]): The polygons to draw in bulk.
    color (Color): The color of the polygons.
    filled (bool, optional): Whether to draw filled polygons or just the outlines. Defaults to True (filled).
    )doc"
    );

    subDraw.def(
        "geometry",
        [](const std::shared_ptr<Texture>& texture, const std::vector<Vertex>& vertices,
           const py::object& indicesObj)
        {
            std::vector<int> indices;
            if (!indicesObj.is_none())
                indices = indicesObj.cast<std::vector<int>>();

            geometry(texture, vertices, indices);
        },
        py::arg("texture").none(true), py::arg("vertices"), py::arg("indices") = py::none(),
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
        "bezier", &bezier, py::arg("control_points"), py::arg("color"), py::arg("thickness") = 1.0,
        py::arg("num_segments") = 24, R"doc(
Draw a Bezier curve with 3 or 4 control points.

Args:
    control_points (Sequence[Vec2]): The control points (3 for quadratic, 4 for cubic).
    color (Color): The color of the curve.
    thickness (float, optional): The line thickness. Defaults to 1.0.
    num_segments (int, optional): Number of segments to approximate the curve. Defaults to 24.
    )doc"
    );

    subDraw.def(
        "sector", &sector, py::arg("circle"), py::arg("start_angle"), py::arg("end_angle"),
        py::arg("color"), py::arg("thickness") = 0.0, py::arg("num_segments") = 24,
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
        "polyline", &polyline, py::arg("points"), py::arg("color"), py::arg("thickness") = 1.0,
        py::arg("closed") = false, R"doc(
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
