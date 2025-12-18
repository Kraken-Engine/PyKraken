"""
Effect factory functions for Orchestrator
"""
from __future__ import annotations
import collections.abc
import pykraken._core
import typing
__all__: list[str] = ['call', 'move_to', 'rotate_to', 'scale_to', 'shake', 'wait']
def call(callback: collections.abc.Callable[[], None]) -> pykraken._core.Effect:
    """
    Create an effect that calls a function.
    
    Args:
        callback (callable): Function to call when this step is reached.
    
    Returns:
        Effect: The call effect.
    """
def move_to(pos: typing.Any = None, dur: typing.SupportsFloat = 0.0, ease: typing.Any = None) -> pykraken._core.Effect:
    """
    Create a move-to effect.
    
    Args:
        pos (Vec2): Target position.
        dur (float): Duration in seconds.
        ease (callable): Easing function (t -> t).
    
    Returns:
        Effect: The move-to effect.
    """
def rotate_to(angle: typing.SupportsFloat, dur: typing.SupportsFloat = 0.0, ease: typing.Any = None) -> pykraken._core.Effect:
    """
    Create a rotate-to effect.
    
    Args:
        angle (float): Target angle in radians.
        dur (float): Duration in seconds.
        ease (callable): Easing function (t -> t).
    
    Returns:
        Effect: The rotate-to effect.
    """
def scale_to(scale: typing.Any = None, dur: typing.SupportsFloat = 0.0, ease: typing.Any = None) -> pykraken._core.Effect:
    """
    Create a scale-to effect.
    
    Args:
        scale (float or Vec2): Target scale. A single number applies to both axes.
        dur (float): Duration in seconds.
        ease (callable): Easing function (t -> t).
    
    Returns:
        Effect: The scale-to effect.
    """
def shake(amp: typing.SupportsFloat, freq: typing.SupportsFloat, dur: typing.SupportsFloat) -> pykraken._core.Effect:
    """
    Create a shake effect.
    
    Args:
        amp (float): Shake amplitude in pixels.
        freq (float): Shake frequency in Hz.
        dur (float): Duration in seconds.
    
    Returns:
        Effect: The shake effect.
    """
def wait(dur: typing.SupportsFloat) -> pykraken._core.Effect:
    """
    Create a wait/delay effect.
    
    Args:
        dur (float): Duration to wait in seconds.
    
    Returns:
        Effect: The wait effect.
    """
