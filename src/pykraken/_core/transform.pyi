"""

Submodule for Transform-related functionality.
        
"""
from __future__ import annotations
import pykraken._core
__all__: list[str] = ['compose', 'compose_chain']
def compose(*args) -> pykraken._core.Transform:
    """
    Compose multiple Transform objects in order and return the resulting Transform in world space.
    The first transform is treated as already in world space; each subsequent transform is local to the previous.
    
    Args:
        *transforms: Two or more Transform objects to compose.
    
    Returns:
        Transform: The composed Transform in world space.
    """
def compose_chain(*args) -> list[pykraken._core.Transform]:
    """
    Returns a list of cumulative world-space transforms excluding the initial input.
    
    Args:
        *transforms: Two or more Transform objects to compose.
    
    Returns:
        list[Transform]: The composed Transforms for inputs 2..N in world space.
    """
