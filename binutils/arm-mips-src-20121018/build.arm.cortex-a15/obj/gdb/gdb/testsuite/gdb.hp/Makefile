VPATH = /home6/daewon2/Release/glibc/SP/SP_Linux/LP_Linux_Prj/REL/VDLinux.3.x.2012/toolchain/src/arm-mips-src-20121018/build.arm.cortex-a15/sources/gdb/gdb/testsuite/gdb.hp
srcdir = /home6/daewon2/Release/glibc/SP/SP_Linux/LP_Linux_Prj/REL/VDLinux.3.x.2012/toolchain/src/arm-mips-src-20121018/build.arm.cortex-a15/sources/gdb/gdb/testsuite/gdb.hp

SUBDIRS = gdb.aCC gdb.base-hp gdb.compat gdb.defects gdb.objdbg

all:
	@echo "Nothing to be done for all..."

info:
install-info:
dvi:
install:
uninstall: force
installcheck:
check:

clean mostlyclean:
	-rm -f *~ core *.o
	if [ x"${SUBDIRS}" != x ] ; then \
	    for dir in ${SUBDIRS}; \
	    do \
		    echo "$$dir:"; \
		    if [ -d $$dir ]; then \
			    (cd $$dir; $(MAKE) clean); \
		    fi; \
	    done ; \
	else true; fi

distclean maintainer-clean realclean: clean
	-rm -f Makefile config.status config.log
	-rm -f *-init.exp gdb.log gdb.sum
	-rm -fr *.log summary detail *.plog *.sum *.psum site.*
	if [ x"${SUBDIRS}" != x ] ; then \
	    for dir in ${SUBDIRS}; \
	    do \
		    echo "$$dir:"; \
		    if [ -d $$dir ]; then \
			    (cd $$dir; $(MAKE) distclean); \
		    fi; \
	    done ; \
	else true; fi
