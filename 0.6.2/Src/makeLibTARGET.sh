#! /bin/sh
 
##
## make MMS library for $TARGET (ECHO, ECHOB, FIRENZE, X10)
## 
## by raj.yadav (SISC) 
##

# Please setup the TARGET and other env using setup_env_target.sh, 
# followed by setenv_OP_$TARGET

##### set environment

echo ""
echo "======= START of MMS LIB (libmms.so) ======="
echo ""

function build_mms()
{
	BUILD_TYPE=$1
	BUILD_TYPE_FLAGS=$2

if [ -d $OPENSOURCE_DIR/Graphics/2DToolkit/Glib/2.28.0/Lib/$TARGET ]; then
	GLIB_PATH=$OPENSOURCE_DIR/Graphics/2DToolkit/Glib/2.28.0
else
	GLIB_PATH=$OPENSOURCE_DIR/Graphics/2DToolkit/Glib/2.22.5
fi

	export LDFLAGS="-L$GLIB_PATH/Lib/$TARGET/rel"

	export AR=${CROSS_COMPILE}ar
	export AS=${CROSS_COMPILE}as
	export LD=${CROSS_COMPILE}ld
	export RANLIB=${CROSS_COMPILE}ranlib
	export CC="${CROSS_COMPILE}gcc $TOOLCHAIN_CFLAGS"
	export CPP="${CROSS_COMPILE}cpp  $TOOLCHAIN_CFLAGS"
	export NM=${CROSS_COMPILE}nm
	export STRIP=${CROSS_COMPILE}strip
	export CFLAGS="$TOOLCHAIN_CFLAGS -fPIC $BUILD_TYPE_FLAGS"
	export PREFIX_PATH=$PWD/target

##### config
	make  distclean
	make  clean
	./configure \
		--prefix=$PREFIX_PATH \
		--enable-shared \
		--host=$TOOLCHAIN_HOST \
		--build=i686-linux

##### make
	make all
	make install

##### copy

	if [ ! -d `pwd`/../Lib/$TARGET/$BUILD_TYPE ]; then
		mkdir -vp `pwd`/../Lib/$TARGET/$BUILD_TYPE
	fi

	cp -arvf $PREFIX_PATH/lib/libmms.so* `pwd`/../Lib/$TARGET/$BUILD_TYPE
	cd `pwd`/../Lib/$TARGET/$BUILD_TYPE
	tar -cf libmms_0.6.2.tar libmms.so  libmms.so.0  libmms.so.0.0.2 
	cd -


}

build_mms "rel" "-D_FILE_OFFSET_BITS=64"
build_mms "dbg" "-DLIBMMS_DEBUG -D_FILE_OFFSET_BITS=64"

echo ""
echo "======= END of MMS LIB (libmms.so) ======="
echo ""

