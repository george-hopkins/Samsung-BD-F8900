
Source Code
========
 * original(LGPL) version : libgphoto2-2.3.1.tar.gz
 * modified version(used in the Samsung Product): libptp.tar.gz


Building
========

* Building libgphoto2 for the original version:
    * cd into libgphoto2.
    * With emdebian,
      "PKG_CONFIG_PATH=/tmp/samsung-inst/lib/pkgconfig/ ./configure --host=mips-linux-gnu --build=x86_64-unknown-linux-gnu --prefix=/tmp/samsung-inst--without-libusb"
      where "x86_64-unknown-linux-gnu" is replaced with your host type,
      maybe "i686-pc-linux-gnu".
    * Run "make".


* Building libgphoto2 for the modified version:
    * This software distribution consists of LGPL components used in the Samsung Product.
    * cd into libgphoto2.
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
     * libptp.so used in the Samsung Product.

