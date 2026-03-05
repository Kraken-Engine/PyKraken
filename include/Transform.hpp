#pragma once

#include <box2d/box2d.h>
#include <nanobind/nanobind.h>

#include <vector>

#include "Math.hpp"
#include "_globals.hpp"

namespace nb = nanobind;

namespace kn
{
struct Transform
{
    Vec2 pos{0.0, 0.0};
    double angle = 0.0;  // In radians
    Vec2 scale{1.0, 1.0};

    explicit operator b2Transform() const;
};

namespace transform
{
Transform composePair(const Transform& parent, Transform child);

void _bind(nb::module_& module);
}  // namespace transform
}  // namespace kn
