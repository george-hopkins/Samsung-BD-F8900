FW_ROOT := ../../..
OBJ_PATH = $(FW_ROOT)/output/$(CONFIG)/$(TARGET)/obj/gsp/src/fsal
include $(FW_ROOT)/local_rules.mk
include $(FW_ROOT)/m_targets_autogen.mk

LIB_NAME := $(call lib_name,csr_fsal)
FSAL_LIB_NAME := $(call lib_name,csr_fsal_lib)

LIB_PATH := $(FW_LIB)

INC += $(BSP_INC) $(FW_INC) -I../inc

LIB_SRC := csr_fsal_free_up.c \
		csr_fsal_free_down.c \
		csr_fsal_lib.c \
		csr_fsal_lib_priv.c

SRC := csr_fsal_handler.c \
	csr_fsal_sef.c \
	csr_fsal_sef_utils.c \
	csr_fsal_utils.c

LIB_OBJ := $(addprefix $(OBJ_PATH)/,$(call path_subst,$(LIB_SRC:.c=.o)))
OBJ := $(addprefix $(OBJ_PATH)/,$(call path_subst,$(SRC:.c=.o)))

DEP := $(OBJ:.o=.d)
include $(FW_ROOT)/m_targets.mk

all: lib

bin:

lib: $(LIB_PATH)/$(LIB_NAME) $(LIB_PATH)/$(FSAL_LIB_NAME)

$(LIB_PATH)/$(LIB_NAME): $(OBJ)
	$(MKDIR) $(@D)
	$(call create_lib,$@,$(OBJ))

$(LIB_PATH)/$(FSAL_LIB_NAME): $(LIB_OBJ)
	$(MKDIR) $(@D)
	$(call create_lib,$@,$(LIB_OBJ))

clean:
	$(RM) $(call path_subst,$(LIB_PATH)/$(LIB_NAME))
	$(RM) $(call path_subst,$(OBJ))
	$(RM) $(call path_subst,$(LIB_PATH)/$(FSAL_LIB_NAME))
	$(RM) $(call path_subst,$(LIB_OBJ))
