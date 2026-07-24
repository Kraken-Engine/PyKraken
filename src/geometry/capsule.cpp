#include "kraken/geometry/Capsule.hpp"

#include <algorithm>
#include <string>

#include "kraken/geometry/Rect.hpp"

namespace kn
{
Capsule::Capsule(const Vec2& p1, const Vec2& p2, const double radius)
    : p1(p1),
      p2(p2),
      radius(radius)
{
}

Capsule::Capsule(
    const double x1, const double y1, const double x2, const double y2, const double radius
)
    : p1(x1, y1),
      p2(x2, y2),
      radius(radius)
{
}

Rect Capsule::asRect() const
{
    const double minX = std::min(p1.x, p2.x) - radius;
    const double minY = std::min(p1.y, p2.y) - radius;
    const double maxX = std::max(p1.x, p2.x) + radius;
    const double maxY = std::max(p1.y, p2.y) + radius;

    return {minX, minY, maxX - minX, maxY - minY};
}

Capsule Capsule::copy() const
{
    return {p1, p2, radius};
}

bool Capsule::operator==(const Capsule& other) const
{
    return p1 == other.p1 && p2 == other.p2 && radius == other.radius;
}

bool Capsule::operator!=(const Capsule& other) const
{
    return !(*this == other);
}

}  // namespace kn
