HDP_LIB_SRC += csr_bt_hdp_handler.c \
               csr_bt_hdp_sef.c \
               csr_bt_hdp_util.c \
               csr_bt_hdp_prim_out.c \
               csr_bt_hdp_free_down.c

HDP_API_SRC += csr_bt_hdp_lib.c \
               csr_bt_hdp_free_up.c

LIB_SRC += $(HDP_LIB_SRC)
API_SRC += $(HDP_API_SRC)

PM_LIB_OBJ += $(addprefix hdp/code/,$(HDP_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix hdp/code/,$(HDP_API_SRC:.c=.o))
