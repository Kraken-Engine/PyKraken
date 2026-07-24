if(SKBUILD)
  set(KRAKEN_BUILD_PYTHON ON CACHE BOOL "Build Python bindings via nanobind" FORCE)
else()
  option(KRAKEN_BUILD_PYTHON "Build Python bindings via nanobind" OFF)
endif()

if(KRAKEN_BUILD_PYTHON)
  message(STATUS "KrakenEngine: Building Python bindings (_pykraken)")
  set(KRAKEN_PYTHON_POOL_CAPACITY 128 CACHE STRING
      "Maximum cached Python wrappers per pooled type (0 disables pooling)")
  if(NOT KRAKEN_PYTHON_POOL_CAPACITY MATCHES "^[0-9]+$")
    message(FATAL_ERROR "KRAKEN_PYTHON_POOL_CAPACITY must be a non-negative integer")
  endif()
else()
  message(STATUS "KrakenEngine: Building as C++ library (Kraken::Kraken)")
  option(KRAKEN_INSTALL "Generate native C++ install and package-export rules" ON)
endif()

include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON CACHE BOOL
    "Skip remote updates for already-populated FetchContent dependencies" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "KrakenEngine only supports static builds" FORCE)

set(KRAKEN_DEPENDENCIES "AUTO" CACHE STRING
    "Dependency provider: AUTO preserves per-dependency settings, BUNDLED builds everything from source, SYSTEM uses installed packages")
set_property(CACHE KRAKEN_DEPENDENCIES PROPERTY STRINGS AUTO BUNDLED SYSTEM)
string(TOUPPER "${KRAKEN_DEPENDENCIES}" KRAKEN_DEPENDENCIES)
if(NOT KRAKEN_DEPENDENCIES MATCHES "^(AUTO|BUNDLED|SYSTEM)$")
  message(FATAL_ERROR "KRAKEN_DEPENDENCIES must be AUTO, BUNDLED, or SYSTEM")
endif()

option(KRAKEN_VENDORED "Set default value for all *_VENDORED dependency options" OFF)

option(SDL3_STACK_VENDORED       "Build all SDL3 deps from source" ON)
option(SDL3_VENDORED             "Build SDL3 from source"          ${SDL3_STACK_VENDORED})
option(SDL3_IMAGE_VENDORED       "Build SDL3_image from source"    ${SDL3_STACK_VENDORED})
option(SDL3_TTF_VENDORED         "Build SDL3_ttf from source"      ${SDL3_STACK_VENDORED})
option(SDL3_MIXER_VENDORED       "Build SDL3_mixer from source"    ${SDL3_STACK_VENDORED})
option(SDL3_SHADERCROSS_VENDORED "Build SDL3_shadercross"          OFF)
option(BOX2D_VENDORED            "Build box2d from source"         ${KRAKEN_VENDORED})
option(SPDLOG_VENDORED           "Build spdlog from source"        ${KRAKEN_VENDORED})
option(ZSTD_VENDORED             "Build zstd from source"          ${KRAKEN_VENDORED})
option(TMXLITE_VENDORED          "Build tmxlite from source"       ${KRAKEN_VENDORED})

if(KRAKEN_DEPENDENCIES STREQUAL "BUNDLED")
  foreach(option_name
      SDL3_VENDORED
      SDL3_IMAGE_VENDORED
      SDL3_TTF_VENDORED
      SDL3_MIXER_VENDORED
      BOX2D_VENDORED
      SPDLOG_VENDORED
      ZSTD_VENDORED
      TMXLITE_VENDORED)
    set(${option_name} ON CACHE BOOL "" FORCE)
  endforeach()
elseif(KRAKEN_DEPENDENCIES STREQUAL "SYSTEM")
  foreach(option_name
      SDL3_VENDORED
      SDL3_IMAGE_VENDORED
      SDL3_TTF_VENDORED
      SDL3_MIXER_VENDORED
      SDL3_SHADERCROSS_VENDORED
      BOX2D_VENDORED
      SPDLOG_VENDORED
      ZSTD_VENDORED
      TMXLITE_VENDORED)
    set(${option_name} OFF CACHE BOOL "" FORCE)
  endforeach()
endif()

message(STATUS "KrakenEngine: KRAKEN_DEPENDENCIES=${KRAKEN_DEPENDENCIES}")
message(STATUS "KrakenEngine: KRAKEN_VENDORED=${KRAKEN_VENDORED} (default for dependency *_VENDORED options)")
message(STATUS "KrakenEngine: SDL3_VENDORED=${SDL3_VENDORED}")
message(STATUS "KrakenEngine: SDL3_IMAGE_VENDORED=${SDL3_IMAGE_VENDORED}")
message(STATUS "KrakenEngine: SDL3_TTF_VENDORED=${SDL3_TTF_VENDORED}")
message(STATUS "KrakenEngine: SDL3_MIXER_VENDORED=${SDL3_MIXER_VENDORED}")
message(STATUS "KrakenEngine: SDL3_SHADERCROSS_VENDORED=${SDL3_SHADERCROSS_VENDORED}")
message(STATUS "KrakenEngine: BOX2D_VENDORED=${BOX2D_VENDORED}")
message(STATUS "KrakenEngine: SPDLOG_VENDORED=${SPDLOG_VENDORED}")
message(STATUS "KrakenEngine: ZSTD_VENDORED=${ZSTD_VENDORED}")
message(STATUS "KrakenEngine: TMXLITE_VENDORED=${TMXLITE_VENDORED}")
