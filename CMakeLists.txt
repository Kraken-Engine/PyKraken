cmake_minimum_required(VERSION 3.24...3.31)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Detect scikit-build (pip) vs standalone CMake
if (DEFINED SKBUILD)
    set(_PK_NAME ${SKBUILD_PROJECT_NAME})
    set(_PK_VERSION ${SKBUILD_PROJECT_VERSION})
    set(_PK_SKBUILD ON)
else ()
    set(_PK_NAME pykraken)
    set(_PK_VERSION 0.0.0)
    set(_PK_SKBUILD OFF)
endif ()

project(${_PK_NAME} VERSION ${_PK_VERSION} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Use FindPython from pybind11
set(PYBIND11_FINDPYTHON ON)

# -------------------- Dependencies --------------------

if (_PK_SKBUILD)
    # pip/scikit-build: expect packages available in build env
    find_package(pybind11 CONFIG REQUIRED)
else ()
    # Standalone: vendor pybind11 so CLion always has a target
    include(FetchContent)
    FetchContent_Declare(pybind11
            GIT_REPOSITORY https://github.com/pybind/pybind11.git
            GIT_TAG v3.0.1
    )
    FetchContent_MakeAvailable(pybind11)

    # Make sure we bind against the Python you're going to run
    find_package(Python3 COMPONENTS Interpreter Development.Module REQUIRED)
endif ()

# SDL via environment hints (works in BOTH modes)
# Point these env vars to either the package root OR the cmake config dir.
# Examples (Windows):
#   SDL3_DIR=C:/SDL3/lib/cmake/SDL3
#   SDL3_image_DIR=C:/SDL3_image/lib/cmake/SDL3_image
foreach (VAR IN ITEMS SDL3_DIR SDL3_ROOT SDL3_image_DIR SDL3_IMAGE_DIR SDL3_IMAGE_ROOT SDL3_ttf_DIR SDL3_TTF_DIR SDL3_TTF_ROOT)
    if (DEFINED ENV{${VAR}} AND NOT DEFINED ${VAR})
        set(${VAR} "$ENV{${VAR}}")
        # Also help CMake search paths when a root was given
        list(PREPEND CMAKE_PREFIX_PATH "${${VAR}}")
    endif ()
endforeach ()

option(KRAKEN_VENDOR_SDL "Vendor SDL3(+image,+ttf) if not all found" ON)

find_package(SDL3 CONFIG QUIET)
find_package(SDL3_image CONFIG QUIET)
find_package(SDL3_ttf CONFIG QUIET)

if (KRAKEN_VENDOR_SDL AND (NOT SDL3_FOUND OR NOT SDL3_image_FOUND OR NOT SDL3_ttf_FOUND))
    include(FetchContent)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

    if (NOT SDL3_FOUND)
        FetchContent_Declare(SDL3
                GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
                GIT_TAG release-3.2.22
                GIT_SHALLOW TRUE
                OVERRIDE_FIND_PACKAGE
        )
        FetchContent_MakeAvailable(SDL3)
    endif ()

    if (NOT SDL3_image_FOUND)
        set(SDLIMAGE_AVIF OFF CACHE BOOL "" FORCE)
        set(SDLIMAGE_JXL OFF CACHE BOOL "" FORCE)
        set(SDLIMAGE_TIF OFF CACHE BOOL "" FORCE)
        set(SDLIMAGE_VENDORED ON CACHE BOOL "" FORCE)
        FetchContent_Declare(SDL3_image
                GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
                GIT_TAG release-3.2.4
                GIT_SHALLOW TRUE
                OVERRIDE_FIND_PACKAGE
        )
        FetchContent_MakeAvailable(SDL3_image)
    endif ()

    if (NOT SDL3_ttf_FOUND)
        set(SDLTTF_VENDORED ON CACHE BOOL "" FORCE)
        FetchContent_Declare(SDL3_ttf
                GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
                GIT_TAG release-3.2.2
                GIT_SHALLOW TRUE
                OVERRIDE_FIND_PACKAGE
        )
        FetchContent_MakeAvailable(SDL3_ttf)
    endif ()
endif ()

# -------------------- Sources --------------------
set(_CORE_SOURCES
        src/_core.cpp
        src/gfx/SDL3_gfxPrimitives.cpp
        src/gfx/SDL3_rotozoom.cpp
        src/pugixml/pugixml.cpp
        src/animation_controller.cpp
        src/camera.cpp
        src/circle.cpp
        src/color.cpp
        src/constants.cpp
        src/draw.cpp
        src/ease.cpp
        src/event.cpp
        src/gamepad.cpp
        src/input.cpp
        src/key.cpp
        src/line.cpp
        src/mask.cpp
        src/math.cpp
        src/mixer.cpp
        src/mouse.cpp
        src/pixel_array.cpp
        src/polygon.cpp
        src/rect.cpp
        src/renderer.cpp
        src/texture.cpp
        src/tile_map.cpp
        src/time.cpp
        src/transform.cpp
        src/window.cpp
)

# -------------------- Python extension --------------------
pybind11_add_module(_core MODULE ${_CORE_SOURCES} WITH_SOABI)
target_compile_features(_core PRIVATE cxx_std_20)

target_include_directories(_core PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(_core PRIVATE
        pybind11::headers
        SDL3::SDL3
        SDL3_image::SDL3_image
        SDL3_ttf::SDL3_ttf
)

if (MSVC)
    target_compile_options(_core PRIVATE /bigobj)
endif ()

# Install path works for both pip and standalone 'cmake --install'
install(TARGETS _core
        LIBRARY DESTINATION pykraken
        RUNTIME DESTINATION pykraken
        ARCHIVE DESTINATION pykraken
)
