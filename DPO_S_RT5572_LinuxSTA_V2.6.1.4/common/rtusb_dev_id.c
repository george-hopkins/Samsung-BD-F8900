/****************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 * (c) Copyright 2002, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ****************************************************************************

    Module Name:
    rtusb_dev_id.c

    Abstract:

    Revision History:
    Who        When          What
    ---------  ----------    ----------------------------------------------
 */

#define RTMP_MODULE_OS

/*#include "rt_config.h"*/
#include "rtmp_comm.h"
#include "rt_os_util.h"
#include "rt_os_net.h"




USB_DEVICE_ID rtusb_ll_dev_id[] = {
		{USB_DEVICE(0x148F, 0x2870)},
		{}
};


USB_DEVICE_ID rtusb_extern_dev_id[] = {
		{USB_DEVICE(0x148F, 0x3572)},
		{USB_DEVICE(0x04E8, 0x2018)},
		{ }
};


/* module table */
USB_DEVICE_ID rtusb_dev_id[] = {
		{USB_DEVICE(0x148F, 0x5572)},
		{USB_DEVICE(0x04E8, 0x2078)},
		{ }
};

INT const rtusb_usb_id_len = sizeof(rtusb_dev_id) / sizeof(USB_DEVICE_ID);
#ifdef DPA_S
INT const rtusb_extern_usb_id_len = sizeof(rtusb_extern_dev_id) / sizeof(USB_DEVICE_ID);
INT const rtusb_ll_usb_len = sizeof(rtusb_ll_dev_id) / sizeof(USB_DEVICE_ID);
#endif /* DPO_S */
MODULE_DEVICE_TABLE(usb, rtusb_dev_id);
#ifdef DPA_S
MODULE_DEVICE_TABLE(usb, rtusb_extern_dev_id);
MODULE_DEVICE_TABLE(usb, rtusb_ll_dev_id);
#endif /* DPO_S */

