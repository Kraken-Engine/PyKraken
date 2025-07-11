#pragma once

#include <pybind11/pybind11.h>

namespace py = pybind11;

class Surface;
namespace math
{
class Vec2;
} // namespace math

namespace transform
{
void _bind(py::module_& module);

Surface* flip(const Surface& surface, bool flipX, bool flipY);

Surface* scale(const Surface& surface, const math::Vec2& size);

Surface* scaleBy(const Surface& surface, double factor);

Surface* rotate(const Surface& surface, double angle);

Surface* boxBlur(const Surface& surface, int radius, bool repeatEdgePixels = true);

Surface* gaussianBlur(const Surface& surface, int radius, bool repeatEdgePixels = true);

Surface* invert(const Surface& surface);

Surface* grayscale(const Surface& surface);
} // namespace transform
