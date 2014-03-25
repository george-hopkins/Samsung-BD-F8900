#-*-Makefile-*-
###############################################################################
#
#                (c) Cambridge Silicon Radio Limited 2009
#
#                All rights reserved and confidential information of CSR
#
# REVISION:      $Revision: #1 $
###############################################################################
TOP := ../../../../..
include $(TOP)/local_rules.mk

ifneq ($(filter $(CFLAGS),-DEXCLUDE_CSR_BT_RFC_MODULE -DEXCLUDE_CSR_BT_L2CA_MODULE -DEXCLUDE_CSR_BT_AV_MODULE -DEXCLUDE_CSR_BT_SPP_MODULE),)
$(warning Can not build with EXCLUDE_CSR_BT_RFC_MODULE/EXCLUDE_CSR_BT_L2CA_MODULE/EXCLUDE_CSR_BT_AV_MODULE/EXCLUDE_CSR_BT_SPP_MODULE defined)
all:
bin:
clean:
else
all: bin 
bin: bin_hci bin_rfc
clean: clean_hci clean_rfc
endif

bin_hci: 
	$(MAKE) -C tools/generate_serialization_libs all

ifeq ($(TARGET),bchsdevel-arm)
	$(MAKE) -C $(BCHSDEVEL_BOARD_ROOT)/drivers/low_level/arm lib
	$(MAKE) -C $(BCHSDEVEL_BOARD_ROOT)/drivers/scheduler/arm lib
	$(MAKE) -C $(BCHSDEVEL_BOARD_ROOT)/drivers/peripherals/touch_screen/generic lib
	$(MAKE) -C $(BCHSDEVEL_BOARD_ROOT)/drivers/ramdisk lib
	$(MAKE) -C $(TOP)/porting/sc_db/file lib
	$(MAKE) -C $(TOP)/porting/csr_sbc/code lib
	$(MAKE) -C $(TOP) lib
endif
ifeq ($(TARGET),integrator-arm)
	$(MAKE) -C $(BSP_ROOT)/arm/integrator lib
	$(MAKE) -C drivers/low_level/arm/integrator lib
endif
ifeq ($(HCI_IN_RELEASE),1)
	$(MAKE) -f makefile.hci arm_bin TRANSPORT=BCSP
endif

clean_hci:
ifeq ($(HCI_IN_RELEASE),1)
	$(MAKE) -f makefile.hci arm_clean TRANSPORT=BCSP
endif

bin_rfc: bin_hci
	$(MAKE) -C tools/generate_serialization_libs all

ifeq ($(TARGET),bchsdevel-arm)
	$(MAKE) -C $(BCHSDEVEL_BOARD_ROOT)/drivers/low_level/arm lib
	$(MAKE) -C $(BCHSDEVEL_BOARD_ROOT)/drivers/scheduler/arm lib
	$(MAKE) -C $(BCHSDEVEL_BOARD_ROOT)/drivers/peripherals/touch_screen/generic lib
	$(MAKE) -C $(BCHSDEVEL_BOARD_ROOT)/drivers/ramdisk lib
	$(MAKE) -C $(TOP)/porting/sc_db/file lib
	$(MAKE) -C $(TOP)/porting/csr_sbc/code lib
	$(MAKE) -C $(TOP) lib
endif
ifeq ($(TARGET),integrator-arm)
	$(MAKE) -C $(BSP_ROOT)/arm/integrator lib
	$(MAKE) -C drivers/low_level/arm/integrator lib
endif

clean_rfc:
