#-*-Makefile-*-
###############################################################################
#
#                (c) Cambridge Silicon Radio Limited 2009
#
#                All rights reserved and confidential information of CSR
#
###############################################################################
FW_ROOT := ../../../..
OBJ_PATH = $(FW_ROOT)/output/$(CONFIG)/$(TARGET)/obj/gsp/src/application/ui
include $(FW_ROOT)/local_rules.mk
include $(FW_ROOT)/m_targets_autogen.mk

LIB_NAME := $(call lib_name,csr_ui)
LIB_PATH := $(FW_LIB)

INC := $(FW_INC) $(BSP_INC) -I$(FW_ROOT)/gsp/src/inc

SRC := csr_ui_ctrl.c \
       csr_ui_displaystack.c \
       csr_ui_emulator_touchdecoder.c \
       csr_ui_exception.c \
       csr_ui_font_sax_mono_12x22.c \
       csr_ui_font_sax_mono_10x21.c \
       csr_ui_graphics_driver.c \
       csr_ui_icon.c \
       csr_ui_keyhandler.c \
       csr_ui_lib.c \
       csr_ui_renderer.c \
       csr_ui_renderer_text.c \
       csr_ui_sef.c \
       csr_ui_uiemanager.c \
       csr_ui_util.c

FREE_SRC := csr_ui_free_up.c \
            csr_ui_free_down.c 

SRC += $(FREE_SRC)

OBJ := $(addprefix $(OBJ_PATH)/,$(call path_subst,$(SRC:.c=.o)))

DEP := $(OBJ:.o=.d)
include $(FW_ROOT)/m_targets.mk

all: lib

lib: $(LIB_PATH)/$(LIB_NAME)

$(LIB_PATH)/$(LIB_NAME): $(OBJ)
	$(MKDIR) $(@D)
	$(call create_lib,$@,$(OBJ))


clean:
	$(RM) $(call path_subst,$(LIB_PATH)/$(LIB_NAME))
	$(RM) $(call path_subst,$(OBJ))
