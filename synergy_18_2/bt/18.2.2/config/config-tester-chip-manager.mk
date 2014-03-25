ifneq ($(TARGET),bdb3-nucleus-arm6kz)
    include $(TOP)/config/config-tester.mk
else
    include $(TOP)/config/config-tester-bdb.mk
endif

CHIP_MANAGER_TEST_ENABLE := 1

####### Framework config
FW_CONFIG := chip-manager-test

