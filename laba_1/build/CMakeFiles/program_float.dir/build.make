# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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

#Suppress display of executed commands.
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
CMAKE_SOURCE_DIR = /home/a.kalugina2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/a.kalugina2/build

# Include any dependencies generated for this target.
include CMakeFiles/program_float.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/program_float.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/program_float.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/program_float.dir/flags.make

CMakeFiles/program_float.dir/1.cpp.o: CMakeFiles/program_float.dir/flags.make
CMakeFiles/program_float.dir/1.cpp.o: ../1.cpp
CMakeFiles/program_float.dir/1.cpp.o: CMakeFiles/program_float.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/a.kalugina2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/program_float.dir/1.cpp.o"
	/opt/nvidia/hpc_sdk/Linux_x86_64/23.11/compilers/bin/nvc++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/program_float.dir/1.cpp.o -MF CMakeFiles/program_float.dir/1.cpp.o.d -o CMakeFiles/program_float.dir/1.cpp.o -c /home/a.kalugina2/1.cpp

CMakeFiles/program_float.dir/1.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/program_float.dir/1.cpp.i"
	/opt/nvidia/hpc_sdk/Linux_x86_64/23.11/compilers/bin/nvc++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/a.kalugina2/1.cpp > CMakeFiles/program_float.dir/1.cpp.i

CMakeFiles/program_float.dir/1.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/program_float.dir/1.cpp.s"
	/opt/nvidia/hpc_sdk/Linux_x86_64/23.11/compilers/bin/nvc++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/a.kalugina2/1.cpp -o CMakeFiles/program_float.dir/1.cpp.s

# Object files for target program_float
program_float_OBJECTS = \
"CMakeFiles/program_float.dir/1.cpp.o"

# External object files for target program_float
program_float_EXTERNAL_OBJECTS =

program_float: CMakeFiles/program_float.dir/1.cpp.o
program_float: CMakeFiles/program_float.dir/build.make
program_float: CMakeFiles/program_float.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/a.kalugina2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable program_float"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/program_float.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/program_float.dir/build: program_float
.PHONY : CMakeFiles/program_float.dir/build

CMakeFiles/program_float.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/program_float.dir/cmake_clean.cmake
.PHONY : CMakeFiles/program_float.dir/clean

CMakeFiles/program_float.dir/depend:
	cd /home/a.kalugina2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/a.kalugina2 /home/a.kalugina2 /home/a.kalugina2/build /home/a.kalugina2/build /home/a.kalugina2/build/CMakeFiles/program_float.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/program_float.dir/depend

