#include "kraken/geometry/Collision.hpp"

#include <algorithm>

#include "kraken/geometry/Circle.hpp"
#include "kraken/geometry/Line.hpp"
#include "kraken/geometry/Polygon.hpp"
#include "kraken/geometry/Rect.hpp"
#include "kraken/math/Math.hpp"

namespace kn::collision
{
bool overlap(const Rect& a, const Rect& b)
{
    return a.x < b.x + b.w && a.x + a.w > b.x && a.y < b.y + b.h && a.y + a.h > b.y;
}

bool overlap(const Rect& rect, const Circle& circle)
{
    const double nearestX = std::max(rect.x, std::min(circle.pos.x, rect.x + rect.w));
    const double nearestY = std::max(rect.y, std::min(circle.pos.y, rect.y + rect.h));

    const double deltaX = circle.pos.x - nearestX;
    const double deltaY = circle.pos.y - nearestY;

    return (deltaX * deltaX + deltaY * deltaY) <= (circle.radius * circle.radius);
}

bool overlap(const Rect& rect, const Line& line)
{
    // Check if either endpoint is inside the rectangle
    if (overlap(rect, line.getA()) || overlap(rect, line.getB()))
        return true;

    // Check for intersection with rectangle edges using inline calculations
    // instead of creating temporary Line objects
    const double rx = rect.x;
    const double ry = rect.y;
    const double rw = rect.w;
    const double rh = rect.h;

    // Helper lambda to check line-segment intersection
    auto intersects = [&](const double x1, const double y1, const double x2,
                          const double y2) -> bool
    {
        const double denom = (y2 - y1) * (line.bx - line.ax) - (x2 - x1) * (line.by - line.ay);
        if (denom == 0.0)
            return false;

        const double ua = ((x2 - x1) * (line.ay - y1) - (y2 - y1) * (line.ax - x1)) / denom;
        if (ua < 0.0 || ua > 1.0)
            return false;

        const double ub = ((line.bx - line.ax) * (line.ay - y1) -
                           (line.by - line.ay) * (line.ax - x1)) /
                          denom;
        return ub >= 0.0 && ub <= 1.0;
    };

    // Check all four edges
    return intersects(rx, ry, rx + rw, ry) ||            // top
           intersects(rx, ry + rh, rx + rw, ry + rh) ||  // bottom
           intersects(rx, ry, rx, ry + rh) ||            // left
           intersects(rx + rw, ry, rx + rw, ry + rh);    // right
}

bool overlap(const Rect& rect, const Vec2& point)
{
    const auto px = point.x;
    const auto py = point.y;

    return px >= rect.x && px <= rect.x + rect.w && py >= rect.y && py <= rect.y + rect.h;
}

bool overlap(const Circle& a, const Circle& b)
{
    const double distSquared = (a.pos - b.pos).getLengthSquared();
    const double radiusSum = a.radius + b.radius;

    return distSquared <= (radiusSum * radiusSum);
}

bool overlap(const Circle& circle, const Rect& rect)
{
    return overlap(rect, circle);
}

bool overlap(const Circle& circle, const Line& line)
{
    // Calculate vectors once
    const double abx = line.bx - line.ax;
    const double aby = line.by - line.ay;
    const double acx = circle.pos.x - line.ax;
    const double acy = circle.pos.y - line.ay;

    const double abLengthSquared = abx * abx + aby * aby;

    // Degenerate line (point)
    if (abLengthSquared == 0.0)
    {
        const double dx = circle.pos.x - line.ax;
        const double dy = circle.pos.y - line.ay;
        return (dx * dx + dy * dy) <= (circle.radius * circle.radius);
    }

    // Project circle center onto line and clamp to segment
    const double t = std::clamp((acx * abx + acy * aby) / abLengthSquared, 0.0, 1.0);

    // Find the closest point on segment
    const double closestX = line.ax + abx * t;
    const double closestY = line.ay + aby * t;

    // Check distance from circle center to the closest point
    const double dx = circle.pos.x - closestX;
    const double dy = circle.pos.y - closestY;

    return (dx * dx + dy * dy) <= (circle.radius * circle.radius);
}

bool overlap(const Circle& circle, const Vec2& point)
{
    const double distSquared = (circle.pos - point).getLengthSquared();
    return distSquared <= (circle.radius * circle.radius);
}

bool overlap(const Line& a, const Line& b)
{
    const double denom = (b.by - b.ay) * (a.bx - a.ax) - (b.bx - b.ax) * (a.by - a.ay);
    if (denom == 0.0)
        return false;  // Parallel lines

    const double ua = ((b.bx - b.ax) * (a.ay - b.ay) - (b.by - b.ay) * (a.ax - b.ax)) / denom;

    // Early exit if ua is out of range
    if (ua < 0.0 || ua > 1.0)
        return false;

    const double ub = ((a.bx - a.ax) * (a.ay - b.ay) - (a.by - a.ay) * (a.ax - b.ax)) / denom;

    return ub >= 0.0 && ub <= 1.0;
}

bool overlap(const Line& line, const Rect& rect)
{
    return overlap(rect, line);
}

bool overlap(const Line& line, const Circle& circle)
{
    return overlap(circle, line);
}

bool overlap(const Vec2& point, const Rect& rect)
{
    return overlap(rect, point);
}

bool overlap(const Vec2& point, const Circle& circle)
{
    return overlap(circle, point);
}

bool contains(const Rect& outer, const Rect& inner)
{
    return inner.x >= outer.x && inner.y >= outer.y && inner.x + inner.w <= outer.x + outer.w &&
           inner.y + inner.h <= outer.y + outer.h;
}

bool contains(const Rect& rect, const Circle& circle)
{
    const double left = circle.pos.x - circle.radius;
    const double right = circle.pos.x + circle.radius;
    const double top = circle.pos.y - circle.radius;
    const double bottom = circle.pos.y + circle.radius;

    return left >= rect.x && right <= rect.x + rect.w && top >= rect.y && bottom <= rect.y + rect.h;
}

bool contains(const Rect& rect, const Line& line)
{
    const double minX = std::min(line.ax, line.bx);
    const double maxX = std::max(line.ax, line.bx);
    const double minY = std::min(line.ay, line.by);
    const double maxY = std::max(line.ay, line.by);

    return minX >= rect.x && maxX <= rect.x + rect.w && minY >= rect.y && maxY <= rect.y + rect.h;
}

bool contains(const Circle& outer, const Circle& inner)
{
    const double radiusDiff = outer.radius - inner.radius;

    // Outer circle must be larger than inner
    if (radiusDiff < 0.0)
        return false;

    const double distSquared = (outer.pos - inner.pos).getLengthSquared();

    return distSquared <= (radiusDiff * radiusDiff);
}

bool contains(const Circle& circle, const Rect& rect)
{
    // Check if two opposite corners are inside the circle
    // If both diagonal corners are inside, all corners must be inside
    const double radiusSquared = circle.radius * circle.radius;

    // Top-left corner
    double dx = rect.x - circle.pos.x;
    double dy = rect.y - circle.pos.y;
    if (dx * dx + dy * dy > radiusSquared)
        return false;

    // Bottom-right corner
    dx = (rect.x + rect.w) - circle.pos.x;
    dy = (rect.y + rect.h) - circle.pos.y;

    return (dx * dx + dy * dy) <= radiusSquared;
}

bool contains(const Circle& circle, const Line& line)
{
    const double distAX = line.ax - circle.pos.x;
    const double distAY = line.ay - circle.pos.y;
    const double distBX = line.bx - circle.pos.x;
    const double distBY = line.by - circle.pos.y;

    const double radiusSquared = circle.radius * circle.radius;

    return (distAX * distAX + distAY * distAY) <= radiusSquared &&
           (distBX * distBX + distBY * distBY) <= radiusSquared;
}

bool overlap(const Polygon& polygon, const Vec2& point)
{
    if (polygon.points.size() < 3)
        return false;

    bool inside = false;
    size_t count = polygon.points.size();

    for (size_t i = 0, j = count - 1; i < count; j = i++)
    {
        const Vec2& vi = polygon.points[i];
        const Vec2& vj = polygon.points[j];

        bool condition = ((vi.y > point.y) != (vj.y > point.y)) &&
                         (point.x < (vj.x - vi.x) * (point.y - vi.y) / (vj.y - vi.y) + vi.x);
        if (condition)
            inside = !inside;
    }

    return inside;
}

bool overlap(const Vec2& point, const Polygon& polygon)
{
    return overlap(polygon, point);
}

bool overlap(const Polygon& polygon, const Rect& rect)
{
    if (polygon.points.empty())
        return false;

    for (const auto& point : polygon.points)
    {
        if (overlap(rect, point))
            return true;
    }

    Vec2 corners[4] =
        {Vec2{rect.x, rect.y}, Vec2{rect.x + rect.w, rect.y},
         Vec2{rect.x + rect.w, rect.y + rect.h}, Vec2{rect.x, rect.y + rect.h}};

    for (const auto& corner : corners)
    {
        if (overlap(polygon, corner))
            return true;
    }

    return false;
}

bool overlap(const Rect& rect, const Polygon& polygon)
{
    return overlap(polygon, rect);
}

}  // namespace kn::collision
