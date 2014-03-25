##############################################################################
#
#           (c) Cambridge Silicon Radio Limited 2010
#
#           All rights reserved and confidential information of CSR
#
##############################################################################
FW_ROOT := ../../../../..
OBJ_PATH = $(FW_ROOT)/output/$(CONFIG)/$(TARGET)/obj/gsp/src/bluecore/transport/sdio
include $(FW_ROOT)/local_rules.mk

LIB_NAME := $(call lib_name,csr_sdio)
LIB_PATH := $(FW_LIB)

SRC := \
    csr_sdio.c \
    csr_sdio_sdio.c \
    csr_sdio_cspi.c

OBJ := $(addprefix $(OBJ_PATH)/,$(call path_subst,$(SRC:.c=.o)))

DEP := $(OBJ:.o=.d)
include $(FW_ROOT)/m_targets.mk

all: lib

lib: $(LIB_PATH)/$(LIB_NAME)

$(LIB_PATH)/$(LIB_NAME): $(OBJ)
	$(MKDIR) $(@D)
	$(call create_lib,$@,$(OBJ))

clean:
	$(RM) $(call path_subst,$(LIB_PATH)/$(LIB_NAME))
	$(RM) $(call path_subst,$(OBJ))
