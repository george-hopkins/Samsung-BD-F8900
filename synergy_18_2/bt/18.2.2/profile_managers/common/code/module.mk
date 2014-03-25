COMMON_LIB_SRC+=\
	csr_bt_util.c \
	csr_bt_util_packer.c \
	csr_bt_mblk_helpers.c \
	csr_bt_cmn_sdp_lib.c \
	csr_bt_cmn_sdr_tagbased_lib.c \
	csr_bt_cmn_linked_list.c \
	csr_bt_cmn_sdc_rfc_util.c \
	csr_bt_sdc_support.c \
	csr_bt_core_stack_fsm16_log.c \
	csr_bt_core_stack_fsm32_log.c \
	fsm16.c \
	fsm32.c

LIB_SRC += $(COMMON_LIB_SRC)
API_SRC += $(COMMON_API_SRC)

PM_LIB_OBJ += $(addprefix common/code/,$(COMMON_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix common/code/,$(COMMON_API_SRC:.c=.o))

