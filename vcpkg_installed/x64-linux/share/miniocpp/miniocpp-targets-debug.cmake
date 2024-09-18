#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "miniocpp::miniocpp" for configuration "Debug"
set_property(TARGET miniocpp::miniocpp APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(miniocpp::miniocpp PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/debug/lib/libminiocpp.a"
  )

list(APPEND _cmake_import_check_targets miniocpp::miniocpp )
list(APPEND _cmake_import_check_files_for_miniocpp::miniocpp "${_IMPORT_PREFIX}/debug/lib/libminiocpp.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
