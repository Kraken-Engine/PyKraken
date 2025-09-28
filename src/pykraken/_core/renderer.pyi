"""
Functions for rendering graphics
"""
from __future__ import annotations
import pykraken._core
import typing
__all__: list[str] = ['clear', 'draw', 'get_res', 'present']
@typing.overload
def clear(color: typing.Any = None) -> None:
    """
    Clear the renderer with the specified color.
    
    Args:
        color (Color, optional): The color to clear with. Defaults to black (0, 0, 0, 255).
    
    Raises:
        ValueError: If color values are not between 0 and 255.
    """
@typing.overload
def clear(r: typing.SupportsInt, g: typing.SupportsInt, b: typing.SupportsInt, a: typing.SupportsInt = 255) -> None:
    """
    Clear the renderer with the specified color.
    
    Args:
        r (int): Red component (0-255).
        g (int): Green component (0-255).
        b (int): Blue component (0-255).
        a (int, optional): Alpha component (0-255). Defaults to 255.
    """
@typing.overload
def draw(texture: pykraken._core.Texture, dst: pykraken._core.Rect, src: typing.Any = None) -> None:
    """
        Render a texture with specified destination and source rectangles.
    
        Args:
            texture (Texture): The texture to render.
            dst (Rect): The destination rectangle on the renderer.
            src (Rect, optional): The source rectangle from the texture. Defaults to entire texture if not specified.
    """
@typing.overload
def draw(texture: pykraken._core.Texture, pos: typing.Any = None, anchor: pykraken._core.Anchor = pykraken._core.Anchor.CENTER) -> None:
    """
        Render a texture at the specified position with anchor alignment.
    
        Args:
            texture (Texture): The texture to render.
            pos (Vec2, optional): The position to draw at. Defaults to (0, 0).
            anchor (Anchor, optional): The anchor point for positioning. Defaults to CENTER.
    """
def get_res() -> pykraken._core.Vec2:
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
