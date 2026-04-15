#include <nanobind/operators.h>
#include <nanobind/stl/function.h>
#include <nanobind/stl/vector.h>

#include "Capsule.hpp"
#include "Circle.hpp"
#include "Polygon.hpp"
#include "Rect.hpp"
#include "TileMap.hpp"
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
void _bind(nb::module_& m)
{
    using namespace nb::literals;

    auto subPhysics = m.def_submodule("physics", "Physics engine related classes and functions");

    subPhysics.def("set_fixed_delta", &setFixedDelta, "fixed_delta"_a, R"doc(
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
    subPhysics.def("set_max_substeps", &setMaxSubsteps, "max_substeps"_a, R"doc(
Set the maximum number of substeps for physics stepping. Default is 4.

Args:
    max_substeps (int): The number of substeps per time step.
    )doc");
    subPhysics.def("get_max_substeps", &getMaxSubsteps, R"doc(
Get the current maximum number of substeps for physics stepping.

Returns:
    int: The number of substeps per time step.
    )doc");

    auto pyWorld = nb::class_<World>(subPhysics, "World", R"doc(
A physics world that manages bodies, joints, and collision detection.

The world handles stepping the physics simulation, creating and destroying bodies
and joints, and provides spatial queries such as ray casts and shape casts.
Multiple worlds can exist simultaneously, each with their own gravity and bodies.
        )doc");

    nb::class_<Body>(subPhysics, "Body", R"doc(
Base class for all physics bodies.

A body represents a physical object in the simulation. It can have one or more
colliders (shapes) attached to it and participates in collision detection and
resolution. This is an abstract base class — use RigidBody, StaticBody, or
CharacterBody instead.
        )doc")
        .def(
            "add_collider",
            nb::overload_cast<const Circle&, float, float, float, bool, bool>(&Body::addCollider),
            "circle"_a, "density"_a = 1.0f, "friction"_a = 0.2f, "restitution"_a = 0.0f,
            "enable_events"_a = false, "is_sensor"_a = false, R"doc(
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
            nb::overload_cast<const Polygon&, float, float, float, bool, bool>(&Body::addCollider),
            "polygon"_a, "density"_a = 1.0f, "friction"_a = 0.2f, "restitution"_a = 0.0f,
            "enable_events"_a = false, "is_sensor"_a = false, R"doc(
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
            nb::overload_cast<const Rect&, float, float, float, bool, bool>(&Body::addCollider),
            "rect"_a, "density"_a = 1.0f, "friction"_a = 0.2f, "restitution"_a = 0.0f,
            "enable_events"_a = false, "is_sensor"_a = false, R"doc(
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
            nb::overload_cast<const Capsule&, float, float, float, bool, bool>(&Body::addCollider),
            "capsule"_a, "density"_a = 1.0f, "friction"_a = 0.2f, "restitution"_a = 0.0f,
            "enable_events"_a = false, "is_sensor"_a = false, R"doc(
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

        .def_prop_rw(
            "pos", &Body::getPos, &Body::setPos,
            R"doc(The position of the body in world coordinates.)doc"
        )
        .def_prop_rw(
            "rotation", &Body::getRotation, &Body::setRotation,
            R"doc(The rotation of the body in radians.)doc"
        )
        .def_prop_rw(
            "collision_layer", &Body::getCollisionLayer, &Body::setCollisionLayer,
            R"doc(The body's collision layer (category bits).)doc"
        )
        .def_prop_rw(
            "collision_mask", &Body::getCollisionMask, &Body::setCollisionMask,
            R"doc(The body's collision mask.)doc"
        )
        .def_prop_ro(
            "is_valid", &Body::isValid, R"doc(Indicates whether the body is not destroyed.)doc"
        )

        .def("get_transform", &Body::getTransform, R"doc(
            Get the current transform of the body (position, rotation, and scale).

            Returns:
                Transform: The current transform of the body.
            )doc")
        .def("destroy", &Body::destroy, R"doc(Destroy the body manually.)doc")
        .def(nb::self == nb::self)
        .def(nb::self != nb::self);

    nb::class_<RigidBody, Body>(subPhysics, "RigidBody", R"doc(
A dynamic physics body that responds to forces, impulses, and collisions.

Rigid bodies are fully simulated by the physics engine. They have mass, velocity,
and can be affected by gravity, forces, and impulses. Use this for objects like
projectiles, crates, or anything that should move realistically.
        )doc")
        .def(nb::init<World&>(), "world"_a, R"doc(
Create a new rigid body in the specified world.

The body is created at the origin with no colliders. Add colliders with
``add_collider`` before the body can interact with other objects.

Args:
    world (World): The physics world to create the body in.
        )doc")
        .def_prop_rw(
            "linear_velocity", &RigidBody::getLinearVelocity, &RigidBody::setLinearVelocity,
            R"doc(The linear velocity of the body.)doc"
        )
        .def_prop_rw(
            "angular_velocity", &RigidBody::getAngularVelocity, &RigidBody::setAngularVelocity,
            R"doc(The angular velocity of the body.)doc"
        )
        .def_prop_rw(
            "linear_damping", &RigidBody::getLinearDamping, &RigidBody::setLinearDamping,
            R"doc(The linear damping of the body.)doc"
        )
        .def_prop_rw(
            "angular_damping", &RigidBody::getAngularDamping, &RigidBody::setAngularDamping,
            R"doc(The angular damping of the body.)doc"
        )
        .def_prop_rw(
            "fixed_rotation", &RigidBody::isFixedRotation, &RigidBody::setFixedRotation,
            R"doc(Whether the body has fixed rotation.)doc"
        )
        .def_prop_rw(
            "is_bullet", &RigidBody::isBullet, &RigidBody::setBullet,
            R"doc(Whether CCD is enabled for this body.)doc"
        )
        .def_prop_ro(
            "is_awake", &RigidBody::isAwake,
            R"doc(Whether the body is currently awake and simulating.)doc"
        )
        .def_prop_ro("mass", &RigidBody::getMass, R"doc(The mass of the body.)doc")

        .def("wake", &RigidBody::wake, R"doc(Manually wake the body from sleep.)doc")
        .def("apply_force", &RigidBody::applyForce, "force"_a, "point"_a, "wake"_a = true, R"doc(
Apply a force to the body at a specific point.

Args:
    force (Vec2): The force vector to apply.
    point (Vec2): The point (in world coordinates) where the force is applied.
    wake (bool, optional): Whether to wake the body if it's sleeping. Defaults to True.
            )doc")
        .def(
            "apply_force_to_center", &RigidBody::applyForceToCenter, "force"_a, "wake"_a = true,
            R"doc(
Apply a force to the center of mass of the body.

Args:
    force (Vec2): The force vector to apply.
    wake (bool, optional): Whether to wake the body if it's sleeping. Defaults to True.
            )doc"
        )
        .def(
            "apply_torque", &RigidBody::applyTorque, "torque"_a, "wake"_a = true,
            R"doc(
Apply a torque to the body.

Args:
    torque (float): The torque to apply.
    wake (bool, optional): Whether to wake the body if it's sleeping. Defaults to True.
            )doc"
        )
        .def(
            "apply_linear_impulse", &RigidBody::applyLinearImpulse, "impulse"_a, "point"_a,
            "wake"_a = true, R"doc(
Apply a linear impulse to the body at a specific point.

Args:
    impulse (Vec2): The impulse vector to apply.
    point (Vec2): The point (in world coordinates) where the impulse is applied.
    wake (bool, optional): Whether to wake the body if it's sleeping. Defaults to True.
            )doc"
        )
        .def(
            "apply_linear_impulse_to_center", &RigidBody::applyLinearImpulseToCenter, "impulse"_a,
            "wake"_a = true, R"doc(
Apply a linear impulse to the center of mass of the body.

Args:
    impulse (Vec2): The impulse vector to apply.
    wake (bool, optional): Whether to wake the body if it's sleeping. Defaults to True.
            )doc"
        )
        .def(
            "apply_angular_impulse", &RigidBody::applyAngularImpulse, "impulse"_a, "wake"_a = true,
            R"doc(
Apply an angular impulse to the body.

Args:
    impulse (float): The angular impulse to apply.
    wake (bool, optional): Whether to wake the body if it's sleeping. Defaults to True.
            )doc"
        );

    nb::class_<StaticBody, Body>(subPhysics, "StaticBody", R"doc(
A physics body that does not move.

Static bodies are fixed in place and are not affected by forces or collisions.
They are ideal for level geometry such as floors, walls, and platforms. Static
bodies have zero velocity and infinite mass.
        )doc")
        .def(nb::init<World&>(), "world"_a, R"doc(
Create a new static body in the specified world.

The body is created at the origin with no colliders. Add colliders with
``add_collider`` to define its shape.

Args:
    world (World): The physics world to create the body in.
        )doc");

    nb::class_<CharacterBody, Body> charBody(subPhysics, "CharacterBody", R"doc(
A kinematic physics body designed for player-controlled characters.

Character bodies use a move-and-slide algorithm for movement and collision
resolution. They automatically detect floor, wall, and ceiling contacts and
slide along surfaces rather than bouncing off them. Ideal for platformer
characters, NPCs, or any entity that needs precise movement control.
        )doc");

    nb::enum_<CharacterBody::MotionMode>(charBody, "MotionMode", R"doc(
The motion mode for the character body.
        )doc")
        .value("GROUNDED", CharacterBody::MotionMode::Grounded, R"doc(
The character is affected by gravity and can stand on floors.
        )doc")
        .value("FLOATING", CharacterBody::MotionMode::Floating, R"doc(
The character is not affected by gravity and can move freely in the air.
        )doc");

    charBody
        .def(nb::init<World&>(), "world"_a, R"doc(
Create a new character body in the specified world.

The body is created at the origin with default movement properties.
Add colliders with ``add_collider`` before calling ``move_and_slide``.

Args:
    world (World): The physics world to create the body in.
        )doc")

        .def_rw(
            "motion_mode", &CharacterBody::motionMode, R"doc(The motion mode of the character.)doc"
        )
        .def_rw(
            "velocity", &CharacterBody::velocity, R"doc(The velocity of the character body.)doc"
        )
        .def_rw(
            "mass", &CharacterBody::mass,
            R"doc(The simulated mass of the character for pushing rigid bodies.)doc"
        )
        .def_rw(
            "max_speed", &CharacterBody::maxSpeed,
            R"doc(The maximum speed the character can move.)doc"
        )
        .def_rw(
            "acceleration", &CharacterBody::acceleration,
            R"doc(The acceleration applied when moving.)doc"
        )
        .def_rw(
            "friction", &CharacterBody::friction,
            R"doc(The friction applied when on the ground.)doc"
        )
        .def_rw(
            "stop_speed", &CharacterBody::stopSpeed,
            R"doc(The speed threshold below which the character will stop moving.)doc"
        )
        .def_rw(
            "air_steer", &CharacterBody::airSteer,
            R"doc(The control factor for steering while in the air (0.0 to 1.0).)doc"
        )

        .def_prop_rw(
            "capsule_shape", &CharacterBody::getCapsuleShape, &CharacterBody::setCapsuleShape,
            R"doc(The capsule shape used for ground detection and collision casting.)doc"
        )

        .def(
            "move_and_slide", &CharacterBody::moveAndSlide, "delta"_a = -1.0,
            R"doc(
Perform movement and collision resolution for the character.

This method moves the character according to the velocity property and resolves
collisions by sliding along surfaces. It also updates the floor/ceiling/wall
contact states.

Args:
    delta (float, optional): The time step to use for movement.
                             Defaults to -1.0, which uses the frame delta.
        )doc"
        )
        .def("is_on_floor", &CharacterBody::isOnFloor, R"doc(
Check if the character is currently in contact with the floor.

Returns:
    bool: True if the character is on the floor, False otherwise.
        )doc")
        .def("is_on_ceiling", &CharacterBody::isOnCeiling, R"doc(
Check if the character is currently in contact with the ceiling.

Returns:
    bool: True if the character is on the ceiling, False otherwise.
        )doc")
        .def("is_on_wall", &CharacterBody::isOnWall, R"doc(
Check if the character is currently in contact with a wall.

Returns:
    bool: True if the character is on a wall, False otherwise.
        )doc");

    nb::class_<Joint>(subPhysics, "Joint", R"doc(
Base class for all physics joints.

A joint constrains the relative motion of two bodies. Joints are created
through the World's ``create_*_joint`` methods and can be destroyed manually
or when either attached body is destroyed.
        )doc")
        .def_prop_rw(
            "collide_connected", &Joint::getCollideConnected, &Joint::setCollideConnected,
            R"doc(Whether the connected bodies should collide with each other.)doc"
        )
        .def_prop_rw(
            "local_anchor_a", &Joint::getLocalAnchorA, &Joint::setLocalAnchorA,
            R"doc(The local anchor point relative to body A's origin.)doc"
        )
        .def_prop_rw(
            "local_anchor_b", &Joint::getLocalAnchorB, &Joint::setLocalAnchorB,
            R"doc(The local anchor point relative to body B's origin.)doc"
        )
        .def_prop_ro("body_a", &Joint::getBodyA, R"doc(The first body attached to the joint.)doc")
        .def_prop_ro("body_b", &Joint::getBodyB, R"doc(The second body attached to the joint.)doc")
        .def_prop_ro(
            "is_valid", &Joint::isValid, R"doc(Indicates whether the joint is not destroyed.)doc"
        )
        .def("destroy", &Joint::destroy, R"doc(Destroy the joint manually.)doc");

    nb::class_<DistanceJoint, Joint>(subPhysics, "DistanceJoint", R"doc(
A joint that constrains two bodies to maintain a fixed distance between their anchor points.

Optionally supports a spring to soften the constraint, a motor to drive the
distance, and limits to allow a range of distances.
        )doc")
        .def_prop_rw(
            "length", &DistanceJoint::getLength, &DistanceJoint::setLength,
            R"doc(The rest length of the joint.)doc"
        )
        .def_prop_rw(
            "spring_enabled", &DistanceJoint::isSpringEnabled, &DistanceJoint::enableSpring,
            R"doc(Whether the spring is enabled.)doc"
        )
        .def_prop_rw(
            "spring_hz", &DistanceJoint::getSpringHertz, &DistanceJoint::setSpringHertz,
            R"doc(The spring frequency in Hertz.)doc"
        )
        .def_prop_rw(
            "spring_damping_ratio", &DistanceJoint::getSpringDampingRatio,
            &DistanceJoint::setSpringDampingRatio, R"doc(The spring damping ratio.)doc"
        )
        .def_prop_rw(
            "limit_enabled", &DistanceJoint::isLimitEnabled, &DistanceJoint::enableLimit,
            R"doc(Whether the length limits are enabled.)doc"
        )
        .def_prop_rw(
            "motor_enabled", &DistanceJoint::isMotorEnabled, &DistanceJoint::enableMotor,
            R"doc(Whether the motor is enabled.)doc"
        )
        .def_prop_rw(
            "motor_speed", &DistanceJoint::getMotorSpeed, &DistanceJoint::setMotorSpeed,
            R"doc(The target motor speed.)doc"
        )
        .def_prop_rw(
            "max_motor_force", &DistanceJoint::getMaxMotorForce, &DistanceJoint::setMaxMotorForce,
            R"doc(The maximum motor force.)doc"
        )
        .def_prop_ro(
            "min_length", &DistanceJoint::getMinLength, R"doc(The minimum length limit.)doc"
        )
        .def_prop_ro(
            "max_length", &DistanceJoint::getMaxLength, R"doc(The maximum length limit.)doc"
        )
        .def_prop_ro(
            "current_length", &DistanceJoint::getCurrentLength,
            R"doc(The current length between the anchors.)doc"
        )
        .def_prop_ro(
            "motor_force", &DistanceJoint::getMotorForce, R"doc(The current motor force.)doc"
        )
        .def(
            "set_length_range", &DistanceJoint::setLengthRange, "min_length"_a, "max_length"_a,
            R"doc(
Set the minimum and maximum length limits.

Args:
    min_length (float): The minimum length.
    max_length (float): The maximum length.
            )doc"
        );

    nb::class_<FilterJoint, Joint>(
        subPhysics, "FilterJoint", R"doc(A joint used to filter collisions between two bodies.)doc"
    );

    nb::class_<MotorJoint, Joint>(
        subPhysics, "MotorJoint",
        R"doc(
A joint that drives two bodies toward a target relative linear and angular offset.

The motor joint applies forces and torques to maintain the specified offsets.
Useful for animating bodies to follow paths or track positions while still
interacting with the physics simulation.
        )doc"
    )
        .def_prop_rw(
            "linear_offset", &MotorJoint::getLinearOffset, &MotorJoint::setLinearOffset,
            R"doc(The target linear offset from body A to body B.)doc"
        )
        .def_prop_rw(
            "angular_offset", &MotorJoint::getAngularOffset, &MotorJoint::setAngularOffset,
            R"doc(The target angular offset in radians.)doc"
        )
        .def_prop_rw(
            "max_force", &MotorJoint::getMaxForce, &MotorJoint::setMaxForce,
            R"doc(The maximum motor force.)doc"
        )
        .def_prop_rw(
            "max_torque", &MotorJoint::getMaxTorque, &MotorJoint::setMaxTorque,
            R"doc(The maximum motor torque.)doc"
        )
        .def_prop_rw(
            "correction_factor", &MotorJoint::getCorrectionFactor, &MotorJoint::setCorrectionFactor,
            R"doc(The position correction factor in [0, 1].)doc"
        );

    nb::class_<MouseJoint, Joint>(subPhysics, "MouseJoint", R"doc(
A joint that pulls a body toward a world-space target point using a spring.

The mouse joint is typically used for click-and-drag interactions, where a
user drags a body around the world. It applies a spring force to pull the
body toward the target position.
        )doc")
        .def_prop_rw(
            "target", &MouseJoint::getTarget, &MouseJoint::setTarget,
            R"doc(The target point in world coordinates.)doc"
        )
        .def_prop_rw(
            "spring_hz", &MouseJoint::getSpringHertz, &MouseJoint::setSpringHertz,
            R"doc(The spring frequency in Hertz.)doc"
        )
        .def_prop_rw(
            "spring_damping_ratio", &MouseJoint::getSpringDampingRatio,
            &MouseJoint::setSpringDampingRatio, R"doc(The spring damping ratio.)doc"
        )
        .def_prop_rw(
            "max_force", &MouseJoint::getMaxForce, &MouseJoint::setMaxForce,
            R"doc(The maximum constraint force.)doc"
        );

    nb::class_<PrismaticJoint, Joint>(subPhysics, "PrismaticJoint", R"doc(
A joint that constrains two bodies to move only along a specified axis.

Also known as a slider joint. Supports optional translation limits, a spring,
and a motor. Useful for pistons, elevators, or sliding doors.
        )doc")
        .def_prop_rw(
            "spring_enabled", &PrismaticJoint::isSpringEnabled, &PrismaticJoint::enableSpring,
            R"doc(Whether the spring is enabled.)doc"
        )
        .def_prop_rw(
            "spring_hz", &PrismaticJoint::getSpringHertz, &PrismaticJoint::setSpringHertz,
            R"doc(The spring frequency in Hertz.)doc"
        )
        .def_prop_rw(
            "spring_damping_ratio", &PrismaticJoint::getSpringDampingRatio,
            &PrismaticJoint::setSpringDampingRatio, R"doc(The spring damping ratio.)doc"
        )
        .def_prop_rw(
            "target_translation", &PrismaticJoint::getTargetTranslation,
            &PrismaticJoint::setTargetTranslation, R"doc(The target translation for the motor.)doc"
        )
        .def_prop_rw(
            "limit_enabled", &PrismaticJoint::isLimitEnabled, &PrismaticJoint::enableLimit,
            R"doc(Whether the translation limits are enabled.)doc"
        )
        .def_prop_rw(
            "motor_enabled", &PrismaticJoint::isMotorEnabled, &PrismaticJoint::enableMotor,
            R"doc(Whether the motor is enabled.)doc"
        )
        .def_prop_rw(
            "motor_speed", &PrismaticJoint::getMotorSpeed, &PrismaticJoint::setMotorSpeed,
            R"doc(The target motor speed.)doc"
        )
        .def_prop_rw(
            "max_motor_force", &PrismaticJoint::getMaxMotorForce, &PrismaticJoint::setMaxMotorForce,
            R"doc(The maximum motor force.)doc"
        )
        .def_prop_ro(
            "lower_limit", &PrismaticJoint::getLowerLimit, R"doc(The lower translation limit.)doc"
        )
        .def_prop_ro(
            "upper_limit", &PrismaticJoint::getUpperLimit, R"doc(The upper translation limit.)doc"
        )
        .def_prop_ro(
            "motor_force", &PrismaticJoint::getMotorForce, R"doc(The current motor force.)doc"
        )
        .def_prop_ro(
            "translation", &PrismaticJoint::getTranslation,
            R"doc(The current joint translation.)doc"
        )
        .def_prop_ro(
            "speed", &PrismaticJoint::getSpeed, R"doc(The current joint translation speed.)doc"
        )
        .def("set_limits", &PrismaticJoint::setLimits, "lower"_a, "upper"_a, R"doc(
Set the translation limits.

Args:
    lower (float): The lower translation limit.
    upper (float): The upper translation limit.
            )doc");

    nb::class_<RevoluteJoint, Joint>(subPhysics, "RevoluteJoint", R"doc(
A joint that allows two bodies to rotate around a shared anchor point.

Also known as a hinge or pin joint. Supports optional angle limits, a spring,
and a motor. Useful for doors, wheels, pendulums, and rotating platforms.
        )doc")
        .def_prop_rw(
            "spring_enabled", &RevoluteJoint::isSpringEnabled, &RevoluteJoint::enableSpring,
            R"doc(Whether the spring is enabled.)doc"
        )
        .def_prop_rw(
            "spring_hz", &RevoluteJoint::getSpringHertz, &RevoluteJoint::setSpringHertz,
            R"doc(The spring frequency in Hertz.)doc"
        )
        .def_prop_rw(
            "spring_damping_ratio", &RevoluteJoint::getSpringDampingRatio,
            &RevoluteJoint::setSpringDampingRatio, R"doc(The spring damping ratio.)doc"
        )
        .def_prop_rw(
            "target_angle", &RevoluteJoint::getTargetAngle, &RevoluteJoint::setTargetAngle,
            R"doc(The target angle for the motor in radians.)doc"
        )
        .def_prop_rw(
            "limit_enabled", &RevoluteJoint::isLimitEnabled, &RevoluteJoint::enableLimit,
            R"doc(Whether the angle limits are enabled.)doc"
        )
        .def_prop_rw(
            "motor_enabled", &RevoluteJoint::isMotorEnabled, &RevoluteJoint::enableMotor,
            R"doc(Whether the motor is enabled.)doc"
        )
        .def_prop_rw(
            "motor_speed", &RevoluteJoint::getMotorSpeed, &RevoluteJoint::setMotorSpeed,
            R"doc(The target motor speed in radians per second.)doc"
        )
        .def_prop_rw(
            "max_motor_torque", &RevoluteJoint::getMaxMotorTorque,
            &RevoluteJoint::setMaxMotorTorque, R"doc(The maximum motor torque.)doc"
        )
        .def_prop_ro(
            "lower_limit", &RevoluteJoint::getLowerLimit,
            R"doc(The lower angle limit in radians.)doc"
        )
        .def_prop_ro(
            "upper_limit", &RevoluteJoint::getUpperLimit,
            R"doc(The upper angle limit in radians.)doc"
        )
        .def_prop_ro(
            "angle", &RevoluteJoint::getAngle, R"doc(The current joint angle in radians.)doc"
        )
        .def_prop_ro(
            "motor_torque", &RevoluteJoint::getMotorTorque, R"doc(The current motor torque.)doc"
        )
        .def("set_limits", &RevoluteJoint::setLimits, "lower"_a, "upper"_a, R"doc(
Set the angle limits.

Args:
    lower (float): The lower angle limit in radians.
    upper (float): The upper angle limit in radians.
            )doc");

    nb::class_<WeldJoint, Joint>(subPhysics, "WeldJoint", R"doc(
A joint that rigidly connects two bodies at an anchor point.

The weld joint attempts to keep the bodies at a fixed relative position and
angle. Optionally supports linear and angular springs to allow some flex.
Useful for breakable structures or soft connections between bodies.
        )doc")
        .def_prop_rw(
            "linear_hz", &WeldJoint::getLinearHertz, &WeldJoint::setLinearHertz,
            R"doc(The linear spring frequency in Hertz.)doc"
        )
        .def_prop_rw(
            "linear_damping_ratio", &WeldJoint::getLinearDampingRatio,
            &WeldJoint::setLinearDampingRatio, R"doc(The linear spring damping ratio.)doc"
        )
        .def_prop_rw(
            "angular_hz", &WeldJoint::getAngularHertz, &WeldJoint::setAngularHertz,
            R"doc(The angular spring frequency in Hertz.)doc"
        )
        .def_prop_rw(
            "angular_damping_ratio", &WeldJoint::getAngularDampingRatio,
            &WeldJoint::setAngularDampingRatio, R"doc(The angular spring damping ratio.)doc"
        );

    nb::class_<WheelJoint, Joint>(subPhysics, "WheelJoint", R"doc(
A joint that simulates a wheel attached to a vehicle body.

Provides a suspension spring along a specified axis and an optional motor for
driving rotation. Supports translation limits along the suspension axis.
Ideal for vehicle wheels and similar mechanisms.
        )doc")
        .def_prop_rw(
            "spring_enabled", &WheelJoint::isSpringEnabled, &WheelJoint::enableSpring,
            R"doc(Whether the spring is enabled.)doc"
        )
        .def_prop_rw(
            "spring_hz", &WheelJoint::getSpringHertz, &WheelJoint::setSpringHertz,
            R"doc(The spring frequency in Hertz.)doc"
        )
        .def_prop_rw(
            "spring_damping_ratio", &WheelJoint::getSpringDampingRatio,
            &WheelJoint::setSpringDampingRatio, R"doc(The spring damping ratio.)doc"
        )
        .def_prop_rw(
            "limit_enabled", &WheelJoint::isLimitEnabled, &WheelJoint::enableLimit,
            R"doc(Whether the translation limits are enabled.)doc"
        )
        .def_prop_rw(
            "motor_enabled", &WheelJoint::isMotorEnabled, &WheelJoint::enableMotor,
            R"doc(Whether the motor is enabled.)doc"
        )
        .def_prop_rw(
            "motor_speed", &WheelJoint::getMotorSpeed, &WheelJoint::setMotorSpeed,
            R"doc(The target motor speed in radians per second.)doc"
        )
        .def_prop_rw(
            "max_motor_torque", &WheelJoint::getMaxMotorTorque, &WheelJoint::setMaxMotorTorque,
            R"doc(The maximum motor torque.)doc"
        )
        .def_prop_ro(
            "lower_limit", &WheelJoint::getLowerLimit, R"doc(The lower translation limit.)doc"
        )
        .def_prop_ro(
            "upper_limit", &WheelJoint::getUpperLimit, R"doc(The upper translation limit.)doc"
        )
        .def_prop_ro(
            "motor_torque", &WheelJoint::getMotorTorque, R"doc(The current motor torque.)doc"
        )
        .def("set_limits", &WheelJoint::setLimits, "lower"_a, "upper"_a, R"doc(
Set the translation limits.

Args:
    lower (float): The lower translation limit.
    upper (float): The upper translation limit.
            )doc");

    nb::class_<Collision>(subPhysics, "Collision", R"doc(
Information about a collision between two bodies.

Collision events are generated during the physics step for colliders that have
``enable_events`` set to True. Retrieve them with ``World.get_collisions()``.
        )doc")
        .def_ro("body_a", &Collision::bodyA, "The first body involved in the collision.")
        .def_ro("body_b", &Collision::bodyB, "The second body involved in the collision.")
        .def_ro("point", &Collision::point, "The point of impact in world coordinates.")
        .def_ro("normal", &Collision::normal, "The normal vector of the collision.")
        .def_ro(
            "approach_speed", &Collision::approachSpeed,
            "The speed at which the bodies approached each other."
        );

    nb::class_<CastHit>(subPhysics, "CastHit", R"doc(
Result of a ray cast or shape cast query.

Contains the body that was hit, the point and normal of the intersection,
and the fraction along the cast path where the hit occurred.
        )doc")
        .def_ro("body", &CastHit::body, "The body that was hit.")
        .def_ro("point", &CastHit::point, "The point of the hit in world coordinates.")
        .def_ro("normal", &CastHit::normal, "The normal vector of the hit surface.")
        .def_ro(
            "fraction", &CastHit::fraction,
            "The fraction along the cast path at which the hit occurred."
        );

    pyWorld
        .def(nb::init<const Vec2&>(), "gravity"_a = Vec2::ZERO, R"doc(
Create a new physics world with the specified gravity.

Args:
    gravity (Vec2, optional): The gravity vector for the world. Defaults to (0, 0).
        )doc")

        .def_prop_rw(
            "gravity", &World::getGravity, &World::setGravity,
            R"doc(The gravity vector of the world.)doc"
        )
        .def(
            "debug_draw",
            [](const World& self, const Color& color, bool filledShapes, bool shapes, bool joints,
               bool jointExtras, bool bounds, bool mass, bool bodyNames, bool contacts,
               bool graphColors, bool contactNormals, bool contactImpulses, bool contactFeatures,
               bool frictionImpulses, bool islands)
            {
                DebugDrawOptions options{};
                options.filledShapes = filledShapes;
                options.shapes = shapes;
                options.joints = joints;
                options.jointExtras = jointExtras;
                options.bounds = bounds;
                options.mass = mass;
                options.bodyNames = bodyNames;
                options.contacts = contacts;
                options.graphColors = graphColors;
                options.contactNormals = contactNormals;
                options.contactImpulses = contactImpulses;
                options.contactFeatures = contactFeatures;
                options.frictionImpulses = frictionImpulses;
                options.islands = islands;

                self.debugDraw(color, options);
            },
            "color"_a = Color::RED, "filled_shapes"_a = false, "shapes"_a = true, "joints"_a = true,
            "joint_extras"_a = true, "bounds"_a = false, "mass"_a = false, "body_names"_a = false,
            "contacts"_a = false, "graph_colors"_a = false, "contact_normals"_a = false,
            "contact_impulses"_a = false, "contact_features"_a = false,
            "friction_impulses"_a = false, "islands"_a = false, R"doc(
Draw physics debug geometry.
            )doc"
        )

        .def("from_map_layer", &World::fromMapLayer, "layer"_a, R"doc(
Create a single StaticBody from a TileMap ObjectGroup layer.

This method iterates through all rectangular and polygonal objects in the
specified layer and adds them as colliders to a new StaticBody. Points,
lines, and ellipses are discarded.

Args:
    layer (Layer): The TileMap ObjectGroup layer.

Returns:
    StaticBody: The created static body with all shapes attached.

Raises:
    TypeError: If the layer is not an ObjectGroup.
            )doc")

        .def(
            "add_fixed_update",
            [](World& self, nb::typed<nb::callable, void(float)> callback) -> void
            {
                if (nb::hasattr(callback, "__self__") &&
                    !nb::getattr(callback, "__self__").is_none())
                {
                    nb::object selfObj = callback.attr("__self__");
                    nb::object weakOwner = nb::weakref(selfObj);
                    nb::object unboundMethod = callback.attr("__func__");
                    self.addFixedUpdate(
                        [weakOwner, unboundMethod](float delta)
                        {
                            nb::object owner = weakOwner();
                            if (!owner.is_none())
                                unboundMethod(owner, delta);
                        }
                    );
                }
                else
                {
                    // For lambdas and regular functions, keep a strong reference
                    self.addFixedUpdate([callback](float delta) { callback(delta); });
                }
            },
            "callback"_a,
            R"doc(Add a callback function to be executed during each physics step.)doc"
        )
        .def(
            "fixed_callback",
            [](World& self,
               nb::typed<nb::callable, void(float)> func) -> nb::typed<nb::callable, void(float)>
            {
                if (nb::hasattr(func, "__self__") && !nb::getattr(func, "__self__").is_none())
                {
                    nb::object selfObj = func.attr("__self__");
                    nb::object weakOwner = nb::weakref(selfObj);
                    nb::object unboundMethod = func.attr("__func__");
                    self.addFixedUpdate(
                        [weakOwner, unboundMethod](float delta)
                        {
                            nb::object owner = weakOwner();
                            if (!owner.is_none())
                                unboundMethod(owner, delta);
                        }
                    );
                }
                else
                {
                    self.addFixedUpdate([func](float delta) { func(delta); });
                }
                return func;
            },
            "callback"_a,
            R"doc(A decorator to register a function as a physics update callback.)doc"
        )
        .def(
            "clear_fixed_updates", &World::clearFixedUpdates,
            R"doc(Remove all registered fixed update callbacks.)doc"
        )

        .def(
            "create_distance_joint", &World::createDistanceJoint, "body_a"_a, "body_b"_a,
            "anchor_a"_a, "anchor_b"_a, R"doc(
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
            "create_filter_joint", &World::createFilterJoint, "body_a"_a, "body_b"_a,
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
            "create_motor_joint", &World::createMotorJoint, "body_a"_a, "body_b"_a,
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
            "create_mouse_joint", &World::createMouseJoint, "ground_body"_a, "pulled_body"_a,
            "target"_a, R"doc(
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
            "create_prismatic_joint", &World::createPrismaticJoint, "body_a"_a, "body_b"_a,
            "anchor"_a, "axis"_a, R"doc(
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
            "create_revolute_joint", &World::createRevoluteJoint, "body_a"_a, "body_b"_a,
            "anchor"_a, R"doc(
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
            "create_weld_joint", &World::createWeldJoint, "body_a"_a, "body_b"_a, "anchor"_a,
            R"doc(
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
            "create_wheel_joint", &World::createWheelJoint, "body_a"_a, "body_b"_a, "anchor"_a,
            "axis"_a, R"doc(
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
        .def("query_point", &World::queryPoint, "point"_a, R"doc(
Find all bodies that contain the specified point.

Args:
    point (Vec2): The point to query in world coordinates.

Returns:
    list[Body]: A list of bodies at the point.
        )doc")
        .def("query_aabb", &World::queryAABB, "rect"_a, R"doc(
Find all bodies that overlap with the specified rectangular area.

Args:
    rect (Rect): The rectangular area to query.

Returns:
    list[Body]: A list of bodies overlapping the area.
        )doc")
        .def("ray_cast", &World::rayCast, "origin"_a, "translation"_a, R"doc(
Cast a ray into the world and find all bodies that intersect it.

Args:
    origin (Vec2): The starting point of the ray.
    translation (Vec2): The direction and length of the ray.

Returns:
    list[RayCastHit]: A list of hits, sorted by distance (fraction).
        )doc")
        .def(
            "shape_cast",
            nb::overload_cast<const Circle&, const Transform&, const Vec2&>(&World::shapeCast),
            "circle"_a, "transform"_a, "translation"_a, R"doc(
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
            nb::overload_cast<const Capsule&, const Transform&, const Vec2&>(&World::shapeCast),
            "capsule"_a, "transform"_a, "translation"_a, R"doc(
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
            nb::overload_cast<const Polygon&, const Transform&, const Vec2&>(&World::shapeCast),
            "polygon"_a, "transform"_a, "translation"_a, R"doc(
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
            nb::overload_cast<const Rect&, const Transform&, const Vec2&>(&World::shapeCast),
            "rect"_a, "transform"_a, "translation"_a, R"doc(
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
