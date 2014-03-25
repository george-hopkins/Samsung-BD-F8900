HIDH_LIB_SRC += csr_bt_hidh_con.c \
	csr_bt_hidh_data.c \
	csr_bt_hidh_main.c \
	csr_bt_hidh_sdc.c \
	csr_bt_hidh_sigup.c \
	csr_bt_hidh_util.c \
	csr_bt_hidh_free_down.c

HIDH_API_SRC += csr_bt_hidh_lib.c\
	csr_bt_hidh_free_up.c

LIB_SRC += $(HIDH_LIB_SRC)
API_SRC += $(HIDH_API_SRC)

PM_LIB_OBJ += $(addprefix hid/host/code/,$(HIDH_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix hid/host/code/,$(HIDH_API_SRC:.c=.o))
