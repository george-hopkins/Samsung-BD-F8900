FW_ROOT := ../../../..
OBJ_PATH = $(FW_ROOT)/output/$(CONFIG)/$(TARGET)/obj/gsp/src/bluecore/hci
include $(FW_ROOT)/local_rules.mk
include $(FW_ROOT)/m_targets_autogen_free.mk

LIB_NAME := $(call lib_name,csr_hci)
LIB_PATH := $(FW_LIB)

INC += -I . -I inc -I ../inc/bccmd -I ../../inc -I ../inc/tm -I ../inc/hci

SRC := csr_hci_handler.c csr_hci_lib.c csr_hci_sef.c csr_hci_util.c csr_hci_sco.c

FREE_SRC := csr_hci_free_down.c csr_hci_free_up.c csr_hci_private_free_down.c csr_hci_private_free_up.c

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
