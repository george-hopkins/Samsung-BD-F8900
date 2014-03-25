SYNCS_LIB_SRC+=csr_bt_syncs_handler.c \
	csr_bt_syncs_sef.c\
	csr_bt_syncs_free_down.c

SYNCS_API_SRC+=csr_bt_syncs_lib.c\
	csr_bt_syncs_free_up.c

LIB_SRC += $(SYNCS_LIB_SRC)
API_SRC += $(SYNCS_API_SRC)

PM_LIB_OBJ += $(addprefix obex/sync/syncs/code/,$(SYNCS_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix obex/sync/syncs/code/,$(SYNCS_API_SRC:.c=.o))

