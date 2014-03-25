#-*-Makefile-*-
###############################################################################
#
#                (c) Cambridge Silicon Radio Limited 2009
#
#                All rights reserved and confidential information of CSR
#
# REVISION:      $Revision$
###############################################################################

######## Framework root
FW_ROOT ?= /home/matthew2/work/samsung/synergy_18_2/fw/3.1.2
WIFI_ROOT ?= t:/pre-build/synergy/wifi/pre-4.2/main

######## Figure out Framework version
-include $(FW_ROOT)/ver.mk
FW_MAJOR_VERSION := $(VERSION_MAJOR)
FW_MINOR_VERSION := $(VERSION_MINOR)
FW_FIXLEVEL_VERSION := $(VERSION_FIXLEVEL)
FW_VERSION := $(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_FIXLEVEL)

######## Figure out WiFi version
VERSION_MAJOR := 0
VERSION_MINOR := 0
VERSION_FIXLEVEL := 0

-include $(WIFI_ROOT)/ver.mk
WIFI_MAJOR_VERSION := $(VERSION_MAJOR)
WIFI_MINOR_VERSION := $(VERSION_MINOR)
WIFI_FIXLEVEL_VERSION := $(VERSION_FIXLEVEL)
WIFI_VERSION := $(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_FIXLEVEL)

CFLAGS += -DFW_MAJOR_VERSION=$(FW_MAJOR_VERSION)
CFLAGS += -DFW_MINOR_VERSION=$(FW_MINOR_VERSION)
CFLAGS += -DFW_FIXLEVEL_VERSION=$(FW_FIXLEVEL_VERSION)

CFLAGS += -DWIFI_MAJOR_VERSION=$(WIFI_MAJOR_VERSION)
CFLAGS += -DWIFI_MINOR_VERSION=$(WIFI_MINOR_VERSION)
CFLAGS += -DWIFI_FIXLEVEL_VERSION=$(WIFI_FIXLEVEL_VERSION)

FW_GRATER_THAN_3_0_4 :=	$(shell if [ $(FW_MAJOR_VERSION) -gt 3 ] || [ $(FW_MAJOR_VERSION) -eq 3 -a $(FW_MINOR_VERSION) -gt 0 ] || [ $(FW_MAJOR_VERSION) -eq 3 -a $(FW_MINOR_VERSION) -eq 0 -a $(FW_FIXLEVEL_VERSION) -ge 4 ] ; then echo 1; else echo 0; fi)

ifeq ($(FW_GRATER_THAN_3_0_4),1)
EXTRA_AUTOGEN_FLAGS := --guard=1
else
EXTRA_AUTOGEN_FLAGS := 
endif

debug2:
	@$(ECHO) "FW_GRATER_THAN_3_0_4: $(FW_GRATER_THAN_3_0_4)"
	@$(ECHO) "AUTOGEN_FLAGS: $(AUTOGEN_FLAGS)"


######### Set default config
#CONFIG ?= default
CONFIG ?= samsung
CONFIG_ROOT ?= $(TOP)/config

WIFI_CONFIG ?= default

include $(CONFIG_ROOT)/config-$(CONFIG).mk

######## BlueStack on host - Must be set here for the correct header files with the ramaining core-stack configs are included. 
CFLAGS += -DBUILD_FOR_HOST
CFLAGS += -DBUILD_FOR_HOST_OLD_LINK_KEY_API

ifeq ($(INSTALL_IBFP),1)
CFLAGS += -DINSTALL_IBFP

CSR_IBFP_H4I_SUPPORT=1
endif

######## Parse config to rules
ifeq ($(CSR_AMP_ENABLE),1)
CFLAGS += -DCSR_AMP_ENABLE
ifeq ($(CSR_BT_CONFIG_L2CAP_FCS),1)
CFLAGS += -DCSR_BT_CONFIG_L2CAP_FCS
endif
endif

ifeq ($(CSR_BT_LE_ENABLE),1)
CFLAGS += -DCSR_BT_LE_ENABLE 
endif

#Qualification loggin also implies log enable
ifeq ($(QUAL_LOG),1)
CFLAGS += -DCSR_QUAL_LOG_ENABLE \
	-DCSR_LOG_ENABLE \
	-DCSR_LOG_INCLUDE_FILE_NAME_AND_LINE_NUMBER
endif

CSR_BT_BLUE_STACK_DEBUG ?= 1
ifeq ($(CSR_BT_BLUE_STACK_DEBUG),1)
CFLAGS += -DCSR_BT_BLUE_STACK_DEBUG
endif

ifeq ($(LOG),1)
CFLAGS += -DCSR_LOG_ENABLE \
	-DCSR_LOG_INCLUDE_FILE_NAME_AND_LINE_NUMBER
endif

ifeq ($(ENABLE_SHUTDOWN),1)
CFLAGS += -DENABLE_SHUTDOWN
endif

ifeq ($(EXCEPTION_PANIC),1)
CFLAGS += -DEXCEPTION_PANIC
endif

ifneq ($(FTS_VER),)
CFLAGS += -DFTS_VER=$(FTS_VER)
endif

ifneq ($(INCLUDE_EXCEPTION_HANDLER_MODULE),1)
CFLAGS += -DEXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
endif

ifeq ($(INSTALL_INTERNAL_APP_DEPENDENCIES),1)
CFLAGS += -DCSR_BT_INSTALL_INTERNAL_APP_DEPENDENCIES
endif

ifeq ($(INSTALL_INTERNAL_TEST_DEPENDENCIES),1)
CFLAGS += -DCSR_BT_INSTALL_INTERNAL_TEST_DEPENDENCIES
endif

ifeq ($(CSR_TESTER),1)
CFLAGS += -DCLIENT_SERVER \
	-DENABLE_TEST_TIMEOUT \
	-DENABLE_VERDICT_PASS_FLUSH
CFLAGS += \
	-DINSTALL_L2CAP_RAW_SUPPORT \
	-DINSTALL_AES
endif

ifeq ($(SEC_MODE_2),1)
CFLAGS += -DCSR_BT_DEFAULT_SEC_MODE=2 \
	-DCSR_BT_BT_VERSION=BLUETOOTH_VERSION_2P0
endif

ifeq ($(CHIP_MANAGER_TEST_ENABLE),1)
CFLAGS += -DCSR_CHIP_MANAGER_TEST_ENABLE
CHIP_MANAGER_ENABLE=1
endif

ifeq ($(CHIP_MANAGER_QUERY_ENABLE),1)
CFLAGS += -DCSR_CHIP_MANAGER_QUERY_ENABLE
CHIP_MANAGER_ENABLE=1
endif

ifeq ($(CHIP_MANAGER_ENABLE),1)
CFLAGS += -DCSR_CHIP_MANAGER_ENABLE
endif

ifeq ($(USB_WARMREST),1)
CFLAGS += -DCSR_ENABLE_WARM_RESET
endif

#flag for enabling DSPM
ifeq ($(CSR_DSPM_ENABLE),1)
FW_GREATER_THAN_3_1_0 := $(shell if [ $(FW_MAJOR_VERSION) -gt 3 ] || [ $(FW_MAJOR_VERSION) -eq 3 -a $(FW_MINOR_VERSION) -gt 0 ] ; then echo 1; else echo 0; fi)
ifeq ($(FW_GREATER_THAN_3_1_0),1)
	CFLAGS += -DCSR_USE_DSPM
endif
endif

####### Unused flags for RFC builds
RFC_CFLAGS += -DRFC_BUILD \
	-DEXCLUDE_CSR_BT_BNEP_MODULE \
	-DEXCLUDE_CSR_BT_BSL_MODULE \
	-DEXCLUDE_CSR_BT_HIDH_MODULE \
	-DEXCLUDE_CSR_BT_HIDD_MODULE \
	-DEXCLUDE_CSR_BT_HID_PARSER_MODULE

####### Exclude profile manager/application code.
#CFLAGS += -DEXCLUDE_CSR_BT_AT_MODULE \
#	-DEXCLUDE_CSR_BT_AVRCP_MODULE \
#	-DEXCLUDE_CSR_BT_AV_MODULE \
#	-DEXCLUDE_CSR_BT_BIPC_MODULE \
#	-DEXCLUDE_CSR_BT_BIPS_MODULE \
#	-DEXCLUDE_CSR_BT_BPPS_MODULE \
#	-DEXCLUDE_CSR_BT_BPPC_MODULE \
#	-DEXCLUDE_CSR_BT_BSL_MODULE \
#	-DEXCLUDE_CSR_BT_DG_MODULE \
#	-DEXCLUDE_CSR_BT_DUNC_MODULE \
#	-DEXCLUDE_CSR_BT_FTC_MODULE \
#	-DEXCLUDE_CSR_BT_FTS_MODULE \
#	-DEXCLUDE_CSR_BT_HCRP_MODULE \
#	-DEXCLUDE_CSR_BT_HFG_MODULE \
#	-DEXCLUDE_CSR_BT_HF_MODULE \
#	-DEXCLUDE_CSR_BT_HIDD_MODULE \
#	-DEXCLUDE_CSR_BT_HIDH_MODULE \
#	-DEXCLUDE_CSR_BT_IWU_MODULE \
#	-DEXCLUDE_CSR_BT_JSR82_MODULE \
#	-DEXCLUDE_CSR_BT_OPC_MODULE \
#	-DEXCLUDE_CSR_BT_OPS_MODULE \
#	-DEXCLUDE_CSR_BT_PAC_MODULE \
#	-DEXCLUDE_CSR_BT_PAS_MODULE \
#	-DEXCLUDE_CSR_BT_SAPC_MODULE \
#	-DEXCLUDE_CSR_BT_SAPS_MODULE \
#	-DEXCLUDE_CSR_BT_SMLC_MODULE \
#	-DEXCLUDE_CSR_BT_SMLS_MODULE \
#	-DEXCLUDE_CSR_BT_SPP_MODULE \
#	-DEXCLUDE_CSR_BT_SYNCS_MODULE \
#	-DEXCLUDE_CSR_BT_SYNCC_MODULE \
#	-DEXCLUDE_CSR_BT_MAPC_MODULE \
#	-DEXCLUDE_CSR_BT_MAPS_MODULE \
#	-DEXCLUDE_CSR_BT_HDP_MODULE \

####### To enable WiFi AMP PAL support in Bluetooth demo apps
ifeq ($(CSR_BT_APP_AMP_WIFI),1)
CFLAGS += -DCSR_BT_APP_AMP_WIFI=1
endif

####### To enable UWB PAL support in Bluetooth demo apps
ifeq ($(CSR_BT_APP_AMP_UWB),1)
CFLAGS += -DCSR_BT_APP_AMP_UWB=1
endif

#
# Exclude generic code.
# These defines guard code that is "generic" and used by several profiles.
#
# Note: Consistency in the specified defines has to be maintained or
# build failures will occur, e.g. specifying EXCLUDE_BCHS_RFC_MODULE
# without excluding all the RFCOMM based profiles.
#
#CFLAGS += -DEXCLUDE_BCCMD_MODULE \
#	-DEXCLUDE_BCHS_RFC_MODULE \
#	-DEXCLUDE_BCHS_L2CA_MODULE \
#	-DEXCLUDE_CSR_BT_BNEP_MODULE \
#	-DEXCLUDE_CSR_BT_PPP_MODULE \
#	-DEXCLUDE_CSR_BT_SCO_MODULE \
#	-DEXCLUDE_CSR_BT_HID_PARSER_MODULE

####### Build and target rules
include $(FW_ROOT)/global_rules.mk

INC += $(BSP_INC) $(FW_INC)
LDPATH += $(call create_ldpath,$(BSP_LIB)) $(call create_ldpath,$(FW_LIB))

###### Use locals for defs and release_type
DEFS_ROOT ?= $(TOP)
include $(TOP)/release_type.mk

####### HCI/RFC build
ifeq ($(HOSTBUILD),HCI)
BUILD_TYPE = hci
BUILD_TYPE_EXT = _$(BUILD_TYPE)
endif

ifeq ($(HOSTBUILD),RFC)
BUILD_TYPE = rfc
BUILD_TYPE_EXT = _$(BUILD_TYPE)
endif

####### Minimum include path
INC += -I$(TOP)/config $(EXTRA_INC)

A_CONFIG_ROOT= $(shell echo $(CONFIG_ROOT) | sed -e "s,\(.\):\/,\/cygdrive\/\1\/,g")

####### Copy to config header
$(A_CONFIG_ROOT)/csr_bt_usr_config.h: $(A_CONFIG_ROOT)/$(CONFIG_H)
	$(CP) $< $@

config: $(A_CONFIG_ROOT)/csr_bt_usr_config.h

rules_debug:
	@echo "TARGET               : $(TARGET)"
	@echo "TARGET_PLATFORM      : $(TARGET_PLATFORM)"
	@echo "TARGET_OS            : $(TARGET_OS)"
	@echo "TARGET_ARCH          : $(TARGET_ARCH)"
	@echo "FW_ROOT              : $(FW_ROOT)"
	@echo "BSP_ROOT             : $(BSP_ROOT)"
	@echo "CONFIG_ROOT          : $(CONFIG_ROOT)"
	@echo "CONFIG               : $(CONFIG)"
	@echo "FW_CONFIG            : $(FW_CONFIG)"
	@echo "CFLAGS               : $(CFLAGS)"
	@echo "BSP_INC              : $(BSP_INC)"
	@echo "FW_INC               : $(FW_INC)"
	@echo "INC                  : $(INC)"
	@echo "LDPATH               : $(LDPATH)"
	@echo "BSP_LIBS             : $(BSP_LIBS)"
	@echo "LIBS                 : $(LIBS)"
	@echo "EXTRA_LIBS           : $(EXTRA_LIBS)"

####### Dependency mapping library search path (depmap)
DEPMAP_LIBPATHS := $(FW_LIB),$(BSP_LIB),$(TOP)/output/$(CONFIG)/$(TARGET)/lib,$(TOP)/output/$(CONFIG)/$(TARGET)/lib/extra
DEPMAP_MAPLIBS = $(LIB_NAME)

CODESIZE_LIBPATHS := $(FW_LIB),$(BSP_LIB),$(TOP)/output/$(CONFIG)/$(TARGET)/lib,$(TOP)/output/$(CONFIG)/$(TARGET)/lib/extra
CODESIZE_LIBS = $(LIB_NAME)

