#-*-Makefile-*-
###############################################################################
#
#                (c) Cambridge Silicon Radio Limited 2009
#
#                All rights reserved and confidential information of CSR
#
# REVISION:      $Revision: #2 $
###############################################################################

#
# This makefile builds the common application libraries. It is invoked
# automatically from makefile.deps, if the library has not been built
#

TOP := ../..
OBJ_PATH = $(TOP)/output/$(CONFIG)/$(TARGET)/obj/applications/common/$(BUILD_TYPE)
include $(TOP)/local_rules.mk

.PHONY: all bin lib clean

LIB_NAME := $(call lib_name,csr_bt_app_common_$(BUILD_TYPE))
LIB_PATH := $(TOP)/output/$(CONFIG)/$(TARGET)/lib/app

INC += \
        -I. -I../inc -I$(BSP_ROOT)/inc -I$(TOP)/inc \
        -I $(TOP)/applications/bootstrap \
        -I $(BSP_ROOT)/inc/platform
CFLAGS += \
        -DCSR_BT_RUN_STACK=1 \
        -DCSR_BT_TOPDIR=\"$(TOP)\"

# WiFi AMP PAL
ifeq ($(CSR_BT_APP_AMP_WIFI),1)
INC += \
        -I $(WIFI_ROOT)/inc \
        -I $(WIFI_ROOT)/src/inc \
        -I $(WIFI_ROOT)/src/router_remote/code \
        -I $(WIFI_ROOT)/porting/extra/mib/inc \
        -I $(WIFI_ROOT)/porting/ports/common/inc \
        -I $(WIFI_ROOT)/example_apps/generic/inc
endif

SRC := \
        csr_bt_cmdline.c \
        csr_bt_transport.c \
        csr_bt_di_lib.c \
        csr_bt_cod_decode.c \
        csr_bt_sc_demo_app.c \
        csr_bt_sc_demo_app_dopairing.c \
        csr_bt_sc_demo_app_reject.c \
        csr_bt_file.c \
        csr_bt_declare_tasks.c \
        csr_bt_init_tasks.c

ifeq ($(LOG),1)
SRC += csr_bt_log.c \
       csr_bt_converter.c
endif

ifneq ($(filter $(TARGET_PLATFORM),bdb2 bdb3),)
SRC += \
        ./ports/bdb/csr_bt_app_main.c \
        ./ports/bdb/csr_bt_platform.c
endif

ifeq ($(TARGET_PLATFORM),pcwin)
SRC += \
        ./ports/pcwin/csr_bt_app_main.c \
        ./ports/pcwin/csr_bt_platform.c
endif

ifeq ($(TARGET_PLATFORM),pclin)
SRC += \
        ./ports/pclin/csr_bt_app_main.c \
        ./ports/pclin/csr_bt_platform.c
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
	$(RM) $(call path_subst,$(LIB_PATH)/$(LIB_NAME))
	$(RM) $(call path_subst,$(OBJ))
