####### Build
DEBUG ?= 1
LOG ?= 0
USE_DEPENDENCY_FILES ?= 0
ENABLE_SHUTDOWN ?= 1
EXCEPTION_PANIC ?= 0
INCLUDE_EXCEPTION_HANDLER_MODULE ?= 1
INSTALL_INTERNAL_APP_DEPENDENCIES ?= 1
INSTALL_INTERNAL_TEST_DEPENDENCIES ?= 0
NO_TREAT_WARN_AS_ERROR ?= 0

####### Enable AMP
CSR_AMP_ENABLE ?= 1

####### Link against these AMPs in demo applications
CSR_BT_APP_AMP_WIFI ?= 1
CSR_BT_APP_AMP_UWB  ?= 0

####### Framework config
FW_CONFIG ?= default

####### Local tools
DEVEL_TOOL_PATH  ?= $(FW_ROOT)/devel/tools
LOG_TOOL_PATH    ?= $(DEVEL_TOOL_PATH)/log
CREATE_PRIM_CODE ?= $(DEVEL_TOOL_PATH)/prim_code_generator/create_prim_code.sh

####### FTS
FTS_VER ?= 9.2.26.0

####### Wireshark
WIRESHARK_DIS_DEVEL_VERSION ?= 4.0.0
WIRESHARK_BUILD_DIS_ROOT    ?= "s:/install/3rdparty/sw/wireshark_disector_devel/$(WIRESHARK_DIS_DEVEL_VERSION)"
WIRESHARK_VERSION           ?= 1.0.5
WIRESHARK_EXE_ROOT          ?= "c:/Program Files/Wireshark"
WIRESHARK_PLUGIN_ROOT       ?= $(WIRESHARK_EXE_ROOT)/plugins/$(WIRESHARK_VERSION)

####### CASTE
CASTE_ROOT ?= s:/install/test/caste/6.0.0
CASTE_CONFIG ?= default

####### Config header
CONFIG_H := csr_bt_usr_config_default.h

