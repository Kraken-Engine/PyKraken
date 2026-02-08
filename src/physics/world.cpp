#include "physics/World.hpp"

#include <pybind11/native_enum.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

#include "Circle.hpp"
#include "Color.hpp"
#include "Math.hpp"
#include "Polygon.hpp"
#include "Rect.hpp"
#include "physics/Body.hpp"
#include "physics/DistanceJoint.hpp"
#include "physics/FilterJoint.hpp"
#include "physics/Joint.hpp"
#include "physics/MotorJoint.hpp"
#include "physics/PrismaticJoint.hpp"
#include "physics/RevoluteJoint.hpp"
#include "physics/TargetJoint.hpp"
#include "physics/WeldJoint.hpp"
#include "physics/WheelJoint.hpp"

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

DistanceJoint World::createDistanceJoint(
    const Body& bodyA, const Body& bodyB, const Vec2& anchorA, const Vec2& anchorB
)
{
    _checkValid();
    _checkBodiesForJoint(bodyA, bodyB);

    const auto pA = static_cast<b2Vec2>(anchorA);
    const auto pB = static_cast<b2Vec2>(anchorB);

    b2DistanceJointDef jointDef = b2DefaultDistanceJointDef();
    jointDef.bodyIdA = bodyA.m_bodyId;
    jointDef.bodyIdB = bodyB.m_bodyId;
    jointDef.localAnchorA = b2Body_GetLocalPoint(bodyA.m_bodyId, pA);
    jointDef.localAnchorB = b2Body_GetLocalPoint(bodyB.m_bodyId, pB);
    jointDef.length = b2Length(b2Sub(pB, pA));

    return DistanceJoint(b2CreateDistanceJoint(m_worldId, &jointDef));
}

FilterJoint World::createFilterJoint(const Body& bodyA, const Body& bodyB)
{
    _checkValid();
    _checkBodiesForJoint(bodyA, bodyB);

    b2FilterJointDef jointDef = b2DefaultFilterJointDef();
    jointDef.bodyIdA = bodyA.m_bodyId;
    jointDef.bodyIdB = bodyB.m_bodyId;

    return FilterJoint(b2CreateFilterJoint(m_worldId, &jointDef));
}

MotorJoint World::createMotorJoint(const Body& bodyA, const Body& bodyB)
{
    _checkValid();
    _checkBodiesForJoint(bodyA, bodyB);

    b2MotorJointDef jointDef = b2DefaultMotorJointDef();
    jointDef.bodyIdA = bodyA.m_bodyId;
    jointDef.bodyIdB = bodyB.m_bodyId;

    return MotorJoint(b2CreateMotorJoint(m_worldId, &jointDef));
}

TargetJoint World::createTargetJoint(
    const Body& groundBody, const Body& pulledBody, const Vec2& target
)
{
    _checkValid();
    _checkBodiesForJoint(groundBody, pulledBody);

    b2MouseJointDef jointDef = b2DefaultMouseJointDef();
    jointDef.bodyIdA = groundBody.m_bodyId;
    jointDef.bodyIdB = pulledBody.m_bodyId;
    jointDef.target = static_cast<b2Vec2>(target);

    return TargetJoint(b2CreateMouseJoint(m_worldId, &jointDef));
}

PrismaticJoint World::createPrismaticJoint(
    const Body& bodyA, const Body& bodyB, const Vec2& anchor, const Vec2& axis
)
{
    _checkValid();
    _checkBodiesForJoint(bodyA, bodyB);

    const auto b2anchor = static_cast<b2Vec2>(anchor);
    auto b2axis = static_cast<b2Vec2>(axis);
    b2axis = b2Normalize(b2axis);
    if (b2axis.x == 0.0f && b2axis.y == 0.0f)
        throw std::invalid_argument("Axis vector cannot be zero.");

    b2PrismaticJointDef jointDef = b2DefaultPrismaticJointDef();
    jointDef.bodyIdA = bodyA.m_bodyId;
    jointDef.bodyIdB = bodyB.m_bodyId;
    jointDef.localAnchorA = b2Body_GetLocalPoint(bodyA.m_bodyId, b2anchor);
    jointDef.localAnchorB = b2Body_GetLocalPoint(bodyB.m_bodyId, b2anchor);
    jointDef.localAxisA = b2Body_GetLocalVector(bodyA.m_bodyId, b2axis);
    jointDef.referenceAngle = b2Rot_GetAngle(b2Body_GetRotation(bodyB.m_bodyId)) -
                              b2Rot_GetAngle(b2Body_GetRotation(bodyA.m_bodyId));

    return PrismaticJoint(b2CreatePrismaticJoint(m_worldId, &jointDef));
}

RevoluteJoint World::createRevoluteJoint(const Body& bodyA, const Body& bodyB, const Vec2& anchor)
{
    _checkValid();
    _checkBodiesForJoint(bodyA, bodyB);

    b2RevoluteJointDef jointDef = b2DefaultRevoluteJointDef();
    jointDef.bodyIdA = bodyA.m_bodyId;
    jointDef.bodyIdB = bodyB.m_bodyId;
    jointDef.localAnchorA = b2Body_GetLocalPoint(bodyA.m_bodyId, static_cast<b2Vec2>(anchor));
    jointDef.localAnchorB = b2Body_GetLocalPoint(bodyB.m_bodyId, static_cast<b2Vec2>(anchor));
    jointDef.referenceAngle = b2Rot_GetAngle(b2Body_GetRotation(bodyB.m_bodyId)) -
                              b2Rot_GetAngle(b2Body_GetRotation(bodyA.m_bodyId));

    return RevoluteJoint(b2CreateRevoluteJoint(m_worldId, &jointDef));
}

WeldJoint World::createWeldJoint(const Body& bodyA, const Body& bodyB, const Vec2& anchor)
{
    _checkValid();
    _checkBodiesForJoint(bodyA, bodyB);

    b2WeldJointDef jointDef = b2DefaultWeldJointDef();
    jointDef.bodyIdA = bodyA.m_bodyId;
    jointDef.bodyIdB = bodyB.m_bodyId;
    jointDef.localAnchorA = b2Body_GetLocalPoint(bodyA.m_bodyId, static_cast<b2Vec2>(anchor));
    jointDef.localAnchorB = b2Body_GetLocalPoint(bodyB.m_bodyId, static_cast<b2Vec2>(anchor));
    jointDef.referenceAngle = b2Rot_GetAngle(b2Body_GetRotation(bodyB.m_bodyId)) -
                              b2Rot_GetAngle(b2Body_GetRotation(bodyA.m_bodyId));

    return WeldJoint(b2CreateWeldJoint(m_worldId, &jointDef));
}

WheelJoint World::createWheelJoint(
    const Body& bodyA, const Body& bodyB, const Vec2& anchor, const Vec2& axis
)
{
    _checkValid();
    _checkBodiesForJoint(bodyA, bodyB);

    const auto b2anchor = static_cast<b2Vec2>(anchor);
    auto b2axis = static_cast<b2Vec2>(axis);
    b2axis = b2Normalize(b2axis);
    if (b2axis.x == 0.0f && b2axis.y == 0.0f)
        throw std::invalid_argument("Axis vector cannot be zero.");

    b2WheelJointDef jointDef = b2DefaultWheelJointDef();
    jointDef.bodyIdA = bodyA.m_bodyId;
    jointDef.bodyIdB = bodyB.m_bodyId;
    jointDef.localAnchorA = b2Body_GetLocalPoint(bodyA.m_bodyId, b2anchor);
    jointDef.localAnchorB = b2Body_GetLocalPoint(bodyB.m_bodyId, b2anchor);
    jointDef.localAxisA = b2Body_GetLocalVector(bodyA.m_bodyId, b2axis);

    return WheelJoint(b2CreateWheelJoint(m_worldId, &jointDef));
}

void World::step(float timeStep, int subStepCount)
{
    _checkValid();
    b2World_Step(m_worldId, timeStep, subStepCount);
}

std::vector<Collision> World::getCollisions()
{
    _checkValid();
    b2ContactEvents events = b2World_GetContactEvents(m_worldId);

    std::vector<Collision> collisions;
    collisions.reserve(events.hitCount);
    for (int i = 0; i < events.hitCount; ++i)
    {
        const b2ContactHitEvent* event = events.hitEvents + i;
        Collision c;
        c.bodyA = Body(b2Shape_GetBody(event->shapeIdA));
        c.bodyB = Body(b2Shape_GetBody(event->shapeIdB));
        c.point = {event->point.x, event->point.y};
        c.normal = {event->normal.x, event->normal.y};
        c.approachSpeed = event->approachSpeed;
        collisions.push_back(c);
    }

    return collisions;
}

bool World::QueryCallback(b2ShapeId shapeId, void* context)
{
    auto* ctx = static_cast<QueryContext*>(context);
    if (ctx->isPointQuery)
    {
        if (!b2Shape_TestPoint(shapeId, static_cast<b2Vec2>(ctx->point)))
            return true;
    }

    ctx->bodies.push_back(Body(b2Shape_GetBody(shapeId)));
    return true;
}

std::vector<Body> World::queryPoint(const Vec2& point)
{
    _checkValid();
    b2AABB aabb;
    constexpr float d = 0.001f;
    aabb.lowerBound = {static_cast<float>(point.x) - d, static_cast<float>(point.y) - d};
    aabb.upperBound = {static_cast<float>(point.x) + d, static_cast<float>(point.y) + d};

    QueryContext ctx;
    ctx.point = point;
    ctx.isPointQuery = true;
    b2World_OverlapAABB(m_worldId, aabb, b2DefaultQueryFilter(), QueryCallback, &ctx);

    return ctx.bodies;
}

std::vector<Body> World::queryAABB(const Rect& rect)
{
    _checkValid();
    b2AABB aabb;
    aabb.lowerBound = static_cast<b2Vec2>(rect.getTopLeft());
    aabb.upperBound = static_cast<b2Vec2>(rect.getBottomRight());

    QueryContext ctx;
    ctx.isPointQuery = false;
    b2World_OverlapAABB(m_worldId, aabb, b2DefaultQueryFilter(), QueryCallback, &ctx);

    return ctx.bodies;
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
    return {gravity.x, gravity.y};
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

void World::_checkBodiesForJoint(const Body& bodyA, const Body& bodyB) const
{
    if (!bodyA.isValid() || !bodyB.isValid())
        throw std::invalid_argument("Both bodies must be valid to create a joint.");

    const b2WorldId worldA = b2Body_GetWorld(bodyA.m_bodyId);
    const b2WorldId worldB = b2Body_GetWorld(bodyB.m_bodyId);
    if (worldA.index1 != m_worldId.index1 || worldB.index1 != m_worldId.index1)
        throw std::invalid_argument("Both bodies must belong to this World to create a joint.");
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
            py::overload_cast<const Circle&, float, float, float, bool, bool>(&Body::addCollider),
            py::arg("circle"), py::arg("density") = 1.0f, py::arg("friction") = 0.2f,
            py::arg("restitution") = 0.0f, py::arg("enable_events") = false,
            py::arg("is_sensor") = false, R"doc(
Add a circular collider to the body.

Args:
    circle (Circle): The circular shape to add as a collider.
    density (float, optional): The density of the collider. Defaults to 1.0.
    friction (float, optional): The friction coefficient of the collider. Defaults to 0.2.
    restitution (float, optional): The restitution (bounciness) of the collider. Defaults to 0.0.
    enable_events (bool, optional): Whether to enable hit events for this collider. Defaults to False.
    is_sensor (bool, optional): Whether the collider is a sensor. Defaults to False.
            )doc"
        )
        .def(
            "add_collider",
            py::overload_cast<const Polygon&, float, float, float, bool, bool>(&Body::addCollider),
            py::arg("polygon"), py::arg("density") = 1.0f, py::arg("friction") = 0.2f,
            py::arg("restitution") = 0.0f, py::arg("enable_events") = false,
            py::arg("is_sensor") = false, R"doc(
Add a polygonal collider to the body.

Args:
    polygon (Polygon): The polygonal shape to add as a collider.
    density (float, optional): The density of the collider. Defaults to 1.0.
    friction (float, optional): The friction coefficient of the collider. Defaults to 0.2.
    restitution (float, optional): The restitution (bounciness) of the collider. Defaults to 0.0.
    enable_events (bool, optional): Whether to enable hit events for this collider. Defaults to False.
    is_sensor (bool, optional): Whether the collider is a sensor. Defaults to False.
            )doc"
        )
        .def(
            "add_collider",
            py::overload_cast<const Rect&, float, float, float, bool, bool>(&Body::addCollider),
            py::arg("rect"), py::arg("density") = 1.0f, py::arg("friction") = 0.2f,
            py::arg("restitution") = 0.0f, py::arg("enable_events") = false,
            py::arg("is_sensor") = false, R"doc(
Add a rectangular collider to the body.

Args:
    rect (Rect): The rectangular shape to add as a collider.
    density (float, optional): The density of the collider. Defaults to 1.0.
    friction (float, optional): The friction coefficient of the collider. Defaults to 0.2.
    restitution (float, optional): The restitution (bounciness) of the collider. Defaults to 0.0.
    enable_events (bool, optional): Whether to enable hit events for this collider. Defaults to False.
    is_sensor (bool, optional): Whether the collider is a sensor. Defaults to False.
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
        .def_property(
            "linear_damping", &Body::getLinearDamping, &Body::setLinearDamping,
            R"doc(The linear damping of the body.)doc"
        )
        .def_property(
            "angular_damping", &Body::getAngularDamping, &Body::setAngularDamping,
            R"doc(The angular damping of the body.)doc"
        )
        .def_property(
            "fixed_rotation", &Body::isFixedRotation, &Body::setFixedRotation,
            R"doc(Whether the body has fixed rotation.)doc"
        )
        .def_property_readonly(
            "awake", &Body::isAwake,
            R"doc(Whether the body is currently awake and participating in simulation.)doc"
        )
        .def_property_readonly("mass", &Body::getMass, R"doc(The mass of the body.)doc")
        .def_property_readonly(
            "is_valid", &Body::isValid, R"doc(Indicates whether the body is not destroyed.)doc"
        )

        .def("draw", &Body::draw, py::arg("color"), R"doc(
Draw all colliders attached to the body.

Args:
    color (Color): The color to draw the colliders with.
            )doc")
        .def("wake", &Body::wake, R"doc(
Manually wake the body from sleep.
            )doc")
        .def(
            "apply_force", &Body::applyForce, py::arg("force"), py::arg("point"),
            py::arg("wake") = true, R"doc(
Apply a force to the body at a specific point.

Args:
    force (Vec2): The force vector to apply.
    point (Vec2): The point (in world coordinates) where the force is applied.
    wake (bool, optional): Whether to wake the body if it's sleeping. Defaults to True.
            )doc"
        )
        .def(
            "apply_force_to_center", &Body::applyForceToCenter, py::arg("force"),
            py::arg("wake") = true, R"doc(
Apply a force to the center of mass of the body.

Args:
    force (Vec2): The force vector to apply.
    wake (bool, optional): Whether to wake the body if it's sleeping. Defaults to True.
            )doc"
        )
        .def("apply_torque", &Body::applyTorque, py::arg("torque"), py::arg("wake") = true, R"doc(
Apply a torque to the body.

Args:
    torque (float): The torque to apply.
    wake (bool, optional): Whether to wake the body if it's sleeping. Defaults to True.
            )doc")
        .def(
            "apply_linear_impulse", &Body::applyLinearImpulse, py::arg("impulse"), py::arg("point"),
            py::arg("wake") = true, R"doc(
Apply a linear impulse to the body at a specific point.

Args:
    impulse (Vec2): The impulse vector to apply.
    point (Vec2): The point (in world coordinates) where the impulse is applied.
    wake (bool, optional): Whether to wake the body if it's sleeping. Defaults to True.
            )doc"
        )
        .def(
            "apply_linear_impulse_to_center", &Body::applyLinearImpulseToCenter, py::arg("impulse"),
            py::arg("wake") = true, R"doc(
Apply a linear impulse to the center of mass of the body.

Args:
    impulse (Vec2): The impulse vector to apply.
    wake (bool, optional): Whether to wake the body if it's sleeping. Defaults to True.
            )doc"
        )
        .def(
            "apply_angular_impulse", &Body::applyAngularImpulse, py::arg("impulse"),
            py::arg("wake") = true, R"doc(
Apply an angular impulse to the body.

Args:
    impulse (float): The angular impulse to apply.
    wake (bool, optional): Whether to wake the body if it's sleeping. Defaults to True.
            )doc"
        )
        .def("destroy", &Body::destroy, R"doc(Destroy the body manually.)doc")
        .def(py::self == py::self)
        .def(py::self != py::self);

    py::classh<Joint>(subPhysics, "Joint")
        .def_property(
            "collide_connected", &Joint::getCollideConnected, &Joint::setCollideConnected,
            R"doc(Whether the connected bodies should collide with each other.)doc"
        )
        .def_property(
            "local_anchor_a", &Joint::getLocalAnchorA, &Joint::setLocalAnchorA,
            R"doc(The local anchor point relative to body A's origin.)doc"
        )
        .def_property(
            "local_anchor_b", &Joint::getLocalAnchorB, &Joint::setLocalAnchorB,
            R"doc(The local anchor point relative to body B's origin.)doc"
        )
        .def_property_readonly(
            "body_a", &Joint::getBodyA, R"doc(The first body attached to the joint.)doc"
        )
        .def_property_readonly(
            "body_b", &Joint::getBodyB, R"doc(The second body attached to the joint.)doc"
        )
        .def_property_readonly(
            "is_valid", &Joint::isValid, R"doc(Indicates whether the joint is not destroyed.)doc"
        )
        .def("destroy", &Joint::destroy, R"doc(Destroy the joint manually.)doc");

    py::classh<DistanceJoint, Joint>(subPhysics, "DistanceJoint")
        .def_property(
            "length", &DistanceJoint::getLength, &DistanceJoint::setLength,
            R"doc(The rest length of the joint.)doc"
        )
        .def_property(
            "spring_enabled", &DistanceJoint::isSpringEnabled, &DistanceJoint::enableSpring,
            R"doc(Whether the spring is enabled.)doc"
        )
        .def_property(
            "spring_hz", &DistanceJoint::getSpringHertz, &DistanceJoint::setSpringHertz,
            R"doc(The spring frequency in Hertz.)doc"
        )
        .def_property(
            "spring_damping_ratio", &DistanceJoint::getSpringDampingRatio,
            &DistanceJoint::setSpringDampingRatio, R"doc(The spring damping ratio.)doc"
        )
        .def_property(
            "limit_enabled", &DistanceJoint::isLimitEnabled, &DistanceJoint::enableLimit,
            R"doc(Whether the length limits are enabled.)doc"
        )
        .def_property(
            "motor_enabled", &DistanceJoint::isMotorEnabled, &DistanceJoint::enableMotor,
            R"doc(Whether the motor is enabled.)doc"
        )
        .def_property(
            "motor_speed", &DistanceJoint::getMotorSpeed, &DistanceJoint::setMotorSpeed,
            R"doc(The target motor speed.)doc"
        )
        .def_property(
            "max_motor_force", &DistanceJoint::getMaxMotorForce, &DistanceJoint::setMaxMotorForce,
            R"doc(The maximum motor force.)doc"
        )
        .def_property_readonly(
            "min_length", &DistanceJoint::getMinLength, R"doc(The minimum length limit.)doc"
        )
        .def_property_readonly(
            "max_length", &DistanceJoint::getMaxLength, R"doc(The maximum length limit.)doc"
        )
        .def_property_readonly(
            "current_length", &DistanceJoint::getCurrentLength,
            R"doc(The current length between the anchors.)doc"
        )
        .def_property_readonly(
            "motor_force", &DistanceJoint::getMotorForce, R"doc(The current motor force.)doc"
        )
        .def(
            "set_length_range", &DistanceJoint::setLengthRange, py::arg("min_length"),
            py::arg("max_length"), R"doc(
Set the minimum and maximum length limits.

Args:
    min_length (float): The minimum length.
    max_length (float): The maximum length.
            )doc"
        );

    py::classh<FilterJoint, Joint>(
        subPhysics, "FilterJoint", R"doc(A joint used to filter collisions between two bodies.)doc"
    );

    py::classh<MotorJoint, Joint>(subPhysics, "MotorJoint")
        .def_property(
            "linear_offset", &MotorJoint::getLinearOffset, &MotorJoint::setLinearOffset,
            R"doc(The target linear offset from body A to body B.)doc"
        )
        .def_property(
            "angular_offset", &MotorJoint::getAngularOffset, &MotorJoint::setAngularOffset,
            R"doc(The target angular offset in radians.)doc"
        )
        .def_property(
            "max_force", &MotorJoint::getMaxForce, &MotorJoint::setMaxForce,
            R"doc(The maximum motor force.)doc"
        )
        .def_property(
            "max_torque", &MotorJoint::getMaxTorque, &MotorJoint::setMaxTorque,
            R"doc(The maximum motor torque.)doc"
        )
        .def_property(
            "correction_factor", &MotorJoint::getCorrectionFactor, &MotorJoint::setCorrectionFactor,
            R"doc(The position correction factor in [0, 1].)doc"
        );

    py::classh<TargetJoint, Joint>(subPhysics, "TargetJoint")
        .def_property(
            "target", &TargetJoint::getTarget, &TargetJoint::setTarget,
            R"doc(The target point in world coordinates.)doc"
        )
        .def_property(
            "spring_hz", &TargetJoint::getSpringHertz, &TargetJoint::setSpringHertz,
            R"doc(The spring frequency in Hertz.)doc"
        )
        .def_property(
            "spring_damping_ratio", &TargetJoint::getSpringDampingRatio,
            &TargetJoint::setSpringDampingRatio, R"doc(The spring damping ratio.)doc"
        )
        .def_property(
            "max_force", &TargetJoint::getMaxForce, &TargetJoint::setMaxForce,
            R"doc(The maximum constraint force.)doc"
        );

    py::classh<PrismaticJoint, Joint>(subPhysics, "PrismaticJoint")
        .def_property(
            "spring_enabled", &PrismaticJoint::isSpringEnabled, &PrismaticJoint::enableSpring,
            R"doc(Whether the spring is enabled.)doc"
        )
        .def_property(
            "spring_hz", &PrismaticJoint::getSpringHertz, &PrismaticJoint::setSpringHertz,
            R"doc(The spring frequency in Hertz.)doc"
        )
        .def_property(
            "spring_damping_ratio", &PrismaticJoint::getSpringDampingRatio,
            &PrismaticJoint::setSpringDampingRatio, R"doc(The spring damping ratio.)doc"
        )
        .def_property(
            "target_translation", &PrismaticJoint::getTargetTranslation,
            &PrismaticJoint::setTargetTranslation, R"doc(The target translation for the motor.)doc"
        )
        .def_property(
            "limit_enabled", &PrismaticJoint::isLimitEnabled, &PrismaticJoint::enableLimit,
            R"doc(Whether the translation limits are enabled.)doc"
        )
        .def_property(
            "motor_enabled", &PrismaticJoint::isMotorEnabled, &PrismaticJoint::enableMotor,
            R"doc(Whether the motor is enabled.)doc"
        )
        .def_property(
            "motor_speed", &PrismaticJoint::getMotorSpeed, &PrismaticJoint::setMotorSpeed,
            R"doc(The target motor speed.)doc"
        )
        .def_property(
            "max_motor_force", &PrismaticJoint::getMaxMotorForce, &PrismaticJoint::setMaxMotorForce,
            R"doc(The maximum motor force.)doc"
        )
        .def_property_readonly(
            "lower_limit", &PrismaticJoint::getLowerLimit, R"doc(The lower translation limit.)doc"
        )
        .def_property_readonly(
            "upper_limit", &PrismaticJoint::getUpperLimit, R"doc(The upper translation limit.)doc"
        )
        .def_property_readonly(
            "motor_force", &PrismaticJoint::getMotorForce, R"doc(The current motor force.)doc"
        )
        .def_property_readonly(
            "translation", &PrismaticJoint::getTranslation,
            R"doc(The current joint translation.)doc"
        )
        .def_property_readonly(
            "speed", &PrismaticJoint::getSpeed, R"doc(The current joint translation speed.)doc"
        )
        .def("set_limits", &PrismaticJoint::setLimits, py::arg("lower"), py::arg("upper"), R"doc(
Set the translation limits.

Args:
    lower (float): The lower translation limit.
    upper (float): The upper translation limit.
            )doc");

    py::classh<RevoluteJoint, Joint>(subPhysics, "RevoluteJoint")
        .def_property(
            "spring_enabled", &RevoluteJoint::isSpringEnabled, &RevoluteJoint::enableSpring,
            R"doc(Whether the spring is enabled.)doc"
        )
        .def_property(
            "spring_hz", &RevoluteJoint::getSpringHertz, &RevoluteJoint::setSpringHertz,
            R"doc(The spring frequency in Hertz.)doc"
        )
        .def_property(
            "spring_damping_ratio", &RevoluteJoint::getSpringDampingRatio,
            &RevoluteJoint::setSpringDampingRatio, R"doc(The spring damping ratio.)doc"
        )
        .def_property(
            "target_angle", &RevoluteJoint::getTargetAngle, &RevoluteJoint::setTargetAngle,
            R"doc(The target angle for the motor in radians.)doc"
        )
        .def_property(
            "limit_enabled", &RevoluteJoint::isLimitEnabled, &RevoluteJoint::enableLimit,
            R"doc(Whether the angle limits are enabled.)doc"
        )
        .def_property(
            "motor_enabled", &RevoluteJoint::isMotorEnabled, &RevoluteJoint::enableMotor,
            R"doc(Whether the motor is enabled.)doc"
        )
        .def_property(
            "motor_speed", &RevoluteJoint::getMotorSpeed, &RevoluteJoint::setMotorSpeed,
            R"doc(The target motor speed in radians per second.)doc"
        )
        .def_property(
            "max_motor_torque", &RevoluteJoint::getMaxMotorTorque,
            &RevoluteJoint::setMaxMotorTorque, R"doc(The maximum motor torque.)doc"
        )
        .def_property_readonly(
            "lower_limit", &RevoluteJoint::getLowerLimit,
            R"doc(The lower angle limit in radians.)doc"
        )
        .def_property_readonly(
            "upper_limit", &RevoluteJoint::getUpperLimit,
            R"doc(The upper angle limit in radians.)doc"
        )
        .def_property_readonly(
            "angle", &RevoluteJoint::getAngle, R"doc(The current joint angle in radians.)doc"
        )
        .def_property_readonly(
            "motor_torque", &RevoluteJoint::getMotorTorque, R"doc(The current motor torque.)doc"
        )
        .def("set_limits", &RevoluteJoint::setLimits, py::arg("lower"), py::arg("upper"), R"doc(
Set the angle limits.

Args:
    lower (float): The lower angle limit in radians.
    upper (float): The upper angle limit in radians.
            )doc");

    py::classh<WeldJoint, Joint>(subPhysics, "WeldJoint")
        .def_property(
            "linear_hz", &WeldJoint::getLinearHertz, &WeldJoint::setLinearHertz,
            R"doc(The linear spring frequency in Hertz.)doc"
        )
        .def_property(
            "linear_damping_ratio", &WeldJoint::getLinearDampingRatio,
            &WeldJoint::setLinearDampingRatio, R"doc(The linear spring damping ratio.)doc"
        )
        .def_property(
            "angular_hz", &WeldJoint::getAngularHertz, &WeldJoint::setAngularHertz,
            R"doc(The angular spring frequency in Hertz.)doc"
        )
        .def_property(
            "angular_damping_ratio", &WeldJoint::getAngularDampingRatio,
            &WeldJoint::setAngularDampingRatio, R"doc(The angular spring damping ratio.)doc"
        );

    py::classh<WheelJoint, Joint>(subPhysics, "WheelJoint")
        .def_property(
            "spring_enabled", &WheelJoint::isSpringEnabled, &WheelJoint::enableSpring,
            R"doc(Whether the spring is enabled.)doc"
        )
        .def_property(
            "spring_hz", &WheelJoint::getSpringHertz, &WheelJoint::setSpringHertz,
            R"doc(The spring frequency in Hertz.)doc"
        )
        .def_property(
            "spring_damping_ratio", &WheelJoint::getSpringDampingRatio,
            &WheelJoint::setSpringDampingRatio, R"doc(The spring damping ratio.)doc"
        )
        .def_property(
            "limit_enabled", &WheelJoint::isLimitEnabled, &WheelJoint::enableLimit,
            R"doc(Whether the translation limits are enabled.)doc"
        )
        .def_property(
            "motor_enabled", &WheelJoint::isMotorEnabled, &WheelJoint::enableMotor,
            R"doc(Whether the motor is enabled.)doc"
        )
        .def_property(
            "motor_speed", &WheelJoint::getMotorSpeed, &WheelJoint::setMotorSpeed,
            R"doc(The target motor speed in radians per second.)doc"
        )
        .def_property(
            "max_motor_torque", &WheelJoint::getMaxMotorTorque, &WheelJoint::setMaxMotorTorque,
            R"doc(The maximum motor torque.)doc"
        )
        .def_property_readonly(
            "lower_limit", &WheelJoint::getLowerLimit, R"doc(The lower translation limit.)doc"
        )
        .def_property_readonly(
            "upper_limit", &WheelJoint::getUpperLimit, R"doc(The upper translation limit.)doc"
        )
        .def_property_readonly(
            "motor_torque", &WheelJoint::getMotorTorque, R"doc(The current motor torque.)doc"
        )
        .def("set_limits", &WheelJoint::setLimits, py::arg("lower"), py::arg("upper"), R"doc(
Set the translation limits.

Args:
    lower (float): The lower translation limit.
    upper (float): The upper translation limit.
            )doc");

    py::classh<Collision>(subPhysics, "Collision")
        .def_readonly("body_a", &Collision::bodyA, "The first body involved in the collision.")
        .def_readonly("body_b", &Collision::bodyB, "The second body involved in the collision.")
        .def_readonly("point", &Collision::point, "The point of impact in world coordinates.")
        .def_readonly("normal", &Collision::normal, "The normal vector of the collision.")
        .def_readonly(
            "approach_speed", &Collision::approachSpeed,
            "The speed at which the bodies approached each other."
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

        .def(
            "create_distance_joint", &World::createDistanceJoint, py::arg("body_a"),
            py::arg("body_b"), py::arg("anchor_a"), py::arg("anchor_b"), R"doc(
Create a distance joint between two bodies.

Args:
    body_a (Body): The first body.
    body_b (Body): The second body.
    anchor_a (Vec2): The anchor point on the first body in world coordinates.
    anchor_b (Vec2): The anchor point on the second body in world coordinates.

Returns:
    DistanceJoint: The created joint.
        )doc"
        )
        .def(
            "create_filter_joint", &World::createFilterJoint, py::arg("body_a"), py::arg("body_b"),
            R"doc(
Create a filter joint between two bodies to disable collision.

Args:
    body_a (Body): The first body.
    body_b (Body): The second body.

Returns:
    FilterJoint: The created joint.
        )doc"
        )
        .def(
            "create_motor_joint", &World::createMotorJoint, py::arg("body_a"), py::arg("body_b"),
            R"doc(
Create a motor joint between two bodies.

Args:
    body_a (Body): The first body.
    body_b (Body): The second body.

Returns:
    MotorJoint: The created joint.
        )doc"
        )
        .def(
            "create_target_joint", &World::createTargetJoint, py::arg("ground_body"),
            py::arg("pulled_body"), py::arg("target"), R"doc(
Create a target joint between a ground body and a target body.

Args:
    ground_body (Body): The ground body (usually a static body).
    pulled_body (Body): The body to be pulled and moved to the target.
    target (Vec2): The initial target point in world coordinates.

Returns:
    TargetJoint: The created joint.
        )doc"
        )
        .def(
            "create_prismatic_joint", &World::createPrismaticJoint, py::arg("body_a"),
            py::arg("body_b"), py::arg("anchor"), py::arg("axis"), R"doc(
Create a prismatic joint between two bodies.

Args:
    body_a (Body): The first body.
    body_b (Body): The second body.
    anchor (Vec2): The anchor point in world coordinates.
    axis (Vec2): The axis of movement in world coordinates.

Returns:
    PrismaticJoint: The created joint.
        )doc"
        )
        .def(
            "create_revolute_joint", &World::createRevoluteJoint, py::arg("body_a"),
            py::arg("body_b"), py::arg("anchor"), R"doc(
Create a revolute joint between two bodies.

Args:
    body_a (Body): The first body.
    body_b (Body): The second body.
    anchor (Vec2): The anchor point in world coordinates.

Returns:
    RevoluteJoint: The created joint.
        )doc"
        )
        .def(
            "create_weld_joint", &World::createWeldJoint, py::arg("body_a"), py::arg("body_b"),
            py::arg("anchor"), R"doc(
Create a weld joint between two bodies.

Args:
    body_a (Body): The first body.
    body_b (Body): The second body.
    anchor (Vec2): The anchor point in world coordinates.

Returns:
    WeldJoint: The created joint.
        )doc"
        )
        .def(
            "create_wheel_joint", &World::createWheelJoint, py::arg("body_a"), py::arg("body_b"),
            py::arg("anchor"), py::arg("axis"), R"doc(
Create a wheel joint between two bodies.

Args:
    body_a (Body): The first body.
    body_b (Body): The second body.
    anchor (Vec2): The anchor point in world coordinates.
    axis (Vec2): The axis of movement in world coordinates.

Returns:
    WheelJoint: The created joint.
        )doc"
        )

        .def("step", &World::step, py::arg("time_step"), py::arg("sub_step_count"), R"doc(
Advance the physics simulation by a time step.

Args:
    time_step (float): The time step to advance the simulation.
    sub_step_count (int): The number of sub steps to take.
            )doc")
        .def("get_collisions", &World::getCollisions, R"doc(
Get all collision events that occurred during the last physics step.

Note:
    This only includes hit events. The list is cleared after each call.

Returns:
    list[Collision]: A list of collision events.
        )doc")
        .def("query_point", &World::queryPoint, py::arg("point"), R"doc(
Find all bodies that contain the specified point.

Args:
    point (Vec2): The point to query in world coordinates.

Returns:
    list[Body]: A list of bodies at the point.
        )doc")
        .def("query_aabb", &World::queryAABB, py::arg("rect"), R"doc(
Find all bodies that overlap with the specified rectangular area.

Args:
    rect (Rect): The rectangular area to query.

Returns:
    list[Body]: A list of bodies overlapping the area.
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
