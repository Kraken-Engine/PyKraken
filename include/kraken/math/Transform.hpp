#pragma once

#include <box2d/box2d.h>

#include <vector>

#include "kraken/core/_globals.hpp"
#include "kraken/math/Math.hpp"

namespace kn
{
struct Transform
{
    Vec2 pos{};
    double angle{};  // In radians
    Vec2 scale{1.0};

    explicit operator b2Transform() const;
};

namespace transform
{
Transform composePair(const Transform& parent, Transform child);

}  // namespace transform
}  // namespace kn
