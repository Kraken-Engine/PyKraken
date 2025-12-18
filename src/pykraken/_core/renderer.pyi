"""
Functions for rendering graphics
"""
from __future__ import annotations
import pykraken._core
import typing
__all__: list[str] = ['clear', 'draw', 'get_res', 'present', 'read_pixels']
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
def draw(texture: pykraken._core.Texture, transform: typing.Any = None, src: typing.Any = None) -> None:
    """
    Render a texture with the specified transform and source rectangle.
    
    Args:
        texture (Texture): The texture to render.
        transform (Transform, optional): The transform (position, rotation, scale, anchor, pivot). Defaults to identity transform.
        src (Rect, optional): The source rectangle from the texture. Defaults to entire texture if not specified.
    
    Raises:
        TypeError: If arguments are not of expected types.
        RuntimeError: If renderer is not initialized.
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
def read_pixels(src: typing.Any = None) -> pykraken._core.PixelArray:
    """
    Read pixel data from the renderer within the specified rectangle.
    
    Args:
        src (Rect, optional): The rectangle area to read pixels from. Defaults to entire renderer if None.
    Returns:
        PixelArray: An array containing the pixel data.
    Raises:
        RuntimeError: If reading pixels fails.
    """
