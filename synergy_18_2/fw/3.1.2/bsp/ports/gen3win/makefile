###############################################################################
#
#                (c) Cambridge Silicon Radio Limited 2009
#
#                All rights reserved and confidential information of CSR
#
###############################################################################
FW_ROOT := ../../..
include $(FW_ROOT)/local_rules.mk

all: lib 

lib clean:
	$(MAKE) -C src/aclbuf $@	
#	$(MAKE) -C src/app_main $@	
	$(MAKE) -C src/exceptionhandler $@
	$(MAKE) -C src/framework_ext $@
	$(MAKE) -C src/fs $@
#	$(MAKE) -C src/ip $@
#	$(MAKE) -C src/logtrans $@
#	$(MAKE) -C src/lowlevel/sdio $@
	$(MAKE) -C src/lowlevel/uart $@
#	$(MAKE) -C src/lowlevel/usb $@
#	$(MAKE) -C src/main $@
	$(MAKE) -C src/membuf $@
	$(MAKE) -C src/panic $@
	$(MAKE) -C src/pmem $@
	$(MAKE) -C src/time $@
	$(MAKE) -C src/util $@

veryclean:
	$(RM) -rf output
