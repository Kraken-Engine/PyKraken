"""
Functions for drawing shape objects
"""
from __future__ import annotations
import collections.abc
import numpy
import numpy.typing
import pykraken._core
import typing
__all__: list[str] = ['bezier', 'capsule', 'capsules', 'circle', 'circles', 'ellipse', 'ellipses', 'geometry', 'line', 'lines', 'point', 'points', 'points_from_ndarray', 'polygon', 'polygons', 'rect', 'rects', 'sector']
def bezier(control_points: collections.abc.Sequence[pykraken._core.Vec2], color: pykraken._core.Color, thickness: typing.SupportsFloat = 1.0, num_segments: typing.SupportsInt = 24) -> None:
    """
    Draw a Bezier curve with 3 or 4 control points.
    
    Args:
        control_points (Sequence[Vec2]): The control points (3 for quadratic, 4 for cubic).
        color (Color): The color of the curve.
        thickness (float, optional): The line thickness. Defaults to 1.0.
        num_segments (int, optional): Number of segments to approximate the curve. Defaults to 24.
    """
def capsule(capsule: pykraken._core.Capsule, color: pykraken._core.Color, thickness: typing.SupportsFloat = 0, num_segments: typing.SupportsInt = 24) -> None:
    """
    Draw a capsule to the renderer.
    
    Args:
        capsule (Capsule): The capsule to draw.
        color (Color): The color of the capsule.
        thickness (float, optional): The line thickness. If <= 0 or >= radius, draws filled capsule. Defaults to 0 (filled).
        num_segments (int, optional): Number of segments to approximate the capsule ends.
                                      Higher values yield smoother capsules. Defaults to 24.
    """
def capsules(capsules: collections.abc.Sequence[pykraken._core.Capsule], color: pykraken._core.Color, thickness: typing.SupportsFloat = 0, num_segments: typing.SupportsInt = 24) -> None:
    """
    Draw an array of capsules in bulk to the renderer.
    
    Args:
        capsules (Sequence[Capsule]): The capsules to draw in bulk.
        color (Color): The color of the capsules.
        thickness (float, optional): The line thickness. If <= 0 or >= radius, draws filled capsules. Defaults to 0 (filled).
        num_segments (int, optional): Number of segments to approximate each capsule end.
                                      Higher values yield smoother capsules. Defaults to 24.
    """
def circle(circle: pykraken._core.Circle, color: pykraken._core.Color, thickness: typing.SupportsFloat = 0, num_segments: typing.SupportsInt = 24) -> None:
    """
    Draw a circle to the renderer.
    
    Args:
        circle (Circle): The circle to draw.
        color (Color): The color of the circle.
        thickness (float, optional): The line thickness. If <= 0 or >= radius, draws filled circle. Defaults to 0 (filled).
        num_segments (int, optional): Number of segments to approximate the circle.
                                      Higher values yield smoother circles. Defaults to 24.
    """
def circles(circles: collections.abc.Sequence[pykraken._core.Circle], color: pykraken._core.Color, thickness: typing.SupportsFloat = 0, num_segments: typing.SupportsInt = 24) -> None:
    """
    Draw an array of circles in bulk to the renderer.
    
    Args:
        circles (Sequence[Circle]): The circles to draw in bulk.
        color (Color): The color of the circles.
        thickness (float, optional): The line thickness. If <= 0 or >= radius, draws filled circle. Defaults to 0 (filled).
        num_segments (int, optional): Number of segments to approximate each circle.
                                      Higher values yield smoother circles. Defaults to 24.
    """
def ellipse(bounds: pykraken._core.Rect, color: pykraken._core.Color, thickness: typing.SupportsFloat = 0.0, num_segments: typing.SupportsInt = 24) -> None:
    """
    Draw an ellipse to the renderer.
    
    Args:
        bounds (Rect): The bounding box of the ellipse.
        color (Color): The color of the ellipse.
        thickness (float, optional): The line thickness. If <= 0 or >= radius, draws filled ellipse. Defaults to 0 (filled).
        num_segments (int, optional): Number of segments to approximate the ellipse.
                                      Higher values yield smoother ellipses. Defaults to 24.
    """
def ellipses(bounds: collections.abc.Sequence[pykraken._core.Rect], color: pykraken._core.Color, thickness: typing.SupportsFloat = 0.0, num_segments: typing.SupportsInt = 24) -> None:
    """
    Draw an array of ellipses in bulk to the renderer.
    
    Args:
        bounds (Sequence[Rect]): The bounding boxes of the ellipses to draw in bulk.
        color (Color): The color of the ellipses.
        thickness (float, optional): The line thickness. If <= 0 or >= radius, draws filled ellipses. Defaults to 0 (filled).
        num_segments (int, optional): Number of segments to approximate each ellipse.
                                      Higher values yield smoother ellipses. Defaults to 24.
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
def line(line: pykraken._core.Line, color: pykraken._core.Color, thickness: typing.SupportsFloat = 1.0) -> None:
    """
    Draw a line to the renderer.
    
    Args:
        line (Line): The line to draw.
        color (Color): The color of the line.
        thickness (float, optional): The line thickness in pixels. Defaults to 1.0.
    """
def lines(lines: collections.abc.Sequence[pykraken._core.Line], color: pykraken._core.Color, thickness: typing.SupportsFloat = 1.0) -> None:
    """
    Batch draw an array of lines to the renderer.
    
    Args:
        lines (Sequence[Line]): The lines to batch draw.
        color (Color): The color of the lines.
        thickness (float, optional): The line thickness in pixels. Defaults to 1.0.
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
        filled (bool, optional): Whether to draw a filled polygon or just the outline. Defaults to False (outline).
    """
def polygons(polygons: collections.abc.Sequence[pykraken._core.Polygon], color: pykraken._core.Color, filled: bool = True) -> None:
    """
    Draw an array of polygons in bulk to the renderer.
    
    Args:
        polygons (Sequence[Polygon]): The polygons to draw in bulk.
        color (Color): The color of the polygons.
        filled (bool, optional): Whether to draw filled polygons or just the outlines. Defaults to True (filled).
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
def sector(circle: pykraken._core.Circle, start_angle: typing.SupportsFloat, end_angle: typing.SupportsFloat, color: pykraken._core.Color, thickness: typing.SupportsFloat = 0.0, num_segments: typing.SupportsInt = 24) -> None:
    """
    Draw a circular sector or arc.
    
    Args:
        circle (Circle): The circle defining the sector.
        start_angle (float): The start angle in radians.
        end_angle (float): The end angle in radians.
        color (Color): The color of the sector.
        thickness (float, optional): The line thickness. If <= 0 or >= radius, draws filled sector. Defaults to 0 (filled).
        num_segments (int, optional): Number of segments to approximate the arc. Defaults to 24.
    """
