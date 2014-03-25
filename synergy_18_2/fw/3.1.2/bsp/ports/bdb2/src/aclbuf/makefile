###############################################################################
#
#                (c) Cambridge Silicon Radio Limited 2009
#
#                All rights reserved and confidential information of CSR
#
###############################################################################
FW_ROOT := ../../../../..
OBJ_PATH = $(BSP_ROOT)/output/$(CONFIG)/$(TARGET)/obj/src/aclbuf
include $(FW_ROOT)/local_rules.mk

LIB_PATH = $(BSP_LIB)
LIB_NAME := $(call lib_name,csr_aclbuf)

SRC := csr_aclbuf.c 

ifeq ($(EXCEPTION_PANIC),1)
CFLAGS += -DEXCEPTION_PANIC
endif

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
