#-*-Makefile-*-
###############################################################################
#
#                (c) Cambridge Silicon Radio Limited 2009
#
#                All rights reserved and confidential information of CSR
#
# REVISION:      $Revision: #1 $
###############################################################################
TOP := ../../..

OBJ_PATH = $(TOP)/output/$(CONFIG)/$(TARGET)/obj/porting/sbc/code/$(TARGET)

include $(TOP)/local_rules.mk

MODULE_NAME := sbc

ifneq ($(filter bdb2 bdb3 gen3audio,$(TARGET_PLATFORM)),)
ifeq ($(TARGET_PLATFORM),bdb3)
LIB_NAME_ARM11 := $(call lib_name,csr_$(MODULE_NAME)_arm11)
endif
LIB_NAME_ARM9_5TE := $(call lib_name,csr_$(MODULE_NAME)_comp)
LIB_NAME_ARM9_5T := $(call lib_name,csr_$(MODULE_NAME))
else
LIB_NAME_OTHER := $(call lib_name,csr_$(MODULE_NAME))
endif

LIB_PATH := $(TOP)/output/$(CONFIG)/$(TARGET)/lib

INC += -I. -I../inc -I../../inc -I$(BSP_ROOT)/inc -I$(TOP)/inc

SRC_ARM11 :=    csr_polyphase_arm11.c \
		csr_polyphase_synthesis_arm11.c csr_sbc_arm11.c \
		csr_sbc_dec_arm11.c csr_sbc_enc_arm11.c

ASM_ARM11 := 	csr_memmove_arm11.s csr_polyphase_analysis_arm11.s

SRC_ARM9_5TE := csr_polyphase.c \
		csr_polyphase_synthesis.c csr_sbc.c csr_sbc_dec.c \
		csr_sbc_enc.c csr_sbc_memmove.c

ASM_ARM9_5TE := csr_polyphase_analysis_arm9e_comp.s

SRC_ARM9_5T  := csr_polyphase.c \
		csr_polyphase_synthesis.c csr_sbc.c csr_sbc_dec.c \
		csr_sbc_enc.c csr_sbc_memmove.c

ASM_ARM9_5T  := csr_polyphase_analysis_arm9.s

SRC_OTHER    := csr_polyphase.c csr_polyphase_analysis.c \
		csr_polyphase_synthesis.c csr_sbc.c csr_sbc_dec.c \
		csr_sbc_enc.c csr_sbc_memmove.c

OBJ_SRC_ARM11 := $(addprefix $(OBJ_PATH)/,$(SRC_ARM11:.c=.o))
OBJ_ASM_ARM11 := $(addprefix $(OBJ_PATH)/,$(ASM_ARM11:.s=.o))
OBJ_ARM11 := $(OBJ_SRC_ARM11) $(OBJ_ASM_ARM11)

OBJ_SRC_ARM9_5TE := $(addprefix $(OBJ_PATH)/,$(SRC_ARM9_5TE:.c=.o))
OBJ_ASM_ARM9_5TE := $(addprefix $(OBJ_PATH)/,$(ASM_ARM9_5TE:.s=.o))
OBJ_ARM9_5TE := $(OBJ_SRC_ARM9_5TE) $(OBJ_ASM_ARM9_5TE)

OBJ_SRC_ARM9_5T := $(addprefix $(OBJ_PATH)/,$(SRC_ARM9_5T:.c=.o))
OBJ_ASM_ARM9_5T := $(addprefix $(OBJ_PATH)/,$(ASM_ARM9_5T:.s=.o))
OBJ_ARM9_5T := $(OBJ_SRC_ARM9_5T) $(OBJ_ASM_ARM9_5T)

OBJ_SRC_OTHER := $(addprefix $(OBJ_PATH)/,$(SRC_OTHER:.c=.o))
OBJ_OTHER := $(OBJ_SRC_OTHER)

ifneq ($(filter bdb2 bdb3 gen3audio,$(TARGET_PLATFORM)),)
ifeq ($(TARGET_PLATFORM),bdb3)
DEP := $(OBJ_ARM11:.o=.d) 
endif

DEP+= $(OBJ_ARM9_5TE:.o=.d) $(OBJ_ARM9_5T:.o=.d)
lib all: lib_arm
else
DEP := $(OBJ_OTHER:.o=.d)
lib all: lib_other
endif

include $(FW_ROOT)/m_targets.mk

lib_arm: 
ifeq ($(TARGET_PLATFORM),bdb3)
	$(MAKE) lib_arm11 
endif
	$(MAKE) lib_arm9_5te 
	$(MAKE) lib_arm9_5t

lib_arm11: create_dirs $(LIB_PATH)/$(LIB_NAME_ARM11) 
lib_arm9_5te: create_dirs $(LIB_PATH)/$(LIB_NAME_ARM9_5TE) 
lib_arm9_5t: create_dirs $(LIB_PATH)/$(LIB_NAME_ARM9_5T) 
lib_other: create_dirs $(LIB_PATH)/$(LIB_NAME_OTHER) 

bin:

ifneq ($(filter bdb2 bdb3 gen3audio,$(TARGET_PLATFORM)),)
ifeq ($(TARGET_PLATFORM),bdb3)
$(LIB_PATH)/$(LIB_NAME_ARM11): $(OBJ_ARM11) 
	$(call create_lib,$@,$^)
endif
$(LIB_PATH)/$(LIB_NAME_ARM9_5TE): $(OBJ_ARM9_5TE) 
	$(call create_lib,$@,$^)

$(LIB_PATH)/$(LIB_NAME_ARM9_5T): $(OBJ_ARM9_5T) 
	$(call create_lib,$@,$^)
else
$(LIB_PATH)/$(LIB_NAME_OTHER): $(OBJ_OTHER) 
	$(call create_lib,$@,$^)
endif

create_dirs: $(LIB_PATH) $(OBJ_PATH)

$(LIB_PATH):
	$(MKDIR) $(call path_subst,$(LIB_PATH))

$(OBJ_PATH):
	$(MKDIR) $(call path_subst,$(OBJ_PATH))

clean:
	$(RM) *~
ifneq ($(filter bdb2 bdb3 gen3audio,$(TARGET_PLATFORM)),)
ifeq ($(TARGET_PLATFORM),bdb3)
	$(RM) -r $(call path_subst,$(LIB_PATH)/$(LIB_NAME_ARM11))
	$(RM) $(call path_subst,$(OBJ_ARM11))
endif
	$(RM) -r $(call path_subst,$(LIB_PATH)/$(LIB_NAME_ARM9_5TE))
	$(RM) $(call path_subst,$(OBJ_ARM9_5TE))

	$(RM) -r $(call path_subst,$(LIB_PATH)/$(LIB_NAME_ARM9_5T))
	$(RM) $(call path_subst,$(OBJ_ARM9_5T))
else
	$(RM) -r $(call path_subst,$(LIB_PATH)/$(LIB_NAME_OTHER))
	$(RM) $(call path_subst,$(OBJ_OTHER))
endif

