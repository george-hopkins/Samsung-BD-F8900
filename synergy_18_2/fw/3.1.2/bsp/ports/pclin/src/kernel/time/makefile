FW_ROOT := ../../../../../..
OBJ_PATH = $(BSP_ROOT)/output/$(CONFIG)/$(TARGET)/obj/src/kernel/time
include $(FW_ROOT)/local_rules.mk

LIB_NAME := $(call lib_name,csr_time)
LIB_PATH := $(BSP_LIB)

SRC := \
	csr_time.c 

OBJ := $(addprefix $(OBJ_PATH)/,$(SRC:.c=.o))

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
	$(RM) $(LIB_PATH)/$(LIB_NAME)
	$(RM) $(OBJ)
