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
APP_TARGETS = mapc maps

maps:
	$(MAKE) -f makefile.maps $(ACTION) BUILD_TYPE=$(BUILD_TYPE)

mapc:
	$(MAKE) -f makefile.mapc $(ACTION) BUILD_TYPE=$(BUILD_TYPE)

include $(TOP)/local_rules.mk
include $(TOP)/applications/common/makefile.common
