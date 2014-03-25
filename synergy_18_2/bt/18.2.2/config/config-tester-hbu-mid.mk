INSTALL_INTERNAL_APP_DEPENDENCIES = 0
INSTALL_INTERNAL_TEST_DEPENDENCIES = 1
include $(TOP)/config/config-codesize-hbu-mid.mk
ifneq ($(TARGET),bdb3-nucleus-arm6kz)
    include $(TOP)/config/config-tester.mk
else
    include $(TOP)/config/config-tester-bdb.mk
endif

# Tester related flags:
CFLAGS += -DEXCLUDE_CSR_BT_BNEP_SPEC_TEST_CASES

