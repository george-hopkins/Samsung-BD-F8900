#! /bin/sh
 
##
## make Gloox library for FIRENZE 
## 
## by raj.yadav (SISC) 
##

#TARGET=FIRENZE
#OPENSOURCE_DIR=/data/pkumar/hq_perforce/Users/raj.yadav/OP/OP_Prj/REL

OPENSSL_PATH=$OPENSOURCE_DIR/Connectivity/Protocol/OpenSSL/0.9.8o/Src

##### set environment
export CPPFLAGS="-I$OPENSSL_PATH/include/openssl"
export LDFLAGS="-L$OPENSSL_PATH/../Lib/$TARGET"
export LIBS="-ldl"

export AR=${CROSS_COMPILE}ar
export AS=${CROSS_COMPILE}as
export LD=${CROSS_COMPILE}ld
export RANLIB=${CROSS_COMPILE}ranlib
export CC=${CROSS_COMPILE}gcc
export CPP=${CROSS_COMPILE}cpp
export NM=${CROSS_COMPILE}nm
export STRIP=${CROSS_COMPILE}strip
export CFLAGS="-Wno-ignored-qualifiers -Wno-array-bounds -Wno-empty-body $TOOLCHAIN_CFLAGS"
 
##### config
make  clean
./configure \
--with-openssl=$OPENSSL_PATH \
--host=$TOOLCHAIN_HOST \
--build=i686-linux \
--prefix=`pwd`/target

##### make
make
make install 
##### copy

if [ ! -d `pwd`/../Lib/$TARGET ]; then
mkdir -v `pwd`/../Lib/$TARGET

if [ ! -d `pwd`/../Lib/$TARGET/rel ]; then
mkdir -v `pwd`/../Lib/$TARGET/rel
fi
fi

cp -frv `pwd`/target/lib/libgloox.a `pwd`/../Lib/$TARGET/rel/


