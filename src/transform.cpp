#include "Transform.hpp"

#include <box2d/box2d.h>
#include <pybind11/stl.h>

namespace kn
{
Transform::operator b2Transform() const
{
    b2Transform xf;
    xf.p = static_cast<b2Vec2>(pos);
    xf.q = b2MakeRot(static_cast<float>(angle));
    return xf;
}
}  // namespace kn

namespace kn::transform
{
Transform composePair(const Transform& parent, Transform child)
{
    // Child translation is affected by parent scale + rotation
    child.pos *= parent.scale;
    child.pos.rotate(parent.angle);

    child.pos += parent.pos;
    child.angle += parent.angle;
    child.scale *= parent.scale;

    return child;
}

void _bind(py::module_& module)
{
    py::classh<Transform>(module, "Transform", R"doc(
Transform represents a 2D transformation with position, rotation, and scale.

Attributes:
    pos (Vec2): Position component.
    angle (float): Rotation component in radians.
    scale (Vec2): Scale component.
    )doc")
        .def(
            py::init(
                [](const py::object& posObj, double angle, const py::object& scaleObj) -> Transform
                {
                    try
                    {
                        const auto pos = posObj.is_none() ? Vec2{} : posObj.cast<Vec2>();
                        Vec2 scale{1.0};
                        if (!scaleObj.is_none())
                        {
                            const bool isNumeric = py::isinstance<py::int_>(scaleObj) ||
                                                   py::isinstance<py::float_>(scaleObj);
                            scale = isNumeric ? Vec2{scaleObj.cast<double>()}
                                              : scaleObj.cast<Vec2>();
                        }
                        return {pos, angle, scale};
                    }
                    catch (const py::cast_error&)
                    {
                        throw py::type_error(
                            "Invalid type for Transform arguments, expected Vec2 for pos and scale"
                        );
                    }
                }
            ),
            py::arg("pos") = py::none(), py::arg("angle") = 0.0, py::arg("scale") = py::none(),
            R"doc(
Initialize a Transform with optional keyword arguments.

Args:
    pos (Vec2): Position component. Defaults to (0, 0).
    angle (float): Rotation in radians. Defaults to 0.
    scale (Vec2): Scale multiplier. Defaults to (1, 1).
        )doc"
        )
        .def_readwrite("pos", &Transform::pos, R"doc(
The position component as a Vec2.
        )doc")
        .def_readwrite("angle", &Transform::angle, R"doc(
The rotation component in radians.
        )doc")
        .def_readwrite("scale", &Transform::scale, R"doc(
The scale component as a Vec2.
        )doc");

    auto subTransform = module.def_submodule("transform", R"doc(
Submodule for Transform-related functionality.
        )doc");

    subTransform.def(
        "compose",
        [](const py::args& args) -> Transform
        {
            if (args.size() < 2)
            {
                throw py::value_error("compose requires at least two Transform arguments");
            }

            auto result = args[0].cast<Transform>();

            for (size_t i = 1; i < args.size(); ++i)
            {
                const auto& child = args[i].cast<Transform>();
                result = composePair(result, child);
            }

            return result;
        },
        R"doc(
Compose multiple Transform objects in order and return the resulting Transform in world space.
The first transform is treated as already in world space; each subsequent transform is local to the previous.

Args:
    *transforms: Two or more Transform objects to compose.

Returns:
    Transform: The composed Transform in world space.
        )doc"
    );

    subTransform.def(
        "compose_chain",
        [](const py::args& args) -> std::vector<Transform>
        {
            if (args.size() < 2)
            {
                throw py::value_error("compose_chain requires at least two Transform arguments");
            }

            std::vector<Transform> worlds;
            worlds.reserve(args.size());

            auto world = args[0].cast<Transform>();
            for (size_t i = 1; i < args.size(); ++i)
            {
                const auto& local = args[i].cast<Transform>();
                world = composePair(world, local);
                worlds.push_back(world);
            }

            return worlds;
        },
        R"doc(
Returns a list of cumulative world-space transforms excluding the initial input.

Args:
    *transforms: Two or more Transform objects to compose.

Returns:
    list[Transform]: The composed Transforms for inputs 2..N in world space.
        )doc"
    );
}
}  // namespace kn::transform
