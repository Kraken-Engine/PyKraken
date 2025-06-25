#include "Camera.hpp"

static math::Vec2 _cameraPos;

namespace camera
{
void _bind(py::module_& module)
{
    py::class_<Camera>(module, "Camera", R"doc(
        Represents a 2D camera used for rendering. Controls viewport translation and scaling.
    )doc")
        .def(py::init(), R"doc(
            Default constructor for Camera.

            Initializes the camera at position (0.0, 0.0).
        )doc")
        .def(py::init([](const double x, const double y) -> Camera* { return new Camera({x, y}); }),
             R"doc(
            Create a Camera at a specific position.

            :param x: X-coordinate of the camera.
            :type x: float
            :param y: Y-coordinate of the camera.
            :type y: float
        )doc",
             py::arg("x"), py::arg("y"))
        .def(py::init(
                 [](const py::object& pos) -> Camera*
                 {
                     if (py::isinstance<math::Vec2>(pos))
                         return new Camera(py::cast<math::Vec2>(pos));

                     if (py::isinstance<py::sequence>(pos))
                     {
                         const auto posSeq = py::cast<py::sequence>(pos);
                         if (posSeq.size() == 2)
                             return new Camera(
                                 {posSeq[0].cast<double>(), posSeq[1].cast<double>()});
                         throw std::invalid_argument(
                             "Position sequence must have exactly two elements.");
                     }

                     throw std::invalid_argument(
                         "Position must be a Vec2 or a sequence of two numbers.");
                 }),
             R"doc(
            Create a Camera using a Vec2 or a sequence of two values.

            :param pos: A ``Vec2`` object or a sequence of two floats representing the camera's position.
            :type pos: Union[Vec2, tuple[float, float]]
        )doc")
        .def_property(
            "pos", &Camera::getPos,
            [](Camera& self, const py::object& setterObj) -> void
            {
                if (py::isinstance<math::Vec2>(setterObj))
                {
                    self.setPos(py::cast<math::Vec2>(setterObj));
                }
                else if (py::isinstance<py::sequence>(setterObj))
                {
                    const auto posSeq = py::cast<py::sequence>(setterObj);
                    if (posSeq.size() == 2)
                        self.setPos({posSeq[0].cast<double>(), posSeq[1].cast<double>()});
                    else
                        throw std::invalid_argument(
                            "Position sequence must have exactly two elements.");
                }
                else
                {
                    throw std::invalid_argument(
                        "Position must be a Vec2 or a sequence of two numbers.");
                }
            },
            R"doc(
            Get or set the camera's position.

            :getter: Get the camera's position as a Vec2.
            :setter: Set the camera's position using a Vec2 or a sequence of two floats.
            :type: Union[Vec2, tuple[float, float]]
        )doc")
        .def("set", &Camera::set, R"doc(
            Set this camera as the active one for rendering.

            Only one camera can be active at a time.
        )doc");
}

math::Vec2 getActivePos() { return _cameraPos; }

Camera* Camera::active = nullptr;

Camera::Camera(const math::Vec2& pos) : pos(pos) {}

void Camera::setPos(const math::Vec2& pos)
{
    this->pos = pos;
    if (Camera::active == this)
        _cameraPos = pos;
}

math::Vec2 Camera::getPos() const { return pos; }

void Camera::set()
{
    Camera::active = this;
    _cameraPos = pos;
}
} // namespace camera
