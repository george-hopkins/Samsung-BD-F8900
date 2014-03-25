SMLC_LIB_SRC+=csr_bt_smlc_handler.c \
	csr_bt_smlc_sef.c\
	csr_bt_smlc_free_down.c

SMLC_API_SRC+= csr_bt_smlc_lib.c\
	csr_bt_smlc_free_up.c

LIB_SRC += $(SMLC_LIB_SRC)
API_SRC += $(SMLC_API_SRC)

PM_LIB_OBJ += $(addprefix obex/syncml/smlc/code/,$(SMLC_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix obex/syncml/smlc/code/,$(SMLC_API_SRC:.c=.o))
