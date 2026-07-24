cmake_minimum_required(VERSION 3.15...3.27)

# End users should install with `pip install .`, which creates an isolated
# build environment. Developers can use `pip install --no-build-isolation -ve .`
# after installing nanobind and scikit-build-core once.

# The vcpkg toolchain must be selected before project().
include(cmake/KrakenVcpkg.cmake)

project(KrakenEngine LANGUAGES CXX)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

include(cmake/KrakenOptions.cmake)
include(cmake/KrakenDependencies.cmake)
include(cmake/KrakenSources.cmake)
include(cmake/KrakenTargets.cmake)

if(KRAKEN_BUILD_PYTHON)
  include(cmake/KrakenPython.cmake)
elseif(KRAKEN_INSTALL)
  include(cmake/KrakenInstall.cmake)
endif()
