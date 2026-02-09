#include <pybind11/native_enum.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

#include "Capsule.hpp"
#include "Circle.hpp"
#include "Polygon.hpp"
#include "Rect.hpp"
#include "Transform.hpp"
#include "physics/World.hpp"
#include "physics/bodies/Body.hpp"
#include "physics/bodies/CharacterBody.hpp"
#include "physics/bodies/RigidBody.hpp"
#include "physics/bodies/StaticBody.hpp"
#include "physics/joints/DistanceJoint.hpp"
#include "physics/joints/FilterJoint.hpp"
#include "physics/joints/Joint.hpp"
#include "physics/joints/MotorJoint.hpp"
#include "physics/joints/MouseJoint.hpp"
#include "physics/joints/PrismaticJoint.hpp"
#include "physics/joints/RevoluteJoint.hpp"
#include "physics/joints/WeldJoint.hpp"
#include "physics/joints/WheelJoint.hpp"

namespace kn::physics
{
void _bind(py::module_& m)
{
    auto subPhysics = m.def_submodule("physics", "Physics engine related classes and functions");

    subPhysics.def("set_fixed_delta", &setFixedDelta, py::arg("fixed_delta"), R"doc(
Set the fixed delta time for automatic physics stepping. Default is 1/60 seconds (60 FPS).

Setting this to a value greater than 0.0 enables automatic physics stepping
in the engine backend. The physics will be updated with this fixed time step,
using an accumulator to handle variable frame rates.

Args:
    fixed_delta (float): The fixed time step in seconds (e.g., 1.0/60.0).
                         Set to 0.0 to disable automatic stepping.
    )doc");
    subPhysics.def("get_fixed_delta", &getFixedDelta, R"doc(
Get the current fixed delta time for physics stepping.

Returns:
    float: The fixed time step in seconds.
    )doc");
    subPhysics.def("set_max_substeps", &setMaxSubsteps, py::arg("max_substeps"), R"doc(
Set the maximum number of substeps for physics stepping.

Args:
    max_substeps (int): The number of substeps per time step.
    )doc");
    subPhysics.def("get_max_substeps", &getMaxSubsteps, R"doc(
Get the current maximum number of substeps for physics stepping.

Returns:
    int: The number of substeps per time step.
    )doc");

    auto pyWorld = py::classh<World>(subPhysics, "World");

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
        .def(
            "add_collider",
            py::overload_cast<const Capsule&, float, float, float, bool, bool>(&Body::addCollider),
            py::arg("capsule"), py::arg("density") = 1.0f, py::arg("friction") = 0.2f,
            py::arg("restitution") = 0.0f, py::arg("enable_events") = false,
            py::arg("is_sensor") = false, R"doc(
Add a capsule collider to the body.

Args:
    capsule (Capsule): The capsule shape to add as a collider.
    density (float, optional): The density of the collider. Defaults to 1.0.
    friction (float, optional): The friction coefficient of the collider. Defaults to 0.2.
    restitution (float, optional): The restitution (bounciness) of the collider. Defaults to 0.0.
    enable_events (bool, optional): Whether to enable hit events for this collider. Defaults to False.
    is_sensor (bool, optional): Whether the collider is a sensor. Defaults to False.
            )doc"
        )

        .def_property(
            "pos", &Body::getPos, &Body::setPos,
            R"doc(The position of the body in world coordinates.)doc"
        )
        .def_property(
            "rotation", &Body::getRotation, &Body::setRotation,
            R"doc(The rotation of the body in radians.)doc"
        )
        .def_property_readonly(
            "is_valid", &Body::isValid, R"doc(Indicates whether the body is not destroyed.)doc"
        )

        .def("get_transform", &Body::getTransform, R"doc(
            Get the current transform of the body (position, rotation, and scale).

            Returns:
                Transform: The current transform of the body.
            )doc")
        .def("debug_draw", &Body::debugDraw, R"doc(
Draw all colliders attached to the body (debug/development only).
            )doc")
        .def("destroy", &Body::destroy, R"doc(Destroy the body manually.)doc")
        .def(py::self == py::self)
        .def(py::self != py::self);

    py::classh<RigidBody, Body>(subPhysics, "RigidBody")
        .def(py::init<World&>(), py::arg("world"))
        .def_property(
            "linear_velocity", &RigidBody::getLinearVelocity, &RigidBody::setLinearVelocity,
            R"doc(The linear velocity of the body.)doc"
        )
        .def_property(
            "angular_velocity", &RigidBody::getAngularVelocity, &RigidBody::setAngularVelocity,
            R"doc(The angular velocity of the body.)doc"
        )
        .def_property(
            "linear_damping", &RigidBody::getLinearDamping, &RigidBody::setLinearDamping,
            R"doc(The linear damping of the body.)doc"
        )
        .def_property(
            "angular_damping", &RigidBody::getAngularDamping, &RigidBody::setAngularDamping,
            R"doc(The angular damping of the body.)doc"
        )
        .def_property(
            "fixed_rotation", &RigidBody::isFixedRotation, &RigidBody::setFixedRotation,
            R"doc(Whether the body has fixed rotation.)doc"
        )
        .def_property_readonly(
            "awake", &RigidBody::isAwake,
            R"doc(Whether the body is currently awake and participating in simulation.)doc"
        )
        .def_property_readonly("mass", &RigidBody::getMass, R"doc(The mass of the body.)doc")

        .def("wake", &RigidBody::wake, R"doc(
Manually wake the body from sleep.
            )doc")
        .def(
            "apply_force", &RigidBody::applyForce, py::arg("force"), py::arg("point"),
            py::arg("wake") = true, R"doc(
Apply a force to the body at a specific point.

Args:
    force (Vec2): The force vector to apply.
    point (Vec2): The point (in world coordinates) where the force is applied.
    wake (bool, optional): Whether to wake the body if it's sleeping. Defaults to True.
            )doc"
        )
        .def(
            "apply_force_to_center", &RigidBody::applyForceToCenter, py::arg("force"),
            py::arg("wake") = true, R"doc(
Apply a force to the center of mass of the body.

Args:
    force (Vec2): The force vector to apply.
    wake (bool, optional): Whether to wake the body if it's sleeping. Defaults to True.
            )doc"
        )
        .def(
            "apply_torque", &RigidBody::applyTorque, py::arg("torque"), py::arg("wake") = true,
            R"doc(
Apply a torque to the body.

Args:
    torque (float): The torque to apply.
    wake (bool, optional): Whether to wake the body if it's sleeping. Defaults to True.
            )doc"
        )
        .def(
            "apply_linear_impulse", &RigidBody::applyLinearImpulse, py::arg("impulse"),
            py::arg("point"), py::arg("wake") = true, R"doc(
Apply a linear impulse to the body at a specific point.

Args:
    impulse (Vec2): The impulse vector to apply.
    point (Vec2): The point (in world coordinates) where the impulse is applied.
    wake (bool, optional): Whether to wake the body if it's sleeping. Defaults to True.
            )doc"
        )
        .def(
            "apply_linear_impulse_to_center", &RigidBody::applyLinearImpulseToCenter,
            py::arg("impulse"), py::arg("wake") = true, R"doc(
Apply a linear impulse to the center of mass of the body.

Args:
    impulse (Vec2): The impulse vector to apply.
    wake (bool, optional): Whether to wake the body if it's sleeping. Defaults to True.
            )doc"
        )
        .def(
            "apply_angular_impulse", &RigidBody::applyAngularImpulse, py::arg("impulse"),
            py::arg("wake") = true, R"doc(
Apply an angular impulse to the body.

Args:
    impulse (float): The angular impulse to apply.
    wake (bool, optional): Whether to wake the body if it's sleeping. Defaults to True.
            )doc"
        );

    py::classh<StaticBody, Body>(subPhysics, "StaticBody")
        .def(py::init<World&>(), py::arg("world"));

    py::classh<CharacterBody, Body>(subPhysics, "CharacterBody")
        .def(py::init<World&>(), py::arg("world"))
        .def_readwrite(
            "velocity", &CharacterBody::velocity, R"doc(The velocity of the character body.)doc"
        )
        .def_readwrite(
            "mass", &CharacterBody::mass,
            R"doc(The mass of the character body. Default is 80.0.)doc"
        )
        .def_readwrite(
            "floor_max_angle", &CharacterBody::floorMaxAngle,
            R"doc(Maximum angle (in radians) to consider a surface as a floor. Default is ~45 degrees.)doc"
        )
        .def_readwrite(
            "floor_snap_distance", &CharacterBody::floorSnapDistance,
            R"doc(Distance in pixels to probe downward for floor detection. Default is 5.0.)doc"
        )

        .def(
            "is_on_floor", &CharacterBody::isOnFloor,
            R"doc(Whether the character is currently on a floor surface.)doc"
        )
        .def(
            "is_on_ceiling", &CharacterBody::isOnCeiling,
            R"doc(Whether the character is currently touching a ceiling.)doc"
        )
        .def(
            "is_on_wall", &CharacterBody::isOnWall,
            R"doc(Whether the character is currently touching a wall.)doc"
        )
        .def(
            "move_and_slide", &CharacterBody::moveAndSlide, py::arg("delta") = -1.0,
            R"doc(
Perform movement and collision resolution for the character.

This method moves the character according to the velocity property and resolves
collisions by sliding along surfaces. It also updates the floor/ceiling/wall
contact states.

Args:
    delta (float, optional): The time step to use for movement.
                             Defaults to -1.0, which uses the frame delta.
        )doc"
        );

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

    py::classh<MouseJoint, Joint>(subPhysics, "MouseJoint")
        .def_property(
            "target", &MouseJoint::getTarget, &MouseJoint::setTarget,
            R"doc(The target point in world coordinates.)doc"
        )
        .def_property(
            "spring_hz", &MouseJoint::getSpringHertz, &MouseJoint::setSpringHertz,
            R"doc(The spring frequency in Hertz.)doc"
        )
        .def_property(
            "spring_damping_ratio", &MouseJoint::getSpringDampingRatio,
            &MouseJoint::setSpringDampingRatio, R"doc(The spring damping ratio.)doc"
        )
        .def_property(
            "max_force", &MouseJoint::getMaxForce, &MouseJoint::setMaxForce,
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

    py::classh<CastHit>(subPhysics, "CastHit")
        .def_readonly("body", &CastHit::body, "The body that was hit.")
        .def_readonly("point", &CastHit::point, "The point of the hit in world coordinates.")
        .def_readonly("normal", &CastHit::normal, "The normal vector of the hit surface.")
        .def_readonly(
            "fraction", &CastHit::fraction,
            "The fraction along the cast path at which the hit occurred."
        );

    pyWorld
        .def(py::init<const Vec2&>(), py::arg("gravity"), R"doc(
Create a new physics world with the specified gravity.

Args:
    gravity (Vec2): The gravity vector for the world.
        )doc")

        .def_property(
            "gravity", &World::getGravity, &World::setGravity,
            R"doc(The gravity vector of the world.)doc"
        )

        .def(
            "add_fixed_update", &World::addFixedUpdate, py::arg("callback"),
            R"doc(Add a callback function to be executed during each physics step.)doc"
        )
        .def(
            "fixed_callback",
            [](World& self, const py::object& func)
            {
                self.addFixedUpdate(func);
                return func;
            },
            py::arg("callback"),
            R"doc(A decorator to register a function as a physics update callback.)doc"
        )
        .def(
            "clear_fixed_updates", &World::clearFixedUpdates,
            R"doc(Remove all registered fixed update callbacks.)doc"
        )

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
            "create_mouse_joint", &World::createMouseJoint, py::arg("ground_body"),
            py::arg("pulled_body"), py::arg("target"), R"doc(
Create a mouse joint between a ground body and a target body.

Args:
    ground_body (Body): The ground body (usually a static body).
    pulled_body (Body): The body to be pulled and moved to the target.
    target (Vec2): The initial target point in world coordinates.

Returns:
    MouseJoint: The created joint.
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
        .def("ray_cast", &World::rayCast, py::arg("origin"), py::arg("translation"), R"doc(
Cast a ray into the world and find all bodies that intersect it.

Args:
    origin (Vec2): The starting point of the ray.
    translation (Vec2): The direction and length of the ray.

Returns:
    list[RayCastHit]: A list of hits, sorted by distance (fraction).
        )doc")
        .def(
            "shape_cast",
            py::overload_cast<const Circle&, const Transform&, const Vec2&>(&World::shapeCast),
            py::arg("circle"), py::arg("transform"), py::arg("translation"), R"doc(
Cast a circular shape into the world.

Args:
    circle (Circle): The circular shape.
    transform (Transform): The initial transform of the shape.
    translation (Vec2): The movement vector.

Returns:
    list[ShapeCastHit]: A list of hits, sorted by distance.
            )doc"
        )
        .def(
            "shape_cast",
            py::overload_cast<const Capsule&, const Transform&, const Vec2&>(&World::shapeCast),
            py::arg("capsule"), py::arg("transform"), py::arg("translation"), R"doc(
Cast a capsule shape into the world.

Args:
    capsule (Capsule): The capsule shape.
    transform (Transform): The initial transform of the shape.
    translation (Vec2): The movement vector.

Returns:
    list[ShapeCastHit]: A list of hits, sorted by distance.
            )doc"
        )
        .def(
            "shape_cast",
            py::overload_cast<const Polygon&, const Transform&, const Vec2&>(&World::shapeCast),
            py::arg("polygon"), py::arg("transform"), py::arg("translation"), R"doc(
Cast a polygonal shape into the world.

Args:
    polygon (Polygon): The polygonal shape.
    transform (Transform): The initial transform of the shape.
    translation (Vec2): The movement vector.

Returns:
    list[ShapeCastHit]: A list of hits, sorted by distance.
            )doc"
        )
        .def(
            "shape_cast",
            py::overload_cast<const Rect&, const Transform&, const Vec2&>(&World::shapeCast),
            py::arg("rect"), py::arg("transform"), py::arg("translation"), R"doc(
Cast a rectangular shape into the world.

Args:
    rect (Rect): The rectangular shape.
    transform (Transform): The initial transform of the shape.
    translation (Vec2): The movement vector.

Returns:
    list[ShapeCastHit]: A list of hits, sorted by distance.
            )doc"
        );
}
}  // namespace kn::physics
