#!/bin/bash
set -e

echo "Installing SDL3 dependencies via Homebrew..."
brew install sdl3 sdl3_image sdl3_ttf pkg-config cmake ninja

# SDL3_mixer is not yet in Homebrew (as of Feb 2026), so we build it from source
echo "Building SDL3_mixer..."
MIXER_VER="3.1.2"
curl -L "https://github.com/libsdl-org/SDL_mixer/releases/download/prerelease-${MIXER_VER}/SDL3_mixer-${MIXER_VER}.tar.gz" | tar xz

cd "SDL3_mixer-${MIXER_VER}"
# Install to a local prefix or /usr/local if possible. 
# On GitHub Actions macOS runners, /usr/local is writable.
cmake -S . -B build -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="/usr/local" \
    -DBUILD_SHARED_LIBS=ON

cmake --build build --target install
cd ..
rm -rf "SDL3_mixer-${MIXER_VER}"

echo "SDL dependencies installed"
