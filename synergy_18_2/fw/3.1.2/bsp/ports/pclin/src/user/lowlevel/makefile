###############################################################################
#
#                (c) Cambridge Silicon Radio Limited 2009
#
#                All rights reserved and confidential information of CSR
#
###############################################################################
FW_ROOT := ../../../../../..
include $(FW_ROOT)/local_rules.mk

all: lib

bin:

lib: 
	$(MAKE) -C uart lib
	$(MAKE) -C usb lib

clean:
	$(MAKE) -C uart clean
	$(MAKE) -C usb clean
