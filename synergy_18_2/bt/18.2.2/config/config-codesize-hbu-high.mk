#
# This is the HBU high-end configuration with the following profiles and features:
# AV (+ SBC), AVRCP, HFG, DG, SPP, FTS, FTC, OPS, OPC, BPPC, 
# SAPS, PBAPS, JSR82, MAPS, BIPC, BIPS, (+AMP), (+BTLE/GATT)
#
#
# TODO: Update
# This is the HBU low-end configuration with the following profiles and features:
# - HFG:
#    - Excluding CSR2CSR features
#    - Only single connection support
#    - No dynamic AT-command filter
#    - ?? Maybe exclude some AT-command handling
#    - Excluding dynamic security mode change
# - OPP (OPC + OPS)
#   - Exclude the optional feature: Set Path 
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

#-DEXCLUDE_CSR_BT_HFG_MODULE
#-DEXCLUDE_CSR_BT_AVRCP_MODULE 
#-DEXCLUDE_CSR_BT_AV_MODULE 
#-DEXCLUDE_CSR_BT_OPC_MODULE 
#-DEXCLUDE_CSR_BT_OPS_MODULE 
#-DEXCLUDE_CSR_BT_DG_MODULE \
#-DEXCLUDE_CSR_BT_SPP_MODULE \
#-DEXCLUDE_CSR_BT_FTC_MODULE \
#-DEXCLUDE_CSR_BT_FTS_MODULE \
#-DEXCLUDE_CSR_BT_BPPC_MODULE \
#-DEXCLUDE_CSR_BT_SAPS_MODULE \
#-DEXCLUDE_CSR_BT_JSR82_MODULE \
#-DEXCLUDE_CSR_BT_MAPS_MODULE \
#-DEXCLUDE_CSR_BT_BIPC_MODULE \
#-DEXCLUDE_CSR_BT_BIPS_MODULE \
#-DEXCLUDE_CSR_BT_PAS_MODULE \


CFLAGS += \
-DEXCLUDE_CSR_BT_HFG_MODULE_OPTIONAL \
-DEXCLUDE_CSR_BT_AV_OPTIONAL \
-DEXCLUDE_CSR_BT_AVRCP_CT_MODULE \
-DEXCLUDE_CSR_BT_CM_MODULE_OPTIONAL \
-DEXCLUDE_CSR_BT_SC_MODULE_OPTIONAL \
-DEXCLUDE_CSR_BT_SD_MODULE_OPTIONAL \
-DEXCLUDE_CSR_BT_SPP_MODULE_OPTIONAL \
-DEXCLUDE_CSR_BT_OPTIONAL_UTILS \
-DEXCLUDE_CSR_BT_TPT_MODULE \
-DEXCLUDE_CSR_BT_AT_MODULE \
-DEXCLUDE_CSR_BT_BPPS_MODULE \
-DEXCLUDE_CSR_BT_BSL_MODULE \
-DEXCLUDE_CSR_BT_DUNC_MODULE \
-DEXCLUDE_CSR_BT_HCRP_MODULE \
-DEXCLUDE_CSR_BT_HIDD_MODULE \
-DEXCLUDE_CSR_BT_HIDH_MODULE \
-DEXCLUDE_CSR_BT_PAC_MODULE \
-DEXCLUDE_CSR_BT_SAPC_MODULE \
-DEXCLUDE_CSR_BT_SMLC_MODULE \
-DEXCLUDE_CSR_BT_SMLS_MODULE \
-DEXCLUDE_CSR_BT_SYNCS_MODULE \
-DEXCLUDE_CSR_BT_SYNCC_MODULE \
-DEXCLUDE_CSR_BT_MAPC_MODULE \
-DEXCLUDE_CSR_BT_HDP_MODULE \
-DEXCLUDE_CSR_BT_MCAP_MODULE \
-DEXCLUDE_CSR_BT_MDER_MODULE \
-DEXCLUDE_CSR_BT_PHDC_AG_MODULE \
-DEXCLUDE_CSR_BT_PHDC_MGR_MODULE \
-DEXCLUDE_CSR_BT_HF_MODULE \
-DEXCLUDE_CSR_BT_PROX_SRV_MODULE \
-DEXCLUDE_CSR_BT_THERM_SRV_MODULE 


#
# Exclude generic code.
# These defines guard code that is "generic" and used by several profiles.
#
# Note: Consistency in the specified defines has to be maintained or
# build failures will occur, e.g. specifying EXCLUDE_BCHS_RFC_MODULE
# without excluding all the RFCOMM based profiles.
# TODO: Move the relevant ones into the header files
CFLAGS += \
-DEXCLUDE_CSR_BT_PPP_MODULE

#-DEXCLUDE_CSR_BT_SCO_MODULE \
#-DEXCLUDE_BCCMD_MODULE 
#-DEXCLUDE_BCHS_RFC_MODULE 
#-DEXCLUDE_BCHS_L2CA_MODULE 
#-DEXCLUDE_CSR_BT_SBC_MODULE \


include $(TOP)/config/config-default.mk


