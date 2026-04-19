#pragma once

#ifdef KRAKEN_ENABLE_PYTHON
#include <nanobind/nanobind.h>
#endif  // KRAKEN_ENABLE_PYTHON

#include "Math.hpp"

#ifdef KRAKEN_ENABLE_PYTHON
namespace nb = nanobind;
#endif  // KRAKEN_ENABLE_PYTHON

namespace kn
{
class Camera
{
  public:
    Camera(bool setActive = false);
    ~Camera() = default;

    [[nodiscard]] Vec2 getWorldPos() const;
    void setWorldPos(const Vec2& worldPos);
    [[nodiscard]] Vec2 getLocalPos() const;
    void setLocalPos(const Vec2& localPos);

    [[nodiscard]] double getAngle() const;
    void setAngle(double angle);

    void moveLocal(const Vec2& localDelta);
    void moveWorld(const Vec2& worldDelta);

    [[nodiscard]] Vec2 worldToScreen(const Vec2& worldPos) const;
    [[nodiscard]] Vec2 screenToWorld(const Vec2& screenPos) const;

    void set();
    void unset();

    static Camera* active;

  private:
    Vec2 m_pos;
    double m_angle = 0.0;
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
