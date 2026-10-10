from ._pykraken import *


def init(debug: bool = False) -> None:
    """Initialize the Kraken engine subsystems.

    Args:
        debug (bool): When True, enables logging outputs.

    Raises:
        RuntimeError: If SDL initialization fails.
    """
