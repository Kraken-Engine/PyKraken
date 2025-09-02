"""
Keyboard key state checks
"""
from __future__ import annotations
import pykraken._core
import typing
__all__: list[str] = ['is_just_pressed', 'is_just_released', 'is_pressed']
@typing.overload
def is_just_pressed(scancode: pykraken._core.Scancode) -> bool:
    """
    Check if a key was pressed this frame (by scancode).
    
    Args:
        scancode (Scancode): The physical key.
    
    Returns:
        bool: True if the key was newly pressed.
    """
@typing.overload
def is_just_pressed(keycode: pykraken._core.Keycode) -> bool:
    """
    Check if a key was pressed this frame (by keycode).
    
    Args:
        keycode (Keycode): The symbolic key.
    
    Returns:
        bool: True if the key was newly pressed.
    """
@typing.overload
def is_just_released(scancode: pykraken._core.Scancode) -> bool:
    """
    Check if a key was released this frame (by scancode).
    
    Args:
        scancode (Scancode): The physical key.
    
    Returns:
        bool: True if the key was newly released.
    """
@typing.overload
def is_just_released(keycode: pykraken._core.Keycode) -> bool:
    """
    Check if a key was released this frame (by keycode).
    
    Args:
        keycode (Keycode): The symbolic key.
    
    Returns:
        bool: True if the key was newly released.
    """
@typing.overload
def is_pressed(scancode: pykraken._core.Scancode) -> bool:
    """
    Check if a key is currently held down (by scancode).
    
    Args:
        scancode (Scancode): The physical key (e.g., S_w).
    
    Returns:
        bool: True if the key is held.
    """
@typing.overload
def is_pressed(keycode: pykraken._core.Keycode) -> bool:
    """
    Check if a key is currently held down (by keycode).
    
    Args:
        keycode (Keycode): The symbolic key (e.g., K_SPACE).
    
    Returns:
        bool: True if the key is held.
    """
