###############################################################################
#
#                (c) Cambridge Silicon Radio Limited 2010
#
#                All rights reserved and confidential information of CSR
#
###############################################################################

CC := cl
AR := lib
LD := LINK
CXX := $(CC)

ifeq ($(_BUILDARCH),AMD64)
LDFLAGS += /nologo /machine:AMD64 /subsystem:console /incremental:no /pdb:$(1).pdb
TARGET_CFLAGS += -nologo /Zm1000 -Iamd64\ -I. -I -I"\WINDDK\3790~1.183\inc\mfc42" -I"objchk_wnet_AMD64\amd64" -I"C:\WINDDK\3790~1.183\inc\wnet" -I"C:\WINDDK\3790~1.183\inc\wnet" -I"C:\WINDDK\3790~1.183\inc\ddk\wnet" -I"C:\WINDDK\3790~1.183\inc\ddk\wdm\wnet" -I"C:\WINDDK\3790~1.183\inc\crt" -D_WIN64 -D_AMD64_ -DAMD64 -DCONDITION_HANDLING=1 -DNT_INST=0 -DWIN32=100 -D_NT1X_=100 -DWINNT=1 -D_WIN32_WINNT=0x0502 /DWINVER=0x0502 -D_WIN32_IE=0x0603 -DWIN32_LEAN_AND_MEAN=1 -D_AMD64_SIMULATOR_PERF_ -D_SKIP_IF_SIMULATOR_ -D_AMD64_SIMULATOR_ -D_AMD64_WORKAROUND_ -DDEVL=1 -DDBG=1 -D__BUILDMACHINE__=WinDDK -DNDEBUG -D_DLL=1 /c /Zl /Zp8 /Gy -cbstring /W3 /Wp64 /EHs-c- /GR- /GF /GS -Z7 /Od /Oi  -Z7   -d2home -FI"C:\WINDDK\3790~1.183\inc\wnet\warning.h"
else
LDFLAGS += /nologo /machine:i386 /subsystem:console /incremental:no /pdb:$(1).pdb
TARGET_CFLAGS += -nologo /Zm1000 -Ii386\ -I. -I"$(MFC_INCLUDES)" -I"objchk_wxp_x86\i386" -I"$(INCLUDE)" -I"$(DDK_INC_PATH)" -I"$(WDM_INC_PATH)" -I"$(CRT_INC_PATH)" -D_X86_=1 -Di386=1  -DSTD_CALL -DCONDITION_HANDLING=1   -DNT_INST=0 -DWIN32=100 -D_NT1X_=100 -DWINNT=1 -D_WIN32_WINNT=0x0502 /DWINVER=0x0502 -D_WIN32_IE=0x0603 -DWIN32_LEAN_AND_MEAN=1 -DDEVL=1 -DDBG=1 -D__BUILDMACHINE__=WinDDK -DFPO=0  -DNDEBUG -D_DLL=1  /c /Zl /Zp8 /Gy /Gm- -cbstring /W3 /Gz  /GX-  /GR- /GF /GS /G6 /Ze /Gi- /QIfdiv- /hotpatch -Z7 /Od /Oi  /Oy-   -FI"$(INCLUDE)\warning.h"
endif

ifneq ($(NO_TREAT_WARN_AS_ERROR),1)
TARGET_CFLAGS += /WX
endif

ifeq ($(DEBUG),1)
TARGET_CFLAGS +=  /Od -D_DEBUG /MTd /Z7
LDFLAGS += /debug 
else
TARGET_CFLAGS +=  /MT
endif

ifeq ($(CXX),1)
TARGET_CFLAGS +=  /TP
else
TARGET_CFLAGS +=  /TC
endif

TARGET_CXXFLAGS += $(TARGET_CFLAGS)
EXTRA_CXXFLAGS += $(EXTRA_CFLAGS)
CXXFLAGS += $(CFLAGS)

# $(1) = name
lib_name = $(1).lib

# $(1) = name
# $(2) = OBJ
create_lib = $(AR) /out:$(1) $(2)

# $(1) = name
# $(2) = OBJ
# $(3) = LDPATH
# $(4) = LIBS
create_bin = $(LD) /out:$(1) $(2) $(LDFLAGS) $(3) $(4)
create_exec = $(LD) /out:$(1) $(2) $(LDFLAGS) $(3) $(4)

# Clean up after create_exec
#
# $(1) = Output file name (with path)
#
clean_exec = $(RM) $(1)

#
# Rules
#
$(OBJ_PATH)/%.o:%.c
	$(MKDIR) $(@D)
	$(CC) $(EXTRA_CFLAGS) $(TARGET_CFLAGS) $(CFLAGS) $(INC) -c -Fo$@ $<

$(OBJ_PATH)/%.o:%.cpp
	$(MKDIR) $(@D)
	$(CXX) $(EXTRA_CXXFLAGS) $(TARGET_CXXFLAGS) $(CXXFLAGS) $(INC) -c -Fo$@ $<

