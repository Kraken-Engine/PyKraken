"""
Color related functions
"""
from __future__ import annotations
import pykraken._core
__all__ = ['BLACK', 'BLUE', 'BROWN', 'CYAN', 'DARK_GRAY', 'DARK_GREY', 'GRAY', 'GREEN', 'GREY', 'LIGHT_GRAY', 'LIGHT_GREY', 'MAGENTA', 'MAROON', 'NAVY', 'OLIVE', 'ORANGE', 'PINK', 'PURPLE', 'RED', 'TEAL', 'WHITE', 'YELLOW', 'from_hex', 'from_hsv', 'invert', 'lerp', 'to_hex']
def from_hex(*args, **kwargs):
    """
    Create a Color from a hex string.
    """
def from_hsv(*args, **kwargs):
    """
    Create a Color from HSV values.
    """
def invert(*args, **kwargs):
    """
    Invert the given Color.
    """
def lerp(*args, **kwargs):
    """
    Linearly interpolate between two Colors.
    """
def to_hex(*args, **kwargs):
    """
    Convert a Color to a hex string.
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
