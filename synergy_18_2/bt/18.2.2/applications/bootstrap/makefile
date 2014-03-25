#-*-Makefile-*-
##############################################################################
#
#       Copyright Cambridge Silicon Radio Limited and its licensors 2011.
#       All rights reserved.
#
# REVISION:      $Revision: #3 $
##############################################################################

TOP := ../..
OBJ_PATH = $(TOP)/output/$(CONFIG)/$(TARGET)/obj/bootstrap/$(BUILD_TYPE)
include $(TOP)/local_rules.mk

.PHONY: all bin lib clean

LIB_NAME := $(call lib_name,csr_bt_bootstrap)
LIB_PATH := $(TOP)/output/$(CONFIG)/$(TARGET)/lib

INC += -I. -I../inc -I$(BSP_ROOT)/inc -I$(TOP)/inc \
	-I $(TOP)/applications/bootstrap \
	-I $(BSP_ROOT)/inc/platform

PBC_FILES := $(shell $(FIND) psr/ -name "pb-*.c") 
PSC_FILES := $(shell $(FIND) psr/ -name "ps-*.c") 


# Different bootstrap methods: file based or
# compiled-in constant string
#	csr_bt_bootstrap_specific_file_based.c
#	csr_bt_bootstrap_specific_string_based.c
ifeq  ($(CSR_SAMSUNG_SERVICE),1)
APP_SRC += 	csr_bt_bootstrap_specific_samsung_platform.c \
	psr/pb-101_7558_samsung.c \
	psr/pb-90_6817_Samsung.c
else
APP_SRC += 	csr_bt_bootstrap_specific_string_based.c \
	$(PBC_FILES) \
	$(PSC_FILES)
endif

SRC := csr_bt_bootstrap_generic.c \
	$(APP_SRC)
ifeq ($(CSR_AMP_ENABLE),1)
ifeq ($(CSR_BOARD),CSR_BOARD_M2107_A05)
CFLAGS += -DCSR_WIFI_DESIGN_M2107_R02
else
ifeq ($(CSR_BOARD),CSR_BOARD_M2107_B07)
CFLAGS += -DCSR_WIFI_DESIGN_M2107_R03
else
ifeq ($(CSR_BOARD),CSR_BOARD_M2399_A10)
CFLAGS += -DCSR_WIFI_DESIGN_M2399_R03
else
ERROR_MESSAGE := CSR_BOARD shall be set to either CSR_BOARD_M2107_A05, CSR_BOARD_M2107_B07 or CSR_BOARD_M2399_A10 (e.g. CSR_BOARD=CSR_BOARD_M2107_A05)
$(error $(ERROR_MESSAGE))
endif
endif
endif
endif


OBJ := $(addprefix $(OBJ_PATH)/,$(call path_subst,$(SRC:.c=.o)))

DEP := $(OBJ:.o=.d)
include $(FW_ROOT)/m_targets.mk

all: lib

lib: create_dirs $(LIB_PATH)/$(LIB_NAME) 
$(LIB_PATH)/$(LIB_NAME): $(OBJ)
	$(call create_lib,$@,$(OBJ))

create_dirs: $(LIB_PATH)

$(LIB_PATH):
	$(MKDIR) $(call path_subst,$(LIB_PATH))

clean:
	$(RM) *~
	$(RM) $(call path_subst,$(LIB_PATH)/$(LIB_NAME))
	$(RM) $(call path_subst,$(OBJ))
