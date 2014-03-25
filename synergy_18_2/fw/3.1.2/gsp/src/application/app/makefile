#-*-Makefile-*-
###############################################################################
#
#                (c) Cambridge Silicon Radio Limited 2009
#
#                All rights reserved and confidential information of CSR
#
###############################################################################
FW_ROOT := ../../../..
OBJ_PATH = $(FW_ROOT)/output/$(CONFIG)/$(TARGET)/obj/gsp/src/application/app
include $(FW_ROOT)/local_rules.mk
include $(FW_ROOT)/m_targets_autogen.mk

LIB_NAME := $(call lib_name,csr_app)
LIB_PATH := $(FW_LIB)

INC := $(FW_INC) $(BSP_INC) -I$(FW_ROOT)/gsp/src/inc

SRC := csr_app_handler.c \
       csr_app_lib.c \
       csr_app_lower_private.c \
       csr_app_ui_sef.c \
       csr_app_util.c 

FREE_SRC := csr_app_free_up.c \
            csr_app_free_down.c 

SRC += $(FREE_SRC)

OBJ := $(addprefix $(OBJ_PATH)/,$(call path_subst,$(SRC:.c=.o)))

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
