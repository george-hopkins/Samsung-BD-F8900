#-*-Makefile-*-
###############################################################################
#
#                (c) Cambridge Silicon Radio Limited 2009
#
#                All rights reserved and confidential information of CSR
#
# REVISION:      $Revision: #1 $
###############################################################################

TOP := ../..

APP_NAME = ss_demo_test

APP_PREREQ = 

APP_INC = -I ../ss_inc


APP_LDPATH =

APP_CFLAGS = -DCSR_BT_RUN_STACK=1 -DCSR_BT_RUN_APP=1 -D__CSR_COUNT_WD_

APP_SRC = ss_demo_test.c

OBJ_PATH = $(TOP)/output/$(CONFIG)/$(TARGET)/obj/applications/$(APP_NAME)/$(BUILD_TYPE)
include $(TOP)/local_rules.mk



.PHONY: bin
all: bin
.PHONY: all
bin: APP_TARGETS ?= dobin
bin:
bin:
	$(MAKE) $(APP_TARGETS) ACTION=dobin

.PHONY: clean
clean: APP_TARGETS ?= doclean
clean:
	$(MAKE) $(APP_TARGETS) ACTION=doclean
#
# Common flags, paths, libraries
#
INC += $(APP_INC) \
	-I . \
	-I $(TOP)/applications/inc \
	-I $(TOP)/applications/common \
	-I $(TOP)/applications/bootstrap \
	-I $(TOP)/inc \
	-I $(TOP)/profile_managers/inc \
	-I $(BSP_ROOT)/inc \
	-I $(BSP_ROOT)/platform/inc 

ifneq ($(filter $(TARGET),bdb2-nucleus-arm5tej),)
INC += -I $(TOP)/applications/common/ports/bdb2
else
ifeq ($(SUB_TARGET_ARCH),Linux)
INC += -I $(TOP)/applications/common/ports/pclin
else
INC += -I $(TOP)/applications/common/ports/pcwin
endif
endif

ifeq ($(TARGET),pclin-2.6-x86)
APP_CFLAGS += `pkg-config --cflags alsa` -DALSA_PLAY
endif

CFLAGS += $(APP_CFLAGS)


# Framework libraries
LIBS += $(call use_lib,csr_bt_ss_client_app)\
	$(call use_lib,csr_socket)\
	$(call use_lib,csr_panic) \
	$(call use_lib,csr_util) \
	$(call use_lib,csr_pmem) 
	
# alsa libraries
ifeq ($(TARGET),pclin-2.6-x86)
LIBS += $(call use_lib,asound)
endif



# Add target platform specific libraries
LIBS += $(call use_lib,$(TARGET_LIBS))

LDPATH += $(APP_LDPATH) \
	$(call create_ldpath,$(TOP)/output/$(CONFIG)/$(TARGET)/lib) \
	$(call create_ldpath,$(TOP)/output/$(CONFIG)/$(TARGET)/lib/extra) \
	$(call create_ldpath,$(TOP)/output/$(CONFIG)/$(TARGET)/lib/app) \
	$(call create_ldpath,$(BSP_ROOT)/output/$(CONFIG)/$(TARGET)/lib) \
	$(call create_ldpath,$(LOW_LEVEL_ROOT)/output/$(CONFIG)/$(TARGET)/lib)

#
# The linking order for GCC matters, and as this is impossible
# to get right we apply this simple trick
#

build_app_libs:
	$(MAKE) -C $(TOP)/applications/ss_client lib

ifeq ($(SUB_TARGET_ARCH),Linux)
LIBS := $(LIBS) $(LIBS)
endif

#
# Target variables
#
OBJ = $(addprefix $(OBJ_PATH)/,$(APP_SRC:.c=.o))
DEP =  $(OBJ:.o=.d)

ifneq ($(filter $(TARGET),bdb2-nucleus-arm5tej),)
SCATTER_FILE := $(TOP)/applications/common/ports/bdb2/bdb2.sct
LIST_FILE := ./bdb2.lst
DEFAULT_LIBS += csr_low_level_drivers
AXF = $(call axf_name,$(APP_NAME)_app)
BIN = $(call bin_name,$(APP_NAME)_app)
else
BIN = $(call bin_name,$(APP_NAME)_app)
endif

#
# The actual targets
#
include $(FW_ROOT)/m_targets.mk

.PHONY: doprint
doprint:
	@$(ECHO) "* Building $(BIN) ..."

ifneq ($(filter $(CFLAGS),$(APP_PREREQ)),)
$(info ###############################################################)
$(info Can not build $(APP_NAME) demo application with these options: $(APP_PREREQ))
$(info ###############################################################)
$(BIN):
else
$(BIN): build_app_libs $(OBJ) 
ifneq ($(filter $(TARGET),bdb2-nucleus-arm5tej),)
	$(call create_axf,$(AXF),$(OBJ),$(LDPATH),$(LIBS) $(call use_lib,$(DEFAULT_LIBS)))
	$(call create_bin,$(BIN),$(AXF))
else
    
	$(call create_bin,$(BIN),$(OBJ),$(LDPATH),$(LIBS) $(call use_lib,$(DEFAULT_LIBS)))
endif
endif

.PHONY: bin
dobin: $(BIN)

.PHONY: clean
doclean:
	$(RM) *~
	$(RM) $(BIN)
