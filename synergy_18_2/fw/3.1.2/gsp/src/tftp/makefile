FW_ROOT := ../../..
OBJ_PATH = $(FW_ROOT)/output/$(CONFIG)/$(TARGET)/obj/gsp/src/tftp
include $(FW_ROOT)/local_rules.mk
include $(FW_ROOT)/m_targets_autogen.mk

LIB_NAME := $(call lib_name,csr_tftp)
LIB_NAME_TFTP_LIB := $(call lib_name,csr_tftp_lib)

LIB_PATH := $(FW_LIB)

INC += $(BSP_INC) $(FW_INC) -I../inc

SRC := csr_tftp_handler.c \
	csr_tftp_sef.c

TFTP_LIB_SRC := csr_tftp_free_up.c \
		csr_tftp_free_down.c \
		csr_tftp_lib.c \
		csr_tftp_lib_priv.c

OBJ := $(addprefix $(OBJ_PATH)/,$(call path_subst,$(SRC:.c=.o)))

TFTP_LIB_OBJ := $(addprefix $(OBJ_PATH)/,$(call path_subst,$(TFTP_LIB_SRC:.c=.o)))

DEP := $(OBJ:.o=.d) $(TFTP_LIB_OBJ:.o=.d)
include $(FW_ROOT)/m_targets.mk

all: lib

bin:

lib: $(LIB_PATH)/$(LIB_NAME) $(LIB_PATH)/$(LIB_NAME_TFTP_LIB)

$(LIB_PATH)/$(LIB_NAME): $(OBJ)
	$(MKDIR) $(@D)
	$(call create_lib,$@,$(OBJ))

$(LIB_PATH)/$(LIB_NAME_TFTP_LIB): $(TFTP_LIB_OBJ)
	$(MKDIR) $(@D)
	$(call create_lib,$@,$(TFTP_LIB_OBJ))

clean:
	$(RM) $(call path_subst,$(LIB_PATH)/$(LIB_NAME))
	$(RM) $(call path_subst,$(LIB_PATH)/$(LIB_NAME_TFTP_LIB))
	$(RM) $(call path_subst,$(OBJ))
	$(RM) $(call path_subst,$(TFTP_LIB_OBJ))
