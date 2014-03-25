export INSTALL_DIR="$PREFIX_DIR"
export PKG_CONFIG_PATH="$INSTALL_DIR/lib/pkgconfig"


export CFLAGS="-O3 -I$PREFIX_DIR/include"

export LDFLAGS="-L$PREFIX_DIR/lib"



./configure --host=$HOST --build=$BUILD --prefix=$INSTALL_DIR 


