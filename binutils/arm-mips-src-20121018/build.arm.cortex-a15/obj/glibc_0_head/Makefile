srcdir = /home6/daewon2/Release/glibc/SP/SP_Linux/LP_Linux_Prj/REL/VDLinux.3.x.2012/toolchain/src/arm-mips-src-20121018/build.arm.cortex-a15/sources/glibc/libc

# Uncomment the line below if you want to do parallel build.
# PARALLELMFLAGS = -j 4

# This option is for those who modify the sources and keep them in a
# CVS repository.  Sometimes it is necessary to pass options to the cvs
# program (not the command), like -z9 or -x.
# CVSOPTS = -z9

all .DEFAULT:
	$(MAKE) -r PARALLELMFLAGS="$(PARALLELMFLAGS)" CVSOPTS="$(CVSOPTS)" -C $(srcdir) objdir=`pwd` $@

install:
	LANGUAGE=C LC_ALL=C; export LANGUAGE LC_ALL; \
	$(MAKE) -r PARALLELMFLAGS="$(PARALLELMFLAGS)" CVSOPTS="$(CVSOPTS)" -C $(srcdir) objdir=`pwd` $@
