#-*-Makefile-*-
TOP := ../../..
OBJ_PATH = $(TOP)/output/$(CONFIG)/$(TARGET)/obj/src/cs/HCI/hci
include $(TOP)/local_rules.mk

CFLAGS += $(HCI_CFLAGS)

SUB_DIRS := blue_stack \
	converter \
	hci_shim

target_to_dir  = $(patsubst _clean_%,%,$(patsubst _build_%,%,$1))

clean_sub_dirs := $(addprefix _clean_,$(SUB_DIRS))
build_sub_dirs := $(addprefix _build_,$(SUB_DIRS))

.PHONY: lib bin $(build_sub_dirs) $(clean_sub_dirs)

DEP := $(OBJ:.o=.d)
include $(FW_ROOT)/m_targets.mk

$(build_sub_dirs):
	$(MAKE) -C $(call target_to_dir,$@) lib

$(clean_sub_dirs):
	$(MAKE) -C $(call target_to_dir,$@) clean

all: lib

bin:

lib: $(build_sub_dirs)

clean: $(clean_sub_dirs)
	$(RM) *~
