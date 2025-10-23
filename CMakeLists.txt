cmake_minimum_required(VERSION 3.23) # 3.21+ for TARGET_RUNTIME_DLLS; 3.23 is a comfy floor
project(pykraken LANGUAGES CXX)

if(CMAKE_VERSION VERSION_GREATER_EQUAL "4.0")
  set(CMAKE_POLICY_VERSION_MINIMUM "3.5" CACHE STRING "" FORCE)
endif()

# -------------------------
# Configuration (edit me)
# -------------------------
set(PYK_MODULE_NAME    "_core"    CACHE STRING "Python extension module name")
set(PYK_PACKAGE_DIR    "pykraken" CACHE STRING "Where to install the module inside the wheel")
set(PYK_SOURCES_GLOB   "src/*.cpp;src/**/*.cpp" CACHE STRING "Glob(s) for C++ sources")
option(PYK_VENDOR_SDL  "Vendor SDL3, SDL_image, SDL_ttf (static) via FetchContent" ON)

# -------------------------
# Tooling
# -------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Helpful MSVC flags
if (MSVC)
  add_compile_definitions(NOMINMAX WIN32_LEAN_AND_MEAN)
  add_compile_options(/bigobj)
endif()

# -------------------------
# pybind11
# -------------------------
# Expect pybind11 to be provided by your build system (e.g., scikit-build-core)
find_package(pybind11 CONFIG REQUIRED)
# We’ll use pybind11's helper to create a proper Python extension
# (respects SOABI and naming automatically in modern pybind11)
file(GLOB_RECURSE PYK_SOURCES CONFIGURE_DEPENDS ${PYK_SOURCES_GLOB})
if (PYK_SOURCES STREQUAL "")
  message(FATAL_ERROR "No sources found. Adjust PYK_SOURCES_GLOB (currently: ${PYK_SOURCES_GLOB}).")
endif()
pybind11_add_module(${PYK_MODULE_NAME} MODULE ${PYK_SOURCES})

# Include project headers
target_include_directories(${PYK_MODULE_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
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
include(FetchContent)

# Common toggle: we prefer static libs in vendor mode to avoid DLL drama.
# Also, if system-config packages are present, we’ll prefer *-static imported targets if available.
if (PYK_VENDOR_SDL)
  # Global: prefer static
  set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

  # SDL3 options (v3 repos)
  set(SDL_SHARED OFF CACHE BOOL "" FORCE)

  # SDL_image options (static, prune heavy codecs unless you need them)
  set(SDLIMAGE_VENDORED ON CACHE BOOL "" FORCE)
  set(SDLIMAGE_SHARED   OFF CACHE BOOL "" FORCE)
  set(SDLIMAGE_AVIF     OFF CACHE BOOL "" FORCE)
  set(SDLIMAGE_BMP      ON  CACHE BOOL "" FORCE)
  set(SDLIMAGE_GIF      ON  CACHE BOOL "" FORCE)
  set(SDLIMAGE_JPG      ON  CACHE BOOL "" FORCE)
  set(SDLIMAGE_PNG      ON  CACHE BOOL "" FORCE)
  set(SDLIMAGE_TIF      OFF CACHE BOOL "" FORCE)
  set(SDLIMAGE_WEBP     OFF CACHE BOOL "" FORCE)
  set(SDLIMAGE_JXL      OFF CACHE BOOL "" FORCE)
  set(SDLIMAGE_QOI      ON  CACHE BOOL "" FORCE)

  # SDL_ttf options (static, vendored FreeType)
  set(SDLTTF_SHARED   OFF CACHE BOOL "" FORCE)
  set(SDLTTF_VENDORED ON  CACHE BOOL "" FORCE)

  # SDL3
  FetchContent_Declare(SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        release-3.2.24 # or a tag you trust
    GIT_SHALLOW    TRUE
  )
  # SDL_image
  FetchContent_Declare(SDL_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG        release-3.2.4
    GIT_SHALLOW    TRUE
  )
  # SDL_ttf
  FetchContent_Declare(SDL_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG        release-3.2.2
    GIT_SHALLOW    TRUE
  )

  FetchContent_MakeAvailable(SDL3 SDL_image SDL_ttf)
else()
  find_package(SDL3      CONFIG REQUIRED)
  find_package(SDL3_image CONFIG REQUIRED)
  find_package(SDL3_ttf   CONFIG REQUIRED)
endif()

# Prefer static targets when they exist; else use shared
set(_sdl_targets)
foreach(lib SDL3 SDL3_image SDL3_ttf)
  if (TARGET ${lib}::${lib}-static)
    list(APPEND _sdl_targets ${lib}::${lib}-static)
  elseif (TARGET ${lib}::${lib})
    list(APPEND _sdl_targets ${lib}::${lib})
  else()
    message(FATAL_ERROR "Could not locate imported target for ${lib}.")
  endif()
endforeach()

target_link_libraries(${PYK_MODULE_NAME} PRIVATE
  ${_sdl_targets}
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
