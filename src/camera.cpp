#include "Camera.hpp"

static Vec2 _cameraPos;

namespace camera
{
void _bind(py::module_& module)
{
    py::class_<Camera>(module, "Camera", R"doc(
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
        )doc");
}

Vec2 getActivePos() { return _cameraPos; }
} // namespace camera

Camera* Camera::active = nullptr;

Camera::Camera(const Vec2& pos) : pos(pos) {}

Camera::Camera(double x, double y) : pos(x, y) {}

void Camera::setPos(const Vec2& pos)
{
    this->pos = pos;
    if (Camera::active == this)
        _cameraPos = pos;
}

Vec2 Camera::getPos() const { return pos; }

void Camera::set()
{
    Camera::active = this;
    _cameraPos = pos;
}
