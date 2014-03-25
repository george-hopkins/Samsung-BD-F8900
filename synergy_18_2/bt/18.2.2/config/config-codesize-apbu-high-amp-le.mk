#
# This is the APBU high-end + AMP + LE

CSR_AMP_ENABLE := 1

CSR_BT_LE_ENABLE := 1

CFLAGS += \
-DEXCLUDE_CSR_BT_GATT_MODULE_OPTIONAL

include $(TOP)/config/config-codesize-apbu-high.mk


