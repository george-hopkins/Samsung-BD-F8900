AVRCP_LIB_SRC+=csr_bt_avrcp_main.c \
csr_bt_avrcp_cmn_app_handlers.c \
csr_bt_avrcp_cmn_cm_handlers.c \
csr_bt_avrcp_cmn_utils.c \
csr_bt_avrcp_cmn_sdp.c \
csr_bt_avrcp_cmn_data.c \
csr_bt_avrcp_tg_app_handlers.c \
csr_bt_avrcp_tg_msg_handlers.c \
csr_bt_avrcp_tg_util.c \
csr_bt_avrcp_ct_app_handlers.c \
csr_bt_avrcp_ct_msg_handlers.c \
csr_bt_avrcp_ct_util.c \
csr_bt_avrcp_free_down.c

AVRCP_API_SRC+=csr_bt_avrcp_lib.c \
csr_bt_avrcp_free_up.c

LIB_SRC += $(AVRCP_LIB_SRC)
API_SRC += $(AVRCP_API_SRC)

PM_LIB_OBJ += $(addprefix avrcp/code/,$(AVRCP_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix avrcp/code/,$(AVRCP_API_SRC:.c=.o))

