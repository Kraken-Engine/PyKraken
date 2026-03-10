#!/bin/bash
# Build C++ dependencies from source for manylinux_2_28 wheel builds.
# SDL_mixer and tmxlite are handled by FetchContent in CMakeLists.txt.
set -euo pipefail

PREFIX=/opt/deps
JOBS=$(nproc)
CMAKE_COMMON="-DCMAKE_INSTALL_PREFIX=${PREFIX} -DCMAKE_PREFIX_PATH=${PREFIX} -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF"

# SDL3
git clone --depth 1 --branch release-3.4.0 https://github.com/libsdl-org/SDL.git /tmp/SDL3
cmake -S /tmp/SDL3 -B /tmp/SDL3/build ${CMAKE_COMMON} -DSDL_STATIC=ON -DSDL_SHARED=OFF
cmake --build /tmp/SDL3/build --target install --parallel "${JOBS}"

# SDL3_image
git clone --depth 1 --branch release-3.4.0 https://github.com/libsdl-org/SDL_image.git /tmp/SDL3_image
cmake -S /tmp/SDL3_image -B /tmp/SDL3_image/build ${CMAKE_COMMON} -DSDLIMAGE_BACKEND_STB=ON
cmake --build /tmp/SDL3_image/build --target install --parallel "${JOBS}"

# SDL3_ttf (vendored harfbuzz/freetype)
git clone --depth 1 --branch release-3.2.2 --recurse-submodules https://github.com/libsdl-org/SDL_ttf.git /tmp/SDL3_ttf
cmake -S /tmp/SDL3_ttf -B /tmp/SDL3_ttf/build ${CMAKE_COMMON} -DSDLTTF_VENDORED=ON
cmake --build /tmp/SDL3_ttf/build --target install --parallel "${JOBS}"

# box2d
git clone --depth 1 --branch v3.0.0 https://github.com/erincatto/box2d.git /tmp/box2d
cmake -S /tmp/box2d -B /tmp/box2d/build ${CMAKE_COMMON} -DBOX2D_SAMPLES=OFF -DBOX2D_UNIT_TESTS=OFF
cmake --build /tmp/box2d/build --target install --parallel "${JOBS}"
