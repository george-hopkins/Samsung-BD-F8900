PAN_LIB_SRC+=csr_bt_bsl_main.c \
csr_bt_bsl_sef.c \
csr_bt_bsl_util.c\
csr_bt_bsl_free_down.c

PAN_API_SRC += csr_bt_bsl_lib.c\
csr_bt_bsl_free_up.c

LIB_SRC += $(PAN_LIB_SRC)
API_SRC += $(PAN_API_SRC)

PM_LIB_OBJ += $(addprefix pan/code/,$(PAN_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix pan/code/,$(PAN_API_SRC:.c=.o))
