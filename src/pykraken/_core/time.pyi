"""
Time related functions
"""
from __future__ import annotations
__all__ = ['delay', 'get_elapsed']
def delay(milliseconds: int) -> None:
    """
    Delay the program execution for the specified duration.
    
    This function pauses execution for the given number of milliseconds.
    Useful for simple timing control, though using Clock.tick() is generally
    preferred for frame rate control.
    
    Args:
        milliseconds (int): The number of milliseconds to delay.
    """
def get_elapsed() -> float:
    """
    Get the elapsed time since the program started.
    
    Returns:
        float: The total elapsed time since program start, in seconds.
    """
