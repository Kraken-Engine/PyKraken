"""

Color utility functions and predefined color constants.

This module provides functions for color manipulation and conversion,
as well as commonly used color constants for convenience.
    
"""
from __future__ import annotations
import pykraken._core
import typing
__all__: list[str] = ['from_hex', 'from_hsv', 'grayscale', 'invert', 'lerp']
def from_hex(hex: str) -> pykraken._core.Color:
    """
    Create a Color from a hex string.
    
    Supports multiple hex formats:
    - "#RRGGBB" - 6-digit hex with full opacity
    - "#RRGGBBAA" - 8-digit hex with alpha
    - "#RGB" - 3-digit hex (each digit duplicated)
    - "#RGBA" - 4-digit hex with alpha (each digit duplicated)
    
    Args:
        hex (str): Hex color string (with or without '#' prefix).
    
    Returns:
        Color: New Color object from the hex string.
    
    Examples:
        from_hex("#FF00FF")      # Magenta, full opacity
        from_hex("#FF00FF80")    # Magenta, 50% opacity
        from_hex("#F0F")         # Same as "#FF00FF"
        from_hex("RGB")          # Without '#' prefix
    """
def from_hsv(h: typing.SupportsFloat, s: typing.SupportsFloat, v: typing.SupportsFloat, a: typing.SupportsFloat = 1.0) -> pykraken._core.Color:
    """
    Create a Color from HSV(A) values.
    
    Args:
        h (float): Hue angle (0-360).
        s (float): Saturation (0-1).
        v (float): Value/brightness (0-1).
        a (float, optional): Alpha (0-1). Defaults to 1.0.
    """
def grayscale(color: pykraken._core.Color) -> pykraken._core.Color:
    """
    Convert a color to grayscale.
    
    Args:
        color (Color): The color to convert.
    
    Returns:
        Color: New Color object representing the grayscale version.
    
    Example:
        grayscale(Color(255, 0, 0))  # Returns Color(76, 76, 76, 255)
    """
def invert(color: pykraken._core.Color) -> pykraken._core.Color:
    """
    Return the inverse of a color by flipping RGB channels.
    
    The alpha channel is preserved unchanged.
    
    Args:
        color (Color): The color to invert.
    
    Returns:
        Color: New Color with inverted RGB values (255 - original value).
    
    Example:
        invert(Color(255, 0, 128, 200))  # Returns Color(0, 255, 127, 200)
    """
def lerp(a: pykraken._core.Color, b: pykraken._core.Color, t: typing.SupportsFloat) -> pykraken._core.Color:
    """
    Linearly interpolate between two colors.
    
    Performs component-wise linear interpolation between start and end colors.
    All RGBA channels are interpolated independently.
    
    Args:
        a (Color): Start color (when t=0.0).
        b (Color): End color (when t=1.0).
        t (float): Blend factor. Values outside [0,1] will extrapolate.
    
    Returns:
        Color: New interpolated color.
    
    Examples:
        lerp(Color.RED, Color.BLUE, 0.5)    # Purple (halfway between red and blue)
        lerp(Color.BLACK, Color.WHITE, 0.25) # Dark gray
    """
