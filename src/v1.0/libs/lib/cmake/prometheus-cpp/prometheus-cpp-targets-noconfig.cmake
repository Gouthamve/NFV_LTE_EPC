#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "prometheus-cpp::prometheus-cpp" for configuration ""
set_property(TARGET prometheus-cpp::prometheus-cpp APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(prometheus-cpp::prometheus-cpp PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "C;CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libprometheus-cpp.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS prometheus-cpp::prometheus-cpp )
list(APPEND _IMPORT_CHECK_FILES_FOR_prometheus-cpp::prometheus-cpp "${_IMPORT_PREFIX}/lib/libprometheus-cpp.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
