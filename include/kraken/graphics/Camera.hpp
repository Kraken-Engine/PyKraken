#pragma once

#include "kraken/math/Math.hpp"
#include "kraken/math/Transform.hpp"

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

Camera* _getActiveCamera();

Vec2 getActivePos();
double getActiveAngle();

[[nodiscard]] Vec2 worldToScreen(const Vec2& worldPos);
[[nodiscard]] Vec2 screenToWorld(const Vec2& screenPos);

}  // namespace camera
}  // namespace kn
