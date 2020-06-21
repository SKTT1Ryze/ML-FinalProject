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
include example/points81/CMakeFiles/points81.dir/depend.make

# Include the progress variables for this target.
include example/points81/CMakeFiles/points81.dir/progress.make

# Include the compile flags for this target's objects.
include example/points81/CMakeFiles/points81.dir/flags.make

example/points81/CMakeFiles/points81.dir/example.cpp.o: example/points81/CMakeFiles/points81.dir/flags.make
example/points81/CMakeFiles/points81.dir/example.cpp.o: ../example/points81/example.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hustccc/SeetaFace2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object example/points81/CMakeFiles/points81.dir/example.cpp.o"
	cd /home/hustccc/SeetaFace2/build/example/points81 && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/points81.dir/example.cpp.o -c /home/hustccc/SeetaFace2/example/points81/example.cpp

example/points81/CMakeFiles/points81.dir/example.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/points81.dir/example.cpp.i"
	cd /home/hustccc/SeetaFace2/build/example/points81 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hustccc/SeetaFace2/example/points81/example.cpp > CMakeFiles/points81.dir/example.cpp.i

example/points81/CMakeFiles/points81.dir/example.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/points81.dir/example.cpp.s"
	cd /home/hustccc/SeetaFace2/build/example/points81 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hustccc/SeetaFace2/example/points81/example.cpp -o CMakeFiles/points81.dir/example.cpp.s

# Object files for target points81
points81_OBJECTS = \
"CMakeFiles/points81.dir/example.cpp.o"

# External object files for target points81
points81_EXTERNAL_OBJECTS =

bin/points81-21951fe: example/points81/CMakeFiles/points81.dir/example.cpp.o
bin/points81-21951fe: example/points81/CMakeFiles/points81.dir/build.make
bin/points81-21951fe: bin/libSeetaFaceDetector.so.21951fe
bin/points81-21951fe: bin/libSeetaFaceLandmarker.so.21951fe
bin/points81-21951fe: /usr/local/lib/libopencv_dnn.so.4.3.0
bin/points81-21951fe: /usr/local/lib/libopencv_gapi.so.4.3.0
bin/points81-21951fe: /usr/local/lib/libopencv_highgui.so.4.3.0
bin/points81-21951fe: /usr/local/lib/libopencv_ml.so.4.3.0
bin/points81-21951fe: /usr/local/lib/libopencv_objdetect.so.4.3.0
bin/points81-21951fe: /usr/local/lib/libopencv_photo.so.4.3.0
bin/points81-21951fe: /usr/local/lib/libopencv_stitching.so.4.3.0
bin/points81-21951fe: /usr/local/lib/libopencv_video.so.4.3.0
bin/points81-21951fe: /usr/local/lib/libopencv_videoio.so.4.3.0
bin/points81-21951fe: bin/libSeetaNet.so.21951fe
bin/points81-21951fe: /usr/local/lib/libopencv_imgcodecs.so.4.3.0
bin/points81-21951fe: /usr/local/lib/libopencv_calib3d.so.4.3.0
bin/points81-21951fe: /usr/local/lib/libopencv_features2d.so.4.3.0
bin/points81-21951fe: /usr/local/lib/libopencv_flann.so.4.3.0
bin/points81-21951fe: /usr/local/lib/libopencv_imgproc.so.4.3.0
bin/points81-21951fe: /usr/local/lib/libopencv_core.so.4.3.0
bin/points81-21951fe: example/points81/CMakeFiles/points81.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/hustccc/SeetaFace2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../bin/points81"
	cd /home/hustccc/SeetaFace2/build/example/points81 && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/points81.dir/link.txt --verbose=$(VERBOSE)
	cd /home/hustccc/SeetaFace2/build/example/points81 && $(CMAKE_COMMAND) -E cmake_symlink_executable ../../bin/points81-21951fe ../../bin/points81

bin/points81: bin/points81-21951fe


# Rule to build all files generated by this target.
example/points81/CMakeFiles/points81.dir/build: bin/points81

.PHONY : example/points81/CMakeFiles/points81.dir/build

example/points81/CMakeFiles/points81.dir/clean:
	cd /home/hustccc/SeetaFace2/build/example/points81 && $(CMAKE_COMMAND) -P CMakeFiles/points81.dir/cmake_clean.cmake
.PHONY : example/points81/CMakeFiles/points81.dir/clean

example/points81/CMakeFiles/points81.dir/depend:
	cd /home/hustccc/SeetaFace2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hustccc/SeetaFace2 /home/hustccc/SeetaFace2/example/points81 /home/hustccc/SeetaFace2/build /home/hustccc/SeetaFace2/build/example/points81 /home/hustccc/SeetaFace2/build/example/points81/CMakeFiles/points81.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : example/points81/CMakeFiles/points81.dir/depend

