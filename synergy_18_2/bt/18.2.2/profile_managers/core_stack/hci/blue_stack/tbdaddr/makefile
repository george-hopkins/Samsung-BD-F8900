#-*-Makefile-*-

TOP := ../../../../..
OBJ_PATH = $(TOP)/output/$(CONFIG)/$(TARGET)/obj/src/cs/HCI/hci/blue_stack/tbdaddr
include $(TOP)/local_rules.mk

INC += -I. -I$(BSP_ROOT)/inc -I$(TOP)/inc \
	-I$(TOP)/profile_managers/inc \
	-I$(TOP)/profile_managers/core_stack/inc \
	-I$(TOP)/profile_managers/core_stack/hci/inc \
	-I$(TOP)/profile_managers/core_stack/hci/blue_stack/inc

include module.mk

OBJ := $(addprefix $(OBJ_PATH)/,$(call path_subst,$(SRC:.c=.o)))

CFLAGS += $(HCI_CFLAGS)

DEP := $(OBJ:.o=.d)
include $(FW_ROOT)/m_targets.mk

all: lib 

bin:

lib: $(OBJ)

clean:
	$(RM) *~
	$(RM) $(call path_subst,$(OBJ))
