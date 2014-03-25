#-*-Makefile-*-
###############################################################################
#
#                (c) Cambridge Silicon Radio Limited 2009
#
#                All rights reserved and confidential information of CSR
#
# REVISION:      $Revision: #1 $
###############################################################################
LIB_NAME := $(call lib_name,csr_bt_$(MODULE_NAME)_app)
LIB_PATH := $(TOP)/output/$(CONFIG)/$(TARGET)/lib/app

INC += -I. -I../inc -I../../inc \
	-I$(TOP)/applications/inc \
	-I$(TOP)/applications/common \
	-I$(BSP_ROOT)/inc -I$(TOP)/inc \
	-I$(FW_ROOT)/inc/application/app/ \
	-I$(FW_ROOT)/inc/application/ui

OBJ = $(addprefix $(OBJ_PATH)/,$(call path_subst,$(SRC:.c=.o)))

DEP := $(OBJ:.o=.d)
include $(FW_ROOT)/m_targets.mk

# AMP
# Generic AMP support
ifeq ($(CSR_AMP_ENABLE),1)
CFLAGS += -DCSR_AMP_ENABLE
endif


# WiFi AMP PAL
ifeq ($(CSR_BT_APP_AMP_WIFI),1)
CFLAGS += -DCSR_BT_APP_AMP_WIFI
INC += -I $(WIFI_ROOT)/inc \
	-I $(WIFI_ROOT)/src/router_remote/code \
	-I $(WIFI_ROOT)/porting/extra/mib/inc \
	-I $(WIFI_ROOT)/porting/ports/common/inc \
	-I $(WIFI_ROOT)/example_apps/generic/inc
endif


all: lib

bin:

ifneq ($(filter $(CFLAGS),$(APP_PREREQ)),)
$(info ###############################################################)
$(info Can not build $(MODULE_NAME) with these options: $(APP_PREREQ))
$(info ###############################################################)
lib:
else
lib: create_dirs $(LIB_PATH)/$(LIB_NAME)
endif

$(LIB_PATH)/$(LIB_NAME): $(OBJ)
	$(call create_lib,$@,$(OBJ))

create_dirs: $(LIB_PATH) $(OBJ_PATH)

$(LIB_PATH):
	$(MKDIR) $(call path_subst,$(LIB_PATH))

$(OBJ_PATH):
	$(MKDIR) $(call path_subst,$(OBJ_PATH))

clean:
	$(RM) *~
	$(RM) $(call path_subst,$(LIB_PATH)/$(LIB_NAME))
	$(RM) $(call path_subst,$(OBJ))

