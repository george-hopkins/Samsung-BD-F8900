MCAP_LIB_SRC += csr_bt_mcap_handler.c \
	csr_bt_mcap_sef.c \
	csr_bt_mcap_private_free_up.c \
	csr_bt_mcap_private_free_down.c \
	csr_bt_mcap_util.c 

MCAP_API_SRC += csr_bt_mcap_lib.c 

LIB_SRC += $(MCAP_LIB_SRC)
API_SRC += $(MCAP_API_SRC)

PM_LIB_OBJ += $(addprefix mcap/code/,$(MCAP_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix mcap/code/,$(MCAP_API_SRC:.c=.o))
