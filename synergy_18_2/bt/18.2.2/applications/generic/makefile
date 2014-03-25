#-*-Makefile-*-
###############################################################################
#
#                (c) Cambridge Silicon Radio Limited 2009
#
#                All rights reserved and confidential information of CSR
#
# REVISION:      $Revision: #1 $
###############################################################################
TOP := ../..
include $(TOP)/local_rules.mk

bootstrap:
	$(MAKE) -C $(TOP)/applications/bootstrap all

all: bin
bin: bootstrap bin_hci bin_rfc

lib: 

clean: clean_hci clean_rfc

bin_hci: 
ifeq ($(HCI_IN_RELEASE),1)
	$(MAKE) -f makefile.bin bin HOSTBUILD=HCI
endif

clean_hci:
ifeq ($(HCI_IN_RELEASE),1)
	$(MAKE) -f makefile.bin clean HOSTBUILD=HCI
endif

bin_rfc: bin_hci

clean_rfc:

lib:
bin:

