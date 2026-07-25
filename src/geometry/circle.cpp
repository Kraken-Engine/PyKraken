#include "kraken/geometry/Circle.hpp"

#include "kraken/geometry/Line.hpp"
#include "kraken/geometry/Rect.hpp"

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

namespace kn
{
Circle::Circle(const double radius)
    : radius(radius)
{
}

Circle::Circle(const Vec2& center, const double radius)
    : pos(center),
      radius(radius)
{
}

Circle::Circle(const double x, const double y, const double radius)
    : pos({x, y}),
      radius(radius)
{
}

double Circle::getArea() const
{
    return M_PI * radius * radius;
}

double Circle::getCircumference() const
{
    return 2 * M_PI * radius;
}

double Circle::getDiameter() const
{
    return radius * 2;
}

void Circle::setDiameter(const double diameter)
{
    radius = diameter / 2.0;
}

Rect Circle::asRect() const
{
    Rect rect;
    rect.setSize(Vec2{radius * 2});
    rect.setCenter(pos);
    return rect;
}

double Circle::getLeft() const
{
    return pos.x - radius;
}

double Circle::getRight() const
{
    return pos.x + radius;
}

double Circle::getTop() const
{
    return pos.y - radius;
}

double Circle::getBottom() const
{
    return pos.y + radius;
}

void Circle::setLeft(const double left)
{
    pos.x = left + radius;
}

void Circle::setRight(const double right)
{
    pos.x = right - radius;
}

void Circle::setTop(const double top)
{
    pos.y = top + radius;
}

void Circle::setBottom(const double bottom)
{
    pos.y = bottom - radius;
}

Circle Circle::copy() const
{
    return {pos, radius};
}

bool Circle::operator==(const Circle& other) const
{
    return pos == other.pos && radius == other.radius;
}

bool Circle::operator!=(const Circle& other) const
{
    return !(*this == other);
}

}  // namespace kn
