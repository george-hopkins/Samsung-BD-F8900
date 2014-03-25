FW_ROOT := ../../../../..
OBJ_PATH = $(FW_ROOT)/output/$(CONFIG)/$(TARGET)/obj/gsp/src/bluecore/transport/type_a_sync
include $(FW_ROOT)/local_rules.mk

LIB_NAME := $(call lib_name,csr_type_a_sync)
LIB_PATH := $(FW_LIB)

INC += $(FW_INC) -I../inc -I../../inc/tm -I../../../../inc/bluecore/tm

###############################################################################
#
#                (c) Cambridge Silicon Radio Limited 2010
#
#                All rights reserved and confidential information of CSR
#
###############################################################################
include module.mk

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
