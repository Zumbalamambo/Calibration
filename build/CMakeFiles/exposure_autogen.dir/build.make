# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/victor/workspace/cpp/projects/Calibration

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/victor/workspace/cpp/projects/Calibration/build

# Utility rule file for exposure_autogen.

# Include the progress variables for this target.
include CMakeFiles/exposure_autogen.dir/progress.make

CMakeFiles/exposure_autogen:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/victor/workspace/cpp/projects/Calibration/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC and UIC for target exposure"
	/usr/bin/cmake -E cmake_autogen /home/victor/workspace/cpp/projects/Calibration/build/CMakeFiles/exposure_autogen.dir/AutogenInfo.json Debug

exposure_autogen: CMakeFiles/exposure_autogen
exposure_autogen: CMakeFiles/exposure_autogen.dir/build.make

.PHONY : exposure_autogen

# Rule to build all files generated by this target.
CMakeFiles/exposure_autogen.dir/build: exposure_autogen

.PHONY : CMakeFiles/exposure_autogen.dir/build

CMakeFiles/exposure_autogen.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/exposure_autogen.dir/cmake_clean.cmake
.PHONY : CMakeFiles/exposure_autogen.dir/clean

CMakeFiles/exposure_autogen.dir/depend:
	cd /home/victor/workspace/cpp/projects/Calibration/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/victor/workspace/cpp/projects/Calibration /home/victor/workspace/cpp/projects/Calibration /home/victor/workspace/cpp/projects/Calibration/build /home/victor/workspace/cpp/projects/Calibration/build /home/victor/workspace/cpp/projects/Calibration/build/CMakeFiles/exposure_autogen.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/exposure_autogen.dir/depend

