#-*-Makefile-*-
###############################################################################
#
#                (c) Cambridge Silicon Radio Limited 2009
#
#                All rights reserved and confidential information of CSR
#
# REVISION:      $Revision: #2 $
###############################################################################
TOP := ..

OBJ_PATH = $(TOP)/output/$(CONFIG)/$(TARGET)/obj/src

include $(TOP)/local_rules.mk

#
# List of sub dirs to process
# New profiles are added at the end of this list
# If the new profile has a prim file make sure to include it in 
# the FREE_SUB_DIRS as well
#

SUB_DIRS += common/code \
	cm/code \
	sc/code \
	sd/code 

FREE_SUB_DIRS := cm/code \
	sc/code \
	sd/code

PRIVATE_FREE_SUB_DIRS :=  sd/code \
	sc/code

GENFSM_SUB_DIRS :=

ifeq ($(filter $(CFLAGS),-DEXCLUDE_CSR_BT_AV_MODULE),)
SUB_DIRS      += av/code
endif
FREE_SUB_DIRS += av/code

ifeq ($(filter $(CFLAGS),-DEXCLUDE_CSR_BT_AVRCP_MODULE),)
SUB_DIRS      += avrcp/code
endif
FREE_SUB_DIRS += avrcp/code

ifeq ($(filter $(CFLAGS),-DEXCLUDE_CSR_BT_DUNC_MODULE),)
SUB_DIRS      += dun/client/code
endif
FREE_SUB_DIRS += dun/client/code

ifeq ($(filter $(CFLAGS),-DEXCLUDE_CSR_BT_DG_MODULE),)
SUB_DIRS      += dun/gateway/code
endif
FREE_SUB_DIRS += dun/gateway/code

ifeq ($(filter $(CFLAGS),-DEXCLUDE_CSR_BT_HF_MODULE),)
SUB_DIRS      += handsfree/hf/code
endif
FREE_SUB_DIRS += handsfree/hf/code

ifeq ($(filter $(CFLAGS),-DEXCLUDE_CSR_BT_HFG_MODULE),)
SUB_DIRS      += handsfree/hfg/code
endif
FREE_SUB_DIRS += handsfree/hfg/code

ifeq ($(filter $(CFLAGS),-DEXCLUDE_CSR_BT_HCRP_MODULE),)
SUB_DIRS      += hcrp/code 
endif
FREE_SUB_DIRS += hcrp/code 

ifeq ($(filter $(CFLAGS),-DEXCLUDE_CSR_BT_MCAP_MODULE),)
SUB_DIRS      += mcap/code
PRIVATE_FREE_SUB_DIRS += mcap/code

ifeq ($(filter $(CFLAGS),-DEXCLUDE_CSR_BT_HDP_MODULE),)
SUB_DIRS      += hdp/code
endif
endif
FREE_SUB_DIRS += hdp/code 
#FREE_SUB_DIRS += mcap/code 

ifeq ($(filter $(CFLAGS),-DEXCLUDE_CSR_BT_BIPS_MODULE),)
SUB_DIRS      += obex/bip/bips/code
endif
FREE_SUB_DIRS += obex/bip/bips/code

ifeq ($(filter $(CFLAGS),-DEXCLUDE_CSR_BT_BIPC_MODULE),)
SUB_DIRS      += obex/bip/bipc/code
endif
FREE_SUB_DIRS += obex/bip/bipc/code

ifeq ($(filter $(CFLAGS),-DEXCLUDE_CSR_BT_BPPC_MODULE),)
SUB_DIRS      += obex/bpp/bppc/code
endif
FREE_SUB_DIRS += obex/bpp/bppc/code

ifeq ($(filter $(CFLAGS),-DEXCLUDE_CSR_BT_BPPS_MODULE),)
SUB_DIRS      += obex/bpp/bpps/code
endif
FREE_SUB_DIRS += obex/bpp/bpps/code

ifneq ($(filter-out $(CFLAGS),-DEXCLUDE_CSR_BT_SYNCS_MODULE -DEXCLUDE_CSR_BT_BIPS_MODULE -DEXCLUDE_CSR_BT_BIPC_MODULE -DEXCLUDE_CSR_BT_BPPC_MODULE -DEXCLUDE_CSR_BT_BPPS_MODULE -DEXCLUDE_CSR_BT_FTC_MODULE -DEXCLUDE_CSR_BT_FTS_MODULE -DEXCLUDE_CSR_BT_OPC_MODULE -DEXCLUDE_CSR_BT_OPS_MODULE -DEXCLUDE_CSR_BT_PAC_MODULE -DEXCLUDE_CSR_BT_PAS_MODULE -DEXCLUDE_CSR_BT_SMLC_MODULE -DEXCLUDE_CSR_BT_SMLS_MODULE),)
SUB_DIRS      += obex/common
endif

ifeq ($(filter $(CFLAGS),-DEXCLUDE_CSR_BT_FTC_MODULE),)
SUB_DIRS      += obex/ftp/ftc/code
endif
FREE_SUB_DIRS += obex/ftp/ftc/code

ifeq ($(filter $(CFLAGS),-DEXCLUDE_CSR_BT_FTS_MODULE),)
SUB_DIRS      += obex/ftp/fts/code
endif
FREE_SUB_DIRS += obex/ftp/fts/code

ifeq ($(filter $(CFLAGS),-DEXCLUDE_CSR_BT_MAPC_MODULE),)
SUB_DIRS      += obex/map/mapc/code
endif
FREE_SUB_DIRS += obex/map/mapc/code

ifeq ($(filter $(CFLAGS),-DEXCLUDE_CSR_BT_MAPS_MODULE),)
SUB_DIRS      += obex/map/maps/code
endif
FREE_SUB_DIRS += obex/map/maps/code

ifeq ($(filter $(CFLAGS),-DEXCLUDE_CSR_BT_OPC_MODULE),)
SUB_DIRS      += obex/opp/opc/code
endif
FREE_SUB_DIRS += obex/opp/opc/code

ifeq ($(filter $(CFLAGS),-DEXCLUDE_CSR_BT_OPS_MODULE),)
SUB_DIRS      += obex/opp/ops/code
endif
FREE_SUB_DIRS += obex/opp/ops/code

ifeq ($(filter $(CFLAGS),-DEXCLUDE_CSR_BT_PAC_MODULE),)
SUB_DIRS      += obex/pbap/pac/code
endif
FREE_SUB_DIRS += obex/pbap/pac/code

ifeq ($(filter $(CFLAGS),-DEXCLUDE_CSR_BT_PAS_MODULE),)
SUB_DIRS      += obex/pbap/pas/code
endif
FREE_SUB_DIRS += obex/pbap/pas/code

ifeq ($(filter $(CFLAGS),-DEXCLUDE_CSR_BT_SMLC_MODULE),)
SUB_DIRS      += obex/syncml/smlc/code
endif
FREE_SUB_DIRS += obex/syncml/smlc/code

ifeq ($(filter $(CFLAGS),-DEXCLUDE_CSR_BT_SMLS_MODULE),)
SUB_DIRS      += obex/syncml/smls/code
endif
FREE_SUB_DIRS += obex/syncml/smls/code

ifeq ($(filter $(CFLAGS),-DEXCLUDE_CSR_BT_SYNCS_MODULE),)
SUB_DIRS      += obex/sync/syncs/code
endif
FREE_SUB_DIRS += obex/sync/syncs/code

ifeq ($(filter $(CFLAGS),-DEXCLUDE_CSR_BT_SYNCC_MODULE),)
SUB_DIRS      += obex/sync/syncc/code
endif
FREE_SUB_DIRS += obex/sync/syncc/code

ifeq ($(filter $(CFLAGS),-DEXCLUDE_CSR_BT_SAPC_MODULE),)
SUB_DIRS      += sap/sapc/code
endif
FREE_SUB_DIRS += sap/sapc/code

ifeq ($(filter $(CFLAGS),-DEXCLUDE_CSR_BT_SAPS_MODULE),)
SUB_DIRS      += sap/saps/code
endif
FREE_SUB_DIRS += sap/saps/code

ifeq ($(filter $(CFLAGS),-DEXCLUDE_CSR_BT_SPP_MODULE),)
SUB_DIRS      += spp/code
endif
FREE_SUB_DIRS += spp/code

ifeq ($(filter $(CFLAGS),-DEXCLUDE_CSR_BT_JSR82_MODULE),)
SUB_DIRS      += jsr82/code
endif
FREE_SUB_DIRS += jsr82/code

ifeq ($(HCI_IN_RELEASE),1)
ifeq ($(filter $(CFLAGS),-DEXCLUDE_CSR_BT_TPT_MODULE),)
SUB_DIRS      += tpt/code
endif
endif

ifeq ($(HCI_IN_RELEASE),1)
SUB_DIRS      += bnep/code

ifeq ($(filter $(CFLAGS),-DEXCLUDE_CSR_BT_BSL_MODULE),)
SUB_DIRS      += pan/code
endif
FREE_SUB_DIRS += pan/code

ifeq ($(filter $(CFLAGS),-DEXCLUDE_CSR_BT_HIDH_MODULE),)
SUB_DIRS      += hid/host/code
endif
FREE_SUB_DIRS += hid/host/code

ifeq ($(filter $(CFLAGS),-DEXCLUDE_CSR_BT_HIDD_MODULE),)
SUB_DIRS      += hid/device/code
endif
FREE_SUB_DIRS += hid/device/code 

ifeq ($(filter $(CFLAGS),-DEXCLUDE_CSR_BT_HID_PARSER_MODULE),)
SUB_DIRS      += hid/parser
endif
endif

ifeq ($(filter $(CFLAGS),-DEXCLUDE_CSR_BT_AT_MODULE),)
SUB_DIRS      += at/code
endif
FREE_SUB_DIRS += at/code

ifeq ($(CSR_AMP_ENABLE),1)
SUB_DIRS      += ampm/code
endif
FREE_SUB_DIRS += ampm/code
GENFSM_SUB_DIRS += ampm/code

ifeq ($(CSR_BT_LE_ENABLE),1)
SUB_DIRS      += gatt/code
endif
FREE_SUB_DIRS += gatt/code
PRIVATE_FREE_SUB_DIRS += gatt/code
GENFSM_SUB_DIRS += gatt/code

ifeq ($(CSR_BT_LE_ENABLE),1)
ifeq ($(filter $(CFLAGS),-DEXCLUDE_CSR_BT_PROX_SRV_MODULE),)
SUB_DIRS      += proximity/server/code
endif
endif
FREE_SUB_DIRS += proximity/server/code
ifeq ($(CSR_BT_LE_ENABLE),1)
ifeq ($(filter $(CFLAGS),-DEXCLUDE_CSR_BT_THERM_SRV_MODULE),)
SUB_DIRS      += thermometer/server/code
endif
endif
FREE_SUB_DIRS += thermometer/server/code
ifeq ($(filter $(CFLAGS),-DEXCLUDE_CSR_BT_MDER_MODULE),)
SUB_DIRS      += phdc/mder/code
endif
#FREE_SUB_DIRS += phdc/mder/code

ifeq ($(filter $(CFLAGS),-DEXCLUDE_CSR_BT_PHDC_MGR_MODULE),)
SUB_DIRS      += phdc/mgr/code
endif
FREE_SUB_DIRS += phdc/mgr/code
GENFSM_SUB_DIRS += phdc/mgr/code

ifeq ($(filter $(CFLAGS),-DEXCLUDE_CSR_BT_PHDC_AG_MODULE),)
SUB_DIRS      += phdc/ag/code
endif
FREE_SUB_DIRS += phdc/ag/code
GENFSM_SUB_DIRS += phdc/ag/code

LIB_NAME := $(call lib_name,csr_bt)
API_NAME := $(call lib_name,csr_bt_lib)

LIB_PATH := $(call path_subst,$(TOP)/output/$(CONFIG)/$(TARGET)/lib)

-include $(addsuffix /module.mk,$(SUB_DIRS)) 

clean_sub_dirs  := $(addprefix _clean_,$(SUB_DIRS))
build_sub_dirs  := $(addprefix _build_,$(SUB_DIRS))
free_sub_dirs   := $(addprefix _free_, $(FREE_SUB_DIRS))
private_free_sub_dirs   := $(addprefix _private_free_, $(PRIVATE_FREE_SUB_DIRS))
genfsm_sub_dirs := $(addprefix _genfsm_,$(GENFSM_SUB_DIRS))

.PHONY: $(clean_sub_dirs) $(build_sub_dirs) $(free_sub_dirs) $(private_free_sub_dirs) $(genfsm_sub_dirs)  veryclean build_converter clean_converter

all: lib

bin:

lib: create_dirs $(LIB_PATH)/$(LIB_NAME) $(LIB_PATH)/$(API_NAME)

free_functions: $(free_sub_dirs) $(private_free_sub_dirs)

genfsm: $(genfsm_sub_dirs)

$(free_sub_dirs):
	$(MAKE) -C $(patsubst _free_%,%,$@) free_functions

$(private_free_sub_dirs):
	$(MAKE) -C $(patsubst _private_free_%,%,$@) private_free_functions

$(genfsm_sub_dirs):
	$(MAKE) -C $(patsubst _genfsm_%,%,$@) genfsm

$(LIB_PATH)/$(LIB_NAME): build_converter
	$(call create_lib,$@,$(addprefix $(OBJ_PATH)/,$(PM_LIB_OBJ)))

$(LIB_PATH)/$(API_NAME): build_converter
	$(call create_lib,$@,$(addprefix $(OBJ_PATH)/,$(PM_API_OBJ)))

$(build_sub_dirs): 
	$(MAKE) -C $(patsubst _build_%,%,$@) lib

build_converter: $(build_sub_dirs) 
# make the converter build dependent of the profile-builds, to support -jx see D-24403
ifeq ($(filter $(CFLAGS),-DEXCLUDE_CSR_BT_CONVERTER_MODULE),)
	$(MAKE) -C converter lib
endif

clean_converter:
ifeq ($(filter $(CFLAGS),-DEXCLUDE_CSR_BT_CONVERTER_MODULE),)
	$(MAKE) -C converter clean
endif


$(clean_sub_dirs):
	$(MAKE) -C $(patsubst _clean_%,%,$@) clean

create_dirs:
	$(MKDIR) $(LIB_PATH)

veryclean:
	$(MAKE) -C converter veryclean

clean: $(clean_sub_dirs) clean_converter
	$(RM) *~
	$(RM) $(OBJ_PATH)/templib_api
	$(RM) $(OBJ_PATH)/templib_pm
	$(RM) $(call path_subst,$(LIB_PATH)/$(LIB_NAME))
	$(RM) $(call path_subst,$(LIB_PATH)/$(API_NAME))
