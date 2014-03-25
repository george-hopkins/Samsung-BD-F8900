FTC_LIB_SRC+=csr_bt_ftc_handler.c \
	csr_bt_ftc_sef.c\
	csr_bt_ftc_free_down.c

FTC_API_SRC +=  csr_bt_ftc_lib.c\
	csr_bt_ftc_free_up.c

LIB_SRC += $(FTC_LIB_SRC)
API_SRC += $(FTC_API_SRC)

PM_LIB_OBJ += $(addprefix obex/ftp/ftc/code/,$(FTC_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix obex/ftp/ftc/code/,$(FTC_API_SRC:.c=.o))
