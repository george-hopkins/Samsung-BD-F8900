#
# Makefile for MStar eMMC host drivers.
#

# general options
EXTRA_CFLAGS		+= -Idrivers/mstar/mmc/
EXTRA_CFLAGS		+= -Idrivers/mstar/mmc/eMMCDrv/inc/api/
EXTRA_CFLAGS		+= -Idrivers/mstar/mmc/eMMCDrv/inc/common/
EXTRA_CFLAGS		+= -Idrivers/mstar/mmc/eMMCDrv/inc/config/
#EXTRA_CFLAGS		+= -Werror

# specific options


# files
obj-y               += mstar_mci.o 
obj-y	            += eMMCDrv/src/common/eMMC_hal.o
obj-y	            += eMMCDrv/src/config/eMMC_platform.o

