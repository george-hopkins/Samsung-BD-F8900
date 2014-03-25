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
APP_TARGETS = pbapc pbaps

pbaps:
	$(MAKE) -f makefile.pbaps $(ACTION) BUILD_TYPE=$(BUILD_TYPE)

pbapc:
	$(MAKE) -f makefile.pbapc $(ACTION) BUILD_TYPE=$(BUILD_TYPE)

include $(TOP)/local_rules.mk
include $(TOP)/applications/common/makefile.common
