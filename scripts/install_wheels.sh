#!/usr/bin/env bash
set -e

echo "--- Upgrading pip ---"
python -m pip install --upgrade pip

echo "--- Searching for wheels ---"
# This finds any .whl file inside the dist folder, even in subdirectories
WHEEL=$(find dist -name "*.whl" | head -n 1)

if [ -z "$WHEEL" ]; then
    echo "Error: No .whl files found in dist/"
    # Let's list what IS there to help debug
    echo "Current directory contents:"
    ls -R
    exit 1
fi

echo "Installing: $WHEEL"
pip install "$WHEEL"

echo "--- Installing dev-requirements ---"
if [ -f "dev-requirements.txt" ]; then
    pip install -r dev-requirements.txt
fi
