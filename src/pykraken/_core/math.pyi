"""
Math related functions
"""
from __future__ import annotations
import pykraken._core
import typing
__all__ = ['angle_between', 'clamp', 'cross', 'dot', 'from_polar', 'lerp', 'normalize', 'remap', 'scale_to_length', 'to_deg', 'to_rad']
def angle_between(a: pykraken._core.Vec2, b: pykraken._core.Vec2) -> float:
    """
    Calculate the angle between two vectors.
    
    Args:
        a (Vec2): The first vector.
        b (Vec2): The second vector.
    
    Returns:
        float: The angle between the vectors in radians [0, π].
    """
@typing.overload
def clamp(vec: pykraken._core.Vec2, min_vec: pykraken._core.Vec2, max_vec: pykraken._core.Vec2) -> pykraken._core.Vec2:
    """
    Clamp a vector between two boundary vectors.
    
    Args:
        vector (Vec2): The vector to clamp.
        min_vec (Vec2): The minimum boundary vector.
        max_vec (Vec2): The maximum boundary vector.
    
    Returns:
        Vec2: A new vector with components clamped between min and max.
    """
@typing.overload
def clamp(value: float, min_val: float, max_val: float) -> float:
    """
    Clamp a value between two boundaries.
    
    Args:
        value (float): The value to clamp.
        min_val (float): The minimum boundary.
        max_val (float): The maximum boundary.
    
    Returns:
        float: The clamped value.
    """
def cross(a: pykraken._core.Vec2, b: pykraken._core.Vec2) -> float:
    """
    Calculate the 2D cross product of two vectors.
    
    Args:
        a (Vec2): The first vector.
        b (Vec2): The second vector.
    
    Returns:
        float: The 2D cross product (a.x * b.y - a.y * b.x).
    """
def dot(a: pykraken._core.Vec2, b: pykraken._core.Vec2) -> float:
    """
    Calculate the dot product of two vectors.
    
    Args:
        a (Vec2): The first vector.
        b (Vec2): The second vector.
    
    Returns:
        float: The dot product (a.x * b.x + a.y * b.y).
    """
@typing.overload
def from_polar(angle: float, radius: float) -> pykraken._core.Vec2:
    """
    Convert polar coordinates to a Cartesian vector.
    
    Args:
        angle (float): The angle in radians.
        radius (float): The radius/distance from origin.
    
    Returns:
        Vec2: The equivalent Cartesian vector.
    """
@typing.overload
def from_polar(polar: pykraken._core.PolarCoordinate) -> pykraken._core.Vec2:
    """
    Convert a PolarCoordinate object to a Cartesian vector.
    
    Args:
        polar (PolarCoordinate): The polar coordinate to convert.
    
    Returns:
        Vec2: The equivalent Cartesian vector.
    """
@typing.overload
def lerp(a: pykraken._core.Vec2, b: pykraken._core.Vec2, t: float) -> pykraken._core.Vec2:
    """
    Linearly interpolate between two Vec2s.
    
    Args:
        a (Vec2): The start vector.
        b (Vec2): The end vector.
        t (float): The interpolation factor [0.0, 1.0].
    
    Returns:
        Vec2: The interpolated vector.
    """
@typing.overload
def lerp(a: float, b: float, t: float) -> float:
    """
    Linearly interpolate between two values.
    
    Args:
        a (float): The start value.
        b (float): The end value.
        t (float): The interpolation factor [0.0, 1.0].
    
    Returns:
        float: The interpolated value.
    """
def normalize(vec: pykraken._core.Vec2) -> pykraken._core.Vec2:
    """
    Normalize a vector to unit length.
    
    Args:
        vector (Vec2): The input vector.
    
    Returns:
        Vec2: A new normalized vector.
    """
def remap(in_min: float, in_max: float, out_min: float, out_max: float, value: float) -> float:
    """
    Remap a value from one range to another.
    
    Args:
        in_min (float): Input range minimum.
        in_max (float): Input range maximum.
        out_min (float): Output range minimum.
        out_max (float): Output range maximum.
        value (float): The value to remap.
    
    Returns:
        float: The remapped value in the output range.
    
    Raises:
        ValueError: If in_min equals in_max.
    """
def scale_to_length(vector: pykraken._core.Vec2, length: float) -> pykraken._core.Vec2:
    """
    Scale a vector to a given length.
    
    Args:
        vector (Vec2): The input vector.
        length (float): The target length.
    
    Returns:
        Vec2: A new vector scaled to the specified length.
    """
def to_deg(radians: float) -> float:
    """
    Convert radians to degrees.
    
    Args:
        radians (float): The angle in radians.
    
    Returns:
        float: The angle in degrees.
    """
def to_rad(degrees: float) -> float:
    """
    Convert degrees to radians.
    
    Args:
        degrees (float): The angle in degrees.
    
    Returns:
        float: The angle in radians.
    """
