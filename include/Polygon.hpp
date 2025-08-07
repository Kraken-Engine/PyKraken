#pragma once

#include <pybind11/pybind11.h>
#include <vector>

namespace py = pybind11;

namespace polygon
{
void _bind(py::module_& module);
} // namespace polygon

class Vec2;

class Polygon
{
  public:
    Polygon(const std::vector<Vec2>& points);
    ~Polygon() = default;
};
