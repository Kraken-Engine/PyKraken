"""
Collision detection functions
"""
from __future__ import annotations
import pykraken._core
import typing
__all__: list[str] = ['contains', 'overlap']
@typing.overload
def contains(outer: pykraken._core.Rect, inner: pykraken._core.Rect) -> bool:
    """
    Check whether one rectangle completely contains another rectangle.
    
    Args:
        outer (Rect): The outer rectangle.
        inner (Rect): The inner rectangle.
    
    Returns:
        bool: True if the outer rectangle completely contains the inner rectangle.
    """
@typing.overload
def contains(rect: pykraken._core.Rect, circle: pykraken._core.Circle) -> bool:
    """
    Check whether a rectangle completely contains a circle.
    
    Args:
        rect (Rect): The rectangle.
        circle (Circle): The circle.
    
    Returns:
        bool: True if the rectangle completely contains the circle.
    """
@typing.overload
def contains(rect: pykraken._core.Rect, line: pykraken._core.Line) -> bool:
    """
    Check whether a rectangle completely contains a line.
    
    Args:
        rect (Rect): The rectangle.
        line (Line): The line.
    
    Returns:
        bool: True if the rectangle completely contains the line.
    """
@typing.overload
def contains(outer: pykraken._core.Circle, inner: pykraken._core.Circle) -> bool:
    """
    Check whether one circle completely contains another circle.
    
    Args:
        outer (Circle): The outer circle.
        inner (Circle): The inner circle.
    
    Returns:
        bool: True if the outer circle completely contains the inner circle.
    """
@typing.overload
def contains(circle: pykraken._core.Circle, rect: pykraken._core.Rect) -> bool:
    """
    Check whether a circle completely contains a rectangle.
    
    Args:
        circle (Circle): The circle.
        rect (Rect): The rectangle.
    
    Returns:
        bool: True if the circle completely contains the rectangle.
    """
@typing.overload
def contains(circle: pykraken._core.Circle, line: pykraken._core.Line) -> bool:
    """
    Check whether a circle completely contains a line.
    
    Args:
        circle (Circle): The circle.
        line (Line): The line.
    
    Returns:
        bool: True if the circle completely contains the line.
    """
@typing.overload
def overlap(a: pykraken._core.Rect, b: pykraken._core.Rect) -> bool:
    """
    Check whether two rectangles overlap.
    
    Args:
        a (Rect): The first rectangle.
        b (Rect): The second rectangle.
    
    Returns:
        bool: True if the rectangles overlap.
    """
@typing.overload
def overlap(rect: pykraken._core.Rect, circle: pykraken._core.Circle) -> bool:
    """
    Check whether a rectangle and a circle overlap.
    
    Args:
        rect (Rect): The rectangle.
        circle (Circle): The circle.
    
    Returns:
        bool: True if the rectangle and circle overlap.
    """
@typing.overload
def overlap(rect: pykraken._core.Rect, line: pykraken._core.Line) -> bool:
    """
    Check whether a rectangle and a line overlap.
    
    Args:
        rect (Rect): The rectangle.
        line (Line): The line.
    
    Returns:
        bool: True if the rectangle and line overlap.
    """
@typing.overload
def overlap(rect: pykraken._core.Rect, point: pykraken._core.Vec2) -> bool:
    """
    Check whether a rectangle contains a point.
    
    Args:
        rect (Rect): The rectangle.
        point (Vec2): The point.
    
    Returns:
        bool: True if the rectangle contains the point.
    """
@typing.overload
def overlap(a: pykraken._core.Circle, b: pykraken._core.Circle) -> bool:
    """
    Check whether two circles overlap.
    
    Args:
        a (Circle): The first circle.
        b (Circle): The second circle.
    
    Returns:
        bool: True if the circles overlap.
    """
@typing.overload
def overlap(circle: pykraken._core.Circle, rect: pykraken._core.Rect) -> bool:
    """
    Check whether a circle and a rectangle overlap.
    
    Args:
        circle (Circle): The circle.
        rect (Rect): The rectangle.
    
    Returns:
        bool: True if the circle and rectangle overlap.
    """
@typing.overload
def overlap(circle: pykraken._core.Circle, line: pykraken._core.Line) -> bool:
    """
    Check whether a circle and a line overlap.
    
    Args:
        circle (Circle): The circle.
        line (Line): The line.
    
    Returns:
        bool: True if the circle and line overlap.
    """
@typing.overload
def overlap(circle: pykraken._core.Circle, point: pykraken._core.Vec2) -> bool:
    """
    Check whether a circle contains a point.
    
    Args:
        circle (Circle): The circle.
        point (Vec2): The point.
    
    Returns:
        bool: True if the circle contains the point.
    """
@typing.overload
def overlap(a: pykraken._core.Line, b: pykraken._core.Line) -> bool:
    """
    Check whether two lines overlap (intersect).
    
    Args:
        a (Line): The first line.
        b (Line): The second line.
    
    Returns:
        bool: True if the lines intersect.
    """
@typing.overload
def overlap(line: pykraken._core.Line, rect: pykraken._core.Rect) -> bool:
    """
    Check whether a line and a rectangle overlap.
    
    Args:
        line (Line): The line.
        rect (Rect): The rectangle.
    
    Returns:
        bool: True if the line and rectangle overlap.
    """
@typing.overload
def overlap(line: pykraken._core.Line, circle: pykraken._core.Circle) -> bool:
    """
    Check whether a line and a circle overlap.
    
    Args:
        line (Line): The line.
        circle (Circle): The circle.
    
    Returns:
        bool: True if the line and circle overlap.
    """
@typing.overload
def overlap(point: pykraken._core.Vec2, rect: pykraken._core.Rect) -> bool:
    """
    Check whether a point is inside a rectangle.
    
    Args:
        point (Vec2): The point.
        rect (Rect): The rectangle.
    
    Returns:
        bool: True if the point is inside the rectangle.
    """
@typing.overload
def overlap(point: pykraken._core.Vec2, circle: pykraken._core.Circle) -> bool:
    """
    Check whether a point is inside a circle.
    
    Args:
        point (Vec2): The point.
        circle (Circle): The circle.
    
    Returns:
        bool: True if the point is inside the circle.
    """
@typing.overload
def overlap(polygon: pykraken._core.Polygon, point: pykraken._core.Vec2) -> bool:
    """
    Check whether a polygon contains a point.
    
    Args:
        polygon (Polygon): The polygon.
        point (Vec2): The point.
    
    Returns:
        bool: True if the polygon contains the point.
    """
@typing.overload
def overlap(point: pykraken._core.Vec2, polygon: pykraken._core.Polygon) -> bool:
    """
    Check whether a point is inside a polygon.
    
    Args:
        point (Vec2): The point.
        polygon (Polygon): The polygon.
    
    Returns:
        bool: True if the point is inside the polygon.
    """
@typing.overload
def overlap(polygon: pykraken._core.Polygon, rect: pykraken._core.Rect) -> bool:
    """
    Check whether a polygon and a rectangle overlap.
    
    Args:
        polygon (Polygon): The polygon.
        rect (Rect): The rectangle.
    
    Returns:
        bool: True if the polygon and rectangle overlap.
    """
@typing.overload
def overlap(rect: pykraken._core.Rect, polygon: pykraken._core.Polygon) -> bool:
    """
    Check whether a rectangle and a polygon overlap.
    
    Args:
        rect (Rect): The rectangle.
        polygon (Polygon): The polygon.
    
    Returns:
        bool: True if the rectangle and polygon overlap.
    """
