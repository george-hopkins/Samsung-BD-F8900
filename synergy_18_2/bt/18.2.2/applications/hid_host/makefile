#-*-Makefile-*-
###############################################################################
#
#                (c) Cambridge Silicon Radio Limited 2009
#
#                All rights reserved and confidential information of CSR
#
# REVISION:      $Revision: #1 $
###############################################################################

TOP := ../..

APP_NAME = hidh

APP_PREREQ = -DEXCLUDE_CSR_BT_L2CA_MODULE -DEXCLUDE_BCCMD_MODULE -DEXCLUDE_CSR_BT_HIDH_MODULE

APP_INC = 

APP_LIBS = $(call use_lib,csr_bt_hidparser)

APP_LDPATH =

APP_CFLAGS = -DCSR_BT_RUN_STACK=1 -DCSR_BT_RUN_APP=1

APP_SRC = app_files/csr_bt_tasks.c \
	app_files/csr_bt_hidh_demo_app.c \
	app_files/csr_bt_nvs_hid.c

OBJ_PATH = $(TOP)/output/$(CONFIG)/$(TARGET)/obj/applications/$(APP_NAME)/$(BUILD_TYPE)
include $(TOP)/local_rules.mk

# HID does not work with RFC so force disable (after local rules)
RFC_IN_RELEASE := 0

include $(TOP)/applications/common/makefile.common
