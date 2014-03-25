#
# This is the HBU high-end + AMP + LE 

CSR_BT_LE_ENABLE := 1

CFLAGS += \
-DEXCLUDE_CSR_BT_GATT_MODULE_OPTIONAL

CSR_AMP_ENABLE := 1

include $(TOP)/config/config-codesize-hbu-high.mk


