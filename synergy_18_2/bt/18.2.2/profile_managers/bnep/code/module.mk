BNEP_LIB_SRC += \
    csr_bt_bnep.c \
    sdputil.c

BNEP_API_SRC += \
    csr_bt_bnep_lib.c

LIB_SRC += $(BNEP_LIB_SRC)
API_SRC += $(BNEP_API_SRC)

PM_LIB_OBJ += $(addprefix bnep/code/,$(BNEP_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix bnep/code/,$(BNEP_API_SRC:.c=.o))
