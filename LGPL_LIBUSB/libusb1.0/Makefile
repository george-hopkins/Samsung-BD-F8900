##############################################
#Makefile for Common USB Driver library (libusb1.0) 
##############################################
.SUFFIXES = .c .o 

SHARED_LIB = y
COMPILE_OPT = y

OPENSRC_DEF_CHIP=$(TARGET)

##########################################################
##########################################################
ifeq ($(OPENSRC_DEF_CHIP), undefined)
	CC = gcc
	AR = ar
	RANLIB = ranlib
else
	CC = $(CROSS_COMPILE)gcc
	AR = $(CROSS_COMPILE)ar
	RANLIB = $(CROSS_COMPILE)ranlib
endif

INC =  ./Inc

PTP_DEFINES += HOST_ENDIAN
PTP_DEFINES += _GNU_SOURCE
PTP_DEFINES += SISC_MEMLEAK_FLAG
 
##########################################################
##########################################################
# Setting Platform Type
ifeq ($(OPENSRC_DEF_CHIP), FOXP)
	PTP_DEFINES += NO_INCLUDE_GPHOTO2_ENDIAN
 	ADD_COMPILE_OPT +=  -Wno-ignored-qualifiers -Wno-array-bounds -Wno-empty-body -Wcast-qual
endif
ifeq ($(OPENSRC_DEF_CHIP), X13)
 	ADD_COMPILE_OPT +=  -march=mips32r2 -mmemcpy
 	ADD_LINK_OPT +=  -march=mips32r2 -mmemcpy
endif
ifeq ($(OPENSRC_DEF_CHIP), X12)
	PTP_DEFINES += NO_INCLUDE_GPHOTO2_ENDIAN
 	ADD_COMPILE_OPT +=  -Wno-ignored-qualifiers -Wno-array-bounds -Wno-empty-body -Wcast-qual
endif
ifeq ($(OPENSRC_DEF_CHIP), FOXB)
        PTP_DEFINES += NO_INCLUDE_GPHOTO2_ENDIAN
        ADD_COMPILE_OPT +=  -Wno-ignored-qualifiers -Wno-array-bounds -Wno-empty-body -Wcast-qual
endif
ifeq ($(OPENSRC_DEF_CHIP), NVT569)
 	ADD_COMPILE_OPT +=  -march=mips32r2 -mmemcpy -fstrict-aliasing  -EL  -msoft-float  -Wno-ignored-qualifiers -Wno-array-bounds -Wno-empty-body  -Wswitch-default
endif
##########################################################
##########################################################

DEFINES += $(addprefix -D, $(PTP_DEFINES))

ifeq ($(COMPILE_OPT), n)
CFLAGS = -c -O2 -Wall -Wno-write-strings -fPIC -I $(INC)
CPPFLAGS = -c -O2 -Wall -fPIC -I $(INC) -D_LINUX
endif
ifeq ($(COMPILE_OPT), y)
#delete "-fstack-protector-all"
CFLAGS = -fPIC -Wall -Wextra -Wno-unused-parameter -Wshadow \
		 -fno-strict-aliasing -fstrict-overflow -fsigned-char -O2 \
		  -ffunction-sections -fdata-sections \
  		 -fno-common -fno-omit-frame-pointer -fno-optimize-sibling-calls \
		  -fshort-wchar -fstrict-overflow \
		  -Werror $(ADD_COMPILE_OPT) \
		 -I $(INC) $(DEFINES) -D_LINUX  $(TOOLCHAIN_CFLAGS)
endif
##########################################################
##########################################################
#OBJS = descriptors.o error.o linux.o usb.o
#SRCS = descriptors.c error.c linux.c usb.c
OBJS = descriptor.o core.o io.o sync.o linux_usbfs.o
SRCS = descriptor.c core.c io.c sync.c linux_usbfs.c

ifeq ($(SHARED_LIB), y)
TARGET = libusb.so
$(TARGET) : $(OBJS)
	$(CC) -shared -Wl,-soname,$@ -o $@ $(OBJS) $(ADD_LINK_OPT)
else
TARGET = libusb.a
$(TARGET) : $(OBJS)
	$(AR) rcv $@ $(OBJS)
	$(RANLIB) $@
endif

##########################################################
##########################################################
all : 	$(TARGET)

dep :
	gccmakedep $(INC) $(SRCS)
clean :
	rm -rf $(OBJS) $(TARGET) core
