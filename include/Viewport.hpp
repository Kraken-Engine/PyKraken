#pragma once

#include <inttypes.h>
#include <pybind11/pybind11.h>

#include <vector>

namespace py = pybind11;

namespace kn
{
class Rect;

struct Viewport
{
  public:
};

enum class ViewportMode
{
    VERTICAL,
    HORIZONTAL,
};

namespace viewport
{
std::vector<Rect> layout(uint8_t count, ViewportMode mode = ViewportMode::VERTICAL);

void set(const Rect& rect);

void unset();

void _bind(py::module_& module);
}  // namespace viewport
}  // namespace kn