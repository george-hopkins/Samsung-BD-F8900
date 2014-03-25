####### Global configuration

####### Build 
ifeq ($(BUILD),)
ifeq ($(OS),Windows_NT)
ifneq ($(filter $(TERM),cygwin xterm),)
BUILD := cygwin
else
BUILD := winnt
endif
else
OS := $(shell uname -pms)
ifneq ($(filter $(OS),Linux),)
BUILD := linux
ifneq ($(filter $(OS),x86_64 ia64 amd64),)
BUILD_ARCH := x86_64
else
BUILD_ARCH := x86
endif
else
BUILD := 
endif
endif
else
BUILD = $(BUILD)
endif
ifeq ($(BUILD),)
$(error BUILD undetermined)
endif
BUILD_SCRIPT_ROOT := $(FW_ROOT)/scripts
include $(BUILD_SCRIPT_ROOT)/build-$(BUILD).mk

####### BSP root
BSP_ROOT = $(FW_ROOT)/bsp/ports/$(TARGET_PLATFORM)

####### Target
ifeq ($(BUILD),linux)
TARGET ?= pclin-2.6-$(BUILD_ARCH)
else
TARGET ?= pcwin-nt-x86
endif

TARGET_PLATFORM := $(word 1,$(subst -, ,$(TARGET)))
TARGET_OS := $(word 2,$(subst -, ,$(TARGET)))
TARGET_ARCH := $(word 3,$(subst -, ,$(TARGET)))
TARGET_SCRIPT_ROOT := $(FW_ROOT)/bsp/ports/$(TARGET_PLATFORM)/scripts
include $(TARGET_SCRIPT_ROOT)/target-$(TARGET).mk

####### FW + BSP inc and lib path definitions
include $(BSP_ROOT)/paths.mk
include $(FW_ROOT)/paths.mk

####### Dependency Mapping #######
#
# The depmap target is used for mapping the dependencies of any given library, 
# as well as the dependencies towards any library (reverse dependencies). It
# analyses the libraries and therefore requires a full build to be completed
# before invoking this target. It is recommended to specify TARGET both when
# building the libraries and invoking the depmap target, to use the actual 
# platform libraries for the analysis. Also note that any compile time options
# affects the results, as the mapping shows the actual picture of link-time
# dependencies.
#
# The depmap target can be invoked from any makefile that defines LIB_NAME, 
# optionally overriding DEPMAP_LIBPATHS to include additional/other library 
# search paths (comma-separated list of directories), or DEPMAP_MAPLIBS to 
# specify additional/other libraries to map dependencies for (comma-separated 
# list of any mix of library names (with or without path) and directories 
# containing libraries). DEPMAP_MAPLIBS must be overridden if the makefile
# does not set LIB_NAME. DEPMAP_ARGS can be set to specify additional options:
#
#     --listlibs: If this option is specified, all libraries found in the
#                 directories specified by --libpaths will be printed, and
#                 no dependency mapping will be conducted.
#     --collapse: Collapse individual symbol dependencies into library
#                 dependencies. This is recommended when using --dot.
#     --noforward: Disregard forward dependencies (from maplibs to others)
#     --noreverse: Disregard reverse dependencies (to maplibs from others)
#     --noundefined: Disregard dependencies on symbols that are not defined in
#                    any library.
#     --dot: Output in dot format suitable for Graphviz (dot or neato).
#
# See http://www.graphviz.org/ for procuring and using Graphviz, which is a
# tool for converting dot files into graphical illustrations.
#
# Examples:
#  Everything related to LIB_NAME in dot format collapsed and piped to neato:
#   make depmap DEPMAP_ARGS="--collapse --dot" | neato -Tpdf > depmap.pdf
#
#  All dependencies relating to csr_log.lib and csr_sched.lib:
#   make depmap DEPMAP_MAPLIBS=csr_log.lib,csr_sched.lib
#
#  Every dependency on csr_framework_ext.lib
#   make depmap DEPMAP_MAPLIBS=csr_framework_ext.lib DEPMAP_ARGS=--noforward
#
# Note that the location where the target is invoked only matters as long as 
# DEPMAP_MAPLIBS is not overridden, because the only thing used from the 
# makefile is the LIB_NAME symbol. If DEPMAP_MAPLIBS is not overridden, it will
# usually result in outputting the dependencies of/on the library corresponding
# to the source code in the directory where the depmap target is invoked.
#
DEPMAP_LIBPATHS := $(FW_LIB),$(BSP_LIB)
DEPMAP_MAPLIBS = $(LIB_NAME)
depmap:
	@perl $(FW_ROOT)/tools/depmap/depmap.pl --libpaths $(DEPMAP_LIBPATHS) --maplibs $(DEPMAP_MAPLIBS) $(DEPMAP_ARGS)


####### Codesize Analysis #######
#
# The codesize target is used for reporting the code size of any given library
# as well as the code size of symbols within the library. It analyses the 
# libraries and therefore requires a build to be completed before invoking 
# this target. It is recommended to specify TARGET both when building the 
# libraries and invoking the codesize target, to use the actual platform 
# libraries for the analysis. Also note that any compile time options
# affects the results, as the mapping shows the actual picture of pre link-time
# code sizes.
#
# The codesize target can be invoked from any makefile that defines LIB_NAME, 
# optionally overriding CODESIZE_LIBPATHS to include additional/other library 
# search paths (comma-separated list of directories), or CODESIZE_LIBS to 
# specify additional/other libraries to report code sizes for (comma-separated 
# list of any mix of library names (with or without path) and directories 
# containing libraries). CODESIZE_LIBS must be overridden if the makefile
# does not set LIB_NAME. CODESIZE_ARGS can be set to specify additional options:
#
#     --listlibs: If this option is specified, all libraries found in the
#                 directories specified by --libpaths will be printed, and
#                 no code sizes will be reported.
#     --symbols: Report the code size of individual symbols.
#     --zeroinit: Include zero initialised sections.
#
# Example:
#  Code size of the crs_log.lib library:
#   make codesize CODESIZE_LIBS=csr_log.lib
#
#  Code size of all the symbols in the crs_log.lib library:
#   make codesize CODESIZE_LIBS=csr_log.lib CODESIZE_ARGS="--symbols"
#
# Note that the location where the target is invoked only matters as long as 
# CODESIZE_LIBS is not overridden, because the only thing used from the 
# makefile is the LIB_NAME symbol. If CODESIZE_LIBS is not overridden, it will
# usually result in reporting the codesize of the library corresponding
# to the source code in the directory where the codesize target is invoked.
#
CODESIZE_LIBPATHS := $(FW_LIB),$(BSP_LIB)
CODESIZE_LIBS = $(LIB_NAME)
codesize:
	@perl $(FW_ROOT)/tools/codesize/codesize.pl --libpaths $(CODESIZE_LIBPATHS) --libs $(CODESIZE_LIBS) $(CODESIZE_ARGS)
