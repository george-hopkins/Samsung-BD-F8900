#!/bin/sh

HOSTCFLAGS="-Wall -Wmissing-prototypes -Wstrict-prototypes -O2 -fomit-frame-pointer"

gcc -v -Wp,-MD,scripts/basic/.fixdep.d $HOSTCFLAGS -o scripts/basic/fixdep scripts/basic/fixdep.c
gcc -v -Wp,-MD,scripts/.recordmcount.d $HOSTCFLAGS -o scripts/recordmcount scripts/recordmcount.c
gcc -v -Wp,-MD,scripts/mod/.file2alias.o.d $HOSTCFLAGS -c -o scripts/mod/file2alias.o scripts/mod/file2alias.c
gcc -v -Wp,-MD,scripts/mod/.modpost.o.d $HOSTCFLAGS -c -o scripts/mod/modpost.o scripts/mod/modpost.c
gcc -v -Wp,-MD,scripts/mod/.sumversion.o.d $HOSTCFLAGS -c -o scripts/mod/sumversion.o scripts/mod/sumversion.c
gcc -v -o scripts/mod/modpost scripts/mod/modpost.o scripts/mod/file2alias.o scripts/mod/sumversion.o
