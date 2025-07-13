"""
Color-related functions and constants
"""
from __future__ import annotations
import pykraken._core
import typing
__all__ = ['BLACK', 'BLUE', 'BROWN', 'CYAN', 'DARK_GRAY', 'DARK_GREY', 'GRAY', 'GREEN', 'GREY', 'LIGHT_GRAY', 'LIGHT_GREY', 'MAGENTA', 'MAROON', 'NAVY', 'OLIVE', 'ORANGE', 'PINK', 'PURPLE', 'RED', 'TEAL', 'WHITE', 'YELLOW', 'from_hex', 'from_hsv', 'invert', 'lerp', 'to_hex']
def from_hex(hex: str) -> pykraken._core.Color:
    """
    Create a Color from a hex string (e.g. "#FF00FF" or "#FF00FF80").
    """
def from_hsv(h: float, s: float, v: float, a: float = 1.0) -> pykraken._core.Color:
    """
    Create a Color from HSV(A) values.
    
    Args:
        h (float): Hue angle (0-360).
        s (float): Saturation (0-1).
        v (float): Value/brightness (0-1).
        a (float, optional): Alpha (0-1). Defaults to 1.0.
    """
def invert(color: pykraken._core.Color) -> pykraken._core.Color:
    """
    Return the inverse of a color (flips RGB channels).
    """
def lerp(a: pykraken._core.Color, b: pykraken._core.Color, t: float) -> pykraken._core.Color:
    """
    Linearly interpolate between two colors.
    
    Args:
        a (Color): Start color.
        b (Color): End color.
        t (float): Blend factor (0.0 = a, 1.0 = b).
    """
def to_hex(color: typing.Any) -> str:
    """
    Convert a Color or RGB(A) sequence to a hex string.
    """
BLACK: pykraken._core.Color  # value = Color(0, 0, 0, 255)
BLUE: pykraken._core.Color  # value = Color(0, 0, 255, 255)
BROWN: pykraken._core.Color  # value = Color(139, 69, 19, 255)
CYAN: pykraken._core.Color  # value = Color(0, 255, 255, 255)
DARK_GRAY: pykraken._core.Color  # value = Color(64, 64, 64, 255)
DARK_GREY: pykraken._core.Color  # value = Color(64, 64, 64, 255)
GRAY: pykraken._core.Color  # value = Color(128, 128, 128, 255)
GREEN: pykraken._core.Color  # value = Color(0, 255, 0, 255)
GREY: pykraken._core.Color  # value = Color(128, 128, 128, 255)
LIGHT_GRAY: pykraken._core.Color  # value = Color(192, 192, 192, 255)
LIGHT_GREY: pykraken._core.Color  # value = Color(192, 192, 192, 255)
MAGENTA: pykraken._core.Color  # value = Color(255, 0, 255, 255)
MAROON: pykraken._core.Color  # value = Color(128, 0, 0, 255)
NAVY: pykraken._core.Color  # value = Color(0, 0, 128, 255)
OLIVE: pykraken._core.Color  # value = Color(128, 128, 0, 255)
ORANGE: pykraken._core.Color  # value = Color(255, 165, 0, 255)
PINK: pykraken._core.Color  # value = Color(255, 192, 203, 255)
PURPLE: pykraken._core.Color  # value = Color(128, 0, 128, 255)
RED: pykraken._core.Color  # value = Color(255, 0, 0, 255)
TEAL: pykraken._core.Color  # value = Color(0, 128, 128, 255)
WHITE: pykraken._core.Color  # value = Color(255, 255, 255, 255)
YELLOW: pykraken._core.Color  # value = Color(255, 255, 0, 255)
