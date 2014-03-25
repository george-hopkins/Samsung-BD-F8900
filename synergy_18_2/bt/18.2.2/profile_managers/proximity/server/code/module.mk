PROX_SRV_LIB_SRC+=csr_bt_prox_srv_handler.c \
	 csr_bt_prox_srv_db.c \
     csr_bt_prox_srv_free_down.c
PROX_SRV_API_SRC+=csr_bt_prox_srv_lib.c \
     csr_bt_prox_srv_free_up.c

LIB_SRC += $(PROX_SRV_LIB_SRC)
API_SRC += $(PROX_SRV_API_SRC)

PM_LIB_OBJ += $(addprefix proximity/server/code/,$(PROX_SRV_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix proximity/server/code/,$(PROX_SRV_API_SRC:.c=.o))
