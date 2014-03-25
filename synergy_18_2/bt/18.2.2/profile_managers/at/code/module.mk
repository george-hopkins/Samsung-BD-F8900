AT_LIB_SRC+= csr_bt_at_and_handler.c \
    csr_bt_at_parent.c \
    csr_bt_at_helpfunc.c \
    csr_bt_at_result_res_handler.c \
    csr_bt_at_handler.c \
    csr_bt_at_shandler.c \
    csr_bt_at_indhandler.c \
    csr_bt_at_sqres_handler.c \
    csr_bt_at_msg_factory.c \
    csr_bt_at_main.c \
    csr_bt_at_prefik.c \
    csr_bt_at_tokenhandler.c \
    csr_bt_at_free_down.c

AT_API_SRC += csr_bt_at_lib.c \
		csr_bt_at_free_up.c

LIB_SRC += $(AT_LIB_SRC)
API_SRC += $(AT_API_SRC)

PM_LIB_OBJ += $(addprefix at/code/,$(AT_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix at/code/,$(AT_API_SRC:.c=.o))
