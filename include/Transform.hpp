#pragma once

#include <pybind11/pybind11.h>

#include <vector>

#include "Math.hpp"
#include "_globals.hpp"

namespace py = pybind11;

namespace kn
{
struct Transform
{
    Vec2 pos{0.0, 0.0};
    Vec2 size{};         // Explicit size (empty = use texture/srcRect size)
    double angle = 0.0;  // In radians
    Vec2 scale{1.0, 1.0};
    Anchor anchor = Anchor::TopLeft;
    Vec2 pivot{0.5, 0.5};  // Normalized pivot point, centered by default
};

namespace transform
{
Transform composePair(const Transform& parent, Transform child);

void _bind(py::module_& module);
}  // namespace transform
}  // namespace kn
