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

APP_NAME = pan

APP_PREREQ = -DEXCLUDE_CSR_BT_L2CA_MODULE -DEXCLUDE_CSR_BT_BNEP_MODULE -DEXCLUDE_CSR_BT_BSL_MODULE

APP_INC = -I $(TOP)/applications/pan/app_files

APP_LIBS =

APP_LDPATH =

APP_CFLAGS = -DCSR_BT_RUN_STACK=1 -DCSR_BT_RUN_APP=1 -DCSR_BT_RUN_IP=1

include $(TOP)/applications/internet/module.mk
APP_SRC = app_files/csr_bt_pan_demo_app.c \
	app_files/csr_bt_tasks.c \
	$(INET_SRC)

OBJ_PATH = $(TOP)/output/$(CONFIG)/$(TARGET)/obj/applications/$(APP_NAME)/$(BUILD_TYPE)/1
include $(TOP)/local_rules.mk

include $(TOP)/applications/common/makefile.common
