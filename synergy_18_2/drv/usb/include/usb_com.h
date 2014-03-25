#ifndef USB_COM_H__
#define USB_COM_H__
/******************************************************************************

                (c) CSR plc 2007

                All rights reserved

FILE:           usb_com.h

DESCRIPTION:    Cross-platform interfaces for the USB communication module.

REVISION:       $Revision: #1 $

******************************************************************************/

#include "sched/csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

bool_t  UsbDrv_Start(char *deviceName);
void    UsbDrv_Stop(void);
void    UsbDrv_Reset(void);
bool_t  UsbDrv_Tx(uint8_t channel, uint8_t *data, uint16_t size);

#ifdef __linux__
int     UsbDrv_EnterDFU(void);
#endif

#ifdef __linux__
#ifdef __KERNEL__

/*************************************************************
 * NAME:
 *      CsrUsbInit
 *
 * DESCRIPTION:
 *      This function is called by the core, when the module is
 *      loaded - hence the function will register this driver.
 *
 * RETURNS:
 *      -
 *************************************************************/
int CsrUsbInit(void);

/*************************************************************
 * NAME:
 *      CsrUsbExit
 *
 * DESCRIPTION:
 *      This function is called by the core, when the module is
 *      unloaded - hence the driver must be unregistered
 *
 * RETURNS:
 *      -
 *************************************************************/
void CsrUsbExit(void);
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif /* USB_COM_H_ */

