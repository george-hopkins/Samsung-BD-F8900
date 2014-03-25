####### Build
DEBUG ?= 1
LOG ?= 1
USE_DEPENDENCY_FILES ?= 0
ENABLE_SHUTDOWN ?= 1
EXCEPTION_PANIC ?= 0
INCLUDE_EXCEPTION_HANDLER_MODULE ?= 1
INSTALL_INTERNAL_APP_DEPENDENCIES ?= 0
INSTALL_INTERNAL_TEST_DEPENDENCIES ?= 1
CSR_TESTER ?= 1
SEC_MODE_2 ?= 0

####### Framework config
FW_CONFIG ?= default

####### AMP config
CSR_AMP_ENABLE ?= 1
CSR_BT_APP_AMP_WIFI ?= 1
CSR_BT_APP_AMP_UWB  ?= 0

####### Local tools
DEVEL_TOOL_PATH  ?= $(FW_ROOT)/devel/tools
LOG_TOOL_PATH    ?= $(DEVEL_TOOL_PATH)/log
CREATE_PRIM_CODE ?= $(DEVEL_TOOL_PATH)/prim_code_generator/create_prim_code.sh

####### FTS
FTS_VER ?= 8.7.13.0

####### Wireshark
WIRESHARK_DIS_DEVEL_VERSION ?= 5.0.0
WIRESHARK_VERSION           ?= 1.2.0
WIRESHARK_BUILD_DIS_ROOT    ?= "s:/install/3rdparty/sw/wireshark_disector_devel/$(WIRESHARK_DIS_DEVEL_VERSION)"
WIRESHARK_EXE_ROOT          ?= "c:/Program Files/Wireshark_120"
WIRESHARK_PLUGIN_ROOT       ?= $(WIRESHARK_EXE_ROOT)/plugins/$(WIRESHARK_VERSION)

####### Config header
CFLAGS += -DINSTALL_L2CAP_RAW_SUPPORT
CFLAGS += -DINSTALL_L2CAP_FIXED_CHANNEL_SUPPORT
CFLAGS += -DINSTALL_L2CAP_FLOWSPEC_SUPPORT
CFLAGS += -DONE_SHOT_TESTER
CONFIG_H := csr_bt_usr_config_default.h

####### To call exit in tester/tester x86 in stead of abort set this flag to 1
ifeq ($(CSR_BT_EXIT_AT_PANIC),1)
CFLAGS += -DCSR_BT_EXIT_AT_PANIC=1
endif

####### CASTE
CASTE_ROOT ?= s:/install/test/caste/8.0.0
CASTE_CONFIG ?= default


