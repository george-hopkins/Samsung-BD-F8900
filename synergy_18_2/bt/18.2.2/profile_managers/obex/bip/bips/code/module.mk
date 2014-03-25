BIPS_LIB_SRC+=csr_bt_bips_handler.c \
	 csr_bt_bips_sef.c\
	csr_bt_bips_free_down.c

BIPS_API_SRC+= csr_bt_bips_lib.c\
	csr_bt_bips_free_up.c

LIB_SRC += $(BIPS_LIB_SRC)
API_SRC += $(BIPS_API_SRC)

PM_LIB_OBJ += $(addprefix obex/bip/bips/code/,$(BIPS_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix obex/bip/bips/code/,$(BIPS_API_SRC:.c=.o))

