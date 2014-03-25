FW_ROOT := ../../..
OBJ_PATH = $(FW_ROOT)/output/$(CONFIG)/$(TARGET)/obj/gsp/src/tftp_file
include $(FW_ROOT)/local_rules.mk
include $(FW_ROOT)/m_targets_autogen.mk

LIB_NAME := $(call lib_name,csr_tftp_file)
LIB_NAME_TFTP_FILE_LIB := $(call lib_name,csr_tftp_file_lib)

LIB_PATH := $(FW_LIB)

INC += $(BSP_INC) $(FW_INC) -I../inc

SRC := csr_tftp_file_handler.c \
	csr_tftp_file_sef.c

OBJ := $(addprefix $(OBJ_PATH)/,$(call path_subst,$(SRC:.c=.o)))

TFTP_FILE_LIB_OBJ := $(addprefix $(OBJ_PATH)/,$(call path_subst,$(TFTP_FILE_LIB_SRC:.c=.o)))

DEP := $(OBJ:.o=.d) $(TFTP_FILE_LIB_OBJ:.o=.d)
include $(FW_ROOT)/m_targets.mk

all: lib

bin:

lib: $(LIB_PATH)/$(LIB_NAME) $(LIB_PATH)/$(LIB_NAME_TFTP_FILE_LIB)

$(LIB_PATH)/$(LIB_NAME): $(OBJ)
	$(MKDIR) $(@D)
	$(call create_lib,$@,$(OBJ))

$(LIB_PATH)/$(LIB_NAME_TFTP_FILE_LIB): $(TFTP_FILE_LIB_OBJ)
	$(MKDIR) $(@D)
	$(call create_lib,$@,$(TFTP_FILE_LIB_OBJ))

clean:
	$(RM) $(call path_subst,$(LIB_PATH)/$(LIB_NAME))
	$(RM) $(call path_subst,$(LIB_PATH)/$(LIB_NAME_TFTP_FILE_LIB))
	$(RM) $(call path_subst,$(OBJ))
	$(RM) $(call path_subst,$(TFTP_FILE_LIB_OBJ))
