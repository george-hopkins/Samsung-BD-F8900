BIPC_LIB_SRC+=csr_bt_bipc_handler.c \
	 csr_bt_bipc_sef.c\
	csr_bt_bipc_free_down.c

BIPC_API_SRC+= csr_bt_bipc_lib.c\
	csr_bt_bipc_free_up.c

LIB_SRC += $(BIPC_LIB_SRC)
API_SRC += $(BIPC_API_SRC)

PM_LIB_OBJ += $(addprefix obex/bip/bipc/code/,$(BIPC_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix obex/bip/bipc/code/,$(BIPC_API_SRC:.c=.o))
