#! /bin/sh
 
##
## make Gloox library for $TARGET (ECHO, ECHOB, FIRENZE, X10)
## 
## by raj.yadav (SISC) 
##

# Please setup the TARGET and other env using setup_env_target.sh, 
# followed by setenv_OP_$TARGET

##### set environment

OPENSSL_PATH=$OPENSOURCE_DIR/Connectivity/Protocol/OpenSSL/0.9.8o
OPTION_FLAG="-O2 -Wall -Wcast-align -Wcast-qual -Wextra -Wshadow -Wwrite-strings -fno-common -fno-omit-frame-pointer -fno-optimize-sibling-calls -fsigned-char -fstrict-overflow -Wno-ignored-qualifiers -Wno-array-bounds -Wno-empty-body -Wno-c++0x-compat"

export REL_MODE=rel
export CPPFLAGS="-I$OPENSSL_PATH/Src/include/openssl -I$OPENSOURCE_DIR/OPFixes/Inc -DOP_FIXES"
export CFLAGS=$CFLAGS" -I$OPENSOURCE_DIR/OPFixes/Inc -DOP_FIXES "
export LDFLAGS="-L$OPENSSL_PATH/Lib/$TARGET/rel/ -L$OPENSOURCE_DIR/OPFixes/Lib/$TARGET/$REL_MODE"
export LIBS="-ldl -lOPFixes"

export AR=${CROSS_COMPILE}ar
export AS=${CROSS_COMPILE}as
export LD=${CROSS_COMPILE}ld
export RANLIB=${CROSS_COMPILE}ranlib
export CC="${CROSS_COMPILE}gcc $TOOLCHAIN_CFLAGS"
export CPP="${CROSS_COMPILE}cpp $TOOLCHAIN_CFLAGS"
export CXX="${CROSS_COMPILE}g++ $TOOLCHAIN_CFLAGS ${OPTION_FLAG}"
export NM=${CROSS_COMPILE}nm
export STRIP=${CROSS_COMPILE}strip
export CFLAGS="$TOOLCHAIN_CFLAGS -fPIC ${OPTION_FLAG}"

export PREFIX_PATH=$PWD/target

########### Clean up
make clean
make  distclean

##### config
./configure \
--with-ssl=$PREFIX_DIR-$TARGET \
--host=$TOOLCHAIN_HOST \
--build=i686-linux \
--enable-shared=no \
--enable-static \
--prefix=$PREFIX_PATH

##### make
make
make install
 
##### copy

if [ ! -d `pwd`/../Lib/$TARGET ]; then
mkdir -v `pwd`/../Lib/$TARGET
mkdir -v `pwd`/../Lib/$TARGET/rel
fi

cp -frv `pwd`/target/lib/libgloox.a `pwd`/../Lib/$TARGET/rel/

