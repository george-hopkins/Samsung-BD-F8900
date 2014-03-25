VPATH = /home6/daewon2/Release/glibc/SP/SP_Linux/LP_Linux_Prj/REL/VDLinux.3.x.2012/toolchain/src/arm-mips-src-20121018/build.arm.cortex-a15/sources/gdb/gdb/testsuite/gdb.python
srcdir = /home6/daewon2/Release/glibc/SP/SP_Linux/LP_Linux_Prj/REL/VDLinux.3.x.2012/toolchain/src/arm-mips-src-20121018/build.arm.cortex-a15/sources/gdb/gdb/testsuite/gdb.python

EXECUTABLES = py-type py-value py-prettyprint py-template py-block \
	py-symbol py-mi py-breakpoint py-inferior py-infthread \
	py-shared python lib-types py-events py-evthreads py-frame \
	py-pp-maint py-progspace py-section-script py-objfile

MISCELLANEOUS = py-shared-sl.sl

all info install-info dvi install uninstall installcheck check:
	@echo "Nothing to be done for $@..."

clean mostlyclean:
	-rm -f *~ *.o *.ci
	-rm -f core $(EXECUTABLES) $(MISCELLANEOUS)

distclean maintainer-clean realclean: clean
	-rm -f Makefile config.status config.log gdb.log gdb.sum
