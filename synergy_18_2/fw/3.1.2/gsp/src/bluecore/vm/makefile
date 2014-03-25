FW_ROOT := ../../../..
OBJ_PATH = $(FW_ROOT)/output/$(CONFIG)/$(TARGET)/obj/gsp/src/bluecore/wm
include $(FW_ROOT)/local_rules.mk
include $(FW_ROOT)/m_targets_autogen.mk

LIB_NAME := $(call lib_name,csr_vm)
LIB_PATH := $(FW_LIB)

INC += -I . -I ../../inc

SRC := csr_vm_handler.c csr_vm_lib.c

FREE_SRC := csr_vm_free_down.c csr_vm_free_up.c

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
	$(RM) *~
	$(RM) $(call path_subst,$(LIB_PATH)/$(LIB_NAME))
	$(RM) $(call path_subst,$(OBJ))
