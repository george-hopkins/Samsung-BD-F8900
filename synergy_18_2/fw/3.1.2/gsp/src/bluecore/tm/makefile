FW_ROOT := ../../../..
OBJ_PATH = $(FW_ROOT)/output/$(CONFIG)/$(TARGET)/obj/gsp/src/bluecore/tm
include $(FW_ROOT)/local_rules.mk
include $(FW_ROOT)/m_targets_autogen_free.mk

LIB_NAME := $(call lib_name,csr_tm_bluecore)
LIB_PATH := $(FW_LIB)

INC += -I . -I ../../inc -I ../inc/tm -I ../inc/bccmd -I ../inc/hci -I ../../../inc/bluecore/tm/

SRC :=	csr_tm_bluecore_handler.c \
		csr_tm_bluecore_lib.c \
		csr_tm_bluecore_private_lib.c \
		csr_tm_bluecore_sef.c \
		csr_tm_bluecore_transport.c 

FREE_SRC := csr_tm_bluecore_free_down.c csr_tm_bluecore_free_up.c csr_tm_bluecore_private_free_down.c csr_tm_bluecore_private_free_up.c

SRC += $(FREE_SRC)

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
