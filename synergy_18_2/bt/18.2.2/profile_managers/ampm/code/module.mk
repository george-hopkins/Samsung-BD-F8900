AUTOFSM := \
	csr_bt_ampm_btdev_genfsm.c \
	csr_bt_ampm_l2cap_genfsm.c \
	csr_bt_ampm_local_genfsm.c \
	csr_bt_ampm_logical_genfsm.c \
	csr_bt_ampm_phy_genfsm.c

AMPM_LIB_SRC += \
	csr_bt_ampm_btdev_genfsm.c \
	csr_bt_ampm_l2cap_genfsm.c \
	csr_bt_ampm_local_genfsm.c \
	csr_bt_ampm_logical_genfsm.c \
	csr_bt_ampm_phy_genfsm.c \
	csr_bt_ampm_main.c \
	csr_bt_ampm_app.c \
	csr_bt_ampm_cmapp.c \
	csr_bt_ampm_prf.c \
	csr_bt_ampm_data.c \
	csr_bt_ampm_dm_interface.c \
	csr_bt_ampm_gen_list.c \
	csr_bt_ampm_hci_interface.c \
	csr_bt_ampm_hmac_sha256.c \
	csr_bt_ampm_interface.c \
	csr_bt_ampm_l2cap_interface.c \
	csr_bt_ampm_sdu.c \
	csr_bt_ampm_l2cap.c \
	csr_bt_ampm_local.c \
	csr_bt_ampm_btdev.c \
	csr_bt_ampm_logical.c \
	csr_bt_ampm_phy.c \
	csr_bt_ampm_send_hci_command.c \
	csr_bt_ampm_send_upstream.c

AMPM_API_SRC := csr_bt_ampm_free_down.c \
	csr_bt_ampm_lib.c \
	csr_bt_ampm_free_up.c

LIB_SRC += $(AMPM_LIB_SRC)
API_SRC += $(AMPM_API_SRC)

PM_LIB_OBJ += $(addprefix ampm/code/,$(AMPM_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix ampm/code/,$(AMPM_API_SRC:.c=.o))
