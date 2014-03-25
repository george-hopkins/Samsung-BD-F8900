BPPS_LIB_SRC += csr_bt_bpps_handler.c \
	csr_bt_bpps_sef.c \
	csr_bt_bpps_soapbuilder.c \
	csr_bt_bpps_free_down.c


BPPS_API_SRC += csr_bt_bpps_lib.c\
	csr_bt_bpps_free_up.c

LIB_SRC += $(BPPS_LIB_SRC)
API_SRC += $(BPPS_API_SRC)

PM_LIB_OBJ += $(addprefix obex/bpp/bpps/code/,$(BPPS_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix obex/bpp/bpps/code/,$(BPPS_API_SRC:.c=.o))
