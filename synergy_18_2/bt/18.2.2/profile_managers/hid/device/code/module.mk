HIDD_LIB_SRC += csr_bt_hidd_main.c \
	csr_bt_hidd_sef.c \
	csr_bt_hidd_local_lib.c\
	csr_bt_hidd_free_down.c

HIDD_API_SRC += csr_bt_hidd_lib.c\
	csr_bt_hidd_free_up.c

LIB_SRC += $(HIDD_LIB_SRC)
API_SRC += $(HIDD_API_SRC)

PM_LIB_OBJ += $(addprefix hid/device/code/,$(HIDD_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix hid/device/code/,$(HIDD_API_SRC:.c=.o))
