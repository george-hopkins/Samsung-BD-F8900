FW_ROOT := ../../..
OBJ_PATH = $(FW_ROOT)/output/$(CONFIG)/$(TARGET)/obj/gsp/src/dhcp_server
include $(FW_ROOT)/local_rules.mk
include $(FW_ROOT)/m_targets_autogen.mk

LIB_NAME := $(call lib_name,csr_dhcp_server)
LIB_NAME_DHCP_SERVER_LIB := $(call lib_name,csr_dhcp_server_lib)

LIB_PATH := $(FW_LIB)

INC += $(BSP_INC) $(FW_INC) -I../inc

SRC := csr_dhcp_server_handler.c \
	csr_dhcp_server_sef.c \
	csr_dhcp_server_util.c

DHCP_SERVER_LIB_SRC := csr_dhcp_server_free_up.c \
			csr_dhcp_server_free_down.c \
			csr_dhcp_server_lib.c \
			csr_dhcp_server_lib_priv.c

OBJ := $(addprefix $(OBJ_PATH)/,$(call path_subst,$(SRC:.c=.o)))

DHCP_SERVER_LIB_OBJ := $(addprefix $(OBJ_PATH)/,$(call path_subst,$(DHCP_SERVER_LIB_SRC:.c=.o)))

DEP := $(OBJ:.o=.d) $(DHCP_SERVER_LIB_OBJ:.o=.d)
include $(FW_ROOT)/m_targets.mk

all: lib

bin:

lib: $(LIB_PATH)/$(LIB_NAME) $(LIB_PATH)/$(LIB_NAME_DHCP_SERVER_LIB)

$(LIB_PATH)/$(LIB_NAME): $(OBJ)
	$(MKDIR) $(@D)
	$(call create_lib,$@,$(OBJ))

$(LIB_PATH)/$(LIB_NAME_DHCP_SERVER_LIB): $(DHCP_SERVER_LIB_OBJ)
	$(MKDIR) $(@D)
	$(call create_lib,$@,$(DHCP_SERVER_LIB_OBJ))

clean:
	$(RM) $(call path_subst,$(LIB_PATH)/$(LIB_NAME))
	$(RM) $(call path_subst,$(LIB_PATH)/$(LIB_NAME_DHCP_SERVER_LIB))
	$(RM) $(call path_subst,$(OBJ))
	$(RM) $(call path_subst,$(DHCP_SERVER_LIB_OBJ))
