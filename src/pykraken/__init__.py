from ._pykraken import *
from .shader_uniform import ShaderUniform

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
    try:
        from importlib.metadata import version
        ver = version("kraken-engine")
        log.info(f"Kraken Engine v{ver}")
    except Exception:
        pass

init = _init
