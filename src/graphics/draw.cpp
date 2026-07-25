#include "kraken/graphics/Draw.hpp"

#include <algorithm>
#include <array>
#include <sstream>

#include "geometry/earcut.hpp"
#include "kraken/geometry/Capsule.hpp"
#include "kraken/geometry/Circle.hpp"
#include "kraken/geometry/Line.hpp"
#include "kraken/geometry/Polygon.hpp"
#include "kraken/geometry/Rect.hpp"
#include "kraken/graphics/Camera.hpp"
#include "kraken/graphics/Renderer.hpp"
#include "kraken/graphics/Texture.hpp"

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

namespace kn::draw
{
SDL_Renderer* rend = nullptr;

// Line helper for thick lines
void _thickLine(const Line& line, const Color& color, double thickness);

namespace
{
bool isScreenAabbVisible(
    const double minX, const double minY, const double maxX, const double maxY,
    const Vec2& renderSize
)
{
    return !(maxX < 0.0 || maxY < 0.0 || minX >= renderSize.x || minY >= renderSize.y);
}

bool arePointsVisible(const std::vector<Vec2>& points, const Vec2& renderSize)
{
    if (points.empty())
        return false;

    auto [minX, minY] = points.front();
    auto [maxX, maxY] = points.front();

    for (const Vec2& point : points)
    {
        minX = std::min(minX, point.x);
        minY = std::min(minY, point.y);
        maxX = std::max(maxX, point.x);
        maxY = std::max(maxY, point.y);
    }

    return isScreenAabbVisible(minX, minY, maxX, maxY, renderSize);
}

std::vector<Vec2> toScreenPoints(const std::vector<Vec2>& worldPoints)
{
    std::vector<Vec2> screenPoints;
    screenPoints.reserve(worldPoints.size());

    for (const Vec2& worldPoint : worldPoints)
        screenPoints.push_back(camera::worldToScreen(worldPoint));

    return screenPoints;
}

std::vector<Vec2> getEllipsePoints(
    const Vec2& center, const double radiusX, const double radiusY, const int numSegments
)
{
    const int segments = std::max(8, numSegments);

    std::vector<Vec2> points;
    points.reserve(static_cast<size_t>(segments));

    for (int i = 0; i < segments; ++i)
    {
        const double theta = (2.0 * M_PI * static_cast<double>(i)) / static_cast<double>(segments);
        points.push_back(center + Vec2(cos(theta) * radiusX, sin(theta) * radiusY));
    }

    return points;
}

void drawPolylineScreen(
    const std::vector<Vec2>& points, const Color& color, const double thickness, const bool closed
)
{
    if (points.size() < 2)
        return;

    if (thickness <= 1.0)
    {
        if (!SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a))
            throw std::runtime_error("Failed to set draw color: " + std::string(SDL_GetError()));

        std::vector<SDL_FPoint> sdlPoints;
        sdlPoints.reserve(points.size() + (closed ? 1 : 0));
        for (const Vec2& point : points)
            sdlPoints.push_back(static_cast<SDL_FPoint>(point));

        if (closed)
            sdlPoints.push_back(sdlPoints.front());

        if (!SDL_RenderLines(rend, sdlPoints.data(), static_cast<int>(sdlPoints.size())))
            throw std::runtime_error("Failed to render polyline: " + std::string(SDL_GetError()));

        return;
    }

    for (size_t i = 0; i < points.size() - 1; ++i)
        _thickLine(Line(points[i], points[i + 1]), color, thickness);

    if (closed)
        _thickLine(Line(points.back(), points.front()), color, thickness);
}
}  // namespace

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

    const Vec2 rendRes = renderer::getCurrentResolution();

    const Vec2 center = camera::worldToScreen(circle.pos);
    if (!isScreenAabbVisible(
            center.x - circle.radius, center.y - circle.radius, center.x + circle.radius,
            center.y + circle.radius, rendRes
        ))
    {
        return;
    }

    const bool filled = (thickness <= 0.0 || thickness >= circle.radius);
    if (filled)
        _ellipseFilled(center, circle.radius, circle.radius, color, numSegments);
    else
        _ellipseOutline(center, circle.radius, circle.radius, color, thickness, numSegments);
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

    const Vec2 rendRes = renderer::getCurrentResolution();

    for (const Circle& circle : circles)
    {
        if (circle.radius < 1.0)
            continue;

        const Vec2 center = camera::worldToScreen(circle.pos);
        if (!isScreenAabbVisible(
                center.x - circle.radius, center.y - circle.radius, center.x + circle.radius,
                center.y + circle.radius, rendRes
            ))
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

    const Vec2 rendRes = renderer::getCurrentResolution();

    const double r = capsule.radius;
    const Vec2 p1 = camera::worldToScreen(capsule.p1);
    const Vec2 p2 = camera::worldToScreen(capsule.p2);

    const double minX = std::min(p1.x, p2.x) - r;
    const double minY = std::min(p1.y, p2.y) - r;
    const double maxX = std::max(p1.x, p2.x) + r;
    const double maxY = std::max(p1.y, p2.y) + r;
    if (maxX < 0.0 || maxY < 0.0 || minX >= rendRes.x || minY >= rendRes.y)
        return;

    const bool filled = (thickness <= 0.0 || thickness >= r);
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

    const Vec2 rendRes = renderer::getCurrentResolution();

    for (const auto& c : capsules)
    {
        if (c.radius < 1.0 || color.a == 0)
            continue;

        const double r = c.radius;
        const Vec2 p1 = camera::worldToScreen(c.p1);
        const Vec2 p2 = camera::worldToScreen(c.p2);

        const double minX = std::min(p1.x, p2.x) - r;
        const double minY = std::min(p1.y, p2.y) - r;
        const double maxX = std::max(p1.x, p2.x) + r;
        const double maxY = std::max(p1.y, p2.y) + r;
        if (maxX < 0.0 || maxY < 0.0 || minX >= rendRes.x || minY >= rendRes.y)
            continue;

        const bool filled = (thickness <= 0.0 || thickness >= r);
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

    const Vec2 rendRes = renderer::getCurrentResolution();
    point = camera::worldToScreen(point);
    if (point.x < 0.0 || point.y < 0.0 || point.x >= rendRes.x || point.y >= rendRes.y)
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

    const Vec2 rendRes = renderer::getCurrentResolution();
    for (Vec2 point : points)
    {
        point = camera::worldToScreen(point);
        if (point.x >= 0.0 && point.y >= 0.0 && point.x < rendRes.x && point.y < rendRes.y)
            sdlPoints.push_back(static_cast<SDL_FPoint>(point));
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

    const Vec2 center = bounds.getCenter();
    const double radiusX = bounds.w / 2.0;
    const double radiusY = bounds.h / 2.0;
    const std::vector<Vec2> screenPoints = toScreenPoints(
        getEllipsePoints(center, radiusX, radiusY, numSegments)
    );

    if (!arePointsVisible(screenPoints, renderer::getCurrentResolution()))
        return;

    const bool filled = (thickness <= 0.0 || (thickness >= radiusX && thickness >= radiusY));
    if (filled)
        _polygonFilled(Polygon(screenPoints), color);
    else
        drawPolylineScreen(screenPoints, color, thickness, true);
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

    for (const auto& rect : bounds)
    {
        if (rect.w < 1 || rect.h < 1)
            continue;

        const Vec2 center = rect.getCenter();
        const double radiusX = rect.w / 2.0;
        const double radiusY = rect.h / 2.0;
        const std::vector<Vec2> screenPoints = toScreenPoints(
            getEllipsePoints(center, radiusX, radiusY, numSegments)
        );

        if (!arePointsVisible(screenPoints, renderer::getCurrentResolution()))
            continue;

        if (thickness <= 0.0 || (thickness >= radiusX && thickness >= radiusY))
            _polygonFilled(Polygon(screenPoints), color);
        else
            drawPolylineScreen(screenPoints, color, thickness, true);
    }
}

void line(Line line, const Color& color, const double thickness)
{
    if (!rend)
        throw std::runtime_error("Renderer not yet initialized");

    if (color.a == 0)
        return;

    const Vec2 screenA = line.getA();
    const Vec2 screenB = line.getB();

    if (thickness <= 1.0)
    {
        const auto a = static_cast<SDL_FPoint>(screenA);
        const auto b = static_cast<SDL_FPoint>(screenB);

        if (!SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a))
            throw std::runtime_error("Failed to set draw color: " + std::string(SDL_GetError()));
        if (!SDL_RenderLine(rend, a.x, a.y, b.x, b.y))
            throw std::runtime_error("Failed to render line: " + std::string(SDL_GetError()));
    }
    else
    {
        _thickLine({screenA, screenB}, color, thickness);
    }
}

void lines(const std::vector<Line>& lines, const Color& color, const double thickness)
{
    if (!rend)
        throw std::runtime_error("Renderer not yet initialized");

    if (lines.empty() || color.a == 0)
        return;

    if (thickness <= 1.0)
    {
        if (!SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a))
            throw std::runtime_error("Failed to set draw color: " + std::string(SDL_GetError()));

        for (const auto& line : lines)
        {
            const auto a = static_cast<SDL_FPoint>(camera::worldToScreen(line.getA()));
            const auto b = static_cast<SDL_FPoint>(camera::worldToScreen(line.getB()));

            if (!SDL_RenderLine(rend, a.x, a.y, b.x, b.y))
                throw std::runtime_error("Failed to render line: " + std::string(SDL_GetError()));
        }
    }
    else
    {
        for (const auto& line : lines)
            _thickLine(
                {
                    camera::worldToScreen(line.getA()),
                    camera::worldToScreen(line.getB()),
                },
                color, thickness
            );
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

    if (radiusTopLeft < 0.0)
        radiusTopLeft = borderRadius;
    if (radiusTopRight < 0.0)
        radiusTopRight = borderRadius;
    if (radiusBottomRight < 0.0)
        radiusBottomRight = borderRadius;
    if (radiusBottomLeft < 0.0)
        radiusBottomLeft = borderRadius;

    std::vector<Vec2> worldPoints;

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
        worldPoints = _roundedRectPolygon(rect, radii).points;
    }
    else
    {
        const auto corners = rect.getCorners();
        worldPoints.insert(worldPoints.end(), corners.begin(), corners.end());
    }

    std::vector<Vec2> screenPoints = toScreenPoints(worldPoints);
    if (!arePointsVisible(screenPoints, renderer::getCurrentResolution()))
        return;

    if (thickness <= 0 || thickness > rect.w / 2.0 || thickness > rect.h / 2.0)
        _polygonFilled(Polygon(screenPoints), color);
    else
        drawPolylineScreen(screenPoints, color, static_cast<double>(thickness), true);
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

    for (const Rect& rectValue : rects)
        rect(
            rectValue, color, thickness, borderRadius, radiusTopLeft, radiusTopRight,
            radiusBottomRight, radiusBottomLeft
        );
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

    if (size == 1)
    {
        const auto [x, y] = static_cast<SDL_FPoint>(camera::worldToScreen(polygon.points.at(0)));
        if (!SDL_RenderPoint(rend, x, y))
            throw std::runtime_error("Failed to render point: " + std::string(SDL_GetError()));

        return;
    }

    if (size == 2)
    {
        const auto a = static_cast<SDL_FPoint>(camera::worldToScreen(polygon.points.at(0)));
        const auto b = static_cast<SDL_FPoint>(camera::worldToScreen(polygon.points.at(1)));

        if (!SDL_RenderLine(rend, a.x, a.y, b.x, b.y))
            throw std::runtime_error("Failed to render line: " + std::string(SDL_GetError()));

        return;
    }

    Polygon cameraPolygon(toScreenPoints(polygon.points));

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

    for (const Polygon& polygon : polygons)
    {
        const size_t size = polygon.points.size();
        if (size == 0)
            continue;
        if (size == 1)
        {
            const auto [x, y] = static_cast<SDL_FPoint>(
                camera::worldToScreen(polygon.points.at(0))
            );
            if (!SDL_RenderPoint(rend, x, y))
                throw std::runtime_error("Failed to render point: " + std::string(SDL_GetError()));

            continue;
        }
        if (size == 2)
        {
            const auto a = static_cast<SDL_FPoint>(camera::worldToScreen(polygon.points.at(0)));
            const auto b = static_cast<SDL_FPoint>(camera::worldToScreen(polygon.points.at(1)));

            if (!SDL_RenderLine(rend, a.x, a.y, b.x, b.y))
                throw std::runtime_error("Failed to render line: " + std::string(SDL_GetError()));

            continue;
        }

        Polygon cameraPolygon(toScreenPoints(polygon.points));

        if (!filled)
        {
            std::vector<SDL_FPoint> points;
            points.reserve(cameraPolygon.points.size() + 1);
            for (const auto& p : cameraPolygon.points)
                points.push_back(static_cast<SDL_FPoint>(p));
            points.push_back(points.front());

            if (!SDL_RenderLines(rend, points.data(), static_cast<int>(points.size())))
                throw std::runtime_error(
                    "Failed to render polygon outline: " + std::string(SDL_GetError())
                );

            continue;
        }

        _polygonFilled(cameraPolygon, color);
    }
}

void geometry(
    const Texture* texture, const std::vector<Vertex>& vertices, const std::vector<int>& indices
)
{
    if (!rend)
        throw std::runtime_error("Renderer not yet initialized");

    if (vertices.empty())
        return;

    if (texture && !texture->hasUsage(TextureUsage::Drawable))
        throw std::runtime_error("Texture is not drawable");

    std::vector<SDL_Vertex> sdlVertices;
    sdlVertices.reserve(vertices.size());
    for (const auto& v : vertices)
    {
        const SDL_Vertex vert{
            static_cast<SDL_FPoint>(camera::worldToScreen(v.pos)),
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
        throw std::runtime_error("Failed to draw geometry: " + std::string(SDL_GetError()));
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

        points.push_back(camera::worldToScreen(p));
    }

    if (thickness > 1.0)
    {
        for (size_t i = 0; i < points.size() - 1; ++i)
            _thickLine({points[i], points[i + 1]}, color, thickness);
        return;
    }

    std::vector<SDL_FPoint> sdlPoints;
    sdlPoints.reserve(points.size());
    for (const auto& p : points)
        sdlPoints.push_back(static_cast<SDL_FPoint>(p));

    if (!SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a))
        throw std::runtime_error("Failed to set draw color: " + std::string(SDL_GetError()));

    if (!SDL_RenderLines(rend, sdlPoints.data(), static_cast<int>(sdlPoints.size())))
        throw std::runtime_error("Failed to render bezier lines: " + std::string(SDL_GetError()));
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

    const Vec2 rendRes = renderer::getCurrentResolution();
    const Vec2 center = camera::worldToScreen(circle.pos);
    const double cameraAngle = camera::getActiveAngle();

    // Basic culling
    if (center.x + circle.radius < 0.0 || center.y + circle.radius < 0.0 ||
        center.x - circle.radius >= rendRes.x || center.y - circle.radius >= rendRes.y)
        return;

    const auto fColor = static_cast<SDL_FColor>(color);
    const int segments = std::max(1, numSegments);

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
            double theta = startAngle + (endAngle - startAngle) * t + cameraAngle;
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

        return;
    }

    // Outline arc with thickness
    std::vector<SDL_Vertex> vertices;
    vertices.reserve((segments + 1) * 2);

    for (int i = 0; i <= segments; ++i)
    {
        double t = static_cast<double>(i) / static_cast<double>(segments);
        double theta = startAngle + (endAngle - startAngle) * t + cameraAngle;
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
        throw std::runtime_error(std::string("Failed to render sector outline: ") + SDL_GetError());
}

void polyline(
    const std::vector<Vec2>& points, const Color& color, const double thickness, const bool closed
)
{
    if (!rend)
        throw std::runtime_error("Renderer not yet initialized");

    if (points.size() < 2 || color.a == 0)
        return;

    std::vector<SDL_FPoint> sdlPoints;
    sdlPoints.reserve(points.size() + (closed ? 1 : 0));
    for (const auto& p : points)
        sdlPoints.push_back(static_cast<SDL_FPoint>(camera::worldToScreen(p)));

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
        sdlVertices.push_back({static_cast<SDL_FPoint>(point), fColor, {}});

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
    const Vec2 p1 = camera::worldToScreen(capsule.p1);
    const Vec2 p2 = camera::worldToScreen(capsule.p2);

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
    const Vec2 p1 = camera::worldToScreen(capsule.p1);
    const Vec2 p2 = camera::worldToScreen(capsule.p2);

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

    static constexpr int indices[6] = {0, 1, 2, 2, 1, 3};

    if (!SDL_RenderGeometry(rend, nullptr, vertices, 4, indices, 6))
        throw std::runtime_error(std::string("Failed to render thick line: ") + SDL_GetError());
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

}  // namespace kn::draw
