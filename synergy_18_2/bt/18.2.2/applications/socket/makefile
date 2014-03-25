TOP := ../..

OBJ_PATH = $(TOP)/output/$(CONFIG)/$(TARGET)/obj/socket
include $(TOP)/local_rules.mk

LIB_NAME := $(call lib_name,csr_socket)
LIB_PATH := $(TOP)/output/$(CONFIG)/$(TARGET)/lib
.PHONY: all bin lib clean

INC += -I. -I../inc -I$(BSP_ROOT)/inc -I$(TOP)/inc -I ../ss_inc

SRC := \
	server.c \
    client.c \
	common.c

OBJ := $(addprefix $(OBJ_PATH)/,$(call path_subst,$(SRC:.c=.o)))

DEP := $(OBJ:.o=.d)
include $(FW_ROOT)/m_targets.mk

all: lib

lib: create_dirs $(LIB_PATH)/$(LIB_NAME) 
$(LIB_PATH)/$(LIB_NAME): $(OBJ)
	$(call create_lib,$@,$(OBJ))

create_dirs: $(LIB_PATH)

$(LIB_PATH):
	$(MKDIR) $(call path_subst,$(LIB_PATH))

clean:
	$(RM) *~
	$(RM) $(call path_subst,$(LIB_PATH)/$(LIB_NAME))
	$(RM) $(call path_subst,$(OBJ))
