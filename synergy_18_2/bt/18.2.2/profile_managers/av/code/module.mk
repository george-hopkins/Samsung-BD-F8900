###############################################################################
#
#       Copyright Cambridge Silicon Radio Limited and its licensors 2012. 
#
#                All rights reserved.
#
###############################################################################
AV_LIB_SRC+=csr_bt_av_con.c \
	csr_bt_av_signal.c \
	csr_bt_av_main.c \
	csr_bt_av_register.c \
	csr_bt_av_util.c \
	csr_bt_av_fragmentation.c \
	csr_bt_av_free_down.c

AV_API_SRC += csr_bt_av_lib.c \
	csr_bt_av_free_up.c

LIB_SRC += $(AV_LIB_SRC)
API_SRC += $(AV_API_SRC)

PM_LIB_OBJ += $(addprefix av/code/,$(AV_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix av/code/,$(AV_API_SRC:.c=.o))


