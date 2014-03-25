DC_LIB_SRC += csr_bt_dunc_main.c \
	csr_bt_dunc_sef.c\
	csr_bt_dunc_free_down.c

DC_API_SRC += csr_bt_dunc_lib.c\
	csr_bt_dunc_free_up.c

LIB_SRC += $(DC_LIB_SRC)
API_SRC += $(DC_API_SRC)

PM_LIB_OBJ += $(addprefix dun/client/code/,$(DC_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix dun/client/code/,$(DC_API_SRC:.c=.o))
