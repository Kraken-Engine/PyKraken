"""
Functions for rendering graphics
"""
from __future__ import annotations
import pykraken._core
__all__ = ['clear', 'get_res', 'present']
def clear(color: pykraken._core.Color = ...) -> None:
    """
    Clear the renderer with the specified color.
    
    Args:
        color (Color, optional): The color to clear with. Defaults to black (0, 0, 0, 255).
    
    Raises:
        ValueError: If color values are not between 0 and 255.
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
