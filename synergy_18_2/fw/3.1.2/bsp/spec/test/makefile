###############################################################################
#
#                (c) Cambridge Silicon Radio Limited 2009
#
#                All rights reserved and confidential information of CSR
#
###############################################################################

FW_ROOT := ../../..

include $(FW_ROOT)/local_rules.mk

.PHONY: all clean 

DEFAULT: all

all:
	$(MAKE) -C coal all
	$(MAKE) -C framework_ext all

clean:
	$(MAKE) -C coal clean
	$(MAKE) -C framework_ext clean

veryclean:
	$(RM) -rf output
