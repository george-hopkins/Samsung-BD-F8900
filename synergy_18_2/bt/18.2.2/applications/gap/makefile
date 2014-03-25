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

MODULE_NAME := gap

OBJ_PATH = $(TOP)/output/$(CONFIG)/$(TARGET)/obj/applications/$(MODULE_NAME)
include $(TOP)/local_rules.mk

SRC :=  csr_bt_gap_app_cm.c \
	csr_bt_gap_app_cm_event_handler.c \
	csr_bt_gap_app_handler.c \
	csr_bt_gap_app_lib.c \
	csr_bt_gap_app_sc.c \
	csr_bt_gap_app_sc_event_handler.c \
	csr_bt_gap_app_sd.c \
	csr_bt_gap_app_sd_event_handler.c \
	csr_bt_gap_app_ui_sef.c \
	csr_bt_gap_app_util.c 

include $(TOP)/applications/generic/makefile.common.app
