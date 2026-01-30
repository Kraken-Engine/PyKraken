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
    double angle = 0.0;  // In radians
    Vec2 scale{1.0, 1.0};
};

namespace transform
{
Transform composePair(const Transform& parent, Transform child);

void _bind(py::module_& module);
}  // namespace transform
}  // namespace kn
