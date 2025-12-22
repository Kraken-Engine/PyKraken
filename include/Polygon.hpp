#pragma once

#include <pybind11/pybind11.h>

#include <vector>

namespace py = pybind11;

namespace kn
{
class Vec2;

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
    [[nodiscard]] class Rect getBounds() const;

    void rotate(double angle, const Vec2& pivot);
    void translate(const Vec2& offset);
    void scale(double factor, const Vec2& pivot);
    void scale(const Vec2& factor, const Vec2& pivot);
};

namespace polygon
{
void _bind(const py::module_& module);
}  // namespace polygon
}  // namespace kn
