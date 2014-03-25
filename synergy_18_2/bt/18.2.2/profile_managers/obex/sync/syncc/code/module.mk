SYNCC_LIB_SRC+=csr_bt_syncc_handler.c \
	csr_bt_syncc_sef.c\
	csr_bt_syncc_free_down.c

SYNCC_API_SRC +=  csr_bt_syncc_lib.c\
	csr_bt_syncc_free_up.c

LIB_SRC += $(SYNCC_LIB_SRC)
API_SRC += $(SYNCC_API_SRC)

PM_LIB_OBJ += $(addprefix obex/sync/syncc/code/,$(SYNCC_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix obex/sync/syncc/code/,$(SYNCC_API_SRC:.c=.o))
