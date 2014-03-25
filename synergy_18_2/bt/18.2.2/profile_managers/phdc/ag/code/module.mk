AUTOFSM := \
	csr_bt_phdc_ag_genfsm.c \
	csr_bt_phdc_ag_device_genfsm.c

PHDC_AG_LIB_SRC += \
	csr_bt_phdc_ag_genfsm.c \
	csr_bt_phdc_ag_device_genfsm.c \
	csr_bt_phdc_ag_common.c \
	csr_bt_phdc_ag_fsm.c \
	csr_bt_phdc_ag_main.c   \
	csr_bt_phdc_ag_out.c \
	csr_bt_phdc_ag_free_down.c

PHDC_AG_API_SRC += \
	csr_bt_phdc_ag_lib.c \
	csr_bt_phdc_ag_free_up.c

LIB_SRC += $(PHDC_AG_LIB_SRC)
API_SRC += $(PHDC_AG_API_SRC)

PM_LIB_OBJ += $(addprefix phdc/ag/code/,$(PHDC_AG_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix phdc/ag/code/,$(PHDC_AG_API_SRC:.c=.o))
