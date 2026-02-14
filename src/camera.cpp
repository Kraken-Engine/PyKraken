#include "Camera.hpp"

#include "Math.hpp"

namespace kn
{
static Vec2 _cameraPos;
Camera* Camera::active = nullptr;

Camera::Camera(const Vec2& pos)
    : pos(pos)
{
}

Camera::Camera(const double x, const double y)
    : pos(x, y)
{
}

void Camera::setPos(const Vec2& newPos)
{
    this->pos = newPos;
    if (active == this)
        _cameraPos = newPos;
}

Vec2 Camera::getPos() const
{
    return pos;
}

Vec2 Camera::worldToScreen(const Vec2& worldPos) const
{
    return worldPos - Vec2(std::floor(pos.x), std::floor(pos.y));
}

Vec2 Camera::screenToWorld(const Vec2& screenPos) const
{
    return screenPos + Vec2(std::floor(pos.x), std::floor(pos.y));
}

void Camera::set()
{
    active = this;
    _cameraPos = pos;
}

namespace camera
{
Vec2 getActivePos()
{
    return {std::floor(_cameraPos.x), std::floor(_cameraPos.y)};
}

Vec2 worldToScreen(const Vec2& worldPos)
{
    return worldPos - getActivePos();
}

Vec2 screenToWorld(const Vec2& screenPos)
{
    return screenPos + getActivePos();
}

void _bind(py::module_& module)
{
    auto subCamera = module.def_submodule("camera", "Camera management and coordinate conversion");

    subCamera.def("get_active_pos", &camera::getActivePos, R"doc(
Get the position of the currently active camera.
If no camera is active, returns (0, 0).

Returns:
    Vec2: The position of the active camera.
    )doc");

    subCamera.def("world_to_screen", &camera::worldToScreen, py::arg("world_pos"), R"doc(
Convert a world position to a screen position using the active camera's translation.

Args:
    world_pos (Vec2): The world position to convert.

Returns:
    Vec2: The resulting screen position.
    )doc");

    subCamera.def("screen_to_world", &camera::screenToWorld, py::arg("screen_pos"), R"doc(
Convert a screen position to a world position using the active camera's translation.

Args:
    screen_pos (Vec2): The screen position to convert.

Returns:
    Vec2: The resulting world position.
    )doc");

    py::classh<Camera>(module, "Camera", R"doc(
Represents a 2D camera used for rendering.

Controls the viewport's translation, allowing you to move the view of the world.
    )doc")
        .def(py::init(), R"doc(
Create a camera at the default position (0, 0).

Returns:
    Camera: A new camera instance.
        )doc")
        .def(py::init<const Vec2&>(), py::arg("pos"), R"doc(
Create a camera at the given position.

Args:
    pos (Vec2): The camera's initial position.
        )doc")
        .def(py::init<double, double>(), py::arg("x"), py::arg("y"), R"doc(
Create a camera at the given position.

Args:
    x (float): The x-coordinate of the camera's initial position.
    y (float): The y-coordinate of the camera's initial position.
        )doc")

        .def_property("pos", &Camera::getPos, &Camera::setPos, R"doc(
Get or set the camera's position.

Returns:
    Vec2: The camera's current position.

You can also assign a Vec2 or a (x, y) sequence to set the position.
        )doc")

        .def("set", &Camera::set, R"doc(
Set this camera as the active one for rendering.

Only one camera can be active at a time.
        )doc")

        .def("world_to_screen", &Camera::worldToScreen, py::arg("world_pos"), R"doc(
Convert a world position to a screen position using this camera's translation.

Args:
    world_pos (Vec2): The world position to convert.

Returns:
    Vec2: The resulting screen position.
        )doc")

        .def("screen_to_world", &Camera::screenToWorld, py::arg("screen_pos"), R"doc(
Convert a screen position to a world position using this camera's translation.

Args:
    screen_pos (Vec2): The screen position to convert.

Returns:
    Vec2: The resulting world position.
        )doc");
}
}  // namespace camera
}  // namespace kn
