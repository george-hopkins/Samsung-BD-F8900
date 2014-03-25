TPT_LIB_SRC+=csr_bt_tpt_cmd_serialize.c \
	csr_bt_tpt_handler.c \
	csr_bt_tpt_handler_av.c \
	csr_bt_tpt_handler_cm.c \
	csr_bt_tpt_handler_sc.c \
	csr_bt_tpt_handler_spp.c \
	csr_bt_tpt_srv.c

TPT_API_SRC+=

LIB_SRC += $(TPT_LIB_SRC)
API_SRC += $(TPT_API_SRC)

PM_LIB_OBJ += $(addprefix tpt/code/,$(TPT_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix tpt/code/,$(TPT_API_SRC:.c=.o))
