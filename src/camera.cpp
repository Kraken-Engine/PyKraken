#include "Camera.hpp"

#include "Math.hpp"
#include "Renderer.hpp"

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
Convert a world position to a screen position using the active camera.

Args:
    world_pos (Vec2): The world position to convert.

Returns:
    Vec2: The resulting screen position.
    )doc");

    subCamera.def("screen_to_world", &camera::screenToWorld, "screen_pos"_a, R"doc(
Convert a screen position to a world position using the active camera.

Args:
    screen_pos (Vec2): The screen position to convert.

Returns:
    Vec2: The resulting world position.
    )doc");

    nb::class_<Camera>(module, "Camera", R"doc(
Represents a 2D camera used for rendering.

The camera position is the world point displayed at the center of the current render target.
    )doc")
        .def(nb::init<bool>(), "set_active"_a = false, R"doc(
Create a camera with an optional active state.

Args:
    set_active (bool, optional): Whether to set this camera as active, unsetting any existing active camera. Defaults to false.
        )doc")

        .def_rw("transform", &Camera::transform, R"doc(
The camera transform. `transform.pos` is the world point at the center of the view.
`transform.angle` rotates the rendered view in radians. `transform.scale` is reserved for future zoom support.
        )doc")

        .def("move_world", &Camera::moveWorld, "delta"_a, R"doc(
Move the camera by a delta in world space.

Args:
    delta (Vec2): World movement delta.
    )doc")
        .def("move_screen", &Camera::moveScreen, "delta"_a, R"doc(
Move the camera by a delta in screen/camera space.

Args:
    delta (Vec2): Screen-space movement delta.
    )doc")
        .def("rotate", &Camera::rotate, "delta"_a, R"doc(
Rotate the camera view by a delta in radians.

Args:
    delta (float): Angle delta in radians.
    )doc")

        .def("set", &Camera::set, R"doc(
Set this camera as the active one for rendering.

Only one camera can be active at a time.
        )doc")

        .def("unset", &Camera::unset, R"doc(
Unset this camera as the active one for rendering.
        )doc")

        .def("world_to_screen", &Camera::worldToScreen, "world_pos"_a, R"doc(
Convert a world position to a screen position using this camera.

Args:
    world_pos (Vec2): The world position to convert.

Returns:
    Vec2: The resulting screen position.
        )doc")

        .def("screen_to_world", &Camera::screenToWorld, "screen_pos"_a, R"doc(
Convert a screen position to a world position using this camera.

Args:
    screen_pos (Vec2): The screen position to convert.

Returns:
    Vec2: The resulting world position.
        )doc");
}
#endif  // KRAKEN_ENABLE_PYTHON

}  // namespace camera
}  // namespace kn
