#pragma once

#ifdef KRAKEN_ENABLE_PYTHON
#include <nanobind/nanobind.h>
#endif  // KRAKEN_ENABLE_PYTHON

#include <vector>

#include "Color.hpp"
#include "Math.hpp"

#ifdef KRAKEN_ENABLE_PYTHON
namespace nb = nanobind;
#endif  // KRAKEN_ENABLE_PYTHON

namespace kn
{
class PixelArray;
class Rect;

class Mask
{
  public:
    Mask() = default;
    explicit Mask(const Vec2& size, bool filled = false);
    explicit Mask(const PixelArray& pixelArray, uint8_t threshold = 1);
    ~Mask() = default;

    [[nodiscard]] Mask copy() const;

    [[nodiscard]] Vec2 getSize() const;

    [[nodiscard]] Rect getRect() const;

    [[nodiscard]] bool getAt(int x, int y) const;

    void setAt(int x, int y, bool value);

    [[nodiscard]] int getOverlapArea(const Mask& other, const Vec2& offset = {}) const;

    [[nodiscard]] Mask getOverlapMask(const Mask& other, const Vec2& offset = {}) const;

    void fill();

    void clear();

    void invert();

    void add(const Mask& other, const Vec2& offset = {});

    void subtract(const Mask& other, const Vec2& offset = {});

    [[nodiscard]] int getCount() const;

    [[nodiscard]] Vec2 getCenterOfMass() const;

    [[nodiscard]] std::vector<Vec2> getOutline() const;

    [[nodiscard]] Rect getBoundingRect() const;

    [[nodiscard]] bool collideMask(const Mask& other, const Vec2& offset = {}) const;

    [[nodiscard]] std::vector<Vec2> getCollisionPoints(
        const Mask& other, const Vec2& offset = {}
    ) const;

    [[nodiscard]] bool isEmpty() const;

    [[nodiscard]] int getWidth() const;

    [[nodiscard]] int getHeight() const;

    [[nodiscard]] PixelArray getPixelArray(const Color& color = {255, 255, 255, 255}) const;

  private:
    int m_width = 0, m_height = 0;
    std::vector<bool> m_maskData;
};

#ifdef KRAKEN_ENABLE_PYTHON
namespace mask
{
void _bind(const nb::module_& module);
}  // namespace mask
#endif  // KRAKEN_ENABLE_PYTHON
}  // namespace kn
