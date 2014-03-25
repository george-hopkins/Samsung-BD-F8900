###############################################################################
#
#                (c) Cambridge Silicon Radio Limited 2009
#
#                All rights reserved and confidential information of CSR
#
# REVISION:      $Revision: #1 $
###############################################################################
TOP := ../..
OBJ_PATH = $(TOP)/output/$(CONFIG)/$(TARGET)/obj/src/cs
include $(TOP)/local_rules.mk

LIB_PATH := $(TOP)/output/$(CONFIG)/$(TARGET)/lib

ifeq ($(HCI_IN_RELEASE),1)
LIB_NAME_HCI := $(call lib_name,csr_bt_corestack_hci)
endif

SRC_CMN := $(call path_subst,$(shell $(FIND) common -name *.c))

ifeq ($(HCI_IN_RELEASE),1)
SRC_HCI := $(call path_subst,$(shell $(FIND) hci -name *.c))
endif

ifeq ($(HCI_IN_RELEASE),1)
OBJ_CMN_HCI := $(addprefix $(OBJ_PATH)/HCI/,$(SRC_CMN:.c=.o))
OBJ_HCI     := $(addprefix $(OBJ_PATH)/HCI/,$(SRC_HCI:.c=.o))
LOCAL_OBJ_CMN_HCI := $(addprefix tmp/,$(notdir $(SRC_CMN:.c=.o)))
LOCAL_OBJ_HCI     := $(addprefix tmp/,$(notdir $(SRC_HCI:.c=.o)))

endif

# Fake target (used for satisfying deps)
BUILD_HCI := $(OBJ_PATH)/BUILD_HCI.o
LIB_HCI   := $(OBJ_PATH)/LIB_HCI.o

all: lib

lib: create_dirs $(LIB_HCI)

$(BUILD_HCI): $(SRC_CMN) $(SRC_HCI)
ifeq ($(HCI_IN_RELEASE),1)
	$(MAKE) -C common all HOSTBUILD=HCI
	$(MAKE) -C hci all
	touch $@
endif

bin:

$(LIB_HCI): $(BUILD_HCI)
ifeq ($(HCI_IN_RELEASE),1)
	$(MAKE) $(LIB_PATH)/$(LIB_NAME_HCI)
	touch $@
endif

$(LIB_PATH)/$(LIB_NAME_HCI): $(OBJ_HCI) $(OBJ_CMN_HCI)
ifeq ($(HCI_IN_RELEASE),1)
	$(MKDIR) tmp
	$(CP) $(OBJ_HCI) tmp
	$(CP) $(OBJ_CMN_HCI) tmp

	$(call create_lib,$@,$(LOCAL_OBJ_HCI) $(LOCAL_OBJ_CMN_HCI))

	$(RM) -rf tmp
endif

create_dirs: $(LIB_PATH)

$(LIB_PATH):
	$(MKDIR) $(call path_subst,$(LIB_PATH))

clean:
	$(RM) *~
	$(RM) $(call path_subst,$(LIB_PATH)/$(LIB_NAME_HCI))
	$(RM) $(call path_subst,$(BUILD_HCI))
	$(RM) $(call path_subst,$(LIB_HCI))
	$(MAKE) -C common $@ HOSTBUILD=HCI
	$(MAKE) -C hci $@
