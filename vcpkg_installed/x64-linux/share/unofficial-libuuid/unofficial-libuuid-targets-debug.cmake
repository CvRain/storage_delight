#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "unofficial::UUID::uuid" for configuration "Debug"
set_property(TARGET unofficial::UUID::uuid APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(unofficial::UUID::uuid PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/debug/lib/libuuid.a"
  )

list(APPEND _cmake_import_check_targets unofficial::UUID::uuid )
list(APPEND _cmake_import_check_files_for_unofficial::UUID::uuid "${_IMPORT_PREFIX}/debug/lib/libuuid.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
