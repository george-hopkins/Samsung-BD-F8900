###############################################################################
#
#                (c) Cambridge Silicon Radio Limited 2010
#
#                All rights reserved and confidential information of CSR
#
###############################################################################

#
# Reference parameters for measuring code size. Do not change, and do not change
# any parameters in the rest of the target script that affect code generation.
#
ifeq ($(CODESIZE_BUILD),1)
ARM_COMPILER := 3.0
ARM_INSTRUCTION_SET := thumb
OPTIMIZE_SPACE := 1
OPTIMIZE_LEVEL := 3
endif

#
# Reference parameters for measuring performance. Do not change, and do not change
# any parameters in the rest of the target script that affect code generation.
#
ifeq ($(PERFORMANCE_BUILD),1)
ARM_COMPILER := 3.0
ARM_INSTRUCTION_SET := arm
OPTIMIZE_TIME := 1
OPTIMIZE_LEVEL := 2
endif

#
# Set default optimization level based on DEBUG
# Set DEBUG or OPTIMIZE_LEVEL to change (latter wins)
#
ifneq ($(DEBUG),1)
OPTIMIZE_LEVEL ?= 3
else
OPTIMIZE_LEVEL ?= 0
endif

#
# Set optimize for time as default
# Set OPTIMIZE_SPACE=1 to optimize for space
# Set OPTIMIZE_TIME=1 to optimize for time
#
OPTIMIZE_TIME ?= 1
ifeq ($(OPTIMIZE_TIME),1)
    TARGET_CFLAGS := -Otime
endif
ifeq ($(OPTIMIZE_SPACE),1)
    TARGET_CFLAGS := -Ospace
endif

#
# Set default instruction set to Thumb
# Set ARM_INSTRUCTION_SET=arm to change
#
ARM_INSTRUCTION_SET ?= thumb

#
# Default to RVCT3.0
#
ARM_COMPILER ?= 3.0

# ADS1.2
ifeq ($(ARM_COMPILER),ADS)
    ARM_COMPILER_PATH := "$(ARMHOME)\bin"
    ifeq ($(OPTIMIZE_LEVEL),3)
        OPTIMIZE_LEVEL := 2
    endif
    ARM_INSTRUCTION_SET := arm
    ifeq ($(ARM_INSTRUCTION_SET),thumb)
        $(error ARM_INSTRUCTION_SET=thumb is not supported when ARM_COMPILER=$(ARM_COMPILER))
    endif
endif

# RVCT2.2
ifeq ($(ARM_COMPILER),2.2)
    ARM_COMPILER_PATH := "$(RVCT22BIN)"
    ARM_INSTRUCTION_SET := arm
    ifeq ($(ARM_INSTRUCTION_SET),thumb)
        $(error ARM_INSTRUCTION_SET=thumb is not supported when ARM_COMPILER=$(ARM_COMPILER))
    endif
endif

# RVCT3.0
ifeq ($(ARM_COMPILER),3.0)
    ARM_COMPILER_PATH := "$(RVCT30BIN)"
    ifneq ($(filter $(TARGET_CFLAGS),-Otime),)
        ifeq ($(OPTIMIZE_LEVEL),3)
            OPTIMIZE_LEVEL := 2
        endif
        ifeq ($(OPTIMIZE_LEVEL),3)
            $(error Realview ARM Compiler 3.0 will generate invalid code when using -O3 -Otime, use -O2 -Otime instead)
        endif
    endif
endif

# RVCT3.1
ifeq ($(ARM_COMPILER),3.1)
    ARM_COMPILER_PATH := "$(RVCT31BIN)"
endif

# RVCT4.0
ifeq ($(ARM_COMPILER),4.0)
    ARM_COMPILER_PATH := "$(RVCT40BIN)"
endif

# RVCT4.1
ifeq ($(ARM_COMPILER),4.1)
    ARM_COMPILER_PATH := "$(ARMCC41BIN)"
endif

ifeq ($(ARM_COMPILER_PATH),)
    $(error ARM_COMPILER=$(ARM_COMPILER) invalid, use: ADS, 2.2, 3.0, 3.1, 4.0 or 4.1)
endif
CC  := $(ARM_COMPILER_PATH)/armcc
LD  := $(ARM_COMPILER_PATH)/armlink
AR  := $(ARM_COMPILER_PATH)/armar
CXX := $(ARM_COMPILER_PATH)/armcc
ASM := $(ARM_COMPILER_PATH)/armasm
FE  := $(ARM_COMPILER_PATH)/fromelf

# The path to Nucleus PLUS RTOS - only needed when compiling to targets
# that run Nucleus PLUS RTOS. This parameter is not used for compiling
# generic code.
#
NUCLEUS_ROOT ?= c:/csr/nucleus/2.6.0
NUCLEUS_CONFIG ?= default

MODULE_TEST_MAIN ?= $(FW_ROOT)/test/module_test/csr_module_test_main_nucleus.c
SCATTER_FILE ?= $(BSP_ROOT)/scripts/bdb2.sct

BSP_SUPPORT_ACLBUF := 1

TARGET_LIBS := $(NUCLEUS_ROOT)/output/$(NUCLEUS_CONFIG)/$(TARGET)/lib/nucleus

ifeq ($(CSR_INSTRUMENTED_PROFILING_SERVICE),1)
TARGET_LIBS += csr_ips
endif

# Hardware
ARM_CORE ?= ARM926EJ-S
ARM_FPU  ?= SoftVFP

ifneq ($(filter $(ARM_COMPILER),2.2 3.0 3.1 4.0 4.1),)
    # Compiler flags
    TARGET_CFLAGS += -c -g -O$(OPTIMIZE_LEVEL) --$(ARM_INSTRUCTION_SET) --cpu $(ARM_CORE) --fpu $(ARM_FPU) --diag_suppress=9931,9933
    # Assembler flags
    TARGET_SFLAGS = -g --keep --cpu $(ARM_CORE) --fpu $(ARM_FPU) --apcs /interwork
    # Linker Flags
    TARGET_LFLAGS = --info totals --info unused --info veneers --info sizes --symbols --map --xref --nolocals --entry __main --list TMP_LIST_FILE_NAME --scatter $(SCATTER_FILE) --output
    # Fromelf Flags
    TARGET_EFLAGS = --bin --output
    # Treat warnings/remarks as errors
    ifneq ($(NO_TREAT_WARN_AS_ERROR),1)
        ifneq ($(filter $(ARM_COMPILER),4.0 4.1),)
            TARGET_CFLAGS += --diag_error=warning
        else
            TARGET_CFLAGS += --diag_error=1,9,69,111,144,167,174,175,177,181,186,187,188,223,224,260,513,550,767,940,1134,1166,1293,1295,1441,3017
        endif
    endif
endif

ifneq ($(filter $(ARM_COMPILER),ADS),)
    # Compiler flags
    TARGET_CFLAGS += -c -g -O$(OPTIMIZE_LEVEL) -cpu $(ARM_CORE) -fpu $(ARM_FPU)
    # Assembler flags
    TARGET_SFLAGS := -g -keep -cpu $(ARM_CORE) -fpu $(ARM_FPU) -apcs /interwork
    # Linker Flags
    TARGET_LFLAGS = -info totals -info unused -info veneers -info sizes -symbols -map -xref -nolocals -entry __main -list TMP_LIST_FILE_NAME -scatter $(SCATTER_FILE) -output
    # Fromelf Flags
    TARGET_EFLAGS := -bin
endif

TARGET_CXXFLAGS += $(TARGET_CFLAGS)
EXTRA_CXXFLAGS += $(EXTRA_CFLAGS)
CXXFLAGS += $(CFLAGS)

ifneq ($(ARM_COMPILER),ADS)
    CXXFLAGS += --using_std --no_exceptions --no_rtti
endif

#####################
# Macro definitions #
#####################

#
# Append correct filename extension
#
# $(1) = Name
#
axf_name = $(1).axf
lib_name = $(1).lib
bin_name = $(1).bin

#
# Create library using appropriate archiver
#
# $(1) = Output file name (with path)
# $(2) = List of object files (with path)
#
create_lib = $(AR) -r $(1) $(2)

#
# Create ARM Executable Format (AXF) file
#
# $(1) = Output file name (with path)
# $(2) = List of object files and libraries (optionally with path)
# $(3) = LDPATH (optional)
# $(4) = LIBS (optional)
#
create_axf = $(LD) $(EXTRA_LFLAGS) $(3) $(TARGET_LFLAGS:TMP_LIST_FILE_NAME=$(1:.axf=.lst)) $(1) $(2) $(4)

#
# Create Binary Image from AXF file
#
# $(1) = Output file name (with path)
# $(2) = Input file (axf file)
#
create_bin = $(FE) $(EXTRA_EFLAGS) $(TARGET_EFLAGS) $(1) $(2)

# Create Binary Image using intermediate AXF file
#
# $(1) = Output file name (with path)
# $(2) = List of object files and libraries (optionally with path)
# $(3) = LDPATH (optional)
# $(4) = LIBS (optional)
#
create_exec = $(LD) $(EXTRA_LFLAGS) $(3) $(TARGET_LFLAGS:TMP_LIST_FILE_NAME=$(1:.bin=.lst)) $(1:.bin=.axf) $(2) $(4) && $(FE) $(EXTRA_EFLAGS) $(TARGET_EFLAGS) $(1) $(1:.bin=.axf)

# Clean up after create_exec
#
# $(1) = Output file name (with path)
#
clean_exec = $(RM) $(1) $(1:.bin=.axf) $(1:.bin=.lst)

# $(1) = list of libs path.
create_ldpath = $(addprefix --userlibpath ,$(1))

#
# Format list of libraries for use by create_axf
#
# $(1) = List of libraries (with path)
#
use_lib = $(addsuffix .lib,$(1))

#
# Rules
#
$(OBJ_PATH)/%.o:%.c
	$(MKDIR) $(@D)
	$(CC) $(EXTRA_CFLAGS) $(CFLAGS) $(TARGET_CFLAGS) $(INC) -c -o $@ $<

$(OBJ_PATH)/%.o:%.cpp
	$(MKDIR) $(@D)
	$(CXX) $(EXTRA_CXXFLAGS) $(CXXFLAGS) $(TARGET_CXXFLAGS) $(INC) -c -o $@ $<

$(OBJ_PATH)/%.o:%.s
	$(MKDIR) $(@D)
	$(ASM) $(EXTRA_SFLAGS) $(SFLAGS) $(TARGET_SFLAGS) -o $@ $<
