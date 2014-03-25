AUTOFSM := \
	csr_bt_phdc_mgr_genfsm.c \
	csr_bt_phdc_mgr_device_genfsm.c 
	
PHDC_MGR_LIB_SRC += \
	csr_bt_phdc_mgr_device_genfsm.c \
	csr_bt_phdc_mgr_genfsm.c \
	csr_bt_phdc_mgr_fsm.c \
	csr_bt_phdc_mgr_main.c \
	csr_bt_phdc_mgr_out.c \
	csr_bt_phdc_mgr_dev_spec.c \
	csr_bt_phdc_mgr_util.c \
	csr_bt_phdc_mgr_free_down.c

PHDC_MGR_API_SRC += \
    csr_bt_phdc_mgr_lib.c \
    csr_bt_phdc_mgr_free_up.c

LIB_SRC += $(PHDC_MGR_LIB_SRC)
API_SRC += $(PHDC_MGR_API_SRC)

PM_LIB_OBJ += $(addprefix phdc/mgr/code/,$(PHDC_MGR_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix phdc/mgr/code/,$(PHDC_MGR_API_SRC:.c=.o))
