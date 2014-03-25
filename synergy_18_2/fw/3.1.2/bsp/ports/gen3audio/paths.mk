###############################################################################
#
#                (c) Cambridge Silicon Radio Limited 2010
#
#                All rights reserved and confidential information of CSR
#
###############################################################################

#
# NOTE: inc/platform/ is deliberately NOT in the default
#       search list in order for people to explicitly
#       prefix "platform/" to their platform dependant
#       include files.  The goal is to prevent generic
#       code (e.g. BT) from accidentally pulling in
#       platform specific code.
#       Do NOT readd it.
# 

BSP_INC := \
    -I$(FW_ROOT)/bsp/inc \
    -I$(BSP_ROOT)/inc \
    -I$(NUCLEUS_ROOT)/inc

BSP_LIB := $(BSP_ROOT)/output/$(FW_CONFIG)/$(TARGET)/lib

BSP_LIBS := \
    csr_util \
    csr_pmem \
    csr_time \
    csr_eh \
    csr_panic \
    csr_fs \
    csr_console \
    csr_ether \
    csr_low_level_drivers \
    csr_sdio_master \
    csr_spi_master \
    csr_ser_com \
    csr_aclbuf \
    csr_membuf \
    csr_framework_ext \
    csr_logtrans \
    $(TARGET_LIBS)
