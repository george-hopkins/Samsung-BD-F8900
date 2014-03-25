FW_ROOT := ../../../..
OBJ_PATH = $(FW_ROOT)/output/$(CONFIG)/$(TARGET)/obj/gsp/src/application/sdio_probe
include $(FW_ROOT)/local_rules.mk

LIB_NAME := $(call lib_name,csr_sdio_probe)
LIB_PATH := $(FW_LIB)

INC += -I../../inc

SRC := \
        csr_sdio_probe_handler.c \
        csr_sdio_probe_sef.c \
        csr_sdio_probe_test.c

OBJ := $(addprefix $(OBJ_PATH)/,$(call path_subst,$(SRC:.c=.o)))

DEP := $(OBJ:.o=.d)
include $(FW_ROOT)/m_targets.mk

all: lib

bin:

lib: $(LIB_PATH)/$(LIB_NAME)

$(LIB_PATH)/$(LIB_NAME): $(OBJ)
	$(MKDIR) $(@D)
	$(call create_lib,$@,$(OBJ))

clean:
	$(RM) $(call path_subst,$(LIB_PATH)/$(LIB_NAME))
	$(RM) $(call path_subst,$(OBJ))
