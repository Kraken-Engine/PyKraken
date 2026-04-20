#include "Camera.hpp"

#include "Math.hpp"
#include "Renderer.hpp"

namespace kn
{
static Vec2 _cameraPos{};
static double _cameraAngle = 0.0;
Camera* Camera::active = nullptr;

Camera::Camera(const bool setActive)
{
    if (setActive)
        set();
}

void Camera::setWorldPos(const Vec2& worldPos)
{
    this->m_pos = worldPos;
    if (active == this)
        _cameraPos = worldPos;
}

Vec2 Camera::getWorldPos() const
{
    return m_pos;
}

Vec2 Camera::getLocalPos() const
{
    return m_pos.rotated(m_angle);
}

void Camera::setLocalPos(const Vec2& localPos)
{
    setWorldPos(localPos.rotated(-m_angle));
}

double Camera::getAngle() const
{
    return m_angle;
}

void Camera::setAngle(const double angle)
{
    m_angle = angle;
    if (active == this)
        _cameraAngle = angle;
}

void Camera::moveLocal(const Vec2& localDelta)
{
    setWorldPos(m_pos + localDelta.rotated(-m_angle));
}

void Camera::moveWorld(const Vec2& worldDelta)
{
    setWorldPos(m_pos + worldDelta);
}

Vec2 Camera::worldToScreen(const Vec2& worldPos) const
{
    Vec2 screenPos = worldPos - m_pos;
    if (m_angle == 0.0)
        return screenPos;

    const Vec2 center = renderer::getCurrentResolution() * 0.5;
    screenPos -= center;
    screenPos.rotate(m_angle);
    screenPos += center;

    return screenPos;
}

Vec2 Camera::screenToWorld(const Vec2& screenPos) const
{
    Vec2 worldPos = screenPos;
    if (m_angle != 0.0)
    {
        const Vec2 center = renderer::getCurrentResolution() * 0.5;
        worldPos -= center;
        worldPos.rotate(-m_angle);
        worldPos += center;
    }
    worldPos += m_pos;

    return worldPos;
}

void Camera::set()
{
    active = this;
    _cameraPos = m_pos;
    _cameraAngle = m_angle;
}

void Camera::unset()
{
    if (active == this)
    {
        active = nullptr;
        _cameraPos = Vec2{0, 0};
        _cameraAngle = 0.0;
    }
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
    return _cameraPos;
}

double getActiveAngle()
{
    return _cameraAngle;
}

Camera* _getActiveCamera()
{
    return Camera::active;
}

#ifdef KRAKEN_ENABLE_PYTHON
void _bind(nb::module_& module)
{
    using namespace nanobind::literals;

    auto subCamera = module.def_submodule("camera", "Camera management and coordinate conversion");

    subCamera.def("get_active_pos", &camera::getActivePos, R"doc(
Get the position of the currently active camera.
If no camera is active, returns (0, 0).

Returns:
    Vec2: The position of the active camera.
    )doc");

    subCamera.def("get_active_angle", &camera::getActiveAngle, R"doc(
Get the angle of the currently active camera in radians.
If no camera is active, returns 0.

Returns:
    float: The angle of the active camera in radians.
    )doc");

    subCamera.def("world_to_screen", &camera::worldToScreen, "world_pos"_a, R"doc(
Convert a world position to a screen position using the active camera's translation.

Args:
    world_pos (Vec2): The world position to convert.

Returns:
    Vec2: The resulting screen position.
    )doc");

    subCamera.def("screen_to_world", &camera::screenToWorld, "screen_pos"_a, R"doc(
Convert a screen position to a world position using the active camera's translation.

Args:
    screen_pos (Vec2): The screen position to convert.

Returns:
    Vec2: The resulting world position.
    )doc");

    nb::class_<Camera>(module, "Camera", R"doc(
Represents a 2D camera used for rendering.

Controls the viewport's translation, allowing you to move the view of the world.
    )doc")
        .def(nb::init<bool>(), "set_active"_a = false, R"doc(
Create a camera with an optional active state.

Args:
    set_active (bool, optional): Whether to set this camera as active, unsetting any existing active camera. Defaults to false.
        )doc")

        .def_prop_rw("world_pos", &Camera::getWorldPos, &Camera::setWorldPos, R"doc(
Get or set the camera's world position.

Returns:
    Vec2: The camera's current world position.
        )doc")
        .def_prop_rw("local_pos", &Camera::getLocalPos, &Camera::setLocalPos, R"doc(
Get or set the camera position in camera-local space.

Returns:
    Vec2: The camera's current local position.
    )doc")
        .def_prop_rw("angle", &Camera::getAngle, &Camera::setAngle, R"doc(
Get or set the camera angle in radians.
    )doc")

        .def("move_local", &Camera::moveLocal, "delta"_a, R"doc(
Move the camera by a delta in camera-local (screen) space.

Args:
    delta (Vec2): Local movement delta.
    )doc")
        .def("move_world", &Camera::moveWorld, "delta"_a, R"doc(
Move the camera by a delta in world space.

Args:
    delta (Vec2): World movement delta.
    )doc")

        .def("set", &Camera::set, R"doc(
Set this camera as the active one for rendering.

Only one camera can be active at a time.
        )doc")

        .def("unset", &Camera::unset, R"doc(
Unset this camera as the active one for rendering.
        )doc")

        .def("world_to_screen", &Camera::worldToScreen, "world_pos"_a, R"doc(
Convert a world position to a screen position using this camera's translation.

Args:
    world_pos (Vec2): The world position to convert.

Returns:
    Vec2: The resulting screen position.
        )doc")

        .def("screen_to_world", &Camera::screenToWorld, "screen_pos"_a, R"doc(
Convert a screen position to a world position using this camera's translation.

Args:
    screen_pos (Vec2): The screen position to convert.

Returns:
    Vec2: The resulting world position.
        )doc");
}
#endif  // KRAKEN_ENABLE_PYTHON

}  // namespace camera
}  // namespace kn
