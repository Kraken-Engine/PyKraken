#pragma once

#include <inttypes.h>
#include <nanobind/nanobind.h>

#include <vector>

namespace nb = nanobind;

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

void _bind(nb::module_& module);
}  // namespace viewport
}  // namespace kn