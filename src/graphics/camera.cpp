#include "kraken/graphics/Camera.hpp"

#include "kraken/graphics/Renderer.hpp"
#include "kraken/math/Math.hpp"

namespace kn
{
Camera* Camera::active = nullptr;

Camera::Camera(const bool setActive)
{
    if (setActive)
        set();
}

void Camera::moveWorld(const Vec2& worldDelta)
{
    transform.pos += worldDelta;
}

void Camera::moveScreen(const Vec2& screenDelta)
{
    transform.pos += screenDelta.rotated(-transform.angle);
}

void Camera::rotate(const double delta)
{
    transform.angle += delta;
}

Vec2 Camera::worldToScreen(const Vec2& worldPos) const
{
    const Vec2 center = renderer::getCurrentResolution() * 0.5;
    Vec2 screenPos = worldPos - transform.pos;
    if (transform.angle != 0.0)
        screenPos.rotate(transform.angle);
    screenPos += center;

    return screenPos;
}

Vec2 Camera::screenToWorld(const Vec2& screenPos) const
{
    const Vec2 center = renderer::getCurrentResolution() * 0.5;
    Vec2 worldPos = screenPos - center;
    if (transform.angle != 0.0)
        worldPos.rotate(-transform.angle);
    worldPos += transform.pos;

    return worldPos;
}

void Camera::set()
{
    active = this;
}

void Camera::unset()
{
    if (active == this)
        active = nullptr;
}

namespace camera
{
Vec2 worldToScreen(const Vec2& worldPos)
{
    if (Camera::active)
        return Camera::active->worldToScreen(worldPos);

    return worldPos;
}

Vec2 screenToWorld(const Vec2& screenPos)
{
    if (Camera::active)
        return Camera::active->screenToWorld(screenPos);

    return screenPos;
}

Vec2 getActivePos()
{
    if (Camera::active)
        return Camera::active->transform.pos;

    return {};
}

double getActiveAngle()
{
    if (Camera::active)
        return Camera::active->transform.angle;

    return 0.0;
}

Camera* _getActiveCamera()
{
    return Camera::active;
}

}  // namespace camera
}  // namespace kn
