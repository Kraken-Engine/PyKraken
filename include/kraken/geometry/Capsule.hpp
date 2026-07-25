#pragma once

#include "kraken/math/Math.hpp"

namespace kn
{
class Rect;

class Capsule
{
  public:
    Vec2 p1;
    Vec2 p2;
    double radius = 0.0;

    Capsule() = default;
    Capsule(const Vec2& p1, const Vec2& p2, double radius);
    Capsule(double x1, double y1, double x2, double y2, double radius);

    ~Capsule() = default;

    [[nodiscard]] Rect asRect() const;

    [[nodiscard]] Capsule copy() const;

    bool operator==(const Capsule& other) const;
    bool operator!=(const Capsule& other) const;
};

}  // namespace kn
