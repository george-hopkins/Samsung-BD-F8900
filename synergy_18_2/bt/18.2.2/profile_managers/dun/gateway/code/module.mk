DG_LIB_SRC+=csr_bt_dg_main.c \
	csr_bt_dg_sef.c \
	csr_bt_dg_free_down.c

DG_API_SRC+=csr_bt_dg_lib.c\
	csr_bt_dg_free_up.c

LIB_SRC += $(DG_LIB_SRC)
API_SRC += $(DG_API_SRC)

PM_LIB_OBJ += $(addprefix dun/gateway/code/,$(DG_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix dun/gateway/code/,$(DG_API_SRC:.c=.o))
