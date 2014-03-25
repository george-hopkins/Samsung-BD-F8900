###############################################################################
#
#                (c) Cambridge Silicon Radio Limited 2010
#
#                All rights reserved and confidential information of CSR
#
###############################################################################

# Default Compiler
COMPILER ?= VC8



ifeq ($(COMPILER),VC8)
TOOLCHAIN_PATH := "C:\Program Files\Microsoft Visual Studio 8\VC\ce\bin\x86_mips"
export LIB
export INCLUDE
export PATH
#LIB := c:\Program Files\Microsoft Visual Studio 8\VC\lib;C:\Program Files\Microsoft Visual Studio 8\VC\PlatformSDK\Lib
#INCLUDE := c:\Program Files\Microsoft Visual Studio 8\VC\include;C:\Program Files\Microsoft Visual Studio 8\VC\PlatformSDK\Include
LIB := c:\Program Files\Microsoft Visual Studio 8\VC\ce\lib\mipsii;C:\Program Files\Windows CE Tools\wce600\GEN3_AU1350\Lib\MIPSII
INCLUDE := C:\Program Files\Windows CE Tools\wce600\GEN3_AU1350\Include\Mipsii
PATH := /cygdrive/c/Program Files/Microsoft Visual Studio 8/Common7/IDE:$(PATH)
endif


ifeq ($(COMPILER),VC9)
TOOLCHAIN_PATH := "C:\Program Files\Microsoft Visual Studio 9.0\VC\ce\bin\x86_mips"
export LIB
export INCLUDE
export PATH
LIB := C:\Program Files\Windows CE Tools\wce600\GEN3_AU1350\Lib\MIPSII
INCLUDE := C:\Program Files\Windows CE Tools\wce600\GEN3_AU1350\Include\Mipsii
PATH := /cygdrive/c/Program Files/Microsoft Visual Studio 9.0/Common7/IDE:$(PATH)
endif


ifneq ($(TOOLCHAIN_PATH),)
CC := $(TOOLCHAIN_PATH)/cl
AR := $(TOOLCHAIN_PATH)/lib
LD := $(TOOLCHAIN_PATH)/link
CXX := $(TOOLCHAIN_PATH)/cl
endif

TARGET_LIBS := \
    ole32 \
    oleaut32 \
    uuid \
    ws2 \
    winsock \
    coredll \
    corelibc \
    commctrl
#    advapi32 \
#    comdlg32 \
#    gdi32 \
#    kernel32 \
#    odbc32 \
#    odbccp32 \
#    shell32 \
#    user32 \
#    winmm \
#    winspool \
 #   setupapi


# Common Options
TARGET_CFLAGS += /nologo /Zm1000
LDFLAGS += /nologo /incremental:no /fixed:no  /subsystem:windowsce,6.00 /machine:MIPS

ifneq ($(NO_TREAT_WARN_AS_ERROR),1)
TARGET_CFLAGS += /WX
endif

# Disables non-ANSI Microsoft extensions
ifeq ($(FORCE_ANSI),1)
TARGET_CFLAGS += /Za
endif


#TARGET_CFLAGS += /W3 /GS /D_CRT_SECURE_NO_WARNINGS /D_CRT_SECURE_NO_DEPRECATE /D_CRT_NONSTDC_NO_DEPRECATE
ifeq ($(DEBUG),1)
TARGET_CFLAGS += /Zi /MTd
LDFLAGS += /debug
else
TARGET_CFLAGS += /MT
endif

TARGET_CFLAGS +=  /QMmips2 /TC

TARGET_CFLAGS +=  /D_WIN32_WCE=$(CEVER) /DUNDER_CE /D$(PLATFORMDEFINES) /DWINCE /D_WINDOWS /D$(ARCHFAM) /D$(_ARCHFAM_) /D_UNICODE /DUNICODE
#TARGET_CFLAGS += /DSTANDARDSHELL_UI_MODEL
TARGET_CXXFLAGS += $(TARGET_CFLAGS)
EXTRA_CXXFLAGS += $(EXTRA_CFLAGS)
CXXFLAGS += $(CFLAGS)

MODULE_TEST_MAIN ?= $(FW_ROOT)/test/module_test/csr_module_test_main.c

BSP_SUPPORT_ACLBUF := 1

#
# Macro definitions
#

# $(1) = name
lib_name = $(1).lib

# $(1) = name
bin_name = $(1).exe

# $(1) = name
# $(2) = OBJ
create_lib = $(AR) /nologo /out:$(1) $(2)

# $(1) = name
# $(2) = OBJ
# $(3) = LDPATH
# $(4) = LIBS
create_bin = $(LD) /out:$(1) $(2) $(EXTRA_LDFLAGS) $(LDFLAGS) $(3) $(4)
create_exec = $(LD) /out:$(1) $(2) $(EXTRA_LDFLAGS) $(LDFLAGS) $(3) $(4)

# Clean up after create_exec
#
# $(1) = Output file name (with path)
#
clean_exec = $(RM) $(1)

# $(1) = list of libs path.
create_ldpath = $(shell echo $(addprefix /libpath:,$(shell echo $(1) | sed -e 's/\ /*/g')) | sed -e 's/\*/\\ /g')

# $(1) = list of libs.
use_lib = $(addsuffix .lib,$(1))

# $(1) = relative input file
ifeq ($(USE_ABSOLUTE_PATH),1)
abs_path = "$(shell cygpath -a -m $(1))"
else
abs_path = "$(1)"
endif

#
# Rules
#
$(OBJ_PATH)/%.o:%.c
	$(MKDIR) $(@D)
	$(CC) $(EXTRA_CFLAGS) $(TARGET_CFLAGS) $(CFLAGS) $(INC) -c -Fo$@ $(call abs_path,$<)

$(OBJ_PATH)/%.o:%.cpp
	$(MKDIR) $(@D)
	$(CXX) $(EXTRA_CXXFLAGS) $(TARGET_CXXFLAGS) $(CXXFLAGS) $(INC) -c -Fo$@ $(call abs_path,$<)
