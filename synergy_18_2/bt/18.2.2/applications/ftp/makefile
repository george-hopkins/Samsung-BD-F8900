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

default: lib

# We're dealing with two demo applications, so trigger building via
# separate makefiles
APP_TARGETS = ftc fts

ftc:
	$(MAKE) -f makefile.ftc $(ACTION) BUILD_TYPE=$(BUILD_TYPE)
fts:
	$(MAKE) -f makefile.fts $(ACTION) BUILD_TYPE=$(BUILD_TYPE)

