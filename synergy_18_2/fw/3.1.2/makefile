###############################################################################
#
#                (c) Cambridge Silicon Radio Limited 2010
#
#                All rights reserved and confidential information of CSR
#
###############################################################################
FW_ROOT := .
include $(FW_ROOT)/local_rules.mk

DEFAULT: all

all: bin

bin: lib
ifneq ($(CSR_SAMSUNG_SERVICE),1)
ifneq ($(TARGET),gen3audio-nucleus-arm)
ifneq ($(SUB_TARGET_ARCH),LinuxKernel)
	$(MAKE) -C example_apps $@
endif
endif
endif
lib:
	$(MAKE) -C $(BSP_ROOT) all
	$(MAKE) -C gsp/src all

clean:
	$(MAKE) -C $(BSP_ROOT) clean
	$(MAKE) -C gsp/src clean

veryclean:
	find . -depth  \( -name "*.pdb" -or \( \( -path "./drivers*" \
		 -or -path "./example_apps*" \) \
		 -and -name "*.exe" \) \
		 -or -name obj -type d \
		 -or -name "Debug" -type d \
		 -or -name "details" -type d \
		 -or -name .tmp_versions \
		 -or -name "*.pdb" \
		 -or -name "*.lib" \
		 -or -name "*.lst" \
		 -or -name "*app.bin" \
		 -or -name "*.axf" \
		 -or -name "*.cap" \
		 -or -name "*.ncb" \
		 -or -name "*.opt" \
		 -or -name "*.plg" \
		 -or -name "sc_db.db" \
		 -or -name "dev*.hid" \
		 -or -name "*.o" \
		 -or -name "*.obj" \
		 -or -name "*.d" \
		 -or -name "*.fti" \
		 -or -name "*.frm" \
		 -or -name "*.cfa" \
		 -or -name "*.log" \
		 -or -name "*~" \
		 -or -name "core.*" \
		 -or -name "*.ko" \
		 -or -name "*.mod.*" \
		 -or -name "*.mod.*.d" \
		 -or -name ".*.o.cmd" \
		 -or -name ".*.ko.cmd" \
		 -or -name "out" \
		 -or -name "*_exe.pft" \
		 -or -name "*_pure.ini" \
		 -or -name "*_pure.log" \
		 -or -name ".tmp_versions" \) -print0 | xargs -n1000 -0r rm -rf

