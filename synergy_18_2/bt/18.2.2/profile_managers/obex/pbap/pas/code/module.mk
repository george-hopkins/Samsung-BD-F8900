PAS_LIB_SRC+=csr_bt_pas_handler.c \
	csr_bt_pas_sef.c \
	csr_bt_pas_free_down.c

PAS_API_SRC += csr_bt_pas_lib.c\
	csr_bt_pas_free_up.c

LIB_SRC += $(PAS_LIB_SRC)
API_SRC += $(PAS_API_SRC)

PM_LIB_OBJ += $(addprefix obex/pbap/pas/code/,$(PAS_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix obex/pbap/pas/code/,$(PAS_API_SRC:.c=.o))
