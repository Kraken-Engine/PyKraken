#!/usr/bin/env bash
set -e

echo "--- Searching for wheels ---"
# This finds any .whl file inside the dist folder, even in subdirectories
WHEEL=$(find dist -name "*.whl" | head -n 1)

if [ -n "$WHEEL" ]; then
    echo "Installing wheel file: $WHEEL"
    pip install "$WHEEL"
else
    echo "Error: No wheel file or unpacked metadata found."
    ls -R dist
    exit 1
fi

echo "--- Installing dev-requirements ---"
if [ -f "dev-requirements.txt" ]; then
    pip install -r dev-requirements.txt
fi
