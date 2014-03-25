ifneq ($(TARGET),bdb3-nucleus-arm6kz)
    include $(TOP)/config/config-tester.mk
else
    include $(TOP)/config/config-tester-bdb.mk
endif

####### Framework config
CSR_BT_LE_ENABLE := 1

