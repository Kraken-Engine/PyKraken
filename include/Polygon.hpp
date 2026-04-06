#pragma once

#include <nanobind/nanobind.h>

#include <vector>

#include "Math.hpp"

namespace nb = nanobind;

namespace kn
{
class Polygon
{
  public:
    std::vector<Vec2> points;

    Polygon() = default;
    explicit Polygon(const std::vector<Vec2>& points);
    ~Polygon() = default;

    [[nodiscard]] Polygon copy() const;

    [[nodiscard]] double getPerimeter() const;

    [[nodiscard]] double getArea() const;

    [[nodiscard]] Vec2 getCentroid() const;

    [[nodiscard]] class Rect getRect() const;

    [[nodiscard]] bool isConvex() const;

    [[nodiscard]] bool isConcave() const;

    void rotate(double angle, const Vec2& pivot = {0.5, 0.5});
    Polygon rotated(double angle, const Vec2& pivot = {0.5, 0.5}) const;

    void move(const Vec2& offset);
    Polygon moved(const Vec2& offset) const;

    void scaleBy(double factor, const Vec2& pivot = {0.5, 0.5});
    void scaleBy(const Vec2& factor, const Vec2& pivot = {0.5, 0.5});
    Polygon scaledBy(double factor, const Vec2& pivot = {0.5, 0.5}) const;
    Polygon scaledBy(const Vec2& factor, const Vec2& pivot = {0.5, 0.5}) const;
};

namespace polygon
{
void _bind(const nb::module_& module);
}  // namespace polygon
}  // namespace kn
