HCRP_LIB_SRC += csr_bt_hcrp_handler.c \
	csr_bt_hcrp_sef.c\
	csr_bt_hcrp_free_down.c

HCRP_API_SRC += csr_bt_hcrp_lib.c\
	csr_bt_hcrp_free_up.c

LIB_SRC += $(HCRP_LIB_SRC)
API_SRC += $(HCRP_API_SRC)

PM_LIB_OBJ += $(addprefix hcrp/code/,$(HCRP_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix hcrp/code/,$(HCRP_API_SRC:.c=.o))
