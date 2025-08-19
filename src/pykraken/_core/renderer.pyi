"""
Functions for rendering graphics
"""
from __future__ import annotations
import pykraken._core
import typing
__all__ = ['clear', 'get_res', 'present']
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
