#-*-Makefile-*-
##############################################################################
#
#       Copyright Cambridge Silicon Radio Limited and its licensors 2011.
#       All rights reserved.
#
# REVISION:      $Revision: #3 $
##############################################################################

TOP := ../..

MODULE_NAME := ampwifi

OBJ_PATH = $(TOP)/output/$(CONFIG)/$(TARGET)/obj/applications/$(MODULE_NAME)
include $(TOP)/local_rules.mk

CFLAGS += $(EXTRA_CFLAGS) $(APP_CFLAGS)

SRC :=  csr_bt_ampwifi.c

ifeq ($(CSR_AMP_ENABLE),1)
ifeq ($(CSR_BOARD),CSR_BOARD_M2107_A05)
CFLAGS += -DCSR_WIFI_DESIGN_M2107_R02
else
ifeq ($(CSR_BOARD),CSR_BOARD_M2107_B07)
CFLAGS += -DCSR_WIFI_DESIGN_M2107_R03
else
ifeq ($(CSR_BOARD),CSR_BOARD_M2399_A10)
CFLAGS += -DCSR_WIFI_DESIGN_M2399_R03
else
ERROR_MESSAGE := CSR_BOARD shall be set to either CSR_BOARD_M2107_A05, CSR_BOARD_M2107_B07 or CSR_BOARD_M2399_A10 (e.g. CSR_BOARD=CSR_BOARD_M2107_A05)
$(error $(ERROR_MESSAGE))
endif
endif
endif
endif

include $(TOP)/applications/generic/makefile.common.app
