"""
Mouse related functions
"""
from __future__ import annotations
__all__ = ['get_pos', 'get_rel', 'hide', 'is_hidden', 'is_just_pressed', 'is_just_released', 'is_locked', 'is_pressed', 'lock', 'show', 'unlock']
def get_pos(*args, **kwargs):
    """
    
            Get the current position of the mouse cursor.
    
            :return: The current mouse position.
            :rtype: ``tuple[float, float]``
        
    """
def get_rel(*args, **kwargs):
    """
    
            Get the relative mouse movement since the last frame.
    
            :return: The relative movement of the mouse.
            :rtype: ``tuple[float, float]``
        
    """
def hide(*args, **kwargs):
    """
    
            Hide the mouse cursor from view.
        
    """
def is_hidden(*args, **kwargs):
    """
    
            Check if the mouse cursor is currently hidden.
    
            :return: True if the cursor is hidden.
            :rtype: ``bool``
        
    """
def is_just_pressed(*args, **kwargs):
    """
    
            Check if a mouse button was pressed this frame.
    
            :param button: The mouse button to check.
            :type button: MouseButton
            :return: True if the button was just pressed.
            :rtype: ``bool``
        
    """
def is_just_released(*args, **kwargs):
    """
    
            Check if a mouse button was released this frame.
    
            :param button: The mouse button to check.
            :type button: MouseButton
            :return: True if the button was just released.
            :rtype: ``bool``
        
    """
def is_locked(*args, **kwargs):
    """
    
            Check if the mouse is currently locked to the window.
    
            :return: True if the mouse is locked.
            :rtype: ``bool``
        
    """
def is_pressed(*args, **kwargs):
    """
    
            Check if a mouse button is currently pressed.
    
            :param button: The mouse button to check (e.g., kn.MOUSE_LEFT).
            :type button: MouseButton
            :return: True if the button is currently pressed.
            :rtype: ``bool``
        
    """
def lock(*args, **kwargs):
    """
    
            Lock the mouse to the center of the window. Useful for first-person controls.
        
    """
def show(*args, **kwargs):
    """
    
            Show the mouse cursor if it was hidden.
        
    """
def unlock(*args, **kwargs):
    """
    
            Unlock the mouse from the window, allowing it to move freely.
        
    """
