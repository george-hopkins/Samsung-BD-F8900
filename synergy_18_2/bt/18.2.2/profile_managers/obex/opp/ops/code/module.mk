OPS_LIB_SRC+=csr_bt_ops_handler.c \
	csr_bt_ops_sef.c \
	csr_bt_ops_free_down.c

OPS_API_SRC+=csr_bt_ops_lib.c\
	csr_bt_ops_free_up.c

LIB_SRC += $(OPS_LIB_SRC)
API_SRC += $(OPS_API_SRC)

PM_LIB_OBJ += $(addprefix obex/opp/ops/code/,$(OPS_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix obex/opp/ops/code/,$(OPS_API_SRC:.c=.o))
