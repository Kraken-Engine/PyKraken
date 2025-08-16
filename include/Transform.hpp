#pragma once

#include <memory>
#include <pybind11/pybind11.h>

namespace py = pybind11;

class Surface;
class Vec2;

namespace transform
{
void _bind(py::module_& module);

std::unique_ptr<Surface> flip(const Surface& surface, bool flipX, bool flipY);

std::unique_ptr<Surface> scaleTo(const Surface& surface, const Vec2& size);

std::unique_ptr<Surface> scaleBy(const Surface& surface, double factor);

std::unique_ptr<Surface> scaleBy(const Surface& surface, const Vec2& factor);

std::unique_ptr<Surface> rotate(const Surface& surface, double angle);

std::unique_ptr<Surface> boxBlur(const Surface& surface, int radius, bool repeatEdgePixels = true);

std::unique_ptr<Surface> gaussianBlur(const Surface& surface, int radius,
                                      bool repeatEdgePixels = true);

std::unique_ptr<Surface> invert(const Surface& surface);

std::unique_ptr<Surface> grayscale(const Surface& surface);
} // namespace transform
