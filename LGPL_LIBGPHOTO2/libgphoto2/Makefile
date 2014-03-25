##############################################
#Makefile for PTP Driver Lib
##############################################
.SUFFIXES = .c .o 

##############################################
#THREAD_SAFE =n    # ( it will compile for libusb 0.1 ver )
#THREAD_SAFE =y    # ( it will compile for libusb 1.0 ver )
export THREAD_SAFE=y
#############################################
SHARED_LIB = y
COMPILE_OPT = y

OPENSRC_DEF_CHIP=$(TARGET)
ifeq ($(origin OPENSRC_DEF_CHIP), undefined)
exit
endif
##########################################################
##########################################################
ifeq ($(OPENSRC_DEF_CHIP), undefined)
	CC = g++
	AR = ar
	RANLIB = ranlib
	PTP_DEFINES += PTP_PC_COMPILE
	PTP_DEFINES += NDEBUG
else
	CC = $(CROSS_COMPILE)g++
	CXX = $(CROSS_COMPILE)g++
	AR = $(CROSS_COMPILE)ar
	RANLIB = $(CROSS_COMPILE)ranlib
	LD = $(CROSS_COMPILE)ld
	PTP_DEFINES += NDEBUG
endif

INC =  ./Inc

ifeq ($(THREAD_SAFE), y)
PTP_DEFINES += PTP_THREAD_SAFE
PTP_DEFINES += NEW_CONFIG
endif
##########################################################
# Setting Platform Type
ifeq ($(OPENSRC_DEF_CHIP), FOXP)
	PTP_DEFINES += NO_INCLUDE_GPHOTO2_ENDIAN
endif
ifeq ($(OPENSRC_DEF_CHIP), X13)
 	ADD_COMPILE_OPT += -EL -msoft-float -march=mips32r2 -mmemcpy
 	ADD_COMPILE_OPT2 += -EL -msoft-float -march=mips32r2 -mmemcpy
 	ADD_LINK_OPT += -EL -msoft-float -march=mips32r2 -mmemcpy
endif
ifeq ($(OPENSRC_DEF_CHIP), X12)
	PTP_DEFINES += NO_INCLUDE_GPHOTO2_ENDIAN
 	ADD_COMPILE_OPT +=  -Wno-ignored-qualifiers -Wno-array-bounds -Wno-empty-body
 	ADD_COMPILE_OPT2+= -Wno-ignored-qualifiers -Wno-c++0x-compat -Wno-array-bounds -Wno-empty-body
endif
ifeq ($(OPENSRC_DEF_CHIP), FOXB)
        PTP_DEFINES += NO_INCLUDE_GPHOTO2_ENDIAN
        ADD_COMPILE_OPT +=  -Wno-ignored-qualifiers -Wno-array-bounds -Wno-empty-body
        ADD_COMPILE_OPT2+= -Wno-ignored-qualifiers -Wno-c++0x-compat -Wno-array-bounds -Wno-empty-body
endif
ifeq ($(OPENSRC_DEF_CHIP), NVT569)
 	ADD_COMPILE_OPT +=  -march=mips32r2 -mmemcpy -fstrict-aliasing  -EL  -msoft-float  -Wno-ignored-qualifiers -Wno-array-bounds -Wno-empty-body  -Wswitch-default
 	ADD_COMPILE_OPT2 +=  -march=mips32r2 -mmemcpy -fstrict-aliasing  -EL  -msoft-float  -Wno-ignored-qualifiers -Wno-array-bounds -Wno-empty-body  -Wswitch-default
endif
##########################################################
##########################################################

DEFINES += $(addprefix -D, $(PTP_DEFINES)) 

ifeq ($(COMPILE_OPT), y)
CFLAGS = -fPIC -Wall -Wextra -Wcast-align -Wno-unused-parameter -Wshadow \
	  	 -Wwrite-strings -Wcast-qual \
		 -fno-strict-aliasing -fsigned-char -O2\
		 -ffunction-sections -fdata-sections\
		 -fno-common -fno-omit-frame-pointer -fno-optimize-sibling-calls\
		  -fshort-wchar -fstrict-overflow\
		  -Werror $(ADD_COMPILE_OPT)\
		  -I $(INC) $(DEFINES) $(TOOLCHAIN_CFLAGS)
CPPFLAGS = -fPIC -Wall -Wextra -Wcast-align -Wno-unused-parameter -Wshadow \
		 -Wwrite-strings -Wcast-qual \
		 -fno-strict-aliasing -fsigned-char -O2\
		 -ffunction-sections -fdata-sections\
		 -fno-common -fno-omit-frame-pointer -fno-optimize-sibling-calls\
		  -fshort-wchar -fstrict-overflow\
		  -Werror $(ADD_COMPILE_OPT2)\
		  -I $(INC) $(DEFINES) -D_LINUX  $(TOOLCHAIN_CFLAGS)
endif
ifeq ($(COMPILE_OPT), n)
CFLAGS = -c -O2 -Wall -Wno-write-strings -fPIC -I $(INC) $(ADD_COMPILE_OPT) $(DEFINES)
CPPFLAGS = -c -O2 -Wall -fPIC -I $(INC) -D_LINUX $(ADD_COMPILE_OPT2) $(DEFINES)
endif

##########################################################
##########################################################
ifeq ($(THREAD_SAFE), n)
OBJS = PTPDeviceListUpdate.o  ptp.o ptpapi.o
SRCS = PTPDeviceListUpdate.cpp  ptp.c ptpapi.c
else
OBJS = PTPDeviceListUpdate.o PtpDeviceAccess.o ptp.o ptpapi.o PtpCallback.o
SRCS = PTPDeviceListUpdate.c PtpDeviceAccess.c ptp.c ptpapi.c PtpCallback.c
endif
ifeq ($(SHARED_LIB), y)
TARGET = libptp.so
$(TARGET) : $(OBJS)
	$(AR) rcv $@ $(OBJS)
	$(RANLIB) $@
	$(CC) -shared -Wl,-soname,$@ -o $@ $(OBJS) $(ADD_LINK_OPT)
#	$(LD) -o $@	$(OBJS)
endif

##########################################################
##########################################################

all : 	$(TARGET)
dep :
	gccmakedep $(INC) $(SRCS)
clean :
	rm -rf $(OBJS) $(TARGET) core
