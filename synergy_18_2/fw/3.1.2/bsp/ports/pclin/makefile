###############################################################################
#
#                (c) Cambridge Silicon Radio Limited 2010
#
#                All rights reserved and confidential information of CSR
#
###############################################################################
FW_ROOT := ../../..
include $(FW_ROOT)/local_rules.mk

all: lib

lib clean:
	$(MAKE) -C src/$(MODE)/framework_ext $@
	$(MAKE) -C src/$(MODE)/pmem $@
	$(MAKE) -C src/$(MODE)/time $@
	$(MAKE) -C src/$(MODE)/util $@
	$(MAKE) -C src/$(MODE)/panic $@
	$(MAKE) -C src/$(MODE)/lowlevel $@
ifeq ($(MODE),user)
	$(MAKE) -C src/$(MODE)/app_main $@
	$(MAKE) -C src/$(MODE)/main $@
	$(MAKE) -C src/$(MODE)/fs $@
	$(MAKE) -C src/$(MODE)/exceptionhandler $@
	$(MAKE) -C src/$(MODE)/logtrans $@
	$(MAKE) -C src/$(MODE)/ip $@
ifeq ($(BSP_SUPPORT_OPENSSL),1)
ifneq ($(CSR_IP_SUPPORT_TLS),1)
	$(MAKE) -C src/$(MODE)/tls $@
endif
endif
ifeq ($(BSP_SUPPORT_HCI_SOCKET),1)
	$(MAKE) -C src/$(MODE)/bluecore/transport/hci_socket $@
endif
endif

