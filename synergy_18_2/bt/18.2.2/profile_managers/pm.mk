#-*-Makefile-*-
###############################################################################
#
#                (c) Cambridge Silicon Radio Limited 2009
#
#                All rights reserved and confidential information of CSR
#
# REVISION:      $Revision$
###############################################################################
LIB_NAME := $(call lib_name,csr_bt_$(MODULE_NAME))
API_NAME := $(call lib_name,csr_bt_$(MODULE_NAME)_lib)

LIB_PATH := $(TOP)/output/$(CONFIG)/$(TARGET)/lib
EXTRA_PATH := $(TOP)/output/$(CONFIG)/$(TARGET)/lib/extra

INC += -I. -I../inc -I../../inc -I../../../inc -I../../../../inc -I$(BSP_ROOT)/inc -I$(TOP)/inc

include module.mk

ifneq ($(LIB_SRC),)
LIB_OBJ := $(addprefix $(OBJ_PATH)/,$(LIB_SRC:.c=.o))
endif

ifneq ($(API_SRC),)
API_OBJ := $(addprefix $(OBJ_PATH)/,$(API_SRC:.c=.o))
endif

DEP := $(LIB_OBJ:.o=.d) $(API_OBJ:.o=.d)
include $(FW_ROOT)/m_targets.mk

all: lib

bin:

lib: create_dirs $(EXTRA_PATH)/$(LIB_NAME) $(EXTRA_PATH)/$(API_NAME)

$(EXTRA_PATH)/$(LIB_NAME): $(LIB_OBJ)
ifneq ($(LIB_OBJ),)
	$(call create_lib,$@,$(LIB_OBJ))
endif

$(EXTRA_PATH)/$(API_NAME): $(API_OBJ)
ifneq ($(API_OBJ),)
	$(call create_lib,$@,$(API_OBJ))
endif

create_dirs: $(LIB_PATH) $(EXTRA_PATH) $(OBJ_PATH)

$(LIB_PATH):
	$(MKDIR) $(call path_subst,$(LIB_PATH))

$(EXTRA_PATH):
	$(MKDIR) $(call path_subst,$(EXTRA_PATH))

$(OBJ_PATH):
	$(MKDIR) $(call path_subst,$(OBJ_PATH))

clean:
	$(RM) *~
	$(RM) $(call path_subst,$(EXTRA_PATH)/$(LIB_NAME))
	$(RM) $(call path_subst,$(EXTRA_PATH)/$(API_NAME))
	$(RM) $(call path_subst,$(LIB_OBJ))
	$(RM) $(call path_subst,$(API_OBJ))
