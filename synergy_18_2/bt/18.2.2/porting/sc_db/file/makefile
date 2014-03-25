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

OBJ_PATH = $(TOP)/output/$(CONFIG)/$(TARGET)/obj/porting/sc_db/file

include $(TOP)/local_rules.mk

MODULE_NAME := sc_db-file

LIB_NAME := $(call lib_name,csr_bt_$(MODULE_NAME))
LIB_PATH := $(TOP)/output/$(CONFIG)/$(TARGET)/lib

INC += -I. -I../inc -I../../inc -I$(BSP_ROOT)/inc -I$(TOP)/inc 

SRC += csr_bt_sc_db-file.c 

OBJ := $(addprefix $(OBJ_PATH)/,$(SRC:.c=.o))
DEP := $(OBJ:.o=.d) $(OBJ_COMP:.o=.d)

include $(FW_ROOT)/m_targets.mk

all: lib 

bin:

lib: create_dirs $(LIB_PATH)/$(LIB_NAME) 

$(LIB_PATH)/$(LIB_NAME): $(OBJ) 
	$(call create_lib,$@,$(OBJ))

create_dirs: $(LIB_PATH) $(OBJ_PATH)

$(LIB_PATH):
	$(MKDIR) $(call path_subst,$(LIB_PATH))

$(OBJ_PATH):
	$(MKDIR) $(call path_subst,$(OBJ_PATH))

clean:
	$(RM) *~
	$(RM) $(call path_subst,$(LIB_PATH)/$(LIB_NAME))
	$(RM) $(call path_subst,$(OBJ))

