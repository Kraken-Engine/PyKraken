"""
Mouse related functions
"""
from __future__ import annotations
import pykraken._core
__all__: list[str] = ['get_pos', 'get_rel', 'hide', 'is_hidden', 'is_just_pressed', 'is_just_released', 'is_locked', 'is_pressed', 'lock', 'show', 'unlock']
def get_pos() -> pykraken._core.Vec2:
    """
    Get the current position of the mouse cursor.
    
    Returns:
        tuple[float, float]: The current mouse position as (x, y) coordinates.
    """
def get_rel() -> pykraken._core.Vec2:
    """
    Get the relative mouse movement since the last frame.
    
    Returns:
        tuple[float, float]: The relative movement of the mouse as (dx, dy).
    """
def hide() -> None:
    """
    Hide the mouse cursor from view.
    
    The cursor will be invisible but mouse input will still be tracked.
    """
def is_hidden() -> bool:
    """
    Check if the mouse cursor is currently hidden.
    
    Returns:
        bool: True if the cursor is hidden.
    """
def is_just_pressed(button: pykraken._core.MouseButton) -> bool:
    """
    Check if a mouse button was pressed this frame.
    
    Args:
        button (MouseButton): The mouse button to check.
    
    Returns:
        bool: True if the button was just pressed.
    """
def is_just_released(button: pykraken._core.MouseButton) -> bool:
    """
    Check if a mouse button was released this frame.
    
    Args:
        button (MouseButton): The mouse button to check.
    
    Returns:
        bool: True if the button was just released.
    """
def is_locked() -> bool:
    """
    Check if the mouse is currently locked to the window.
    
    Returns:
        bool: True if the mouse is locked.
    """
def is_pressed(button: pykraken._core.MouseButton) -> bool:
    """
    Check if a mouse button is currently pressed.
    
    Args:
        button (MouseButton): The mouse button to check (e.g., kn.MOUSE_LEFT).
    
    Returns:
        bool: True if the button is currently pressed.
    """
def lock() -> None:
    """
    Lock the mouse to the center of the window.
    
    Useful for first-person controls where you want to capture mouse movement
    without letting the cursor leave the window area.
    """
def show() -> None:
    """
    Show the mouse cursor if it was hidden.
    """
def unlock() -> None:
    """
    Unlock the mouse from the window, allowing it to move freely.
    """
