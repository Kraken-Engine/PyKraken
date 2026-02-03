cmake_minimum_required(VERSION 3.23)
project(pykraken LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# -------------------------
# Project Version (from pyproject.toml)
# -------------------------
# SDL projects commonly generate version headers via CMake; we do the same,
# but source the version from Python packaging metadata.
# Use the unified FindPython module; request Development.Module so CMake defines
# the add-library helper needed by newer pybind11 toolchains.
find_package(Python COMPONENTS Interpreter Development.Module REQUIRED)

# Compatibility: pybind11's newer CMake files may call python_add_library()/python3_add_library(),
# while some CMake versions only provide Python_add_library()/Python3_add_library().
if (COMMAND Python_add_library AND NOT COMMAND python_add_library)
  function(python_add_library)
    Python_add_library(${ARGV})
  endfunction()
endif()
if (COMMAND Python3_add_library AND NOT COMMAND python3_add_library)
  function(python3_add_library)
    Python3_add_library(${ARGV})
  endfunction()
endif()
execute_process(
  COMMAND ${Python_EXECUTABLE} -c [=[
import pathlib
try:
    import tomllib as _toml  # py>=3.11
except ModuleNotFoundError:
    import tomli as _toml

with open("pyproject.toml", "rb") as f:
    data = _toml.load(f)
print(data["project"]["version"])
]=]
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  OUTPUT_VARIABLE PYK_PROJECT_VERSION
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

if (NOT PYK_PROJECT_VERSION)
  message(FATAL_ERROR "Failed to read [project].version from pyproject.toml")
endif()

# Extract numeric MAJOR.MINOR.MICRO (e.g. 1.6.1). If you later use pre-releases
# like 1.6.1rc1, the regex still captures 1.6.1.
string(REGEX MATCH "^([0-9]+)\\.([0-9]+)\\.([0-9]+)" _ver_match "${PYK_PROJECT_VERSION}")
if (NOT _ver_match)
  message(FATAL_ERROR "pyproject.toml version must start with MAJOR.MINOR.MICRO, got: ${PYK_PROJECT_VERSION}")
endif()
set(PYK_VERSION_MAJOR "${CMAKE_MATCH_1}")
set(PYK_VERSION_MINOR "${CMAKE_MATCH_2}")
set(PYK_VERSION_MICRO "${CMAKE_MATCH_3}")

if(CMAKE_VERSION VERSION_GREATER_EQUAL "4.0")
  set(CMAKE_POLICY_VERSION_MINIMUM "3.5" CACHE STRING "" FORCE)
endif()

# -------------------------
# Configuration
# -------------------------
set(PYK_MODULE_NAME    "_core"    CACHE STRING "Python extension module name")
set(PYK_PACKAGE_DIR    "pykraken" CACHE STRING "Where to install the module inside the wheel")
set(PYK_SOURCES_GLOB   "src/*.cpp;src/**/*.cpp" CACHE STRING "Glob(s) for C++ sources")
option(PYK_VENDOR_SDL  "Vendor SDL3, SDL_image, SDL_ttf, SDL_mixer (static) via FetchContent" ON)
option(PYK_VENDOR_TMXLITE "Vendor tmxlite (static) via FetchContent" ON)
option(PYK_VENDOR_PYBIND11 "Vendor pybind11 via FetchContent" OFF)

# -------------------------
# Tooling
# -------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if (MSVC)
  add_compile_definitions(NOMINMAX WIN32_LEAN_AND_MEAN)
  add_compile_options(/bigobj /utf-8)
endif()

include(FetchContent)

# -------------------------
# pybind11
# -------------------------
if (PYK_VENDOR_PYBIND11)
  FetchContent_Declare(
      pybind11
      GIT_REPOSITORY https://github.com/pybind/pybind11.git
      GIT_TAG        v3.0.1
  )
  FetchContent_MakeAvailable(pybind11)
else()
  find_package(pybind11 CONFIG REQUIRED)
endif()
file(GLOB_RECURSE PYK_SOURCES CONFIGURE_DEPENDS ${PYK_SOURCES_GLOB})
if (PYK_SOURCES STREQUAL "")
  message(FATAL_ERROR "No sources found. Adjust PYK_SOURCES_GLOB (currently: ${PYK_SOURCES_GLOB}).")
endif()
pybind11_add_module(${PYK_MODULE_NAME} MODULE ${PYK_SOURCES})

# Generate version header (available to all translation units)
set(_pyk_generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")
file(MAKE_DIRECTORY "${_pyk_generated_dir}")
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/pykraken_version.hpp.in
  ${_pyk_generated_dir}/pykraken_version_generated.hpp
  @ONLY
)

# Include project headers
target_include_directories(${PYK_MODULE_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${_pyk_generated_dir}
)

# On Windows, Python extensions are DLLs; on *nix they're .so
# Make sure CMake installs them to the package dir
set_target_properties(${PYK_MODULE_NAME} PROPERTIES
  OUTPUT_NAME        ${PYK_MODULE_NAME}
  CXX_VISIBILITY_PRESET hidden
  VISIBILITY_INLINES_HIDDEN ON
)

# -------------------------
# SDL3, SDL_image, SDL_ttf
# -------------------------
if (PYK_VENDOR_SDL)
  # Global: prefer static
  set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

  # SDL3 options (v3 repos)
  set(SDL_SHARED OFF CACHE BOOL "" FORCE)

  # SDL_image options (static, prune heavy codecs unless you need them)
  option(SDLIMAGE_ANI  "Support loading ANI animations" OFF)
  option(SDLIMAGE_AVIF "Support loading AVIF images"    OFF)
  option(SDLIMAGE_BMP  "Support loading BMP images"     ON)
  option(SDLIMAGE_GIF  "Support loading GIF images"     OFF)
  option(SDLIMAGE_JPG  "Support loading JPEG images"    ON)
  option(SDLIMAGE_JXL  "Support loading JXL images"     OFF)
  option(SDLIMAGE_LBM  "Support loading LBM images"     OFF)
  option(SDLIMAGE_PCX  "Support loading PCX images"     OFF)
  option(SDLIMAGE_PNG  "Support loading PNG images"     ON)
  option(SDLIMAGE_PNM  "Support loading PNM images"     OFF)
  option(SDLIMAGE_QOI  "Support loading QOI images"     ON)
  option(SDLIMAGE_SVG  "Support loading SVG images"     ON)
  option(SDLIMAGE_TGA  "Support loading TGA images"     ON)
  option(SDLIMAGE_TIF  "Support loading TIFF images"    OFF)
  option(SDLIMAGE_WEBP "Support loading WEBP images"    OFF)
  option(SDLIMAGE_XCF  "Support loading XCF images"     OFF)
  option(SDLIMAGE_XPM  "Support loading XPM images"     OFF)
  option(SDLIMAGE_XV   "Support loading XV images"      OFF)

  # SDL_ttf options (static, vendored FreeType)
  set(SDLTTF_SHARED   OFF CACHE BOOL "" FORCE)
  set(SDLTTF_VENDORED ON  CACHE BOOL "" FORCE)

  # SDL_mixer options
  set(SDLMIXER_MIDI_NATIVE       OFF)
  set(SDLMIXER_GME               OFF)
  set(SDLMIXER_WAVPACK           OFF)
  set(SDLMIXER_MOD               OFF)
  set(SDLMIXER_MOD_XMP           OFF)
  set(SDLMIXER_OPUS              OFF)
  set(SDLMIXER_MP3_MPG123        ON)
  set(SDLMIXER_VORBIS_VORBISFILE ON)
  set(SDLMIXER_FLAC_LIBFLAC      ON)
  set(SDLMIXER_VENDORED ON CACHE BOOL "" FORCE)

  FetchContent_Declare(SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        release-3.4.0
    GIT_SHALLOW    TRUE
  )
  FetchContent_Declare(SDL_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG        release-3.4.0
    GIT_SHALLOW    TRUE
  )
  FetchContent_Declare(SDL_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG        release-3.2.2
    GIT_SHALLOW    TRUE
  )
  FetchContent_Declare(SDL_mixer
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
    GIT_TAG        prerelease-3.1.2
    GIT_SHALLOW    TRUE
  )

  FetchContent_MakeAvailable(SDL3 SDL_image SDL_ttf SDL_mixer)
else()
  find_package(SDL3       CONFIG REQUIRED)
  find_package(SDL3_image CONFIG REQUIRED)
  find_package(SDL3_ttf   CONFIG REQUIRED)
  find_package(SDL3_mixer CONFIG REQUIRED)
endif()

# Prefer static targets when they exist; else use shared
set(_sdl_targets)
foreach(lib SDL3 SDL3_image SDL3_ttf SDL3_mixer)
  if (TARGET ${lib}::${lib}-static)
    list(APPEND _sdl_targets ${lib}::${lib}-static)
  elseif (TARGET ${lib}::${lib})
    list(APPEND _sdl_targets ${lib}::${lib})
  else()
    message(FATAL_ERROR "Could not locate imported target for ${lib}.")
  endif()
endforeach()

# -------------------------
# tmxlite
# -------------------------
if (PYK_VENDOR_TMXLITE)
  set(TMXLITE_STATIC_LIB ON CACHE BOOL "" FORCE)
  FetchContent_Declare(tmxlite
      GIT_REPOSITORY https://github.com/fallahn/tmxlite.git
      GIT_TAG        v1.4.4
      GIT_SHALLOW    TRUE
      SOURCE_SUBDIR  "tmxlite"
  )
  FetchContent_MakeAvailable(tmxlite)

#   if (TARGET tmxlite)
#     target_include_directories(${PYK_MODULE_NAME} PRIVATE
#       $<TARGET_PROPERTY:tmxlite,INTERFACE_INCLUDE_DIRECTORIES>
#     )
#   else()
#     message(FATAL_ERROR "Vendored tmxlite target was not created!")
#   endif()
else()
  find_package(tmxlite CONFIG REQUIRED)
endif()

# -------------------------
# Link libraries
# -------------------------
target_link_libraries(${PYK_MODULE_NAME} PRIVATE
  ${_sdl_targets}
  tmxlite
)

# -------------------------
# RPATH / loader paths (so side-by-side deps are found)
# -------------------------
if (APPLE)
  # @loader_path = directory of the .so/.dylib at runtime
  set_target_properties(${PYK_MODULE_NAME} PROPERTIES
    INSTALL_RPATH "@loader_path"
    BUILD_WITH_INSTALL_RPATH OFF
  )
elseif(UNIX)
  # $ORIGIN = directory of the .so at runtime
  set_target_properties(${PYK_MODULE_NAME} PROPERTIES
    INSTALL_RPATH "$ORIGIN"
    BUILD_WITH_INSTALL_RPATH OFF
  )
endif()

# -------------------------
# Install (into your Python package)
# -------------------------
# On Windows, RUNTIME is the correct slot; on UNIX, LIBRARY covers .so
install(TARGETS ${PYK_MODULE_NAME}
  LIBRARY DESTINATION ${PYK_PACKAGE_DIR}
  RUNTIME DESTINATION ${PYK_PACKAGE_DIR}
  ARCHIVE DESTINATION ${PYK_PACKAGE_DIR} # not typically needed for modules, harmless
)

# If shared libs are in play on Windows (e.g., using system SDL shared),
# bundle all transitive runtime DLLs next to the extension to avoid ImportError.
if (WIN32)
  install(FILES $<TARGET_RUNTIME_DLLS:${PYK_MODULE_NAME}>
          DESTINATION ${PYK_PACKAGE_DIR}
          COMPONENT Runtime)

  # Fixed: use a normal quoted string and escape inner stuff
  install(CODE "
    file(GET_RUNTIME_DEPENDENCIES
         RESOLVED_DEPENDENCIES_VAR resolved
         UNRESOLVED_DEPENDENCIES_VAR unresolved
         EXECUTABLES \"$<TARGET_FILE:${PYK_MODULE_NAME}>\")
    foreach(f \${resolved})
      message(STATUS \"[bundle] \${f}\")
    endforeach()
    foreach(f \${unresolved})
      message(WARNING \"[unresolved] \${f}\")
    endforeach()
  ")
endif()
