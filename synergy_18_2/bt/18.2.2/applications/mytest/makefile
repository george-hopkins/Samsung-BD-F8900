#-*-Makefile-*-
###############################################################################
#
#                (c) Cambridge Silicon Radio Limited 2009
#
#                All rights reserved and confidential information of CSR
#
# REVISION:      $Revision: #1 $
###############################################################################

TOP := ../..

APP_NAME = mytest

APP_PREREQ =

APP_INC = 



APP_LDPATH =


APP_CFLAGS += -DCSR_BT_RUN_STACK=1 -DCSR_BT_RUN_APP=1 

# Set SBCFILE as the default flavour
ifeq ($(CSR_BT_APP_AV_FILTER),)
ifeq ($(CSR_DSPM_ENABLE),1)
	CSR_BT_APP_AV_FILTER=avrouter
else
	CSR_BT_APP_AV_FILTER=sbcaudio
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
APP_LIBS += $(call use_lib,csr_sbc)
APP_CFLAGS += -DUSE_SBC -DUSE_WAVE
APP_SRC += csr_bt_filter_sbc.c \
        csr_bt_filter_wave.c
endif

# Build SBCAUDIO flavour
ifeq ($(CSR_BT_APP_AV_FILTER),sbcaudio)
APP_LIBS += $(call use_lib,csr_sbc)
APP_CFLAGS += -DUSE_SBC
APP_SRC += csr_bt_filter_sbc.c

# Autodetect audio system
ifeq ($(word 1,$(subst -, ,$(TARGET))),pcwin)
APP_CFLAGS += -DUSE_WINAUDIO
APP_SRC   += csr_bt_filter_winplay.c
endif

ifeq ($(word 1,$(subst -, ,$(TARGET))),pclin)
APP_LIBS += $(call use_lib,asound)
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


APP_SRC += csr_bt_mobis_app.c \
	csr_bt_tasks.c \
	Bluetooth_AV.c \
	Bluetooth_AVRC.c \
	Bluetooth_CM.c \
	Bluetooth_SC.c \
	Bluetooth_SD.c \
	csr_bt_filter_ss.c \
	csr_bt_av2_filter.c \
	csr_bt_filter_av2.c \
	Bluetooth_Key.c

OBJ_PATH = $(TOP)/output/$(CONFIG)/$(TARGET)/obj/applications/$(APP_NAME)/$(BUILD_TYPE)
include $(TOP)/local_rules.mk

include $(TOP)/applications/common/makefile.common
