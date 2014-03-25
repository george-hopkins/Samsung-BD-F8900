#-*-Makefile-*-
###############################################################################
#
#                (c) Cambridge Silicon Radio Limited 2009
#
#                All rights reserved and confidential information of CSR
#
###############################################################################
FW_ROOT := ../../../../../..
OBJ_PATH = $(BSP_ROOT)/output/$(CONFIG)/$(TARGET)/obj/src/app_main
include $(FW_ROOT)/local_rules.mk

LIB_NAME := $(call lib_name,csr_app_main)
LIB_BLUECORE_NAME := $(call lib_name,csr_app_main_bluecore)
LIB_PATH := $(BSP_LIB)

INC += $(FW_INC) $(BSP_INC)

ifeq ($(BSP_SUPPORT_SDL),1)
CFLAGS += -DBSP_SUPPORT_SDL $(shell pkg-config --cflags sdl)
endif

SRC_COMMON := csr_app_main.c \
              csr_key_input.c \
              csr_ui_lower.c

SRC := $(SRC_COMMON) csr_app_main_transport.c

SRC_BLUECORE := $(SRC_COMMON) csr_app_main_transport_bluecore.c

OBJ := $(addprefix $(OBJ_PATH)/,$(call path_subst,$(SRC:.c=.o)))
OBJ_BLUECORE := $(addprefix $(OBJ_PATH)/,$(call path_subst,$(SRC_BLUECORE:.c=.o)))

DEP := $(OBJ:.o=.d) $(OBJ_BLUECORE:.o=.d)
include $(FW_ROOT)/m_targets.mk

all: lib

lib: $(LIB_PATH)/$(LIB_NAME) $(LIB_PATH)/$(LIB_BLUECORE_NAME)

$(LIB_PATH)/$(LIB_NAME): $(OBJ)
	$(MKDIR) $(@D)
	$(call create_lib,$@,$(OBJ))

$(LIB_PATH)/$(LIB_BLUECORE_NAME): $(OBJ_BLUECORE)
	$(MKDIR) $(@D)
	$(call create_lib,$@,$(OBJ_BLUECORE))

clean:
	$(RM) $(call path_subst,$(LIB_PATH)/$(LIB_NAME))
	$(RM) $(call path_subst,$(OBJ) $(OBJ_BLUECORE))
