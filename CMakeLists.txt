cmake_minimum_required(VERSION 3.15...3.27)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "Vcpkg toolchain file")
endif()

project(KrakenEngine LANGUAGES CXX)

if (NOT SKBUILD)
  message(WARNING "\
  This CMake file is meant to be executed using 'scikit-build-core'.
  Running it directly will almost certainly not produce the desired
  result. If you are a user trying to install this package, use the
  command below, which will install all necessary build dependencies,
  compile the package in an isolated environment, and then install it.
  =====================================================================
   $ pip install .
  =====================================================================
  If you are a software developer, and this is your own package, then
  it is usually much more efficient to install the build dependencies
  in your environment once and use the following command that avoids
  a costly creation of a new virtual environment at every compilation:
  =====================================================================
   $ pip install nanobind scikit-build-core[pyproject]
   $ pip install --no-build-isolation -ve .
  =====================================================================
  You may optionally add -Ceditable.rebuild=true to auto-rebuild when
  the package is imported. Otherwise, you need to rerun the above
  after editing C++ files.")
endif()

include(FetchContent)

find_package(Python 3.12
  REQUIRED COMPONENTS Interpreter Development.Module
  OPTIONAL_COMPONENTS Development.SABIModule)

find_package(nanobind CONFIG REQUIRED)

# SDL_mixer options (built via FetchContent, not vcpkg)
set(SDLMIXER_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(SDLMIXER_MIDI_NATIVE       OFF CACHE BOOL "" FORCE)
set(SDLMIXER_GME               OFF CACHE BOOL "" FORCE)
set(SDLMIXER_WAVPACK           OFF CACHE BOOL "" FORCE)
set(SDLMIXER_MOD               OFF CACHE BOOL "" FORCE)
set(SDLMIXER_MOD_XMP           OFF CACHE BOOL "" FORCE)
set(SDLMIXER_OPUS              ON CACHE BOOL "" FORCE)
set(SDLMIXER_MP3_MPG123        ON CACHE BOOL "" FORCE)
set(SDLMIXER_VORBIS_VORBISFILE ON CACHE BOOL "" FORCE)
set(SDLMIXER_FLAC_LIBFLAC      ON CACHE BOOL "" FORCE)
set(SDLMIXER_VENDORED          ON CACHE BOOL "" FORCE)

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)
find_package(box2d CONFIG REQUIRED)
find_package(PkgConfig REQUIRED)
# Point pkgconf at the release .pc files (vcpkg's debug .pc files link the -d variants)
set(ENV{PKG_CONFIG_PATH} "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/pkgconfig")
pkg_check_modules(tmxlite REQUIRED IMPORTED_TARGET tmxlite)
FetchContent_Declare(SDL_mixer
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
  GIT_TAG        prerelease-3.1.2
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(SDL_mixer)

nanobind_add_module(
  _pykraken

  # Target the stable ABI for Python 3.12+, which reduces
  # the number of binary wheels that must be built. This
  # does nothing on older Python versions
  STABLE_ABI

  src/animation_controller.cpp
  src/camera.cpp
  src/capsule.cpp
  src/circle.cpp
  src/collision.cpp
  src/color.cpp
  src/constants.cpp
  src/draw.cpp
  src/ease.cpp
  src/event.cpp
  src/font.cpp
  src/gamepad.cpp
  src/input.cpp
  src/key.cpp
  src/line.cpp
  src/log.cpp
  src/mask.cpp
  src/math.cpp
  src/mixer.cpp
  src/mouse.cpp
  src/orchestrator.cpp
  src/pixel_array.cpp
  src/polygon.cpp
  src/pykraken.cpp
  src/rect.cpp
  src/renderer.cpp
  src/shader_state.cpp
  src/text.cpp
  src/texture.cpp
  src/tile_map.cpp
  src/time.cpp
  src/transform.cpp
  src/viewport.cpp
  src/window.cpp
  src/physics/_bind_physics.cpp
  src/physics/world.cpp
  src/physics/bodies/body.cpp
  src/physics/bodies/character_body.cpp
  src/physics/bodies/rigid_body.cpp
  src/physics/bodies/static_body.cpp
  src/physics/joints/distance_joint.cpp
  src/physics/joints/filter_joint.cpp
  src/physics/joints/joint.cpp
  src/physics/joints/motor_joint.cpp
  src/physics/joints/mouse_joint.cpp
  src/physics/joints/prismatic_joint.cpp
  src/physics/joints/revolute_joint.cpp
  src/physics/joints/weld_joint.cpp
  src/physics/joints/wheel_joint.cpp
)

target_include_directories(_pykraken PRIVATE include)

set_target_properties(_pykraken PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

if (MSVC)
  target_compile_options(_pykraken PRIVATE /utf-8)
endif()

target_link_libraries(_pykraken PRIVATE
  SDL3::SDL3
  SDL3_image::SDL3_image
  SDL3_ttf::SDL3_ttf
  SDL3_mixer::SDL3_mixer
  PkgConfig::tmxlite
  box2d::box2d
)

# Copy runtime DLLs next to the module so it can be imported (needed for stubgen and local dev)
if(WIN32)
  set(_VCPKG_BIN_DIR "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin")
  file(GLOB _VCPKG_DLLS "${_VCPKG_BIN_DIR}/*.dll")
  add_custom_command(TARGET _pykraken POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:_pykraken> $<TARGET_FILE_DIR:_pykraken>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${_VCPKG_DLLS} $<TARGET_FILE_DIR:_pykraken>
    COMMAND_EXPAND_LISTS
  )
endif()

# Ensure the module is placed in the correct package directory for scikit-build-core
install(TARGETS _pykraken LIBRARY DESTINATION pykraken)
if(WIN32)
  install(FILES $<TARGET_RUNTIME_DLLS:_pykraken> ${_VCPKG_DLLS} DESTINATION pykraken)
endif()

nanobind_add_stub(
  pykraken_stub
  MODULE _pykraken
  OUTPUT _pykraken/__init__.pyi
  PYTHON_PATH $<TARGET_FILE_DIR:_pykraken>
  DEPENDS _pykraken
  MARKER_FILE _pykraken/py.typed
  RECURSIVE
)

# Copy stubs to source tree for IDE support (editable installs)
add_custom_target(copy_stubs ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    $<TARGET_FILE_DIR:_pykraken>/_pykraken
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pykraken/_pykraken
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_BINARY_DIR}/_pykraken/py.typed
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pykraken/py.typed
  DEPENDS pykraken_stub
  COMMENT "Copying stubs to source tree for IDE support"
)

# Install stubs for wheel builds
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/pykraken/_pykraken/ DESTINATION pykraken/_pykraken FILES_MATCHING PATTERN "*.pyi")
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/_pykraken/py.typed DESTINATION pykraken)
