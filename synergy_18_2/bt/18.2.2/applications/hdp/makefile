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

APP_NAME = hdp

APP_PREREQ = -DEXCLUDE_CSR_BT_L2CA_MODULE -DEXCLUDE_CSR_BT_HDP_MODULE

APP_INC = 

APP_LIBS =

APP_LDPATH =

APP_CFLAGS = -DCSR_BT_RUN_STACK=1 -DCSR_BT_RUN_APP=1

APP_SRC = app_files/csr_bt_tasks.c \
	app_files/csr_bt_hdp_demo_app.c  \
	app_files/csr_bt_hdp_demo_util.c

OBJ_PATH = $(TOP)/output/$(CONFIG)/$(TARGET)/obj/applications/$(APP_NAME)/$(BUILD_TYPE)
include $(TOP)/local_rules.mk
include $(TOP)/applications/common/makefile.common
