cmake_minimum_required(VERSION 3.15...3.27)

# --- vcpkg integration (Windows CI and local dev) ---
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "Vcpkg toolchain file")

  if(DEFINED ENV{VCPKG_DEFAULT_TRIPLET} AND NOT DEFINED VCPKG_TARGET_TRIPLET)
    set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}" CACHE STRING "")
  endif()

  if(NOT DEFINED VCPKG_OVERLAY_TRIPLETS AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/triplets")
    set(VCPKG_OVERLAY_TRIPLETS "${CMAKE_CURRENT_SOURCE_DIR}/triplets" CACHE STRING "")
  endif()

  set(VCPKG_APPLOCAL_DEPS OFF CACHE BOOL "" FORCE)
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
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

find_package(Python 3.12
  REQUIRED COMPONENTS Interpreter Development.Module
  OPTIONAL_COMPONENTS Development.SABIModule)
find_package(nanobind CONFIG REQUIRED)

# SDL3 and friends --------------------------------------------
find_package(SDL3 CONFIG REQUIRED)

# find_package(SDL3_image CONFIG REQUIRED)
set(SDLIMAGE_ANI  OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_AVIF OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_BMP  OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_GIF  OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_LBM  OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_PCX  OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_PNM  OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_QOI  OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_TIF  OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_WEBP OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_XCF  OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_XPM  OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_XV   OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_VENDORED ON CACHE BOOL "" FORCE)
set(SDLIMAGE_PNG_SHARED OFF CACHE BOOL "" FORCE)
FetchContent_Declare(SDL_image
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
  GIT_TAG        release-3.4.0
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(SDL_image)

# NOTE:
# SDL3_ttf must be found *after* SDL_image is fetched/built on Windows CI.
# Otherwise a transitive dependency can define PNG::PNG first, and SDL_image's
# vendored libpng path will fail trying to create the same alias target.
find_package(SDL3_ttf CONFIG REQUIRED)

set(SDLMIXER_GME               OFF CACHE BOOL "" FORCE)
set(SDLMIXER_WAVPACK           OFF CACHE BOOL "" FORCE)
set(SDLMIXER_MOD               OFF CACHE BOOL "" FORCE)
set(SDLMIXER_MIDI              OFF CACHE BOOL "" FORCE)
set(SDLMIXER_AIFF              OFF CACHE BOOL "" FORCE)
set(SDLMIXER_VOC               OFF CACHE BOOL "" FORCE)
set(SDLMIXER_AU                OFF CACHE BOOL "" FORCE)
set(SDLMIXER_VENDORED          ON CACHE BOOL "" FORCE)
FetchContent_Declare(SDL_mixer
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
  GIT_TAG        release-3.2.0
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(SDL_mixer)
# ------------------------------------------------------------

find_package(box2d CONFIG REQUIRED)

# Used by tmxlite and SDL image
find_package(zstd CONFIG QUIET)
if(NOT zstd_FOUND)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(ZSTD REQUIRED libzstd)

  add_library(zstd::zstd INTERFACE IMPORTED)
  target_include_directories(zstd::zstd INTERFACE ${ZSTD_INCLUDE_DIRS})
  target_link_libraries(zstd::zstd INTERFACE ${ZSTD_LIBRARIES})
endif()

FetchContent_Declare(tmxlite
  GIT_REPOSITORY https://github.com/fallahn/tmxlite.git
  # Use the upstream branch (master) which includes fixes not present in the
  # v1.4.4 release (project version and CMake minimum updates).
  GIT_TAG        master
  GIT_SHALLOW    TRUE
  SOURCE_SUBDIR  tmxlite
)
set(TMXLITE_STATIC_LIB ON CACHE BOOL "" FORCE)
set(USE_ZSTD ON CACHE BOOL "" FORCE)  # Big boy compression
FetchContent_MakeAvailable(tmxlite)
set_target_properties(tmxlite PROPERTIES POSITION_INDEPENDENT_CODE ON)

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
  tmxlite
  $<IF:$<TARGET_EXISTS:zstd::libzstd_static>,zstd::libzstd_static,
    $<IF:$<TARGET_EXISTS:zstd::zstd>,zstd::zstd,zstd::libzstd_shared>>
  box2d::box2d
)

# Copy runtime DLLs next to the module (only needed with dynamic vcpkg triplets like x64-windows)
if(WIN32)
  set(_VCPKG_BIN_DIR "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin")
  file(GLOB _VCPKG_DLLS "${_VCPKG_BIN_DIR}/*.dll")
  if(_VCPKG_DLLS)
    add_custom_command(TARGET _pykraken POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${_VCPKG_DLLS} $<TARGET_FILE_DIR:_pykraken>
      COMMAND_EXPAND_LISTS
    )
  endif()
endif()

# Ensure the module is placed in the correct package directory for scikit-build-core
install(TARGETS _pykraken LIBRARY DESTINATION pykraken)
if(WIN32 AND _VCPKG_DLLS)
  install(FILES ${_VCPKG_DLLS} DESTINATION pykraken)
endif()

nanobind_add_stub(
  _pykraken_stub
  MODULE _pykraken

  PYTHON_PATH $<TARGET_FILE_DIR:_pykraken>
  DEPENDS _pykraken

  RECURSIVE

  OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}
  MARKER_FILE py.typed
)

add_custom_command(
  TARGET _pykraken_stub POST_BUILD
  COMMAND ${Python_EXECUTABLE} "${CMAKE_SOURCE_DIR}/scripts/clean_stubs.py" "${CMAKE_CURRENT_BINARY_DIR}/_pykraken"
  COMMENT "Running custom stub cleanup script..."
)

install(
  DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/_pykraken/
  DESTINATION pykraken
)
install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/py.typed
  DESTINATION pykraken
)
