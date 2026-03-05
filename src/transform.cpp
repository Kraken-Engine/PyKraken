#include "Transform.hpp"

#include <box2d/box2d.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/vector.h>

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

void _bind(nb::module_& module)
{
    using namespace nb::literals;

    nb::class_<Transform>(module, "Transform", R"doc(
Transform represents a 2D transformation with position, rotation, and scale.

Attributes:
    pos (Vec2): Position component.
    angle (float): Rotation component in radians.
    scale (Vec2): Scale component.
    )doc")
        .def(
            "__init__",
            [](Transform* self, std::optional<Vec2> pos, double angle,
               const nb::object& scaleObj) -> void
            {
                Vec2 scale{1.0};
                if (!scaleObj.is_none())
                {
                    const bool isNumeric = nb::isinstance<nb::int_>(scaleObj) ||
                                           nb::isinstance<nb::float_>(scaleObj);
                    if (isNumeric || nb::isinstance<Vec2>(scaleObj))
                        scale = isNumeric ? Vec2{nb::cast<double>(scaleObj)}
                                          : nb::cast<Vec2>(scaleObj);
                    else
                        throw nb::type_error("scale must be a number or Vec2");
                }
                new (self) Transform{pos.value_or(Vec2{}), angle, scale};
            },
            "pos"_a = nb::none(), "angle"_a = 0.0, "scale"_a = nb::none(), R"doc(
Initialize a Transform with optional keyword arguments.

Args:
    pos (Vec2): Position component. Defaults to (0, 0).
    angle (float): Rotation in radians. Defaults to 0.
    scale (Vec2): Scale multiplier. Defaults to (1, 1).
        )doc"
        )
        .def_rw("pos", &Transform::pos, R"doc(
The position component as a Vec2.
        )doc")
        .def_rw("angle", &Transform::angle, R"doc(
The rotation component in radians.
        )doc")
        .def_rw("scale", &Transform::scale, R"doc(
The scale component as a Vec2.
        )doc");

    auto subTransform = module.def_submodule("transform", R"doc(
Submodule for Transform-related functionality.
        )doc");

    subTransform.def(
        "compose",
        [](const nb::args& args) -> Transform
        {
            if (args.size() < 2)
            {
                throw nb::value_error("compose requires at least two Transform arguments");
            }

            auto result = nb::cast<Transform>(args[0]);

            for (size_t i = 1; i < args.size(); ++i)
            {
                const auto& child = nb::cast<Transform>(args[i]);
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
        [](const nb::args& args) -> std::vector<Transform>
        {
            if (args.size() < 2)
            {
                throw nb::value_error("compose_chain requires at least two Transform arguments");
            }

            std::vector<Transform> worlds;
            worlds.reserve(args.size());

            auto world = nb::cast<Transform>(args[0]);
            for (size_t i = 1; i < args.size(); ++i)
            {
                const auto& local = nb::cast<Transform>(args[i]);
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
