#-*-Makefile-*-
TOP := ../../../..
OBJ_PATH = $(TOP)/output/$(CONFIG)/$(TARGET)/obj/src/cs/HCI/hci/blue_stack
include $(TOP)/local_rules.mk

INC += -I. -I$(BSP_ROOT)/inc -I$(TOP)/inc \
	-I$(TOP)/profile_managers/inc \
	-I$(TOP)/profile_managers/core_stack/inc \
	-I$(TOP)/profile_managers/core_stack/hci/inc \
	-I$(TOP)/profile_managers/core_stack/hci/blue_stack/inc \
	-I$(TOP)/profile_managers/core_stack/hci/blue_stack/dm \
	-I$(TOP)/profile_managers/core_stack/hci/blue_stack/sm \
	-I$(TOP)/profile_managers/core_stack/hci/blue_stack/l2cap \
	-I$(TOP)/profile_managers/core_stack/hci/blue_stack/att 

CFLAGS += $(HCI_CFLAGS)

SUB_DIRS := \
	att \
	dm \
	l2cap \
	rfcomm \
	sdp \
	sm \
	tbdaddr

FSM_SUB_DIRS := \
	dm \
	l2cap \
	rfcomm \
	sdp

target_to_dir  = $(patsubst _genfsm_%,%,$(patsubst _clean_%,%,$(patsubst _build_%,%,$1)))

clean_sub_dirs := $(addprefix _clean_,$(SUB_DIRS))
build_sub_dirs := $(addprefix _build_,$(SUB_DIRS))
genfsm_sub_dirs:= $(addprefix _genfsm_,$(FSM_SUB_DIRS))

.PHONY: lib bin $(build_sub_dirs) $(clean_sub_dirs)

DEP := $(OBJ:.o=.d)
include $(FW_ROOT)/m_targets.mk

$(genfsm_sub_dirs):
	$(MAKE) -C $(call target_to_dir,$@) genfsm

$(build_sub_dirs):
	$(MAKE) -C $(call target_to_dir,$@) lib

$(clean_sub_dirs):
	$(MAKE) -C $(call target_to_dir,$@) clean

all: lib

bin:

genfsm: $(genfsm_sub_dirs)

lib: $(build_sub_dirs)

clean: $(clean_sub_dirs)
	$(RM) *~
