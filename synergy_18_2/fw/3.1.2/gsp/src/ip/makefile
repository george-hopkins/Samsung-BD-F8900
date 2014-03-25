FW_ROOT := ../../..
OBJ_PATH = $(FW_ROOT)/output/$(CONFIG)/$(TARGET)/obj/gsp/src/ip
include $(FW_ROOT)/local_rules.mk
include $(FW_ROOT)/m_targets_autogen.mk
include module.mk

LIB_NAME := $(call lib_name,csr_ip)
LIB_NAME_ETHER_LIB := $(call lib_name,csr_ip_ether_lib)
LIB_NAME_SOCKET_LIB := $(call lib_name,csr_ip_socket_lib)
LIB_NAME_IFCONFIG_LIB := $(call lib_name,csr_ip_ifconfig_lib)

LIB_PATH := $(FW_LIB)

INC += $(BSP_INC) $(FW_INC) -I../inc

SRC := $(MODULE_SRC)

ETHER_LIB_SRC := \
			csr_ip_ether_free_up.c \
			csr_ip_ether_free_down.c \
			csr_ip_ether_lib.c \
			csr_ip_ether_private_lib.c

SOCKET_LIB_SRC := \
			csr_ip_socket_free_up.c \
			csr_ip_socket_free_down.c \
			csr_ip_socket_lib.c \
			csr_ip_socket_private_lib.c

IFCONFIG_LIB_SRC := \
			csr_ip_ifconfig_free_up.c \
			csr_ip_ifconfig_free_down.c \
			csr_ip_ifconfig_lib.c \
			csr_ip_ifconfig_private_lib.c

OBJ := $(addprefix $(OBJ_PATH)/,$(SRC:.c=.o))

ETHER_LIB_OBJ := $(addprefix $(OBJ_PATH)/,$(ETHER_LIB_SRC:.c=.o))
SOCKET_LIB_OBJ := $(addprefix $(OBJ_PATH)/,$(SOCKET_LIB_SRC:.c=.o))
IFCONFIG_LIB_OBJ := $(addprefix $(OBJ_PATH)/,$(IFCONFIG_LIB_SRC:.c=.o))

DEP := $(OBJ:.o=.d) $(ETHER_LIB_OBJ:.o=.d) $(SOCKET_LIB_OBJ:.o=.d) $(IFCONFIG_LIB_OBJ:.o=.d)
include $(FW_ROOT)/m_targets.mk

all: lib

bin:

lib: $(LIB_PATH)/$(LIB_NAME) $(LIB_PATH)/$(LIB_NAME_ETHER_LIB) $(LIB_PATH)/$(LIB_NAME_SOCKET_LIB) $(LIB_PATH)/$(LIB_NAME_IFCONFIG_LIB)

$(LIB_PATH)/$(LIB_NAME): $(OBJ)
	$(MKDIR) $(@D)
	$(call create_lib,$@,$(OBJ))

$(LIB_PATH)/$(LIB_NAME_ETHER_LIB): $(ETHER_LIB_OBJ)
	$(MKDIR) $(@D)
	$(call create_lib,$@,$(ETHER_LIB_OBJ))

$(LIB_PATH)/$(LIB_NAME_SOCKET_LIB): $(SOCKET_LIB_OBJ)
	$(MKDIR) $(@D)
	$(call create_lib,$@,$(SOCKET_LIB_OBJ))

$(LIB_PATH)/$(LIB_NAME_IFCONFIG_LIB): $(IFCONFIG_LIB_OBJ)
	$(MKDIR) $(@D)
	$(call create_lib,$@,$(IFCONFIG_LIB_OBJ))

clean:
	$(RM) $(LIB_PATH)/$(LIB_NAME)
	$(RM) $(LIB_PATH)/$(LIB_NAME_ETHER_LIB)
	$(RM) $(LIB_PATH)/$(LIB_NAME_SOCKET_LIB)
	$(RM) $(LIB_PATH)/$(LIB_NAME_IFCONFIG_LIB)
	$(RM) $(OBJ)
	$(RM) $(ETHER_LIB_OBJ)
	$(RM) $(SOCKET_LIB_OBJ)
	$(RM) $(IFCONFIG_LIB_OBJ)
