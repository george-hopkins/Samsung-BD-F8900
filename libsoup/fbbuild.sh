#!/bin/bash

_PWD=`pwd`
BROWSER_ROOT=`pwd`
pushd ./ > /dev/null
while [ ! -f "fbbuild.cfg" ]
do
    cd ../
    BROWSER_ROOT=`pwd`
    if [ "$BROWSER_ROOT" == "/" ]; then
		cat ${_PWD}/err.msg
        exit 1
    fi
done
popd > /dev/null
cd ${_PWD}

source $BROWSER_ROOT/fbbuild.cfg

NAME="libsoup-2.31.90"
CONFIGURE_ARG="$CONFIGURE_ARG --with-libgcrypt-prefix=$BINDIR --without-gnome --disable-glibtest --enable-ssl --disable-static --enable-sqlite"

source $BROWSER_ROOT/fbbuild.exe $1 $2 || exit 1
