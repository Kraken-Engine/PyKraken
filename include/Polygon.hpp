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

    [[nodiscard]] double getUmaPerimeter() const;
    [[nodiscard]] double getUmaArea() const;
    [[nodiscard]] Vec2 getUmaCentroid() const;
    [[nodiscard]] class Rect getUmaBounds() const;
};

namespace polygon
{
void _bind(const py::module_& module);
} // namespace polygon
} // namespace kn
