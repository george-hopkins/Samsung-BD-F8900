###############################################################################
#
#                (c) Cambridge Silicon Radio Limited 2010
#
#                All rights reserved and confidential information of CSR
#
###############################################################################
FW_ROOT := ../../../../../../..
OBJ_PATH = $(BSP_ROOT)/output/$(CONFIG)/$(TARGET)/obj/src/kernel/lowlevel/usb
include $(FW_ROOT)/local_rules.mk

MODULE_NAME := bt_usb
APP_NAME := $(call bin_name,$(MODULE_NAME))

LIBS := $(call use_lib,csr_usb_com) \
	$(call use_lib,csr_pmem) \
	$(call use_lib,csr_panic) \
	$(call use_lib,csr_util)

LDPATH := $(call create_ldpath,$(BSP_LIB))

LIB_NAME := $(call lib_name,csr_usb_com)

LIB_PATH := $(BSP_LIB)

INC += $(BSP_INC)/kernel

LIB_SRC += csr_usb_com.c \
	csr_usb_intf.c \
	csr_usb_queue.c \
	csr_usb_dfu.c

LIB_OBJ := $(addprefix $(OBJ_PATH)/,$(LIB_SRC:.c=.o))

DEP := $(LIB_OBJ:.o=.d)

APP_SRC := csr_usb_main.c
APP_OBJ = $(addprefix $(OBJ_PATH)/,$(APP_SRC:.c=.o))

include $(FW_ROOT)/m_targets.mk

CFLAGS+= -DUSB_NO_SCHEDULER

all: lib 

bin: 

$(APP_NAME): $(APP_OBJ)
	$(call create_bin,$(APP_NAME),$(APP_OBJ),$(LDPATH),$(LIBS))


lib: $(LIB_PATH)/$(LIB_NAME) $(APP_NAME)

$(LIB_PATH)/$(LIB_NAME): $(LIB_OBJ)
	$(MKDIR) $(@D)
	$(call create_lib,$@,$(LIB_OBJ))

clean:
	$(RM) *~
	$(RM) $(call path_subst,$(LIB_PATH)/$(LIB_NAME))
	$(RM) $(call path_subst,$(LIB_OBJ))
	$(RM) $(APP_NAME)
	$(RM) $(APP_OBJ)
	$(RM) m_$(MODULE_NAME).ko.o
	$(RM) m_$(MODULE_NAME).ko.mod.o
	$(RM) m_$(MODULE_NAME).ko.mod.c
