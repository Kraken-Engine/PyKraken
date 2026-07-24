include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(KRAKEN_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/KrakenEngine")

install(TARGETS KrakenEngine
  EXPORT KrakenEngineTargets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/FindTMXLITE.cmake"
  DESTINATION "${KRAKEN_INSTALL_CMAKEDIR}/modules"
)

install(EXPORT KrakenEngineTargets
  FILE KrakenEngineTargets.cmake
  NAMESPACE Kraken::
  DESTINATION "${KRAKEN_INSTALL_CMAKEDIR}"
)

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/KrakenEngineConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/KrakenEngineConfig.cmake"
  INSTALL_DESTINATION "${KRAKEN_INSTALL_CMAKEDIR}"
)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/KrakenEngineConfigVersion.cmake"
  VERSION "${PROJECT_VERSION}"
  COMPATIBILITY SameMajorVersion
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/KrakenEngineConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/KrakenEngineConfigVersion.cmake"
  DESTINATION "${KRAKEN_INSTALL_CMAKEDIR}"
)
