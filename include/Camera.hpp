#pragma once

#ifdef KRAKEN_ENABLE_PYTHON
#include <nanobind/nanobind.h>
#endif  // KRAKEN_ENABLE_PYTHON

#include "Math.hpp"
#include "Transform.hpp"

#ifdef KRAKEN_ENABLE_PYTHON
namespace nb = nanobind;
#endif  // KRAKEN_ENABLE_PYTHON

namespace kn
{
class Camera
{
  public:
    Transform transform{};

    Camera(bool setActive = false);
    ~Camera() = default;

    void moveWorld(const Vec2& worldDelta);
    void moveScreen(const Vec2& screenDelta);
    void rotate(double delta);

    [[nodiscard]] Vec2 worldToScreen(const Vec2& worldPos) const;
    [[nodiscard]] Vec2 screenToWorld(const Vec2& screenPos) const;

    void set();
    void unset();

    static Camera* active;
};

namespace camera
{

#ifdef KRAKEN_ENABLE_PYTHON
void _bind(nb::module_& module);
#endif  // KRAKEN_ENABLE_PYTHON

Camera* _getActiveCamera();

Vec2 getActivePos();
double getActiveAngle();

[[nodiscard]] Vec2 worldToScreen(const Vec2& worldPos);
[[nodiscard]] Vec2 screenToWorld(const Vec2& screenPos);

}  // namespace camera
}  // namespace kn
