"""Effect factory functions for Orchestrator timeline animations.

This module must be explicitly imported:
    from pykraken import fx

Example:
    from pykraken import fx

    orch = kn.Orchestrator(sprite)
    orch.parallel(
        fx.move_to(pos=(100, 100), dur=1.0),
        fx.scale_to(scale=2.0, dur=1.0),
    ).finalize()
    orch.play()
"""

from ._core import (
    _fx_move_to as move_to,
    _fx_scale_to as scale_to,
    _fx_rotate_to as rotate_to,
    _fx_shake as shake,
    _fx_call as call,
    _fx_wait as wait,
)

__all__ = ["move_to", "scale_to", "rotate_to", "shake", "call", "wait"]
