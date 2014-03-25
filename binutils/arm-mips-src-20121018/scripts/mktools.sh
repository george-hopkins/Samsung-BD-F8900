#!/bin/bash
# This script builds toolchain for ARM cortex-A9

check_var () {
        if [[ -z "$1" ]]; then
                echo "Error: not defined first parameter for check_var() function:"
                echo "variable name"
                exit 1
        fi
        eval VARIABLE_VALUE="\$$1"

        if [ -z "$2" ]; then
                # Global has no description
                if [ -z "$VARIABLE_VALUE" ]; then
                        echo "Error: please specify $1 variable"
                        exit 1
                fi
        else
                # Additionally describe variable
                if [ -z "$VARIABLE_VALUE" ]; then
                        echo "Error: please specify $1 variable: $2"
                        exit 1
                fi
        fi
}

check_dir () {
        if [[ -z "$1" ]]; then
                echo "Error: not defined first parameter for check_dir() function:"
                echo "directory name"
                exit 1
        fi

        if [[ ! -d "$1" ]]; then
                echo "Error: can not find $1 directory"
                exit 1
        fi
}

check_file () {
        if [[ -z "$1" ]]; then
                echo "Error: not defined first parameter for check_file() function:"
                echo "file name"
                exit 1
        fi

        if [[ ! -f "$1" ]]; then
                echo "Error: can not find $1 file"
                exit 1
        fi
}

save_var () {
        if [[ -z "$1" ]]; then
                echo "Error: not defined input parameter for save_var() function"
                echo "variable name"
                exit 1
        fi
        VAR_NAME=$1
        eval VAR_VALUE="\$$1"

        eval ${VAR_NAME}_SAFE="$VAR_VALUE"
}

restore_var () {
        if [[ -z "$1" ]]; then
                echo "Error: not defined input parameter for restore_var() function:"
                echo "variable name"
                exit 1
        fi
        VAR_NAME=$1
        eval VAR_VALUE="\$$1"

        eval ${VAR_NAME}="\$${VAR_NAME}_SAFE"
}

SCRIPT_RUN_DIR=`pwd`
SCRIPT_BASE_NAME=`basename $0`
pushd `dirname $0`
SCRIPT_TOP_DIR=`pwd`
popd

#Set initial values
SRC_DIR__=
BUILD_DIR__=
INSTALL_DIR__=
NON_STANDARD=false

help(){
  echo "Usage $0 options"
  echo "By default sources are obtained from in ../sources directory"
  echo "By default toolchain is built in ../build.\${TARGET_CPU}.\${CPU_TUNE}.\${BUILD_VERSION}"
  echo "    where TARGET_CPU, CPU_TUNE, BUILD_VERSION variables is defined in config-file"
  echo "By default toolchain will be installed to /opt/vd folder"
  echo "Use -c to specify config file name. Availible configs are:"
  echo "`ls ${SCRIPT_TOP_DIR}/toolchain.cfg/*.config | sed -e "s/^/    /"`"
  echo "Use -s SRC_DIR__ to specific different source directory"
  echo "Use -b BUILD_DIR__ to specify a build folder"
  echo "Use -i INSTALL_DIR__ to specify a toolchain install directory"
  echo "Use -n to build toolchain on non VD-standard build environment"
  echo "Use -h print this help"
}

while getopts "c:s:b:i:hn" OPTION
do
  case $OPTION in
    c)
      CONFIG_FILE="$OPTARG"
      ;;
    n)
      NON_STANDARD=true
      ;;
    i)
      INSTALL_DIR__="$OPTARG"
      ;;
    h)
      help
      exit 0
      ;;
    s)
      SRC_DIR__="$OPTARG"
      ;;
    b)
      BUILD_DIR__="$OPTARG"
      ;;
	*)
#      echo "Unknown option $OPTION"
      echo "Error.Unknown option or missed argument"
      echo "To get help run $0 -h"
      exit
  esac
done

#Set default folder names if this folders are not set by dommand line options 
if [ -z "$CONFIG_FILE" ]; then
	echo "Please specify config file"
        help
	exit 1
fi

# Soursing a config file
check_file $CONFIG_FILE
source $CONFIG_FILE

# Check a config file
check_var "BUILD_DATE" "dashed date of build (e.g. 2003-01-30)"
check_var "PKG_VERSION" "label of package"
check_var "WITH_BUGURL" "bug report URL"
check_var "TARGET_CPU" "target CPU"
check_var "CPU_TUNE" "CPU tune"
check_var "CPU_TUNE_WITH_VER" "CPU tune with toolchain version"
check_var "TARGET" "target triplet"

# Set this option if it needs separately to build and to install more than one toolchain version
# on one computer simultaneously.
# This is service option is useful to toolchain development. 

#Set the file name of result packet
if [ -z "${BUILD_VERSION}" ]; then
  DOT_BUILD_VERSION=""
else
  DOT_BUILD_VERSION=".${BUILD_VERSION}"
fi

#Set default folder names
DEFAULT_SRC_DIR__="../sources"
DEFAULT_BUILD_DIR__="../build.${TARGET_CPU}.${CPU_TUNE}${DOT_BUILD_VERSION}"
DEFAULT_INSTALL_DIR__="/opt/vd"

if [ -z "$SRC_DIR__" ]; then
	SRC_DIR__=${DEFAULT_SRC_DIR__}
fi

if [ -z "$BUILD_DIR__" ]; then
	BUILD_DIR__=${DEFAULT_BUILD_DIR__}
fi

if [ -z "$INSTALL_DIR__" ]; then
	INSTALL_DIR__=${DEFAULT_INSTALL_DIR__}
fi

if ! [ -d "$SRC_DIR__" ]; then
	echo "Error. Can not find the sources folder $SRC_DIR__"
	exit
fi

#Clear build directory
#if [ -d "$BUILD_DIR__" ]; then
#	rm -rf $BUILD_DIR__	
#fi
#mkdir -p $BUILD_DIR__

if ! [ -d "$BUILD_DIR__" ]; then
	mkdir -p $BUILD_DIR__	
fi

if ! [ -d "$INSTALL_DIR__" ]; then
        mkdir -p $INSTALL_DIR__
fi

#Get absolute path/name of folders
pushd $SRC_DIR__
SRC_DIR=`pwd`
popd

pushd $BUILD_DIR__
BUILD_DIR=`pwd`
popd

pushd $INSTALL_DIR__
INSTALL_DIR=`pwd`
popd

MK_FLAGS=-j4

echo "Build the cross tools for ${TARGET_CPU} ${CPU_TUNE}"

#Host name settings
UNAME_M=`uname -m | tr -t [:upper:] [:lower:]`
HOST=${UNAME_M}-pc-linux-gnu
BUILD=${UNAME_M}-pc-linux-gnu

BUILD_OS=`uname`
echo Building on $BUILD_OS

# Add target-unspecific flags
GCC_SPECS='--with-specs=%{!ffunction-sections:%{!fno-function-sections:-ffunction-sections}} %{funwind-tables|fno-unwind-tables|mabi=*|ffreestanding|nostdlib:;:-funwind-tables} %{O3:%{!funswitch-loops:%{!fno-unswitch-loops:-fno-unswitch-loops}}}'

TOOLCHAIN_PREFIX="${FULL_TARGET_CPU_NAME}-${CPU_TUNE_WITH_VER}${DOT_BUILD_VERSION}"

PREFIX_CROSS="${INSTALL_DIR}/${TOOLCHAIN_PREFIX}"

#Set the dir with cross tools
TOOLS_DIR=$PREFIX_CROSS/bin

#Set the SYSROOT
SYSROOT=${PREFIX_CROSS}/${TARGET}/sys-root
SYSROOT_DIR=${SYSROOT}
CROSS_TOOLS_PREFIX="${TOOLS_DIR}/${TARGET}"

#Set target to make kernel headers
KERNEL_HEADER_ARCH=$TARGET_CPU

#Directory for temporary build ojects
OBJS_BUILD_DIR=${BUILD_DIR}/obj

#Directory to prepare the sources to build
SRC_BUILD_DIR=${BUILD_DIR}/sources

#Set log dir
LOG_DIR=${BUILD_DIR}/log

# This log files is needed to catch log messages because any defects in build script
GLOB_LOG_FILE=${LOG_DIR}/not_included_to_privite_logs.log

if ! [ -d "$OBJS_BUILD_DIR" ]; then
 mkdir -p $OBJS_BUILD_DIR
fi

if ! [ -d "$SRC_BUILD_DIR" ]; then
 mkdir -p $SRC_BUILD_DIR
fi

if ! [ -d "$LOG_DIR" ]; then
 mkdir -p $LOG_DIR
fi

TIME_LOG=$LOG_DIR/time_needded.log
echo "Start building toolchain at `date +%Y-%m-%d\ %X`" > $TIME_LOG


# Definitions of toolchan parts
ADD_HEADS_NAME=additional-headers

#Binutils definitions
BU_NAME=binutils

#CLOOG definitions
CLOOG_NAME=cloog
#CLOOG_NAME=cloog-ppl-0.15.11

#EXPAT definitions
EXPAT_NAME=expat

#GCC definitions
GCC_NAME=gcc

#GDB definitions
GDB_NAME=gdb

#EGLIBC definitions
EGLIBC_NAME=glibc

#GMP definitions
GMP_NAME=gmp

#KERNEL definitions
KERNEL_NAME=kernel

#LIBELF definitions
LIBELF_NAME=libelf

#LIBICONV definitions
LIBICONV_NAME=libiconv

#LIBTIRPC definitions
LIBTIRPC_NAME=libtirpc

#MPC definitions
MPC_NAME=mpc

#MPFR definitions
MPFR_NAME=mpfr

#PPL definitions
PPL_NAME=ppl

#ZLIB definitions
ZLIB_NAME=zlib





abort() {
    echo $1
    exec false
}

check_success() {
   if [ $? -ne 0 ]
   then
      echo Failed
      exit 1
   fi
   echo Done
}

#parameters
# $1 Source directory name
# $2 Destination directory name
# $3 if defined name of log file

copy_dir_clean(){

    if [ ! "$(ls -A $2)" ]; then

        if ! [ $3 ] ; then
                CP_D_LOG=${GLOB_LOG_FILE}
        else
                CP_D_LOG=$3
        fi

        echo "copy directory $1 to $2"
        echo "copy directory $1 to $2" >> $CP_D_LOG 2>&1

        mkdir -vp $2 >> $CP_D_LOG 2>&1

        pushd $1
         tar cf - --exclude=CVS --exclude=.svn --exclude=.git --exclude="*.orig" --exclude="*.rej" * | ( cd $2 && tar xvpf - >> $CP_D_LOG 2>&1 )  >> $CP_D_LOG 2>&1
        popd
    fi
}


#parameters
# $1 directory name
# $2 if defined name of log file
make_clean_dir ()
{
#echo 1=$1
#echo 2=$2

	if ! [ $2 ] ; then
		MK_CL_LOG=${GLOB_LOG_FILE}
	else
		MK_CL_LOG=$2
	fi
#echo MK_CL_LOG=$MK_CL_LOG

    if [ -n $1 ] ; then
		if [ -d $1 ]; then
             rm -rfv $1 >> $MK_CL_LOG 2>&1;
		fi
		mkdir -pv $1 >> $MK_CL_LOG;
    fi
}

# common function to build tools
# this function do configure, make, make install.
# if you need to do some additional action, you should write your own wrapper.
# arguments:
#           $1 - source dir of tool
#           $2 - list of option to ./configure script
#           $3 - file to output log

mk_tool () {

	if ! [ -d $1 ]; then
		abort "Error: check the $1"
	fi

	if [ "$3" == "" ]; then
		TOOL_LOG=${GLOB_LOG_FILE}
	else
		TOOL_LOG=$3
	fi

	echo "Building : $1"
	echo "Building : $1" >> $TOOL_LOG

	TOOL_SRC_DIR=`basename $1`
    TOOL_BUILD_DIR="$OBJS_BUILD_DIR/$TOOL_SRC_DIR"

#        pushd $BUILD_DIR

    make_clean_dir $TOOL_BUILD_DIR $TOOL_LOG

    pushd $TOOL_BUILD_DIR

    echo "Configure $TOOL_SRC_DIR"
    echo "Configure $TOOL_SRC_DIR" >> $TOOL_LOG 2>&1

    LDFLAGS=$LDFLAGS CPPFLAGS=$CPPFLAGS $1/configure $2  >> $TOOL_LOG 2>&1
    check_success

    echo "Make $TOOL_SRC_DIR"
    echo "Make $TOOL_SRC_DIR" >> $TOOL_LOG

    make ${MK_FLAGS} >> $TOOL_LOG 2>&1
	check_success

    echo "Install $TOOL_SRC_DIR"
    echo "Install $TOOL_SRC_DIR" >> $TOOL_LOG

    make install >> $TOOL_LOG 2>&1
	check_success

	popd
}


# common function to log start time of stage
#           $1 - String to log of time
echo_start(){
	if [ "x$1" == "x" ]; then
	echo "Internal error. Need to define parameter for function $0"
	exit
	fi

	echo "Start make $1 at `date +%Y-%m-%d\ %X`" >> $TIME_LOG
	echo "Start make $1 at `date +%Y-%m-%d\ %X`"

}

# common function to log finish time of stage
#           $1 - String to log of time
echo_stop(){
	if [ "x$1" == "x" ]; then
	echo "Internal error. Need to define parameter for function $0"
	exit
	fi

	echo "Stop  make $1 at `date +%Y-%m-%d\ %X`" >> $TIME_LOG
	echo "Stop  make $1 at `date +%Y-%m-%d\ %X`"
}



# common function: create log file, create a copy of source folder
#           $1 - Name of module used as source folder
#           $2 - Name of folder to copy source of module to build
#           $3 - Log file name
prepare_to_build(){

	if [ -z $3 ]; then
	abort "Internal error. Need to define 3 parameter for function $0"
	fi

#	if [ -z $2 ]; then
#	abort "Internal error. Need to define 2 parameter for function $0"
#	fi

	if ! [ -d "${SRC_DIR}/${1}" ]; then
		abort "Internal error. Can not found a source folder {SRC_DIR}/${1} "
	fi

	_TOOL_LOG="${LOG_DIR}/${3}"

	if [ -f ${_TOOL_LOG} ]; then
	    rm -v ${_TOOL_LOG}
	fi;

#prepare folder for binutils sources
	make_clean_dir "${SRC_BUILD_DIR}/${2}" "${_TOOL_LOG}"
#Get sources from sources repository
	copy_dir_clean "${SRC_DIR}/${1}" "${SRC_BUILD_DIR}/${2}" "${_TOOL_LOG}"

}


check_prefix(){
#If $PREFIX_CROSS is present then make compressed tar to save previous version of toolchain
# then clean directory to prepare to install new one

	if [ -d "$PREFIX_CROSS" ]; then
		echo "Found: $PREFIX_CROSS"		
		pushd ${INSTALL_DIR}
		rm -rf ${TOOLCHAIN_PREFIX}
		popd

	else
		echo "Not found: $PREFIX_CROSS"
	fi

}

mk_binutls() {

echo_start ${BU_NAME}

   prepare_to_build ${BU_NAME} ${BU_NAME} "${BU_NAME}.log"

    save_var "CPPFLAGS"
    save_var "LDFLAGS"

    LDFLAGS="-L${PREFIX_CROSS}/lib"
    CPPFLAGS="-I${PREFIX_CROSS}/include"

    make_clean_dir "$OBJS_BUILD_DIR/${BU_NAME}" "${LOG_DIR}/${BU_NAME}.log" 2>&1

    pushd "$OBJS_BUILD_DIR/${BU_NAME}" 

    echo "Configure ${BU_NAME}"
    echo "Configure ${BU_NAME}" >> "${LOG_DIR}/${BU_NAME}.log" 2>&1

    LDFLAGS=$LDFLAGS CPPFLAGS=$CPPFLAGS \
    ${SRC_BUILD_DIR}/${BU_NAME}/configure \
      --prefix=$PREFIX_CROSS \
      --build=$BUILD \
      --target=$TARGET \
      --host=$HOST \
      --with-pkgversion=${PKG_VERSION} \
      --with-bugurl=${WITH_BUGURL} \
      --disable-nls \
      --with-sysroot=$SYSROOT \
      --enable-poison-system-directories \
      --disable-werror  >> "${LOG_DIR}/${BU_NAME}.log" 2>&1
      check_success

    echo "Make ${BU_NAME}"
    echo "Make ${BU_NAME}" >> "${LOG_DIR}/${BU_NAME}.log" 2>&1

    LDFLAGS=$LDFLAGS CPPFLAGS=$CPPFLAGS \
         make ${MK_FLAGS} >> "${LOG_DIR}/${BU_NAME}.log" 2>&1
    check_success

    echo "Install ${BU_NAME}"
    echo "Install ${BU_NAME}" >> "${LOG_DIR}/${BU_NAME}.log" 2>&1

    LDFLAGS=$LDFLAGS CPPFLAGS=$CPPFLAGS \
         make install >> "${LOG_DIR}/${BU_NAME}.log" 2>&1
    check_success

    restore_var "CPPFLAGS"
    restore_var "LDFLAGS"

    popd
echo_stop ${BU_NAME}
}

mk_gmp(){

    echo_start ${GMP_NAME}

    save_var "CPPFLAGS"

    prepare_to_build ${GMP_NAME} ${GMP_NAME} "${GMP_NAME}.log"

    CPPFLAGS=-fexceptions
    mk_tool "${SRC_BUILD_DIR}/${GMP_NAME}" "--prefix=$PREFIX_CROSS \
                          --enable-cxx \
                          --disable-shared \
                          --disable-nls \
                          --enable-static" "${LOG_DIR}/${GMP_NAME}.log"
    CPPFLAGS=""

    #                          --build=$BUILD \
    #                          --host=$HOST \

    restore_var "CPPFLAGS"

    echo_stop ${GMP_NAME}
}

mk_mpfr(){

echo_start ${MPFR_NAME}

prepare_to_build ${MPFR_NAME} ${MPFR_NAME} "${MPFR_NAME}.log"


    mk_tool "${SRC_BUILD_DIR}/${MPFR_NAME}" "--prefix=$PREFIX_CROSS \
                      --with-gmp=$PREFIX_CROSS \
                       --disable-shared \
                       --disable-nls\
                       --with-gmp=$PREFIX_CROSS \
                       --enable-static" "${LOG_DIR}/${MPFR_NAME}.log"

#                       --build=$BUILD \
#                       --host=$HOST \
echo_stop ${MPFR_NAME}
}

mk_mpc(){
echo_start ${MPC_NAME}

prepare_to_build ${MPC_NAME} ${MPC_NAME} "${MPC_NAME}.log"

    mk_tool "${SRC_BUILD_DIR}/${MPC_NAME}" "--prefix=$PREFIX_CROSS \
                      --with-gmp=$PREFIX_CROSS \
                      --with-mpfr=$PREFIX_CROSS \
                      --disable-shared \
                      --disable-nls \
                      --with-gmp=$PREFIX_CROSS \
                      --with-mpfr=$PREFIX_CROSS \
                      --enable-static" "${LOG_DIR}/${MPC_NAME}.log"

#                      --build=$BUILD \
#                      --host=$HOST \
echo_stop ${MPC_NAME}
}

mk_ppl(){
echo_start ${PPL_NAME}

prepare_to_build ${PPL_NAME} ${PPL_NAME} "${PPL_NAME}.log"

    mk_tool "${SRC_BUILD_DIR}/${PPL_NAME}" "--prefix=$PREFIX_CROSS \
                      --with-gmp=$PREFIX_CROSS \
                      --disable-shared \
                      --disable-watchdog \
                      --enable-static \
                      --disable-nls \
                      --with-gmp=$PREFIX_CROSS \
                     CPPFLAGS=-I$PREFIX_CROSS/include \
                      LDFLAGS=-L$PREFIX_CROSS/lib " "${LOG_DIR}/${PPL_NAME}.log"

#                      --build=$BUILD \
#                      --host=$HOST \
echo_stop ${PPL_NAME}
}

mk_cloog(){
echo_start ${CLOOG_NAME}

prepare_to_build ${CLOOG_NAME} ${CLOOG_NAME} "${CLOOG_NAME}.log"

        if [ "$NON_STANDARD" == "true" ]
        then
          pushd "${SRC_BUILD_DIR}/${CLOOG_NAME}"
          ./autogen.sh
          popd
        fi

	LDFLAGS_BEFORE=$LDFLAGS

LDFLAGS="-lstdc++ -lm -L$PREFIX_CROSS/lib"

#     mk_tool "${SRC_BUILD_DIR}/${CLOOG_NAME}"  "--prefix=$PREFIX_CROSS \
#                      --with-ppl-prefix=$PREFIX_CROSS \
#                      --with-gmp-prefix=$PREFIX_CROSS \
#                      --with-bits=gmp \
#                      --disable-shared \
#                      --enable-static \
#                       --disable-nls \
#                       CPPFLAGS=-I$PREFIX_CROSS/include " "${LOG_DIR}/${CLOOG_NAME}.log"

    mk_tool "${SRC_BUILD_DIR}/${CLOOG_NAME}"  "--prefix=$PREFIX_CROSS \
                     --with-ppl=$PREFIX_CROSS \
                     --with-gmp=$PREFIX_CROSS \
                     --with-bits=gmp \
                     --disable-shared \
                     --enable-static \
                      --disable-nls \
                      CPPFLAGS=-I$PREFIX_CROSS/include " "${LOG_DIR}/${CLOOG_NAME}.log"





#                      --build=$BUILD \
#                      --host=$HOST \

    LDFLAGS="$LDFLAGS_BEFORE"

echo_stop ${CLOOG_NAME}
}


mk_libelf(){
echo_start ${LIBELF_NAME}

prepare_to_build ${LIBELF_NAME} ${LIBELF_NAME} "${LIBELF_NAME}.log"

    mk_tool "${SRC_BUILD_DIR}/${LIBELF_NAME}" "--prefix=$PREFIX_CROSS \
	                   --disable-shared " "${LOG_DIR}/${LIBELF_NAME}.log"

echo_stop ${LIBELF_NAME}
}


mk_sysroot_dir(){

	mkdir -pv ${SYSROOT_DIR}/./

	if ! [ -d "${SYSROOT_DIR}/./" ]; then
	 abort "Error: cannnot create the sysroot dir '$SYSROOT_DIR'"
	fi

	#mkdir -pv ${SYSROOT_DIR}/soft-float/

	#if ! [ -d "${SYSROOT_DIR}/soft-float/" ]; then
	# abort "Error: cannot create the sysroot dir '$SYSROOT_DIR'/soft-float"
	#fi
}


mk_kernel_headers_eglibc(){

echo_start ${KERNEL_NAME}	

prepare_to_build ${KERNEL_NAME} "${KERNEL_NAME}-headers" "${KERNEL_NAME}-headers.log"


	pushd "${SRC_BUILD_DIR}/${KERNEL_NAME}-headers"

	make ARCH=${KERNEL_HEADER_ARCH} CROSS_COMPILE=${CROSS_TOOLS_PREFIX}- INSTALL_HDR_PATH="${SYSROOT_DIR}/./usr" headers_install >> "${LOG_DIR}/${KERNEL_NAME}-headers.log" 2>&1
	check_success

	rm -v ${SYSROOT_DIR}/./usr/include/.install \
              ${SYSROOT_DIR}/./usr/include/..install.cmd >> "${LOG_DIR}/${KERNEL_NAME}-headers.log" 2>&1

	popd
echo_stop ${KERNEL_NAME}

}


mk_gcc_1(){
echo_start "${GCC_NAME}_1"	

prepare_to_build ${GCC_NAME} "${GCC_NAME}_1" "${GCC_NAME}_1.log"

	GCC_1_BUILD_DIR="$OBJS_BUILD_DIR/${GCC_NAME}_1"

	make_clean_dir $GCC_1_BUILD_DIR "${LOG_DIR}/${GCC_NAME}_1.log"


	pushd $GCC_1_BUILD_DIR

    echo "Configure GCC stage 1"
    echo "Configure GCC stage 1" >> "${LOG_DIR}/${GCC_NAME}_1.log"

            PATH=${TOOLS_DIR}:$PATH \
            LDFLAGS=-L$PREFIX_CROSS/lib \
            "${SRC_BUILD_DIR}/${GCC_NAME}_1"/configure \
                --build=$BUILD \
                --host=$HOST \
                --target=$TARGET \
                --enable-__cxa_atexit \
                --with-host-libstdcxx='-static-libgcc -Wl,-Bstatic,-lstdc++,-Bdynamic -lm' \
                --disable-libmudflap \
                --disable-libssp \
                --disable-libstdcxx-pch \
                --with-gnu-as \
                --with-gnu-ld \
                --enable-lto \
                --disable-nls \
                --prefix=$PREFIX_CROSS \
                --disable-shared \
                --disable-threads \
                --disable-libssp \
                --disable-libgomp \
                --without-headers \
                --with-newlib \
                --disable-decimal-float \
                --disable-libffi \
                --disable-libquadmath \
                --enable-languages=c \
                --with-sysroot=$SYSROOT \
                --with-gmp=$PREFIX_CROSS \
                --with-mpfr=$PREFIX_CROSS \
                --with-ppl=$PREFIX_CROSS \
                --with-cloog=$PREFIX_CROSS \
                --with-libelf=$PREFIX_CROSS \
		"$GCC_SPECS" \
		$GCC_TARGET_SPECIFIC_FLAGS >> "${LOG_DIR}/${GCC_NAME}_1.log" 2>&1
                check_success

	echo "Make GCC stage 1"
	echo "Make GCC stage 1">> "${LOG_DIR}/${GCC_NAME}_1.log"

	LD_LIBRARY_PATH=$PREFIX_CROSS/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH} make ${MK_FLAGS} >> ${LOG_DIR}/${GCC_NAME}_1.log 2>&1
	check_success

	echo "Install GCC stage 1"
	echo "Install GCC stage 1">> "${LOG_DIR}/${GCC_NAME}_1.log"

	LD_LIBRARY_PATH=$PREFIX_CROSS/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH} make ${MK_FLAGS} install  >> "${LOG_DIR}/${GCC_NAME}_1.log" 2>&1
		check_success

	popd
echo_stop "${GCC_NAME}_1"
}


prep_eglibc(){
echo_start "prepare_${EGLIBC_NAME}"

	EGLIBC_PREP_LOG="${LOG_DIR}/prep_${EGLIBC_NAME}.log"
	echo "Prepare EGLIBC source codes" > "${EGLIBC_PREP_LOG}"

	make_clean_dir "${SRC_BUILD_DIR}/${EGLIBC_NAME}" "${EGLIBC_PREP_LOG}"

	copy_dir_clean "${SRC_DIR}/${EGLIBC_NAME}" "${SRC_BUILD_DIR}/${EGLIBC_NAME}"  ${EGLIBC_PREP_LOG}

#	move port folder
	echo "move port folder" >> "${EGLIBC_PREP_LOG}"
	make_clean_dir "${SRC_BUILD_DIR}/${EGLIBC_NAME}/libc/ports" "${EGLIBC_PREP_LOG}"
	copy_dir_clean "${SRC_BUILD_DIR}/${EGLIBC_NAME}/ports" "${SRC_BUILD_DIR}/${EGLIBC_NAME}/libc/ports" ${EGLIBC_PREP_LOG}
	rm -rvf "${SRC_BUILD_DIR}/${EGLIBC_NAME}/ports" >> "${EGLIBC_PREP_LOG}" 2>&1

echo_stop "prepare_${EGLIBC_NAME}"
}

mk_eglibc_0_headers(){
echo_start "eglibc_0_headers"

	EGLIBC_0_HEAD_LOG="${LOG_DIR}/${EGLIBC_NAME}_0_head.log"
	EGLIBC_0_HEAD_BUILD_DIR="$OBJS_BUILD_DIR/${EGLIBC_NAME}_0_head"

	make_clean_dir $EGLIBC_0_HEAD_BUILD_DIR ${EGLIBC_0_HEAD_LOG}


	pushd $EGLIBC_0_HEAD_BUILD_DIR

    echo "Configure EGLIBC_0 headers "
    echo "Configure EGLIBC_0 headers " > ${EGLIBC_0_HEAD_LOG}


            LD_LIBRARY_PATH=$PREFIX_CROSS/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH} \
            BUILD_CC=gcc \
            CC=${CROSS_TOOLS_PREFIX}-gcc  \
            CXX=${CROSS_TOOLS_PREFIX}-g++  \
            CFLAGS='-g -O2' \
            AR=${CROSS_TOOLS_PREFIX}-ar \
            RANLIB=${CROSS_TOOLS_PREFIX}-ranlib \
            ${SRC_BUILD_DIR}/${EGLIBC_NAME}/libc/configure --prefix=/usr \
	    ${GLIBC_TARGET_SPECIFIC_FLAGS} \
            --with-headers=${SYSROOT_DIR}/./usr/include \
            --build=${BUILD} \
            --host=${TARGET} \
            --disable-profile \
            --without-gd \
            --without-cvs \
            --disable-sanity-checks \
            --enable-hacker-mode \
            --enable-add-ons=ports,nptl \
            --enable-kernel=$KERNEL_VERSION  >> ${EGLIBC_0_HEAD_LOG} 2>&1
            check_success

	echo "Make EGLIBC_0 headers "
	echo "Make EGLIBC_0 headers " >> ${EGLIBC_0_HEAD_LOG}

	LD_LIBRARY_PATH=$PREFIX_CROSS/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH} \
	BUILD_CC=gcc \
	CC=${CROSS_TOOLS_PREFIX}-gcc  \
	CXX=${CROSS_TOOLS_PREFIX}-g++  \
	CFLAGS='-g -O2' \
	AR=${CROSS_TOOLS_PREFIX}-ar \
	RANLIB=${CROSS_TOOLS_PREFIX}-ranlib \
	make install_root=${SYSROOT_DIR}/./ install-bootstrap-headers=yes install-headers >> ${EGLIBC_0_HEAD_LOG} 2>&1
	check_success

	echo "Make EGLIBC_0 crtxx staff "
	echo "Make EGLIBC_0 crtxx staff " >> ${EGLIBC_0_HEAD_LOG}

	LD_LIBRARY_PATH=$PREFIX_CROSS/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH} \
	BUILD_CC=gcc \
	CC=${CROSS_TOOLS_PREFIX}-gcc  \
	CXX=${CROSS_TOOLS_PREFIX}-g++  \
	CFLAGS='-g -O2' \
	AR=${CROSS_TOOLS_PREFIX}-ar \
	RANLIB=${CROSS_TOOLS_PREFIX}-ranlib \
	make csu/subdir_lib >> ${EGLIBC_0_HEAD_LOG} 2>&1
	check_success

	make_clean_dir ${SYSROOT_DIR}/./usr/lib ${EGLIBC_0_HEAD_LOG}

	cp -v csu/crt1.o csu/crti.o csu/crtn.o ${SYSROOT_DIR}/./usr/lib/ >> ${EGLIBC_0_HEAD_LOG} 2>&1
	check_success

	popd

	echo "Make fake libc.so"
	echo "Make fake libc.so " >> ${EGLIBC_0_HEAD_LOG}

	${CROSS_TOOLS_PREFIX}-gcc -o ${SYSROOT_DIR}/./usr/lib/./libc.so -nostdlib -nostartfiles -shared -x c /dev/null >> ${EGLIBC_0_HEAD_LOG} 2>&1
	check_success

echo_stop "eglibc_0_headers"
}

mk_gcc_2(){
echo_start "${GCC_NAME}_2"

	GCC_2_BUILD_DIR="$OBJS_BUILD_DIR/${GCC_NAME}_2"
	GCC_2_LOG="${LOG_DIR}/${GCC_NAME}_2.log"

	make_clean_dir $GCC_2_BUILD_DIR "${GCC_2_LOG}"

	echo "Build GCC stage 2"
	echo "Build GCC stage 2" > ${GCC_2_LOG}
	
	pushd $GCC_2_BUILD_DIR

    echo "Configure GCC stage 2"
    echo "Configure GCC stage 2" >> ${GCC_2_LOG}

    #We will use sources of GCC has been prepared before in GCC_1 stage
            PATH=$TOOLS_DIR:$PATH \
            LDFLAGS=-L$PREFIX_CROSS/lib \
            "${SRC_BUILD_DIR}/${GCC_NAME}_1"/configure \
            --build=$BUILD \
            --host=$HOST \
            --target=$TARGET \
            --prefix=$PREFIX_CROSS \
            --with-sysroot=$SYSROOT \
            --enable-threads \
            --disable-libssp \
            --disable-libstdcxx-pch \
            --enable-languages=c \
            --enable-shared \
            --enable-lto \
            --enable-symvers=gnu \
            --enable-__cxa_atexit \
            --disable-nls \
            --disable-libffi \
            --disable-libquadmath \
            --with-host-libstdcxx='-static-libgcc -Wl,-Bstatic,-lstdc++,-Bdynamic -lm' \
            --disable-libmudflap \
            --disable-libgomp \
            --with-gmp=$PREFIX_CROSS \
            --with-mpfr=$PREFIX_CROSS \
            --with-ppl=$PREFIX_CROSS \
            --with-cloog=$PREFIX_CROSS \
            --with-libelf=$PREFIX_CROSS \
	    "$GCC_SPECS" \
	    $GCC_TARGET_SPECIFIC_FLAGS >> ${GCC_2_LOG} 2>&1
            check_success

	echo "Make GCC stage 2"
	echo "Make GCC stage 2" >> ${GCC_2_LOG}

	PATH=${TOOLS_DIR}:$PATH LD_LIBRARY_PATH=$PREFIX_CROSS/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH} make ${MK_FLAGS} LDFLAGS_FOR_TARGET=--sysroot=$SYSROOT CPPFLAGS_FOR_TARGET=--sysroot=$SYSROOT build_tooldir=${TOOLS_DIR} >> ${GCC_2_LOG} 2>&1
	check_success

	echo "Install GCC stage 2"
	echo "Install GCC stage 2" >> ${GCC_2_LOG}

	PATH=${TOOLS_DIR}:$PATH LD_LIBRARY_PATH=$PREFIX_CROSS/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH} make ${MK_FLAGS} install >> ${GCC_2_LOG} 2>&1
	check_success

	popd

echo_stop "${GCC_NAME}_2"
}

mk_eglibc_0_final(){
echo_start "${EGLIBC_NAME}_0_final"

	EGLIBC_0_FINAL_BUILD_DIR="$OBJS_BUILD_DIR/${EGLIBC_NAME}_0_final"
	EGLIBC_0_FINAL_LOG="${LOG_DIR}/${EGLIBC_NAME}_0_final.log"

	make_clean_dir $EGLIBC_0_FINAL_BUILD_DIR ${EGLIBC_0_FINAL_LOG}

	pushd $EGLIBC_0_FINAL_BUILD_DIR

     echo "Configure EGLIBC_0 final "
     echo "Configure EGLIBC_0 final " > ${EGLIBC_0_FINAL_LOG}

     if [[ "x${BUILD_OS}" == "xLinux" ]]; then
	# Linux hosts

	    LD_LIBRARY_PATH=$PREFIX_CROSS/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH} \
            BUILD_CC=gcc \
            CC=${CROSS_TOOLS_PREFIX}-gcc  \
            CXX=${CROSS_TOOLS_PREFIX}-g++  \
            CFLAGS='-g -O2' \
            AR=${CROSS_TOOLS_PREFIX}-ar \
            RANLIB=${CROSS_TOOLS_PREFIX}-ranlib \
            ${SRC_BUILD_DIR}/${EGLIBC_NAME}/libc/configure --prefix=/usr \
	    ${GLIBC_TARGET_SPECIFIC_FLAGS} \
            --with-headers=$SYSROOT/./usr/include \
            --build=$BUILD \
            --host=$TARGET \
            --disable-profile \
            --without-gd    \
            --without-cvs  \
            --enable-add-ons=nptl,ports \
            --enable-kernel=$KERNEL_VERSION \
            --with-pkgversion=${PKG_VERSION} \
            --disable-multi-arch  >> ${EGLIBC_0_FINAL_LOG} 2>&1
            check_success
    else
	# Other hosts (e.g. cygwin)
	# an ugly spigot for cygwin build compatability
	
	    LD_LIBRARY_PATH=$PREFIX_CROSS/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH} \
            BUILD_CC=gcc \
            CC=${CROSS_TOOLS_PREFIX}-gcc  \
            CXX=${CROSS_TOOLS_PREFIX}-g++  \
            CFLAGS='-g -O2' \
            AR=${CROSS_TOOLS_PREFIX}-ar \
            RANLIB=${CROSS_TOOLS_PREFIX}-ranlib \
            ${SRC_BUILD_DIR}/${EGLIBC_NAME}/libc/configure --prefix=/usr \
            ${GLIBC_TARGET_SPECIFIC_FLAGS} \
            --with-headers=$SYSROOT/./usr/include \
            --host=$TARGET \
            --disable-profile \
            --without-gd    \
            --without-cvs  \
            --enable-add-ons=nptl,ports \
            --enable-kernel=$KERNEL_VERSION \
            --with-pkgversion=${PKG_VERSION} \
            --disable-multi-arch  >> ${EGLIBC_0_FINAL_LOG} 2>&1
            check_success
    fi

	echo "Build EGLIBC_0 final "
	echo "Build EGLIBC_0 final " >> ${EGLIBC_0_FINAL_LOG}

	LD_LIBRARY_PATH=$PREFIX_CROSS/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH} \
	BUILD_CC=gcc \
	CC=${CROSS_TOOLS_PREFIX}-gcc  \
	CXX=${CROSS_TOOLS_PREFIX}-g++  \
	CFLAGS='-g -O2' \
	AR=${CROSS_TOOLS_PREFIX}-ar \
	RANLIB=${CROSS_TOOLS_PREFIX}-ranlib \
	PARALLELMFLAGS=${MK_FLAGS} \
	make >> ${EGLIBC_0_FINAL_LOG} 2>&1
	check_success

	echo "Install EGLIBC_0 final "
	echo "Install EGLIBC_0 final " >> ${EGLIBC_0_FINAL_LOG}


	LD_LIBRARY_PATH=$PREFIX_CROSS/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH} \
	BUILD_CC=gcc \
	CC=${CROSS_TOOLS_PREFIX}-gcc  \
	CXX=${CROSS_TOOLS_PREFIX}-g++  \
	CFLAGS='-g -O2' \
	AR=${CROSS_TOOLS_PREFIX}-ar \
	RANLIB=${CROSS_TOOLS_PREFIX}-ranlib \
	make install_root=${SYSROOT_DIR} install   >> ${EGLIBC_0_FINAL_LOG} 2>&1
	check_success

	popd
echo_stop "${EGLIBC_NAME}_0_final"
}


mk_eglibc_localedef(){
    echo_start "EGLIBC_localedef"

	LOCALEDEF_LOG="${LOG_DIR}/${EGLIBC_NAME}_localedef.log"
	LOCALEDEF_BUILD_DIR="$OBJS_BUILD_DIR/${EGLIBC_NAME}_localedef"
	LOCALEDEF_EGLIBC_0_SRC_DIR="${SRC_BUILD_DIR}/${EGLIBC_NAME}/libc"

	echo "Build EGLIBC localedef "
	echo "Build EGLIBC localedef " > ${LOCALEDEF_LOG}


    make_clean_dir $LOCALEDEF_BUILD_DIR ${LOCALEDEF_LOG}
    pushd $LOCALEDEF_BUILD_DIR

    save_var "CPPFLAGS"

     echo "Configure EGLIBC localedef "
     echo "Configure EGLIBC localedef " >> ${LOCALEDEF_LOG}

            if [ "$BUILD_OS" = "Linux" ]; then
                # XXX: Need to define _GNU_SOURCE for correct locale_t detection on some Linux hosts
                CPPFLAGS_LOCALEDEF="-D_GNU_SOURCE"
            fi

            CPPFLAGS="$CPPFLAGS_LOCALEDEF" ${SRC_BUILD_DIR}/${EGLIBC_NAME}/localedef/configure --prefix=/usr \
            --with-glibc=${LOCALEDEF_EGLIBC_0_SRC_DIR} >> ${LOCALEDEF_LOG} 2>&1
            check_success

	echo "Make EGLIBC localedef "
	echo "Make EGLIBC localedef " >> ${LOCALEDEF_LOG}

	make ${MK_FLAGS} >> ${LOCALEDEF_LOG} 2>&1
	check_success

	echo "Install EGLIBC 0 localedef "
	echo "Install EGLIBC 0 localedef " >> ${LOCALEDEF_LOG}

	make ${MK_FLAGS} install_root=$LOCALEDEF_BUILD_DIR/little4 'LOCALEDEF_OPTS=--little-endian --uint32-align=4' install-locales >> ${LOCALEDEF_LOG} 2>&1
	check_success

LODEF_FLAGS="--quiet -c --little-endian --uint32-align=4"
GLIBC_LOCALES="glibc/localedata/locales"
GLIBC_CHARMAPS="glibc/localedata/charmaps"
LODEF_TEST_DIR="little4/usr/lib/locale/test"

	make_clean_dir $LODEF_TEST_DIR  ${LOCALEDEF_LOG}
	cat > $LODEF_TEST_DIR/README <<'EOF0'
	The locales in this directory are used by the GLIBC test harness
EOF0


I18NPATH=${LOCALEDEF_EGLIBC_0_SRC_DIR}/localedata GCONV_PATH=iconvdata \
./localedef ${LODEF_FLAGS} -i ${GLIBC_LOCALES}/de_DE -f ${GLIBC_CHARMAPS}/ISO-8859-1 ${LODEF_TEST_DIR}/de_DE.ISO-8859-1 >> ${LOCALEDEF_LOG} 2>&1
	check_success

I18NPATH=${LOCALEDEF_EGLIBC_0_SRC_DIR}/localedata GCONV_PATH=iconvdata \
./localedef ${LODEF_FLAGS} -i ${GLIBC_LOCALES}/de_DE -f ${GLIBC_CHARMAPS}/UTF-8 ${LODEF_TEST_DIR}/de_DE.UTF-8 >> ${LOCALEDEF_LOG} 2>&1
	check_success

I18NPATH=${LOCALEDEF_EGLIBC_0_SRC_DIR}/localedata GCONV_PATH=iconvdata \
./localedef ${LODEF_FLAGS} -i ${GLIBC_LOCALES}/en_US -f ${GLIBC_CHARMAPS}/ANSI_X3.4-1968 ${LODEF_TEST_DIR}/en_US.ANSI_X3.4-1968 >> ${LOCALEDEF_LOG} 2>&1
	check_success

I18NPATH=${LOCALEDEF_EGLIBC_0_SRC_DIR}/localedata GCONV_PATH=iconvdata \
./localedef ${LODEF_FLAGS} -i ${GLIBC_LOCALES}/en_US -f ${GLIBC_CHARMAPS}/ISO-8859-1 ${LODEF_TEST_DIR}/en_US.ISO-8859-1 >> ${LOCALEDEF_LOG} 2>&1
	check_success

I18NPATH=${LOCALEDEF_EGLIBC_0_SRC_DIR}/localedata GCONV_PATH=iconvdata \
./localedef ${LODEF_FLAGS} -i ${GLIBC_LOCALES}/en_US -f ${GLIBC_CHARMAPS}/UTF-8 ${LODEF_TEST_DIR}/en_US.UTF-8 >> ${LOCALEDEF_LOG} 2>&1
	check_success

I18NPATH=${LOCALEDEF_EGLIBC_0_SRC_DIR}/localedata GCONV_PATH=iconvdata \
./localedef ${LODEF_FLAGS} -i ${GLIBC_LOCALES}/ja_JP -f ${GLIBC_CHARMAPS}/EUC-JP ${LODEF_TEST_DIR}/ja_JP.EUC-JP >> ${LOCALEDEF_LOG} 2>&1
	check_success

I18NPATH=${LOCALEDEF_EGLIBC_0_SRC_DIR}/localedata GCONV_PATH=iconvdata \
./localedef ${LODEF_FLAGS} -i ${GLIBC_LOCALES}/da_DK -f ${GLIBC_CHARMAPS}/ISO-8859-1 ${LODEF_TEST_DIR}/da_DK.ISO-8859-1 >> ${LOCALEDEF_LOG} 2>&1
	check_success

I18NPATH=${LOCALEDEF_EGLIBC_0_SRC_DIR}/localedata GCONV_PATH=iconvdata \
./localedef ${LODEF_FLAGS} -i ${GLIBC_LOCALES}/hr_HR -f ${GLIBC_CHARMAPS}/ISO-8859-2 ${LODEF_TEST_DIR}/hr_HR.ISO-8859-2 >> ${LOCALEDEF_LOG} 2>&1
	check_success

I18NPATH=${LOCALEDEF_EGLIBC_0_SRC_DIR}/localedata GCONV_PATH=iconvdata \
./localedef ${LODEF_FLAGS} -i ${GLIBC_LOCALES}/sv_SE -f ${GLIBC_CHARMAPS}/ISO-8859-1 ${LODEF_TEST_DIR}/sv_SE.ISO-8859-1 >> ${LOCALEDEF_LOG} 2>&1
	check_success

I18NPATH=${LOCALEDEF_EGLIBC_0_SRC_DIR}/localedata GCONV_PATH=iconvdata \
./localedef ${LODEF_FLAGS} -i ${GLIBC_LOCALES}/ja_JP -f ${GLIBC_CHARMAPS}/SHIFT_JIS ${LODEF_TEST_DIR}/ja_JP.SJIS >> ${LOCALEDEF_LOG} 2>&1
	check_success

I18NPATH=${LOCALEDEF_EGLIBC_0_SRC_DIR}/localedata GCONV_PATH=iconvdata \
./localedef ${LODEF_FLAGS} -i ${GLIBC_LOCALES}/fr_FR -f ${GLIBC_CHARMAPS}/ISO-8859-1 ${LODEF_TEST_DIR}/fr_FR.ISO-8859-1 >> ${LOCALEDEF_LOG} 2>&1
	check_success

I18NPATH=${LOCALEDEF_EGLIBC_0_SRC_DIR}/localedata GCONV_PATH=iconvdata \
./localedef ${LODEF_FLAGS} -i ${GLIBC_LOCALES}/vi_VN -f ${GLIBC_CHARMAPS}/TCVN5712-1 ${LODEF_TEST_DIR}/vi_VN.TCVN5712-1 >> ${LOCALEDEF_LOG} 2>&1
	check_success

I18NPATH=${LOCALEDEF_EGLIBC_0_SRC_DIR}/localedata GCONV_PATH=iconvdata \
./localedef ${LODEF_FLAGS} -i ${GLIBC_LOCALES}/nb_NO -f ${GLIBC_CHARMAPS}/ISO-8859-1 ${LODEF_TEST_DIR}/nb_NO.ISO-8859-1 >> ${LOCALEDEF_LOG} 2>&1
	check_success

I18NPATH=${LOCALEDEF_EGLIBC_0_SRC_DIR}/localedata GCONV_PATH=iconvdata \
./localedef ${LODEF_FLAGS} -i ${GLIBC_LOCALES}/nn_NO -f ${GLIBC_CHARMAPS}/ISO-8859-1 ${LODEF_TEST_DIR}/nn_NO.ISO-8859-1 >> ${LOCALEDEF_LOG} 2>&1
	check_success

I18NPATH=${LOCALEDEF_EGLIBC_0_SRC_DIR}/localedata GCONV_PATH=iconvdata \
./localedef ${LODEF_FLAGS} -i ${GLIBC_LOCALES}/tr_TR -f ${GLIBC_CHARMAPS}/UTF-8 ${LODEF_TEST_DIR}/tr_TR.UTF-8 >> ${LOCALEDEF_LOG} 2>&1
	check_success

I18NPATH=${LOCALEDEF_EGLIBC_0_SRC_DIR}/localedata GCONV_PATH=iconvdata \
./localedef ${LODEF_FLAGS} -i ${GLIBC_LOCALES}/cs_CZ -f ${GLIBC_CHARMAPS}/UTF-8 ${LODEF_TEST_DIR}/cs_CZ.UTF-8 >> ${LOCALEDEF_LOG} 2>&1
	check_success

I18NPATH=${LOCALEDEF_EGLIBC_0_SRC_DIR}/localedata GCONV_PATH=iconvdata \
./localedef ${LODEF_FLAGS} -i ${GLIBC_LOCALES}/zh_TW -f ${GLIBC_CHARMAPS}/EUC-TW ${LODEF_TEST_DIR}/zh_TW.EUC-TW >> ${LOCALEDEF_LOG} 2>&1
	check_success

I18NPATH=${LOCALEDEF_EGLIBC_0_SRC_DIR}/localedata GCONV_PATH=iconvdata \
./localedef ${LODEF_FLAGS} -i ${GLIBC_LOCALES}/fa_IR -f ${GLIBC_CHARMAPS}/UTF-8 ${LODEF_TEST_DIR}/fa_IR.UTF-8 >> ${LOCALEDEF_LOG} 2>&1
	check_success

I18NPATH=${LOCALEDEF_EGLIBC_0_SRC_DIR}/localedata GCONV_PATH=iconvdata \
./localedef ${LODEF_FLAGS} -i ${GLIBC_LOCALES}/fr_FR -f ${GLIBC_CHARMAPS}/UTF-8 ${LODEF_TEST_DIR}/fr_FR.UTF-8 >> ${LOCALEDEF_LOG} 2>&1
	check_success

I18NPATH=${LOCALEDEF_EGLIBC_0_SRC_DIR}/localedata GCONV_PATH=iconvdata \
./localedef ${LODEF_FLAGS} -i ${GLIBC_LOCALES}/ja_JP -f ${GLIBC_CHARMAPS}/UTF-8 ${LODEF_TEST_DIR}/ja_JP.UTF-8 >> ${LOCALEDEF_LOG} 2>&1
	check_success

I18NPATH=${LOCALEDEF_EGLIBC_0_SRC_DIR}/localedata GCONV_PATH=iconvdata \
./localedef ${LODEF_FLAGS} -i ${GLIBC_LOCALES}/si_LK -f ${GLIBC_CHARMAPS}/UTF-8 ${LODEF_TEST_DIR}/si_LK.UTF-8 >> ${LOCALEDEF_LOG} 2>&1
	check_success

copy_dir_clean little4 ${SYSROOT_DIR} ${LOCALEDEF_LOG}

	restore_var "CPPFLAGS"

	popd

echo_stop "EGLIBC_localedef"
}


mk_gcc_final(){
echo_start "${GCC_NAME}_final"

	GCC_FINAL_BUILD_DIR="$OBJS_BUILD_DIR/${GCC_NAME}_final"
	GCC_FINAL_LOG="${LOG_DIR}/${GCC_NAME}_final.log"

	echo "Build GCC final"
	echo "Build GCC final" > ${GCC_FINAL_LOG}

	make_clean_dir $GCC_FINAL_BUILD_DIR ${GCC_FINAL_LOG}

	pushd $GCC_FINAL_BUILD_DIR

     echo "Configure GCC final"
     echo "Configure GCC final" >> ${GCC_FINAL_LOG}

#We will use sources of GCC has been prepared before in GCC_1 stage
            PATH=$TOOLS_DIR:$PATH \
            LDFLAGS=-L$PREFIX_CROSS/lib \
            "${SRC_BUILD_DIR}/${GCC_NAME}_1"/configure \
            --build=$BUILD \
            --host=$HOST \
            --target=$TARGET \
            --prefix=$PREFIX_CROSS \
            --with-sysroot=$SYSROOT \
            --disable-libmudflap \
            --disable-libssp \
            --disable-nls \
            --disable-libstdcxx-pch \
            "$GCC_SPECS" \
	    $GCC_TARGET_SPECIFIC_FLAGS \
	    $GCC_FINAL_TARGET_SPECIFIC_FLAGS \
            --enable-poison-system-directories \
            --enable-long-long \
            --enable-threads \
            --enable-languages=c,c++ \
            --enable-shared \
            --enable-lto \
            --enable-symvers=gnu \
            --enable-__cxa_atexit \
            --with-pkgversion=${PKG_VERSION} \
            --with-bugurl=${WITH_BUGURL} \
            --with-gnu-as \
            --with-gnu-ld \
            --with-host-libstdcxx='-static-libgcc -Wl,-Bstatic,-lstdc++,-Bdynamic -lm' \
            --with-build-time-tools=$TOOLS_DIR \
            --with-gmp=$PREFIX_CROSS \
            --with-mpfr=$PREFIX_CROSS \
            --with-ppl=$PREFIX_CROSS \
            --with-cloog=$PREFIX_CROSS \
            --with-libelf=$PREFIX_CROSS  >> ${GCC_FINAL_LOG} 2>&1
            check_success

	echo "Make GCC final"
	echo "Make GCC final" >> ${GCC_FINAL_LOG}

	PATH=${TOOLS_DIR}:$PATH LD_LIBRARY_PATH=$PREFIX_CROSS/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH} make ${MK_FLAGS} LDFLAGS_FOR_TARGET=--sysroot=$SYSROOT CPPFLAGS_FOR_TARGET=--sysroot=$SYSROOT build_tooldir=${TOOLS_DIR} >> ${GCC_FINAL_LOG} 2>&1
	check_success

	echo "Install GCC final"
	echo "Install GCC final" >> ${GCC_FINAL_LOG}

	PATH=${TOOLS_DIR}:$PATH LD_LIBRARY_PATH=$PREFIX_CROSS/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH} make ${MK_FLAGS} install >> ${GCC_FINAL_LOG} 2>&1
	check_success

	popd

echo_stop "${GCC_NAME}_final"
}


mk_zlib()
{
echo_start "${ZLIB_NAME}"
#Impossible use the 'mk_tool' because ZLIB should be built in the source folder
	ZLIB_BUILD_DIR="$OBJS_BUILD_DIR/${ZLIB_NAME}"
	ZLIB_LOG="${LOG_DIR}/${ZLIB_NAME}.log"

	echo "Build ZLIB"
	echo "Build ZLIB" > ${ZLIB_LOG}

	make_clean_dir ${ZLIB_BUILD_DIR} ${ZLIB_LOG}
	copy_dir_clean "${SRC_DIR}/${ZLIB_NAME}" ${ZLIB_BUILD_DIR} ${ZLIB_LOG}

	pushd ${ZLIB_BUILD_DIR}

        if [ ! -f zlib.pc ]; then

            echo "Configure ZLIB"
            echo "Configure ZLIB" >> ${ZLIB_LOG}

            ./configure --prefix=${PREFIX_CROSS} --static >> ${ZLIB_LOG} 2>&1
            check_success
        fi

 	echo "Make ZLIB"
 	echo "Make ZLIB" >> ${ZLIB_LOG}
 	
 	make ${MK_FLAGS} >> ${ZLIB_LOG} 2>&1
 	check_success

 	echo "Install ZLIB"
 	echo "Install ZLIB" >> ${ZLIB_LOG}

 	make ${MK_FLAGS} install >> ${ZLIB_LOG} 2>&1
 	check_success

    popd

echo_stop "${ZLIB_NAME}"
}

mk_expat()
{
echo_start "${EXPAT_NAME}"

prepare_to_build ${EXPAT_NAME} ${EXPAT_NAME} "${EXPAT_NAME}.log"

mk_tool "${SRC_BUILD_DIR}/${EXPAT_NAME}" "--build=${BUILD} \
                                          --target=${TARGET} \
                                          --prefix=${PREFIX_CROSS} \
                                          --disable-shared \
                                          --host=${HOST} \
                                          --disable-nls" "${LOG_DIR}/${EXPAT_NAME}.log"
echo_stop "${EXPAT_NAME}"
}

mk_gdb()
{
echo_start "${GDB_NAME}"

prepare_to_build "${GDB_NAME}" "${GDB_NAME}" "${GDB_NAME}.log"

    mk_tool "${SRC_BUILD_DIR}/${GDB_NAME}" "--build=${BUILD} \
				--target=${TARGET} \
				--prefix=${PREFIX_CROSS} \
				--host=${HOST} \
				--disable-sim \
				--with-pkgversion=${PKG_VERSION} \
				--with-bugurl=${WITH_BUGURL} \
				--disable-nls \
				--with-libexpat-prefix=${PREFIX_CROSS} \
				--with-system-gdbinit=${PREFIX_CROSS}/${HOST}/${TARGET}/lib/gdbinit \
				--with-sysroot=${SYSROOT} \
				--with-gdb-datadir=${PREFIX_CROSS}/${TARGET}/share/gdb \
				--without-python" "$GDB_LOG"

echo_stop "${GDB_NAME}"
}

mk_gdbserver_0()
{
echo_start "${GDB_NAME}_gdbserver_0"

GDBSRV0_LOG="$LOG_DIR/gdbserver_0.log"
GDBSRV0_BUILD_DIR="$OBJS_BUILD_DIR/${GDB_NAME}_gdbserver_0"
GDBSRV0_SRC_DIR="$SRC_BUILD_DIR/${GDB_NAME}_gdbserver_0"

make_clean_dir "${GDBSRV0_SRC_DIR}" "${GDBSRV0_LOG}"
copy_dir_clean "${SRC_DIR}/${GDB_NAME}" ${GDBSRV0_SRC_DIR} "${GDBSRV0_LOG}"


make_clean_dir	"${GDBSRV0_BUILD_DIR}" ${GDBSRV0_LOG}

    pushd  "${GDBSRV0_BUILD_DIR}"

	echo "Configure GDB-server 0"
	echo "Configure GDB-server 0" >> ${GDBSRV0_LOG}

	PATH="${PREFIX_CROSS}/bin:$PATH" CC="${TARGET}-gcc" ${GDBSRV0_SRC_DIR}/gdb/gdbserver/configure \
				--build=${HOST} \
				--host=${TARGET} \
				--prefix=/usr \
				--with-pkgversion=${PKG_VERSION} \
				--with-bugurl=${WITH_BUGURL} >> $GDBSRV0_LOG 2>&1
	check_success

	echo "Make GDB-server 0"
	echo "Make GDB-server 0" >> ${GDBSRV0_LOG}
	PATH="${PREFIX_CROSS}/bin:$PATH" CC="${TARGET}-gcc"	make ${MK_FLAGS} >> $GDBSRV0_LOG 2>&1
	check_success

	echo "Install GDB-server 0"
	echo "Install GDB-server 0" >> ${GDBSRV0_LOG}
	PATH="${PREFIX_CROSS}/bin:$PATH" CC="${TARGET}-gcc" make ${MK_FLAGS} DESTDIR=$SYSROOT install >> $GDBSRV0_LOG 2>&1
	check_success

	popd

echo_stop "${GDB_NAME}_gdbserver_0"
}

mk_libtirpc-glibc-nfs()
{
echo_start "additional headers to support RPC"

prepare_to_build ${ADD_HEADS_NAME} ${ADD_HEADS_NAME} "${ADD_HEADS_NAME}.log"


	echo "Install additional headers to support RPC "
	echo "Install additional headers to support RPC " >> "$LOG_DIR/${ADD_HEADS_NAME}.log"

	pushd "${SRC_BUILD_DIR}/${ADD_HEADS_NAME}"
	cp -rfv * ${SYSROOT_DIR}/usr/include >>"$LOG_DIR/${ADD_HEADS_NAME}.log" 2>&1
	check_success
	popd


echo_stop "additional headers to support RPC"
}

mk_libtirpc()
{
#LIBTIRPC_NAME
echo_start $LIBTIRPC_NAME

LIBTIRPC_LOG="$LOG_DIR/${LIBTIRPC_NAME}.log"
LIBTIRPC_BUILD_DIR="$OBJS_BUILD_DIR/${LIBTIRPC_NAME}"

	echo "Prepare tirpc library"
	echo "Prepare tirpc library " > ${LIBTIRPC_LOG}


make_clean_dir "${LIBTIRPC_BUILD_DIR}" "${LIBTIRPC_LOG}"
copy_dir_clean "${SRC_DIR}/${LIBTIRPC_NAME}" ${LIBTIRPC_BUILD_DIR} "${LIBTIRPC_LOG}"

    pushd  "${LIBTIRPC_BUILD_DIR}"

	echo "Configure libtirpc"
	echo "Configure libtirpc" >> ${LIBTIRPC_LOG}

PATH=$TOOLS_DIR:$PATH \
LDFLAGS=-L$PREFIX_CROSS/lib \
CPPFLAGS=-I${SYSROOT_DIR}/usr/include/tirpc \
./configure \
	--host=$TARGET --build=$HOST --prefix=/usr \
	--disable-gss >> $LIBTIRPC_LOG 2>&1
	check_success

	echo "Build libtirpc"
	echo "Build libtirpc" >> $LIBTIRPC_LOG

PATH=$TOOLS_DIR:$PATH \
LDFLAGS=-L$PREFIX_CROSS/lib \
CPPFLAGS=-I${SYSROOT_DIR}/usr/include/tirpc \
	make >> $LIBTIRPC_LOG 2>&1
	check_success

	echo "Install libtirpc"
	echo "Install libtirpc" >> $LIBTIRPC_LOG

PATH=$TOOLS_DIR:$PATH \
LDFLAGS=-L$PREFIX_CROSS/lib \
CPPFLAGS=-I${SYSROOT_DIR}/usr/include/tirpc \
	make DESTDIR=${SYSROOT_DIR} install >> $LIBTIRPC_LOG 2>&1
	check_success

	popd

echo_stop $LIBTIRPC_NAME
}

mk_libs_finalize(){
echo_start "Libs finalize"
#Move libs (mostly built in GCC builds) to target SYSROOT.
LIBS_FINALIZE_LOG="$LOG_DIR/libs_finalize.log"

	echo "Libs finalize"
	echo "Libs finalize">$LIBS_FINALIZE_LOG

 	echo "Move Libs to SYSROOT '${SYSROOT_DIR} '"
 	echo "Move Libs to SYSROOT '${SYSROOT_DIR} '" >> $LIBS_FINALIZE_LOG

 	cp -fv ${PREFIX_CROSS}/${TARGET}/lib/libgcc_s.so         ${SYSROOT_DIR}/lib/     >> ${LIBS_FINALIZE_LOG} 2>&1
 	check_success
 	cp -fv ${PREFIX_CROSS}/${TARGET}/lib/libgcc_s.so.1       ${SYSROOT_DIR}/lib/     >> ${LIBS_FINALIZE_LOG} 2>&1
 	check_success
 	cp -fv ${PREFIX_CROSS}/${TARGET}/lib/libstdc++.so        ${SYSROOT_DIR}/usr/lib/ >> ${LIBS_FINALIZE_LOG} 2>&1
 	check_success
 	cp -fv ${PREFIX_CROSS}/${TARGET}/lib/libstdc++.so.6      ${SYSROOT_DIR}/usr/lib/ >> ${LIBS_FINALIZE_LOG} 2>&1
 	check_success
 	cp -fv ${PREFIX_CROSS}/${TARGET}/lib/libstdc++.so.6.0.16 ${SYSROOT_DIR}/usr/lib/ >> ${LIBS_FINALIZE_LOG} 2>&1
 	check_success
 	cp -fv ${PREFIX_CROSS}/${TARGET}/lib/libstdc++.a         ${SYSROOT_DIR}/usr/lib/ >> ${LIBS_FINALIZE_LOG} 2>&1
 	check_success

# #XXX Ugly hack
# # Some (may be old) applications do not know about tirpc (RPC realisation without EGLIBC)
# # they try to find 'rpc.h' in 'rpc' subfolder, but this toolchain uses 'tirpc/rpc/rpc.h'
# # So, make symbolic links from files of  'rpc' subfolder to files in 'tirpc/rpc/rpc.h'subfolder
	
	echo "Make symlink for RPC support '${SYSROOT_DIR} '"
	echo "Make symlink for RPC support '${SYSROOT_DIR} '" >> $LIBS_FINALIZE_LOG
	pushd ${SYSROOT_DIR}/usr/include/tirpc/rpc
	LIST_OF_FILES=`ls`
	popd

	pushd ${SYSROOT_DIR}/usr/include/rpc
	for fh in $LIST_OF_FILES; do
		ln -svf ../tirpc/rpc/$fh $fh >> $LIBS_FINALIZE_LOG 2>&1
	done

	popd

	pushd ${SYSROOT_DIR}/usr/include
		ln -svf ./tirpc/netconfig.h netconfig.h >> $LIBS_FINALIZE_LOG 2>&1
	popd

echo_stop "Libs finalize"
}

#   main()

    check_prefix
    mk_zlib
    mk_binutls
    mk_gmp
    mk_mpfr
    mk_mpc
    mk_ppl
    mk_cloog
    mk_libelf
# Not need to call mk_sysroot_dir without multilib
#    mk_sysroot_dir
    mk_kernel_headers_eglibc
    mk_gcc_1
    prep_eglibc
    mk_eglibc_0_headers
    mk_gcc_2
    mk_eglibc_0_final
    mk_eglibc_localedef
    mk_gcc_final
    mk_expat
    mk_gdb
    mk_gdbserver_0
    mk_libtirpc-glibc-nfs
    mk_libtirpc
    mk_libs_finalize

