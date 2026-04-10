import os

# Ensure `src/` is on sys.path so tests can import pure-python packages.
# For native extensions, it's safer to rely on the installed wheel or `-e .`
ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
SRC = os.path.join(ROOT, "src")
