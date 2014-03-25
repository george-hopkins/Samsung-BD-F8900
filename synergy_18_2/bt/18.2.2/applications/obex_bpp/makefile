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

default: all

# We're dealing with two demo applications, so trigger building via
# separate makefiles
APP_TARGETS = bppc bpps

bpps:
	$(MAKE) -f makefile.bpps $(ACTION) BUILD_TYPE=$(BUILD_TYPE)

bppc:
	$(MAKE) -f makefile.bppc $(ACTION) BUILD_TYPE=$(BUILD_TYPE)

include $(TOP)/local_rules.mk
include $(TOP)/applications/common/makefile.common
