#-*-Makefile-*-
###############################################################################
#
#                (c) Cambridge Silicon Radio Limited 2009
#
#                All rights reserved and confidential information of CSR
#
# REVISION:      $Revision: #2 $
###############################################################################
TOP := ../..

MODULE_NAME := spp

APP_PREREQ = -DEXCLUDE_CSR_BT_SPP_MODULE

OBJ_PATH = $(TOP)/output/$(CONFIG)/$(TARGET)/obj/applications/$(MODULE_NAME)
include $(TOP)/local_rules.mk

SRC :=  csr_bt_spp_app_ui_sef.c \
	csr_bt_spp_app.c 

include $(TOP)/applications/generic/makefile.common.app
