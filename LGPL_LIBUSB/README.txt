
Source Code
========
 * original(LGPL) version : libusb-1.0.tar.gz (http://www.libusb.org/wiki/libusb-1.0)
 * modified version(used in the Samsung television): libusb.tar.gz
 

 Building
========

* Building libusb for the original version:
    * cd into libusb1.0.
    * mkdir /tmp/samsung-inst
    * If you are using the emdebian toolchain,
	"./configure --host=mips-linux-gnu --build=x86_64-unknown-linux-gnu --prefix=/tmp/samsung-inst".
      where "x86_64-unknown-linux-gnu" is replaced with your host type,
      maybe "i686-pc-linux-gnu".
    * Run "make".
    * Run "make install".  This will put libusb where it is accessible by libgphoto2.
    * If you want to build other platform, make modify for makefile

* Building libusb for the modified version:
    * This software distribution consists of LGPL components used in the Samsung television
    * cd into libusb1.0.
    * Choose one for the desired platform & Set environment variables as below
      1. FoxP
         export TARGET=FOXP
         export CROSS_COMPILE=arm-v7a15v3r1-linux-gnueabi-
      2. FoxB
         export TARGET=FOXB
         export CROSS_COMPILE=arm-v7a8v3r1-linux-gnueabi-
      3. X12
         export TARGET=X12
         export CROSS_COMPILE=arm-v7a8v3r1-linux-gnueabi-
      4.X13
         export TARGET=X13
         export  CROSS_COMPILE=mipsel-34kv3r1-linux-gnu-
      5.NVT
         export TARGET=NVT569
         export  CROSS_COMPILE=mipsel-24kv3r1-linux-gnu-
    * With emdebian,
	Run "make".
    * libusb.so used in the Samsung television.
    