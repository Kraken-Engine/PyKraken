"""
Function for rendering graphics
"""
from __future__ import annotations
import pykraken._core
import typing
__all__ = ['clear', 'draw_circle', 'draw_line', 'draw_point', 'draw_points', 'draw_polygon', 'draw_rect', 'draw_rects', 'draw_texture', 'get_resolution', 'present']
def clear(color: pykraken._core.Color = ...) -> None:
    """
    Clear the renderer with the specified color.
    
    Args:
        color (Color, optional): The color to clear with. Defaults to black (0, 0, 0, 255).
    
    Raises:
        ValueError: If color values are not between 0 and 255.
    """
def draw_circle(circle: pykraken._core.Circle, color: pykraken._core.Color, thickness: int = 0) -> None:
    """
    Draw a circle to the renderer.
    
    Args:
        circle (Circle): The circle to draw.
        color (Color): The color of the circle.
        thickness (int, optional): The line thickness. If 0 or >= radius, draws filled circle.
                                  Defaults to 0 (filled).
    """
def draw_line(line: pykraken._core.Line, color: pykraken._core.Color, thickness: int = 1) -> None:
    """
    Draw a line to the renderer.
    
    Args:
        line (Line): The line to draw.
        color (Color): The color of the line.
        thickness (int, optional): The line thickness in pixels. Defaults to 1.
    """
def draw_point(point: pykraken._core.Vec2, color: pykraken._core.Color) -> None:
    """
    Draw a single point to the renderer.
    
    Args:
        point (Vec2): The position of the point.
        color (Color): The color of the point.
    
    Raises:
        RuntimeError: If point rendering fails.
    """
def draw_points(points: list[pykraken._core.Vec2], color: pykraken._core.Color) -> None:
    """
    Batch draw an array of points to the renderer.
    
    Args:
        points (Sequence[Vec2]): The points to batch draw.
        color (Color): The color of the points.
    
    Raises:
        RuntimeError: If point rendering fails.
    """
def draw_polygon(polygon: pykraken._core.Polygon, color: pykraken._core.Color, filled: bool = False) -> None:
    """
    Draw a polygon to the renderer.
    
    Args:
        polygon (Polygon): The polygon to draw.
        color (Color): The color of the polygon.
        filled (bool, optional): Whether to draw a filled polygon or just the outline.
                                 Defaults to True (filled). Works with both convex and concave polygons.
    """
def draw_rect(rect: pykraken._core.Rect, color: pykraken._core.Color, thickness: int = 0) -> None:
    """
    Draw a rectangle to the renderer.
    
    Args:
        rect (Rect): The rectangle to draw.
        color (Color): The color of the rectangle.
        thickness (int, optional): The border thickness. If 0 or >= half width/height, draws filled rectangle. Defaults to 0 (filled).
    """
def draw_rects(rects: list[pykraken._core.Rect], color: pykraken._core.Color, thickness: int = 0) -> None:
    """
    Batch draw an array of rectangles to the renderer.
    
    Args:
        rects (Sequence[Rect]): The rectangles to batch draw.
        color (Color): The color of the rectangles.
        thickness (int, optional): The border thickness of the rectangles. If 0 or >= half width/height, draws filled rectangles. Defaults to 0 (filled).
    """
@typing.overload
def draw_texture(texture: pykraken._core.Texture, dst_rect: pykraken._core.Rect, src_rect: pykraken._core.Rect = ...) -> None:
    """
    Draw a texture with specified destination and source rectangles.
    
    Args:
        texture (Texture): The texture to draw.
        dst_rect (Rect): The destination rectangle on the renderer.
        src_rect (Rect, optional): The source rectangle from the texture. 
                                  Defaults to entire texture if not specified.
    """
@typing.overload
def draw_texture(texture: pykraken._core.Texture, pos: pykraken._core.Vec2 = ..., anchor: pykraken._core.Anchor = pykraken._core.Anchor.CENTER) -> None:
    """
    Draw a texture at the specified position with anchor alignment.
    
    Args:
        texture (Texture): The texture to draw.
        pos (Vec2, optional): The position to draw at. Defaults to (0, 0).
        anchor (Anchor, optional): The anchor point for positioning. Defaults to CENTER.
    """
def get_resolution() -> pykraken._core.Vec2:
    """
    Get the resolution of the renderer.
    
    Returns:
        Vec2: The current rendering resolution as (width, height).
    """
def present() -> None:
    """
    Present the rendered content to the screen.
    
    This finalizes the current frame and displays it. Should be called after
    all drawing operations for the frame are complete.
    """
