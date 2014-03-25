####### Build
DEBUG ?= 1
LOG ?= 0
USE_DEPENDENCY_FILES ?= 0
ENABLE_SHUTDOWN ?= 1
EXCEPTION_PANIC ?= 0
NO_TREAT_WARN_AS_ERROR ?= 0
ACLBUF ?= 1
ACLBUF_USB ?= 0
CHIP_MANAGER_ENABLE ?= 0
CHIP_MANAGER_QUERY_ENABLE ?= 0
CSR_SDIO_USE_SDIO ?= 1
CSR_SDIO_USE_CSPI ?= 1
CSR_IP_SUPPORT_TLS ?= 0
MBLK_DEBUG ?= 0
CSR_HCI_SOCKET_TRANSPORT ?= 0
USB_WARMREST ?= 1
####### Local tools
DEVEL_TOOL_PATH  ?= $(FW_ROOT)/devel/tools
LOG_TOOL_PATH    ?= $(DEVEL_TOOL_PATH)/log
CREATE_PRIM_CODE ?= $(DEVEL_TOOL_PATH)/prim_code_generator/create_prim_code.sh
XML_TOOL_PATH ?= $(FW_ROOT)/tools/autogen

####### FTS
FTS_VER ?= 10.11.19.0

####### Wireshark
WIRESHARK_DIS_DEVEL_VERSION ?= 5.0.0
WIRESHARK_BUILD_DIS_ROOT ?= "c:/csr/3rdparty/sw/wireshark_disector_devel/$(WIRESHARK_DIS_DEVEL_VERSION)"
WIRESHARK_VERSION        ?= 1.2.0
WIRESHARK_EXE_ROOT       ?= "c:/Program Files/Wireshark"
WIRESHARK_PLUGIN_ROOT    ?= $(WIRESHARK_EXE_ROOT)/plugins/$(WIRESHARK_VERSION)

####### CASTE
CASTE_ROOT ?= s:/install/test/caste/8.0.0
CASTE_CONFIG ?= default

####### Config header
CONFIG_H ?= csr_usr_config_default.h

