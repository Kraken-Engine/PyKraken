#pragma once

#include <pybind11/pybind11.h>

#include "Math.hpp"

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

    [[nodiscard]] Vec2 worldToScreen(const Vec2& worldPos) const;
    [[nodiscard]] Vec2 screenToWorld(const Vec2& screenPos) const;

    void set();

    static Camera* active;

  private:
    Vec2 pos;
};

namespace camera
{
void _bind(py::module_& module);

Vec2 getActivePos();

[[nodiscard]] Vec2 worldToScreen(const Vec2& worldPos);
[[nodiscard]] Vec2 screenToWorld(const Vec2& screenPos);
}  // namespace camera
}  // namespace kn
