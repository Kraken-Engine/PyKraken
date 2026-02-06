#pragma once

#include <pybind11/pybind11.h>

#include <vector>

#include "Math.hpp"

namespace py = pybind11;

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
    void translate(const Vec2& offset);
    void scale(double factor, const Vec2& pivot = {0.5, 0.5});
    void scale(const Vec2& factor, const Vec2& pivot = {0.5, 0.5});
};

namespace polygon
{
void _bind(const py::module_& module);
}  // namespace polygon
}  // namespace kn
