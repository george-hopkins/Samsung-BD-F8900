FW_ROOT := ../../../../..
OBJ_PATH = $(FW_ROOT)/output/$(CONFIG)/$(TARGET)/obj/gsp/src/log/pcap
include $(FW_ROOT)/local_rules.mk

LIB_NAME := $(call lib_name,csr_log_pcap)
LIB_PATH := $(FW_LIB)

INC += -I. -I../../inc -I ../../../inc -I $(FW_INC)

SRC := csr_log_pcap.c csr_log_pcap_common.c 

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
