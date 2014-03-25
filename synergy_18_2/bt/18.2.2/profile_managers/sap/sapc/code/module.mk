SAPC_LIB_SRC+=csr_bt_sapc_main.c \
csr_bt_sapc_sef.c \
csr_bt_sapc_free_down.c

SAPC_API_SRC += csr_bt_sapc_lib.c\
csr_bt_sapc_free_up.c

LIB_SRC += $(SAPC_LIB_SRC)
API_SRC += $(SAPC_API_SRC)

PM_LIB_OBJ += $(addprefix sap/sapc/code/,$(SAPC_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix sap/sapc/code/,$(SAPC_API_SRC:.c=.o))
