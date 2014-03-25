MAPC_LIB_SRC+=csr_bt_mapc_handler.c \
	csr_bt_mapc_sef.c\
	csr_bt_mapc_free_down.c \
	csr_bt_mapc_free_hc_impl.c

MAPC_API_SRC +=  csr_bt_mapc_lib.c\
	csr_bt_mapc_free_up.c

LIB_SRC += $(MAPC_LIB_SRC)
API_SRC += $(MAPC_API_SRC)

PM_LIB_OBJ += $(addprefix obex/map/mapc/code/,$(MAPC_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix obex/map/mapc/code/,$(MAPC_API_SRC:.c=.o))
