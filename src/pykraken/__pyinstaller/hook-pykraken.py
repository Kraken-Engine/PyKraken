# pykraken/__pyinstaller/hook-pykraken.py
from PyInstaller.utils.hooks import (
    collect_dynamic_libs, collect_data_files, collect_submodules
)

hidden_imports = collect_submodules("pykraken") # dynamic imports/plugins
binaries = collect_dynamic_libs("pykraken") # SDL3/ttf/image libs
datas = collect_data_files("pykraken") # default fonts/shaders/etc.
