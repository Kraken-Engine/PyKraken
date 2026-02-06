"""
Physics engine related classes and functions
"""
from __future__ import annotations
import enum
import pykraken._core
import typing
__all__: list[str] = ['Body', 'BodyType', 'World']
class Body:
    @typing.overload
    def add_collider(self, circle: pykraken._core.Circle, density: typing.SupportsFloat = 1.0, friction: typing.SupportsFloat = 0.20000000298023224, restitution: typing.SupportsFloat = 0.0) -> None:
        """
        Add a circular collider to the body.
        
        Args:
            circle (Circle): The circular shape to add as a collider.
            density (float, optional): The density of the collider. Defaults to 1.0.
            friction (float, optional): The friction coefficient of the collider. Defaults to 0.2.
            restitution (float, optional): The restitution (bounciness) of the collider. Defaults to 0.0.
        """
    @typing.overload
    def add_collider(self, polygon: pykraken._core.Polygon, density: typing.SupportsFloat = 1.0, friction: typing.SupportsFloat = 0.20000000298023224, restitution: typing.SupportsFloat = 0.0) -> None:
        """
        Add a polygonal collider to the body.
        
        Args:
            polygon (Polygon): The polygonal shape to add as a collider.
            density (float, optional): The density of the collider. Defaults to 1.0.
            friction (float, optional): The friction coefficient of the collider. Defaults to 0.2.
            restitution (float, optional): The restitution (bounciness) of the collider. Defaults to 0.0.
        """
    @typing.overload
    def add_collider(self, rect: pykraken._core.Rect, density: typing.SupportsFloat = 1.0, friction: typing.SupportsFloat = 0.20000000298023224, restitution: typing.SupportsFloat = 0.0) -> None:
        """
        Add a rectangular collider to the body.
        
        Args:
            rect (Rect): The rectangular shape to add as a collider.
            density (float, optional): The density of the collider. Defaults to 1.0.
            friction (float, optional): The friction coefficient of the collider. Defaults to 0.2.
            restitution (float, optional): The restitution (bounciness) of the collider. Defaults to 0.0.
        """
    def apply_angular_impulse(self, impulse: typing.SupportsFloat, wake: bool = True) -> None:
        """
        Apply an angular impulse to the body.
        
        Args:
            impulse (float): The angular impulse to apply.
            wake (bool, optional): Whether to wake the body if it's sleeping. Defaults to True
        """
    def apply_force(self, force: pykraken._core.Vec2, point: pykraken._core.Vec2, wake: bool = True) -> None:
        """
        Apply a force to the body at a specific point.
        
        Args:
            force (Vec2): The force vector to apply.
            point (Vec2): The point (in world coordinates) where the force is applied.
            wake (bool, optional): Whether to wake the body if it's sleeping. Defaults to True
        """
    def apply_force_to_center(self, force: pykraken._core.Vec2, wake: bool = True) -> None:
        """
        Apply a force to the center of mass of the body.
        
        Args:
            force (Vec2): The force vector to apply.
            wake (bool, optional): Whether to wake the body if it's sleeping. Defaults to True
        """
    def apply_linear_impulse(self, impulse: pykraken._core.Vec2, point: pykraken._core.Vec2, wake: bool = True) -> None:
        """
        Apply a linear impulse to the body at a specific point.
        
        Args:
            impulse (Vec2): The impulse vector to apply.
            point (Vec2): The point (in world coordinates) where the impulse is applied.
            wake (bool, optional): Whether to wake the body if it's sleeping. Defaults to True
        """
    def apply_linear_impulse_to_center(self, impulse: pykraken._core.Vec2, wake: bool = True) -> None:
        """
        Apply a linear impulse to the center of mass of the body.
        
        Args:
            impulse (Vec2): The impulse vector to apply.
            wake (bool, optional): Whether to wake the body if it's sleeping. Defaults to True
        """
    def apply_torque(self, torque: typing.SupportsFloat, wake: bool = True) -> None:
        """
        Apply a torque to the body.
        
        Args:
            torque (float): The torque to apply.
            wake (bool, optional): Whether to wake the body if it's sleeping. Defaults to True
        """
    def draw(self, color: pykraken._core.Color) -> None:
        """
        Draw all colliders attached to the body.
        
        Args:
            color (Color): The color to draw the colliders with.
        """
    @property
    def angular_velocity(self) -> float:
        """
        The angular velocity of the body.
        """
    @angular_velocity.setter
    def angular_velocity(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def is_valid(self) -> bool:
        """
        Indicates whether the body is not destroyed.
        """
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
    def destroy_body(self, body: Body) -> None:
        """
        Destroy a body in the world.
        
        Args:
            body (Body): The body instance to destroy.
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
