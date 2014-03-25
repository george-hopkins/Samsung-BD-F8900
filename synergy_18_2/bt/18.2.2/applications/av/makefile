#-*-Makefile-*-
###############################################################################
#
#                (c) Cambridge Silicon Radio Limited 2009
#
#                All rights reserved and confidential information of CSR
#
# REVISION:      $Revision: #2 $
###############################################################################
TOP := ../..

MODULE_NAME := av

OBJ_PATH = $(TOP)/output/$(CONFIG)/$(TARGET)/obj/applications/$(MODULE_NAME)
include $(TOP)/local_rules.mk

# Set SBCFILE as the default flavour
ifeq ($(CSR_BT_APP_AV_FILTER),)
ifeq ($(CSR_DSPM_ENABLE),1)
	CSR_BT_APP_AV_FILTER=avrouter
else
	CSR_BT_APP_AV_FILTER=sbcfile
endif
endif

# Build AAC flavour
ifeq ($(CSR_BT_APP_AV_FILTER),aac)
APP_CFLAGS += -DUSE_AAC -DUSE_FILEDUMP
APP_SRC += csr_bt_aac_parser.c \
        csr_bt_filter_aac.c
endif

# Build MP3 flavour
ifeq ($(CSR_BT_APP_AV_FILTER),mp3)
APP_CFLAGS += -DUSE_MP3 -DUSE_FILEDUMP
APP_SRC += csr_bt_filter_mp3.c
endif

# Build SBCWAV flavour
ifeq ($(CSR_BT_APP_AV_FILTER),sbcwav)
APP_CFLAGS += -DUSE_SBC -DUSE_WAVE
APP_SRC += csr_bt_filter_sbc.c \
        csr_bt_filter_wave.c
endif

# Build SBCAUDIO flavour
ifeq ($(CSR_BT_APP_AV_FILTER),sbcaudio)
APP_CFLAGS += -DUSE_SBC
APP_SRC += csr_bt_filter_sbc.c

# Autodetect audio system
ifeq ($(TARGET),pcwin-nt-x86)
APP_SRC   += csr_bt_filter_winplay.c csr_bt_filter_wingrab.c
APP_CFLAGS += -DUSE_WINAUDIO
endif

ifeq ($(TARGET),pclin-2.6-x86)
APP_SRC   += csr_bt_filter_alsa.c
APP_CFLAGS += -DUSE_ALSA `pkg-config --cflags alsa`
endif

ifeq ($(TARGET),pclin-2.6-generic)
APP_SRC   += csr_bt_filter_alsa.c
APP_CFLAGS += -DUSE_ALSA `pkg-config --cflags alsa`
endif

endif

# Build SBCFILE flavour
ifeq ($(CSR_BT_APP_AV_FILTER),sbcfile)
APP_CFLAGS += -DUSE_SBCFILE
APP_SRC += csr_bt_filter_sbcfile.c
endif

# Build AVROUTER flavour
ifeq ($(CSR_BT_APP_AV_FILTER),avrouter)
APP_CFLAGS += -DUSE_AVROUTER
APP_SRC += csr_bt_filter_avrouter.c
endif

CFLAGS += $(EXTRA_CFLAGS) $(APP_CFLAGS)

#\
#          -DCSR_BT_RUN_STACK=1 -DCSR_BT_RUN_APP=1 

SRC :=  csr_bt_av2_main.c \
        csr_bt_av2_utils.c \
        csr_bt_av2_avup.c \
        csr_bt_av2_avhandlers.c \
        csr_bt_av2_avdown.c \
        csr_bt_av2_filter.c \
        csr_bt_filter_av2.c \
        csr_bt_av_app_ui_sef.c \
        csr_bt_av_app_util.c \
        csr_bt_filter_filedump.c \
		csr_bt_av2_dspm.c \
	$(APP_SRC)

include $(TOP)/applications/generic/makefile.common.app
