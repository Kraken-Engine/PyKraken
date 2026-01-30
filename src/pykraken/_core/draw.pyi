"""
Functions for drawing shape objects
"""
from __future__ import annotations
import collections.abc
import numpy
import numpy.typing
import pykraken._core
import typing
__all__: list[str] = ['circle', 'circles', 'ellipse', 'geometry', 'line', 'point', 'points', 'points_from_ndarray', 'polygon', 'polygons', 'rect', 'rects']
def circle(circle: pykraken._core.Circle, color: pykraken._core.Color, thickness: typing.SupportsFloat = 0, num_segments: typing.SupportsInt = 36) -> None:
    """
    Draw a circle to the renderer.
    
    Args:
        circle (Circle): The circle to draw.
        color (Color): The color of the circle.
        thickness (float, optional): The line thickness. If <= 0 or >= radius, draws filled circle. Defaults to 0 (filled).
        num_segments (int, optional): Number of segments to approximate the circle.
                                      Higher values yield smoother circles. Defaults to 36.
    """
def circles(circles: collections.abc.Sequence[pykraken._core.Circle], color: pykraken._core.Color, thickness: typing.SupportsFloat = 0, num_segments: typing.SupportsInt = 36) -> None:
    """
    Draw an array of circles in bulk to the renderer.
    
    Args:
        circles (Sequence[Circle]): The circles to draw in bulk.
        color (Color): The color of the circles.
        thickness (float, optional): The line thickness. If <= 0 or >= radius, draws filled circle. Defaults to 0 (filled).
        num_segments (int, optional): Number of segments to approximate each circle.
                                      Higher values yield smoother circles. Defaults to 36.
    """
def ellipse(bounds: pykraken._core.Rect, color: pykraken._core.Color, filled: bool = False) -> None:
    """
    Draw an ellipse to the renderer.
    
    Args:
        bounds (Rect): The bounding box of the ellipse.
        color (Color): The color of the ellipse.
        filled (bool, optional): Whether to draw a filled ellipse or just the outline.
                                 Defaults to False (outline).
    """
def geometry(texture: pykraken._core.Texture | None, vertices: collections.abc.Sequence[pykraken._core.Vertex], indices: typing.Any = None) -> None:
    """
    Draw arbitrary geometry using vertices and optional indices.
    
    Args:
        texture (Texture | None): The texture to apply to the geometry. Can be None.
        vertices (Sequence[Vertex]): A list of Vertex objects.
        indices (Sequence[int] | None): A list of indices defining the primitives.
                                       If None or empty, vertices are drawn sequentially.
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
def polygon(polygon: pykraken._core.Polygon, color: pykraken._core.Color, filled: bool = True) -> None:
    """
    Draw a polygon to the renderer.
    
    Args:
        polygon (Polygon): The polygon to draw.
        color (Color): The color of the polygon.
        filled (bool, optional): Whether to draw a filled polygon or just the outline.
                                 Defaults to False (outline). Works with both convex and concave polygons.
    """
def polygons(polygons: collections.abc.Sequence[pykraken._core.Polygon], color: pykraken._core.Color, filled: bool = True) -> None:
    """
    Draw an array of polygons in bulk to the renderer.
    
    Args:
        polygons (Sequence[Polygon]): The polygons to draw in bulk.
        color (Color): The color of the polygons.
        filled (bool, optional): Whether to draw filled polygons or just the outlines.
                                 Defaults to True (filled). Works with both convex and concave polygons.
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
