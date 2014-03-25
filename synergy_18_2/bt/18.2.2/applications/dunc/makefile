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

APP_NAME = dunc

APP_PREREQ = -DEXCLUDE_CSR_BT_RFC_MODULE -DEXCLUDE_CSR_BT_DUNC_MODULE -DEXCLUDE_CSR_BT_PPP_MODULE

APP_INC = -I $(TOP)/applications/dunc/app_files

APP_LIBS =

APP_LDPATH =

APP_CFLAGS =  -DEXCLUDE_ARP_MODULE \
	-DCSR_BT_RUN_STACK=1 -DCSR_BT_RUN_APP=1 \
	-DCSR_BT_RUN_IP=1 -DCSR_BT_RUN_PPP=1

include $(TOP)/applications/internet/module.mk
include $(TOP)/applications/internet/ppp/module.mk
APP_SRC := app_files/csr_bt_at_dte.c \
	app_files/csr_bt_dunc_ctrl_app.c \
	app_files/csr_bt_dunc_data_app.c \
	app_files/csr_bt_tasks.c \
	$(INET_SRC) \
	$(PPP_SRC)

OBJ_PATH = $(TOP)/output/$(CONFIG)/$(TARGET)/obj/applications/$(APP_NAME)/$(BUILD_TYPE)/1
include $(TOP)/local_rules.mk
include $(TOP)/applications/common/makefile.common
