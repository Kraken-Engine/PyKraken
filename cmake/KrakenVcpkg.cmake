# Configure vcpkg before project() enables a compiler.
# Python builds need the optional shadercross dependency. Select it before
# project() runs vcpkg's manifest install, including fresh scikit-build caches.
# SYSTEM overrides the per-dependency vendoring option in KrakenOptions.cmake.
string(TOUPPER "${KRAKEN_DEPENDENCIES}" _KRAKEN_DEPENDENCY_PROVIDER)
if((SKBUILD OR KRAKEN_BUILD_PYTHON) AND
   (NOT SDL3_SHADERCROSS_VENDORED OR _KRAKEN_DEPENDENCY_PROVIDER STREQUAL "SYSTEM"))
  list(APPEND VCPKG_MANIFEST_FEATURES python)
  list(REMOVE_DUPLICATES VCPKG_MANIFEST_FEATURES)
endif()
unset(_KRAKEN_DEPENDENCY_PROVIDER)

set(_KRAKEN_VCPKG_TRIPLET "")

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

if(DEFINED VCPKG_TARGET_TRIPLET)
  set(_KRAKEN_VCPKG_TRIPLET "${VCPKG_TARGET_TRIPLET}")
elseif(DEFINED ENV{VCPKG_DEFAULT_TRIPLET})
  set(_KRAKEN_VCPKG_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}")
elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed/x64-linux")
  set(_KRAKEN_VCPKG_TRIPLET "x64-linux")
endif()

if(_KRAKEN_VCPKG_TRIPLET)
  set(_KRAKEN_VCPKG_INSTALLED_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg_installed")

  # Manifest installs can exist before the toolchain populates find roots.
  if(EXISTS "${_KRAKEN_VCPKG_INSTALLED_DIR}/${_KRAKEN_VCPKG_TRIPLET}")
    set(VCPKG_INSTALLED_DIR "${_KRAKEN_VCPKG_INSTALLED_DIR}"
        CACHE PATH "Vcpkg installed package directory")
    list(PREPEND CMAKE_PREFIX_PATH
      "${_KRAKEN_VCPKG_INSTALLED_DIR}/${_KRAKEN_VCPKG_TRIPLET}")
  endif()
endif()
