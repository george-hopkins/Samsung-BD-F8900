FTS_LIB_SRC+=csr_bt_fts_handler.c \
	csr_bt_fts_sef.c\
	csr_bt_fts_free_down.c

FTS_API_SRC+= csr_bt_fts_lib.c\
	csr_bt_fts_free_up.c

LIB_SRC += $(FTS_LIB_SRC)
API_SRC += $(FTS_API_SRC)

PM_LIB_OBJ += $(addprefix obex/ftp/fts/code/,$(FTS_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix obex/ftp/fts/code/,$(FTS_API_SRC:.c=.o))
