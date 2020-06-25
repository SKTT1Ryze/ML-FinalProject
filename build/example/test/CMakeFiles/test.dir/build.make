# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/hustccc/SeetaFace2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/hustccc/SeetaFace2/build

# Include any dependencies generated for this target.
include example/test/CMakeFiles/test.dir/depend.make

# Include the progress variables for this target.
include example/test/CMakeFiles/test.dir/progress.make

# Include the compile flags for this target's objects.
include example/test/CMakeFiles/test.dir/flags.make

example/test/CMakeFiles/test.dir/example.cpp.o: example/test/CMakeFiles/test.dir/flags.make
example/test/CMakeFiles/test.dir/example.cpp.o: ../example/test/example.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hustccc/SeetaFace2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object example/test/CMakeFiles/test.dir/example.cpp.o"
	cd /home/hustccc/SeetaFace2/build/example/test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test.dir/example.cpp.o -c /home/hustccc/SeetaFace2/example/test/example.cpp

example/test/CMakeFiles/test.dir/example.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test.dir/example.cpp.i"
	cd /home/hustccc/SeetaFace2/build/example/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hustccc/SeetaFace2/example/test/example.cpp > CMakeFiles/test.dir/example.cpp.i

example/test/CMakeFiles/test.dir/example.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test.dir/example.cpp.s"
	cd /home/hustccc/SeetaFace2/build/example/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hustccc/SeetaFace2/example/test/example.cpp -o CMakeFiles/test.dir/example.cpp.s

# Object files for target test
test_OBJECTS = \
"CMakeFiles/test.dir/example.cpp.o"

# External object files for target test
test_EXTERNAL_OBJECTS =

bin/test-da1c589: example/test/CMakeFiles/test.dir/example.cpp.o
bin/test-da1c589: example/test/CMakeFiles/test.dir/build.make
bin/test-da1c589: bin/libSeetaFaceDetector.so.da1c589
bin/test-da1c589: bin/libSeetaFaceLandmarker.so.da1c589
bin/test-da1c589: bin/libSeetaFaceRecognizer.so.da1c589
bin/test-da1c589: bin/libSeetaQualityAssessor.so.da1c589
bin/test-da1c589: /usr/local/lib/libopencv_dnn.so.4.3.0
bin/test-da1c589: /usr/local/lib/libopencv_gapi.so.4.3.0
bin/test-da1c589: /usr/local/lib/libopencv_highgui.so.4.3.0
bin/test-da1c589: /usr/local/lib/libopencv_ml.so.4.3.0
bin/test-da1c589: /usr/local/lib/libopencv_objdetect.so.4.3.0
bin/test-da1c589: /usr/local/lib/libopencv_photo.so.4.3.0
bin/test-da1c589: /usr/local/lib/libopencv_stitching.so.4.3.0
bin/test-da1c589: /usr/local/lib/libopencv_video.so.4.3.0
bin/test-da1c589: /usr/local/lib/libopencv_videoio.so.4.3.0
bin/test-da1c589: bin/libSeetaNet.so.da1c589
bin/test-da1c589: /usr/local/lib/libopencv_imgcodecs.so.4.3.0
bin/test-da1c589: /usr/local/lib/libopencv_calib3d.so.4.3.0
bin/test-da1c589: /usr/local/lib/libopencv_features2d.so.4.3.0
bin/test-da1c589: /usr/local/lib/libopencv_flann.so.4.3.0
bin/test-da1c589: /usr/local/lib/libopencv_imgproc.so.4.3.0
bin/test-da1c589: /usr/local/lib/libopencv_core.so.4.3.0
bin/test-da1c589: example/test/CMakeFiles/test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/hustccc/SeetaFace2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../bin/test"
	cd /home/hustccc/SeetaFace2/build/example/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test.dir/link.txt --verbose=$(VERBOSE)
	cd /home/hustccc/SeetaFace2/build/example/test && $(CMAKE_COMMAND) -E cmake_symlink_executable ../../bin/test-da1c589 ../../bin/test

bin/test: bin/test-da1c589


# Rule to build all files generated by this target.
example/test/CMakeFiles/test.dir/build: bin/test

.PHONY : example/test/CMakeFiles/test.dir/build

example/test/CMakeFiles/test.dir/clean:
	cd /home/hustccc/SeetaFace2/build/example/test && $(CMAKE_COMMAND) -P CMakeFiles/test.dir/cmake_clean.cmake
.PHONY : example/test/CMakeFiles/test.dir/clean

example/test/CMakeFiles/test.dir/depend:
	cd /home/hustccc/SeetaFace2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hustccc/SeetaFace2 /home/hustccc/SeetaFace2/example/test /home/hustccc/SeetaFace2/build /home/hustccc/SeetaFace2/build/example/test /home/hustccc/SeetaFace2/build/example/test/CMakeFiles/test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : example/test/CMakeFiles/test.dir/depend

