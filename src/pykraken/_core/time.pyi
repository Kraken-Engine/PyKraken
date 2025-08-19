"""
Time related functions
"""
from __future__ import annotations
import typing
__all__ = ['delay', 'get_delta', 'get_elapsed', 'get_fps', 'set_cap']
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
def set_cap(frame_rate: typing.SupportsInt) -> None:
    """
    Set the maximum framerate for the application.
    
    Args:
        frame_rate (int): Maximum framerate to enforce. Set to 0 for unlimited.
    """
