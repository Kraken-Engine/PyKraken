#pragma once

#include <pybind11/pybind11.h>
#include <vector>

class Surface;
class Vec2;
class Rect;

namespace py = pybind11;

namespace mask
{
void _bind(py::module_& module);
}

class Mask
{
  public:
    explicit Mask(const Surface& surface, uint8_t threshold = 1);
    ~Mask() = default;

    bool collideMask(const Mask& other, const Vec2& offset) const;

    bool collideMask(const Mask& other, const Rect& rectA, const Rect& rectB) const;

    bool getPixel(const Vec2& pos) const;

  private:
    int m_width, m_height;
    std::vector<bool> m_maskData;
};
