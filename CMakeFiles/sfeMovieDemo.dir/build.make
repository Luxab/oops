# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.6

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
CMAKE_SOURCE_DIR = /home/user/Nextcloud/School/CSE/OOP/oops

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/user/Nextcloud/School/CSE/OOP/oops

# Include any dependencies generated for this target.
include CMakeFiles/sfeMovieDemo.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/sfeMovieDemo.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/sfeMovieDemo.dir/flags.make

CMakeFiles/sfeMovieDemo.dir/game.cpp.o: CMakeFiles/sfeMovieDemo.dir/flags.make
CMakeFiles/sfeMovieDemo.dir/game.cpp.o: game.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/Nextcloud/School/CSE/OOP/oops/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/sfeMovieDemo.dir/game.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/sfeMovieDemo.dir/game.cpp.o -c /home/user/Nextcloud/School/CSE/OOP/oops/game.cpp

CMakeFiles/sfeMovieDemo.dir/game.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sfeMovieDemo.dir/game.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/user/Nextcloud/School/CSE/OOP/oops/game.cpp > CMakeFiles/sfeMovieDemo.dir/game.cpp.i

CMakeFiles/sfeMovieDemo.dir/game.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sfeMovieDemo.dir/game.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/user/Nextcloud/School/CSE/OOP/oops/game.cpp -o CMakeFiles/sfeMovieDemo.dir/game.cpp.s

CMakeFiles/sfeMovieDemo.dir/game.cpp.o.requires:

.PHONY : CMakeFiles/sfeMovieDemo.dir/game.cpp.o.requires

CMakeFiles/sfeMovieDemo.dir/game.cpp.o.provides: CMakeFiles/sfeMovieDemo.dir/game.cpp.o.requires
	$(MAKE) -f CMakeFiles/sfeMovieDemo.dir/build.make CMakeFiles/sfeMovieDemo.dir/game.cpp.o.provides.build
.PHONY : CMakeFiles/sfeMovieDemo.dir/game.cpp.o.provides

CMakeFiles/sfeMovieDemo.dir/game.cpp.o.provides.build: CMakeFiles/sfeMovieDemo.dir/game.cpp.o


# Object files for target sfeMovieDemo
sfeMovieDemo_OBJECTS = \
"CMakeFiles/sfeMovieDemo.dir/game.cpp.o"

# External object files for target sfeMovieDemo
sfeMovieDemo_EXTERNAL_OBJECTS =

sfeMovieDemo: CMakeFiles/sfeMovieDemo.dir/game.cpp.o
sfeMovieDemo: CMakeFiles/sfeMovieDemo.dir/build.make
sfeMovieDemo: CMakeFiles/sfeMovieDemo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/user/Nextcloud/School/CSE/OOP/oops/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable sfeMovieDemo"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sfeMovieDemo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/sfeMovieDemo.dir/build: sfeMovieDemo

.PHONY : CMakeFiles/sfeMovieDemo.dir/build

CMakeFiles/sfeMovieDemo.dir/requires: CMakeFiles/sfeMovieDemo.dir/game.cpp.o.requires

.PHONY : CMakeFiles/sfeMovieDemo.dir/requires

CMakeFiles/sfeMovieDemo.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/sfeMovieDemo.dir/cmake_clean.cmake
.PHONY : CMakeFiles/sfeMovieDemo.dir/clean

CMakeFiles/sfeMovieDemo.dir/depend:
	cd /home/user/Nextcloud/School/CSE/OOP/oops && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/user/Nextcloud/School/CSE/OOP/oops /home/user/Nextcloud/School/CSE/OOP/oops /home/user/Nextcloud/School/CSE/OOP/oops /home/user/Nextcloud/School/CSE/OOP/oops /home/user/Nextcloud/School/CSE/OOP/oops/CMakeFiles/sfeMovieDemo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/sfeMovieDemo.dir/depend

