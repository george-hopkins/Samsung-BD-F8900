FW_ROOT := ../../../../..
OBJ_PATH = $(BSP_ROOT)/output/$(CONFIG)/$(TARGET)/obj/src/ether
include $(FW_ROOT)/local_rules.mk

LIB_NAME := $(call lib_name,csr_ether)
LIB_PATH := $(BSP_LIB)

INC += -I../inc

SRC := csr_ethernet_handler.c

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
	$(RM) *~
	$(RM) $(call path_subst,$(LIB_PATH)/$(LIB_NAME))
	$(RM) $(call path_subst,$(OBJ))
