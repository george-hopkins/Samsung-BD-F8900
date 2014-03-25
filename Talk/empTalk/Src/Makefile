# Last update date : Aug 13, 2011
# Author : Sehyun Bae
# Revised : hikaru

App=Talk

########################################################################
# Directories
########################################################################

SEFDIR?=../../..
SrcDir=$(SEFDIR)/Src
IncDir=$(SEFDIR)/Inc
EmpAppIncDir=$(IncDir)/EmpApp

ProgInstallDir=$(SEFDIR)/Exe/$(TARGET)/emp$(App)
ProgName=emp$(App)

include $(SEFDIR)/Build/config.mak

########################################################################
# Configuration section
########################################################################

#OPENSSL_DIR=$(OPENSOURCE_DIR)/Connectivity/Protocol/OpenSSL/0.9.8o
#ExtraObjs=$(OPENSSL_DIR)/Lib/$(TARGET)/rel/libssl.a $(OPENSSL_DIR)/Lib/$(TARGET)/rel/libcrypto.a


GlooxVersion=0.9.9.12

ExtraIncDirs+=$(OPENSOURCE_DIR)/Connectivity/Protocol/Gloox/$(GlooxVersion)/Inc/gloox
ExtraIncDirs+=$(OPENSOURCE_DIR)/Connectivity/Protocol/OpenSSL/0.9.8o/Inc

ExtraLibDirs+=$(OPENSOURCE_DIR)/Connectivity/Protocol/Gloox/$(GlooxVersion)/Lib/$(TARGET)/rel
ExtraLibDirs+=$(OPENSOURCE_DIR)/Connectivity/Protocol/OpenSSL/0.9.8o/Lib/$(TARGET)/rel

# Define your flags
ExtraDefs+=_TARGET

ExtraIncDirs+=$(SP_DIR)/BP_SP/CSP/1.1/Inc
ExtraIncDirs+=$(BP_MW_DIR)/BP_CommSS/Inc
ExtraIncDirs+=$(OPENSSL_DIR)/Inc


CFLAGS+=


ExtraLibDirs+=$(SP_DIR)/BP_SP/CSP/1.1/Lib/$(TARGET)/rel
ExtraLibs+=CoreSWPlatform dl resolv gloox ssl crypto pthread

#########################################################################
# Program building
#########################################################################


SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)



include $(SEFDIR)/Build/common.mak

include $(SEFDIR)/Src/EmpApp/empConfig.mak
