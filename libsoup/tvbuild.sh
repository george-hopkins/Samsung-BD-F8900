#!/bin/sh

# TV Browser build script
# common single build script : tvbuild.sh

_PWD=`pwd`
BROWSER_ROOT=`pwd`
pushd ./ > /dev/null
while [ ! -f "tvbuild.cfg" ]
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

source $BROWSER_ROOT/tvbuild.cfg

NAME="libsoup-2.33.6"
CONFIGURE_ARG="$CONFIGURE_ARG --without-gnome --enable-sqlite"
	
source $BROWSER_ROOT/tvbuild.exe $1 $2 || exit 1
