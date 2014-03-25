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

MODULE_NAME := avrcp

OBJ_PATH = $(TOP)/output/$(CONFIG)/$(TARGET)/obj/applications/$(MODULE_NAME)
include $(TOP)/local_rules.mk

CFLAGS += $(EXTRA_CFLAGS) $(APP_CFLAGS)

#\
#          -DCSR_BT_RUN_STACK=1 -DCSR_BT_RUN_APP=1 


SRC :=  csr_bt_avrcp_main.c \
        csr_bt_avrcp_utils.c \
        csr_bt_av2_avrcp.c \
        csr_bt_avrcp_down.c \
        csr_bt_avrcp_app_ui_sef.c \
        csr_bt_avrcp_app_util.c \
    $(APP_SRC)

include $(TOP)/applications/generic/makefile.common.app
