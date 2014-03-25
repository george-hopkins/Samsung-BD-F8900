JSR82_LIB_SRC+=csr_bt_jsr82_main.c \
	csr_bt_jsr82_discovery.c \
	csr_bt_jsr82_local_device.c \
	csr_bt_jsr82_maintenance_handler.c \
	csr_bt_jsr82_service_record.c \
	csr_bt_jsr82_remote_device.c \
	csr_bt_jsr82_l2cap.c \
	csr_bt_jsr82_rfcomm.c \
	csr_bt_jsr82_free_down.c

JSR82_API_SRC+=csr_bt_jsr82_lib.c\
	csr_bt_jsr82_free_up.c

LIB_SRC += $(JSR82_LIB_SRC)
API_SRC += $(JSR82_API_SRC)

PM_LIB_OBJ += $(addprefix jsr82/code/,$(JSR82_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix jsr82/code/,$(JSR82_API_SRC:.c=.o))
