#include "Transform.hpp"

#include <pybind11/stl_bind.h>

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
Transform represents a 2D transformation with position, size, rotation, and scale.

Attributes:
    pos (Vec2): Position component.
    size (Vec2): Explicit size (empty = use texture/srcRect size).
    angle (float): Rotation component in radians.
    scale (Vec2): Scale component.
    anchor (Anchor): Anchor point for positioning.
    pivot (Vec2): Normalized pivot point for rotation.
    )doc")
        .def(
            py::init(
                [](const py::object& posObj, const py::object& sizeObj, double angle,
                   const py::object& scaleObj, Anchor anchor,
                   const py::object& pivotObj) -> Transform
                {
                    try
                    {
                        const auto pos = posObj.is_none() ? Vec2{} : posObj.cast<Vec2>();
                        const auto size = sizeObj.is_none() ? Vec2{} : sizeObj.cast<Vec2>();
                        Vec2 scale{1.0};
                        if (!scaleObj.is_none())
                        {
                            const bool isNumeric = py::isinstance<py::int_>(scaleObj) ||
                                                   py::isinstance<py::float_>(scaleObj);
                            scale = isNumeric ? Vec2{scaleObj.cast<double>()}
                                              : scaleObj.cast<Vec2>();
                        }
                        const auto pivot = pivotObj.is_none() ? Vec2{0.5} : pivotObj.cast<Vec2>();
                        return {pos, size, angle, scale, anchor, pivot};
                    }
                    catch (const py::cast_error&)
                    {
                        throw py::type_error(
                            "Invalid type for Transform arguments, expected Vec2 for pos, size, "
                            "scale, and pivot"
                        );
                    }
                }
            ),
            py::arg("pos") = py::none(), py::arg("size") = py::none(), py::arg("angle") = 0.0,
            py::arg("scale") = py::none(), py::arg("anchor") = Anchor::TopLeft,
            py::arg("pivot") = py::none(),
            R"doc(
Initialize a Transform with optional keyword arguments.

Args:
    pos (Vec2): Position component. Defaults to (0, 0).
    size (Vec2): Explicit size. Defaults to empty (auto-detect).
    angle (float): Rotation in radians. Defaults to 0.
    scale (Vec2): Scale multiplier. Defaults to (1, 1).
    anchor (Anchor): Anchor point for positioning. Defaults to TOP_LEFT.
    pivot (Vec2): Normalized rotation pivot. Defaults to (0.5, 0.5) for center.
        )doc"
        )
        .def_readwrite("pos", &Transform::pos, R"doc(
The position component as a Vec2.
        )doc")
        .def_readwrite("size", &Transform::size, R"doc(
The explicit size as a Vec2. If zero/empty, uses texture or source rect size.
        )doc")
        .def_readwrite("angle", &Transform::angle, R"doc(
The rotation component in radians.
        )doc")
        .def_readwrite("scale", &Transform::scale, R"doc(
The scale component as a Vec2.
        )doc")
        .def_readwrite("anchor", &Transform::anchor, R"doc(
The anchor point for positioning.
        )doc")
        .def_readwrite("pivot", &Transform::pivot, R"doc(
The normalized pivot point for rotation.
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

    py::bind_vector<std::vector<Transform>>(subTransform, "TransformList");
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
    TransformList: The composed Transforms for inputs 2..N in world space.
        )doc"
    );
}
}  // namespace kn::transform
