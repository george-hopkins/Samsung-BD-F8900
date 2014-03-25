srcdir = /home6/daewon2/Release/glibc/SP/SP_Linux/LP_Linux_Prj/REL/VDLinux.3.x.2012/toolchain/src/arm-mips-src-20121018/build.arm.cortex-a15/sources/gdb/gdb/testsuite/gdb.java
VPATH = /home6/daewon2/Release/glibc/SP/SP_Linux/LP_Linux_Prj/REL/VDLinux.3.x.2012/toolchain/src/arm-mips-src-20121018/build.arm.cortex-a15/sources/gdb/gdb/testsuite/gdb.java

all info install-info dvi install uninstall installcheck check:
	@echo "Nothing to be done for $@..."

.SUFFIXES: .java .class .o .exe .exp .check

# Do 'make javamisc.check' to run just the javamisc.exp test.

.exp.check:
	rootme=`pwd`/; export rootme; \
	cd .. ; \
	$(MAKE) just-check RUNTESTFLAGS="${RUNTESTFLAGS} $*.exp" \
	  EXPECT=${EXPECT}

clean mostlyclean:
	-rm -f *.o ${OBJS}  *.class *.exe *~ core jmain jmisc jnpe jprint

distclean maintainer-clean realclean: clean
	-rm -f Makefile config.status config.log gdb.log gdb.sum
