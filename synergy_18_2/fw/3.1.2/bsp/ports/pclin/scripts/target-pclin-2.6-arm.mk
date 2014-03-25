###############################################################################
#
#                (c) Cambridge Silicon Radio Limited 2010
#
#                All rights reserved and confidential information of CSR
#
# REVISION:      $Revision$
###############################################################################

# D-13752
NO_TREAT_WARN_AS_ERROR := 1

include $(TARGET_SCRIPT_ROOT)/target-default.mk

BSP_SUPPORT_POSIX ?= 1

#CROSS_COMPILE := arm-v7a8-linux-gnueabi-
#CROSS_COMPILE := arm-v7a8v2r2-linux-gnueabi-
#CROSS_COMPILE := arm-v7a9v3r0-linux-gnueabi-
CROSS_COMPILE := arm-v7a8v3r1-linux-gnueabi-

#COMPILER_PATH = /opt/VDLinux-armv7a8-toolchain-lite_20100630/bin/
#COMPILER_PATH = /opt/VDLinux-ARMv7-4.4-202-toolchain-v2r2-20110630/bin/
#COMPILER_PATH = /opt/VDLinux-ARMv7-4.6-v3r0-20120321/bin/
COMPILER_PATH = /opt/arm-v7a8v3r1-20121018/bin
# "CROSS_COMPILE" defined in ~/.bashrc
# Defaultly, Codesourcery G++ Lite Edition for ARM is used
CC := $(COMPILER_PATH)/$(CROSS_COMPILE)gcc
LD := $(COMPILER_PATH)/$(CROSS_COMPILE)ld
AR := $(COMPILER_PATH)/$(CROSS_COMPILE)ar
CXX := $(COMPILER_PATH)/$(CROSS_COMPILE)cpp

ifeq ($(FORCE_ANSI),1) # D-13752
TARGET_CFLAGS += -ansi -pedantic
endif

TARGET_CXXFLAGS += $(TARGET_CFLAGS)
EXTRA_CXXFLAGS += $(EXTRA_CFLAGS)
CXXFLAGS += $(CFLAGS)

MODE=user

INC+= -I $(FW_ROOT)/inc
INC+= -I $(BSP_ROOT)/inc/

SUB_TARGET_ARCH := Linux
SUB_TARGET_VERSION := 2.6

ifeq ($(GENERATE_SERIALIZATION),)
GENERATE_SERIALIZATION := 1
else
GENERATE_SERIALIZATION := $(GENERATE_SERIALIZATION)
endif

TARGET_LIBS := pthread rt

# $(1) = name
lib_name = lib$(1).a

# $(1) = name
bin_name = $(1)

# $(1) = name
# $(2) = OBJ
create_lib = $(AR) -r $(1) $(2)

# $(1) = name
# $(2) = OBJ
# $(3) = LDPATH
# $(4) = LIBS
create_bin = $(CC) -o $(1) $(EXTRA_CFLAGS) $(TARGET_CFLAGS) $(CFLAGS) $(2) $(LDFLAGS) $(3) $(4)
create_exec = $(CC) -o $(1) $(EXTRA_CFLAGS) $(TARGET_CFLAGS) $(CFLAGS) $(2) $(LDFLAGS) $(3) $(4)

# Clean up after create_exec
#
# $(1) = Output file name (with path)
#
clean_exec = $(RM) $(1)

# $(1) = list of libs path.
create_ldpath = $(addprefix -L,$(1))

# $(1) = list of libs.
use_lib = $(addprefix -l,$(1))

ifeq ($(USE_ABSOLUTE_PATH),1)
abs_path = "$(shell pwd)/$(1)"
else
abs_path = $(1)
endif

#
# Rules
#
$(OBJ_PATH)/%.o:%.c
	@echo $<
	$(MKDIR) $(@D)
	$(CC) $(EXTRA_CFLAGS) $(TARGET_CFLAGS) $(CFLAGS) $(INC) -c -o $@ $(call abs_path,$<)

$(OBJ_PATH)/%.o:%.cpp
	@echo $<
	$(MKDIR) $(@D)
	$(CXX) $(EXTRA_CXXFLAGS) $(TARGET_CXXFLAGS) $(CXXFLAGS) $(INC) -c -o $@ $(call abs_path,$<)
