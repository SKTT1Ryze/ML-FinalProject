#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "SeetaFace::SeetaNet" for configuration "Release"
set_property(TARGET SeetaFace::SeetaNet APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SeetaFace::SeetaNet PROPERTIES
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "pthread"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libSeetaNet.so.a6e9ef3"
  IMPORTED_SONAME_RELEASE "libSeetaNet.so.a6e9ef3"
  )

list(APPEND _IMPORT_CHECK_TARGETS SeetaFace::SeetaNet )
list(APPEND _IMPORT_CHECK_FILES_FOR_SeetaFace::SeetaNet "${_IMPORT_PREFIX}/lib/libSeetaNet.so.a6e9ef3" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
