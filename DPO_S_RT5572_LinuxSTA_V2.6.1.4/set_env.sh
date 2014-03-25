PLATFORM=${TARGET}
export PLATFORM
echo "PLATFORM is : $PLATFORM"
export LINUX_SRC=${KERNEL_HEADER_DIR}
if [ $PLATFORM = "ECHOB" ]; then
ARCH=arm
fi
export ARCH
#make -f ./Makefile
#make -f ./Makefile
export HAS_SWITCH_CHANNEL_OFFLOAD=y
./make_version.sh
