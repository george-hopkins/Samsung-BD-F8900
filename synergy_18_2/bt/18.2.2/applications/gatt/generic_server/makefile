#-*-Makefile-*-
###############################################################################
#
#   Copyright Cambridge Silicon Radio Limited and its licensors 2011.
#   
#   All rights reserved. 
#
###############################################################################
TOP := ../../..

MODULE_NAME := generic_server

APP_PREREQ = -DEXCLUDE_CSR_BT_L2CA_MODULE -DEXCLUDE_CSR_BT_GATT_MODULE

OBJ_PATH = $(TOP)/output/$(CONFIG)/$(TARGET)/obj/applications/$(MODULE_NAME)
include $(TOP)/local_rules.mk
ifeq ($(CSR_BT_LE_ENABLE),1)

INC += -I$(TOP)/applications/gatt/
#server
SRC :=  csr_bt_gatt_demo_generic_server_app_ui_sef.c \
	    csr_bt_gatt_demo_generic_server_app.c \
        csr_bt_gatt_demo_generic_srv_lib.c \
        csr_bt_gatt_demo_generic_srv_db.c
endif #CSR_BT_LE_ENABLE=1

include $(TOP)/applications/generic/makefile.common.app
