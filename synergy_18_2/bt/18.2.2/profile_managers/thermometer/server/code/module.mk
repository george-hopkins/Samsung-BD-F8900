##############################################################################
#
#   Copyright Cambridge Silicon Radio Limited and its licensors 2011.
#   
#   All rights reserved. 
#
# REVISION:      $Revision: #6 $
##############################################################################
THERM_SRV_LIB_SRC+=csr_bt_therm_srv_handler.c \
	 csr_bt_therm_srv_db.c \
     csr_bt_therm_srv_free_down.c
THERM_SRV_API_SRC+=csr_bt_therm_srv_lib.c \
     csr_bt_therm_srv_free_up.c

LIB_SRC += $(THERM_SRV_LIB_SRC)
API_SRC += $(THERM_SRV_API_SRC)

PM_LIB_OBJ += $(addprefix thermometer/server/code/,$(THERM_SRV_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix thermometer/server/code/,$(THERM_SRV_API_SRC:.c=.o))
