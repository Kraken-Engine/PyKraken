# Copy runtime DLLs next to the Python extension on Windows.
if(WIN32)
  set(_VCPKG_BIN_DIR "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin")
  file(GLOB _VCPKG_DLLS "${_VCPKG_BIN_DIR}/*.dll")
  if(_VCPKG_DLLS)
    add_custom_command(TARGET _pykraken POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${_VCPKG_DLLS} $<TARGET_FILE_DIR:_pykraken>
      COMMAND_EXPAND_LISTS
    )
  endif()

  set(_VENDORED_TARGETS "")
  if(TARGET dxcompiler)
    list(APPEND _VENDORED_TARGETS dxcompiler)
  endif()
  if(TARGET dxil)
    list(APPEND _VENDORED_TARGETS dxil)
  endif()
  if(TARGET spirv-cross-c-shared)
    list(APPEND _VENDORED_TARGETS spirv-cross-c-shared)
  endif()

  foreach(_vtgt IN LISTS _VENDORED_TARGETS)
    add_custom_command(TARGET _pykraken POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
              $<TARGET_FILE:${_vtgt}> $<TARGET_FILE_DIR:_pykraken>
    )
  endforeach()
endif()

install(TARGETS _pykraken LIBRARY DESTINATION pykraken)
install(FILES src/pykraken/__init__.py DESTINATION pykraken)

if(WIN32)
  if(_VCPKG_DLLS)
    install(FILES ${_VCPKG_DLLS} DESTINATION pykraken)
  endif()
  if(_VENDORED_TARGETS)
    install(TARGETS ${_VENDORED_TARGETS} RUNTIME DESTINATION pykraken)
  endif()
elseif(_KRAKEN_VCPKG_TRIPLET AND NOT SDL3_SHADERCROSS_VENDORED)
  set(_KRAKEN_SHADERCROSS_RUNTIME_LIBS "")
  if(DirectXShaderCompiler_dxcompiler_LIBRARY)
    list(APPEND _KRAKEN_SHADERCROSS_RUNTIME_LIBS "${DirectXShaderCompiler_dxcompiler_LIBRARY}")
  endif()
  if(DirectXShaderCompiler_dxil_LIBRARY)
    list(APPEND _KRAKEN_SHADERCROSS_RUNTIME_LIBS "${DirectXShaderCompiler_dxil_LIBRARY}")
  endif()
  if(_KRAKEN_SHADERCROSS_RUNTIME_LIBS)
    install(FILES ${_KRAKEN_SHADERCROSS_RUNTIME_LIBS} DESTINATION pykraken)
  endif()
endif()

nanobind_add_stub(
  _pykraken_stub
  INSTALL_TIME
  MODULE pykraken._pykraken
  PYTHON_PATH .
  RECURSIVE
  OUTPUT_PATH pykraken
  MARKER_FILE pykraken/py.typed
)
