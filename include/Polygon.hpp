#pragma once

#ifdef KRAKEN_ENABLE_PYTHON
#include <nanobind/nanobind.h>
#endif  // KRAKEN_ENABLE_PYTHON

#include <vector>

#include "Math.hpp"

#ifdef KRAKEN_ENABLE_PYTHON
namespace nb = nanobind;
#endif  // KRAKEN_ENABLE_PYTHON

namespace kn
{
class Polygon
{
  public:
    std::vector<Vec2> points;

    Polygon() = default;
    explicit Polygon(const std::vector<Vec2>& points);
    Polygon(uint32_t n, double radius, const Vec2& centroid = Vec2::ZERO);
    ~Polygon() = default;

    [[nodiscard]] Polygon copy() const;

    [[nodiscard]] double getPerimeter() const;
    [[nodiscard]] double getArea() const;

    [[nodiscard]] Vec2 getCentroid() const;
    void setCentroid(const Vec2& centroid);

    [[nodiscard]] class Rect getRect() const;

    [[nodiscard]] bool isConvex() const;
    [[nodiscard]] bool isConcave() const;

    void rotate(double angle);
    Polygon rotated(double angle) const;

    void move(const Vec2& offset);
    Polygon moved(const Vec2& offset) const;

    void scaleBy(double factor);
    void scaleBy(const Vec2& factor);
    Polygon scaledBy(double factor) const;
    Polygon scaledBy(const Vec2& factor) const;
};

#ifdef KRAKEN_ENABLE_PYTHON
namespace polygon
{
void _bind(const nb::module_& module);
}  // namespace polygon
#endif  // KRAKEN_ENABLE_PYTHON

}  // namespace kn
