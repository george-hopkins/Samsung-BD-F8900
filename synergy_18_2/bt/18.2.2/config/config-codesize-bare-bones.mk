#
# This is the Bare-Bones SPP only configuration with the following profiles and features:
#
# - SPP
#   - Exclude the optional feature: SCO support (Audio over bcsp)
# - CM 
#   - Excluding:
#     - L2cap support
#     - Event subscription
#     - Initialization of Park Mode
#     - Cashed parameters for fast connect
#   - Reduced "HCI direct" interface
# - SD
#   - Excluding Device Info service record
#   - Excluding Service Search Attribute Request feature
# - SC
#   - Excluding theJsr82 security interface
# - Exclusion of:
#   - 

CSR_AMP_ENABLE ?= 0
CSR_BT_LE_ENABLE ?= 0


#-DEXCLUDE_CSR_BT_SPP_MODULE \

CFLAGS += \
-DEXCLUDE_CSR_BT_CM_MODULE_OPTIONAL \
-DEXCLUDE_CSR_BT_SC_MODULE_OPTIONAL \
-DEXCLUDE_CSR_BT_SD_MODULE_OPTIONAL \
-DEXCLUDE_CSR_BT_SPP_MODULE_OPTIONAL \
-DEXCLUDE_CSR_BT_OPTIONAL_UTILS \
-DEXCLUDE_CSR_BT_HFG_MODULE \
-DEXCLUDE_CSR_BT_OPC_MODULE \
-DEXCLUDE_CSR_BT_OPS_MODULE \
-DEXCLUDE_CSR_BT_AT_MODULE \
-DEXCLUDE_CSR_BT_BIPC_MODULE \
-DEXCLUDE_CSR_BT_BIPS_MODULE \
-DEXCLUDE_CSR_BT_BPPS_MODULE \
-DEXCLUDE_CSR_BT_BPPC_MODULE \
-DEXCLUDE_CSR_BT_BSL_MODULE \
-DEXCLUDE_CSR_BT_DG_MODULE \
-DEXCLUDE_CSR_BT_DUNC_MODULE \
-DEXCLUDE_CSR_BT_FTC_MODULE \
-DEXCLUDE_CSR_BT_FTS_MODULE \
-DEXCLUDE_CSR_BT_HCRP_MODULE \
-DEXCLUDE_CSR_BT_HIDD_MODULE \
-DEXCLUDE_CSR_BT_HIDH_MODULE \
-DEXCLUDE_CSR_BT_JSR82_MODULE \
-DEXCLUDE_CSR_BT_PAC_MODULE \
-DEXCLUDE_CSR_BT_PAS_MODULE \
-DEXCLUDE_CSR_BT_SAPC_MODULE \
-DEXCLUDE_CSR_BT_SAPS_MODULE \
-DEXCLUDE_CSR_BT_SMLC_MODULE \
-DEXCLUDE_CSR_BT_SMLS_MODULE \
-DEXCLUDE_CSR_BT_SYNCS_MODULE \
-DEXCLUDE_CSR_BT_SYNCC_MODULE \
-DEXCLUDE_CSR_BT_MAPC_MODULE \
-DEXCLUDE_CSR_BT_MAPS_MODULE \
-DEXCLUDE_CSR_BT_HDP_MODULE \
-DEXCLUDE_CSR_BT_TPT_MODULE \
-DEXCLUDE_CSR_BT_MCAP_MODULE \
-DEXCLUDE_CSR_BT_MDER_MODULE \
-DEXCLUDE_CSR_BT_PHDC_AG_MODULE \
-DEXCLUDE_CSR_BT_PHDC_MGR_MODULE \
-DEXCLUDE_CSR_BT_AVRCP_MODULE \
-DEXCLUDE_CSR_BT_AV_MODULE \
-DEXCLUDE_CSR_BT_HF_MODULE \
-DEXCLUDE_CSR_BT_GOEP_20_MODULE

#
# Exclude generic code.
# These defines guard code that is "generic" and used by several profiles.
#
# Note: Consistency in the specified defines has to be maintained or
# build failures will occur, e.g. specifying EXCLUDE_BCHS_RFC_MODULE
# without excluding all the RFCOMM based profiles.
CFLAGS += \
-DEXCLUDE_CSR_BT_SBC_MODULE \
-DEXCLUDE_CSR_BT_PPP_MODULE \
-DEXCLUDE_CSR_BT_SCO_MODULE 


#-DCSR_BT_INSTALL_SCO_MODULE  #TEMP FIX for corestack unable to build without SCO




#-DEXCLUDE_BCCMD_MODULE 
#-DEXCLUDE_BCHS_RFC_MODULE 


include $(TOP)/config/config-default.mk


