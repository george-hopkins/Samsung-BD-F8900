HF_LIB_SRC+=csr_bt_hf_main.c \
csr_bt_hf_call_sef.c \
csr_bt_hf_connect_sef.c \
csr_bt_hf_dspm.c \
csr_bt_hf_util.c \
csr_bt_hf_at_inter.c \
csr_bt_hf_handler.c \
csr_bt_hf_main_sef.c \
csr_bt_hfhs_data_sef.c \
csr_bt_hf_free_down.c

HF_API_SRC+=csr_bt_hf_lib.c \
	csr_bt_hf_free_up.c

LIB_SRC += $(HF_LIB_SRC)
API_SRC += $(HF_API_SRC)

PM_LIB_OBJ += $(addprefix handsfree/hf/code/,$(HF_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix handsfree/hf/code/,$(HF_API_SRC:.c=.o))
