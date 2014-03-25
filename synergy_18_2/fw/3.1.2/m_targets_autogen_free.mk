AUTOGEN_DIR_FREE := $(FW_ROOT)/output/autogen/free
HANDCODED_FILE  := $(call path_subst,$(FW_ROOT)/gsp/src/converter/csr_hand_coded.h)

.SECONDARY:

$(AUTOGEN_DIR_FREE)/csr_%_private_prim.h: $(FW_ROOT)/gsp/src/bluecore/inc/hci/csr_%_private_prim.h
	$(MKDIR) $(@D)
	$(CP) -f $< $@

$(AUTOGEN_DIR_FREE)/csr_%_private_prim.h: $(FW_ROOT)/gsp/src/bluecore/inc/bccmd/csr_%_private_prim.h
	$(MKDIR) $(@D)
	$(CP) -f $< $@

$(AUTOGEN_DIR_FREE)/csr_%_private_prim.h: $(FW_ROOT)/gsp/src/bluecore/inc/tm/csr_%_private_prim.h
	$(MKDIR) $(@D)
	$(CP) -f $< $@

$(AUTOGEN_DIR_FREE)/csr_%_private_prim.h: $(FW_ROOT)/gsp/src/application/ui/csr_%_private_prim.h
	$(MKDIR) $(@D)
	$(CP) -f $< $@

$(AUTOGEN_DIR_FREE)/%_prim.h: $(FW_ROOT)/gsp/inc/%_prim.h
	$(MKDIR) $(@D)
	$(CP) -f $< $@

$(AUTOGEN_DIR_FREE)/%_prim.h: $(FW_ROOT)/bsp/inc/%_prim.h
	$(MKDIR) $(@D)
	$(CP) -f $< $@

%.corr: %.h
	@echo "Creating $@"
	$(CREATE_PRIM_CODE) --debug=$(DEBUG_EXT) --action=-p --prim-file=$< --include="$(INC)" --handcoded-file=$(HANDCODED_FILE) --tools-root=$(DEVEL_TOOL_PATH) --tech=csr --autogen-dir=$(*D)

