#! /bin/sh
srcdir=/home6/daewon2/Release/glibc/SP/SP_Linux/LP_Linux_Prj/REL/VDLinux.3.x.2012/toolchain/src/arm-mips-src-20121018/build.arm.cortex-a15/sources/cloog
PACKAGE=cloog
VERSION=0.15

if test -f $srcdir/.git/HEAD; then
    GIT_REPO="$srcdir/.git"
    GIT_HEAD_ID=`GIT_DIR=$GIT_REPO git describe`
elif test -f $srcdir/CLOOG_HEAD; then
    GIT_HEAD_ID=`cat $srcdir/CLOOG_HEAD`
else
    GIT_HEAD_ID="$PACKAGE-$VERSION-UNKNOWN"
fi

echo $GIT_HEAD_ID | sed -e 's/cloog-//'
