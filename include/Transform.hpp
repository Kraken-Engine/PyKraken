#pragma once

#include <box2d/box2d.h>
#ifdef KRAKEN_ENABLE_PYTHON
#include <nanobind/nanobind.h>
#endif  // KRAKEN_ENABLE_PYTHON

#include <vector>

#include "Math.hpp"
#include "_globals.hpp"

#ifdef KRAKEN_ENABLE_PYTHON
namespace nb = nanobind;
#endif  // KRAKEN_ENABLE_PYTHON

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

#ifdef KRAKEN_ENABLE_PYTHON
void _bind(nb::module_& module);
#endif  // KRAKEN_ENABLE_PYTHON

}  // namespace transform
}  // namespace kn
