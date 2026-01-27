"""
Functions for rendering graphics
"""
from __future__ import annotations
import pykraken._core
import typing
__all__: list[str] = ['clear', 'draw', 'get_default_scale_mode', 'get_target_resolution', 'present', 'read_pixels', 'set_default_scale_mode', 'set_target']
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
def draw(texture: pykraken._core.Texture, transform: typing.Any = None, anchor: typing.Any = None, pivot: typing.Any = None) -> None:
    """
    Render a texture.
    
    Args:
        texture (Texture): The texture to render.
        transform (Transform, optional): The transform (position, rotation, scale).
        anchor (Vec2 | None): The anchor point (0.0-1.0). Defaults to top left (0, 0).
        pivot (Vec2 | None): The rotation pivot (0.0-1.0). Defaults to center (0.5, 0.5).
    """
def get_default_scale_mode() -> pykraken._core.TextureScaleMode:
    """
    Get the current default TextureScaleMode for new textures.
    
    Returns:
        TextureScaleMode: The current default scaling/filtering mode.
    """
def get_target_resolution() -> pykraken._core.Vec2:
    """
    Get the resolution of the current render target.
    If no target is set, returns the logical presentation resolution.
    
    Returns:
        Vec2: The width and height of the render target.
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
def set_default_scale_mode(scale_mode: pykraken._core.TextureScaleMode) -> None:
    """
    Set the default TextureScaleMode for new textures.
    
    Args:
        scale_mode (TextureScaleMode): The default scaling/filtering mode to use for new textures.
    """
def set_target(target: pykraken._core.Texture) -> None:
    """
    Set the current render target to the provided Texture, or unset if None.
    
    Args:
        target (Texture, optional): Texture created with TextureAccess.TARGET, or None to unset.
    
    Raises:
        RuntimeError: If the renderer is not initialized or the texture is not a TARGET texture.
    """
