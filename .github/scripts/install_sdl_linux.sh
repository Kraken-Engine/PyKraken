#!/bin/bash
set -e
INSTALL_DIR="/usr/local"

# Install build dependencies available in the manylinux container
yum -y install \
    cmake ninja-build \
    alsa-lib-devel pipewire-devel pulseaudio-libs-devel \
    libXrandr-devel libXcursor-devel libXi-devel libXinerama-devel libXfixes-devel libXScrnSaver-devel libXtst-devel \
    mesa-libEGL-devel mesa-libgbm-devel \
    wayland-devel wayland-protocols-devel libxkbcommon-devel \
    dbus-devel ibus-devel systemd-devel \
    libpng-devel libjpeg-turbo-devel freetype-devel flac-devel

# Function to build and install
build_and_install() {
    local url=$1
    local name=$2
    local extra_args=$3
    
    echo "Downloading $name..."
    curl -L "$url" | tar xz
    cd "$name"
    
    echo "Building $name..."
    cmake -S . -B build -G Ninja \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX="$INSTALL_DIR" \
        -DBUILD_SHARED_LIBS=OFF \
        -DSDL_STATIC=ON \
        -DSDL_SHARED=OFF \
        $extra_args
        
    cmake --build build --target install
    cd ..
    rm -rf "$name"
}

# SDL3
build_and_install \
    "https://github.com/libsdl-org/SDL/releases/download/release-3.4.0/SDL3-3.4.0.tar.gz" \
    "SDL3-3.4.0" \
    ""

# SDL3_image
build_and_install \
    "https://github.com/libsdl-org/SDL_image/releases/download/release-3.4.0/SDL3_image-3.4.0.tar.gz" \
    "SDL3_image-3.4.0" \
    "-DSDLIMAGE_BACKEND_STB=ON"

# SDL3_ttf
build_and_install \
    "https://github.com/libsdl-org/SDL_ttf/releases/download/release-3.2.2/SDL3_ttf-3.2.2.tar.gz" \
    "SDL3_ttf-3.2.2" \
    ""

# SDL3_mixer
build_and_install \
    "https://github.com/libsdl-org/SDL_mixer/releases/download/prerelease-3.1.2/SDL3_mixer-3.1.2.tar.gz" \
    "SDL3_mixer-3.1.2" \
    ""

echo "SDL dependencies installed to $INSTALL_DIR"
