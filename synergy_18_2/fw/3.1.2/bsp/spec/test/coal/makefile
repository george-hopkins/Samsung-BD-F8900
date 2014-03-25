#-*-Makefile-*-
###############################################################################
#
#                (c) Cambridge Silicon Radio Limited 2010
#
#                All rights reserved and confidential information of CSR
#
###############################################################################
FW_ROOT := ../../../..
OBJ_PATH = $(FW_ROOT)/output/$(CONFIG)/$(TARGET)/obj/bsp/spec/test/coal
include $(FW_ROOT)/local_rules.mk

MODULE_NAME := sched_test

LIB_PATH := $(FW_LIB)/bsp/spec/test/
LIB_NAME := $(call lib_name,$(MODULE_NAME))

BIN_PATH := bin/$(CONFIG)/$(TARGET)/
APP_NAME := $(call bin_name,$(MODULE_NAME))

ifeq ($(LOG),1)
LIBS += \
	$(call use_lib,csr_log) \
	$(call use_lib,csr_log_pcap) \
	$(call use_lib,csr_log_fts)
endif

LIBS += \
	$(call use_lib,$(MODULE_NAME)) \
	$(call use_lib,csr_main) \
	$(call use_lib,csr_arg_search) \
	$(call use_lib,csr_core_msg_converter) \
	$(call use_lib,csr_formatted_io) \
	$(call use_lib,csr_msg_converter) \
	$(call use_lib,csr_unicode) \
	$(call use_lib,csr_mblk) \
	$(call use_lib,csr_sched) \
    $(call use_lib,$(DEFAULT_LIBS))

ifeq ($(INCLUDE_ELABORATE_TESTCASES),1)
CFLAGS += -DINCLUDE_ELABORATE_TESTCASES
endif

LDPATH += \
	$(call create_ldpath,$(FW_LIB)) \
	$(call create_ldpath,$(BSP_LIB)) \
	$(call create_ldpath,$(LIB_PATH))

INC += -I . -I $(BSP_ROOT)/inc

LIBSRC := \
	csr_sched_test_task1.c \
	csr_sched_test_task2.c \
	csr_sched_test_bgint.c \
	csr_testlib.c \
	csr_test_verdict.c \
	csr_test_put_message1.c \
	csr_test_put_message2.c \
	csr_test_timed_event1.c \
	csr_test_timed_event2.c \
	csr_test_timed_event3.c \
	csr_test_timed_event4.c \
	csr_test_cancel_timed_event.c \
	csr_test_broadcast1.c \
	csr_test_broadcast2.c \
	csr_test_timer_impl1.c \
	csr_test_bgint_none.c \
	csr_test_bgint_nomsg_notimer.c \
	csr_test_bgint_nomsg_timer.c \
	csr_test_bgint_msg_notimer.c \
	csr_test_bgint_msg_timer.c \
	csr_test_bgint_all.c \
	csr_test_put_message_perf.c \
	csr_test_timer_perf.c \
	csr_test_put_message_external.c

# Nucleus
ifneq ($(filter $(TARGET_PLATFORM),bdb2 bdb3),)
APPSRC += csr_nucleus_main.c
else
APPSRC += csr_test_main.c
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
	$(RM) $(APPOBJ)
	$(RM) $(LIB_PATH)/$(LIB_NAME)
	$(RM) $(LIBOBJ)
