# pykraken/__pyinstaller/hook-pykraken.py
import importlib.util

from PyInstaller.utils.hooks import (
    collect_data_files,
    collect_dynamic_libs,
    collect_submodules,
)

# PyInstaller looks for `hiddenimports` specifically (no underscore).
hiddenimports = collect_submodules("pykraken")
if "pykraken._pykraken" not in hiddenimports:
    hiddenimports.append("pykraken._pykraken")

binaries = collect_dynamic_libs("pykraken")  # SDL3/ttf/image libs

# Editable installs can place the extension outside `src/pykraken`.
_spec = importlib.util.find_spec("pykraken._pykraken")
if _spec and _spec.origin:
    binaries.append((_spec.origin, "pykraken"))

datas = collect_data_files("pykraken")  # default fonts/shaders/etc.
