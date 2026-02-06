#include "physics/World.hpp"

#include <pybind11/native_enum.h>
#include <pybind11/stl.h>

#include "Circle.hpp"
#include "Color.hpp"
#include "Math.hpp"
#include "Polygon.hpp"
#include "Rect.hpp"
#include "physics/Body.hpp"

namespace kn::physics
{
World::World(const Vec2& gravity)
{
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = static_cast<b2Vec2>(gravity);
    m_worldId = b2CreateWorld(&worldDef);
}

World::~World()
{
    if (b2World_IsValid(m_worldId))
        b2DestroyWorld(m_worldId);
}

Body World::createBody(BodyType type)
{
    _checkValid();
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = static_cast<b2BodyType>(type);

    return Body(b2CreateBody(m_worldId, &bodyDef));
}

void World::destroyBody(Body& body)
{
    _checkValid();
    if (b2Body_IsValid(body.m_bodyId))
    {
        b2DestroyBody(body.m_bodyId);
        body.m_bodyId = b2_nullBodyId;
    }
}

void World::step(float timeStep, int subStepCount)
{
    _checkValid();
    b2World_Step(m_worldId, timeStep, subStepCount);
}

void World::setGravity(const Vec2& gravity)
{
    _checkValid();
    b2World_SetGravity(m_worldId, static_cast<b2Vec2>(gravity));
}

Vec2 World::getGravity() const
{
    _checkValid();
    const b2Vec2 gravity = b2World_GetGravity(m_worldId);
    return Vec2(gravity.x, gravity.y);
}

bool World::isValid() const
{
    return b2World_IsValid(m_worldId);
}

void World::_checkValid() const
{
    if (!b2World_IsValid(m_worldId))
        throw std::runtime_error("Attempted to use an invalid or destroyed World");
}

void _bind(py::module_& m)
{
    auto subPhysics = m.def_submodule("physics", "Physics engine related classes and functions");

    py::native_enum<BodyType>(
        subPhysics, "BodyType", "enum.IntEnum",
        R"doc(Body simulation types in the physics engine.)doc"
    )
        .value("STATIC", BodyType::Static, "Does not move and is unaffected by forces.")
        .value("KINEMATIC", BodyType::Kinematic, "Has velocity but is unaffected by forces.")
        .value("DYNAMIC", BodyType::Dynamic, "Has velocity and is affected by forces.")
        .finalize();

    py::classh<Body>(subPhysics, "Body")
        .def(
            "add_collider",
            py::overload_cast<const Circle&, float, float, float>(&Body::addCollider),
            py::arg("circle"), py::arg("density") = 1.0f, py::arg("friction") = 0.2f,
            py::arg("restitution") = 0.0f, R"doc(
Add a circular collider to the body.

Args:
    circle (Circle): The circular shape to add as a collider.
    density (float, optional): The density of the collider. Defaults to 1.0.
    friction (float, optional): The friction coefficient of the collider. Defaults to 0.2.
    restitution (float, optional): The restitution (bounciness) of the collider. Defaults to 0.0.
            )doc"
        )
        .def(
            "add_collider",
            py::overload_cast<const Polygon&, float, float, float>(&Body::addCollider),
            py::arg("polygon"), py::arg("density") = 1.0f, py::arg("friction") = 0.2f,
            py::arg("restitution") = 0.0f, R"doc(
Add a polygonal collider to the body.

Args:
    polygon (Polygon): The polygonal shape to add as a collider.
    density (float, optional): The density of the collider. Defaults to 1.0.
    friction (float, optional): The friction coefficient of the collider. Defaults to 0.2.
    restitution (float, optional): The restitution (bounciness) of the collider. Defaults to 0.0.
            )doc"
        )
        .def(
            "add_collider", py::overload_cast<const Rect&, float, float, float>(&Body::addCollider),
            py::arg("rect"), py::arg("density") = 1.0f, py::arg("friction") = 0.2f,
            py::arg("restitution") = 0.0f, R"doc(
Add a rectangular collider to the body.

Args:
    rect (Rect): The rectangular shape to add as a collider.
    density (float, optional): The density of the collider. Defaults to 1.0.
    friction (float, optional): The friction coefficient of the collider. Defaults to 0.2.
    restitution (float, optional): The restitution (bounciness) of the collider. Defaults to 0.0.
            )doc"
        )

        .def_property(
            "type", &Body::getType, &Body::setType, R"doc(The simulation type of the body.)doc"
        )
        .def_property(
            "pos", &Body::getPos, &Body::setPos,
            R"doc(The position of the body in world coordinates.)doc"
        )
        .def_property(
            "rotation", &Body::getRotation, &Body::setRotation,
            R"doc(The rotation of the body in radians.)doc"
        )
        .def_property(
            "linear_velocity", &Body::getLinearVelocity, &Body::setLinearVelocity,
            R"doc(The linear velocity of the body.)doc"
        )
        .def_property(
            "angular_velocity", &Body::getAngularVelocity, &Body::setAngularVelocity,
            R"doc(The angular velocity of the body.)doc"
        )

        .def("draw", &Body::draw, py::arg("color"), R"doc(
Draw all colliders attached to the body.

Args:
    color (Color): The color to draw the colliders with.
            )doc")
        .def(
            "apply_force", &Body::applyForce, py::arg("force"), py::arg("point"),
            py::arg("wake") = true, R"doc(
Apply a force to the body at a specific point.

Args:
    force (Vec2): The force vector to apply.
    point (Vec2): The point (in world coordinates) where the force is applied.
    wake (bool, optional): Whether to wake the body if it's sleeping. Defaults to True
            )doc"
        )
        .def(
            "apply_force_to_center", &Body::applyForceToCenter, py::arg("force"),
            py::arg("wake") = true, R"doc(
Apply a force to the center of mass of the body.

Args:
    force (Vec2): The force vector to apply.
    wake (bool, optional): Whether to wake the body if it's sleeping. Defaults to True
            )doc"
        )
        .def("apply_torque", &Body::applyTorque, py::arg("torque"), py::arg("wake") = true, R"doc(
Apply a torque to the body.

Args:
    torque (float): The torque to apply.
    wake (bool, optional): Whether to wake the body if it's sleeping. Defaults to True
            )doc")
        .def(
            "apply_linear_impulse", &Body::applyLinearImpulse, py::arg("impulse"), py::arg("point"),
            py::arg("wake") = true, R"doc(
Apply a linear impulse to the body at a specific point.

Args:
    impulse (Vec2): The impulse vector to apply.
    point (Vec2): The point (in world coordinates) where the impulse is applied.
    wake (bool, optional): Whether to wake the body if it's sleeping. Defaults to True
            )doc"
        )
        .def(
            "apply_linear_impulse_to_center", &Body::applyLinearImpulseToCenter, py::arg("impulse"),
            py::arg("wake") = true, R"doc(
Apply a linear impulse to the center of mass of the body.

Args:
    impulse (Vec2): The impulse vector to apply.
    wake (bool, optional): Whether to wake the body if it's sleeping. Defaults to True
            )doc"
        )
        .def(
            "apply_angular_impulse", &Body::applyAngularImpulse, py::arg("impulse"),
            py::arg("wake") = true, R"doc(
Apply an angular impulse to the body.

Args:
    impulse (float): The angular impulse to apply.
    wake (bool, optional): Whether to wake the body if it's sleeping. Defaults to True
            )doc"
        )

        .def_property_readonly("mass", &Body::getMass, R"doc(The mass of the body.)doc")
        .def_property_readonly(
            "is_valid", &Body::isValid, R"doc(Indicates whether the body is not destroyed.)doc"
        );

    py::classh<World>(subPhysics, "World")
        .def(py::init<const Vec2&>(), py::arg("gravity"), R"doc(
Create a new physics world with the specified gravity.

Args:
    gravity (Vec2): The gravity vector for the world.
        )doc")

        .def("create_body", &World::createBody, py::arg("type"), R"doc(
Create a new body in the world.

Args:
    type (BodyType): The simulation type of the body to create.

Returns:
    Body: The created body instance.
        )doc")
        .def("destroy_body", &World::destroyBody, py::arg("body"), R"doc(
Destroy a body in the world.

Args:
    body (Body): The body instance to destroy.
        )doc")
        .def("step", &World::step, py::arg("time_step"), py::arg("sub_step_count"), R"doc(
Advance the physics simulation by a time step.

Args:
    time_step (float): The time step to advance the simulation.
    sub_step_count (int): The number of sub steps to take.
            )doc")

        .def_property(
            "gravity", &World::getGravity, &World::setGravity,
            R"doc(The gravity vector of the world.)doc"
        )
        .def_property_readonly(
            "is_valid", &World::isValid, R"doc(Indicates whether the world is not destroyed.)doc"
        );
}
}  // namespace kn::physics
