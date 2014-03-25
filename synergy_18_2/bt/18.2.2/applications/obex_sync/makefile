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
APP_TARGETS = osc oss

oss:
	$(MAKE) -f makefile.oss $(ACTION) BUILD_TYPE=$(BUILD_TYPE)

osc:
	$(MAKE) -f makefile.osc $(ACTION) BUILD_TYPE=$(BUILD_TYPE)

include $(TOP)/local_rules.mk
include $(TOP)/applications/common/makefile.common
