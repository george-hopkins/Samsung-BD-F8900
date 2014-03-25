SD_LIB_SRC += csr_bt_sd_main.c \
	csr_bt_sd_util.c \
	csr_bt_sd_search.c \
	csr_bt_sd_sdp.c \
	csr_bt_sd_cmsc_handlers.c \
	csr_bt_sd_app_handlers.c \
	csr_bt_sd_private_lib.c \
	csr_bt_sd_private_free_down.c \
	csr_bt_sd_private_free_up.c \
	csr_bt_sd_free_down.c \
	csr_bt_sd_free_hc_impl.c \
	csr_bt_sd_free_handcoded.c

# LE support begin
ifeq ($(CSR_BT_LE_ENABLE),1)
SD_LIB_SRC += csr_bt_sd_gatt.c
endif

SD_API_SRC += csr_bt_sd_lib.c \
	csr_bt_sd_free_up.c

LIB_SRC += $(SD_LIB_SRC)
API_SRC += $(SD_API_SRC)

PM_LIB_OBJ += $(addprefix sd/code/,$(SD_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix sd/code/,$(SD_API_SRC:.c=.o))

