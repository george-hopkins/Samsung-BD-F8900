AUTOGEN_DIR 	:= $(TOP)/output/autogen
HANDCODED_FILE  := $(call path_subst,$(TOP)/inc/csr_bt_hand_coded.h)

.SECONDARY:

$(AUTOGEN_DIR)/csr_bt_%_private_prim.h: $(TOP)/profile_managers/inc/csr_bt_%_private_prim.h $(TOP)/inc/csr_bt_%_prim.h
	$(MKDIR) $(@D)
	cat $? > $@

$(AUTOGEN_DIR)/%_private_prim.h: $(TOP)/profile_managers/inc/%_private_prim.h
	$(MKDIR) $(@D)
	$(CP) -f $< $@

$(AUTOGEN_DIR)/%_prim.h: $(TOP)/inc/%_prim.h
	$(MKDIR) $(@D)
	$(CP) -f $< $@

%.corr: %.h $(TOP)/inc/csr_bt_hand_coded.h
	@echo "Creating $@"
	$(CREATE_PRIM_CODE) $(EXTRA_AUTOGEN_FLAGS) --debug=$(DEBUG_EXT) --action=-p --prim-file=$< --include="$(CFLAGS) $(INC)" --handcoded-file=$(HANDCODED_FILE) --tools-root=$(DEVEL_TOOL_PATH) --tech=csr_bt --autogen-dir=$(*D)
	@echo "Done creating $@"

# $(1) = public prims
# $(2) = private prims
join_pub_private_prims = $(sort $(filter-out $(subst _private,,$2),$1) $2)
