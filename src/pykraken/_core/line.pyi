from __future__ import annotations
import pykraken._core
__all__: list[str] = ['move']
def move(line: pykraken._core.Line, offset: pykraken._core.Vec2) -> pykraken._core.Line:
    """
    Move the given line by a Vec2 or 2-element sequence.
    
    Args:
        offset (Vec2 | list[float]): The amount to move.
    """
