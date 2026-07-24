if(KRAKEN_BUILD_PYTHON)
  find_package(Python 3.12
    REQUIRED COMPONENTS Interpreter Development.Module
    OPTIONAL_COMPONENTS Development.SABIModule)
  find_package(nanobind CONFIG REQUIRED)

  nanobind_add_module(_pykraken STABLE_ABI
    ${KRAKEN_CORE_SOURCES}
    ${KRAKEN_PYTHON_SOURCES}
    ${KRAKEN_BAKER_SOURCES}
  )

  if(SDL3_SHADERCROSS_VENDORED)
    set_target_properties(_pykraken PROPERTIES
      INSTALL_RPATH "${CMAKE_BINARY_DIR}/_deps/sdl_shadercross-build/external/SPIRV-Cross;${CMAKE_BINARY_DIR}/_deps/sdl_shadercross-build/external/DirectXShaderCompiler/lib"
      INSTALL_RPATH_USE_LINK_PATH TRUE
    )
  elseif(_KRAKEN_VCPKG_TRIPLET)
    set_target_properties(_pykraken PROPERTIES
      INSTALL_RPATH "$ORIGIN"
      INSTALL_RPATH_USE_LINK_PATH TRUE
    )
  else()
    set_target_properties(_pykraken PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)
  endif()

  target_compile_definitions(_pykraken PRIVATE
    KRAKEN_PYTHON_POOL_CAPACITY=${KRAKEN_PYTHON_POOL_CAPACITY}
  )

  set(KRAKEN_TARGET _pykraken)
else()
  add_library(KrakenEngine STATIC ${KRAKEN_CORE_SOURCES})
  add_library(Kraken::Kraken ALIAS KrakenEngine)
  set_target_properties(KrakenEngine PROPERTIES EXPORT_NAME Kraken)
  set(KRAKEN_TARGET KrakenEngine)
endif()

target_include_directories(${KRAKEN_TARGET}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

set_target_properties(${KRAKEN_TARGET} PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
)

target_compile_features(${KRAKEN_TARGET} PUBLIC cxx_std_20)

if(MSVC)
  target_compile_options(${KRAKEN_TARGET} PRIVATE /utf-8)
endif()

target_link_libraries(${KRAKEN_TARGET} PRIVATE
  $<BUILD_INTERFACE:SDL3_image::SDL3_image>
  $<INSTALL_INTERFACE:SDL3_image::SDL3_image>
  $<BUILD_INTERFACE:$<IF:$<TARGET_EXISTS:pugixml::pugixml>,pugixml::pugixml,
    $<IF:$<TARGET_EXISTS:pugixml>,pugixml,>>>
  $<INSTALL_INTERFACE:pugixml::pugixml>
  $<BUILD_INTERFACE:$<IF:$<TARGET_EXISTS:zstd::libzstd_static>,zstd::libzstd_static,
    $<IF:$<TARGET_EXISTS:zstd::zstd>,zstd::zstd,zstd::libzstd_shared>>>
  $<INSTALL_INTERFACE:zstd::libzstd_static>
)

target_link_libraries(${KRAKEN_TARGET} PUBLIC
  $<BUILD_INTERFACE:SDL3::SDL3>
  $<INSTALL_INTERFACE:SDL3::SDL3>
  $<BUILD_INTERFACE:SDL3_ttf::SDL3_ttf>
  $<INSTALL_INTERFACE:SDL3_ttf::SDL3_ttf>
  $<BUILD_INTERFACE:SDL3_mixer::SDL3_mixer>
  $<INSTALL_INTERFACE:SDL3_mixer::SDL3_mixer>
  $<BUILD_INTERFACE:box2d::box2d>
  $<INSTALL_INTERFACE:box2d::box2d>
  $<BUILD_INTERFACE:spdlog::spdlog>
  $<INSTALL_INTERFACE:spdlog::spdlog>
  $<BUILD_INTERFACE:tmxlite::tmxlite>
  $<INSTALL_INTERFACE:tmxlite::tmxlite>
)

if(KRAKEN_BUILD_PYTHON)
  target_link_libraries(_pykraken PRIVATE SDL3_shadercross::SDL3_shadercross)
endif()
