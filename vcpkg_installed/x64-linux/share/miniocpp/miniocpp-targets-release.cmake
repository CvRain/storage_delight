#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "miniocpp::miniocpp" for configuration "Release"
set_property(TARGET miniocpp::miniocpp APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(miniocpp::miniocpp PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libminiocpp.a"
  )

list(APPEND _cmake_import_check_targets miniocpp::miniocpp )
list(APPEND _cmake_import_check_files_for_miniocpp::miniocpp "${_IMPORT_PREFIX}/lib/libminiocpp.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
