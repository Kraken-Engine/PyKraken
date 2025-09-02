"""
Input handling and action binding
"""
from __future__ import annotations
import collections.abc
import pykraken._core
__all__: list[str] = ['bind', 'get_axis', 'get_direction', 'is_just_pressed', 'is_just_released', 'is_pressed', 'unbind']
def bind(name: str, actions: collections.abc.Sequence[pykraken._core.InputAction]) -> None:
    """
    Bind a name to a list of InputActions.
    
    Args:
        name (str): The identifier for this binding (e.g. "jump").
        actions (list[InputAction]): One or more InputActions to bind.
    """
def get_axis(negative: str, positive: str) -> float:
    """
    Get a 1D axis value based on two opposing input actions.
    
    Args:
        negative (str): Name of the negative direction action (e.g. "left").
        positive (str): Name of the positive direction action (e.g. "right").
    
    Returns:
        float: Value in range [-1.0, 1.0] based on input.
    """
def get_direction(up: str, right: str, down: str, left: str) -> pykraken._core.Vec2:
    """
    Get a directional vector based on named input actions.
    
    This is typically used for WASD-style or D-pad movement.
    
    Args:
        up (str): Name of action for upward movement.
        right (str): Name of action for rightward movement.
        down (str): Name of action for downward movement.
        left (str): Name of action for leftward movement.
    
    Returns:
        Vec2: A normalized vector representing the intended direction.
    """
def is_just_pressed(name: str) -> bool:
    """
    Check if the given action was just pressed this frame.
    
    Args:
        name (str): The name of the bound input.
    
    Returns:
        bool: True if pressed this frame only.
    """
def is_just_released(name: str) -> bool:
    """
    Check if the given action was just released this frame.
    
    Args:
        name (str): The name of the bound input.
    
    Returns:
        bool: True if released this frame only.
    """
def is_pressed(name: str) -> bool:
    """
    Check if the given action is currently being held.
    
    Args:
        name (str): The name of the bound input.
    
    Returns:
        bool: True if any action bound to the name is pressed.
    """
def unbind(name: str) -> None:
    """
    Unbind a previously registered input name.
    
    Args:
        name (str): The binding name to remove.
    """
