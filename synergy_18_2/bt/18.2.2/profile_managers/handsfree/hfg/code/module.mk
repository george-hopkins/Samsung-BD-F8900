HFG_LIB_SRC += \
csr_bt_hfg_at_gen.c \
csr_bt_hfg_at_handler.c \
csr_bt_hfg_at_inter.c \
csr_bt_hfg_con.c \
csr_bt_hfg_con_audio.c \
csr_bt_hfg_con_down.c \
csr_bt_hfg_con_sd.c \
csr_bt_hfg_con_up.c \
csr_bt_hfg_top.c \
csr_bt_hfg_top_con.c \
csr_bt_hfg_top_down.c \
csr_bt_hfg_top_record.c \
csr_bt_hfg_top_up.c \
csr_bt_hfg_util_instance.c \
csr_bt_hfg_util_misc.c \
csr_bt_hfg_util_msg.c \
csr_bt_hfg_dspm.c \
csr_bt_hfg_free_down.c

HFG_API_SRC += \
csr_bt_hfg_lib.c\
csr_bt_hfg_free_up.c

LIB_SRC += $(HFG_LIB_SRC)
API_SRC += $(HFG_API_SRC)

PM_LIB_OBJ += $(addprefix handsfree/hfg/code/,$(HFG_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix handsfree/hfg/code/,$(HFG_API_SRC:.c=.o))
