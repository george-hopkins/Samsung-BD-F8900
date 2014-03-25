#-*-Makefile-*-
TOP := ../../../../..
OBJ_PATH = $(TOP)/output/$(CONFIG)/$(TARGET)/obj/src/cs/HCI/hci/blue_stack/sdp

include $(TOP)/local_rules.mk

INC += -I. -I$(BSP_ROOT)/inc -I$(TOP)/inc \
	-I$(TOP)/profile_managers/inc \
	-I$(TOP)/profile_managers/core_stack/inc \
	-I$(TOP)/profile_managers/core_stack/hci/inc \
	-I$(TOP)/profile_managers/core_stack/hci/blue_stack/inc \
	-I$(TOP)/profile_managers/core_stack/hci/blue_stack/dm \
	-I$(TOP)/profile_managers/core_stack/hci/blue_stack/l2cap

include module.mk

OBJ := $(addprefix $(OBJ_PATH)/,$(call path_subst,$(SRC:.c=.o)))

CFLAGS += $(HCI_CFLAGS)

DEP := $(OBJ:.o=.d)
include $(FW_ROOT)/m_targets.mk

all: lib

bin:

lib: $(OBJ)

clean:
	$(RM) -r $(OBJ_PATH)
