SPP_LIB_SRC+=csr_bt_spp_main.c \
	csr_bt_spp_sef.c \
	csr_bt_spp_am.c \
	csr_bt_spp_free_down.c \
	csr_bt_spp_free_hc_impl.c

SPP_API_SRC+=csr_bt_spp_lib.c\
	 csr_bt_spp_free_up.c

LIB_SRC += $(SPP_LIB_SRC)
API_SRC += $(SPP_API_SRC)

PM_LIB_OBJ += $(addprefix spp/code/,$(SPP_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix spp/code/,$(SPP_API_SRC:.c=.o))
