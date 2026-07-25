#include "kraken/geometry/Line.hpp"

#include "kraken/math/Math.hpp"

namespace kn
{
Line::Line()
    : ax(0.0),
      ay(0.0),
      bx(0.0),
      by(0.0)
{
}

Line::Line(const double ax, const double ay, const double bx, const double by)
    : ax(ax),
      ay(ay),
      bx(bx),
      by(by)
{
}

Line::Line(const double ax, const double ay, const Vec2& b)
    : ax(ax),
      ay(ay),
      bx(b.x),
      by(b.y)
{
}

Line::Line(const Vec2& a, const double bx, const double by)
    : ax(a.x),
      ay(a.y),
      bx(bx),
      by(by)
{
}

Line::Line(const Vec2& a, const Vec2& b)
    : ax(a.x),
      ay(a.y),
      bx(b.x),
      by(b.y)
{
}

double Line::getLength() const
{
    const double dx = bx - ax;
    const double dy = by - ay;
    return sqrt(dx * dx + dy * dy);
}

Vec2 Line::getA() const
{
    return {ax, ay};
}

void Line::setA(const Vec2& pos)
{
    ax = pos.x;
    ay = pos.y;
}

Vec2 Line::getB() const
{
    return {bx, by};
}

void Line::setB(const Vec2& pos)
{
    bx = pos.x;
    by = pos.y;
}

void Line::move(const Vec2& offset)
{
    ax += offset.x;
    ay += offset.y;
    bx += offset.x;
    by += offset.y;
}

Line Line::moved(const Vec2& offset) const
{
    Line l = *this;
    l.move(offset);
    return l;
}

Vec2 Line::getMidpoint() const
{
    return {(ax + bx) / 2.0, (ay + by) / 2.0};
}

Line Line::getPerpendicular() const
{
    const double dx = bx - ax;
    const double dy = by - ay;
    return {-dy, dx, -dy, dx};
}

double Line::getAngle() const
{
    return atan2(by - ay, bx - ax);
}

Vec2 Line::getClosestPoint(const Vec2& point) const
{
    const double dx = bx - ax;
    const double dy = by - ay;
    const double lengthSquared = dx * dx + dy * dy;

    if (lengthSquared == 0.0)
        return {ax, ay};  // Line is a point

    const double t = ((point.x - ax) * dx + (point.y - ay) * dy) / lengthSquared;
    const double clampedT = std::max(0.0, std::min(1.0, t));

    return {ax + clampedT * dx, ay + clampedT * dy};
}

Line Line::copy() const
{
    return {ax, ay, bx, by};
}

bool Line::operator==(const Line& other) const
{
    return ax == other.ax && ay == other.ay && bx == other.bx && by == other.by;
}

bool Line::operator!=(const Line& other) const
{
    return !(*this == other);
}

}  // namespace kn
