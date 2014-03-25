SAPS_LIB_SRC+=csr_bt_saps_main.c \
csr_bt_saps_sef.c \
csr_bt_saps_free_down.c

SAPS_API_SRC+= csr_bt_saps_lib.c\
csr_bt_saps_free_up.c

LIB_SRC += $(SAPS_LIB_SRC)
API_SRC += $(SAPS_API_SRC)

PM_LIB_OBJ += $(addprefix sap/saps/code/,$(SAPS_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix sap/saps/code/,$(SAPS_API_SRC:.c=.o))
