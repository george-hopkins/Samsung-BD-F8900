
/* drivers/hid/csrhid/csrhid_loopback.h
 *
 * HID loopback driver for Synergy
 * Copyright (C) 2009 CSR.
 * Author: Sunil Pillai
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/hid.h>
#include <linux/hiddev.h>
#include <linux/hid-debug.h>
#include <linux/hidraw.h>
#include <linux/skbuff.h>
#include <linux/types.h>
#ifndef CSRHIDDRV_H
#define CSRHIDDRV_H

/* Debug macros*/
#define VERBOSE
#if defined(CSR_DEBUG)  /* limited debug messages */
#define CSRHID_DBG(fmt, arg...)  printk(KERN_INFO "(csrhid_drv):"fmt"\n" , ## arg)
#define CSRHID_VER(fmt, arg...)
#define CSRHID_ERR(fmt, arg...)

#elif defined(VERBOSE) /* Print all messages */
#define CSRHID_DBG(fmt, arg...) printk(KERN_INFO "(csrhid_drv):"fmt"\n" , ## arg)
#define CSRHID_VER(fmt, arg...) printk(KERN_INFO "(csrhid_drv):"fmt"\n" , ## arg)
#define CSRHID_ERR(fmt, arg...) printk(KERN_ERR "(csrhid_drv):"fmt"\n" , ## arg)

#else /* Error Messages only */
#define CSRHID_ERR(fmt, arg...) printk(KERN_ERR "(csrhid_drv):"fmt"\n" , ## arg)
#define CSRHID_VER(fmt, arg...)
#define CSRHID_DBG(fmt, arg...)
#endif

#define MAX_DEVICES_SUPPORTED 7

typedef unsigned long int CsrUint32;
typedef signed long int Csrint32;
typedef unsigned short int CsrUint16;
typedef signed short int Csrint16;
typedef unsigned char CsrUint8;
typedef signed  char Csrint8;

/* ERR codes returned by the driver */
enum {
    CSRHID_SUCCESS = 0,
    CSRHID_ERR_DRV_STATE = -125,
    CSRHID_ERR_HID_API_REGISTER_FAIL,
    CSRHID_ERR_HID_API_ALLOC_DEV_FAIL,
    CSRHID_ERR_HID_API_PARSE_FAIL,
    CSRHID_ERR_CHAR_DRV_API_FAIL,
    CSRHID_ERR_CPY_FRM_USER,
    CSRHID_ERR_CPY_TO_USER,
    CSRHID_ERR_NULL_PTR,
    CSRHID_ERR_WRONG_PARAM,
    CSRHID_ERR_INVALID_OPER,

};


/*HID LOOPBACK DRIVER STATES */
typedef enum {
    HID_DRIVER_STATE_NULL_BIT = 0,
    HID_DRIVER_STATE_INIT_BIT,
    HID_DRIVER_STATE_REGISTERED_BIT,
    HID_DRIVER_STATE_ACTIVE,
    HID_DRIVER_STATE_INACTIVE,
} csrhid_driver_state;


/*HID  DEVICE STATES */
typedef enum {
    HID_DEV_STATE_INIT_BIT = 0,
    HID_DEV_STATE_REGISTERED_BIT,
    HID_DEV_STATE_CONNECTED_BIT,
} csrhid_device_state;

#define REPORT_MAX_SIZE 139

typedef struct  {
    int csrhid_chrdev_major;        /* HID major number */
    struct class *csrhid_chrdev_class;  /* HID class during class_create */
    struct device *csrhid_chrdev_device;    /* HID dev during device_create */
} csrhid_bdb3_chrdev_data;


/* Hid Device structure */
typedef struct  {
    struct  hid_device *device;
    unsigned int deviceIndex;                   /* deviceIndexs received with HID connection Confirmation */
    csrhid_device_state state;
	CsrUint8  *report_desc;               /* report decriptors are maitained unless driver is destroyed*/
} csrhid_device_list;


/* HID loopback Driver global handle */

typedef struct  {
    CsrUint8      device_count;
    csrhid_bdb3_chrdev_data *char_drv;
    csrhid_device_list hid_dev[MAX_DEVICES_SUPPORTED];
    struct sk_buff_head rx_queue;
    struct sk_buff_head tx_queue;
    CsrUint8	  rx_count, tx_count;

    wait_queue_head_t     csrhid_wq;
    spinlock_t      csrhid_lock;
    CsrUint16		 state;
} csrhid_driver;

/* Structure to hold data for transfer between userpsace & kernel space */

typedef enum  {
	CSRHID_DRV_CONNECT = 0,
	CSRHID_DRV_DISCONNECT,
	CSRHID_DRV_DATA,
	CSRHID_DRV_ACTIVATE,
	CSRHID_DRV_DEACTIVATE,
} opcodes;

typedef CsrUint8 message_opcode;

typedef struct
{
	message_opcode opcode;
	CsrUint8 deviceIndex;
	CsrUint16 messageSize;
	CsrUint16 vendorId;
	CsrUint16 productId;
	CsrUint16 deviceVersion;
    	CsrUint8 *report;
}csrhid_message;


void  csrhid_initialize_device(CsrUint8 deviceIndex, CsrUint16 vendorId, CsrUint16 productId, CsrUint16 deviceVersion);
Csrint32 initialize_chardriver(void);
#endif /* CSRHIDDRV */
/*List of HID Device Vendor ID and their product ID*/
#define VENDOR_LOGITECH 0x46D
#define VENDOR_MICROSOFT 0x45E
#define VENDOR_MICROSOFT_6000 0x762
