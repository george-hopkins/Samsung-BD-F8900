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

MODULE_NAME := ss_server

OBJ_PATH = $(TOP)/output/$(CONFIG)/$(TARGET)/obj/applications/$(MODULE_NAME)
#OBJ_PATH = ./
include $(TOP)/local_rules.mk

INC += -I ../ss_app -I../ss_inc

#APP_SRC += csr_ss_av_api.c

SRC :=  AppAVSeverCmdHandler.c\
	AppAVlib.c\
	AppAVRCSeverCmdHandler.c\
	AppAVRClib.c\
	AppGapSeverCmdHandler.c\
	AppGaplib.c\
	AppHidhSeverCmdHandler.c\
	AppHidhlib.c

include $(TOP)/applications/ss_app/makefile.common.app

