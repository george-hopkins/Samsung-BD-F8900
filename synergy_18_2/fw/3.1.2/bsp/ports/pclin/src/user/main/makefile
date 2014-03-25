################################################################################
#
#           (c) Cambridge Silicon Radio Limited 2009
#
#           All rights reserved and confidential information of CSR
#
################################################################################
FW_ROOT := ../../../../../..
OBJ_PATH = $(BSP_ROOT)/output/$(CONFIG)/$(TARGET)/obj/src/main
include $(FW_ROOT)/local_rules.mk

LIB_NAME := $(call lib_name,csr_main)
LIB_PATH := $(BSP_LIB)

INC := $(FW_INC) $(BSP_INC)

SRC := csr_main.c

OBJ := $(addprefix $(OBJ_PATH)/,$(SRC:.c=.o))

DEP := $(OBJ:.o=.d)
include $(FW_ROOT)/m_targets.mk

all: lib

lib: $(LIB_PATH)/$(LIB_NAME)

$(LIB_PATH)/$(LIB_NAME): $(OBJ)
	$(MKDIR) $(@D)
	$(call create_lib,$@,$(OBJ))

clean:
	$(RM) $(LIB_PATH)/$(LIB_NAME)
	$(RM) $(OBJ)
