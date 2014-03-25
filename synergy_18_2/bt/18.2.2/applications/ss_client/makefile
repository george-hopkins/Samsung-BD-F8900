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

MODULE_NAME := ss_client

OBJ_PATH = $(TOP)/output/$(CONFIG)/$(TARGET)/obj/applications/$(MODULE_NAME)
#OBJ_PATH = ./
include $(TOP)/local_rules.mk

INC += -I ../ss_inc
#APP_CFLAGS += -DUSE_MP3 -DUSE_FILEDUMP
#APP_SRC += csr_ss_av_api.c

SRC :=  AppAVHandler.c\
	AppAVRCHandler.c\
	AppGapHandler.c\
	AppHidhHandler.c\
	AppHandler.c\
	AppSSUtil.c\
	csr_samsung_log.c

include $(TOP)/applications/ss_app/makefile.common.app
