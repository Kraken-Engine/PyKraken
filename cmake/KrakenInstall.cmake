include(GNUInstallDirs)

install(TARGETS KrakenEngine
  EXPORT KrakenEngineTargets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/FindTMXLITE.cmake"
  DESTINATION ${CMAKE_INSTALL_DATADIR}/kraken-engine/modules
)

install(EXPORT KrakenEngineTargets
  FILE KrakenEngineTargets.cmake
  NAMESPACE Kraken::
  DESTINATION ${CMAKE_INSTALL_DATADIR}/kraken-engine
)

include(CMakePackageConfigHelpers)
set(CONFIG_FILE "${CMAKE_CURRENT_BINARY_DIR}/KrakenEngineConfig.cmake")

# Static consumers must discover Kraken's transitive dependencies.
file(WRITE ${CONFIG_FILE}
  "list(APPEND CMAKE_MODULE_PATH \"\${CMAKE_CURRENT_LIST_DIR}/modules\")\n\n"
  "include(CMakeFindDependencyMacro)\n"
  "find_dependency(SDL3 CONFIG)\n"
  "find_dependency(SDL3_image CONFIG)\n"
  "find_dependency(SDL3_ttf CONFIG)\n"
  "find_dependency(SDL3_mixer CONFIG)\n"
  "find_dependency(box2d CONFIG)\n"
  "find_dependency(spdlog CONFIG)\n"
  "find_dependency(zstd CONFIG)\n"
  "find_dependency(TMXLITE MODULE)\n"
  "include(\"\${CMAKE_CURRENT_LIST_DIR}/KrakenEngineTargets.cmake\")\n"
)

install(FILES ${CONFIG_FILE}
  DESTINATION ${CMAKE_INSTALL_DATADIR}/kraken-engine
)
