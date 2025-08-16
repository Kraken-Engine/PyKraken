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
    Mask() = default;
    Mask(const Vec2& size, bool filled = false);
    Mask(const Surface& surface, uint8_t threshold = 1);
    ~Mask() = default;

    Mask copy() const;

    Vec2 getSize() const;

    Rect getRect() const;

    bool getAt(const Vec2& pos) const;

    void setAt(const Vec2& pos, bool value);
    
    int getOverlapArea(const Mask& other, const Vec2& offset) const;

    Mask getOverlapMask(const Mask& other, const Vec2& offset) const;

    void fill();

    void clear();

    void invert();

    void add(const Mask& other, const Vec2& offset);

    void subtract(const Mask& other, const Vec2& offset);

    int getCount() const;

    Vec2 getCenterOfMass() const;

    std::vector<Vec2> getOutline() const;

    Rect getBoundingRect() const;

    bool collideMask(const Mask& other, const Vec2& offset) const;

    std::vector<Vec2> getCollisionPoints(const Mask& other, const Vec2& offset) const;

    std::vector<Vec2> getCollisionPoints(const Mask& other, const Rect& rectA,
                                         const Rect& rectB) const;

    bool isEmpty() const;

    int getWidth() const;

    int getHeight() const;

  private:
    int m_width, m_height;
    std::vector<bool> m_maskData;
};
