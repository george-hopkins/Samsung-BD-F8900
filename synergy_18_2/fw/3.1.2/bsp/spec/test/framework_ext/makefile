#-*-Makefile-*-
###############################################################################
#
#                (c) Cambridge Silicon Radio Limited 2010
#
#                All rights reserved and confidential information of CSR
#
###############################################################################
FW_ROOT := ../../../..
OBJ_PATH = $(FW_ROOT)/output/$(CONFIG)/$(TARGET)/obj/bsp/spec/test/framework_ext
include $(FW_ROOT)/local_rules.mk

MODULE_NAME := fwext_test

LIB_PATH := $(FW_LIB)/bsp/spec/test/
LIB_NAME := $(call lib_name,$(MODULE_NAME))

BIN_PATH := bin/$(CONFIG)/$(TARGET)/
APP_NAME := $(call bin_name,$(MODULE_NAME))

LIBS := \
	$(call use_lib,$(MODULE_NAME)) \
	$(call use_lib,csr_main) \
	$(call use_lib,csr_arg_search) \
	$(call use_lib,csr_sched) \
	$(call use_lib,$(DEFAULT_LIBS))

ifeq ($(TARGET_OS),nucleus)
CFLAGS += -DTARGET_OS_NUCLEUS
endif

ifeq ($(TARGET_OS),nt)
CFLAGS += -DTARGET_OS_NT
endif

ifeq ($(TARGET_PLATFORM),pclin)
CFLAGS += -DTARGET_OS_LINUX
endif

ifeq ($(USE_CSRTIME),1)
CFLAGS += -DUSE_CSRTIME
endif

LDPATH += $(call create_ldpath,$(BSP_LIB)) \
	$(call create_ldpath,$(FW_LIB)) \
	$(call create_ldpath,$(LIB_PATH))
INC += -I . -I $(BSP_ROOT)/inc

LIBSRC := csr_test_mutex_create_destroy.c \
	csr_test_thread_mutex_lock.c \
	csr_test_thread_sleep.c \
	csr_test_thread_create_too_many.c \
	csr_test_mutex_lock_unlock.c \
	csr_test_globalmutex.c \
	csr_test_event_wait.c \
	csr_test_event_wait_timeout.c \
	csr_test_thread_get_handle.c

# Nucleus
ifneq ($(filter $(TARGET_PLATFORM),bdb2 bdb3),)
APPSRC = csr_nucleus_main.c
# Standard target
else
APPSRC = csr_windows_main.c
endif

LIBOBJ := $(addprefix $(OBJ_PATH)/,$(LIBSRC:.c=.o))
APPOBJ := $(addprefix $(OBJ_PATH)/,$(APPSRC:.c=.o))

DEP := $(LIBOBJ:.o=.d) $(APPOBJ:.o=.d)
include $(FW_ROOT)/m_targets.mk

all: bin

lib: $(LIB_PATH)/$(LIB_NAME)

$(LIB_PATH)/$(LIB_NAME): $(LIBOBJ)
	$(MKDIR) $(@D)
	$(call create_lib,$@,$(LIBOBJ))

bin: $(BIN_PATH)/$(APP_NAME)

$(BIN_PATH)/$(APP_NAME): $(APPOBJ) $(LIB_PATH)/$(LIB_NAME)
	$(MKDIR) $(@D)
	$(call create_exec,$(BIN_PATH)/$(APP_NAME),$(APPOBJ),$(LDPATH),$(LIBS))

clean:
	$(call clean_exec,$(BIN_PATH)/$(APP_NAME))
	$(RM) $(LIB_PATH)/$(LIB_NAME)
	$(RM) $(LIBOBJ)
