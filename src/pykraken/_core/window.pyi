"""
Window related functions
"""
from __future__ import annotations
import pykraken._core
__all__: list[str] = ['close', 'create', 'get_scale', 'get_size', 'get_title', 'is_fullscreen', 'is_open', 'save_screenshot', 'set_fullscreen', 'set_icon', 'set_title']
def close() -> None:
    """
    Close the window.
    
    Marks the window as closed, typically used to signal the main loop to exit.
    This doesn't destroy the window immediately but sets the close flag.
    """
def create(title: str, resolution: pykraken._core.Vec2, scaled: bool = False) -> None:
    """
    Create a window with specified title and size.
    
    Args:
        title (str): The window title. Must be non-empty and <= 255 characters.
        resolution (Vec2): The renderer resolution as (width, height).
        scaled (bool, optional): If True, creates a scaled up window using the
                                display's usable bounds, retaining the resolution's ratio.
                                Defaults to False.
    
    Raises:
        RuntimeError: If a window already exists or window creation fails.
        ValueError: If title is empty, exceeds 255 characters, or size values are <= 0.
    """
def get_scale() -> int:
    """
    Get the scale of the window relative to the renderer resolution.
    
    Returns:
        int: The window's scale
    
    Raises:
        RuntimeError: If the window is not initialized.
    """
def get_size() -> pykraken._core.Vec2:
    """
    Get the current size of the window.
    
    Returns:
        tuple[float, float]: The window size as (width, height).
    
    Raises:
        RuntimeError: If the window is not initialized.
    """
def get_title() -> str:
    """
    Get the current title of the window.
    
    Returns:
        str: The current window title.
    
    Raises:
        RuntimeError: If the window is not initialized.
    """
def is_fullscreen() -> bool:
    """
    Check if the window is in fullscreen mode.
    
    Returns:
        bool: True if the window is currently in fullscreen mode.
    
    Raises:
        RuntimeError: If the window is not initialized.
    """
def is_open() -> bool:
    """
    Check if the window is open.
    
    Returns:
        bool: True if the window is open and active.
    """
def save_screenshot(path: str) -> None:
    """
    Save a screenshot of the current frame to a file.
    
    Args:
        path (str): The path to save the screenshot to.
    """
def set_fullscreen(fullscreen: bool) -> None:
    """
    Set the fullscreen mode of the window.
    
    Args:
        fullscreen (bool): True to enable fullscreen mode, False for windowed mode.
    
    Raises:
        RuntimeError: If the window is not initialized.
    """
def set_icon(path: str) -> None:
    """
    Set the window icon from an image file.
    
    Args:
        path (str): The file path to the image to use as the icon.
    
    Raises:
        RuntimeError: If the window is not initialized or icon setting fails.
    """
def set_title(title: str) -> None:
    """
    Set the title of the window.
    
    Args:
        title (str): The new window title. Must be non-empty and <= 255 characters.
    
    Raises:
        RuntimeError: If the window is not initialized or title setting fails.
        ValueError: If title is empty or exceeds 255 characters.
    """
