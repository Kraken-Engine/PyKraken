"""
Gamepad input handling functions
"""
from __future__ import annotations
import pykraken._core
import typing
__all__: list[str] = ['get_connected_slots', 'get_deadzone', 'get_left_stick', 'get_left_trigger', 'get_right_stick', 'get_right_trigger', 'is_just_pressed', 'is_just_released', 'is_pressed', 'set_deadzone']
def get_connected_slots() -> list[int]:
    """
    Get a list of connected gamepad slot indices.
    
    Returns:
        list[int]: A list of slot IDs with active gamepads.
    """
def get_deadzone(slot: typing.SupportsInt = 0) -> float:
    """
    Get the current dead zone value for a gamepad's analog sticks.
    
    Args:
        slot (int, optional): Gamepad slot ID (default is 0).
    
    Returns:
        float: Deadzone threshold.
    """
def get_left_stick(slot: typing.SupportsInt = 0) -> pykraken._core.Vec2:
    """
    Get the left analog stick position.
    
    Args:
        slot (int, optional): Gamepad slot ID (default is 0).
    
    Returns:
        Vec2: A vector of stick input normalized to [-1, 1], or (0, 0) if inside dead zone.
    """
def get_left_trigger(slot: typing.SupportsInt = 0) -> float:
    """
    Get the left trigger's current pressure value.
    
    Args:
        slot (int, optional): Gamepad slot ID (default is 0).
    
    Returns:
        float: Trigger value in range [0.0, 1.0].
    """
def get_right_stick(slot: typing.SupportsInt = 0) -> pykraken._core.Vec2:
    """
    Get the right analog stick position.
    
    Args:
        slot (int, optional): Gamepad slot ID (default is 0).
    
    Returns:
        Vec2: A vector of stick input normalized to [-1, 1], or (0, 0) if inside dead zone.
    """
def get_right_trigger(slot: typing.SupportsInt = 0) -> float:
    """
    Get the right trigger's current pressure value.
    
    Args:
        slot (int, optional): Gamepad slot ID (default is 0).
    
    Returns:
        float: Trigger value in range [0.0, 1.0].
    """
def is_just_pressed(button: pykraken._core.GamepadButton, slot: typing.SupportsInt = 0) -> bool:
    """
    Check if a gamepad button was pressed during this frame.
    
    Args:
        button (GamepadButton): The button code.
        slot (int, optional): Gamepad slot ID (default is 0).
    
    Returns:
        bool: True if the button was just pressed.
    """
def is_just_released(button: pykraken._core.GamepadButton, slot: typing.SupportsInt = 0) -> bool:
    """
    Check if a gamepad button was released during this frame.
    
    Args:
        button (GamepadButton): The button code.
        slot (int, optional): Gamepad slot ID (default is 0).
    
    Returns:
        bool: True if the button was just released.
    """
def is_pressed(button: pykraken._core.GamepadButton, slot: typing.SupportsInt = 0) -> bool:
    """
    Check if a gamepad button is currently being held down.
    
    Args:
        button (GamepadButton): The button code.
        slot (int, optional): Gamepad slot ID (default is 0).
    
    Returns:
        bool: True if the button is pressed.
    """
def set_deadzone(deadzone: typing.SupportsFloat, slot: typing.SupportsInt = 0) -> None:
    """
    Set the dead zone threshold for a gamepad's analog sticks.
    
    Args:
        deadzone (float): Value from 0.0 to 1.0 where movement is ignored.
        slot (int, optional): Gamepad slot ID (default is 0).
    """
