"""
Time related functions
"""
from __future__ import annotations
import typing
__all__: list[str] = ['delay', 'get_delta', 'get_elapsed', 'get_fps', 'get_scale', 'set_max_delta', 'set_scale', 'set_target']
def delay(milliseconds: typing.SupportsInt) -> None:
    """
    Delay the program execution for the specified duration.
    
    This function pauses execution for the given number of milliseconds.
    Useful for simple timing control, though using time.set_cap() is generally
    preferred for precise frame rate control with nanosecond accuracy.
    
    Args:
        milliseconds (int): The number of milliseconds to delay.
    """
def get_delta() -> float:
    """
    Get the time elapsed since the last frame in seconds.
    
    For stability, the returned delta is clamped so it will not be
    smaller than 1/12 seconds (equivalent to capping at 12 FPS). This prevents
    unstable calculations that rely on delta when very small frame times are
    measured.
    
    Returns:
        float: The time elapsed since the last frame, in seconds.
    """
def get_elapsed() -> float:
    """
    Get the elapsed time since the program started.
    
    Returns:
        float: The total elapsed time since program start, in seconds.
    """
def get_fps() -> float:
    """
    Get the current frames per second of the program.
    
    Returns:
        float: The current FPS based on the last frame time.
    """
def get_scale() -> float:
    """
    Get the current global time scale factor.
    
    Returns:
        float: The current time scale factor.
    """
def set_max_delta(max_delta: typing.SupportsFloat) -> None:
    """
    Set the maximum allowed delta time between frames.
    
    Parameters:
        max_delta (float): The maximum delta time in seconds, greater than 0.0.
                           This is useful to prevent large delta values during
                           frame drops or pauses that could destabilize physics or animations.
    """
def set_scale(scale: typing.SupportsFloat) -> None:
    """
    Set the global time scale factor.
    
    Args:
        scale (float): The time scale factor. Values < 0.0 are clamped to 0.0.
                       A scale of 1.0 represents normal time, 0.5 is half speed,
                       and 2.0 is double speed.
    """
def set_target(frame_rate: typing.SupportsInt) -> None:
    """
    Set the target framerate for the application.
    
    Args:
        frame_rate (int): Target framerate to enforce. Values <= 0 disable frame rate limiting.
    """
