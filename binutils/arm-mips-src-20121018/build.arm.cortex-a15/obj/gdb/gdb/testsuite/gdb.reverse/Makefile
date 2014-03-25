VPATH = /home6/daewon2/Release/glibc/SP/SP_Linux/LP_Linux_Prj/REL/VDLinux.3.x.2012/toolchain/src/arm-mips-src-20121018/build.arm.cortex-a15/sources/gdb/gdb/testsuite/gdb.reverse
srcdir = /home6/daewon2/Release/glibc/SP/SP_Linux/LP_Linux_Prj/REL/VDLinux.3.x.2012/toolchain/src/arm-mips-src-20121018/build.arm.cortex-a15/sources/gdb/gdb/testsuite/gdb.reverse

EXECUTABLES   = break-reverse consecutive-reverse finish-reverse \
	machinestate solib-reverse step-reverse until-reverse \
	watch-reverse i386-reverse

MISCELLANEOUS = 

all info install-info dvi install uninstall installcheck check:
	@echo "Nothing to be done for $@..."

clean mostlyclean:
	rm -f *~ *.o *.x *.ci *.sl a.out core
	rm -f $(EXECUTABLES) $(MISCELLANEOUS)

distclean maintainer-clean realclean: clean
	rm -f Makefile config.status config.log site.* gdb.log gdb.sum

