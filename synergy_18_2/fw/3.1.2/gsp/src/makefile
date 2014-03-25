###############################################################################
#
#                (c) Cambridge Silicon Radio Limited 2010
#
#                All rights reserved and confidential information of CSR
#
# REVISION:      $Revision: #1 $
###############################################################################
FW_ROOT := ../..
include $(FW_ROOT)/local_rules.mk

.PHONY: lib all clean 

DEFAULT: all

all: lib

lib clean:
	$(MAKE) -C application/ui $@
	$(MAKE) -C application/app $@
	$(MAKE) -C application/bluecore_manager $@
	$(MAKE) -C application/dhcp_server_manager $@
	$(MAKE) -C application/ethernet_manager $@
ifeq ($(CSR_INSTRUMENTED_PROFILING_SERVICE),1)
	$(MAKE) -C application/ips_manager $@
endif
	$(MAKE) -C application/sdio_probe $@
	$(MAKE) -C application/tcp_transceiver $@

	$(MAKE) -C bluecore/aclbuf_lower $@
	$(MAKE) -C bluecore/am $@
	$(MAKE) -C bluecore/bccmd $@
	$(MAKE) -C bluecore/dspm $@
	$(MAKE) -C bluecore/fp $@
	$(MAKE) -C bluecore/hci $@
	$(MAKE) -C bluecore/hq $@
	$(MAKE) -C bluecore/tm $@
	$(MAKE) -C bluecore/vm $@
	$(MAKE) -C bluecore/transport/bcsp $@
	$(MAKE) -C bluecore/transport/h4ds $@
	$(MAKE) -C bluecore/transport/h4i $@
	$(MAKE) -C bluecore/transport/sdio $@
	$(MAKE) -C bluecore/transport/type_a $@
	$(MAKE) -C bluecore/transport/type_a_async $@
	$(MAKE) -C bluecore/transport/type_a_sync $@
	$(MAKE) -C bluecore/transport/usb $@
	$(MAKE) -C converter $@
	$(MAKE) -C data_store $@
	$(MAKE) -C dhcp_server $@
	$(MAKE) -C fsal $@
	$(MAKE) -C ip $@
	$(MAKE) -C log/formats/btsnoop $@
	$(MAKE) -C log/formats/cleartext $@
	$(MAKE) -C log/formats/fts $@
	$(MAKE) -C log/formats/pcap $@
	$(MAKE) -C log/log $@
	$(MAKE) -C memstat $@
	$(MAKE) -C random $@
	$(MAKE) -C sched $@
	$(MAKE) -C tftp $@
ifneq ($(SUB_TARGET_ARCH),LinuxKernel)	
	$(MAKE) -C tftp_file $@
endif	
	$(MAKE) -C utils/arg_search $@
	$(MAKE) -C utils/formatted_io $@
	$(MAKE) -C utils/mblk $@
	$(MAKE) -C utils/msgconv $@
	$(MAKE) -C utils/list $@
	$(MAKE) -C utils/queue_lib $@
	$(MAKE) -C utils/message_queue $@
	$(MAKE) -C utils/unicode $@

ifeq ($(CHIP_MANAGER_TEST_ENABLE),1)
	$(MAKE) -C bluecore/chip_manager_test $@
endif

ifeq ($(CSR_INSTRUMENTED_PROFILING_SERVICE),1)
	$(MAKE) -C application/ips_manager $@
endif
