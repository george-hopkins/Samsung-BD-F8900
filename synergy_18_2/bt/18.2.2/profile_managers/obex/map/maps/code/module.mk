MAPS_LIB_SRC+=csr_bt_maps_handler.c \
	csr_bt_maps_sef.c\
	csr_bt_maps_free_down.c \
	csr_bt_maps_free_hc_impl.c

MAPS_API_SRC +=  csr_bt_maps_lib.c\
	csr_bt_maps_free_up.c

LIB_SRC += $(MAPS_LIB_SRC)
API_SRC += $(MAPS_API_SRC)

PM_LIB_OBJ += $(addprefix obex/map/maps/code/,$(MAPS_LIB_SRC:.c=.o))
PM_API_OBJ += $(addprefix obex/map/maps/code/,$(MAPS_API_SRC:.c=.o))
