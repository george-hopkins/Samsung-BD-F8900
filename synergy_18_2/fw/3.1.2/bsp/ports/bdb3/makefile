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
	$(MAKE) -C src/sched $@
	$(MAKE) -C src/pmem $@
	$(MAKE) -C src/time $@
	$(MAKE) -C src/util $@
	$(MAKE) -C src/panic $@
	$(MAKE) -C src/main $@
	$(MAKE) -C src/membuf $@
	$(MAKE) -C src/exceptionhandler $@
	$(MAKE) -C src/aclbuf $@
	$(MAKE) -C src/app_main $@
	$(MAKE) -C src/console $@
	$(MAKE) -C src/ether $@
	$(MAKE) -C src/fs $@
ifeq ($(CSR_INSTRUMENTED_PROFILING_SERVICE),1)
	$(MAKE) -C src/ips $@
endif
	$(MAKE) -C src/logtrans $@
	$(MAKE) -C src/lowlevel/drivers $@
	$(MAKE) -C src/sdio_master $@
	$(MAKE) -C src/spi_master $@
	$(MAKE) -C src/lowlevel/uart $@
	$(MAKE) -C src/framework_ext $@

veryclean:
	$(RM) -rf output
