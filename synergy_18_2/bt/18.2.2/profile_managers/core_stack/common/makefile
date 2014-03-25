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
OBJ_PATH = $(TOP)/output/$(CONFIG)/$(TARGET)/obj/src/cs/$(HOSTBUILD)/common
include $(TOP)/local_rules.mk

INC += \
	-I$(TOP)/inc \
	-I$(TOP)/profile_managers/inc \
	-I../hci/inc

SRC := $(shell $(FIND) bkeyval -name \*.c)
SRC += $(shell $(FIND) bluestack -name \*.c)
SRC += $(shell $(FIND) pmalloc -name \*.c)
SRC += $(shell $(FIND) proxy -name \*.c)

OBJ := $(addprefix $(OBJ_PATH)/,$(call path_subst,$(SRC:.c=.o)))
DEP := $(OBJ:.o=.d)
include $(FW_ROOT)/m_targets.mk

all: lib

bin:

lib: $(OBJ)

clean:
	$(RM) $(call path_subst,$(OBJ))
