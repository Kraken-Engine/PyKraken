"""
Input event handling
"""
from __future__ import annotations
import pykraken._core
__all__: list[str] = ['poll']
def poll() -> list[pykraken._core.Event]:
    """
    Poll for all pending user input events.
    
    This clears input states and returns a list of events that occurred since the last call.
    
    Returns:
        list[Event]: A list of input event objects.
    """
