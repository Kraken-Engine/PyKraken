import subprocess
import sys
from pathlib import Path

# Get the root directory of the project (where this script is located)
root_dir = Path(__file__).parent

# Change to the root directory
import os
os.chdir(root_dir)

# Run the pybind11-stubgen command
command = [
    "pybind11-stubgen",
    "pykraken",
    "-o", "src",
    "--enum-class-locations", "Anchor:pykraken._core",
    "--enum-class-locations", "Type:pykraken._core.TileLayer",
    "--enum-class-locations", "ViewportMode:pykraken._core",
    "--enum-class-locations", "TextureAccess:pykraken._core",
    "--enum-class-locations", "TextureScaleMode:pykraken._core",
]

print(f"Running command in: {root_dir}")
print(f"Command: {' '.join(command)}")

result = subprocess.run(command)

if result.returncode == 0:
    print("Post-processing stubs...")
    src_dir = root_dir / "src"

    # Walk through the src directory to find generated .pyi files
    for pyi_file in src_dir.rglob("*.pyi"):
        # Check if a corresponding .py file exists
        py_file = pyi_file.with_suffix(".py")

        # Keep __init__.pyi as it is important for the package interface
        if pyi_file.name == "__init__.pyi":
            continue

        if py_file.exists():
            # Remove the generated .pyi file if a .py file exists
            # This prevents overwriting or shadowing existing python source files (e.g. fx.py, shader_uniform.py)
            print(f"Removing {pyi_file.relative_to(root_dir)} (shadows existing source)")
            pyi_file.unlink()

sys.exit(result.returncode)
