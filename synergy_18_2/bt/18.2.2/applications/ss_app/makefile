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

APP_NAME = samsung

APP_PREREQ =

APP_INC = -I ../ss_inc



APP_LDPATH =

APP_CFLAGS += -DCSR_BT_RUN_STACK=1 -DCSR_BT_RUN_APP=1 

# Set SBCFILE as the default flavour
ifeq ($(CSR_BT_APP_AV_FILTER),)
ifeq ($(CSR_DSPM_ENABLE),1)
	CSR_BT_APP_AV_FILTER=avrouter
else
	CSR_BT_APP_AV_FILTER=sbcaudio
endif
endif


# Build SBCAUDIO flavour
APP_LIBS += $(call use_lib,csr_sbc)
APP_CFLAGS += -DUSE_SBC
APP_SRC += csr_bt_filter_sbc.c

#ifeq ($(TARGET),pclin-2.6-x86)
#APP_SRC   += csr_bt_filter_alsa.c
#APP_CFLAGS += -DUSE_ALSA `pkg-config --cflags alsa`
#LIBS += $(call use_lib,asound)
#endif

ifeq ($(SS_CODEC_SUPPORT),1)
APP_SRC += csr_bt_filter_ss.c 
endif


APP_SRC += 		csr_bt_app_main.c\
	csr_bt_app_main_transport_bluecore.c\
    csr_bt_init_tasks.c\
    csr_bt_declare_tasks.c\
	csr_bt_tasks.c \
	csr_bt_samsung_app.c \
	csr_bt_av2_filter.c \
	csr_bt_filter_av2.c \
	Bluetooth_CM.c \
	Bluetooth_SC.c \
	Bluetooth_SD.c \
	Bluetooth_AV.c \
	Bluetooth_AVRC.c \
	Bluetooth_HIDH.c 


BUILD_TYPE ?= hci

#APP_LIBS += $(call use_lib,csr_bt_msg_converter)
#APP_LIBS += $(call use_lib,csr_msg_converter)

OBJ_PATH = $(TOP)/output/$(CONFIG)/$(TARGET)/obj/applications/$(APP_NAME)/$(BUILD_TYPE)
include $(TOP)/local_rules.mk

# HID does not work with RFC so force disable (after local rules)
RFC_IN_RELEASE := 0

include $(TOP)/applications/common/makefile.init
# Only enforce dependencies if instructed
ifneq ($(NO_DEPLIBS),1)
include $(TOP)/applications/common/makefile.deps
endif
include $(TOP)/applications/common/makefile.amp

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

CFLAGS += $(APP_CFLAGS)

ifneq ($(filter $(BSP_LIBS),csr_usb_com),)
CFLAGS += -DCSR_BT_APP_USE_USB
endif

ifneq ($(filter $(BSP_LIBS),csr_ser_com),)
CFLAGS += -DCSR_BT_APP_USE_SERIAL_COM
endif


ifeq ($(FW_MAJOR_VERSION),3)
LIBS += $(call use_lib,csr_random)
endif




#LIBS +=	$(call use_lib,csr_ip)
#LIBS +=	$(call use_lib,csr_ip_socket_lib)
#LIBS +=	$(call use_lib,csr_ip_ether_lib)
#LIBS +=	$(call use_lib,csr_ip_ifconfig_lib)
LIBS += $(call use_lib,csr_socket)
LIBS += $(call use_lib,csr_bt_ss_server_app)
LIBS +=	$(call use_lib,csr_bt_sc_db)
LIBS +=	$(call use_lib,csr_bt_hidparser)

# Bluetooth libraries
LIBS += \
        $(call use_lib,csr_bt_corestack_$(BUILD_TYPE)) \
        $(call use_lib,csr_bt) \
        $(call use_lib,csr_bt_lib) \
        $(call use_lib,csr_bt_bootstrap)


# Framework libraries
LIBS += \
        $(APP_LIBS) \
        $(call use_lib,csr_hci) \
        $(call use_lib,csr_bccmd) \
        $(call use_lib,csr_bcsp) \
        $(call use_lib,csr_core_msg_converter) \
        $(call use_lib,csr_formatted_io) \
        $(call use_lib,csr_h4ds) \
        $(call use_lib,csr_hq) \
        $(call use_lib,csr_list) \
        $(call use_lib,csr_mblk) \
        $(call use_lib,csr_msg_converter) \
        $(call use_lib,csr_queue_lib) \
        $(call use_lib,csr_message_queue) \
        $(call use_lib,csr_tm_bluecore) \
        $(call use_lib,csr_unicode) \
        $(call use_lib,csr_usb) \
        $(call use_lib,csr_vm) \
        $(call use_lib,csr_fastpipe) \
        $(call use_lib,csr_arg_search) \
        $(call use_lib,csr_type_a) \
        $(call use_lib,csr_type_a_async) \
        $(call use_lib,csr_aclbuf_lower) \
        $(call use_lib,csr_sched) \
        $(call use_lib,csr_random) \
        $(call use_lib,csr_main) \
        $(call use_lib,$(BSP_LIBS))        

ifeq ($(CSR_IBFP_H4I_SUPPORT),1)
LIBS += $(call use_lib,csr_h4i) \
        $(call use_lib,csr_h4common)
endif
        

ifeq ($(LOG),1)
LIBS += \
        $(call use_lib,csr_log) \
        $(call use_lib,csr_log_btsnoop) \
        $(call use_lib,csr_log_fts) \
        $(call use_lib,csr_log_pcap) \
        $(call use_lib,csr_log_cleartext) \
        $(call use_lib,csr_bt_msg_converter)
endif



LDPATH += \
        $(APP_LDPATH) \
        $(call create_ldpath,$(TOP)/output/$(CONFIG)/$(TARGET)/lib) \
        $(call create_ldpath,$(TOP)/output/$(CONFIG)/$(TARGET)/lib/extra) \
        $(call create_ldpath,$(TOP)/output/$(CONFIG)/$(TARGET)/lib/app) \
        $(call create_ldpath,$(FW_LIB)) \
        $(call create_ldpath,$(BSP_LIB)) \
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
AXF = $(call axf_name,csr_bt_app_$(BUILD_TYPE))
BIN = $(call bin_name,csr_bt_app_$(BUILD_TYPE))
else
BIN = $(call bin_name,csr_bt_app_$(BUILD_TYPE))
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
