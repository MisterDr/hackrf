# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.2

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
CMAKE_COMMAND = /opt/local/bin/cmake

# The command to remove a file.
RM = /opt/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Volumes/mrdr/research/hackrf/firmware

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Volumes/mrdr/research/hackrf/firmware

# Include any dependencies generated for this target.
include mixertx/CMakeFiles/mixertx_m0.elf.dir/depend.make

# Include the progress variables for this target.
include mixertx/CMakeFiles/mixertx_m0.elf.dir/progress.make

# Include the compile flags for this target's objects.
include mixertx/CMakeFiles/mixertx_m0.elf.dir/flags.make

mixertx/CMakeFiles/mixertx_m0.elf.dir/__/common/m0_sleep.c.obj: mixertx/CMakeFiles/mixertx_m0.elf.dir/flags.make
mixertx/CMakeFiles/mixertx_m0.elf.dir/__/common/m0_sleep.c.obj: common/m0_sleep.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Volumes/mrdr/research/hackrf/firmware/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object mixertx/CMakeFiles/mixertx_m0.elf.dir/__/common/m0_sleep.c.obj"
	cd /Volumes/mrdr/research/hackrf/firmware/mixertx && /Users/djenadrazic/arm-cs-tools/bin/arm-none-eabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/mixertx_m0.elf.dir/__/common/m0_sleep.c.obj   -c /Volumes/mrdr/research/hackrf/firmware/common/m0_sleep.c

mixertx/CMakeFiles/mixertx_m0.elf.dir/__/common/m0_sleep.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/mixertx_m0.elf.dir/__/common/m0_sleep.c.i"
	cd /Volumes/mrdr/research/hackrf/firmware/mixertx && /Users/djenadrazic/arm-cs-tools/bin/arm-none-eabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /Volumes/mrdr/research/hackrf/firmware/common/m0_sleep.c > CMakeFiles/mixertx_m0.elf.dir/__/common/m0_sleep.c.i

mixertx/CMakeFiles/mixertx_m0.elf.dir/__/common/m0_sleep.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/mixertx_m0.elf.dir/__/common/m0_sleep.c.s"
	cd /Volumes/mrdr/research/hackrf/firmware/mixertx && /Users/djenadrazic/arm-cs-tools/bin/arm-none-eabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /Volumes/mrdr/research/hackrf/firmware/common/m0_sleep.c -o CMakeFiles/mixertx_m0.elf.dir/__/common/m0_sleep.c.s

mixertx/CMakeFiles/mixertx_m0.elf.dir/__/common/m0_sleep.c.obj.requires:
.PHONY : mixertx/CMakeFiles/mixertx_m0.elf.dir/__/common/m0_sleep.c.obj.requires

mixertx/CMakeFiles/mixertx_m0.elf.dir/__/common/m0_sleep.c.obj.provides: mixertx/CMakeFiles/mixertx_m0.elf.dir/__/common/m0_sleep.c.obj.requires
	$(MAKE) -f mixertx/CMakeFiles/mixertx_m0.elf.dir/build.make mixertx/CMakeFiles/mixertx_m0.elf.dir/__/common/m0_sleep.c.obj.provides.build
.PHONY : mixertx/CMakeFiles/mixertx_m0.elf.dir/__/common/m0_sleep.c.obj.provides

mixertx/CMakeFiles/mixertx_m0.elf.dir/__/common/m0_sleep.c.obj.provides.build: mixertx/CMakeFiles/mixertx_m0.elf.dir/__/common/m0_sleep.c.obj

# Object files for target mixertx_m0.elf
mixertx_m0_elf_OBJECTS = \
"CMakeFiles/mixertx_m0.elf.dir/__/common/m0_sleep.c.obj"

# External object files for target mixertx_m0.elf
mixertx_m0_elf_EXTERNAL_OBJECTS =

mixertx/mixertx_m0.elf: mixertx/CMakeFiles/mixertx_m0.elf.dir/__/common/m0_sleep.c.obj
mixertx/mixertx_m0.elf: mixertx/CMakeFiles/mixertx_m0.elf.dir/build.make
mixertx/mixertx_m0.elf: mixertx/CMakeFiles/mixertx_m0.elf.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable mixertx_m0.elf"
	cd /Volumes/mrdr/research/hackrf/firmware/mixertx && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mixertx_m0.elf.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
mixertx/CMakeFiles/mixertx_m0.elf.dir/build: mixertx/mixertx_m0.elf
.PHONY : mixertx/CMakeFiles/mixertx_m0.elf.dir/build

mixertx/CMakeFiles/mixertx_m0.elf.dir/requires: mixertx/CMakeFiles/mixertx_m0.elf.dir/__/common/m0_sleep.c.obj.requires
.PHONY : mixertx/CMakeFiles/mixertx_m0.elf.dir/requires

mixertx/CMakeFiles/mixertx_m0.elf.dir/clean:
	cd /Volumes/mrdr/research/hackrf/firmware/mixertx && $(CMAKE_COMMAND) -P CMakeFiles/mixertx_m0.elf.dir/cmake_clean.cmake
.PHONY : mixertx/CMakeFiles/mixertx_m0.elf.dir/clean

mixertx/CMakeFiles/mixertx_m0.elf.dir/depend:
	cd /Volumes/mrdr/research/hackrf/firmware && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Volumes/mrdr/research/hackrf/firmware /Volumes/mrdr/research/hackrf/firmware/mixertx /Volumes/mrdr/research/hackrf/firmware /Volumes/mrdr/research/hackrf/firmware/mixertx /Volumes/mrdr/research/hackrf/firmware/mixertx/CMakeFiles/mixertx_m0.elf.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : mixertx/CMakeFiles/mixertx_m0.elf.dir/depend
