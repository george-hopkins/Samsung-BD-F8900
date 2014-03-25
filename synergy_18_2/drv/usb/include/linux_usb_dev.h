#ifndef USBDEV_H__
#define USBDEV_H__
/*****************************************************

              (c) CSR plc 2008

              All rights reserved

FILE:         linux_usb_com.h

DESCRIPTION:  Prototypes for functions operating on
              csr_dev_t objects.

REVISION:     $Revision: #1 $

******************************************************/

#ifndef __KERNEL__
#error This file can only be used for Linux kernel builds
#endif

#ifdef __cplusplus
extern "C" {
#endif


void UsbDev_Rx(uint8_t devno);
bool_t UsbDev_Tx(uint8_t devno, uint8_t channel, uint8_t *data, uint16_t size);
void UsbDev_Reset(uint8_t devno);

#endif /* !USBDEV_H__ */

