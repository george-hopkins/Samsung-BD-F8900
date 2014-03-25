AUTOGEN_DIR 	:= $(FW_ROOT)/output/autogen
HANDCODED_FILE  := $(call path_subst,$(FW_ROOT)/gsp/src/converter/csr_hand_coded.h)

.SECONDARY:

$(AUTOGEN_DIR)/csr_%_private_prim.h: $(FW_ROOT)/gsp/inc/csr_%_prim.h $(FW_ROOT)/gsp/src/bluecore/inc/hci/csr_%_private_prim.h
	$(MKDIR) $(@D)
	cat $? > $@

$(AUTOGEN_DIR)/csr_%_private_prim.h: $(FW_ROOT)/gsp/inc/csr_%_prim.h $(FW_ROOT)/gsp/src/bluecore/inc/bccmd/csr_%_private_prim.h
	$(MKDIR) $(@D)
	cat $? > $@

$(AUTOGEN_DIR)/csr_%_private_prim.h: $(FW_ROOT)/gsp/inc/csr_%_prim.h $(FW_ROOT)/gsp/src/bluecore/inc/tm/csr_%_private_prim.h
	$(MKDIR) $(@D)
	cat $? > $@

$(AUTOGEN_DIR)/csr_%_private_prim.h: $(FW_ROOT)/gsp/inc/csr_%_prim.h $(FW_ROOT)/gsp/src/application/ui/csr_%_private_prim.h
	$(MKDIR) $(@D)
	cat $? > $@

$(AUTOGEN_DIR)/%_prim.h: $(FW_ROOT)/gsp/inc/%_prim.h
	$(MKDIR) $(@D)
	$(CP) -f $< $@

$(AUTOGEN_DIR)/%_prim.h: $(FW_ROOT)/bsp/inc/%_prim.h
	$(MKDIR) $(@D)
	$(CP) -f $< $@

%.corr: %.h
	@echo "Creating $@"
	$(CREATE_PRIM_CODE) --debug=$(DEBUG_EXT) --action=-p --prim-file=$< --include="$(INC)" --handcoded-file=$(HANDCODED_FILE) --tools-root=$(DEVEL_TOOL_PATH) --tech=csr --autogen-dir=$(*D)

# $(1) = public prims
# $(2) = private prims
join_pub_private_prims = $(sort $(filter-out $(subst _private,,$2),$1) $2)
