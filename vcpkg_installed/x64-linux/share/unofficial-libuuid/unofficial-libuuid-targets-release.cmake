#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "unofficial::UUID::uuid" for configuration "Release"
set_property(TARGET unofficial::UUID::uuid APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(unofficial::UUID::uuid PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libuuid.a"
  )

list(APPEND _cmake_import_check_targets unofficial::UUID::uuid )
list(APPEND _cmake_import_check_files_for_unofficial::UUID::uuid "${_IMPORT_PREFIX}/lib/libuuid.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
