# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /home/raphael/Documents/3A/SchoolProjects/Optimisation/Metaheuristics/proto_velib-20161105-07h49

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/raphael/Documents/3A/SchoolProjects/Optimisation/Metaheuristics/proto_velib-20161105-07h49/build

# Utility rule file for txz.

# Include the progress variables for this target.
include CMakeFiles/txz.dir/progress.make

CMakeFiles/txz:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/raphael/Documents/3A/SchoolProjects/Optimisation/Metaheuristics/proto_velib-20161105-07h49/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Création d'une archive datée du projet (TEST XZ)"
	@echo " => duplication du projet en : proto_velib-20161105-07h49-20161117-17h04"
	cp -Rp /home/raphael/Documents/3A/SchoolProjects/Optimisation/Metaheuristics/proto_velib-20161105-07h49 /home/raphael/Documents/3A/SchoolProjects/Optimisation/Metaheuristics/proto_velib-20161105-07h49-20161117-17h04
	rm -r /home/raphael/Documents/3A/SchoolProjects/Optimisation/Metaheuristics/proto_velib-20161105-07h49-20161117-17h04/build/
	mkdir -p /home/raphael/Documents/3A/SchoolProjects/Optimisation/Metaheuristics/proto_velib-20161105-07h49-20161117-17h04/build/
	tar cf /home/raphael/Documents/3A/SchoolProjects/Optimisation/Metaheuristics/proto_velib-20161105-07h49-20161117-17h04.tar -C /home/raphael/Documents/3A/SchoolProjects/Optimisation/Metaheuristics/proto_velib-20161105-07h49/.. proto_velib-20161105-07h49-20161117-17h04
	xz /home/raphael/Documents/3A/SchoolProjects/Optimisation/Metaheuristics/proto_velib-20161105-07h49-20161117-17h04.tar
	mv /home/raphael/Documents/3A/SchoolProjects/Optimisation/Metaheuristics/proto_velib-20161105-07h49-20161117-17h04.tar.xz /home/raphael/Documents/3A/SchoolProjects/Optimisation/Metaheuristics/proto_velib-20161105-07h49-20161117-17h04.txz
	rm -r /home/raphael/Documents/3A/SchoolProjects/Optimisation/Metaheuristics/proto_velib-20161105-07h49-20161117-17h04
	@echo " => Archive faite : proto_velib-20161105-07h49-20161117-17h04.tbz"

txz: CMakeFiles/txz
txz: CMakeFiles/txz.dir/build.make

.PHONY : txz

# Rule to build all files generated by this target.
CMakeFiles/txz.dir/build: txz

.PHONY : CMakeFiles/txz.dir/build

CMakeFiles/txz.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/txz.dir/cmake_clean.cmake
.PHONY : CMakeFiles/txz.dir/clean

CMakeFiles/txz.dir/depend:
	cd /home/raphael/Documents/3A/SchoolProjects/Optimisation/Metaheuristics/proto_velib-20161105-07h49/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/raphael/Documents/3A/SchoolProjects/Optimisation/Metaheuristics/proto_velib-20161105-07h49 /home/raphael/Documents/3A/SchoolProjects/Optimisation/Metaheuristics/proto_velib-20161105-07h49 /home/raphael/Documents/3A/SchoolProjects/Optimisation/Metaheuristics/proto_velib-20161105-07h49/build /home/raphael/Documents/3A/SchoolProjects/Optimisation/Metaheuristics/proto_velib-20161105-07h49/build /home/raphael/Documents/3A/SchoolProjects/Optimisation/Metaheuristics/proto_velib-20161105-07h49/build/CMakeFiles/txz.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/txz.dir/depend

