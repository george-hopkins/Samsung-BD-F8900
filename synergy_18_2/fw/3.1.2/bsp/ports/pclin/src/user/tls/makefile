FW_ROOT := ../../../../../..
BSP_TOP := ../../..
OBJ_PATH = $(BSP_TOP)/output/$(CONFIG)/$(TARGET)/obj/src/user/tls
include $(FW_ROOT)/local_rules.mk

LIB_NAME := $(call lib_name,csr_tls)
LIB_PATH := $(BSP_TOP)/output/$(CONFIG)/$(TARGET)/lib

SRC := 	csr_tls_handler.c \
        csr_tls_lib.c \
        csr_tls_sef_tls.c \
        csr_tls_util.c \
        csr_tls_thread.c \
        csr_tls_free_up.c \
        csr_tls_free_down.c

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
