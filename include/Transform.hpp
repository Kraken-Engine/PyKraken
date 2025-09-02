#pragma once

#include <memory>
#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace kn
{
class PixelArray;
class Vec2;

namespace transform
{
void _bind(py::module_& module);

std::unique_ptr<PixelArray> flip(const PixelArray& pixelArray, bool flipX, bool flipY);

std::unique_ptr<PixelArray> scaleTo(const PixelArray& pixelArray, const Vec2& size);

std::unique_ptr<PixelArray> scaleBy(const PixelArray& pixelArray, double factor);

std::unique_ptr<PixelArray> scaleBy(const PixelArray& pixelArray, const Vec2& factor);

std::unique_ptr<PixelArray> rotate(const PixelArray& pixelArray, double angle);

std::unique_ptr<PixelArray> boxBlur(const PixelArray& pixelArray, int radius,
                                    bool repeatEdgePixels = true);

std::unique_ptr<PixelArray> gaussianBlur(const PixelArray& pixelArray, int radius,
                                         bool repeatEdgePixels = true);

std::unique_ptr<PixelArray> invert(const PixelArray& pixelArray);

std::unique_ptr<PixelArray> grayscale(const PixelArray& pixelArray);
} // namespace transform
} // namespace kn
