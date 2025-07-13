#pragma once

#include "Math.hpp"

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace camera
{
void _bind(py::module_& module);

Vec2 getActivePos();

class Camera
{
  public:
    Camera() = default;
    explicit Camera(const Vec2& pos = {});
    ~Camera() = default;

    Vec2 getPos() const;
    void setPos(const Vec2& pos);
    void set();

    static Camera* active;

  private:
    Vec2 pos;
};
} // namespace camera
