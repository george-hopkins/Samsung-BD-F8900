FW_ROOT := ../../../../../..
OBJ_PATH = $(BSP_ROOT)/output/$(CONFIG)/$(TARGET)/obj/src/user/ip
include $(FW_ROOT)/local_rules.mk

LIB_NAME := $(call lib_name,csr_ip)
LIB_PATH := $(BSP_LIB)

SRC := \
	csr_ip_ether_sef.c \
	csr_ip_handler.c \
	csr_ip_ifconfig_sef.c \
	csr_ip_netlink_parser.c \
	csr_ip_socket_sef.c \
	csr_ip_thread.c \
	csr_ip_util.c

ifeq ($(BSP_SUPPORT_OPENSSL),1)
ifeq ($(CSR_IP_SUPPORT_TLS),1)
SRC += \
	csr_ip_tls_free_down.c \
	csr_ip_tls_free_up.c \
	csr_ip_tls_handler.c \
	csr_ip_tls_lib.c \
	csr_ip_tls_sef.c \
	csr_ip_tls_thread.c \
	csr_ip_tls_util.c
endif
endif

OBJ := $(addprefix $(OBJ_PATH)/,$(call path_subst,$(SRC:.c=.o)))

DEP := $(OBJ:.o=.d)
include $(FW_ROOT)/m_targets.mk

DEFAULT: all

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
