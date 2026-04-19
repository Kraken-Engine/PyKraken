from ._pykraken import *

_original_init = init

def _init(debug: bool = False) -> None:
    """
    Initialize the Kraken engine subsystems.

    Args:
        debug (bool): When True, enables logging outputs.

    Raises:
        RuntimeError: If initialization fails.
    """
    if debug:
        import faulthandler
        faulthandler.enable()

    _original_init(debug=debug)

init = _init
