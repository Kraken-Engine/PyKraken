#pragma once

#include "Math.hpp"
#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace kn
{
class Camera
{
  public:
    Camera() = default;
    explicit Camera(const Vec2& pos);
    Camera(double x, double y);
    ~Camera() = default;

    [[nodiscard]] Vec2 getPos() const;
    void setPos(const Vec2& newPos);
    void set();

    static Camera* active;

  private:
    Vec2 pos;
};

namespace camera
{
void _bind(const py::module_& module);

Vec2 getActivePos();
} // namespace camera
} // namespace kn
