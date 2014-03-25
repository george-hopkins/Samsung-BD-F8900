###############################################################################
#
#                (c) Cambridge Silicon Radio Limited 2010
#
#                All rights reserved and confidential information of CSR
#
###############################################################################
FW_ROOT := ../../../../../..
OBJ_PATH = $(BSP_ROOT)/output/$(CONFIG)/$(TARGET)/obj/src/lowlevel/drivers
include $(FW_ROOT)/local_rules.mk

LIB_PATH = $(BSP_LIB)
LIB_NAME := $(call lib_name,csr_low_level_drivers)

SRC := \
    csr_ac97_ctrl.c \
    csr_ac97_data_in.c \
    csr_ac97_data_mic.c \
    csr_ac97_data_out.c \
    csr_confdata_lr.c \
    csr_confdata_r.c \
    csr_confdata_w.c \
    csr_dma.c \
    csr_delay.c \
    csr_ethernet.c \
    csr_i2c.c \
    csr_lcd.c \
    csr_mmu.c \
    csr_nand_r.c \
    csr_nand_w.c \
    csr_rtc.c \
    csr_timer.c \
    csr_touch.c \
    csr_uart.c

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
