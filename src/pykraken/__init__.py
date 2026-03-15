from ._pykraken import *
from .shader_uniform import ShaderUniform

from importlib.metadata import version

_original_init = init

def _init(debug: bool = False) -> None:
    """
    Initialize the Kraken engine subsystems.

    Args:
        debug (bool): When True, enables logging outputs.

    Raises:
        RuntimeError: If initialization fails.
    """

    _original_init(debug=debug)
    ver = version("kraken-engine")
    log.info(f"Kraken Engine v{ver}")

init = _init
