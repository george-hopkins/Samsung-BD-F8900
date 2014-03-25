###############################################################################
#
#   Copyright Cambridge Silicon Radio Limited and its licensors 2011-2012. 
#
#   All rights reserved. 
#
# REVISION:      $Revision$
###############################################################################
#
# Enable to compile instrumented profiling service into the Framework and BT
#
CSR_INSTRUMENTED_PROFILING_SERVICE := 1
CFLAGS += -DCSR_INSTRUMENTED_PROFILING_SERVICE

ifeq ($(CSR_BT_TESTER_USE_WRITE_THROUGH_CACHE),1)
CFLAGS += -DCSR_BT_TESTER_USE_WRITE_THROUGH_CACHE
endif

NUCLEUS_CONFIG := ips

#
# Enable to compile memory allocation profiling service into the Framework and BT
#
CSR_MEMALLOC_PROFILING  := 1
CFLAGS += -DCSR_MEMALLOC_PROFILING


####### Framework config
FW_CONFIG := ips



ifneq ($(TARGET),bdb3-nucleus-arm6kz)
    include $(TOP)/config/config-tester.mk
else
    include $(TOP)/config/config-tester-bdb.mk
endif
