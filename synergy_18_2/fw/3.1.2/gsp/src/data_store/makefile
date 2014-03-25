FW_ROOT := ../../..
OBJ_PATH = $(FW_ROOT)/output/$(CONFIG)/$(TARGET)/obj/gsp/src/data_store
include $(FW_ROOT)/local_rules.mk
include $(FW_ROOT)/m_targets_autogen.mk

LIB_NAME := $(call lib_name,csr_data_store)
LIB_NAME_DATA_STORE_LIB := $(call lib_name,csr_data_store_lib)

LIB_PATH := $(FW_LIB)

INC += $(BSP_INC) $(FW_INC) -I../inc

SRC := csr_data_store_handler.c \
	csr_data_store_sef.c \
	csr_data_store_utils.c

DATA_STORE_LIB_SRC := csr_data_store_free_up.c \
			csr_data_store_free_down.c \
			csr_data_store_lib.c \
			csr_data_store_lib_priv.c

OBJ := $(addprefix $(OBJ_PATH)/,$(call path_subst,$(SRC:.c=.o)))

DATA_STORE_LIB_OBJ := $(addprefix $(OBJ_PATH)/,$(call path_subst,$(DATA_STORE_LIB_SRC:.c=.o)))

DEP := $(OBJ:.o=.d) $(DATA_STORE_LIB_OBJ:.o=.d)
include $(FW_ROOT)/m_targets.mk

all: lib

bin:

lib: $(LIB_PATH)/$(LIB_NAME) $(LIB_PATH)/$(LIB_NAME_DATA_STORE_LIB)

$(LIB_PATH)/$(LIB_NAME): $(OBJ)
	$(MKDIR) $(@D)
	$(call create_lib,$@,$(OBJ))

$(LIB_PATH)/$(LIB_NAME_DATA_STORE_LIB): $(DATA_STORE_LIB_OBJ)
	$(MKDIR) $(@D)
	$(call create_lib,$@,$(DATA_STORE_LIB_OBJ))

clean:
	$(RM) $(call path_subst,$(LIB_PATH)/$(LIB_NAME))
	$(RM) $(call path_subst,$(LIB_PATH)/$(LIB_NAME_DATA_STORE_LIB))
	$(RM) $(call path_subst,$(OBJ))
	$(RM) $(call path_subst,$(DATA_STORE_LIB_OBJ))
