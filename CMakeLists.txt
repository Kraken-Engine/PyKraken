cmake_minimum_required(VERSION 3.15...3.27)

# If you are a user trying to install this package, use the
# command below, which will install all necessary build dependencies,
# compile the package in an isolated environment, and then install it.
# =====================================================================
#   $ pip install .
# =====================================================================
# If you are a software developer, then
# it is usually much more efficient to install the build dependencies
# in your environment once and use the following command that avoids
# a costly creation of a new virtual environment at every compilation:
# =====================================================================
#   $ pip install nanobind scikit-build-core[pyproject]
#   $ pip install --no-build-isolation -ve .
# =====================================================================
# You may optionally add -Ceditable.rebuild=true to auto-rebuild when
# the package is imported. Otherwise, you need to rerun the above
# after editing C++ files.

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

# Local find modules (for module-mode package discovery).
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

if(SKBUILD)
  set(KRAKEN_BUILD_PYTHON ON CACHE BOOL "Build Python bindings via nanobind" FORCE)
else()
  option(KRAKEN_BUILD_PYTHON "Build Python bindings via nanobind" OFF)
endif()

if(KRAKEN_BUILD_PYTHON)
  message(STATUS "KrakenEngine: Building Python bindings (_pykraken)")
else()
  message(STATUS "KrakenEngine: Building as C++ library (Kraken::Kraken)")
endif()

include(FetchContent)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "KrakenEngine only supports static builds" FORCE)

option(KRAKEN_VENDORED "Set default value for all *_VENDORED dependency options" OFF)

# Individual vendoring controls. KRAKEN_VENDORED only sets their default.
option(SDL3_VENDORED "Build SDL3 from source via FetchContent" ${KRAKEN_VENDORED})
option(SDL3_IMAGE_VENDORED "Build SDL3_image from source via FetchContent" ${KRAKEN_VENDORED})
option(SDL3_TTF_VENDORED "Build SDL3_ttf from source via FetchContent" ${KRAKEN_VENDORED})
option(SDL3_MIXER_VENDORED "Build SDL3_mixer from source via FetchContent" ${KRAKEN_VENDORED})
option(BOX2D_VENDORED "Build box2d from source via FetchContent" ${KRAKEN_VENDORED})
option(SPDLOG_VENDORED "Build spdlog from source via FetchContent" ${KRAKEN_VENDORED})
option(ZSTD_VENDORED "Build zstd from source via FetchContent" ${KRAKEN_VENDORED})
option(TMXLITE_VENDORED "Build tmxlite from source via FetchContent" ${KRAKEN_VENDORED})

message(STATUS "KrakenEngine: KRAKEN_VENDORED=${KRAKEN_VENDORED} (default for dependency *_VENDORED options)")
message(STATUS "KrakenEngine: SDL3_VENDORED=${SDL3_VENDORED}")
message(STATUS "KrakenEngine: SDL3_IMAGE_VENDORED=${SDL3_IMAGE_VENDORED}")
message(STATUS "KrakenEngine: SDL3_TTF_VENDORED=${SDL3_TTF_VENDORED}")
message(STATUS "KrakenEngine: SDL3_MIXER_VENDORED=${SDL3_MIXER_VENDORED}")
message(STATUS "KrakenEngine: BOX2D_VENDORED=${BOX2D_VENDORED}")
message(STATUS "KrakenEngine: SPDLOG_VENDORED=${SPDLOG_VENDORED}")
message(STATUS "KrakenEngine: ZSTD_VENDORED=${ZSTD_VENDORED}")
message(STATUS "KrakenEngine: TMXLITE_VENDORED=${TMXLITE_VENDORED}")

# =====================================================================
# COMMON DEPENDENCIES
# =====================================================================

# SDL3 --------------------------------------------
if(SDL3_VENDORED)
  set(SDL_SHARED OFF CACHE BOOL "" FORCE)
  set(SDL_STATIC ON CACHE BOOL "" FORCE)
  set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
  set(SDL_TESTS OFF CACHE BOOL "" FORCE)

  FetchContent_Declare(SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        release-3.4.0
    GIT_SHALLOW    TRUE
  )
  FetchContent_MakeAvailable(SDL3)
else()
  find_package(SDL3 CONFIG REQUIRED)
endif()

# SDL3_image --------------------------------------
if(SDL3_IMAGE_VENDORED)
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
else()
  find_package(SDL3_image CONFIG REQUIRED)
endif()

# SDL3_ttf ----------------------------------------
# Must be found *after* SDL_image is fetched/built on Windows CI.
# Otherwise a transitive dependency can define PNG::PNG first, and SDL_image's
# vendored libpng path will fail trying to create the same alias target.
if(SDL3_TTF_VENDORED)
  set(SDLTTF_VENDORED ON CACHE BOOL "" FORCE)

  FetchContent_Declare(SDL_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG        release-3.2.2
    GIT_SHALLOW    TRUE
  )
  FetchContent_MakeAvailable(SDL_ttf)
else()
  find_package(SDL3_ttf CONFIG REQUIRED)
endif()

# SDL3_mixer --------------------------------------
if(SDL3_MIXER_VENDORED)
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
else()
  find_package(SDL3_mixer CONFIG REQUIRED)
endif()

# Box2D -------------------------------------------
if(BOX2D_VENDORED)
  FetchContent_Declare(box2d
    GIT_REPOSITORY https://github.com/erincatto/box2d.git
    GIT_TAG        v3.1.1
    GIT_SHALLOW    TRUE
  )
  FetchContent_MakeAvailable(box2d)
else()
  find_package(box2d CONFIG REQUIRED)
endif()

# spdlog ------------------------------------------
if(SPDLOG_VENDORED)
  FetchContent_Declare(spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG        v1.17.0
    GIT_SHALLOW    TRUE
  )
  FetchContent_MakeAvailable(spdlog)
else()
  find_package(spdlog CONFIG REQUIRED)
endif()

# zstd --------------------------------------------
# Used by tmxlite and SDL image
if(ZSTD_VENDORED)
  set(ZSTD_BUILD_PROGRAMS OFF CACHE BOOL "" FORCE)
  set(ZSTD_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  set(ZSTD_BUILD_SHARED OFF CACHE BOOL "" FORCE)
  set(ZSTD_BUILD_STATIC ON CACHE BOOL "" FORCE)

  FetchContent_Declare(zstd
    GIT_REPOSITORY https://github.com/facebook/zstd.git
    GIT_TAG        v1.5.7
    GIT_SHALLOW    TRUE
    SOURCE_SUBDIR  build/cmake
  )
  FetchContent_MakeAvailable(zstd)
else()
  find_package(zstd CONFIG QUIET)
  if(NOT zstd_FOUND)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(ZSTD REQUIRED libzstd)

    add_library(zstd::zstd INTERFACE IMPORTED)
    target_include_directories(zstd::zstd INTERFACE ${ZSTD_INCLUDE_DIRS})
    target_link_libraries(zstd::zstd INTERFACE ${ZSTD_LIBRARIES})
  endif()
endif()

if(TMXLITE_VENDORED)
  FetchContent_Declare(tmxlite
    GIT_REPOSITORY https://github.com/fallahn/tmxlite.git
    GIT_TAG        v1.4.5
    GIT_SHALLOW    TRUE
    SOURCE_SUBDIR  tmxlite
  )
  set(TMXLITE_STATIC_LIB ON CACHE BOOL "" FORCE)
  set(USE_ZSTD ON CACHE BOOL "" FORCE)  # Big boy compression
  FetchContent_MakeAvailable(tmxlite)
  set_target_properties(tmxlite PROPERTIES POSITION_INDEPENDENT_CODE ON)
else()
  find_package(TMXLITE MODULE REQUIRED)
endif()

# =====================================================================
# SOURCES
# =====================================================================

set(KRAKEN_CORE_SOURCES
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
  src/ui/ui.cpp
)

set(KRAKEN_PYTHON_SOURCES
  src/pykraken.cpp
  src/physics/_bind_physics.cpp
)

# =====================================================================
# TARGET CONFIGURATION
# =====================================================================

if(KRAKEN_BUILD_PYTHON)
  find_package(Python 3.12
    REQUIRED COMPONENTS Interpreter Development.Module
    OPTIONAL_COMPONENTS Development.SABIModule)
  find_package(nanobind CONFIG REQUIRED)

  nanobind_add_module(_pykraken STABLE_ABI
    ${KRAKEN_CORE_SOURCES}
    ${KRAKEN_PYTHON_SOURCES}
  )

  # For ifndef guards
  target_compile_definitions(_pykraken PUBLIC KRAKEN_ENABLE_PYTHON)

  set(KRAKEN_TARGET _pykraken)
else()
  # Build standard C++ Library
  add_library(KrakenEngine STATIC ${KRAKEN_CORE_SOURCES})
  add_library(Kraken::Kraken ALIAS KrakenEngine)

  set(KRAKEN_TARGET KrakenEngine)
endif()

# =====================================================================
# COMMON TARGET PROPERTIES
# =====================================================================

target_include_directories(${KRAKEN_TARGET} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

set_target_properties(${KRAKEN_TARGET} PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

if (MSVC)
  target_compile_options(${KRAKEN_TARGET} PRIVATE /utf-8)
endif()

target_link_libraries(${KRAKEN_TARGET} PRIVATE
  SDL3::SDL3
  SDL3_image::SDL3_image
  SDL3_ttf::SDL3_ttf
  SDL3_mixer::SDL3_mixer
  tmxlite::tmxlite
  $<IF:$<TARGET_EXISTS:zstd::libzstd_static>,zstd::libzstd_static,
    $<IF:$<TARGET_EXISTS:zstd::zstd>,zstd::zstd,zstd::libzstd_shared>>
  box2d::box2d
  spdlog::spdlog
)

if(KRAKEN_BUILD_PYTHON)
  # =====================================================================
  # PYTHON-SPECIFIC INSTALLATION & STUBS
  # =====================================================================

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
else()
  # =====================================================================
  # C++ LIBRARY INSTALLATION & CMAKE EXPORT
  # =====================================================================

  include(GNUInstallDirs)

  # Install library and export target
  install(TARGETS KrakenEngine
    EXPORT KrakenEngineTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )

  # Install public headers
  install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )

  # Install custom Find modules
  install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/FindTMXLITE.cmake"
    DESTINATION ${CMAKE_INSTALL_DATADIR}/kraken-engine/modules
  )

  # Export targets file so find_package works
  install(EXPORT KrakenEngineTargets
    FILE KrakenEngineTargets.cmake
    NAMESPACE Kraken::
    DESTINATION ${CMAKE_INSTALL_DATADIR}/kraken-engine
  )

  # Generate a Config file that finds dependencies automatically
  include(CMakePackageConfigHelpers)
  set(CONFIG_FILE "${CMAKE_CURRENT_BINARY_DIR}/KrakenEngineConfig.cmake")

  # Because KrakenEngine is a static library, downstream consumers MUST
  # load these dependencies as well.
  file(WRITE ${CONFIG_FILE}
    # Temporarily add installed modules directory to the user's module path
    "list(APPEND CMAKE_MODULE_PATH \"\${CMAKE_CURRENT_LIST_DIR}/modules\")\n\n"

    "include(CMakeFindDependencyMacro)\n"

    "find_dependency(SDL3 CONFIG)\n"
    "find_dependency(SDL3_image CONFIG)\n"
    "find_dependency(SDL3_ttf CONFIG)\n"
    "find_dependency(SDL3_mixer CONFIG)\n"
    "find_dependency(box2d CONFIG)\n"
    "find_dependency(spdlog CONFIG)\n"
    "find_dependency(zstd CONFIG)\n"
    "find_dependency(TMXLITE MODULE)\n" # Assuming vcpkg provides a FindTMXLITE.cmake module

    "include(\"\${CMAKE_CURRENT_LIST_DIR}/KrakenEngineTargets.cmake\")\n"
  )

  install(FILES ${CONFIG_FILE}
    DESTINATION ${CMAKE_INSTALL_DATADIR}/kraken-engine
  )
endif()
