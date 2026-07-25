# SDL3
if(SDL3_VENDORED)
  set(SDL_SHARED OFF CACHE BOOL "" FORCE)
  set(SDL_STATIC ON CACHE BOOL "" FORCE)
  set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
  if(KRAKEN_INSTALL)
    set(SDL_INSTALL ON CACHE BOOL "" FORCE)
    set(SDL_INSTALL_DOCS OFF CACHE BOOL "" FORCE)
  else()
    set(SDL_INSTALL OFF CACHE BOOL "" FORCE)
  endif()

  # Temporarily use the Kraken fork while waiting for upstream releases.
  FetchContent_Declare(SDL3
    GIT_REPOSITORY https://github.com/durkisneer1/SDL.git
    GIT_TAG main
    GIT_SHALLOW TRUE
  )
  FetchContent_MakeAvailable(SDL3)
else()
  find_package(SDL3 CONFIG REQUIRED)
endif()

# SDL3_image
if(SDL3_IMAGE_VENDORED)
  set(SDLIMAGE_ANI OFF CACHE BOOL "" FORCE)
  set(SDLIMAGE_AVIF OFF CACHE BOOL "" FORCE)
  set(SDLIMAGE_BMP OFF CACHE BOOL "" FORCE)
  set(SDLIMAGE_GIF OFF CACHE BOOL "" FORCE)
  set(SDLIMAGE_LBM OFF CACHE BOOL "" FORCE)
  set(SDLIMAGE_PCX OFF CACHE BOOL "" FORCE)
  set(SDLIMAGE_PNM OFF CACHE BOOL "" FORCE)
  set(SDLIMAGE_QOI OFF CACHE BOOL "" FORCE)
  set(SDLIMAGE_TIF OFF CACHE BOOL "" FORCE)
  set(SDLIMAGE_WEBP OFF CACHE BOOL "" FORCE)
  set(SDLIMAGE_XCF OFF CACHE BOOL "" FORCE)
  set(SDLIMAGE_XPM OFF CACHE BOOL "" FORCE)
  set(SDLIMAGE_XV OFF CACHE BOOL "" FORCE)
  if(KRAKEN_INSTALL)
    set(SDLIMAGE_VENDORED OFF CACHE BOOL "" FORCE)
  else()
    set(SDLIMAGE_VENDORED ON CACHE BOOL "" FORCE)
  endif()
  set(SDLIMAGE_PNG_SHARED OFF CACHE BOOL "" FORCE)
  set(SDLIMAGE_INSTALL ${KRAKEN_INSTALL} CACHE BOOL "" FORCE)
  set(SDLIMAGE_INSTALL_MAN OFF CACHE BOOL "" FORCE)

  FetchContent_Declare(SDL_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG release-3.4.0
    GIT_SHALLOW TRUE
    GIT_SUBMODULES external/jpeg external/libpng external/zlib
  )
  FetchContent_MakeAvailable(SDL_image)
else()
  find_package(SDL3_image CONFIG REQUIRED)
endif()

# SDL3_ttf must follow SDL3_image to avoid duplicate vendored PNG aliases.
if(SDL3_TTF_VENDORED)
  set(SDLTTF_VENDORED ON CACHE BOOL "" FORCE)
  set(SDLTTF_INSTALL ${KRAKEN_INSTALL} CACHE BOOL "" FORCE)
  set(SDLTTF_INSTALL_MAN OFF CACHE BOOL "" FORCE)
  FetchContent_Declare(SDL_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG release-3.2.2
    GIT_SHALLOW TRUE
  )
  FetchContent_MakeAvailable(SDL_ttf)
else()
  find_package(SDL3_ttf CONFIG REQUIRED)
endif()

# SDL3_mixer
if(SDL3_MIXER_VENDORED)
  set(SDLMIXER_GME OFF CACHE BOOL "" FORCE)
  set(SDLMIXER_WAVPACK OFF CACHE BOOL "" FORCE)
  set(SDLMIXER_MOD OFF CACHE BOOL "" FORCE)
  set(SDLMIXER_MIDI OFF CACHE BOOL "" FORCE)
  set(SDLMIXER_AIFF OFF CACHE BOOL "" FORCE)
  set(SDLMIXER_VOC OFF CACHE BOOL "" FORCE)
  set(SDLMIXER_AU OFF CACHE BOOL "" FORCE)
  set(SDLMIXER_VENDORED ON CACHE BOOL "" FORCE)
  set(SDLMIXER_INSTALL ${KRAKEN_INSTALL} CACHE BOOL "" FORCE)
  set(SDLMIXER_INSTALL_MAN OFF CACHE BOOL "" FORCE)
  FetchContent_Declare(SDL_mixer
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
    GIT_TAG release-3.2.0
    GIT_SHALLOW TRUE
  )
  FetchContent_MakeAvailable(SDL_mixer)
else()
  find_package(SDL3_mixer CONFIG REQUIRED)
endif()

# SDL3_shadercross is only needed by the Python-exposed shader baker.
if(KRAKEN_BUILD_PYTHON)
  if(SDL3_SHADERCROSS_VENDORED)
    set(SDLSHADERCROSS_VENDORED ON CACHE BOOL "" FORCE)
    set(SDLSHADERCROSS_SHARED OFF CACHE BOOL "" FORCE)
    set(SDLSHADERCROSS_STATIC ON CACHE BOOL "" FORCE)
    set(SDLSHADERCROSS_CLI OFF CACHE BOOL "" FORCE)

    if(MSVC)
      if(POLICY CMP0141)
        cmake_policy(SET CMP0141 NEW)
        set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
            "$<$<CONFIG:Debug,RelWithDebInfo>:Embedded>"
            CACHE STRING "" FORCE)
      endif()

      foreach(config DEBUG RELWITHDEBINFO)
        string(REPLACE "/Zi" "/Z7" CMAKE_CXX_FLAGS_${config} "${CMAKE_CXX_FLAGS_${config}}")
        string(REPLACE "/Zi" "/Z7" CMAKE_C_FLAGS_${config} "${CMAKE_C_FLAGS_${config}}")
      endforeach()

      add_compile_options("$<$<CONFIG:Debug,RelWithDebInfo>:/Z7>")
      add_compile_options(/FS)
    endif()

    FetchContent_Declare(SDL_shadercross
      GIT_REPOSITORY https://github.com/libsdl-org/SDL_shadercross.git
      GIT_TAG main
      GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(SDL_shadercross)
  else()
    find_package(SDL3_shadercross CONFIG REQUIRED)
  endif()
endif()

# Box2D
if(BOX2D_VENDORED)
  FetchContent_Declare(box2d
    GIT_REPOSITORY https://github.com/erincatto/box2d.git
    GIT_TAG v3.1.1
    GIT_SHALLOW TRUE
  )
  FetchContent_MakeAvailable(box2d)
else()
  find_package(box2d CONFIG REQUIRED)
endif()

# spdlog
if(SPDLOG_VENDORED)
  FetchContent_Declare(spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.17.0
    GIT_SHALLOW TRUE
  )
  FetchContent_MakeAvailable(spdlog)
else()
  find_package(spdlog CONFIG REQUIRED)
endif()

# zstd
if(ZSTD_VENDORED)
  set(ZSTD_BUILD_PROGRAMS OFF CACHE BOOL "" FORCE)
  set(ZSTD_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  set(ZSTD_BUILD_SHARED OFF CACHE BOOL "" FORCE)
  set(ZSTD_BUILD_STATIC ON CACHE BOOL "" FORCE)
  FetchContent_Declare(zstd
    GIT_REPOSITORY https://github.com/facebook/zstd.git
    GIT_TAG v1.5.7
    GIT_SHALLOW TRUE
    SOURCE_SUBDIR build/cmake
  )
  FetchContent_MakeAvailable(zstd)
  if(TARGET libzstd_static AND NOT TARGET zstd::libzstd_static)
    add_library(zstd::libzstd_static ALIAS libzstd_static)
  endif()
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

# tmxlite
if(TMXLITE_VENDORED)
  if(ZSTD_VENDORED)
    set(ZSTD_LIBRARY libzstd_static CACHE STRING "" FORCE)
    set(ZSTD_LIBRARY_DEBUG libzstd_static CACHE STRING "" FORCE)
    set(ZSTD_LIBRARY_RELEASE libzstd_static CACHE STRING "" FORCE)
    set(ZSTD_INCLUDE_DIR "${zstd_SOURCE_DIR}/lib" CACHE PATH "" FORCE)
  endif()

  FetchContent_Declare(tmxlite
    GIT_REPOSITORY https://github.com/fallahn/tmxlite.git
    GIT_TAG v1.4.5
    GIT_SHALLOW TRUE
    SOURCE_SUBDIR tmxlite
  )
  set(TMXLITE_STATIC_LIB ON CACHE BOOL "" FORCE)
  set(USE_ZSTD ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(tmxlite)
  add_library(tmxlite::tmxlite ALIAS tmxlite)
else()
  find_package(TMXLITE MODULE REQUIRED)
  find_package(pugixml CONFIG REQUIRED)
endif()
