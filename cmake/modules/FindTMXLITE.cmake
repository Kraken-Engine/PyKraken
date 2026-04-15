# Locate tmxlite in module mode.
#
# This module defines:
#   TMXLITE_FOUND
#   TMXLITE_INCLUDE_DIR
#   TMXLITE_LIBRARY
#   TMXLITE_INCLUDE_DIRS
#   TMXLITE_LIBRARIES
# and provides imported target:
#   tmxlite

find_path(TMXLITE_INCLUDE_DIR NAMES tmxlite/Config.hpp PATH_SUFFIXES include)

find_library(TMXLITE_LIBRARY_DEBUG NAMES tmxlite-d libtmxlite-d tmxlite-s-d libtmxlite-s-d)
find_library(TMXLITE_LIBRARY_RELEASE NAMES tmxlite libtmxlite tmxlite-s libtmxlite-s)

include(SelectLibraryConfigurations)
select_library_configurations(TMXLITE)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(TMXLITE DEFAULT_MSG TMXLITE_LIBRARY TMXLITE_INCLUDE_DIR)

if(TMXLITE_FOUND)
  set(TMXLITE_INCLUDE_DIRS "${TMXLITE_INCLUDE_DIR}")
  set(TMXLITE_LIBRARIES "${TMXLITE_LIBRARY}")

  if(NOT TARGET tmxlite::tmxlite)
    add_library(tmxlite::tmxlite UNKNOWN IMPORTED)

    if(TMXLITE_LIBRARY_RELEASE)
      set_property(TARGET tmxlite::tmxlite APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
      set_target_properties(tmxlite::tmxlite PROPERTIES
        IMPORTED_LOCATION_RELEASE "${TMXLITE_LIBRARY_RELEASE}"
      )
    endif()

    if(TMXLITE_LIBRARY_DEBUG)
      set_property(TARGET tmxlite::tmxlite APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
      set_target_properties(tmxlite::tmxlite PROPERTIES
        IMPORTED_LOCATION_DEBUG "${TMXLITE_LIBRARY_DEBUG}"
      )
    endif()

    set_target_properties(tmxlite::tmxlite PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${TMXLITE_INCLUDE_DIR}"
    )
  endif()
endif()

mark_as_advanced(
  TMXLITE_INCLUDE_DIR
  TMXLITE_LIBRARY
  TMXLITE_LIBRARY_DEBUG
  TMXLITE_LIBRARY_RELEASE
)
