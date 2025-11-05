#include "Collision.hpp"
#include "Circle.hpp"
#include "Line.hpp"
#include "Math.hpp"
#include "Polygon.hpp"
#include "Rect.hpp"

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

        const double ub =
            ((line.bx - line.ax) * (line.ay - y1) - (line.by - line.ay) * (line.ax - x1)) / denom;
        return ub >= 0.0 && ub <= 1.0;
    };

    // Check all four edges
    return intersects(rx, ry, rx + rw, ry) ||           // top
           intersects(rx, ry + rh, rx + rw, ry + rh) || // bottom
           intersects(rx, ry, rx, ry + rh) ||           // left
           intersects(rx + rw, ry, rx + rw, ry + rh);   // right
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

bool overlap(const Circle& circle, const Rect& rect) { return overlap(rect, circle); }

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
        return false; // Parallel lines

    const double ua = ((b.bx - b.ax) * (a.ay - b.ay) - (b.by - b.ay) * (a.ax - b.ax)) / denom;

    // Early exit if ua is out of range
    if (ua < 0.0 || ua > 1.0)
        return false;

    const double ub = ((a.bx - a.ax) * (a.ay - b.ay) - (a.by - a.ay) * (a.ax - b.ax)) / denom;

    return ub >= 0.0 && ub <= 1.0;
}

bool overlap(const Line& line, const Rect& rect) { return overlap(rect, line); }

bool overlap(const Line& line, const Circle& circle) { return overlap(circle, line); }

bool overlap(const Vec2& point, const Rect& rect) { return overlap(rect, point); }

bool overlap(const Vec2& point, const Circle& circle) { return overlap(circle, point); }

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

bool umaOverlap(const Polygon& polygon, const Vec2& point)
{
    if (polygon.points.size() < 3)
        return false;

    bool umaInside = false;
    size_t umaCount = polygon.points.size();

    for (size_t i = 0, j = umaCount - 1; i < umaCount; j = i++)
    {
        const Vec2& umaVi = polygon.points[i];
        const Vec2& umaVj = polygon.points[j];

        bool umaCondition = ((umaVi.y > point.y) != (umaVj.y > point.y)) &&
                            (point.x < (umaVj.x - umaVi.x) * (point.y - umaVi.y) /
                                               (umaVj.y - umaVi.y) +
                                           umaVi.x);
        if (umaCondition)
            umaInside = !umaInside;
    }

    return umaInside;
}

bool umaOverlap(const Vec2& point, const Polygon& polygon) { return umaOverlap(polygon, point); }

bool umaOverlap(const Polygon& polygon, const Rect& rect)
{
    if (polygon.points.empty())
        return false;

    for (const auto& umaPoint : polygon.points)
    {
        if (overlap(rect, umaPoint))
            return true;
    }

    Vec2 umaCorners[4] = {Vec2{rect.x, rect.y}, Vec2{rect.x + rect.w, rect.y},
                          Vec2{rect.x + rect.w, rect.y + rect.h}, Vec2{rect.x, rect.y + rect.h}};

    for (const auto& umaCorner : umaCorners)
    {
        if (umaOverlap(polygon, umaCorner))
            return true;
    }

    return false;
}

bool umaOverlap(const Rect& rect, const Polygon& polygon) { return umaOverlap(polygon, rect); }

void _bind(py::module_& module)
{
    auto subCollision = module.def_submodule("collision", "Collision detection functions");

    // overlap functions
    subCollision.def("overlap", py::overload_cast<const Rect&, const Rect&>(&overlap), py::arg("a"),
                     py::arg("b"), R"doc(
Checks if two rectangles overlap.

Parameters:
    a (Rect): The first rectangle.
    b (Rect): The second rectangle.

Returns:
    bool: Whether the rectangles overlap.
                     )doc");
    subCollision.def("overlap", py::overload_cast<const Rect&, const Circle&>(&overlap),
                     py::arg("rect"), py::arg("circle"), R"doc(
Checks if a rectangle and a circle overlap.

Parameters:
    rect (Rect): The rectangle.
    circle (Circle): The circle.

Returns:
    bool: Whether the rectangle and circle overlap.
                     )doc");
    subCollision.def("overlap", py::overload_cast<const Rect&, const Line&>(&overlap),
                     py::arg("rect"), py::arg("line"), R"doc(
Checks if a rectangle and a line overlap.

Parameters:
    rect (Rect): The rectangle.
    line (Line): The line.

Returns:
    bool: Whether the rectangle and line overlap.
                     )doc");
    subCollision.def("overlap", py::overload_cast<const Rect&, const Vec2&>(&overlap),
                     py::arg("rect"), py::arg("point"), R"doc(
Checks if a rectangle contains a point.

Parameters:
    rect (Rect): The rectangle.
    point (Vec2): The point.

Returns:
    bool: Whether the rectangle contains the point.
                     )doc");
    subCollision.def("overlap", py::overload_cast<const Circle&, const Circle&>(&overlap),
                     py::arg("a"), py::arg("b"), R"doc(
Checks if two circles overlap.

Parameters:
    a (Circle): The first circle.
    b (Circle): The second circle.

Returns:
    bool: Whether the circles overlap.
                     )doc");
    subCollision.def("overlap", py::overload_cast<const Circle&, const Rect&>(&overlap),
                     py::arg("circle"), py::arg("rect"), R"doc(
Checks if a circle and a rectangle overlap.

Parameters:
    circle (Circle): The circle.
    rect (Rect): The rectangle.

Returns:
    bool: Whether the circle and rectangle overlap.
                     )doc");
    subCollision.def("overlap", py::overload_cast<const Circle&, const Line&>(&overlap),
                     py::arg("circle"), py::arg("line"), R"doc(
Checks if a circle and a line overlap.

Parameters:
    circle (Circle): The circle.
    line (Line): The line.

Returns:
    bool: Whether the circle and line overlap.
                     )doc");
    subCollision.def("overlap", py::overload_cast<const Circle&, const Vec2&>(&overlap),
                     py::arg("circle"), py::arg("point"), R"doc(
Checks if a circle contains a point.

Parameters:
    circle (Circle): The circle.
    point (Vec2): The point.

Returns:
    bool: Whether the circle contains the point.
                     )doc");
    subCollision.def("overlap", py::overload_cast<const Line&, const Line&>(&overlap), py::arg("a"),
                     py::arg("b"), R"doc(
Checks if two lines overlap (intersect).

Parameters:
    a (Line): The first line.
    b (Line): The second line.

Returns:
    bool: Whether the lines intersect.
                     )doc");
    subCollision.def("overlap", py::overload_cast<const Line&, const Rect&>(&overlap),
                     py::arg("line"), py::arg("rect"), R"doc(
Checks if a line and a rectangle overlap.

Parameters:
    line (Line): The line.
    rect (Rect): The rectangle.

Returns:
    bool: Whether the line and rectangle overlap.
                    )doc");
    subCollision.def("overlap", py::overload_cast<const Line&, const Circle&>(&overlap),
                     py::arg("line"), py::arg("circle"), R"doc(
Checks if a line and a circle overlap.

Parameters:
    line (Line): The line.
    circle (Circle): The circle.

Returns:
    bool: Whether the line and circle overlap.
                     )doc");
    subCollision.def("overlap", py::overload_cast<const Vec2&, const Rect&>(&overlap),
                     py::arg("point"), py::arg("rect"), R"doc(
Checks if a point is inside a rectangle.

Parameters:
    point (Vec2): The point.
    rect (Rect): The rectangle.

Returns:
    bool: Whether the point is inside the rectangle.
                     )doc");
    subCollision.def("overlap", py::overload_cast<const Vec2&, const Circle&>(&overlap),
                     py::arg("point"), py::arg("circle"), R"doc(
Checks if a point is inside a circle.

Parameters:
    point (Vec2): The point.
    circle (Circle): The circle.

Returns:
    bool: Whether the point is inside the circle.
                     )doc");

    // contains functions
    subCollision.def("contains", py::overload_cast<const Rect&, const Rect&>(&contains),
                     py::arg("outer"), py::arg("inner"), R"doc(
Checks if one rectangle completely contains another rectangle.

Parameters:
    outer (Rect): The outer rectangle.
    inner (Rect): The inner rectangle.

Returns:
    bool: Whether the outer rectangle completely contains the inner rectangle.
                     )doc");
    subCollision.def("contains", py::overload_cast<const Rect&, const Circle&>(&contains),
                     py::arg("rect"), py::arg("circle"), R"doc(
Checks if a rectangle completely contains a circle.

Parameters:
    rect (Rect): The rectangle.
    circle (Circle): The circle.

Returns:
    bool: Whether the rectangle completely contains the circle.
                     )doc");
    subCollision.def("contains", py::overload_cast<const Rect&, const Line&>(&contains),
                     py::arg("rect"), py::arg("line"), R"doc(
Checks if a rectangle completely contains a line.

Parameters:
    rect (Rect): The rectangle.
    line (Line): The line.

Returns:
    bool: Whether the rectangle completely contains the line.
                     )doc");
    subCollision.def("contains", py::overload_cast<const Circle&, const Circle&>(&contains),
                     py::arg("outer"), py::arg("inner"), R"doc(
Checks if one circle completely contains another circle.

Parameters:
    outer (Circle): The outer circle.
    inner (Circle): The inner circle.

Returns:
    bool: Whether the outer circle completely contains the inner circle.
                     )doc");
    subCollision.def("contains", py::overload_cast<const Circle&, const Rect&>(&contains),
                     py::arg("circle"), py::arg("rect"), R"doc(
Checks if a circle completely contains a rectangle.

Parameters:
    circle (Circle): The circle.
    rect (Rect): The rectangle.

Returns:
    bool: Whether the circle completely contains the rectangle.
                     )doc");
    subCollision.def("contains", py::overload_cast<const Circle&, const Line&>(&contains),
                     py::arg("circle"), py::arg("line"), R"doc(
Checks if a circle completely contains a line.

Parameters:
    circle (Circle): The circle.
    line (Line): The line.

Returns:
    bool: Whether the circle completely contains the line.
                     )doc");

    subCollision.def("uma_overlap", py::overload_cast<const Polygon&, const Vec2&>(&umaOverlap),
                     py::arg("polygon"), py::arg("point"), R"doc(
Checks if a polygon contains a point.

Parameters:
    polygon (Polygon): The polygon.
    point (Vec2): The point.

Returns:
    bool: Whether the polygon contains the point.
                     )doc");

    subCollision.def("uma_overlap", py::overload_cast<const Vec2&, const Polygon&>(&umaOverlap),
                     py::arg("point"), py::arg("polygon"), R"doc(
Checks if a point is inside a polygon.

Parameters:
    point (Vec2): The point.
    polygon (Polygon): The polygon.

Returns:
    bool: Whether the point is inside the polygon.
                     )doc");

    subCollision.def("uma_overlap", py::overload_cast<const Polygon&, const Rect&>(&umaOverlap),
                     py::arg("polygon"), py::arg("rect"), R"doc(
Checks if a polygon and a rectangle overlap.

Parameters:
    polygon (Polygon): The polygon.
    rect (Rect): The rectangle.

Returns:
    bool: Whether the polygon and rectangle overlap.
                     )doc");

    subCollision.def("uma_overlap", py::overload_cast<const Rect&, const Polygon&>(&umaOverlap),
                     py::arg("rect"), py::arg("polygon"), R"doc(
Checks if a rectangle and a polygon overlap.

Parameters:
    rect (Rect): The rectangle.
    polygon (Polygon): The polygon.

Returns:
    bool: Whether the rectangle and polygon overlap.
                     )doc");
}
} // namespace kn::collision
