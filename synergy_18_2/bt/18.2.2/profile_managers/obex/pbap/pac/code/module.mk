PAC_LIB_SRC+=csr_bt_pac_handler.c \
	csr_bt_pac_sef.c \
	csr_bt_pac_free_down.c

PAC_API_SRC += csr_bt_pac_lib.c\
	csr_bt_pac_free_up.c

LIB_SRC += $(PAC_LIB_SRC)
API_SRC += $(PAC_API_SRC)

PM_LIB_OBJ += $(addprefix obex/pbap/pac/code/,$(PAC_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix obex/pbap/pac/code/,$(PAC_API_SRC:.c=.o))
