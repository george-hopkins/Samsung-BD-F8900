SC_LIB_SRC+=csr_bt_sc_bond_unified.c \
    csr_bt_sc_bond_legacy.c \
    csr_bt_sc_utils.c \
    csr_bt_sc_null.c \
    csr_bt_sc_bond_local.c \
    csr_bt_sc_bond_remote.c \
    csr_bt_sc_debond.c \
    csr_bt_sc_bondable_mode.c \
    csr_bt_sc_main.c \
    csr_bt_sc_register.c \
    csr_bt_sc_authenticate.c \
    csr_bt_sc_encrypt.c \
    csr_bt_sc_lib_upstream.c \
    csr_bt_sc_authorise.c \
    csr_bt_sc_ui.c \
    csr_bt_sc_rebond.c \
    csr_bt_sc_oob.c \
    csr_bt_sc_private_free_down.c \
    csr_bt_sc_private_free_up.c

#ifeq ($(CSR_BT_LE_ENABLE),1)
SC_LIB_SRC += csr_bt_sc_le.c
#endif

SC_API_SRC+=csr_bt_sc_lib.c \
	csr_bt_sc_free_up.c \
	csr_bt_sc_free_down.c

LIB_SRC += $(SC_LIB_SRC)
API_SRC += $(SC_API_SRC)

PM_LIB_OBJ += $(addprefix sc/code/,$(SC_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix sc/code/,$(SC_API_SRC:.c=.o))
