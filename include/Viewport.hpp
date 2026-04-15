#pragma once

#include <inttypes.h>
#ifdef KRAKEN_ENABLE_PYTHON
#include <nanobind/nanobind.h>
#endif  // KRAKEN_ENABLE_PYTHON

#include <vector>

#ifdef KRAKEN_ENABLE_PYTHON
namespace nb = nanobind;
#endif  // KRAKEN_ENABLE_PYTHON

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

#ifdef KRAKEN_ENABLE_PYTHON
void _bind(nb::module_& module);
#endif  // KRAKEN_ENABLE_PYTHON

}  // namespace viewport
}  // namespace kn