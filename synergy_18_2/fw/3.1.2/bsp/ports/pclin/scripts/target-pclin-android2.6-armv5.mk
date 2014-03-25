###############################################################################
#
#                (c) Cambridge Silicon Radio Limited 2011
#
#                All rights reserved and confidential information of CSR
#
###############################################################################

# Compiler prefix for selecting Android ARM compiler
COMPILER_PREFIX := android-arm-

include $(TARGET_SCRIPT_ROOT)/target-default.mk

# Override target libraries specified in target-default.mk
TARGET_LIBS :=
