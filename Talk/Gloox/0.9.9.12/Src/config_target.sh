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
export CFLAGS="$TOOLCHAIN_CFLAGS -fPIC ${OPTION_FLAG}"



##### config
if [$DEBUG_MODE=1]
then
./configure \
--enable-debug\
--with-ssl=$PREFIX_DIR \
--host=$HOST \
--build=i686-linux \
--enable-shared=no \
--enable-static \
--prefix=$PREFIX_DIR
 
else 
./configure \
--with-ssl=$PREFIX_DIR \
--host=$HOST \
--build=i686-linux \
--enable-shared=no \
--enable-static \
--prefix=$PREFIX_DIR
fi
 
##### copy

