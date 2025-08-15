"""
Functions for rendering graphics
"""
from __future__ import annotations
import pykraken._core
import typing
__all__ = ['clear', 'draw_texture', 'get_resolution', 'present']
def clear(color: pykraken._core.Color = ...) -> None:
    """
    Clear the renderer with the specified color.
    
    Args:
        color (Color, optional): The color to clear with. Defaults to black (0, 0, 0, 255).
    
    Raises:
        ValueError: If color values are not between 0 and 255.
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
