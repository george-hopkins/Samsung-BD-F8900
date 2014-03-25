OPC_LIB_SRC+=csr_bt_opc_handler.c \
	csr_bt_opc_sef.c \
	csr_bt_opc_free_down.c

OPC_API_SRC += csr_bt_opc_lib.c\
	csr_bt_opc_free_up.c

LIB_SRC += $(OPC_LIB_SRC)
API_SRC += $(OPC_API_SRC)

PM_LIB_OBJ += $(addprefix obex/opp/opc/code/,$(OPC_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix obex/opp/opc/code/,$(OPC_API_SRC:.c=.o))
