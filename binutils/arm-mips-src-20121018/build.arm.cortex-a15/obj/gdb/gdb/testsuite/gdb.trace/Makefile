VPATH = /home6/daewon2/Release/glibc/SP/SP_Linux/LP_Linux_Prj/REL/VDLinux.3.x.2012/toolchain/src/arm-mips-src-20121018/build.arm.cortex-a15/sources/gdb/gdb/testsuite/gdb.trace
srcdir = /home6/daewon2/Release/glibc/SP/SP_Linux/LP_Linux_Prj/REL/VDLinux.3.x.2012/toolchain/src/arm-mips-src-20121018/build.arm.cortex-a15/sources/gdb/gdb/testsuite/gdb.trace

.PHONY: all clean mostlyclean distclean realclean

PROGS = ax backtrace deltrace infotrace packetlen passc-dyn passcount \
	report save-trace tfile tfind tracecmd tsv unavailable while-dyn \
	while-stepping

all info install-info dvi install uninstall installcheck check:
	@echo "Nothing to be done for $@..."

clean mostlyclean:
	-rm -f actions circ collection
	-rm -f *.o *.diff *~ *.bad core sh3 hppa mn10300 $(PROGS)

distclean maintainer-clean realclean: clean
	-rm -f Makefile config.status config.log gdb.log gdb.sum
