"""
Functions for drawing shape objects
"""
from __future__ import annotations
import collections.abc
import numpy
import numpy.typing
import pykraken._core
import typing
__all__: list[str] = ['circle', 'line', 'point', 'points', 'points_from_ndarray', 'polygon', 'rect', 'rects']
def circle(circle: pykraken._core.Circle, color: pykraken._core.Color, thickness: typing.SupportsInt = 0) -> None:
    """
    Draw a circle to the renderer.
    
    Args:
        circle (Circle): The circle to draw.
        color (Color): The color of the circle.
        thickness (int, optional): The line thickness. If 0 or >= radius, draws filled circle.
                                  Defaults to 0 (filled).
    """
def line(line: pykraken._core.Line, color: pykraken._core.Color, thickness: typing.SupportsInt = 1) -> None:
    """
    Draw a line to the renderer.
    
    Args:
        line (Line): The line to draw.
        color (Color): The color of the line.
        thickness (int, optional): The line thickness in pixels. Defaults to 1.
    """
def point(point: pykraken._core.Vec2, color: pykraken._core.Color) -> None:
    """
    Draw a single point to the renderer.
    
    Args:
        point (Vec2): The position of the point.
        color (Color): The color of the point.
    
    Raises:
        RuntimeError: If point rendering fails.
    """
def points(points: collections.abc.Sequence[pykraken._core.Vec2], color: pykraken._core.Color) -> None:
    """
    Batch draw an array of points to the renderer.
    
    Args:
        points (Sequence[Vec2]): The points to batch draw.
        color (Color): The color of the points.
    
    Raises:
        RuntimeError: If point rendering fails.
    """
def points_from_ndarray(points: typing.Annotated[numpy.typing.ArrayLike, numpy.float64], color: pykraken._core.Color) -> None:
    """
    Batch draw points from a NumPy array.
    
    This fast path accepts a contiguous NumPy array of shape (N,2) (dtype float64) and
    reads coordinates directly with minimal overhead. Use this to measure the best-case
    zero-copy/buffer-backed path.
    
    Args:
        points (numpy.ndarray): Array with shape (N,2) containing x,y coordinates.
        color (Color): The color of the points.
    
    Raises:
        ValueError: If the array shape is not (N,2).
        RuntimeError: If point rendering fails.
    """
def polygon(polygon: pykraken._core.Polygon, color: pykraken._core.Color, filled: bool = False) -> None:
    """
    Draw a polygon to the renderer.
    
    Args:
        polygon (Polygon): The polygon to draw.
        color (Color): The color of the polygon.
        filled (bool, optional): Whether to draw a filled polygon or just the outline.
                                 Defaults to False (outline). Works with both convex and concave polygons.
    """
def rect(rect: pykraken._core.Rect, color: pykraken._core.Color, thickness: typing.SupportsInt = 0) -> None:
    """
    Draw a rectangle to the renderer.
    
    Args:
        rect (Rect): The rectangle to draw.
        color (Color): The color of the rectangle.
        thickness (int, optional): The border thickness. If 0 or >= half width/height, draws filled rectangle. Defaults to 0 (filled).
    """
def rects(rects: collections.abc.Sequence[pykraken._core.Rect], color: pykraken._core.Color, thickness: typing.SupportsInt = 0) -> None:
    """
    Batch draw an array of rectangles to the renderer.
    
    Args:
        rects (Sequence[Rect]): The rectangles to batch draw.
        color (Color): The color of the rectangles.
        thickness (int, optional): The border thickness of the rectangles. If 0 or >= half width/height, draws filled rectangles. Defaults to 0 (filled).
    """
