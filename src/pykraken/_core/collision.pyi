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
    Checks if one rectangle completely contains another rectangle.
    
    Parameters:
        outer (Rect): The outer rectangle.
        inner (Rect): The inner rectangle.
    
    Returns:
        bool: Whether the outer rectangle completely contains the inner rectangle.
    """
@typing.overload
def contains(rect: pykraken._core.Rect, circle: pykraken._core.Circle) -> bool:
    """
    Checks if a rectangle completely contains a circle.
    
    Parameters:
        rect (Rect): The rectangle.
        circle (Circle): The circle.
    
    Returns:
        bool: Whether the rectangle completely contains the circle.
    """
@typing.overload
def contains(rect: pykraken._core.Rect, line: pykraken._core.Line) -> bool:
    """
    Checks if a rectangle completely contains a line.
    
    Parameters:
        rect (Rect): The rectangle.
        line (Line): The line.
    
    Returns:
        bool: Whether the rectangle completely contains the line.
    """
@typing.overload
def contains(outer: pykraken._core.Circle, inner: pykraken._core.Circle) -> bool:
    """
    Checks if one circle completely contains another circle.
    
    Parameters:
        outer (Circle): The outer circle.
        inner (Circle): The inner circle.
    
    Returns:
        bool: Whether the outer circle completely contains the inner circle.
    """
@typing.overload
def contains(circle: pykraken._core.Circle, rect: pykraken._core.Rect) -> bool:
    """
    Checks if a circle completely contains a rectangle.
    
    Parameters:
        circle (Circle): The circle.
        rect (Rect): The rectangle.
    
    Returns:
        bool: Whether the circle completely contains the rectangle.
    """
@typing.overload
def contains(circle: pykraken._core.Circle, line: pykraken._core.Line) -> bool:
    """
    Checks if a circle completely contains a line.
    
    Parameters:
        circle (Circle): The circle.
        line (Line): The line.
    
    Returns:
        bool: Whether the circle completely contains the line.
    """
@typing.overload
def overlap(a: pykraken._core.Rect, b: pykraken._core.Rect) -> bool:
    """
    Checks if two rectangles overlap.
    
    Parameters:
        a (Rect): The first rectangle.
        b (Rect): The second rectangle.
    
    Returns:
        bool: Whether the rectangles overlap.
    """
@typing.overload
def overlap(rect: pykraken._core.Rect, circle: pykraken._core.Circle) -> bool:
    """
    Checks if a rectangle and a circle overlap.
    
    Parameters:
        rect (Rect): The rectangle.
        circle (Circle): The circle.
    
    Returns:
        bool: Whether the rectangle and circle overlap.
    """
@typing.overload
def overlap(rect: pykraken._core.Rect, line: pykraken._core.Line) -> bool:
    """
    Checks if a rectangle and a line overlap.
    
    Parameters:
        rect (Rect): The rectangle.
        line (Line): The line.
    
    Returns:
        bool: Whether the rectangle and line overlap.
    """
@typing.overload
def overlap(rect: pykraken._core.Rect, point: pykraken._core.Vec2) -> bool:
    """
    Checks if a rectangle contains a point.
    
    Parameters:
        rect (Rect): The rectangle.
        point (Vec2): The point.
    
    Returns:
        bool: Whether the rectangle contains the point.
    """
@typing.overload
def overlap(a: pykraken._core.Circle, b: pykraken._core.Circle) -> bool:
    """
    Checks if two circles overlap.
    
    Parameters:
        a (Circle): The first circle.
        b (Circle): The second circle.
    
    Returns:
        bool: Whether the circles overlap.
    """
@typing.overload
def overlap(circle: pykraken._core.Circle, rect: pykraken._core.Rect) -> bool:
    """
    Checks if a circle and a rectangle overlap.
    
    Parameters:
        circle (Circle): The circle.
        rect (Rect): The rectangle.
    
    Returns:
        bool: Whether the circle and rectangle overlap.
    """
@typing.overload
def overlap(circle: pykraken._core.Circle, line: pykraken._core.Line) -> bool:
    """
    Checks if a circle and a line overlap.
    
    Parameters:
        circle (Circle): The circle.
        line (Line): The line.
    
    Returns:
        bool: Whether the circle and line overlap.
    """
@typing.overload
def overlap(circle: pykraken._core.Circle, point: pykraken._core.Vec2) -> bool:
    """
    Checks if a circle contains a point.
    
    Parameters:
        circle (Circle): The circle.
        point (Vec2): The point.
    
    Returns:
        bool: Whether the circle contains the point.
    """
@typing.overload
def overlap(a: pykraken._core.Line, b: pykraken._core.Line) -> bool:
    """
    Checks if two lines overlap (intersect).
    
    Parameters:
        a (Line): The first line.
        b (Line): The second line.
    
    Returns:
        bool: Whether the lines intersect.
    """
@typing.overload
def overlap(line: pykraken._core.Line, rect: pykraken._core.Rect) -> bool:
    """
    Checks if a line and a rectangle overlap.
    
    Parameters:
        line (Line): The line.
        rect (Rect): The rectangle.
    
    Returns:
        bool: Whether the line and rectangle overlap.
    """
@typing.overload
def overlap(line: pykraken._core.Line, circle: pykraken._core.Circle) -> bool:
    """
    Checks if a line and a circle overlap.
    
    Parameters:
        line (Line): The line.
        circle (Circle): The circle.
    
    Returns:
        bool: Whether the line and circle overlap.
    """
@typing.overload
def overlap(point: pykraken._core.Vec2, rect: pykraken._core.Rect) -> bool:
    """
    Checks if a point is inside a rectangle.
    
    Parameters:
        point (Vec2): The point.
        rect (Rect): The rectangle.
    
    Returns:
        bool: Whether the point is inside the rectangle.
    """
@typing.overload
def overlap(point: pykraken._core.Vec2, circle: pykraken._core.Circle) -> bool:
    """
    Checks if a point is inside a circle.
    
    Parameters:
        point (Vec2): The point.
        circle (Circle): The circle.
    
    Returns:
        bool: Whether the point is inside the circle.
    """
@typing.overload
def overlap(polygon: pykraken._core.Polygon, point: pykraken._core.Vec2) -> bool:
    """
    Checks if a polygon contains a point.
    
    Parameters:
        polygon (Polygon): The polygon.
        point (Vec2): The point.
    
    Returns:
        bool: Whether the polygon contains the point.
    """
@typing.overload
def overlap(point: pykraken._core.Vec2, polygon: pykraken._core.Polygon) -> bool:
    """
    Checks if a point is inside a polygon.
    
    Parameters:
        point (Vec2): The point.
        polygon (Polygon): The polygon.
    
    Returns:
        bool: Whether the point is inside the polygon.
    """
@typing.overload
def overlap(polygon: pykraken._core.Polygon, rect: pykraken._core.Rect) -> bool:
    """
    Checks if a polygon and a rectangle overlap.
    
    Parameters:
        polygon (Polygon): The polygon.
        rect (Rect): The rectangle.
    
    Returns:
        bool: Whether the polygon and rectangle overlap.
    """
@typing.overload
def overlap(rect: pykraken._core.Rect, polygon: pykraken._core.Polygon) -> bool:
    """
    Checks if a rectangle and a polygon overlap.
    
    Parameters:
        rect (Rect): The rectangle.
        polygon (Polygon): The polygon.
    
    Returns:
        bool: Whether the rectangle and polygon overlap.
    """
