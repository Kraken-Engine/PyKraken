#pragma once

#ifdef KRAKEN_ENABLE_PYTHON
#include <nanobind/nanobind.h>
#endif // KRAKEN_ENABLE_PYTHON

#include "Math.hpp"

#ifdef KRAKEN_ENABLE_PYTHON
namespace nb = nanobind;
#endif // KRAKEN_ENABLE_PYTHON

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
    void unset();

    static Camera* active;

  private:
    Vec2 pos;
};

namespace camera
{
#ifdef KRAKEN_ENABLE_PYTHON
void _bind(nb::module_& module);
#endif // KRAKEN_ENABLE_PYTHON

Camera* _getActiveCamera();

Vec2 getActivePos();

[[nodiscard]] Vec2 worldToScreen(const Vec2& worldPos);
[[nodiscard]] Vec2 screenToWorld(const Vec2& screenPos);
}  // namespace camera
}  // namespace kn
