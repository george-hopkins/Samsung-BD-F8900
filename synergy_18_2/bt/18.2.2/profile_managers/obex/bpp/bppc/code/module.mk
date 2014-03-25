BPPC_LIB_SRC += csr_bt_bppc_handler.c \
	csr_bt_bppc_sef.c \
	csr_bt_bppc_soapbuilder.c\
	csr_bt_bppc_free_down.c


BPPC_API_SRC += csr_bt_bppc_lib.c\
	csr_bt_bppc_free_up.c

LIB_SRC += $(BPPC_LIB_SRC)
API_SRC += $(BPPC_API_SRC)

PM_LIB_OBJ += $(addprefix obex/bpp/bppc/code/,$(BPPC_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix obex/bpp/bppc/code/,$(BPPC_API_SRC:.c=.o))
