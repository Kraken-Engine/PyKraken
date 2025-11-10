"""
Viewport management functions
"""
from __future__ import annotations
import pykraken._core
import typing
__all__: list[str] = ['layout', 'set', 'unset']
def layout(count: typing.SupportsInt, mode: pykraken._core.ViewportMode = pykraken._core.ViewportMode.VERTICAL) -> list[pykraken._core.Rect]:
    """
    Layout the screen into multiple viewports.
    
    Args:
        count (int): The number of viewports to create (between 2 and 4).
        mode (ViewportMode, optional): The layout mode for 2 viewports (VERTICAL or HORIZONTAL).
                                  Defaults to VERTICAL.
    
    Returns:
        list[Rect]: A list of Rects representing the viewports.
    """
def set(rect: pykraken._core.Rect) -> None:
    """
    Set the current viewport to the given rectangle.
    
    Args:
        rect (Rect): The rectangle defining the viewport.
    """
def unset() -> None:
    """
    Unset the current viewport, reverting to the full rendering area.
    """
