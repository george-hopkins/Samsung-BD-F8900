MDER_LIB_SRC += \
    csr_bt_phdc_common.c \
    csr_bt_phdc_ieee_decode_msg.c \
    csr_bt_phdc_ieee_encode_msg.c 
           
#MDER_API_SRC +=

LIB_SRC += $(MDER_LIB_SRC)
API_SRC += $(MDER_API_SRC)

PM_LIB_OBJ += $(addprefix phdc/mder/code/,$(MDER_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix phdc/mder/code/,$(MDER_API_SRC:.c=.o))
