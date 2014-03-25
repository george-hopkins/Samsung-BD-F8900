#-*-Makefile-*-
###############################################################################
#
#                (c) Cambridge Silicon Radio Limited 2010
#
#                All rights reserved and confidential information of CSR
#
###############################################################################
FW_ROOT := ../../..
OBJ_PATH = $(FW_ROOT)/output/$(CONFIG)/$(TARGET)/obj/gsp/src/converter
include $(FW_ROOT)/local_rules.mk

MODULE_NAME := csr_msg_converter

LIB_NAME := $(call lib_name,$(MODULE_NAME))
LIB_PATH := $(FW_LIB)

INC += -I. -I../inc -I../bluecore/inc/bccmd -I../bluecore/inc/tm -I../bluecore/inc/hci -I../application/ui -I$(BSP_INC) -I$(FW_INC)

SRC := $(wildcard *.c)
OBJ := $(addprefix $(OBJ_PATH)/,$(SRC:.c=.o))

DEP := $(OBJ:.o=.d)
include $(FW_ROOT)/m_targets.mk

all: lib

lib: $(LIB_PATH)/$(LIB_NAME)

$(LIB_PATH)/$(LIB_NAME): $(OBJ)
	$(MKDIR) $(@D)
	$(call create_lib,$@,$(OBJ))

clean:
	$(RM) $(call path_subst,$(LIB_PATH)/$(LIB_NAME))
	$(RM) $(call path_subst,$(OBJ))
