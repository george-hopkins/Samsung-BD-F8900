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

APP_NAME = dg

APP_PREREQ = -DEXCLUDE_CSR_BT_RFC_MODULE -DEXCLUDE_CSR_BT_DG_MODULE -DEXCLUDE_CSR_BT_AT_MODULE -DEXCLUDE_CSR_BT_IWU_MODULE

APP_INC = -I ../iwu

APP_LIBS =

APP_LDPATH =

APP_CFLAGS = -DEXTRA_COM_PORT \
	-DCSR_BT_RUN_STACK=1 -DCSR_BT_RUN_APP=1 -DCSR_BT_RUN_MODEM=1

include $(TOP)/applications/iwu/module.mk
APP_SRC = app_files/csr_bt_tasks.c \
	app_files/csr_bt_dun_demo_app.c \
	app_files/csr_bt_iwu_interface.c \
        app_files/csr_bt_iwu_msg_factory.c \
        app_files/csr_bt_iwu_main4pc.c \
	$(IWU_SRC)


OBJ_PATH = $(TOP)/output/$(CONFIG)/$(TARGET)/obj/applications/$(APP_NAME)/$(BUILD_TYPE)
include $(TOP)/local_rules.mk
include $(TOP)/applications/common/makefile.common
