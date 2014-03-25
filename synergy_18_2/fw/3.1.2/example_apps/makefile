################################################################################
#
#           (c) Cambridge Silicon Radio Limited 2010
#
#           All rights reserved and confidential information of CSR
#
################################################################################
FW_ROOT := ..
OBJ_PATH = output/$(CONFIG)/$(TARGET)/obj
include $(FW_ROOT)/local_rules.mk

#
# Default setting for variant of the specific part of the bootstrap.
#
#CSR_BOOTSTRAP_SPECIFIC := FILE_BASED
#CSR_BOOTSTRAP_SPECIFIC := PLATFORM
CSR_BOOTSTRAP_SPECIFIC := STRING_BASED

BIN_PATH := output/$(CONFIG)/$(TARGET)/bin
BIN_NAME := $(call bin_name,framework_example_app)

INC += -Ibootstrap

ifneq ($(filter $(TARGET_PLATFORM),bdb2 bdb3),)
CFLAGS += -DDEMO_USE_ETHERNET_TASK
endif

ifneq ($(filter $(BSP_LIBS),csr_usb_com),)
CFLAGS += -DDEMO_USE_USB
endif

ifneq ($(filter $(BSP_LIBS),csr_spi_master csr_sdio_master csr_sdio),)
CFLAGS += -DDEMO_USE_SDIO
endif

ifeq ($(CSR_BOOTSTRAP_SPECIFIC),FILE_BASED)
CFLAGS += -DCSR_BOOTSTRAP_PSR_DIR=\"bootstrap/psr\"
endif

LIBS := \
        $(FW_LIB)/$(call lib_name,csr_sched) \
        $(call use_lib,csr_ui) \
        $(call use_lib,csr_app) \
        $(call use_lib,csr_main) \
        $(call use_lib,csr_random) \
        $(call use_lib,csr_aclbuf_lower) \
        $(call use_lib,csr_arg_search) \
        $(call use_lib,csr_app_main_bluecore) \
        $(call use_lib,csr_tm_bluecore) \
        $(call use_lib,csr_bccmd) \
        $(call use_lib,csr_fastpipe) \
        $(call use_lib,csr_hq) \
        $(call use_lib,csr_vm) \
        $(call use_lib,csr_am) \
        $(call use_lib,csr_hci) \
        $(call use_lib,csr_memstat) \
        $(call use_lib,csr_bluecore_manager) \
        $(call use_lib,csr_ethernet_manager) \
        $(call use_lib,csr_sdio_probe) \
        $(call use_lib,csr_tcp_transceiver) \
        $(call use_lib,csr_ip) \
        $(call use_lib,csr_ip_ether_lib) \
        $(call use_lib,csr_ip_socket_lib) \
        $(call use_lib,csr_ip_ifconfig_lib) \
        $(call use_lib,csr_dhcp_server_manager) \
        $(call use_lib,csr_dhcp_server) \
        $(call use_lib,csr_dhcp_server_lib) \
        $(call use_lib,csr_tftp) \
        $(call use_lib,csr_tftp_lib) \
        $(call use_lib,csr_tftp_file) \
        $(call use_lib,csr_fsal) \
        $(call use_lib,csr_fsal_lib) \
        $(call use_lib,csr_list) \
        $(call use_lib,csr_message_queue) \
        $(call use_lib,csr_formatted_io) \
        $(call use_lib,csr_msg_converter) \
        $(call use_lib,csr_core_msg_converter) \
        $(call use_lib,csr_unicode) \
        $(call use_lib,csr_mblk) \
        $(call use_lib,csr_sdio) \
        $(call use_lib,csr_type_a) \
        $(call use_lib,csr_type_a_async) \
        $(call use_lib,csr_bcsp) \
        $(call use_lib,csr_h4ds) \
        $(call use_lib,csr_queue_lib) \
        $(call use_lib,$(BSP_LIBS))

ifeq ($(CSR_HCI_SOCKET_TRANSPORT),1)
LIBS += $(call use_lib,csr_hci_socket)
else
LIBS += $(call use_lib,csr_usb)
endif

ifeq ($(LOG),1)
LIBS += $(call use_lib,csr_log) \
        $(call use_lib,csr_log_cleartext) \
        $(call use_lib,csr_log_fts) \
        $(call use_lib,csr_log_pcap) \
        $(call use_lib,csr_log_btsnoop)
endif

ifeq ($(CSR_INSTRUMENTED_PROFILING_SERVICE), 1)
LIBS += $(call use_lib,csr_ips_manager)
endif

ifeq ($(CHIP_MANAGER_TEST_ENABLE),1)
LIBS += $(call use_lib,csr_chip_manager_test)
endif

LDPATH += \
          $(call create_ldpath,$(BSP_LIB)) \
          $(call create_ldpath,$(FW_LIB))

SRC := csr_tasks.c \
       bootstrap/csr_bootstrap_generic.c

ifeq ($(CSR_BOOTSTRAP_SPECIFIC),FILE_BASED)
SRC += bootstrap/csr_bootstrap_specific_file_based.c
endif
ifeq ($(CSR_BOOTSTRAP_SPECIFIC),PLATFORM)
SRC += bootstrap/csr_bootstrap_specific_platform.c
endif
ifeq ($(CSR_BOOTSTRAP_SPECIFIC),STRING_BASED)
SRC += bootstrap/csr_bootstrap_specific_string_based.c \
       $(wildcard bootstrap/psr/*.c)
endif

OBJ := $(addprefix $(OBJ_PATH)/,$(SRC:.c=.o))

DEP := $(OBJ:.o=.d)
include $(FW_ROOT)/m_targets.mk

all: bin

bin: $(BIN_PATH)/$(BIN_NAME)

$(BIN_PATH)/$(BIN_NAME): $(OBJ)
	$(MKDIR) $(@D)
	$(call create_exec,$(BIN_PATH)/$(BIN_NAME),$(OBJ),$(LDPATH),$(LIBS))

clean:
	$(call clean_exec,$(BIN_PATH)/$(BIN_NAME))
	$(RM) $(call path_subst,$(OBJ))
