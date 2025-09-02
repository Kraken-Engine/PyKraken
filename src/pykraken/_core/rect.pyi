"""
Rectangle related functions
"""
from __future__ import annotations
import pykraken._core
import typing
__all__: list[str] = ['clamp', 'move', 'scale_by', 'scale_to']
@typing.overload
def clamp(rect: pykraken._core.Rect, min: pykraken._core.Vec2, max: pykraken._core.Vec2) -> pykraken._core.Rect:
    """
    Clamp a rectangle to be within the specified bounds.
    
    Args:
        rect (Rect): The rectangle to clamp.
        min (Vec2): The minimum bounds as (min_x, min_y).
        max (Vec2): The maximum bounds as (max_x, max_y).
    
    Returns:
        Rect: A new rectangle clamped within the bounds.
    
    Raises:
        ValueError: If min >= max or rectangle is larger than the clamp area.
    """
@typing.overload
def clamp(rect: pykraken._core.Rect, other: pykraken._core.Rect) -> pykraken._core.Rect:
    """
    Clamp a rectangle to be within another rectangle.
    
    Args:
        rect (Rect): The rectangle to clamp.
        other (Rect): The rectangle to clamp within.
    
    Returns:
        Rect: A new rectangle clamped within the other rectangle.
    
    Raises:
        ValueError: If rect is larger than the clamp area.
    """
def move(rect: pykraken._core.Rect, offset: pykraken._core.Vec2) -> pykraken._core.Rect:
    """
    Move a rectangle by the given offset.
    
    Args:
        rect (Rect): The rectangle to move.
        offset (Vec2): The offset to move by as (dx, dy).
    
    Returns:
        Rect: A new rectangle moved by the offset.
    """
@typing.overload
def scale_by(rect: pykraken._core.Rect, factor: typing.SupportsFloat) -> pykraken._core.Rect:
    """
    Scale a rectangle by a uniform factor.
    
    Args:
        rect (Rect): The rectangle to scale.
        factor (float): The scaling factor (must be > 0).
    
    Returns:
        Rect: A new rectangle scaled by the factor.
    
    Raises:
        ValueError: If factor is <= 0.
    """
@typing.overload
def scale_by(rect: pykraken._core.Rect, factor: pykraken._core.Vec2) -> pykraken._core.Rect:
    """
    Scale a rectangle by different factors for width and height.
    
    Args:
        rect (Rect): The rectangle to scale.
        factor (Vec2): The scaling factors as (scale_x, scale_y).
    
    Returns:
        Rect: A new rectangle scaled by the factors.
    
    Raises:
        ValueError: If any factor is <= 0.
    """
def scale_to(rect: pykraken._core.Rect, size: pykraken._core.Vec2) -> pykraken._core.Rect:
    """
    Scale a rectangle to the specified size.
    
    Args:
        rect (Rect): The rectangle to scale.
        size (Vec2): The new size as (width, height).
    
    Returns:
        Rect: A new rectangle scaled to the specified size.
    
    Raises:
        ValueError: If width or height is <= 0.
    """
