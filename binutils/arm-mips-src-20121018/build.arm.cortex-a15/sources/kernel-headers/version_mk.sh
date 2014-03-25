#!/bin/sh

if [ "$#" -gt 1 ]; then
	VERSION=$1
	PATCH_NUM=$1
else
	echo -n "kernel version? > "
	read VERSION
	echo -n "last patch number? > "
	read PATCH_NUM
fi

echo "#define DTV_KERNEL_VERSION "\"$VERSION\" > ./include/linux/vdlp_version.h
echo "#define DTV_LAST_PATCH "\"$PATCH_NUM\" >> ./include/linux/vdlp_version.h

echo "******************************"
echo "*   KERNEL VERSION FINISHED   *"
echo "******************************"
