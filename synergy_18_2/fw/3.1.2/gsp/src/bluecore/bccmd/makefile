FW_ROOT := ../../../..
OBJ_PATH = $(FW_ROOT)/output/$(CONFIG)/$(TARGET)/obj/gsp/src/bluecore/bccmd
include $(FW_ROOT)/local_rules.mk
include $(FW_ROOT)/m_targets_autogen_free.mk

LIB_NAME := $(call lib_name,csr_bccmd)
LIB_PATH := $(FW_LIB)

INC += -I . -I ../inc/bccmd -I ../inc/tm -I ../../inc

SRC := csr_bccmd_handler.c csr_bccmd_lib.c csr_bccmd_sef.c csr_bccmd_private_lib.c csr_bccmd_util.c
ifeq ($(CHIP_MANAGER_ENABLE),1)
SRC += csr_bccmd_cm.c
endif

FREE_SRC := csr_bccmd_free_down.c csr_bccmd_free_up.c

SRC += $(FREE_SRC)

OBJ := $(addprefix $(OBJ_PATH)/,$(call path_subst,$(SRC:.c=.o)))

DEP := $(OBJ:.o=.d)
include $(FW_ROOT)/m_targets.mk

all: lib

bin:

lib: $(LIB_PATH)/$(LIB_NAME)

$(LIB_PATH)/$(LIB_NAME): $(OBJ)
	$(MKDIR) $(@D)
	$(call create_lib,$@,$(OBJ))

clean:
	$(RM) $(call path_subst,$(LIB_PATH)/$(LIB_NAME))
	$(RM) $(call path_subst,$(OBJ))
