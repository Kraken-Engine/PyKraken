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
    "--enum-class-locations", "ViewportMode:pykraken._core"
]

print(f"Running command in: {root_dir}")
print(f"Command: {' '.join(command)}")

result = subprocess.run(command)
sys.exit(result.returncode)
