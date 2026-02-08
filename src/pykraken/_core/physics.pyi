"""
Physics engine related classes and functions
"""
from __future__ import annotations
import enum
import pykraken._core
import typing
__all__: list[str] = ['Body', 'BodyType', 'Collision', 'DistanceJoint', 'FilterJoint', 'Joint', 'MotorJoint', 'PrismaticJoint', 'RevoluteJoint', 'TargetJoint', 'WeldJoint', 'WheelJoint', 'World']
class Body:
    __hash__: typing.ClassVar[None] = None
    def __eq__(self, arg0: Body) -> bool:
        ...
    def __ne__(self, arg0: Body) -> bool:
        ...
    @typing.overload
    def add_collider(self, circle: pykraken._core.Circle, density: typing.SupportsFloat = 1.0, friction: typing.SupportsFloat = 0.20000000298023224, restitution: typing.SupportsFloat = 0.0, enable_events: bool = False, is_sensor: bool = False) -> None:
        """
        Add a circular collider to the body.
        
        Args:
            circle (Circle): The circular shape to add as a collider.
            density (float, optional): The density of the collider. Defaults to 1.0.
            friction (float, optional): The friction coefficient of the collider. Defaults to 0.2.
            restitution (float, optional): The restitution (bounciness) of the collider. Defaults to 0.0.
            enable_events (bool, optional): Whether to enable hit events for this collider. Defaults to False.
            is_sensor (bool, optional): Whether the collider is a sensor. Defaults to False.
        """
    @typing.overload
    def add_collider(self, polygon: pykraken._core.Polygon, density: typing.SupportsFloat = 1.0, friction: typing.SupportsFloat = 0.20000000298023224, restitution: typing.SupportsFloat = 0.0, enable_events: bool = False, is_sensor: bool = False) -> None:
        """
        Add a polygonal collider to the body.
        
        Args:
            polygon (Polygon): The polygonal shape to add as a collider.
            density (float, optional): The density of the collider. Defaults to 1.0.
            friction (float, optional): The friction coefficient of the collider. Defaults to 0.2.
            restitution (float, optional): The restitution (bounciness) of the collider. Defaults to 0.0.
            enable_events (bool, optional): Whether to enable hit events for this collider. Defaults to False.
            is_sensor (bool, optional): Whether the collider is a sensor. Defaults to False.
        """
    @typing.overload
    def add_collider(self, rect: pykraken._core.Rect, density: typing.SupportsFloat = 1.0, friction: typing.SupportsFloat = 0.20000000298023224, restitution: typing.SupportsFloat = 0.0, enable_events: bool = False, is_sensor: bool = False) -> None:
        """
        Add a rectangular collider to the body.
        
        Args:
            rect (Rect): The rectangular shape to add as a collider.
            density (float, optional): The density of the collider. Defaults to 1.0.
            friction (float, optional): The friction coefficient of the collider. Defaults to 0.2.
            restitution (float, optional): The restitution (bounciness) of the collider. Defaults to 0.0.
            enable_events (bool, optional): Whether to enable hit events for this collider. Defaults to False.
            is_sensor (bool, optional): Whether the collider is a sensor. Defaults to False.
        """
    def apply_angular_impulse(self, impulse: typing.SupportsFloat, wake: bool = True) -> None:
        """
        Apply an angular impulse to the body.
        
        Args:
            impulse (float): The angular impulse to apply.
            wake (bool, optional): Whether to wake the body if it's sleeping. Defaults to True.
        """
    def apply_force(self, force: pykraken._core.Vec2, point: pykraken._core.Vec2, wake: bool = True) -> None:
        """
        Apply a force to the body at a specific point.
        
        Args:
            force (Vec2): The force vector to apply.
            point (Vec2): The point (in world coordinates) where the force is applied.
            wake (bool, optional): Whether to wake the body if it's sleeping. Defaults to True.
        """
    def apply_force_to_center(self, force: pykraken._core.Vec2, wake: bool = True) -> None:
        """
        Apply a force to the center of mass of the body.
        
        Args:
            force (Vec2): The force vector to apply.
            wake (bool, optional): Whether to wake the body if it's sleeping. Defaults to True.
        """
    def apply_linear_impulse(self, impulse: pykraken._core.Vec2, point: pykraken._core.Vec2, wake: bool = True) -> None:
        """
        Apply a linear impulse to the body at a specific point.
        
        Args:
            impulse (Vec2): The impulse vector to apply.
            point (Vec2): The point (in world coordinates) where the impulse is applied.
            wake (bool, optional): Whether to wake the body if it's sleeping. Defaults to True.
        """
    def apply_linear_impulse_to_center(self, impulse: pykraken._core.Vec2, wake: bool = True) -> None:
        """
        Apply a linear impulse to the center of mass of the body.
        
        Args:
            impulse (Vec2): The impulse vector to apply.
            wake (bool, optional): Whether to wake the body if it's sleeping. Defaults to True.
        """
    def apply_torque(self, torque: typing.SupportsFloat, wake: bool = True) -> None:
        """
        Apply a torque to the body.
        
        Args:
            torque (float): The torque to apply.
            wake (bool, optional): Whether to wake the body if it's sleeping. Defaults to True.
        """
    def destroy(self) -> None:
        """
        Destroy the body manually.
        """
    def draw(self, color: pykraken._core.Color) -> None:
        """
        Draw all colliders attached to the body.
        
        Args:
            color (Color): The color to draw the colliders with.
        """
    def wake(self) -> None:
        """
        Manually wake the body from sleep.
        """
    @property
    def angular_damping(self) -> float:
        """
        The angular damping of the body.
        """
    @angular_damping.setter
    def angular_damping(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def angular_velocity(self) -> float:
        """
        The angular velocity of the body.
        """
    @angular_velocity.setter
    def angular_velocity(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def awake(self) -> bool:
        """
        Whether the body is currently awake and participating in simulation.
        """
    @property
    def fixed_rotation(self) -> bool:
        """
        Whether the body has fixed rotation.
        """
    @fixed_rotation.setter
    def fixed_rotation(self, arg1: bool) -> None:
        ...
    @property
    def is_valid(self) -> bool:
        """
        Indicates whether the body is not destroyed.
        """
    @property
    def linear_damping(self) -> float:
        """
        The linear damping of the body.
        """
    @linear_damping.setter
    def linear_damping(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def linear_velocity(self) -> pykraken._core.Vec2:
        """
        The linear velocity of the body.
        """
    @linear_velocity.setter
    def linear_velocity(self, arg1: pykraken._core.Vec2) -> None:
        ...
    @property
    def mass(self) -> float:
        """
        The mass of the body.
        """
    @property
    def pos(self) -> pykraken._core.Vec2:
        """
        The position of the body in world coordinates.
        """
    @pos.setter
    def pos(self, arg1: pykraken._core.Vec2) -> None:
        ...
    @property
    def rotation(self) -> float:
        """
        The rotation of the body in radians.
        """
    @rotation.setter
    def rotation(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def type(self) -> BodyType:
        """
        The simulation type of the body.
        """
    @type.setter
    def type(self, arg1: BodyType) -> None:
        ...
class BodyType(enum.IntEnum):
    """
    Body simulation types in the physics engine.
    """
    DYNAMIC: typing.ClassVar[BodyType]  # value = <BodyType.DYNAMIC: 2>
    KINEMATIC: typing.ClassVar[BodyType]  # value = <BodyType.KINEMATIC: 1>
    STATIC: typing.ClassVar[BodyType]  # value = <BodyType.STATIC: 0>
    @classmethod
    def __new__(cls, value):
        ...
    def __format__(self, format_spec):
        """
        Convert to a string according to format_spec.
        """
class Collision:
    @property
    def approach_speed(self) -> float:
        """
        The speed at which the bodies approached each other.
        """
    @property
    def body_a(self) -> Body:
        """
        The first body involved in the collision.
        """
    @property
    def body_b(self) -> Body:
        """
        The second body involved in the collision.
        """
    @property
    def normal(self) -> pykraken._core.Vec2:
        """
        The normal vector of the collision.
        """
    @property
    def point(self) -> pykraken._core.Vec2:
        """
        The point of impact in world coordinates.
        """
class DistanceJoint(Joint):
    def set_length_range(self, min_length: typing.SupportsFloat, max_length: typing.SupportsFloat) -> None:
        """
        Set the minimum and maximum length limits.
        
        Args:
            min_length (float): The minimum length.
            max_length (float): The maximum length.
        """
    @property
    def current_length(self) -> float:
        """
        The current length between the anchors.
        """
    @property
    def length(self) -> float:
        """
        The rest length of the joint.
        """
    @length.setter
    def length(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def limit_enabled(self) -> bool:
        """
        Whether the length limits are enabled.
        """
    @limit_enabled.setter
    def limit_enabled(self, arg1: bool) -> None:
        ...
    @property
    def max_length(self) -> float:
        """
        The maximum length limit.
        """
    @property
    def max_motor_force(self) -> float:
        """
        The maximum motor force.
        """
    @max_motor_force.setter
    def max_motor_force(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def min_length(self) -> float:
        """
        The minimum length limit.
        """
    @property
    def motor_enabled(self) -> bool:
        """
        Whether the motor is enabled.
        """
    @motor_enabled.setter
    def motor_enabled(self, arg1: bool) -> None:
        ...
    @property
    def motor_force(self) -> float:
        """
        The current motor force.
        """
    @property
    def motor_speed(self) -> float:
        """
        The target motor speed.
        """
    @motor_speed.setter
    def motor_speed(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def spring_damping_ratio(self) -> float:
        """
        The spring damping ratio.
        """
    @spring_damping_ratio.setter
    def spring_damping_ratio(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def spring_enabled(self) -> bool:
        """
        Whether the spring is enabled.
        """
    @spring_enabled.setter
    def spring_enabled(self, arg1: bool) -> None:
        ...
    @property
    def spring_hz(self) -> float:
        """
        The spring frequency in Hertz.
        """
    @spring_hz.setter
    def spring_hz(self, arg1: typing.SupportsFloat) -> None:
        ...
class FilterJoint(Joint):
    """
    A joint used to filter collisions between two bodies.
    """
class Joint:
    def destroy(self) -> None:
        """
        Destroy the joint manually.
        """
    @property
    def body_a(self) -> Body:
        """
        The first body attached to the joint.
        """
    @property
    def body_b(self) -> Body:
        """
        The second body attached to the joint.
        """
    @property
    def collide_connected(self) -> bool:
        """
        Whether the connected bodies should collide with each other.
        """
    @collide_connected.setter
    def collide_connected(self, arg1: bool) -> None:
        ...
    @property
    def is_valid(self) -> bool:
        """
        Indicates whether the joint is not destroyed.
        """
    @property
    def local_anchor_a(self) -> pykraken._core.Vec2:
        """
        The local anchor point relative to body A's origin.
        """
    @local_anchor_a.setter
    def local_anchor_a(self, arg1: pykraken._core.Vec2) -> None:
        ...
    @property
    def local_anchor_b(self) -> pykraken._core.Vec2:
        """
        The local anchor point relative to body B's origin.
        """
    @local_anchor_b.setter
    def local_anchor_b(self, arg1: pykraken._core.Vec2) -> None:
        ...
class MotorJoint(Joint):
    @property
    def angular_offset(self) -> float:
        """
        The target angular offset in radians.
        """
    @angular_offset.setter
    def angular_offset(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def correction_factor(self) -> float:
        """
        The position correction factor in [0, 1].
        """
    @correction_factor.setter
    def correction_factor(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def linear_offset(self) -> pykraken._core.Vec2:
        """
        The target linear offset from body A to body B.
        """
    @linear_offset.setter
    def linear_offset(self, arg1: pykraken._core.Vec2) -> None:
        ...
    @property
    def max_force(self) -> float:
        """
        The maximum motor force.
        """
    @max_force.setter
    def max_force(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def max_torque(self) -> float:
        """
        The maximum motor torque.
        """
    @max_torque.setter
    def max_torque(self, arg1: typing.SupportsFloat) -> None:
        ...
class PrismaticJoint(Joint):
    def set_limits(self, lower: typing.SupportsFloat, upper: typing.SupportsFloat) -> None:
        """
        Set the translation limits.
        
        Args:
            lower (float): The lower translation limit.
            upper (float): The upper translation limit.
        """
    @property
    def limit_enabled(self) -> bool:
        """
        Whether the translation limits are enabled.
        """
    @limit_enabled.setter
    def limit_enabled(self, arg1: bool) -> None:
        ...
    @property
    def lower_limit(self) -> float:
        """
        The lower translation limit.
        """
    @property
    def max_motor_force(self) -> float:
        """
        The maximum motor force.
        """
    @max_motor_force.setter
    def max_motor_force(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def motor_enabled(self) -> bool:
        """
        Whether the motor is enabled.
        """
    @motor_enabled.setter
    def motor_enabled(self, arg1: bool) -> None:
        ...
    @property
    def motor_force(self) -> float:
        """
        The current motor force.
        """
    @property
    def motor_speed(self) -> float:
        """
        The target motor speed.
        """
    @motor_speed.setter
    def motor_speed(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def speed(self) -> float:
        """
        The current joint translation speed.
        """
    @property
    def spring_damping_ratio(self) -> float:
        """
        The spring damping ratio.
        """
    @spring_damping_ratio.setter
    def spring_damping_ratio(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def spring_enabled(self) -> bool:
        """
        Whether the spring is enabled.
        """
    @spring_enabled.setter
    def spring_enabled(self, arg1: bool) -> None:
        ...
    @property
    def spring_hz(self) -> float:
        """
        The spring frequency in Hertz.
        """
    @spring_hz.setter
    def spring_hz(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def target_translation(self) -> float:
        """
        The target translation for the motor.
        """
    @target_translation.setter
    def target_translation(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def translation(self) -> float:
        """
        The current joint translation.
        """
    @property
    def upper_limit(self) -> float:
        """
        The upper translation limit.
        """
class RevoluteJoint(Joint):
    def set_limits(self, lower: typing.SupportsFloat, upper: typing.SupportsFloat) -> None:
        """
        Set the angle limits.
        
        Args:
            lower (float): The lower angle limit in radians.
            upper (float): The upper angle limit in radians.
        """
    @property
    def angle(self) -> float:
        """
        The current joint angle in radians.
        """
    @property
    def limit_enabled(self) -> bool:
        """
        Whether the angle limits are enabled.
        """
    @limit_enabled.setter
    def limit_enabled(self, arg1: bool) -> None:
        ...
    @property
    def lower_limit(self) -> float:
        """
        The lower angle limit in radians.
        """
    @property
    def max_motor_torque(self) -> float:
        """
        The maximum motor torque.
        """
    @max_motor_torque.setter
    def max_motor_torque(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def motor_enabled(self) -> bool:
        """
        Whether the motor is enabled.
        """
    @motor_enabled.setter
    def motor_enabled(self, arg1: bool) -> None:
        ...
    @property
    def motor_speed(self) -> float:
        """
        The target motor speed in radians per second.
        """
    @motor_speed.setter
    def motor_speed(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def motor_torque(self) -> float:
        """
        The current motor torque.
        """
    @property
    def spring_damping_ratio(self) -> float:
        """
        The spring damping ratio.
        """
    @spring_damping_ratio.setter
    def spring_damping_ratio(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def spring_enabled(self) -> bool:
        """
        Whether the spring is enabled.
        """
    @spring_enabled.setter
    def spring_enabled(self, arg1: bool) -> None:
        ...
    @property
    def spring_hz(self) -> float:
        """
        The spring frequency in Hertz.
        """
    @spring_hz.setter
    def spring_hz(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def target_angle(self) -> float:
        """
        The target angle for the motor in radians.
        """
    @target_angle.setter
    def target_angle(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def upper_limit(self) -> float:
        """
        The upper angle limit in radians.
        """
class TargetJoint(Joint):
    @property
    def max_force(self) -> float:
        """
        The maximum constraint force.
        """
    @max_force.setter
    def max_force(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def spring_damping_ratio(self) -> float:
        """
        The spring damping ratio.
        """
    @spring_damping_ratio.setter
    def spring_damping_ratio(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def spring_hz(self) -> float:
        """
        The spring frequency in Hertz.
        """
    @spring_hz.setter
    def spring_hz(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def target(self) -> pykraken._core.Vec2:
        """
        The target point in world coordinates.
        """
    @target.setter
    def target(self, arg1: pykraken._core.Vec2) -> None:
        ...
class WeldJoint(Joint):
    @property
    def angular_damping_ratio(self) -> float:
        """
        The angular spring damping ratio.
        """
    @angular_damping_ratio.setter
    def angular_damping_ratio(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def angular_hz(self) -> float:
        """
        The angular spring frequency in Hertz.
        """
    @angular_hz.setter
    def angular_hz(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def linear_damping_ratio(self) -> float:
        """
        The linear spring damping ratio.
        """
    @linear_damping_ratio.setter
    def linear_damping_ratio(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def linear_hz(self) -> float:
        """
        The linear spring frequency in Hertz.
        """
    @linear_hz.setter
    def linear_hz(self, arg1: typing.SupportsFloat) -> None:
        ...
class WheelJoint(Joint):
    def set_limits(self, lower: typing.SupportsFloat, upper: typing.SupportsFloat) -> None:
        """
        Set the translation limits.
        
        Args:
            lower (float): The lower translation limit.
            upper (float): The upper translation limit.
        """
    @property
    def limit_enabled(self) -> bool:
        """
        Whether the translation limits are enabled.
        """
    @limit_enabled.setter
    def limit_enabled(self, arg1: bool) -> None:
        ...
    @property
    def lower_limit(self) -> float:
        """
        The lower translation limit.
        """
    @property
    def max_motor_torque(self) -> float:
        """
        The maximum motor torque.
        """
    @max_motor_torque.setter
    def max_motor_torque(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def motor_enabled(self) -> bool:
        """
        Whether the motor is enabled.
        """
    @motor_enabled.setter
    def motor_enabled(self, arg1: bool) -> None:
        ...
    @property
    def motor_speed(self) -> float:
        """
        The target motor speed in radians per second.
        """
    @motor_speed.setter
    def motor_speed(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def motor_torque(self) -> float:
        """
        The current motor torque.
        """
    @property
    def spring_damping_ratio(self) -> float:
        """
        The spring damping ratio.
        """
    @spring_damping_ratio.setter
    def spring_damping_ratio(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def spring_enabled(self) -> bool:
        """
        Whether the spring is enabled.
        """
    @spring_enabled.setter
    def spring_enabled(self, arg1: bool) -> None:
        ...
    @property
    def spring_hz(self) -> float:
        """
        The spring frequency in Hertz.
        """
    @spring_hz.setter
    def spring_hz(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def upper_limit(self) -> float:
        """
        The upper translation limit.
        """
class World:
    def __init__(self, gravity: pykraken._core.Vec2) -> None:
        """
        Create a new physics world with the specified gravity.
        
        Args:
            gravity (Vec2): The gravity vector for the world.
        """
    def create_body(self, type: BodyType) -> Body:
        """
        Create a new body in the world.
        
        Args:
            type (BodyType): The simulation type of the body to create.
        
        Returns:
            Body: The created body instance.
        """
    def create_distance_joint(self, body_a: Body, body_b: Body, anchor_a: pykraken._core.Vec2, anchor_b: pykraken._core.Vec2) -> DistanceJoint:
        """
        Create a distance joint between two bodies.
        
        Args:
            body_a (Body): The first body.
            body_b (Body): The second body.
            anchor_a (Vec2): The anchor point on the first body in world coordinates.
            anchor_b (Vec2): The anchor point on the second body in world coordinates.
        
        Returns:
            DistanceJoint: The created joint.
        """
    def create_filter_joint(self, body_a: Body, body_b: Body) -> FilterJoint:
        """
        Create a filter joint between two bodies to disable collision.
        
        Args:
            body_a (Body): The first body.
            body_b (Body): The second body.
        
        Returns:
            FilterJoint: The created joint.
        """
    def create_motor_joint(self, body_a: Body, body_b: Body) -> MotorJoint:
        """
        Create a motor joint between two bodies.
        
        Args:
            body_a (Body): The first body.
            body_b (Body): The second body.
        
        Returns:
            MotorJoint: The created joint.
        """
    def create_prismatic_joint(self, body_a: Body, body_b: Body, anchor: pykraken._core.Vec2, axis: pykraken._core.Vec2) -> PrismaticJoint:
        """
        Create a prismatic joint between two bodies.
        
        Args:
            body_a (Body): The first body.
            body_b (Body): The second body.
            anchor (Vec2): The anchor point in world coordinates.
            axis (Vec2): The axis of movement in world coordinates.
        
        Returns:
            PrismaticJoint: The created joint.
        """
    def create_revolute_joint(self, body_a: Body, body_b: Body, anchor: pykraken._core.Vec2) -> RevoluteJoint:
        """
        Create a revolute joint between two bodies.
        
        Args:
            body_a (Body): The first body.
            body_b (Body): The second body.
            anchor (Vec2): The anchor point in world coordinates.
        
        Returns:
            RevoluteJoint: The created joint.
        """
    def create_target_joint(self, ground_body: Body, pulled_body: Body, target: pykraken._core.Vec2) -> TargetJoint:
        """
        Create a target joint between a ground body and a target body.
        
        Args:
            ground_body (Body): The ground body (usually a static body).
            pulled_body (Body): The body to be pulled and moved to the target.
            target (Vec2): The initial target point in world coordinates.
        
        Returns:
            TargetJoint: The created joint.
        """
    def create_weld_joint(self, body_a: Body, body_b: Body, anchor: pykraken._core.Vec2) -> WeldJoint:
        """
        Create a weld joint between two bodies.
        
        Args:
            body_a (Body): The first body.
            body_b (Body): The second body.
            anchor (Vec2): The anchor point in world coordinates.
        
        Returns:
            WeldJoint: The created joint.
        """
    def create_wheel_joint(self, body_a: Body, body_b: Body, anchor: pykraken._core.Vec2, axis: pykraken._core.Vec2) -> WheelJoint:
        """
        Create a wheel joint between two bodies.
        
        Args:
            body_a (Body): The first body.
            body_b (Body): The second body.
            anchor (Vec2): The anchor point in world coordinates.
            axis (Vec2): The axis of movement in world coordinates.
        
        Returns:
            WheelJoint: The created joint.
        """
    def get_collisions(self) -> list[Collision]:
        """
        Get all collision events that occurred during the last physics step.
        
        Note:
            This only includes hit events. The list is cleared after each call.
        
        Returns:
            list[Collision]: A list of collision events.
        """
    def query_aabb(self, rect: pykraken._core.Rect) -> list[Body]:
        """
        Find all bodies that overlap with the specified rectangular area.
        
        Args:
            rect (Rect): The rectangular area to query.
        
        Returns:
            list[Body]: A list of bodies overlapping the area.
        """
    def query_point(self, point: pykraken._core.Vec2) -> list[Body]:
        """
        Find all bodies that contain the specified point.
        
        Args:
            point (Vec2): The point to query in world coordinates.
        
        Returns:
            list[Body]: A list of bodies at the point.
        """
    def step(self, time_step: typing.SupportsFloat, sub_step_count: typing.SupportsInt) -> None:
        """
        Advance the physics simulation by a time step.
        
        Args:
            time_step (float): The time step to advance the simulation.
            sub_step_count (int): The number of sub steps to take.
        """
    @property
    def gravity(self) -> pykraken._core.Vec2:
        """
        The gravity vector of the world.
        """
    @gravity.setter
    def gravity(self, arg1: pykraken._core.Vec2) -> None:
        ...
    @property
    def is_valid(self) -> bool:
        """
        Indicates whether the world is not destroyed.
        """
