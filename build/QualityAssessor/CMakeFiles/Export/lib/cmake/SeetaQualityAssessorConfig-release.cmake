#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "SeetaFace::SeetaQualityAssessor" for configuration "Release"
set_property(TARGET SeetaFace::SeetaQualityAssessor APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SeetaFace::SeetaQualityAssessor PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libSeetaQualityAssessor.so.11d9063"
  IMPORTED_SONAME_RELEASE "libSeetaQualityAssessor.so.11d9063"
  )

list(APPEND _IMPORT_CHECK_TARGETS SeetaFace::SeetaQualityAssessor )
list(APPEND _IMPORT_CHECK_FILES_FOR_SeetaFace::SeetaQualityAssessor "${_IMPORT_PREFIX}/lib/libSeetaQualityAssessor.so.11d9063" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
