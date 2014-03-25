SMLS_LIB_SRC+=csr_bt_smls_handler.c \
	csr_bt_smls_sef.c\
	csr_bt_smls_free_down.c

SMLS_API_SRC+= csr_bt_smls_lib.c\
	csr_bt_smls_free_up.c

LIB_SRC += $(SMLS_LIB_SRC)
API_SRC += $(SMLS_API_SRC)

PM_LIB_OBJ += $(addprefix obex/syncml/smls/code/,$(SMLS_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix obex/syncml/smls/code/,$(SMLS_API_SRC:.c=.o))
