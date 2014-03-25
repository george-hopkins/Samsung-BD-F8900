###############################################################################
#
#                (c) Cambridge Silicon Radio Limited 2010
#
#                All rights reserved and confidential information of CSR
#
###############################################################################

######## Set default config
#CONFIG ?= default
CONFIG ?= samsung
CONFIG_ROOT ?= $(FW_ROOT)/config

include $(CONFIG_ROOT)/config-$(CONFIG).mk

######## Parse config to rules
ifeq ($(LOG),1)
CFLAGS += -DCSR_LOG_ENABLE \
	-DCSR_LOG_INCLUDE_FILE_NAME_AND_LINE_NUMBER
endif

ifeq ($(CSR_IP_SUPPORT_TLS),1)
CFLAGS += -DCSR_IP_SUPPORT_TLS
endif

#
# CSR_LOG Text level excludes
#
#CFLAGS += -DCSR_LOG_LEVEL_TEXT_CRITICAL_DISABLE \
#	-DCSR_LOG_LEVEL_TEXT_ERROR_DISABLE \
#	-DCSR_LOG_LEVEL_TEXT_WARNING_DISABLE \
#	-DCSR_LOG_LEVEL_TEXT_INFO_DISABLE \
#	-DCSR_LOG_LEVEL_TEXT_DEBUG_DISABLE

ifeq ($(MBLK_DEBUG),1)
CFLAGS += -DMBLK_DEBUG
endif

ifeq ($(ENABLE_SHUTDOWN),1)
CFLAGS += -DENABLE_SHUTDOWN
endif

ifeq ($(EXCEPTION_HANDLER),0)
CFLAGS += -DEXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
endif

ifeq ($(EXCEPTION_PANIC),1)
CFLAGS += -DEXCEPTION_PANIC
endif

ifneq ($(FTS_VER),)
CFLAGS += -DFTS_VER=$(FTS_VER)
endif

ifeq ($(CSR_INSTRUMENTED_PROFILING_SERVICE),1)
CFLAGS += -DCSR_INSTRUMENTED_PROFILING_SERVICE
endif

ifeq ($(CSR_SDIO_USE_SDIO),1)
CFLAGS += -DCSR_SDIO_USE_SDIO
endif

ifeq ($(CSR_SDIO_USE_CSPI),1)
CFLAGS += -DCSR_SDIO_USE_CSPI
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

ifeq ($(CSR_HCI_SOCKET_TRANSPORT),1)
CFLAGS += -DCSR_HCI_SOCKET_TRANSPORT
endif
# usb warm reset
ifeq ($(USB_WARMREST),1)
CFLAGS += -DCSR_ENABLE_WARM_RESET
endif
####### Build and target rules
FW_CONFIG := $(CONFIG)
include $(FW_ROOT)/global_rules.mk

####### This depends on features exported by the target script
ifeq ($(BSP_SUPPORT_ACLBUF),1)
ifeq ($(ACLBUF),1)
CFLAGS += -DCSR_ACLBUF_ENABLE
endif
ifeq ($(ACLBUF_USB),1)
CFLAGS += -DCSR_ACLBUF_USB_ENABLE
endif
endif

INC += $(BSP_INC) $(FW_INC)

####### Default libs
DEFAULT_LIBS += $(BSP_LIBS)

####### Default config - TO BE DEPRECATED
INC += -I$(FW_ROOT)/config/

####### Version definitions
-include $(FW_ROOT)/ver.mk
FW_VERSION := $(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_FIXLEVEL)
FW_DLL_VERSION := $(VERSION_MAJOR)$(VERSION_MINOR)$(VERSION_FIXLEVEL)

######## Copy to config header
$(CONFIG_ROOT)/csr_usr_config.h: $(CONFIG_ROOT)/$(CONFIG_H)
	sed -e 's,CSR_USR_CONFIG.*_H__,CSR_USR_CONFIG_H__,g' $< > $@

config: $(CONFIG_ROOT)/csr_usr_config.h

rules_debug:
	@echo " ====== Makefile variables ====== "
	@echo ""
	@echo "TARGET               : $(TARGET)"
	@echo "TARGET_PLATFORM      : $(TARGET_PLATFORM)"
	@echo "TARGET_OS            : $(TARGET_OS)"
	@echo "TARGET_ARCH          : $(TARGET_ARCH)"
	@echo "FW_ROOT              : $(FW_ROOT)"
	@echo "BSP_ROOT             : $(BSP_ROOT)"
	@echo "CONFIG_ROOT          : $(CONFIG_ROOT)"
	@echo "CONFIG               : $(CONFIG)"
	@echo "CFLAGS               : $(CFLAGS)"
	@echo ""
	@echo " ====== Required tools and the version currently installed, if found ====== "
	@echo ""
	@echo "awk                  : $(shell awk --version | grep -o -P '\d+\.\d+\.\d+')"
	@echo "bash                 : $(shell bash --version | grep -o -P '\d+\.\d+\.\d+')"
	@echo "perl                 : $(shell perl --version | grep -o -P '\d+\.\d+\.\d+')"
	@echo "sed                  : $(shell sed --version | grep -o -P '\d+.\d+.\d+')"

