#-*-Makefile-*-

###############################################################################
#
#                (c) Cambridge Silicon Radio Limited 2010
#
#                All rights reserved and confidential information of CSR
#
# REVISION:      $Revision: #1 $
###############################################################################
AUTOFSM := \
    csr_bt_gatt_conn_genfsm.c

GATT_LIB_SRC += \
	csr_bt_gatt_conn_genfsm.c \
	csr_bt_gatt_main.c \
	csr_bt_gatt_conn.c \
	csr_bt_gatt_upstream.c \
	csr_bt_gatt_utils.c \
	csr_bt_gatt_sef.c \
	csr_bt_gatt_cm_sef.c \
	csr_bt_gatt_sc_sef.c \
	csr_bt_gatt_att_sef.c \
	csr_bt_gatt_sd_sef.c \
	csr_bt_gatt_free_down.c \
	csr_bt_gatt_free_hc_impl.c \
	csr_bt_gatt_private_free_down.c \
	csr_bt_gatt_private_free_up.c \
	csr_bt_gatt_free_handcoded.c

GATT_API_SRC := \
	csr_bt_gatt_free_up.c \
	csr_bt_gatt_lib.c

LIB_SRC += $(GATT_LIB_SRC)
API_SRC += $(GATT_API_SRC)

PM_LIB_OBJ += $(addprefix gatt/code/,$(GATT_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix gatt/code/,$(GATT_API_SRC:.c=.o))
