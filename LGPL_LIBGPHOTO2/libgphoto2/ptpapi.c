/** 
* @file 			Ptpapi.c
* @brief			This source file implements of PTP Driver APIs 
*				exposed to Application.Its interface between the Application and PTP Driver
*
* @author		Sandeep Kumar(SISC)
* @Date 			January 8, 2010
* @HISTORY
* Author						Date				Description
* SANG-U, SHIM (VD)			2006/04/14		Implementation of apis for PTP
* ÀÌ½ÂÈÆ Ã¥ÀÓ(VD/SW1)
* Sheetal,SISC and Niraj,SISC 	Nov 2006				Modified for Linux 2.6
* Sandeep Kumar(SISC)		January 8, 2010	Reorganise PTP Driver Architecture i.e. Added New Structure/APIs
*											,Renaming of funtions,Existing Function Modifications for Thread Safe 
*											PTP Driver Support  for Simultaneous Multi Device Access		
*
* Copyright 2010 by Samsung Electronics, Inc.,
* 
* This software is the confidential and proprietary information
* of Samsung Electronics, Inc. ("Confidential Information").  You
* shall not disclose such Confidential Information and shall use
* it only in accordance with the terms of the license agreement
* you entered into with Samsung.
*
*/
#ifdef PTP_THREAD_SAFE
/*New Code Implementation is under PTP_THREAD_SAFE Macro */

/* HEADER FILE INCLUDES */

#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>
#include <config.h>
#include "PtpTypes.h"
#include "PtpDeviceAccess.h"
#include "ptpapi.h"
#include "PtpCallback.h"
#include "PTPDeviceListUpdate.h"

/*PC Mutex Header File Include*/
#ifdef CSP_LAYER_USAGE
#include "PCThread.h"
#endif

/* MACROS */
#define USB_PROC_BUS_USB_DEVICES_FILE 	"/proc/bus/usb/devices"
#define PTP_ATTACHED_STRING1 			"Cls=06"
#define PTP_ATTACHED_STRING2 			"still"
#define PTP_MANUFACTURER 				"Manufacturer="
#define PTP_PRODUCT 						"Product="
#define ONEKB							1024
#define PTP_LENGTH						25

/*Multidevice Support Mutex Implemented*/
#ifndef CSP_LAYER_USAGE
#define PTP_DEV_MUTEX_LOCK(DevInst)	pthread_mutex_lock\
    (&(gPTPDriverHandle[(DevInst)].u32MutexSem))
#define PTP_DEV_MUTEX_UNLOCK(DevInst)	pthread_mutex_unlock\
    (&(gPTPDriverHandle[(DevInst)].u32MutexSem))
/*CSP Layer Implementation To be used in Samsung Platform i.e. Valencia or Trident */
#else
#define PTP_DEV_MUTEX_LOCK(DevInst)	    gPTPDriverHandle[(DevInst)].u32MutexSem.Lock();
#define PTP_DEV_MUTEX_UNLOCK(DevInst)	gPTPDriverHandle[(DevInst)].u32MutexSem.Unlock ();
#endif


/*GLOBAL VARIABLES */  
PTPDriverHandle			gPTPDriverHandle[MAX_DEV_SUPPORTED]; /* Ptp Driver Structure */
static uint32				gPtpInitFlag = 0;                   			/* Flag for checking Initialization */
static PTP_DEBUG_LEVEL	PTP_Debug_Level = PTP_DBG_LVL0; 		/* Debug level flag */


/*STATIC FUNCTIONS*/  
#if defined(ENDIAN_PATCH_BY_SISC)
/**
* @fn	   	dtoh16ap (PTPParams *params, unsigned char *a)
*@brief 	This function is endian patch 
*@param 	PTPParams *params
*@param 	unsigned char *a
*@return	uint16_t
*/
static inline uint16_t
dtoh16ap (PTPParams *params, unsigned char *a)
{
    uint16_t var=0;
    memcpy(&var, a, 2);
    return ((params->byteorder==PTP_DL_LE) ? var : swap16(var));
}
#endif

/**
* @fn	   	Tick(void)
*@brief 		This function returns the time tick
*@param 	none
*@return	time tick
*/
static unsigned long Tick(void)
{
    struct tms buf;

    return (unsigned long)(times(&buf) * 10);

}

/**
* @fn	   	staclearusbstallfeature(PTPDevContext *dc, int ep)
*@brief 		This function clears the stalled usb device
*@param 	PTPDevContext *dc
*@param 	int ep
*@return		PTP_RC_OK : for success , PTP_RC_Undefined : failure
*/
static uint16 staclearusbstallfeature(PTPDevContext *dc, int ep)
{
    uint16	result = PTP_RC_OK;
    int16	nRet = 0;

    nRet = PTPDevUsbSendCtrlMsg(dc->ptp_usb.handle, LIBUSB_RECIPIENT_ENDPOINT, \
        LIBUSB_REQUEST_CLEAR_FEATURE,USB_FEATURE_HALT, ep, NULL, 0, PTP_TIMEOUT);
    if (nRet < 0)
    {
        result = PTP_RC_Undefined;
    }

    return result;
}

/**
* @fn	   	stagetusbendpointstatus(PTPDevContext *dc, uint16 *status)
*@brief 	This function gets endpoint status
*@param 	PTPDevContext *dc
*@param 	uint16 *status
*@return	PTP_RC_OK : for success , PTP_RC_Undefined : failure
*/
static uint16 stagetusbendpointstatus(PTPDevContext *dc, uint16 *status)
{
    uint16	result = PTP_RC_OK;
    int16	nRet = 0;

    if (dc == NULL)
    {
        PTP_DRV_WARN("\nDevContext is null.\n"); 
        return PTP_RC_Undefined;
    }

    nRet = PTPDevUsbSendCtrlMsg(dc->ptp_usb.handle, \
        USB_DP_DTH|LIBUSB_RECIPIENT_ENDPOINT, LIBUSB_REQUEST_GET_STATUS, \
        USB_FEATURE_HALT, dc->ptp_usb.inep, (char *)status, 2, PTP_TIMEOUT);
#if defined(ENDIAN_PATCH_BY_SISC)
    *status = dtoh16ap (&dc->params, (unsigned char *)status);
#endif
    if (nRet < 0)
    {
        PTP_DRV_WARN("\nPTPDevUsbSendCtrlMsg: return error(%x)\n", nRet); 
        result = PTP_RC_Undefined;
    }
    //else
    {
        if (*status)
        {
            PTP_DRV_INFO("\nIN endpoint status(%x)\n", *status); 
            nRet = staclearusbstallfeature(dc, dc->ptp_usb.inep);
            PTP_DRV_INFO("\nIN endpoint Request Clear Feature(%x)\n", nRet);
        }
    }

    *status = 0;

    nRet = PTPDevUsbSendCtrlMsg(dc->ptp_usb.handle, USB_DP_DTH|LIBUSB_RECIPIENT_ENDPOINT, \
        LIBUSB_REQUEST_GET_STATUS,USB_FEATURE_HALT, dc->ptp_usb.outep, (char *)status, 2,\
        PTP_TIMEOUT);
#if defined(ENDIAN_PATCH_BY_SISC)
    *status = dtoh16ap (&dc->params, (unsigned char *)status);
#endif
    if (nRet < 0)
    {
        PTP_DRV_WARN("\nPTPDevUsbSendCtrlMsg return error(%x)\n", nRet); 
        result = PTP_RC_Undefined;
    }
    //else
    {
        if (*status)
        {
            PTP_DRV_INFO("\n- OUT endpoint status(%x)\n", *status); 
            nRet = staclearusbstallfeature(dc, dc->ptp_usb.outep);
            PTP_DRV_INFO( "\nOUT endpoint Request Clear Feature(%x)\n", nRet);
        }
    }

    *status = 0;

    nRet = PTPDevUsbSendCtrlMsg(dc->ptp_usb.handle, USB_DP_DTH|LIBUSB_RECIPIENT_ENDPOINT, \
        LIBUSB_REQUEST_GET_STATUS,USB_FEATURE_HALT, dc->ptp_usb.intep,\
        (char *)status, 2, PTP_TIMEOUT);
#if defined(ENDIAN_PATCH_BY_SISC)
    *status = dtoh16ap (&dc->params, (unsigned char *)status);
#endif
    if (nRet < 0)
    {
        PTP_DRV_WARN("\nPTPDevUsbSendCtrlMsg return error(%x)\n", nRet); 
        result = PTP_RC_Undefined;
    }
    //else
    {
        if (*status)
        {
            PTP_DRV_INFO("\nINTERRUPT endpoint status(%x)\n", *status); 
            nRet = staclearusbstallfeature(dc, dc->ptp_usb.intep);
            PTP_DRV_INFO("\nINTERRUPT endpoint Request Clear Feature(%x)\n", nRet);
        }
    }
    return result;
}


/**
* @fn	   	stagetusbdevicestatus(PTPDevContext *dc, uint16 *device_status)
*@brief 	This function gets status of usb device
*@param 	PTPDevContext *dc
*@param 	uint16 *device_status
*@return	PTP_RC_OK : for success , PTP_RC_Undefined : failure
*/
static uint16 stagetusbdevicestatus(PTPDevContext *dc, uint16 *device_status)
{
    uint16	result = PTP_RC_OK;
    int16	nRet = 0;

#if defined(ENDIAN_PATCH_BY_SISC)
    unsigned char status[8] = {0,};

    nRet = PTPDevUsbSendCtrlMsg(dc->ptp_usb.handle, \
        USB_DP_DTH|LIBUSB_REQUEST_TYPE_CLASS|LIBUSB_RECIPIENT_INTERFACE, \
        USB_REQ_GET_DEVICE_STATUS, 0, 0, (char *)device_status/*status*/, 4, PTP_TIMEOUT);
    *device_status = dtoh16ap (&dc->params, &status[2]);	

    if (nRet < 0)
    {
        result = PTP_RC_Undefined;
    }

    PTP_DRV_INFO("\nDevice Status ===>> 0x%0x\n", *device_status);
#else
    uint16	status[4] = {0,};

    nRet = PTPDevUsbSendCtrlMsg(dc->ptp_usb.handle, \
        USB_DP_DTH|LIBUSB_REQUEST_TYPE_CLASS|LIBUSB_RECIPIENT_INTERFACE, \
        USB_REQ_GET_DEVICE_STATUS, 0, 0, (char *)device_status/*status*/, 4, PTP_TIMEOUT);
    if (nRet < 0)
    {
        result = PTP_RC_Undefined;
    }

    PTP_DRV_INFO("\nDevice Status ===>> 0x%0x\n", *device_status);

    *device_status = status[3];
#endif	
    return result;
}

/**
* @fn	   	stasetusbdevicereset(PTPDevContext *dc)
*@brief 	This function resets usb device
*@param 	PTPDevContext *dc
*@return	PTP_RC_OK : for success , PTP_RC_Undefined : failure
*/
static uint16 stasetusbdevicereset(PTPDevContext *dc)
{
    uint16	result = PTP_RC_OK;
    int16	nRet = 0;


    nRet = PTPDevUsbSendCtrlMsg(dc->ptp_usb.handle, \
        LIBUSB_REQUEST_TYPE_CLASS|LIBUSB_RECIPIENT_INTERFACE, \
        USB_REQ_DEVICE_RESET, 0, 0, NULL, 0, PTP_TIMEOUT);
    if (nRet < 0)
    {
        PTP_DRV_WARN("\nPTPDevUsbSendCtrlMsg : return (%x)\n", nRet);
        result = PTP_RC_Undefined;
    }

    return result;

}

/**
* @fn	   	PTP_print_debug (void *data, const char *format, va_list args)
*@brief 	This function prints debug code
*@param 	void *data
*@param 	const char *format
*@param 	va_list args
*@return	none
*/
static void PTP_print_debug (void *data, const char *format, va_list args)
{
    data = NULL; 					//intialising unused variable
//    PTP_DRV_DEBUG (format, args);
}

/**
* @fn	   	PTP_print_error (void *data, const char *format, va_list args)
*@brief 	This function prints error code
*@param 	void *data
*@param 	const char *format
*@param 	va_list args
*@return	none
*/
static void PTP_print_error (void *data, const char *format, va_list args)
{
    data = NULL; 					//intialising unused variable
//    PTP_DRV_ERR(format, args);
}

/**
* @fn	   	staptpreadfunc(unsigned char *bytes, unsigned int size, void *data, unsigned int *readbytes)
*@brief 	This function reads the data 
*@param 	unsigned char *bytes
*@param 	unsigned int size
*@param 	void *data
*@param 	unsigned int *readbytes
*@return	PTP_RC_OK : for success , PTP_RC_Undefined : failure
*/
static short staptpreadfunc (unsigned char *bytes, unsigned int size, void *data, unsigned int *readbytes)
{
    int result=-1;
    PTP_USB *ptp_usb=(PTP_USB *)data;
    int toread=0;
    signed long int rbytes=size;


    //    readbytes = NULL;					//intialising unused variable  
    *readbytes = 0;

    do {
        bytes += toread;
        if (rbytes > PTP_USB_URB) 
        {
            toread = PTP_USB_URB;
        }
        else
        {
            toread = rbytes;
        }

        result = PTPDevUsbBulkRd(ptp_usb->handle, ptp_usb->inep,(char *)bytes, \
            toread, PTP_TIMEOUT);
        if (result==0)
        {
            result=PTPDevUsbBulkRd(ptp_usb->handle, ptp_usb->inep,(char *)bytes,\
                toread, PTP_TIMEOUT);
        }

        if (result < 0)
        {
            break;
        }

        *readbytes += result;

        rbytes-=PTP_USB_URB;
    } while (rbytes > 0);

    if (result >= 0) 
    {
        return (PTP_RC_OK);
    }
    else 
    {
        return PTP_ERROR_IO;
    }
}

/**
* @fn	   	staptpwritefunc(unsigned char *bytes, unsigned int size, void *data)
*@brief 	This function writes the data 
*@param 	unsigned char *bytes
*@param 	unsigned int size
*@param 	unsigned int *rlen
*@return	PTP_RC_OK : for success , PTP_RC_Undefined : failure
*/
static short staptpwritefunc (unsigned char *bytes, unsigned int size, void *data)
{
    int result = PTP_RC_OK;
    PTP_USB *ptp_usb=(PTP_USB *)data;

    result= PTPDevUsbBulkWr(ptp_usb->handle,ptp_usb->outep,(char *)bytes,size,\
        PTP_TIMEOUT);
    if (result >= 0)
    {
        return (PTP_RC_OK);
    }
    else 
    {

        PTP_DRV_WARN("\nERROR IN SENDING COMMAND TO PTP CAMERA\n");
        return PTP_ERROR_IO;
    }
}

/**
* @fn	   	staptpcheckint(unsigned char *bytes, unsigned int size, void *data, unsigned int *rlen)
*@brief 	This function reads the data 
*@param 	unsigned char *bytes
*@param 	unsigned int size
*@param 	void *data
*@param 	unsigned int *rlen
*@return	PTP_RC_OK : for success , PTP_RC_Undefined : failure
*/
static short staptpcheckint (unsigned char *bytes, unsigned int size, void *data, unsigned int *rlen)
{
    int result = PTP_RC_OK;
    rlen = NULL;//intialising unused variable
    PTP_USB *ptp_usb=(PTP_USB *)data;

    result = PTPDevUsbBulkRd(ptp_usb->handle, ptp_usb->intep,(char *)bytes,size,\
        PTP_TIMEOUT);
    if (result == 0)
    {
        result = PTPDevUsbBulkRd(ptp_usb->handle, ptp_usb->intep,(char *) bytes, size, \
            PTP_TIMEOUT);
    }   

    if (result >= 0) 
    {
        return PTP_RC_OK;
    } 
    else 
    {
        return PTP_ERROR_IO;
    }
}

/**
* @fn	   	stausbclearstall(PTPDevContext *dc)
*@brief 	This function clears the usb stalled device
*@param 	PTPDevContext *dc:	Device context of the given ptp device
*@return	PTP_RC_OK : for success , PTP_RC_Undefined : failure
*/
static uint16 stausbclearstall(PTPDevContext *dc)
{
    uint16	result = PTP_RC_OK;
    int16	nRet = 0;
    int16	status = 0;

    nRet = PTPDevUsbSendCtrlMsg(dc->ptp_usb.handle, \
        USB_DP_DTH|LIBUSB_RECIPIENT_ENDPOINT,LIBUSB_REQUEST_GET_STATUS, \
        USB_FEATURE_HALT, dc->ptp_usb.inep, (char *)&status, 2, PTP_TIMEOUT);
#if defined(ENDIAN_PATCH_BY_SISC)
    status = dtoh16ap (&dc->params, (unsigned char *)&status);
#endif
    if ( nRet < 0)
    {
        PTP_DRV_WARN("\nPTPDevUsbSendCtrlMsg(in ep)"
            "return error(%x)\n", nRet); 
        result = PTP_RC_Undefined;
    }
    else
    {
        if (status)
        {
            PTP_DRV_INFO("\nPTPDevUsbSendCtrlMsg - IN "
                "endpoint status(%x)\n", status); 
            result = staclearusbstallfeature(dc, dc->ptp_usb.inep);
            PTP_DRV_INFO("\nstausbclearstall: - IN "
                "endpoint Request Clear Feature(%x)\n", result);
        }
    }

    nRet  = PTPDevUsbSendCtrlMsg(dc->ptp_usb.handle,\
        USB_DP_DTH|LIBUSB_RECIPIENT_ENDPOINT,LIBUSB_REQUEST_GET_STATUS,\
        USB_FEATURE_HALT, dc->ptp_usb.outep, (char *)&status, 2, PTP_TIMEOUT);
#if defined(ENDIAN_PATCH_BY_SISC)
    status = dtoh16ap (&dc->params, (unsigned char *)&status);
#endif
    if (nRet < 0)
    {
        PTP_DRV_WARN("\nPTPDevUsbSendCtrlMsg(out ep) "
            "return error(%x)\n", nRet); 
        result = PTP_RC_Undefined;
    }
    else
    {
        if (status)
        {
            PTP_DRV_INFO("\nOUT"
                "endpoint status(%x)\n", status); 
            result = staclearusbstallfeature(dc, dc->ptp_usb.outep);
            PTP_DRV_INFO("\n- OUT "
                "endpoint Request Clear Feature(%x)\n", result);
        }
    }

    nRet = PTPDevUsbSendCtrlMsg(dc->ptp_usb.handle, \
        USB_DP_DTH|LIBUSB_RECIPIENT_ENDPOINT, LIBUSB_REQUEST_GET_STATUS, \
        USB_FEATURE_HALT, dc->ptp_usb.intep, (char *)&status, 2, PTP_TIMEOUT);
#if defined(ENDIAN_PATCH_BY_SISC)
    status = dtoh16ap (&dc->params, (unsigned char *)&status);
#endif
    if (nRet < 0)
    {
        PTP_DRV_WARN("\nPTPDevUsbSendCtrlMsg (interrup ep)"
            "return error(%x)\n", result); 
        result = PTP_RC_Undefined;
    }
    else
    {
        if (status)
        {
            PTP_DRV_INFO("\nPTPDevUsbSendCtrlMsg - INTERRUPT"
                "endpoint status(%x)\n", status); 
            result = staclearusbstallfeature(dc, dc->ptp_usb.intep);
            PTP_DRV_INFO("\nstaclearusbstallfeature: - INTERRUPT"
                "endpoint Request Clear Feature(%x)\n", result);
        }
    }

    return result;
}


/**
* @fn	   	stausbinitport(PTPDevContext *dc)
*@brief 	This function initialize the usb port
*@param 	PTPDevContext *dc:	Device context of the given ptp device
*@return	PTP_RC_OK : for success , PTP_RC_Undefined : failure
*/
static uint16 stausbinitport (PTPDevContext *dc)
{
    uint16	result = PTP_RC_OK;
    int16	nRet = 0;
    if (dc == NULL)
    {
        PTP_DRV_WARN("\nDevContext is null\n");
        return PTP_RC_Undefined;
    }
    dc->params.write_func = staptpwritefunc;
    dc->params.read_func = staptpreadfunc;
    dc->params.check_int_func = staptpcheckint;
    dc->params.check_int_fast_func = staptpcheckint;
    dc->params.error_func = PTP_print_error;
    dc->params.debug_func = PTP_print_debug;
    dc->params.sendreq_func = ptp_usb_sendreq;
    dc->params.senddata_func = ptp_usb_senddata;
    dc->params.getresp_func = ptp_usb_getresp;
    dc->params.getdata_func = ptp_usb_getdata;
    dc->params.data = (void *)(&(dc->ptp_usb));
    dc->params.transaction_id = 0;
    //dc->params.byteorder = PTP_DL_BE;
    //Test to check byteorder by Ajeet Yadav
    result = 0xBB11;	
    dc->params.byteorder = (*(unsigned char *)(&result) == '\x11')? PTP_DL_LE : PTP_DL_BE;
    result = PTP_RC_OK;

    if (dc->dev < 0)
    {
        PTP_DRV_WARN("\ndc->dev is null\n");
        return PTP_RC_Undefined;
    }
    /*Usb Handle to Usb Device Structure*/
    dc->ptp_usb.handle = &dc->ptp_usb.DevStruct;

    if ( 0 == PTPDevUsbOpen(dc->dev->pvDev,dc->ptp_usb.handle))
    {
        if ( !(dc->ptp_usb.handle) ) 
        {
            result = PTP_RC_Undefined;
        }
        else
        {
            /* As default config set by OS is 0, so no need to cofigure again for the same value*/
            if(0 != dc->dev->iLastSelectedConfig)
            {
                /* Unconfigure the device by passing config value as -1*/
                PTPDevUsbSetCfg(dc->ptp_usb.handle, -1);
                /* Now set the selected configuration, add 1 for the config value for selected configuration */
                PTPDevUsbSetCfg(dc->ptp_usb.handle, dc->dev->iLastSelectedConfig + 1);
            }

            nRet = PTPDevUsbClaimInterface(dc->ptp_usb.handle, \
                dc->dev->ppConfigDesc[dc->dev->iLastSelectedConfig]->interface[dc->dev->iLastClaimedInfterface].altsetting[dc->dev->iAltSettingNum].bInterfaceNumber);
            if (nRet < 0)
            {
                nRet = PTPDevUsbRelInterface(dc->ptp_usb.handle, \
                    dc->dev->ppConfigDesc[dc->dev->iLastSelectedConfig]->interface[dc->dev->iLastClaimedInfterface].altsetting[dc->dev->iAltSettingNum].bInterfaceNumber);
                if (nRet != 0)
                {
                    PTP_DRV_WARN("\nError in Rel Interface\n");
                }
                else
                {
                    PTP_DRV_INFO("\nOK :: Rel Interface\n");
                }
                nRet = PTPDevUsbClaimInterface(dc->ptp_usb.handle, \
                    dc->dev->ppConfigDesc[dc->dev->iLastSelectedConfig]->interface[dc->dev->iLastClaimedInfterface].altsetting[dc->dev->iAltSettingNum].bInterfaceNumber);
                if (nRet != 0)
                {
                    PTP_DRV_WARN("\nError in Claim Interface");
                }
                PTP_DRV_INFO("\nPTPDevUsbClaimInterface (%x)\n", nRet);
                result = PTP_RC_Undefined;
                PtpPrivateCloseDeviceContext(dc);
            }

        }
    }
    else
    {
        /*Device Open Error*/
        return PTP_RC_AccessDenied;
    }

    return result;
}

/**
* @fn	   	stausbfinddevice(PTPDevContext *dc)
*@brief 	This function finds  the given device and rechecks the device connnectivity
*@ remarks	bus number, device number and product id are matched
*@param 	PTPDevContext *dc:	Device context of the given ptp device
*@return		Bool 
True : device present, False : device absent
*/
static Bool  stausbfinddevice (PTPDevContext *dc)
{
    Bool bReturn = False;
	
    char busNum[15],devNum[15];
	uint8_t uiBusNum;
	uint8_t uiDevNum;
    if (dc == NULL)
    {
        PTP_DRV_WARN("\nDevContext is null.\n");
        return False;
    }
    //	uint16 ProdID = dc->dev->descriptor.idProduct;

    strncpy(busNum,dc->dev->dirPath,10);
    busNum[10] = 0;
    strncpy(devNum,dc->dev->filePath,10);
    devNum[10] = 0;
    PTPDevUsbInit();
#if 0
	TempPTPDevFindPTPDevices(PTP_RECHECK);
	dc->dev = *(PTPDevUsbGetDevices());
	if(dc->dev == NULL)
	{
		PTP_DRV_WARN("\ndc->dev is null.\n");
		return False;
	}
	for(;dc->dev; dc->dev = dc->dev->next)
	{
	   if ((dc->dev->ppConfigDesc[dc->dev->iLastSelectedConfig]->interface[dc->dev->iLastClaimedInfterface].altsetting[dc->dev->iAltSettingNum].bInterfaceClass == LIBUSB_CLASS_PTP)&& (dc->dev->deviceDesc.bDeviceClass!=LIBUSB_CLASS_HUB))
		{
			if ((strcmp(busNum,dc->dev->dirPath)==0) && (strcmp(devNum,dc->dev->filePath)==0) )
			{
				bReturn = True;
				return bReturn;
			}
		}
	}
	return bReturn;

#else

	uiBusNum = (uint8)atoi(busNum);
	uiDevNum = (uint8)atoi(devNum);
	if(1 == TempPTPDevFindPTPDevices(uiBusNum,uiDevNum))
	{
		printf("!!!!Temp PTP Devices Changed\n");
		bReturn = True;
	}
	return bReturn;
#endif
}

/**
* @fn	   	stausbfindendpoints(PTPDevContext *dc)
* @brief	finds the eps
* @param   	PTPDevContext *dc:  device context
* @return   PTP_RC_OK : success, PTP_RC_Undefined : failure
*/
static uint16 stausbfindendpoints(PTPDevContext *dc)
{
    uint16 nIndex = 0;
    uint16 nNumEp = 0;
    uint16 result = PTP_RC_OK;
    const struct libusb_endpoint_descriptor *EP = NULL;

    if (dc == NULL)
    {
        PTP_DRV_WARN("\nDevice Context is null.\n");
        return PTP_RC_Undefined;
    }

    if (dc->dev == NULL)
    {
        PTP_DRV_WARN("\nUSB device is null.\n");
        return PTP_RC_Undefined;
    }

    EP = (const libusb_endpoint_descriptor *)dc->dev->ppConfigDesc[dc->dev->iLastSelectedConfig]->interface[dc->dev->iLastClaimedInfterface].altsetting[dc->dev->iAltSettingNum].endpoint; 
    if (EP == NULL)
    {
        PTP_DRV_WARN("\nEndpoint descriptor is null.\n");
        return PTP_RC_Undefined;
    }
    nNumEp = dc->dev->ppConfigDesc[dc->dev->iLastSelectedConfig]->interface[dc->dev->iLastClaimedInfterface].altsetting[dc->dev->iAltSettingNum].bNumEndpoints; 
    if (nNumEp < 3 || nNumEp > 3)
    {
        PTP_DRV_WARN("\n Endpoint number (%x) is not matched.\n", nNumEp);
        return PTP_RC_Undefined;
    }

    for (nIndex = 0; nIndex < nNumEp; nIndex++)
    {
        switch (EP[nIndex].bmAttributes)
        {
            case LIBUSB_TRANSFER_TYPE_INTERRUPT:
                if ((EP[nIndex].bEndpointAddress & LIBUSB_ENDPOINT_DIR_MASK)\
                    == LIBUSB_ENDPOINT_DIR_MASK)
                {
                    dc->ptp_usb.intep = EP[nIndex].bEndpointAddress;
                    PTP_DRV_INFO("\nfind a interrupt in "
                        "endpoint at 0x%02x\n", dc->ptp_usb.intep);
                }
                break;
            case LIBUSB_TRANSFER_TYPE_BULK:
                if ((EP[nIndex].bEndpointAddress & LIBUSB_ENDPOINT_DIR_MASK)== \
                    LIBUSB_ENDPOINT_DIR_MASK)
                {
                    dc->ptp_usb.inep = EP[nIndex].bEndpointAddress;
                    PTP_DRV_INFO("\nfind a Bulk(IN) in "
                        "endpoint at 0x%02x\n", dc->ptp_usb.inep);
                }
                else if ((EP[nIndex].bEndpointAddress&LIBUSB_ENDPOINT_DIR_MASK)==0)
                {
                    dc->ptp_usb.outep = EP[nIndex].bEndpointAddress;
                    PTP_DRV_INFO("\nstausbfindendpoints : find a Bulk(OUT) in "
                        "endpoint at 0x%02x\n", dc->ptp_usb.outep);
                }
                break;
            default:
                PTP_DRV_INFO("\nundefind "
                    "endpoint type (0x%02x)\n", EP[nIndex].bmAttributes);
        }
    }

    return result;
}

/**
* @fn	   	staFileExist(const char* fileName)
* @brief	      Check whether the file is existing or not
* @remarks		none
* @param		const char *fileName
* @return		Bool
True : success, False : failure
*/ 
static Bool staFileExist(const char* fileName)
{

    Bool bRet = False;
    struct stat stStatBuf;
    if(0 == stat(fileName,&stStatBuf))
    {
        /*Check file is regular*/
        if(	S_ISREG(stStatBuf.st_mode))
        {
            bRet = True;				
        }
        else
        {
            PTP_DRV_WARN("\nstaFileExist ERRRRRRRRRRRRRRRRRRRRRR\n");
        }
    }
    return bRet;
}


/**
* @fn	   	staPtpChkDevExist(PTPDevContext *dc)
* @brief		   Check whether the ptp device is existing or not
* @param   	PTPDevContext *dc:  device context
* @return   True : success, False : failure
*/ 
static Bool staPtpChkDevExist(PTPDevContext *dc)	 
{
    int busNum = -1;
    int devNum = -1;
    FILE *fp = NULL;
    char pDataLine[ONEKB +1];
    int byteCount = 0;

    int finish = False;

    char *pDest = NULL;
    char *pDest1=NULL;
    //char product[50] = {0, };
    int len = -1;

    char dest1[PTP_LENGTH];
    char dest2[PTP_LENGTH];
    if(dc==NULL)
    {
        PTP_DRV_WARN("\nDevice Context is null.\n");
        return False;//added by Niraj	 
    }
    if (!staFileExist(USB_PROC_BUS_USB_DEVICES_FILE))
    {
        system("mount -t usbfs none /proc/bus/usb/");
    }

    if(!(fp = fopen(USB_PROC_BUS_USB_DEVICES_FILE, "r")))
    {
        PTP_DRV_WARN("\n[ FILE ]file open error : %s\n", USB_PROC_BUS_USB_DEVICES_FILE);
        return  False;
    }	 

    memset(dest1,0,PTP_LENGTH);

    busNum=atoi((dc->dev)->dirPath);


    if(busNum<9)
    {
        snprintf(dest1,PTP_LENGTH,"Bus=0%d",busNum);
    }
    else	 
    {
        snprintf(dest1,PTP_LENGTH,"Bus=%d",busNum);
    }

    memset(dest2,0,PTP_LENGTH);
    //devNum=(int)dc->dev->uiDevNum;
    devNum=atoi((dc->dev)->filePath);

    snprintf(dest2,PTP_LENGTH,"Dev#=%3d",devNum);

    while (1) 
    {

        byteCount = fread(pDataLine, sizeof(char), ONEKB, fp);

        if (byteCount == 0)
        {
            finish = True;

        }
        else
        {
            if(byteCount <= (ONEKB))
            {
                pDataLine[byteCount] = '\0';
            }
		else
		{
		   pDataLine[ONEKB] = '\0';
		}

            if(strstr(pDataLine, PTP_ATTACHED_STRING1))
            {	 
                if(0!= fseek(fp,0,SEEK_SET))
                {
                    break;
                } 			
				
    		/*Reads Data*/
		while((byteCount = fread(pDataLine, sizeof(char),ONEKB , fp)))
		{
			 if(byteCount <= (ONEKB))
			 {
				 pDataLine[byteCount] = '\0';
			 }
			 else
			 {
				 pDataLine[ONEKB] = '\0';
			 }

		          pDest = strstr (pDataLine,dest1);

                    if (pDest != NULL)
                    {
                        if((len=strlen(pDest))<50) //actual value is 45
                        {
                            if(0!= fseek(fp,-(len+4),SEEK_CUR))
                            {
                                break;
                            }
                            continue;
                        }

                        pDest1 = strstr (pDest, dest2);
                        if(pDest1!=NULL)
                        {
                            fclose(fp);
                            return True;
                        }
                        else
                        {
                            pDest=pDest+6;//increment the pointer to point after string dest1
                            pDest1 = strstr(pDest,dest1);
                            if(pDest1!=NULL)
                            {
                                len=strlen(pDest1);
                                if(0!= fseek(fp,-(len+4),SEEK_CUR))
                                {
                                    break;
                                }
                            }	 
                            continue;
                        }

                    }			 
                }

                finish = True;
            }
        }

        if(finish)
        {
            break;
        }
    }

    fclose(fp);
    return finish;
}


/*PRIVATE FUNCTIONS USED WITHIN DRIVER*/



/**
* @fn	   	ptp_objectinfo_copy(PTPObjectInfo *dest, PTPObjectInfo *src)
* @brief	This function copies the object info
* @param   	PTPObjectInfo *dest:  destination object info
* @param   	PTPObjectInfo *src:  Source object info
* @return   Error code
*			PTPAPI_ERROR : in case of error.
*			PTP_RC_OK : in case of success.
*/
uint16 ptp_objectinfo_copy(PTPObjectInfo *dest, PTPObjectInfo *src)
{
    int16	result = PTP_RC_OK;
    uint16 len = 0;
    dest->StorageID = src->StorageID;
    dest->ObjectFormat = src->ObjectFormat;
    dest->ProtectionStatus = src->ProtectionStatus;
    dest->ObjectCompressedSize = src->ObjectCompressedSize;
    dest->ThumbFormat = src->ThumbFormat;
    dest->ThumbCompressedSize = src->ThumbCompressedSize;
    dest->ThumbPixWidth = src->ThumbPixWidth;
    dest->ThumbPixHeight = src->ThumbPixHeight;
    dest->ImagePixWidth = src->ImagePixWidth;
    dest->ImagePixHeight = src->ImagePixHeight;
    dest->ImageBitDepth = src->ImageBitDepth;
    dest->ParentObject = src->ParentObject;
    dest->AssociationType = src->AssociationType;
    dest->AssociationDesc = src->AssociationDesc;
    dest->SequenceNumber = src->SequenceNumber;
    dest->CaptureDate = src->CaptureDate;
    dest->ModificationDate = src->ModificationDate;

    if (src->Filename != NULL)
    {
        if (strlen (src->Filename) > 0)
        {
            dest->Filename = (char*)calloc (strlen(src->Filename) + 1, sizeof (char));//typecasting g++ vishal
            if(NULL != dest->Filename)
            {
                len = strlen(src->Filename);
                strncpy (dest->Filename, src->Filename,len);
                dest->Filename[len] = '\0';
            }
        }
    }

    if (src->Keywords != NULL)
    {
        PTP_DRV_INFO("\nkeyWords Not NULL\n");
        if (strlen(src->Keywords) > 0)
        {
            dest->Keywords = (char*)calloc (strlen(src->Keywords) + 1, sizeof (char));//typecasting g++ vishal
            if(NULL != dest->Keywords)
            {

                len = strlen(src->Keywords);
                strncpy (dest->Keywords, src->Keywords,len);
                dest->Keywords[len] = '\0';
            }
        }
    }
    else
    {
        PTP_DRV_WARN("keyWords ISSSSSS   NULL\n");
    }
    return result;

}

/**
* @fn	   	ptp_handles_copy(PTPObjectHandles *dest , PTPObjectHandles *src)
* @brief	This function copies the handles
* @param   	PTPObjectHandles *dest:  destination object handle
* @param   	PTPObjectHandles *src:  Source object handle
* @return   Error code
*			PTPAPI_ERROR : in case of error.
*			PTP_RC_OK : in case of success.
*/
uint16 ptp_handles_copy(PTPObjectHandles *dest , PTPObjectHandles *src)
{
    int16	result = PTP_RC_OK;
    uint32	nIndex = 0;

    dest->n = src->n;
    dest->Handler = (uint32_t*)calloc (sizeof (uint32), src->n);

    if(NULL == dest->Handler)
    {
        return PTPAPI_ERROR;
    }

    for (nIndex = 0; nIndex < src->n; nIndex++)
    {
        dest->Handler[nIndex] = src->Handler[nIndex];
    }

    return result;
}

/**
* @fn	   	PtpPrivateOpenDeviceContext(PTPDevContext *dc)
* @brief	This function opens the given device and updates the DC
* @param   	PTPDevContext *dc:  Device context of the given device
* @return   Error code
*			PTPAPI_ERROR : in case of error.
*			PTP_RC_OK : in case of success.
*/
uint16 PtpPrivateOpenDeviceContext(PTPDevContext *dc)
{

    uint16 result = PTP_RC_Undefined;
    Bool bRet = False;

    if(dc==NULL)
    {
        PTP_DRV_WARN("Device Context is null.\n");
        return PTPAPI_ERROR;//added by Niraj	 
    }

    bRet = stausbfinddevice(dc);

    if (bRet == False)
    {
        PTP_DRV_WARN("\n Can't find devices\n");
        return result;
    }


    result = stausbfindendpoints(dc);
    if (result != PTP_RC_OK)
    {
        PTP_DRV_WARN("\nCan't find endpoint"
            "(0x%4x)\n", result);
        return result;
    }

    result = stausbinitport(dc);
    if (result != PTP_RC_OK)
    {
        PTP_DRV_WARN("\nCan't open usb port "
            "(0x%4x)\n", result);
        PTP_DRV_WARN("\nCan't open usb port "
            "(0x%4x)\n", result);
        return result;
    }

    PTP_DRV_INFO("\nSUCCESS\n");

    return PTP_RC_OK;
}

/**
* @fn	   	PtpPrivateCloseDeviceContext(PTPDevContext *dc)
* @brief	This function closes device
* @param   	PTPDevContext *dc:  Device context of the given device
* @return   Error code
*			PTPAPI_ERROR : in case of error.
*			PTP_RC_OK : in case of success.
*/
uint16 PtpPrivateCloseDeviceContext(PTPDevContext *dc)
{
    uint16  result = PTP_RC_OK;
    int16   nRet = 0;
    PTP_DRV_INFO("\nstart\n");
    result = stausbclearstall(dc);  // defined in libusb

    if (dc->dev != NULL)
    {
        PTPDevUsbRelInterface(dc->ptp_usb.handle,dc->dev->ppConfigDesc[dc->dev->iLastSelectedConfig]->interface[dc->dev->iLastClaimedInfterface].altsetting[dc->dev->iAltSettingNum].bInterfaceNumber); 
    }
    else
    {
        PTP_DRV_WARN("\nDevice Context dev is null.\n");
    }

    nRet = PTPDevUsbClose(dc->ptp_usb.handle);

    PTP_DRV_INFO("\nend(nRet:0x%x)\n", nRet);

    return result;
}



/**
* @fn	 PtpDevLock
* @brief	 Locks Device Mutex
* @param (in)	DevInst
* @return	 None
* @see		 
*/

void PtpDevLock(PtpDevInstances_t DevInst)
{

    if((DevInst > PtpInvalidDevInst)&&(DevInst < MaxPtpDevSupport))
    {
        PTP_DEV_MUTEX_LOCK(DevInst);
    }
}

/**
* @fn		 PtpDevUnLock
* @brief	 UnLocks Device Mutex
* @param (in)	DevInst
* @return	 None
* @see		 
*/

void PtpDevUnLock(PtpDevInstances_t DevInst)
{
    if((DevInst > PtpInvalidDevInst)&&(DevInst < MaxPtpDevSupport))
    {
        PTP_DEV_MUTEX_UNLOCK(DevInst);  
    }
}

/**
* @fn           void PTP_API_Init_DeviceInfo(PTPDeviceInfo *pDevInfo)
* @brief	     Initializes the DeviceInfomation to Null
* @remarks		none
* @param		PTPDeviceInfo
* @return		none
*/ 
void PTP_API_Init_DeviceInfo(PTPDeviceInfo *di)
{
    if(NULL != di)
    {
        memset(di,0,sizeof(PTPDeviceInfo));
        di->VendorExtensionDesc = NULL;
        di->OperationsSupported = NULL;
        di->EventsSupported = NULL;
        di->DevicePropertiesSupported = NULL;
        di->CaptureFormats = NULL;
        di->ImageFormats = NULL;
        di->Manufacturer = NULL;
        di->Model = NULL;
        di->DeviceVersion = NULL;
        di->SerialNumber = NULL;
    }
}

/**
* @fn           void PTP_API_Init_ObjectHandles(PTPObjectHandles *ohpObjHand)
* @brief	     Initializes the Objecthandles
* @remarks		none
* @param		PTPObjectHandles:  Handle for the given ptp device
* @return		none
*/ 
void PTP_API_Init_ObjectHandles(PTPObjectHandles *oh)
{
    if(NULL != oh)
    {
        oh->n = 0;
        oh->Handler = NULL;
    }
}

/**
* @fn           void PTP_API_Init_StorageIDs(PTPStorageIDs *pStorageids)
* @brief	     Initializes the StorageIDs
* @remarks		none
* @param		PTPStorageIDs: StorageIDs for the given ptp device
* @return		none
*/
void PTP_API_Init_StorageIDs(PTPStorageIDs *storageids)
{
    if(NULL != storageids)
    {
        storageids->n = 0;
        storageids->Storage = NULL;
    }
}

/**
* @fn           void PTP_API_Init_ObjectInfo(PTPObjectInfo *pObjextinfo)
* @brief	     Initializing the ObjectInfo
* @remarks		none
* @param		PTPObjectInfo: ObjectInfo for the given ptp device
* @return		none
*/
void PTP_API_Init_ObjectInfo(PTPObjectInfo *oi)
{
    if(NULL != oi)
    {
        memset(oi,0,sizeof(PTPObjectInfo));
        oi->Filename = NULL;
        oi->Keywords = NULL;
    }
}

/**
* @fn           void PTP_API_Init_PTPFileListInfo(PTPFileListInfo **pFileinfo)
* @brief	     Initializing the FileListInfo
* @remarks		none
* @param		PTPFileListInfo: FileListInfo for the given ptp device
* @return		none
*/
void PTP_API_Init_PTPFileListInfo(PTPFileListInfo **fileinfo)
{
    if(NULL != fileinfo)
    {
        *fileinfo = NULL;
    }
}

/**
* @fn           void PTP_API_Init_PTPStorageInfo(PTPStorageInfo *pStorageInfo)
* @brief	     Initializing the StorageInfo
* @remarks		none
* @param		PTPStorageInfo 
* @return		none
*/
void PTP_API_Init_PTPStorageInfo(PTPStorageInfo *storageInfo)
{
    if(NULL != storageInfo)
    {
        memset(storageInfo,0,sizeof(PTPStorageInfo));
        storageInfo->StorageDescription = NULL;
        storageInfo->VolumeLabel = NULL;
    }
}

/**
* @fn           void PtpPrivateInitTransaction(PTPDevContext *pDevCtxt)
* @brief	     Initializing the Context data of the device
* @remarks		none
* @param		PTPDevContext :  
* @return		none
*/
void PtpPrivateInitTransaction(PTPDevContext *dc)
{
    PTP_API_Init_DeviceInfo (&(dc->params.deviceinfo));
    PTP_API_Init_ObjectHandles (&(dc->params.handles));

    if(NULL != dc->params.objectinfo)
    {
        // Memory Leak Check
        PTP_API_Clear_ObjectInfo(dc->params.objectinfo);
        PTPFreeMem(dc->params.objectinfo);        
    }

}

/**
* @fn           void PTP_API_Clear_StorageIDs(PTPStorageIDs *pStorageIds)
* @brief	     Clearing the Storage ids of the device
* @remarks		none
* @param		PTPStorageIDs  
* @return		none
*/
void PTP_API_Clear_StorageIDs(PTPStorageIDs *storageids)
{
    storageids->n = 0;
    PTPFreeMem (storageids->Storage);        

}

/**
* @fn           void PTP_API_Clear_DeviceInfo(PTPDeviceInfo *di)
* @brief	     Clearing the Storage ids of the device
* @remarks		none
* @param		PTPStorageIDs  
* @return		none
*/
void PTP_API_Clear_DeviceInfo(PTPDeviceInfo *di)
{
    if(NULL == di)
    {
        PTP_DRV_WARN("\nNull Device Info Pointer\n");
        return;
    }
    PTPFreeMem (di->VendorExtensionDesc);

    PTPFreeMem (di->OperationsSupported);

    PTPFreeMem (di->EventsSupported);

    PTPFreeMem (di->DevicePropertiesSupported);

    PTPFreeMem (di->CaptureFormats);

    PTPFreeMem (di->ImageFormats);

    PTPFreeMem (di->Manufacturer);

    PTPFreeMem (di->Model);

    PTPFreeMem (di->DeviceVersion);

    PTPFreeMem (di->SerialNumber);
}

/**
* @fn           void PTP_API_Clear_ObjectHandles(PTPObjectHandles *oh)
* @brief	     Clearing the object handles of the device
* @remarks		none
* @param		PTPObjectHandles  
* @return		none
*/
void PTP_API_Clear_ObjectHandles(PTPObjectHandles *oh)
{
    if (oh == NULL)
    {
        return;
    }

    PTPFreeMem (oh->Handler);         
    oh->n = 0;
}

/**
* @fn           void PTP_API_Clear_ObjectInfo(PTPObjectInfo *oi)
* @brief	     Clearing the object info of the device
* @remarks		none
* @param		PTPObjectInfo  
* @return		none
*/
void PTP_API_Clear_ObjectInfo(PTPObjectInfo *oi)
{
    if (oi == NULL)
    {
        return;
    }

    PTPFreeMem (oi->Filename);	

    PTPFreeMem (oi->Keywords);
}

/**
* @fn          void PTP_API_Clear_PTPFileListInfoInfo(PTPFileListInfo **fileinfo)
* @brief	     Clearing the FileListInfo of the device
* @remarks		none
* @param		PTPObjectInfo  
* @return		none
*/
void PTP_API_Clear_PTPFileListInfoInfo(PTPFileListInfo **fileinfo)
{
    PTPFreeMem (*fileinfo);        
}

/**
* @fn          void PTP_API_Clear_PTPStorageInfo(PTPStorageInfo *storageInfo)
* @brief	     Clearing the StorageInfo of the device
* @remarks		none
* @param		PTPStorageInfo  
* @return		none
*/
void PTP_API_Clear_PTPStorageInfo(PTPStorageInfo *storageInfo)
{
    PTPFreeMem (storageInfo->StorageDescription);
    PTPFreeMem (storageInfo->VolumeLabel);  
}

/**
* @fn          void PTP_Terminate_Transaction(PTPDevContext *dc)
* @brief	     Clearing the Context data of the device
* @remarks		none
* @param		PTPDevContext  
* @return		none
*/
void PTP_Terminate_Transaction(PTPDevContext *dc)
{
    if (dc->params.data != NULL)
    {
        dc->params.data = NULL;
    }

    PTP_API_Clear_DeviceInfo(&dc->params.deviceinfo);
    PTP_API_Clear_ObjectHandles(&dc->params.handles);
    PTP_API_Clear_ObjectInfo(dc->params.objectinfo);
}

/*EXPORTED APIS*/  

/**
* @fn		 PTP_API_Init_DeviceList
* @brief		 This function Initialises the PTP Driver.
* @param	 None
* @return	  Error code
*			 PTPAPI_ERROR: in case of error.
*			 PTP_RC_OK: in case of success.
* @see 	  	Added Application Callback Function Parameter
*			 If required Application can pass the Callback Function
*			 for Device Notification
*/

PtpErr_t PTP_API_Init_DeviceList(PtpAppCbFn_t tAppCbFn)

{
    uint32 uiDevInst = 0;
    uint32 uiApplCbInst = 0;	

    /*Check if already intialised*/
    if(True == gPtpInitFlag)
    {
        PTP_DRV_ERR("\nPTP Driver Already Initialized\n");
        return PTPAPI_ERROR;
    }
    /*Initialise PTP Device Data Structure*/
    for(uiDevInst = 0;uiDevInst<MAX_DEV_SUPPORTED;uiDevInst++)
    {

#ifndef CSP_LAYER_USAGE
        gPTPDriverHandle[uiDevInst].u32MutexSem = (pthread_mutex_t)\
            PTHREAD_MUTEX_INITIALIZER;
        /*CSP Layer Implementation To be used in Samsung Platform i.e. Valencia or Trident */
#else

        gPTPDriverHandle[uiDevInst].u32MutexSem.Create ();
#endif
        gPTPDriverHandle[uiDevInst].IsDeviceDetect = 0;
        gPTPDriverHandle[uiDevInst].IsOpen = 0;
        gPTPDriverHandle[uiDevInst].ptpDevice = NULL;
        gPTPDriverHandle[uiDevInst].pPTPDevCtxt = NULL;

        /*Initialize the Callback List*/
        for(uiApplCbInst = 0;uiApplCbInst<MaxPtpCbId;uiApplCbInst++)
        {
            gPTPDriverHandle[uiDevInst].PtpAppCbFn[uiApplCbInst]= NULL;
        }

        /*PtpHandle given to Application*/
        gPTPDriverHandle[uiDevInst].PtpInstHndl.u32Valid = PTP_INVALID_HNDL;
        gPTPDriverHandle[uiDevInst].PTPHndl.BusNum =-1;
        gPTPDriverHandle[uiDevInst].PTPHndl.DevNum =-1;
        gPTPDriverHandle[uiDevInst].PTPHndl.DevInst =PtpInvalidDevInst;

    }
    /*Initalize libusb.0 via Device Access Module*/
    PTPDevUsbInit();

    /*Register Device Manager Callback Function*/
    CPTPDeviceListUpdate::Get().Initialize();
    /*Application to provide Cb function */
    /*For Device Detection Status update to Application*/

    if(NULL != tAppCbFn)
    {
        for(uiDevInst=0;uiDevInst <MAX_DEV_SUPPORTED;uiDevInst++)
        {
            gPTPDriverHandle[uiDevInst].PtpAppCbFn[PtpDevDetectStatusCbId]= tAppCbFn;
        }
    }

    gPtpInitFlag = True;

    PTP_DRV_INFO("\nSUCCESS\n");

    return PTP_RC_OK;
}


/**
* @fn		 PTP_API_DeInit_DeviceList
* @brief		 This function De-Initialises the PTPDriver.
* @param	 None
* @return	  Error code
*			 PTPAPI_ERROR: in case of error.
*			 PTP_RC_OK: in case of success.
* @see 	  	New API 
*/
PtpErr_t PTP_API_DeInit_DeviceList(void)
{
    uint32 uiDevInst;
    uint32 uiAppCbId;
    PtpDevHndl  pPTPHandle;

    /*Check if already intialised*/
    if(False == gPtpInitFlag)
    {
        PTP_DRV_ERR("\nPTP Driver Already DeInitialized\n");
        return PTPAPI_ERROR;
    }

    /*UnRegister Device Manager Callback Function*/
    CPTPDeviceListUpdate::Get().Finalize();
    /*DeInitialise PTP Device Data Structure*/
    for(uiDevInst = 0;uiDevInst<MAX_DEV_SUPPORTED;uiDevInst++)
    {

        PtpDevLock((PtpDevInstances_t)uiDevInst);

        /*If Device is detected Close Device*/
        if(True == gPTPDriverHandle[uiDevInst].IsDeviceDetect)
        {
            pPTPHandle.bus_num = gPTPDriverHandle[uiDevInst].\
                ptpDevice->ptp_path.bus_num;
            pPTPHandle.dev_num = gPTPDriverHandle[uiDevInst].\
                ptpDevice->ptp_path.dev_num;

            if(PTP_RC_OK != PtpCallbackFinishComm(pPTPHandle))
            {
                PTP_DRV_WARN("\n PtpPrivateFinishComm Error\n");
            }
            else
            {
                PTP_DRV_INFO("\nPtpPrivateFinishComm OKKKKKKK\n");
            }
        }

        /*Destroy Mutex*/
#ifndef CSP_LAYER_USAGE
        pthread_mutex_destroy(&gPTPDriverHandle[uiDevInst].u32MutexSem);
        /*CSP Layer Implementation To be used in Samsung Platform i.e. Valencia or Trident */
#else
        gPTPDriverHandle[uiDevInst].u32MutexSem.Destroy ();
#endif
        gPTPDriverHandle[uiDevInst].IsDeviceDetect = 0;
        gPTPDriverHandle[uiDevInst].IsOpen = 0;

        /*PtpHandle given to Application*/
        gPTPDriverHandle[uiDevInst].PtpInstHndl.u32Valid = PTP_INVALID_HNDL;
        gPTPDriverHandle[uiDevInst].PTPHndl.BusNum =-1;
        gPTPDriverHandle[uiDevInst].PTPHndl.DevNum =-1;
        gPTPDriverHandle[uiDevInst].PTPHndl.DevInst =PtpInvalidDevInst;

        /*De-Initialize the Callback List*/

        for( uiAppCbId = 0 ;uiAppCbId<MaxPtpDevSupport;uiAppCbId++)
        {
            gPTPDriverHandle[uiDevInst].PtpAppCbFn[uiAppCbId]= NULL;
        }

        if(NULL != gPTPDriverHandle[uiDevInst].pPTPDevCtxt)
        {
            if(NULL != gPTPDriverHandle[uiDevInst].pPTPDevCtxt->params.objectinfo)
            {
                PTP_API_Clear_ObjectInfo(gPTPDriverHandle[uiDevInst].pPTPDevCtxt->params.objectinfo);
                PTPFreeMem(gPTPDriverHandle[uiDevInst].pPTPDevCtxt->params.objectinfo);
            }

            PTP_API_Clear_DeviceInfo(&(gPTPDriverHandle[uiDevInst].\
                pPTPDevCtxt->params.deviceinfo));
            PTP_API_Clear_ObjectHandles(&(gPTPDriverHandle[uiDevInst].pPTPDevCtxt->params.handles));
        }
        gPTPDriverHandle[uiDevInst].ptpDevice = NULL;
        gPTPDriverHandle[uiDevInst].pPTPDevCtxt = NULL;

        PtpDevUnLock((PtpDevInstances_t)uiDevInst);

    }

    /*Free the Memory in Device Manager for Maintining the List of Devices*/
    CPTPDeviceListUpdate::Get().FreeDeviceInstances();

    /*If Device is detected Close Device*/
    PTPDevUsbExit();

    /*Indicates DeInit Status*/
    gPtpInitFlag = False;

    PTP_DRV_INFO("\nSUCCESS\n");

    return PTP_RC_OK;
}


/**
* @fn		 PTP_API_Get_PTPInstance
* @brief		This function Gets PTP Device Instance for the given Bus and Device number
* @param	(in)BusNum
*			(in)BusNum
*			(out)pPtpDevInst
* @return	 Error code
*			PTPAPI_ERROR: in case of error.
*			PTP_RC_OK: in case of success.
* @see		Can be used before calling PTP_API_Open_Driver_MPTP
*			Added as per Application requirement,
*			Do Not Pass BusNum and DevNum value as -1
*
*/
PtpErr_t  PTP_API_Get_PTPInstance(int *pPtpDevInst , int BusNum,int DevNum)
{
	PtpDevInstances_t tDevInst = PtpInvalidDevInst;
	uint32                   uDevFoundFlag = 0;
	
    if((NULL == pPtpDevInst)||(-1 == BusNum)||(-1 == DevNum))
    {
        PTP_DRV_ERR("\nWrong Device Instance\n");
        return PTPAPI_ERROR;
    }

    /*Check for Init*/
    if(False == gPtpInitFlag)
    {
        PTP_DRV_ERR("\nPTP Drv Not Initialized\n");
        return PTPAPI_ERROR;
    }
	
    /*Initially Fill Invalid Device Instance*/
    *pPtpDevInst = (int) PtpInvalidDevInst;

   for(tDevInst =PtpDevInst0;tDevInst < MaxPtpDevSupport; tDevInst = (PtpDevInstances_t)(tDevInst +1) )
   {
	   /*Lock Device Mutex*/
	  PtpDevLock(tDevInst);

	  /*Check Device Detection*/
	  if(False == gPTPDriverHandle[tDevInst].IsDeviceDetect)
	  {
		  PTP_DRV_ERR("\nDevice[%d] Not Detected\n",tDevInst);
		  PtpDevUnLock(tDevInst);
		  continue;
	  }

	  /*Check Device Context*/
	  if(NULL ==  gPTPDriverHandle[tDevInst].pPTPDevCtxt)
	  {
		  PTP_DRV_ERR("\nDevice[%d] Context NULL\n",tDevInst);
		  PtpDevUnLock(tDevInst);
		  continue;
	  }

	  /*Check Device Exists or not*/
	  if(True != staPtpChkDevExist(gPTPDriverHandle[tDevInst].pPTPDevCtxt))
	  {
		  PTP_DRV_ERR("\nDevice[%d] Not Exist\n",tDevInst);
		  PtpDevUnLock(tDevInst);
		  continue;
	  }
	  	/*Check for Device/Bus Number */
		/*Both should match in the existing List*/
            if((NULL != gPTPDriverHandle[tDevInst].ptpDevice)&&
                (gPTPDriverHandle[tDevInst].ptpDevice->ptp_path.bus_num == BusNum)
                &&(gPTPDriverHandle[tDevInst].ptpDevice->ptp_path.dev_num == DevNum)
                )
            {
		   /*Device is found*/
                PTP_DRV_INFO("\n PTPDevInstance Found::=%d\n",(int)tDevInst);
		   *pPtpDevInst = (int) tDevInst;
		   /*Flag for Device Found*/
		   uDevFoundFlag = 1;

		   /*Unlock The Device Mutex*/
                PtpDevUnLock(tDevInst);
		   /*Come out of Loop*/
                break;
            }
		else
		{
		   PTP_DRV_INFO("\n PTPDevInstance Not Found::=%d Check Another Instance\n",(int)tDevInst);
		/*Unlock The Device Mutex*/
		 PtpDevUnLock(tDevInst);
		}
   }
   /*If Device Number and  Bus Number Matches*/
   /*return SUCCESS else return FAILURE*/
   if(1== uDevFoundFlag)
   {
   	return PTP_RC_OK;
   }
   else
   {
	   return PTPAPI_ERROR;
   }
   
}

/**
* @fn		PTP_API_Open_Driver_MPTP
* @brief		This function opens the Device Instance
* @param	(in)tDevInst
*			(out)pPtpDevHndl
* @return	 Error code
*			PTPAPI_ERROR: in case of error.
*			PTP_RC_OK: in case of success.
* @see		 New API
*			Mandatory for the Application to Call this API to Access PTP Driver
*			Only after calling this API can call other APIs like PTP_API_Get_DirName_MPTP etc.
*
*/
PtpErr_t PTP_API_Open_Driver_MPTP(PtpDevInstances_t tDevInst, PTPDeviceHandle  *pPtpDevHndl)
{

    if((tDevInst <= PtpInvalidDevInst)||(tDevInst >= MaxPtpDevSupport))
    {
        PTP_DRV_ERR("\nWrong Device Instance\n");
        return PTPAPI_ERROR;
    }

    /*Check for Init*/
    if(False == gPtpInitFlag)
    {
        PTP_DRV_ERR("\nPTP Drv Not Initialized\n");
        return PTPAPI_ERROR;
    }

    /*Lock Device Mutex*/
    PtpDevLock(tDevInst);

    /*Check Device Detection*/
    if(False == gPTPDriverHandle[tDevInst].IsDeviceDetect)
    {
        PTP_DRV_ERR("\nDevice[%d] Not Detected\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }
    /*Check Device is already Open*/
    if(True == gPTPDriverHandle[tDevInst].IsOpen)
    {
        PTP_DRV_ERR("\nDevice[%d] Already Open\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }

    /*Check Device Context*/
    if(NULL == 	gPTPDriverHandle[tDevInst].pPTPDevCtxt)
    {
        PTP_DRV_ERR("\nDevice[%d] Context NULL\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }


    /*Check Device Exists or not*/
    if(True != staPtpChkDevExist(gPTPDriverHandle[tDevInst].pPTPDevCtxt))
    {
        PTP_DRV_ERR("\nDevice[%d] Not Exist\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR; 
    }


    /*Indicates Device is Open*/
    gPTPDriverHandle[tDevInst].IsOpen = True;					
    /*Validate Handle*/
    gPTPDriverHandle[tDevInst].PtpInstHndl.u32Valid = PTP_VALID_HNDL(tDevInst);
    /*Fill Bus Number*/
    gPTPDriverHandle[tDevInst].PtpInstHndl.bus_num = \
        gPTPDriverHandle[tDevInst].ptpDevice->ptp_path.bus_num;
    /*Fill Device Number*/
    gPTPDriverHandle[tDevInst].PtpInstHndl.dev_num = \
        gPTPDriverHandle[tDevInst].ptpDevice->ptp_path.dev_num;


    /*Fill Device Instance*/
    gPTPDriverHandle[tDevInst].PTPHndl.DevInst = tDevInst;
    gPTPDriverHandle[tDevInst].PTPHndl.BusNum = \
        gPTPDriverHandle[tDevInst].ptpDevice->ptp_path.bus_num;
    gPTPDriverHandle[tDevInst].PTPHndl.DevNum= \
        gPTPDriverHandle[tDevInst].ptpDevice->ptp_path.dev_num;


    /*Copy PtpInstHndl*/
    *pPtpDevHndl = (PTPDeviceHandle)&(gPTPDriverHandle[tDevInst].PtpInstHndl);										

   PTP_DRV_INFO("\nSUCCESS\n");

    PtpDevUnLock(tDevInst);
    return PTP_RC_OK;


}

/**
* @fn    	PTP_API_Close_Driver_MPTP
* @brief	   	Closes opened PTP Device .
* @param 	(in)  tDevInst
* @return    Error code
* 	   		PTPAPI_ERROR : in case of error.
* 	   		PTP_RC_OK : in case of success.
* @see	   	New API
*			Application should call this API to Close the Opened PTP Device i.e. tDevInst
*			This API provides the opposite functionality of PTP_API_Open_Driver_MPTP API
*/	
PtpErr_t PTP_API_Close_Driver_MPTP(PtpDevInstances_t tDevInst)
{


    /*Check NULL Handle and Invalid Device Instance*/
    if((tDevInst <= PtpInvalidDevInst)||(tDevInst >=MaxPtpDevSupport))
    {
	PTP_DRV_ERR("\nDevice[%d] Invalid\n",tDevInst);
        return PTPAPI_ERROR;
    }

    /*Lock Device Mutex*/
    PtpDevLock(tDevInst);

    if(False == gPTPDriverHandle[tDevInst].IsOpen)
    {
	PTP_DRV_ERR("\nDevice[%d] Not Open\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }


    /*Indicates Device is Closed*/
    gPTPDriverHandle[tDevInst].IsOpen = False;

    /* Invalid handle*/
    gPTPDriverHandle[tDevInst].PtpInstHndl.u32Valid = PTP_INVALID_HNDL;

    PtpDevUnLock(tDevInst);

    PTP_DRV_INFO("\nSUCCESS\n");

    return PTP_RC_OK;

}

/**
* @fn	  	PTP_API_Get_UsbVersion_MPTP
* @brief	  	Copies the LIBUSB Veraion
* @param 
*			(out) uiPackageVer,Package Version Buffer
* @return	  Error code
*		 	PTPAPI_ERROR : in case of error.
*		 	 PTP_RC_OK : in case of success.
* @see 	   	New API
*			Application can call this API to Know the Libusb version supported by PTP Driver
*			This is Optional API
*/

PtpErr_t PTP_API_Get_UsbVersion_MPTP(uint8 *uiPackageVer)
{
	if(NULL == uiPackageVer)
	{
		PTP_DRV_ERR("\nString is NULL\n");
		return PTPAPI_ERROR;
	}
    strncpy((char *)uiPackageVer, PACKAGE_VERSION, 20);
    uiPackageVer[20] = '\0';

	PTP_DRV_INFO("\nSUCCESS\n");
    return PTP_RC_OK;
}

/**
* @fn	  	PTP_API_InstallCb_MPTP
* @brief	  	Installs/Registers Callback Function
* @param 	(in)	 pPtpDevHndl
*		   	(in) 	tCbId
*		  	(in) 	tAppCbFn
* @return	  Error code
*		  	PTPAPI_ERROR : in case of error.
*		  	PTP_RC_OK : in case of success.
* @see 	   	New API
*			Application can register the Callback in opened PTP Device through this API
*			This API will provide the Callback functionlity from the PTP Driver
*			Main Usage of this API is for Async Implementation,currently ASYNC implementation is not provided
*			in PTP Driver
*			This is optional API
*/
PtpErr_t PTP_API_InstallCb_MPTP(PTPDeviceHandle  PtpDevHndl ,PtpCbId_t tAppCbId, PtpAppCbFn_t tAppCbFn)
{

    PTPInstHndl *pInstHndl = NULL;
    PtpDevInstances_t tDevInst = PtpInvalidDevInst;

    /*Check Parameters*/
    if((NULL == PtpDevHndl)||( tAppCbId <= PtpInvalidCbId)||\
        (tAppCbId >=MaxPtpCbId) || (NULL == tAppCbFn))
    {
	 PTP_DRV_ERR("\nInvalid Params\n");
        return PTPAPI_ERROR;
    }


    pInstHndl = (PTPInstHndl *)PtpDevHndl;

    if(!PTP_CHECKHNDL(pInstHndl->u32Valid))
    {
	 PTP_DRV_ERR("\nHandle Not Valid\n");
        return PTPAPI_ERROR;
    }


    /*Masking for Device Instance*/
    tDevInst = (PtpDevInstances_t)(pInstHndl->u32Valid & 0xf);

    if((tDevInst <= PtpInvalidDevInst)||(tDevInst >= MaxPtpDevSupport))
    {
        PTP_DRV_ERR("\nDevice[%d] Invalid\n",tDevInst);
        return PTPAPI_ERROR;
    }


    /*Lock Device Mutex*/
    PtpDevLock(tDevInst);

    /*Check Device Context*/
    if(NULL == 	gPTPDriverHandle[tDevInst].pPTPDevCtxt)
    {        
        PTP_DRV_ERR("\nDevice[%d] Context NULL\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }


    /*Check NULL Handle*/
    if(True != staPtpChkDevExist(gPTPDriverHandle[tDevInst].pPTPDevCtxt))
    {
        PtpDevUnLock(tDevInst);
        PTP_DRV_ERR("\nDevice[%d] Not Exist\n",tDevInst);
        return PTPAPI_ERROR;
    }

    /*Check Device Detection*/
    if(False == gPTPDriverHandle[tDevInst].IsDeviceDetect)
    {
        PTP_DRV_ERR("\nDevice[%d] Not Detected\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }
    /*Check Device  Open*/
    if(False == gPTPDriverHandle[tDevInst].IsOpen)
    {
        PTP_DRV_ERR("\nDevice[%d] Not Open\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }

    /*Register Callback for particular Callback ID*/
    gPTPDriverHandle[tDevInst].PtpAppCbFn[tAppCbId]= tAppCbFn;

     PTP_DRV_INFO("\nSUCCESS\n");		

    PtpDevUnLock(tDevInst);
    return PTP_RC_OK;
}

/**
* @fn	  	PTP_API_UnInstallCb_MPTP
* @brief	  	UnInstalls/UnRegisters Callback Function
* @param 	(in)	 pPtpDevHndl
*		   	(in) 	tAppCbId
* @return	  Error code
*		  	PTPAPI_ERROR : in case of error.
*		  	PTP_RC_OK : in case of success.
* @see 	   	New API
*			Application can unregister the Callback function through this API
*			This is optional API
*/
PtpErr_t PTP_API_UnInstallCb_MPTP(PTPDeviceHandle  PtpDevHndl, PtpCbId_t tAppCbId)
{

    PTPInstHndl *pInstHndl =NULL;
    PtpDevInstances_t tDevInst;

    /*Check Parameters*/
    if((NULL == PtpDevHndl)||( tAppCbId <= PtpInvalidCbId)||(tAppCbId >=MaxPtpCbId))
    {
		PTP_DRV_ERR("\nInvalid Params\n");
		return PTPAPI_ERROR;
    }


    pInstHndl = (PTPInstHndl *)PtpDevHndl;

    if(!PTP_CHECKHNDL(pInstHndl->u32Valid))
    {
        PTP_DRV_ERR("\nHandle Not Valid\n");
        return PTPAPI_ERROR;
    }

    /*Masking for Device Instance*/
    tDevInst = (PtpDevInstances_t)(pInstHndl->u32Valid & 0xf);

    if((tDevInst <= PtpInvalidDevInst)||(tDevInst >= MaxPtpDevSupport))
    {
        PTP_DRV_ERR("\nDevice[%d] Invalid\n",tDevInst);
        return PTPAPI_ERROR;
    }


    /*Lock Device Mutex*/
    PtpDevLock(tDevInst);

    /*Check Device Context*/
    if(NULL ==	gPTPDriverHandle[tDevInst].pPTPDevCtxt)
    {
        PTP_DRV_ERR("\nDevice[%d] Context NULL\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }


    /*Check NULL Handle*/
    if(True != staPtpChkDevExist(gPTPDriverHandle[tDevInst].pPTPDevCtxt))
    {
        PTP_DRV_ERR("\nDevice[%d] Not Exist\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }

    /*Check Device Detection*/
    if(False == gPTPDriverHandle[tDevInst].IsDeviceDetect)
    {
        PTP_DRV_ERR("\nDevice[%d] Not Detected\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }
    /*Check Device	Open*/
    if(False == gPTPDriverHandle[tDevInst].IsOpen)
    {
        PTP_DRV_ERR("\nDevice[%d] Not Open\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }

    /*Register Callback for particular Callback ID*/
    gPTPDriverHandle[tDevInst].PtpAppCbFn[tAppCbId]= NULL;

    PTP_DRV_INFO("\nSUCCESS\n"); 	   

    PtpDevUnLock(tDevInst);
    return PTP_RC_OK;
}

/**
* @fn	  PTP_API_GetDeviceList
* @brief	  Get Device List i.e connected Device
* @param (in)	 PtpDevHandle
*		   (out)      pDevCnt
* @return	  Error code
*		  PTPAPI_ERROR: in case of error.
*		  PTP_RC_OK: in case of success.
* @see 	   Changed 1st parameter in Previous API, i.e.  UsbDev* and swaps the Parameters
*/

PtpErr_t  PTP_API_GetDeviceList(uint32 *pDevCnt,PTPDeviceHandle   PtpDevHandle)
{
    uint32_t uiIndx =0;
    uint32_t uiDevCnt =0;

    PTPInstHndl *pPtpInstHndl=NULL;
    PtpDevInstances_t tDevInst = PtpInvalidDevInst;

    /*Check Parameters*/
    if((NULL == pDevCnt )||(NULL == PtpDevHandle))
    {
        PTP_DRV_ERR("\nInvalid params\n");
        return PTPAPI_ERROR;

    }
    pPtpInstHndl = (PTPInstHndl *)PtpDevHandle;
    if(!PTP_CHECKHNDL(pPtpInstHndl->u32Valid))
    {
        PTP_DRV_ERR("\nHandle Not Valid\n");
        return PTPAPI_ERROR;
    }
    /*Masking for Device Instance*/
    tDevInst = (PtpDevInstances_t)(pPtpInstHndl->u32Valid & 0xf);

    /*Lock Device Mutex*/
    PtpDevLock(tDevInst);
    /*Check Device Detection*/
    if(False == gPTPDriverHandle[tDevInst].IsDeviceDetect)
    {
        PtpDevUnLock(tDevInst);
        PTP_DRV_ERR("\nDevice[%d] Not Detected\n",tDevInst);
        return PTPAPI_ERROR;
    }
    /*Check Device Context*/
    if(NULL ==	gPTPDriverHandle[tDevInst].pPTPDevCtxt)
    {
        PTP_DRV_ERR("\nDevice[%d] Context NULL\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }
    /*Check Device Exists or not*/
    if(True != staPtpChkDevExist(gPTPDriverHandle[tDevInst].pPTPDevCtxt))
    {
        PTP_DRV_ERR("\nDevice[%d] Not Exist\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR; 
    }

    PtpDevUnLock(tDevInst);

    for(uiIndx = 0;uiIndx < MaxPtpDevSupport;uiIndx++)
    {
        PtpDevLock(tDevInst);

        if(True == gPTPDriverHandle[uiIndx].IsDeviceDetect)
        {
            uiDevCnt++;
        }

        PtpDevUnLock(tDevInst);
    }

    *pDevCnt = uiDevCnt;

    PTP_DRV_INFO("\nSUCCESS\n"); 	   

    return PTP_RC_OK;	
}

/**
* @fn	 	PTP_API_Get_DeviceInfo_MPTP
* @brief	  	Gets Device Information Structure
* @param  	(out)	pDevInfo
*			(in)	PtpDevHandle
* @return	  Error code
*		 	PTPAPI_ERROR : in case of error.
*		  	PTP_RC_OK: in case of success.
* @see 	   	Old API
*/ 

PtpErr_t PTP_API_Get_DeviceInfo_MPTP(PTPDeviceInfo *pDevInfo,PTPDeviceHandle   PtpDevHandle)
{
    #define PTP_DEFAULT_MODEL_NAME		"Digital Camera"	 
    #define PTP_DEFAULT_VENDOR_NAME		"PTP Device"
    #define PTP_DEFAULT_SERIAL_NAME		"123456789012345678901234567890"
    uint16 uiResult = PTP_RC_OK;
    PTPDevContext *pPtpDevCtxt = NULL;
    PTPInstHndl *pPtpInstHndl=NULL;
    PtpDevInstances_t tDevInst = PtpInvalidDevInst;

    /*Check Parameters*/
    if((NULL == pDevInfo )||(NULL == PtpDevHandle))
    {
        PTP_DRV_ERR("\nInvalid params\n");
        return PTPAPI_ERROR;
    }
     /*Check for Init*/
    if(False == gPtpInitFlag)
    {
        PTP_DRV_ERR("\nPTP Drv Not Initialized\n");
        return PTPAPI_ERROR;
    }
	
    pPtpInstHndl = (PTPInstHndl *)PtpDevHandle;
    if(!PTP_CHECKHNDL(pPtpInstHndl->u32Valid))
    {
        PTP_DRV_ERR("\nHandle Not Valid\n");
        return PTPAPI_ERROR;
    }

    /*Masking for Device Instance*/
    tDevInst = (PtpDevInstances_t)(pPtpInstHndl->u32Valid & 0xf);

    /*Lock Device Mutex*/
    PtpDevLock(tDevInst);
    /*Check Device Detection*/
    if(False == gPTPDriverHandle[tDevInst].IsDeviceDetect)
    {
        PtpDevUnLock(tDevInst);
        PTP_DRV_ERR("\nDevice[%d] Not Detected\n",tDevInst);
        return PTPAPI_ERROR;
    }

    /*Check Device Context*/
    if(NULL ==	gPTPDriverHandle[tDevInst].pPTPDevCtxt)
    {
        PTP_DRV_ERR("\nDevice[%d] Context NULL\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }


    /*Check Device Exists or not*/
    if(True != staPtpChkDevExist(gPTPDriverHandle[tDevInst].pPTPDevCtxt))
    {
        PTP_DRV_ERR("\nDevice[%d] Not Exist\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR; 
    }

    /*Device Context*/
    pPtpDevCtxt = gPTPDriverHandle[tDevInst].pPTPDevCtxt;

    PtpPrivateInitTransaction (pPtpDevCtxt);

    PTP_DRV_INFO("\nptp_getdeviceinfo : start\n");

    uiResult = ptp_getdeviceinfo (&(pPtpDevCtxt->params), pDevInfo);
    if( uiResult != PTP_RC_OK) 
    {

        PTP_DRV_ERR( "ptp_getdeviceinfo return (%x)\n", uiResult);
        PtpDevUnLock(tDevInst);
        return uiResult;
    }

	if(NULL == pDevInfo->Manufacturer)
	 {
		pDevInfo->Manufacturer = (char *)malloc(sizeof(PTP_DEFAULT_VENDOR_NAME) +1);
		if(NULL != pDevInfo->Manufacturer)
		{
			memset(pDevInfo->Manufacturer, 0,sizeof(PTP_DEFAULT_VENDOR_NAME) + 1 );
		 	strncpy (pDevInfo->Manufacturer, PTP_DEFAULT_VENDOR_NAME, 11);			 
		}

	 }
	 if(NULL == pDevInfo->Model)
	 {
		 pDevInfo->Model = (char *)malloc(sizeof(PTP_DEFAULT_MODEL_NAME) +1);
		if(NULL != pDevInfo->Model)
		{	
			memset(pDevInfo->Model, 0,sizeof(PTP_DEFAULT_MODEL_NAME) +1 );
			 strncpy (pDevInfo->Model, PTP_DEFAULT_MODEL_NAME,15); 		 
		}

	 }
	if(NULL == pDevInfo->SerialNumber)
	{
 	 	pDevInfo->SerialNumber = (char *)malloc(sizeof(PTP_DEFAULT_SERIAL_NAME) +1);
		if(NULL != pDevInfo->SerialNumber)
		{	
			memset(pDevInfo->SerialNumber, 0,sizeof(PTP_DEFAULT_SERIAL_NAME) +1 );
			strncpy (pDevInfo->SerialNumber,PTP_DEFAULT_SERIAL_NAME,31);
		}
	}
    PTP_DRV_INFO("\nptp_getdeviceinfo : end\n");
    PTP_DRV_INFO("\nSUCCESS\n"); 	   

    PtpDevUnLock(tDevInst);
    return uiResult;	
}



/**
* @fn	  	PTP_API_Get_StorageIDs_MPTP
* @brief	  	Gets Storage IDs from PTP Device .
* @param 	(out) 	pStorageIds
*		   	(in)	 PtpDevHandle
* @return	  Error code
*		  	PTPAPI_ERROR : in case of error.
*		  	PTP_RC_OK : in case of success.
* @see 	   	Old API
*/

PtpErr_t PTP_API_Get_StorageIDs_MPTP( PTPStorageIDs *pStorageIds,PTPDeviceHandle   PtpDevHandle)
{
    uint16 uiResult = PTP_RC_OK;
    PTPDevContext *pPtpDevCtxt = NULL;
    PTPInstHndl *pPtpInstHndl=NULL;
    PtpDevInstances_t tDevInst = PtpInvalidDevInst;

    /*Check Parameters*/
    if((NULL == pStorageIds )||(NULL == PtpDevHandle))
    {
        PTP_DRV_ERR("\nInvalid params\n");
        return PTPAPI_ERROR;

    }
    /*Check for Init*/
    if(False == gPtpInitFlag)
    {
        PTP_DRV_ERR("\nPTP Drv Not Initialized\n");
        return PTPAPI_ERROR;
    }

    pPtpInstHndl = (PTPInstHndl *)PtpDevHandle;

    if(!PTP_CHECKHNDL(pPtpInstHndl->u32Valid))
    {
        PTP_DRV_ERR("\nHandle Not Valid\n");
        return PTPAPI_ERROR;
    }


    /*Masking for Device Instance*/
    tDevInst = (PtpDevInstances_t)(pPtpInstHndl->u32Valid & 0xf);

    /*Lock Device Mutex*/
    PtpDevLock(tDevInst);

    /*Check Device Detection*/
    if(False == gPTPDriverHandle[tDevInst].IsDeviceDetect)
    {
        PTP_DRV_ERR("\nDevice[%d] Not Detected\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }

    /*Check Device Context*/
    if(NULL ==	gPTPDriverHandle[tDevInst].pPTPDevCtxt)
    {
        PTP_DRV_ERR("\nDevice[%d] Context NULL\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }


    /*Check Device Exists or not*/
    if(True != staPtpChkDevExist(gPTPDriverHandle[tDevInst].pPTPDevCtxt))
    {
        PTP_DRV_ERR("\nDevice[%d] Not Exist\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR; 
    }

    /*Device Context*/
    pPtpDevCtxt = gPTPDriverHandle[tDevInst].pPTPDevCtxt;


    PtpPrivateInitTransaction (pPtpDevCtxt);

    PTP_DRV_INFO("\nptp_getstorageids : start\n");

    uiResult = ptp_getstorageids (&(pPtpDevCtxt->params), pStorageIds);

    if( uiResult != PTP_RC_OK)
    {
        PTP_DRV_ERR("\nptp_getstorageids return (%x)\n", uiResult);
        PtpDevUnLock(tDevInst);
        return uiResult;
    }       

    PTP_DRV_INFO("\nptp_getstorageids : end(%x)\n", uiResult);

    PTP_DRV_INFO("\nSUCCESS\n");

    PtpDevUnLock(tDevInst);
    return uiResult;
}       

/**
* @fn	  PTP_API_Get_StorageInfos_MPTP
* @brief	  Gets Storage IDs from PTP Device .
* @param (in)	 PtpDevHandle
*		   (in) 	uiStorageId
*		   (out) 	pStorageInfo
* @return	  Error code
*		  PTPAPI_ERROR : in case of error.
*		  PTP_RC_OK : in case of success.
* @see 	   	Old API
*/

PtpErr_t PTP_API_Get_StorageInfos_MPTP(uint32_t uiStorageId, PTPStorageInfo* pStorageInfo,PTPDeviceHandle   PtpDevHandle)
{

    uint16 uiResult = PTP_RC_OK;

    PTPDevContext *pPtpDevCtxt = NULL;
    PTPInstHndl *pPtpInstHndl=NULL;
    PtpDevInstances_t tDevInst = PtpInvalidDevInst;

    /*Check Parameters*/
    if((NULL == pStorageInfo )||(NULL == PtpDevHandle))
    {
        PTP_DRV_ERR("\nInvalid params\n");
        return PTPAPI_ERROR;
    }
    /*Check for Init*/
    if(False == gPtpInitFlag)
    {
        PTP_DRV_ERR("\nPTP Drv Not Initialized\n");
        return PTPAPI_ERROR;
    }
    pPtpInstHndl = (PTPInstHndl *)PtpDevHandle;
    if(!PTP_CHECKHNDL(pPtpInstHndl->u32Valid))
    {
        PTP_DRV_ERR("\nHandle Not Valid\n");
        return PTPAPI_ERROR;
    }

    /*Masking for Device Instance*/
    tDevInst = (PtpDevInstances_t)(pPtpInstHndl->u32Valid & 0xf);

    /*Lock Device Mutex*/
    PtpDevLock(tDevInst);


    /*Check Device Detection*/
    if(False == gPTPDriverHandle[tDevInst].IsDeviceDetect)
    {
        PTP_DRV_ERR("\nDevice[%d] Not Detected\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }

    /*Check Device Context*/
    if(NULL ==	gPTPDriverHandle[tDevInst].pPTPDevCtxt)
    {
        PTP_DRV_ERR("\nDevice[%d] Context NULL\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }


    /*Check Device Exists or not*/
    if(True != staPtpChkDevExist(gPTPDriverHandle[tDevInst].pPTPDevCtxt))
    {
        PTP_DRV_ERR("\nDevice[%d] Not Exist\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR; 
    }

    /*Device Context*/
    pPtpDevCtxt = gPTPDriverHandle[tDevInst].pPTPDevCtxt;
    PtpPrivateInitTransaction (pPtpDevCtxt);

    PTP_DRV_INFO("\nptp_getstorageinfo : start\n");

    if(uiStorageId & PTP_PHYSICAL_STORAGE_MASK) 
    {
        if(uiStorageId & PTP_LOGICAL_STORAGE_MASK)
        {
            PTP_DRV_INFO("\nPTP_LOGICAL_STORAGE existing\n ");
            uiResult = ptp_getstorageinfo (&(pPtpDevCtxt->params), uiStorageId, pStorageInfo);
            if( uiResult != PTP_RC_OK)
            {

                PTP_DRV_ERR("\nptp_getstorageinfo return (%x)\n", uiResult);
                PtpDevUnLock(tDevInst);
                return uiResult;
            }
        }
        else
        {
            PTP_DRV_ERR("\nPTP_LOGICAL_STORAGE not existing\n");
            PtpDevUnLock(tDevInst);
            return PTP_RC_Undefined;

        }
    }
    else
    {
        PTP_DRV_ERR("\nPTP_PHYSICAL_STORAGE not existing\n ");
        PtpDevUnLock(tDevInst);
        return PTP_RC_Undefined;

    }

    PTP_DRV_INFO("\nptp_getstorageinfo : end(%x)\n", uiResult);

    PtpDevUnLock(tDevInst);
    return uiResult;
}


/**
* @fn	  Is_PTP_device_MPTP
* @brief	  Checks device is PTP or Not
* @param (in)	 ptpHandle
* @return	  Error code
*		 PTPAPI_ERROR : in case of error.
*		  PTP_RC_OK : in case of success.
* @see 	   	Old API
*/

PtpErr_t Is_PTP_device_MPTP(PTPDeviceHandle  PtpDevHndl)
{

    uint16 uiResult = PTP_RC_OK;
    PTPDevContext *pPtpDevCtxt = NULL;
    PTPInstHndl *pPtpInstHndl=NULL;
    PtpDevInstances_t tDevInst = PtpInvalidDevInst;

    /*Check Parameters*/
    if(NULL == PtpDevHndl)
    {
        PTP_DRV_ERR("\nInvalid params\n");
        return PTPAPI_ERROR;

    }
    /*Check for Init*/
    if(False == gPtpInitFlag)
    {
        PTP_DRV_ERR("\nPTP Drv Not Initialized\n");
        return PTPAPI_ERROR;
    }
    pPtpInstHndl = (PTPInstHndl *)PtpDevHndl;
    if(!PTP_CHECKHNDL(pPtpInstHndl->u32Valid))
    {
        PTP_DRV_ERR("\nHandle Not Valid\n");
        return PTPAPI_ERROR;
    }

    /*Masking for Device Instance*/
    tDevInst = (PtpDevInstances_t)(pPtpInstHndl->u32Valid & 0xf);
    /*Lock Device Mutex*/
    PtpDevLock(tDevInst);

    /*Check Device Detection*/
    if(False == gPTPDriverHandle[tDevInst].IsDeviceDetect)
    {
        PTP_DRV_ERR("\nDevice[%d] Not Detected\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }

    /*Check Device Context*/
    if(NULL ==	gPTPDriverHandle[tDevInst].pPTPDevCtxt)
    {
        PTP_DRV_ERR("\nDevice[%d] Context NULL\n",tDevInst);
	  PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }
    /*Check Device Exists or not*/
    if(True != staPtpChkDevExist(gPTPDriverHandle[tDevInst].pPTPDevCtxt))
    {
        PTP_DRV_ERR("\nDevice[%d] Not Exist\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR; 
    }
    /*Device Context*/
    pPtpDevCtxt = gPTPDriverHandle[tDevInst].pPTPDevCtxt;
    PtpPrivateInitTransaction (pPtpDevCtxt);

    PTP_DRV_INFO("\nptp_getdeviceinfo : start\n");
	
    uiResult = ptp_getdeviceinfo (&(pPtpDevCtxt->params), \
        &(pPtpDevCtxt->params.deviceinfo));
    if( uiResult != PTP_RC_OK)
    {
        PTP_DRV_ERR( "ptp_getdeviceinfo return (%x)\n", uiResult);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }

    PTP_DRV_ERR( "ptp_getdeviceinfo : end (%x)\n", uiResult);

    PTP_API_Clear_DeviceInfo(&(pPtpDevCtxt->params.deviceinfo));
    PTP_DRV_INFO("\nSUCCESS\n");		
    PtpDevUnLock(tDevInst);
    return PTP_RC_OK;

}

/**
* @fn	  PTP_API_Get_JpegObjectHandles_MPTP
* @brief	  This function gets Jpeg image object handles
* @param (in)	 PtpDevHndl
*		   (in) 	uiStorageID
*		   (out) 	pPtpObjHndl
* @return	  Error code
*		 PTPAPI_ERROR : in case of error.
*		  PTP_RC_OK : in case of success.
*/

PtpErr_t PTP_API_Get_JpegObjectHandles_MPTP( uint32 uiStorageID, PTPObjectHandles *pPtpObjHndl,PTPDeviceHandle  PtpDevHndl)
{

    uint16	uiResult = PTP_RC_OK;
    uint32 i=0;

    PTPDevContext *pPtpDevCtxt = NULL;

    PTPInstHndl *pPtpInstHndl=NULL;
    PtpDevInstances_t tDevInst = PtpInvalidDevInst;

    /*Check Parameters*/
    if((NULL == PtpDevHndl)||(NULL== pPtpObjHndl ))
    {
        PTP_DRV_ERR("\nInvalid params\n");
        return PTPAPI_ERROR;

    }

    /*Check for Init*/
    if(False == gPtpInitFlag)
    {
        PTP_DRV_ERR("\nPTP Drv Not Initialized\n");
        return PTPAPI_ERROR;
    }
    pPtpInstHndl = (PTPInstHndl *)PtpDevHndl;
    if(!PTP_CHECKHNDL(pPtpInstHndl->u32Valid))
    {
        PTP_DRV_ERR("\nHandle Not Valid\n");
        return PTPAPI_ERROR;
    }

    /*Masking for Device Instance*/
    tDevInst = (PtpDevInstances_t)(pPtpInstHndl->u32Valid & 0xf);

    /*Lock Device Mutex*/
    PtpDevLock(tDevInst);

    /*Check Device Detection*/
    if(False == gPTPDriverHandle[tDevInst].IsDeviceDetect)
    {
        PTP_DRV_ERR("\nDevice[%d] Not Detected\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }


    /*Check Device Context*/
    if(NULL ==	gPTPDriverHandle[tDevInst].pPTPDevCtxt)
    {
        PTP_DRV_ERR("\nDevice[%d] Context NULL\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }


    /*Check Device Exists or not*/
    if(True != staPtpChkDevExist(gPTPDriverHandle[tDevInst].pPTPDevCtxt))
    {
	PTP_DRV_ERR("\nDevice[%d] Not Exist\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR; 
    }

    /*Device Context*/
    pPtpDevCtxt = gPTPDriverHandle[tDevInst].pPTPDevCtxt;

    PTP_DRV_INFO("\nptp_getobjecthandles : start\n");	

    //Find all jpeg list from the PTP device
    // ÀÏºÎ PTP ÀåºñÀÇ °æ¿ì(panasonic FX12), jpeg handle ÀÌ¿ÜÀÇ handle À» return ÇÏ¹Ç·Î GetObjectinfo¸¦ 
    // object typeÀ» Ã¼Å©ÇØ¼­ filtering ÇØ¾ßÇÑ´Ù. (by kks)	
    uiResult = ptp_getobjecthandles (&(pPtpDevCtxt->params),uiStorageID, PTP_OFC_EXIF_JPEG, PTP_HANDLER_ROOT, pPtpObjHndl);
    if (uiResult != PTP_RC_OK)
    {
        PTP_DRV_ERR("\n ptp_getobjecthandles fail (0x%x)\n",uiResult);
        PtpDevUnLock(tDevInst);
        return uiResult;
    }

    PTP_DRV_INFO("\nhandles num %d\n", pPtpObjHndl->n);

    for (i = 0; i < pPtpObjHndl->n; i++)
    {
        PTP_DRV_INFO("\nhandle [%lu] : %x \n", i,pPtpObjHndl->Handler[i]);
    }

    PTP_DRV_INFO("\nptp_getobjecthandles : end(%x)\n",uiResult);	

    PTP_DRV_INFO("\nSUCCESS\n");
	
    PtpDevUnLock(tDevInst);
    return uiResult;	
}

/**
* @fn	  PTP_API_Is_Handle_Vaild_MPTP
* @brief	  Checks object Handle is valid or not
* @param (in)	 PtpDevHndl
*		   (in) 	uiObjectHandle
* @return	  Error code
*		  PTPAPI_ERROR: in case of error.
*		  PTP_RC_OK: in case of success.
*/

PtpErr_t PTP_API_Is_Handle_Vaild_MPTP (uint32 uiObjectHandle,PTPDeviceHandle  PtpDevHndl)
{
#ifndef OPTIMIZED_PTP
    /*
    *
    * 2007-11-14 (KKS) : PTP ¸®½ºÆ® ÃßÃâ¼º´É °³¼±À» À§ÇØ »èÁ¦ÇÔ
    * PTP device¿¡ ÆÄÀÏÀÌ ¸¹À»¼ö·Ï handleÀÇ À¯È¿¼º °Ë»ç·Î ÀÎÇØ µ¿ÀÛ½Ã°£ÀÌ ´À·ÁÁö´Â ¹®Á¦°¡ ¹ß»ýÇÔ
    * ÇÚµéÀÇ À¯È¿¼º ÆÇ´ÜÀº API È£Ãâ½Ã ptp commandÀÇ return value Ã¼Å©·Î Ã³¸®ÇØ¾ßÇÔ
    */
    PTPObjectHandles ObjectHandles;
    Bool IsObjectHandleFound = False;
    uint32 i = 0;
    PTPDevContext *pPtpDevCtxt = NULL;

    PTPInstHndl *pPtpInstHndl=NULL;
    PtpDevInstances_t tDevInst = PtpInvalidDevInst;

    /*Check Parameters*/
    if(NULL == PtpDevHndl)
    {
        PTP_DRV_ERR("\nInvalid params\n");
        return PTPAPI_ERROR;

    }

    pPtpInstHndl = (PTPInstHndl *)PtpDevHndl;

    if(!PTP_CHECKHNDL(pPtpInstHndl->u32Valid))
    {
        PTP_DRV_ERR("\nHandle Not Valid\n");
        return PTPAPI_ERROR;
    }


    /*Masking for Device Instance*/
    tDevInst = (PtpDevInstances_t)(pPtpInstHndl->u32Valid & 0xf);

    /*Lock Device Mutex*/
    PtpDevLock(tDevInst);

    /*Check Device Detection*/
    if(False == gPTPDriverHandle[tDevInst].IsDeviceDetect)
    {
        PTP_DRV_ERR("\nDevice[%d] Not Detected\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }

    /*Check Device Context*/
    if(NULL ==	gPTPDriverHandle[tDevInst].pPTPDevCtxt)
    {
        PTP_DRV_ERR("\nDevice[%d] Context NULL\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }


    /*Check Device Exists or not*/
    if(True != staPtpChkDevExist(gPTPDriverHandle[tDevInst].pPTPDevCtxt))
    {
	  PTP_DRV_ERR("\nDevice[%d] Not Exist\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR; 
    }

    /*Device Context*/
    pPtpDevCtxt = gPTPDriverHandle[tDevInst].pPTPDevCtxt;

    PTP_API_Init_ObjectHandles(&ObjectHandles);
    //HANDLE CHECK (All handles in device)

    PTP_DRV_INFO("\nptp_getobjecthandles : start\n");	

    if (ptp_getobjecthandles (&(pPtpDevCtxt->params), 0xffffffff,
        /*uint32_t objectformatcode -> optional*/ 0, /*uint32_t associationOH -> optional*/0,
        &ObjectHandles) != PTP_RC_OK)
    {
	PTP_DRV_ERR("\nptp_getobjecthandles Err\n");
        /*Memory Leak Check*/
        PTP_API_Clear_ObjectHandles(&ObjectHandles);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }

    //Find a handle matched with the input handle

    for (i = 0; i < ObjectHandles.n; i++)
    {
        PTP_DRV_INFO("\nhandle [%lu] : %x ", i, ObjectHandles.Handler[i]);
        if (uiObjectHandle == ObjectHandles.Handler[i])
        {
            PTP_DRV_WARN("\nHandle is found\n");								
            IsObjectHandleFound = True;
            break;
        }
    }

    if (IsObjectHandleFound == False)
    {		
        PTP_DRV_ERR("\n\nHandle is not found\n");		
        /*Memory Leak Check*/
        PTP_API_Clear_ObjectHandles(&ObjectHandles);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }	

    PTP_DRV_INFO("\nptp_getobjecthandles : end\n");	

    /*Memory Leak Check*/
    PTP_API_Clear_ObjectHandles(&ObjectHandles);
    PTP_DRV_INFO("\nSUCCESS\n");
    PtpDevUnLock(tDevInst);
    return PTP_RC_OK;
#else
	return PTP_RC_OK;
#endif
}

/**
* @fn	  PTP_API_Get_ParentHandle_MPTP
* @brief	  Gets Parent Objects Handle
* @param (in)	 PtpDevHndl
*		   (in) 	uiObjectHandle
*		   (out) 	pParentHandle
* @return	  Error code
*		  PTPAPI_ERROR : in case of error.
*		  PTP_RC_OK : in case of success.
*/

PtpErr_t PTP_API_Get_ParentHandle_MPTP ( uint32 uiObjectHandle, uint32 *pParentHandle,PTPDeviceHandle  PtpDevHndl)
{

    PTPObjectInfo 	PtpObjInfo;
    uint16	uiResult = PTP_RC_OK;

    PTPDevContext *pPtpDevCtxt = NULL;

    PTPInstHndl *pPtpInstHndl=NULL;
    PtpDevInstances_t tDevInst = PtpInvalidDevInst;

    /*Check Parameters*/
    if(NULL == PtpDevHndl)
    {
        PTP_DRV_ERR("\nInvalid params\n");
        return PTPAPI_ERROR;

    }

    /*Check for Init*/
    if(False == gPtpInitFlag)
    {
        PTP_DRV_ERR("\nPTP Drv Not Initialized\n");
        return PTPAPI_ERROR;
    }
    pPtpInstHndl = (PTPInstHndl *)PtpDevHndl;

    if(!PTP_CHECKHNDL(pPtpInstHndl->u32Valid))
    {
        PTP_DRV_ERR("\nHandle Not Valid\n");
        return PTPAPI_ERROR;
    }
    /*Masking for Device Instance*/
    tDevInst = (PtpDevInstances_t)(pPtpInstHndl->u32Valid & 0xf);
    /*Lock Device Mutex*/
    PtpDevLock(tDevInst);

    /*Check Device Detection*/
    if(False == gPTPDriverHandle[tDevInst].IsDeviceDetect)
    {
        PTP_DRV_ERR("\nDevice[%d] Not Detected\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }

    /*Check Device Context*/
    if(NULL ==	gPTPDriverHandle[tDevInst].pPTPDevCtxt)
    {
        PTP_DRV_ERR("\nDevice[%d] Context NULL\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }


    /*Check Device Exists or not*/
    if(True != staPtpChkDevExist(gPTPDriverHandle[tDevInst].pPTPDevCtxt))
    {
	PTP_DRV_ERR("\nDevice[%d] Not Exist\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR; 
    }

    /*Device Context*/
    pPtpDevCtxt = gPTPDriverHandle[tDevInst].pPTPDevCtxt;

#ifndef OPTIMIZED_PTP
    if (!PTP_API_Is_Handle_Vaild_MPTP(uiObjectHandle,PtpDevHndl))
    {
        PTP_DRV_ERR("\nObject Handle Not in Device\n");
	  PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR; //just representing handle error		
    }
#endif

    PtpPrivateInitTransaction (pPtpDevCtxt);

    PTP_API_Init_ObjectInfo(&PtpObjInfo);
    PTP_DRV_INFO("\nptp_getobjectinfo : start\n");


    uiResult = ptp_getobjectinfo(&(pPtpDevCtxt->params), uiObjectHandle, &PtpObjInfo );
    if ( uiResult != PTP_RC_OK )
    {
        PTP_DRV_ERR("\nptp_getobjectinfo return(%x)\n", uiResult);
        PTP_API_Clear_ObjectInfo(&PtpObjInfo);	
        PtpDevUnLock(tDevInst);
        return uiResult;
    }

    *pParentHandle = PtpObjInfo.ParentObject;
    PTP_DRV_INFO (" handle = %lu, parent handle = %lu\n", uiObjectHandle, *pParentHandle);
    PTP_API_Clear_ObjectInfo(&PtpObjInfo);


    PTP_DRV_INFO("\nptp_getobjectinfo : end(%x)\n\n", uiResult);
	PTP_DRV_INFO("\nSUCCESS\n");
    PtpDevUnLock(tDevInst);
    return uiResult;	
}

/**
* @fn	  PTP_API_Get_DirName_MPTP
* @brief	  Gets Directory Name
* @param (in)	 PtpDevHndl
*		   (in) 	uiObjectHandle
*		   (out) 	pParentDir
* @return	  Error code
*		 PTPAPI_ERROR : in case of error.
*		  PTP_RC_OK: in case of success.
*/

PtpErr_t PTP_API_Get_DirName_MPTP (uint32 uiObjectHandle, char *pParentDir,PTPDeviceHandle  PtpDevHndl )
{

    PTPObjectInfo 	PtpObjInfo;
    uint16	uiResult = PTP_RC_OK;
    PTPDevContext *pPtpDevCtxt = NULL;
    PTPInstHndl *pPtpInstHndl=NULL;
    PtpDevInstances_t tDevInst= PtpInvalidDevInst;
    uint16 len = 0;
    /*Check Parameters*/
    if((NULL == PtpDevHndl)||(NULL == pParentDir))
    {
        PTP_DRV_ERR("\nInvalid params\n");
        return PTPAPI_ERROR;

    }

    /*Check for Init*/
    if(False == gPtpInitFlag)
    {
        PTP_DRV_ERR("\nPTP Drv Not Initialized\n");
        return PTPAPI_ERROR;
    }


    pPtpInstHndl = (PTPInstHndl *)PtpDevHndl;

    if(!PTP_CHECKHNDL(pPtpInstHndl->u32Valid))
    {
        PTP_DRV_ERR("\nHandle Not Valid\n");
        return PTPAPI_ERROR;
    }


    /*Masking for Device Instance*/
    tDevInst = (PtpDevInstances_t)(pPtpInstHndl->u32Valid & 0xf);

    /*Lock Device Mutex*/
    PtpDevLock(tDevInst);

    /*Check Device Detection*/
    if(False == gPTPDriverHandle[tDevInst].IsDeviceDetect)
    {
        PTP_DRV_ERR("\nDevice[%d] Not Detected\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }


    /*Check Device Context*/
    if(NULL ==	gPTPDriverHandle[tDevInst].pPTPDevCtxt)
    {
        PTP_DRV_ERR("\nDevice[%d] Context NULL\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }


    /*Check Device Exists or not*/
    if(True != staPtpChkDevExist(gPTPDriverHandle[tDevInst].pPTPDevCtxt))
    {
	PTP_DRV_ERR("\nDevice[%d] Not Exist\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR; 
    }

    /*Device Context*/
    pPtpDevCtxt = gPTPDriverHandle[tDevInst].pPTPDevCtxt;

#ifndef OPTIMIZED_PTP
    if (!PTP_API_Is_Handle_Vaild_MPTP(uiObjectHandle,PtpDevHndl))
    {
	PTP_DRV_ERR("\nObject Handle Not Found in Device\n");
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR; //just representing handle error		
    }
#endif

    PtpPrivateInitTransaction (pPtpDevCtxt);

    PTP_API_Init_ObjectInfo(&PtpObjInfo);
    PTP_DRV_INFO("\nptp_getobjectinfo : start(H:%lu)\n", uiObjectHandle);
    uiResult = ptp_getobjectinfo(&(pPtpDevCtxt->params), uiObjectHandle, &PtpObjInfo );
    if ( uiResult != PTP_RC_OK )
    {
        PTP_DRV_ERR("\nptp_getobjectinfo return(%x)\n", uiResult);
        PTP_API_Clear_ObjectInfo(&PtpObjInfo);
        PtpDevUnLock(tDevInst);
        return uiResult;
    }


    len = strlen(PtpObjInfo.Filename);

    strncpy(pParentDir, PtpObjInfo.Filename,len);
    pParentDir[len] = '\0';

    PTP_DRV_INFO (" handle = %lu,parent name = %s\n", uiObjectHandle, pParentDir);
    PTP_API_Clear_ObjectInfo(&PtpObjInfo);
    PTP_DRV_INFO("\nptp_getobjectinfo : end(%x)\n\n", uiResult);
    PTP_DRV_INFO("\nSUCCESS\n");

    PtpDevUnLock(tDevInst);
    return uiResult;	
}


/**
* @fn	  PTP_API_Get_NumOfObjects_MPTP
* @brief	  Get Number of objects
* @param (in)	 PtpDevHndl
*		   (in) 	uiStorageID
*		   (in) 	uiParentObjectHandle
*		   (in) 	ObjectFormat
*		   (out) 	pnNum
* @return	  Error code
*		  PTPAPI_ERROR : in case of error.
*		 PTP_RC_OK: in case of success.
*/

PtpErr_t PTP_API_Get_NumOfObjects_MPTP( uint32 uiStorageID, uint32 uiParentObjectHandle, uint32 ObjectFormat, uint32 *pnNum,PTPDeviceHandle  PtpDevHndl)
{
    uint32 	uiResult = PTP_RC_OK;
    PTPDevContext *pPtpDevCtxt = NULL;
    PTPInstHndl *pPtpInstHndl=NULL;
    PtpDevInstances_t tDevInst= PtpInvalidDevInst;

    /*Check Parameters*/
    if((NULL == PtpDevHndl)||(NULL == pnNum))
    {
        PTP_DRV_ERR("\nInvalid params\n");
        return PTPAPI_ERROR;
    }

    /*Check for Init*/
    if(False == gPtpInitFlag)
    {
        PTP_DRV_ERR("\nPTP Drv Not Initialized\n");
        return PTPAPI_ERROR;
    }

    pPtpInstHndl = (PTPInstHndl *)PtpDevHndl;

    if(!PTP_CHECKHNDL(pPtpInstHndl->u32Valid))
    {
        PTP_DRV_ERR("\nHandle Not Valid\n");
        return PTPAPI_ERROR;
    }

    /*Masking for Device Instance*/
    tDevInst = (PtpDevInstances_t)(pPtpInstHndl->u32Valid & 0xf);
    /*Lock Device Mutex*/
    PtpDevLock(tDevInst);

    /*Check Device Detection*/
    if(False == gPTPDriverHandle[tDevInst].IsDeviceDetect)
    {
        PTP_DRV_ERR("\nDevice[%d] Not Detected\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }

    /*Check Device Context*/
    if(NULL ==	gPTPDriverHandle[tDevInst].pPTPDevCtxt)
    {
        PTP_DRV_ERR("\nDevice[%d] Context NULL\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }
    /*Check Device Exists or not*/
    if(True != staPtpChkDevExist(gPTPDriverHandle[tDevInst].pPTPDevCtxt))
    {
	PTP_DRV_ERR("\nDevice[%d] Not Exist\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR; 
    }

    /*Device Context*/
    pPtpDevCtxt = gPTPDriverHandle[tDevInst].pPTPDevCtxt;

    *pnNum = 0;

    PTP_DRV_INFO("\nptp_GetNumObjects  start(H:%lu)\n", uiParentObjectHandle);

    uiResult = ptp_GetNumObjects (&(pPtpDevCtxt->params), \
        uiStorageID, ObjectFormat, uiParentObjectHandle, (uint32_t*)pnNum);

    if( uiResult != PTP_RC_OK) 
    {
        PTP_DRV_ERR("\nptp_GetNumObjects return (%lu\n", uiResult);
        PtpDevUnLock(tDevInst);
        return uiResult;
    }

    PTP_DRV_INFO("\nptp_GetNumObjects return(%lu)\n", *pnNum);
    PTP_DRV_INFO("\nptp_GetNumObjects : end(%x)\n\n", uiResult);
    PTP_DRV_INFO("\nSUCCESS\n");
	
    PtpDevUnLock(tDevInst);
    return uiResult;	
}

/**
* @fn	  PTP_API_Get_ObjectInfo_MPTP
* @brief	  Get object info
* @param (in)	 PtpDevHndl
*		   (in) 	uiObjectHandle
*		   (out) 	pPtpObjInfo
* @return	  Error code
*		  PTPAPI_ERROR : in case of error.
*		  PTP_RC_OK : in case of success.
*/

PtpErr_t PTP_API_Get_ObjectInfo_MPTP (uint32 uiObjectHandle, PTPObjectInfo *pPtpObjInfo,PTPDeviceHandle  PtpDevHndl )
{	
    uint16	uiResult = PTP_RC_OK;
    PTPDevContext *pPtpDevCtxt = NULL;


    PTPInstHndl *pPtpInstHndl=NULL;
    PtpDevInstances_t tDevInst= PtpInvalidDevInst;

    /*Check Parameters*/
    if((NULL == PtpDevHndl)||(NULL == pPtpObjInfo))
    {
        PTP_DRV_ERR("\nInvalid params\n");
        return PTPAPI_ERROR;
    }
    /*Check for Init*/
    if(False == gPtpInitFlag)
    {
        PTP_DRV_ERR("\nPTP Drv Not Initialized\n");
        return PTPAPI_ERROR;
    }

    pPtpInstHndl = (PTPInstHndl *)PtpDevHndl;

    if(!PTP_CHECKHNDL(pPtpInstHndl->u32Valid))
    {
        PTP_DRV_ERR("\nHandle Not Valid\n");
        return PTPAPI_ERROR;
    }
    /*Masking for Device Instance*/
    tDevInst = (PtpDevInstances_t)(pPtpInstHndl->u32Valid & 0xf);

    /*Lock Device Mutex*/
    PtpDevLock(tDevInst);

    /*Check Device Detection*/
    if(False == gPTPDriverHandle[tDevInst].IsDeviceDetect)
    {
        PTP_DRV_ERR("\nDevice[%d] Not Detected\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }

    /*Check Device Context*/
    if(NULL ==	gPTPDriverHandle[tDevInst].pPTPDevCtxt)
    {
        PTP_DRV_ERR("\nDevice[%d] Context NULL\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }


    /*Check Device Exists or not*/
    if(True != staPtpChkDevExist(gPTPDriverHandle[tDevInst].pPTPDevCtxt))
    {
	PTP_DRV_ERR("\nDevice[%d] Not Exist\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR; 
    }

    /*Device Context*/
    pPtpDevCtxt = gPTPDriverHandle[tDevInst].pPTPDevCtxt;

#ifndef OPTIMIZED_PTP
    if (!PTP_API_Is_Handle_Vaild_MPTP(uiObjectHandle,PtpDevHndl))
    {
	 PTP_DRV_ERR("\nObject Handle Not Found in Device\n");
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR; //just representing handle error		
    }
#endif


    PtpPrivateInitTransaction (pPtpDevCtxt);

    PTP_DRV_INFO("\nptp_getobjectinfo :start (H:%lu)\n", uiObjectHandle);
    uiResult = ptp_getobjectinfo(&(pPtpDevCtxt->params), \
        uiObjectHandle, pPtpObjInfo );
    if ( uiResult != PTP_RC_OK )
    {
        PTP_DRV_ERR("\nptp_getobjectinfo return(%x)\n", uiResult);
        PtpDevUnLock(tDevInst);
        return uiResult;
    }

    if ((pPtpObjInfo->ObjectFormat != PTP_OFC_Association) && \
        (pPtpObjInfo->ObjectFormat != PTP_OFC_EXIF_JPEG)) 
    {
        PTP_DRV_ERR("\ninvalid object format (%x)\n", pPtpObjInfo->ObjectFormat); 
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR; //just representing format error
    }

    PTP_DRV_INFO("\n0x%lu: % 9i\t%16s\t0x%08x\t0x%04x\t0x%08x\n", uiObjectHandle,	\
        pPtpObjInfo->ObjectCompressedSize, pPtpObjInfo->Filename,			\
        pPtpObjInfo->StorageID, pPtpObjInfo->ObjectFormat, pPtpObjInfo->ParentObject);


    PTP_DRV_INFO("\nptp_getobjectinfo : end(%x)\n\n", uiResult);
    PTP_DRV_INFO("\nSUCCESS\n");
	
    PtpDevUnLock(tDevInst);
    return uiResult;	
}


/**
* @fn	  PTP_API_Get_JpegFilesList_MPTP
* @brief	  Get Jpeg File List
* @param (in)	 PtpDevHndl
*		   (in) 	uiStorageID
*		   (in) 	uiParentObjectHandle
*		   (out) 	pFileInfo
*		   (out) 	pnCount
* @return	  Error code
*		  PTPAPI_ERROR : in case of error.
*		  PTP_RC_OK : in case of success.
*/

PtpErr_t PTP_API_Get_JpegFilesList_MPTP (uint32 uiStorageID, uint32 uiParentObjectHandle, PTPFileListInfo **pFileInfo, uint32 *pnCount,PTPDeviceHandle  PtpDevHndl )
{

    uint32	n = 0;
    uint32	nIndex = 0;
    uint16	uiResult = PTP_RC_OK;
    struct tm* tmPtr = 0;
    struct tm lt;
    PTPObjectInfo		PtpObjInfo;
    PTPObjectHandles	PtpObjHndl, PtpObjHndl2;
    uint32 i = 0;
    uint32 numjpg = 0;
    uint32 numdir = 0;
    PTPDevContext *pPtpDevCtxt = NULL;
    PTPInstHndl *pPtpInstHndl=NULL;
    PtpDevInstances_t tDevInst = PtpInvalidDevInst;
    uint16 len = 0;

    /*Check Parameters*/
    if((NULL == PtpDevHndl)||(NULL == pnCount))
    {
        PTP_DRV_ERR("\nInvalid params\n");
        return PTPAPI_ERROR;

    }

    /*Check for Init*/
    if(False == gPtpInitFlag)
    {
        PTP_DRV_ERR("\nPTP Drv Not Initialized\n");
        return PTPAPI_ERROR;
    }

    pPtpInstHndl = (PTPInstHndl *)PtpDevHndl;

    if(!PTP_CHECKHNDL(pPtpInstHndl->u32Valid))
    {
        PTP_DRV_ERR("\nHandle Not Valid\n");
        return PTPAPI_ERROR;
    }

    /*Masking for Device Instance*/
    tDevInst = (PtpDevInstances_t)(pPtpInstHndl->u32Valid & 0xf);

    /*Lock Device Mutex*/
    PtpDevLock(tDevInst);

    /*Check Device Detection*/
    if(False == gPTPDriverHandle[tDevInst].IsDeviceDetect)
    {
        PTP_DRV_ERR("\nDevice[%d] Not Detected\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }


    /*Check Device Context*/
    if(NULL ==	gPTPDriverHandle[tDevInst].pPTPDevCtxt)
    {
        PTP_DRV_ERR("\nDevice[%d] Context NULL\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }


    /*Check Device Exists or not*/
    if(True != staPtpChkDevExist(gPTPDriverHandle[tDevInst].pPTPDevCtxt))
    {
	 PTP_DRV_ERR("\nDevice[%d] Not Exist\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR; 
    }

    /*Device Context*/
    pPtpDevCtxt = gPTPDriverHandle[tDevInst].pPTPDevCtxt;

    PtpPrivateInitTransaction (pPtpDevCtxt);
    PTP_API_Init_ObjectHandles(&PtpObjHndl);
    PTP_API_Init_ObjectHandles(&PtpObjHndl2);

    PTP_DRV_INFO("\nptp_getobjecthandles : start(H:%lu)\n", uiParentObjectHandle);	
    //Find All list from the uiParentObjectHandle
    uiResult = ptp_getobjecthandles (&(pPtpDevCtxt->params),uiStorageID, \
        0x00000000, uiParentObjectHandle, &(pPtpDevCtxt->params.handles));

    if (uiResult != PTP_RC_OK)
    {
    	 PTP_DRV_ERR("\nptp_getobjecthandles returns(%x)\n",uiResult);
        PtpDevUnLock(tDevInst);
        return uiResult;
    }

    PTP_DRV_INFO("\nThe # of handles : %d\n", pPtpDevCtxt->params.handles.n);
    PTP_DRV_INFO("\nHandler:        size: \tname:\n");
    for (i = 0; i < pPtpDevCtxt->params.handles.n; i++)
    {
        PTP_API_Init_ObjectInfo(&PtpObjInfo);
        uiResult = ptp_getobjectinfo(&(pPtpDevCtxt->params),\
            pPtpDevCtxt->params.handles.Handler[i], &PtpObjInfo);
        if (uiResult != PTP_RC_OK)
        {
           PTP_DRV_ERR("\nptp_getobjectinfo returns(%x)\n",uiResult);
            PTP_API_Clear_ObjectInfo (&PtpObjInfo);
            PtpDevUnLock(tDevInst);
            return uiResult;
        }

        if (PtpObjInfo.ObjectFormat == PTP_OFC_Association)
        {
            numdir++;
            PTP_API_Clear_ObjectInfo (&PtpObjInfo);		
            continue;
        }
        if (PtpObjInfo.ObjectFormat == PTP_OFC_EXIF_JPEG)
        {
            numjpg++;
        }
        PTP_DRV_INFO("\n0x%08x: % 9i\t%s\n",pPtpDevCtxt->params.handles.Handler[i],\
            PtpObjInfo.ObjectCompressedSize, PtpObjInfo.Filename);

        PTP_API_Clear_ObjectInfo (&PtpObjInfo);
    }

    PTPFreeMem((pPtpDevCtxt->params.handles.Handler));		

    PTP_DRV_INFO("\nthe num of dir : %lu \n", numdir);
    PTP_DRV_INFO("\nthe num of jpg : %lu \n", numjpg);


    // SONY DSC T20Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½, PTP_OFC_Association cmdÃ¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½  handle list Ã¯Â¿Â½Ã¯Â¿Â½ÃƒÂ»Ã¯Â¿Â½Ã¯Â¿Â½ deviceÃ¯Â¿Â½Ã¯Â¿Â½ STALLÃ¯Â¿Â½Ã¯Â¿Â½
    // Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½,Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã†Â® Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ NULL Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ ÃƒÂ¼Ã…Â©Ã¯Â¿Â½Ã˜Â¼Ã¯Â¿Â½ handle list Ã¯Â¿Â½Ã¯Â¿Â½ÃƒÂ»
    if(numdir != 0)
    {
        uiResult = ptp_getobjecthandles (&(pPtpDevCtxt->params), uiStorageID, \
            PTP_OFC_Association, uiParentObjectHandle, &(PtpObjHndl));
        if( uiResult != PTP_RC_OK) 
        {
            PTP_DRV_ERR("\nptp_getobjecthandles(folder) return (%x)\n", uiResult);
            PtpDevUnLock(tDevInst);
            return uiResult;
        }
    }

    if(numjpg != 0)
    {
        uiResult = ptp_getobjecthandles (&(pPtpDevCtxt->params), uiStorageID,\
            PTP_OFC_EXIF_JPEG, uiParentObjectHandle, &(PtpObjHndl2));
        if( uiResult != PTP_RC_OK) 
        {
            PTP_DRV_ERR("\nptp_getobjecthandles(file) return (%x)\n", uiResult);
		PTP_API_Clear_ObjectHandles(&PtpObjHndl);
            PtpDevUnLock(tDevInst);
            return uiResult;
        }	
    }

    *pnCount = numdir + numjpg; /*PtpObjHndl.n + PtpObjHndl2.n;*/
    if (*pnCount <= 0)
    {
	 PTP_API_Clear_ObjectHandles(&PtpObjHndl);
	 PTP_API_Clear_ObjectHandles(&PtpObjHndl2);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR; // just representing wrong count error		
    }

    PTP_DRV_INFO("\n PtpObjHndl.n = %d, PtpObjHndl2.n = %d, storageId= %lu, parent handle = %lu\n", PtpObjHndl.n, \
        PtpObjHndl2.n, uiStorageID, uiParentObjectHandle);

    *pFileInfo = (PTPFileListInfo *)calloc (sizeof(PTPFileListInfo), *pnCount);
    if (*pFileInfo == NULL)
    {
         PTP_DRV_ERR("\nmemory alloc error(PTPFileListInfo)\n");
	   PTP_API_Clear_ObjectHandles(&PtpObjHndl);
         PTP_API_Clear_ObjectHandles(&PtpObjHndl2);
         PtpDevUnLock(tDevInst);
         return PTPAPI_ERROR; // just representing memory allocation error				
    }

    n = 0;
    for (nIndex = 0; nIndex < numdir/*PtpObjHndl.n*/; nIndex++) 
    {
        PTP_API_Init_ObjectInfo(&PtpObjInfo);
        uiResult = ptp_getobjectinfo(&(pPtpDevCtxt->params),\
            PtpObjHndl.Handler[nIndex], &PtpObjInfo);
        if ( uiResult != PTP_RC_OK )
        {
            PTP_DRV_WARN("\nptp_getobjectinfo return(%x)\n", uiResult);
            continue;
        }

        if (PtpObjInfo.ObjectFormat != PTP_OFC_Association)
        {
            PTP_DRV_WARN("\nPtpObjInfo.ObjectFormat\n");
            PTP_API_Clear_ObjectInfo (&PtpObjInfo);
            continue;
        }


        // 2007.08.06,  Kyungsik
        // Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ : Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½(*.MOV) Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ folderÃ¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½ÃÂ·Ã¯Â¿Â½Ã¯Â¿Â½ÃÂ¿Ã¯Â¿Â½ ObjectFormatÃ¯Â¿Â½ folderÃ¯Â¿Â½Ã¯Â¿Â½ return 
        // Ã¯Â¿Â½Ã¯Â¿Â½ÃƒÂ¥ : ObjectFormat(folder,0x3001)Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½, objectinfoÃ¯Â¿Â½Ã¯Â¿Â½  AssociationType(folder,0x0001) Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ÃŽÂ¸Ã¯Â¿Â½  ÃƒÂ¼Ã…Â©Ã¯Â¿Â½Ã‘Â´Ã¯Â¿Â½.
        //		Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ AssociationTypeÃ¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½ÃŠÂ´Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã‡Â·Ã¯Â¿Â½, AssociationType(undef, 0x0000)Ã¯Â¿Â½Ã¯Â¿Â½ ÃˆÂ®Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã˜Â¾Ã¯Â¿Â½ Ã¯Â¿Â½Ã‘Â´Ã¯Â¿Â½.
        // Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ :  Panasonic FX12 
        if (PtpObjInfo.AssociationType > PTP_AT_GenericFolder)
        {
            PTP_DRV_WARN("\nThis file is not folder \n");
            PTP_API_Clear_ObjectInfo (&PtpObjInfo);		
            continue;
        }


        len = strlen(PtpObjInfo.Filename);
        strncpy ((*pFileInfo)[n].filename, PtpObjInfo.Filename,len);
        (*pFileInfo)[n].filename[len] = '\0';
        (*pFileInfo)[n].filetype = DIR_TYPE;
        (*pFileInfo)[n].storageId = PtpObjInfo.StorageID;
        (*pFileInfo)[n].ParentObject = PtpObjInfo.ParentObject;
        (*pFileInfo)[n].handle = PtpObjHndl.Handler[nIndex];

        PTP_API_Clear_ObjectInfo (&PtpObjInfo);
        ++n;
    }


    for (nIndex = 0; nIndex < numjpg/*PtpObjHndl2.n*/; nIndex++)
    {
        PTP_API_Init_ObjectInfo(&PtpObjInfo);
        uiResult = ptp_getobjectinfo(&(pPtpDevCtxt->params), PtpObjHndl2.Handler[nIndex],&PtpObjInfo);
        if ( uiResult != PTP_RC_OK )
        {
            PTP_DRV_WARN("\nptp_getobjectinfo return(%x)\n", uiResult);
            continue;
        }

        if (PtpObjInfo.ObjectFormat != PTP_OFC_EXIF_JPEG)
        {
            PTP_API_Clear_ObjectInfo (&PtpObjInfo);
            continue;
        }		

        tmPtr = NULL;
        tmPtr = localtime_r ( (const time_t*)&PtpObjInfo.CaptureDate, &lt);
        if (tmPtr != NULL)
        {
            (*pFileInfo)[n].year = tmPtr->tm_year + 1900;
            (*pFileInfo)[n].month = tmPtr->tm_mon + 1;
            (*pFileInfo)[n].day = tmPtr->tm_mday ;
            (*pFileInfo)[n].hour = tmPtr->tm_hour;
            (*pFileInfo)[n].min = tmPtr->tm_min;
            (*pFileInfo)[n].sec = tmPtr->tm_sec;
        }

        len = strlen(PtpObjInfo.Filename);
        strncpy ((*pFileInfo)[n].filename, PtpObjInfo.Filename,len);
        (*pFileInfo)[n].filename[len] = '\0';

        (*pFileInfo)[n].filetype = FILE_TYPE;
        (*pFileInfo)[n].storageId = PtpObjInfo.StorageID;
        (*pFileInfo)[n].ParentObject = PtpObjInfo.ParentObject;
        (*pFileInfo)[n].handle = PtpObjHndl2.Handler[nIndex];


        PTP_API_Clear_ObjectInfo (&PtpObjInfo);
        ++n;
    }

    // 2007.08.06,  ssu
    // For the error case
    // If any file or folder's association type is not proper, at that time we should count again the real number of files.
    if (*pnCount != n)
    {
        *pnCount = n;
    }

    PTP_API_Clear_ObjectHandles(&PtpObjHndl);
    PTP_API_Clear_ObjectHandles(&PtpObjHndl2);


    PTP_DRV_INFO("\nptp_getobjectinfo : end (%x)\n\n", uiResult);
    PTP_DRV_INFO("\nSUCCESS\n");
	
    PtpDevUnLock(tDevInst);
    return uiResult;	
}

/**
* @fn	  PTP_API_Get_Thumbnail_MPTP
* @brief	  Get Jpeg Thumbnail pImage
* @param (in)	 PtpDevHndl
*		   (in) 	uiHndl
*		   (out) 	pImage
*		   (out) 	pImageSize
* @return	  Error code
*		  PTPAPI_ERROR : in case of error.
*		 PTP_RC_OK: in case of success.
*/

PtpErr_t PTP_API_Get_Thumbnail_MPTP(uint32 uiHndl, char **pImage, uint32 *pImageSize,PTPDeviceHandle  PtpDevHndl)
{

    PTPObjectInfo		PtpObjInfo;
    unsigned long stTick = 0;
    unsigned long edTick = 0;
    uint16	uiResult = PTP_RC_OK;
    PTPDevContext *pPtpDevCtxt;
    PTPInstHndl *pPtpInstHndl=NULL;
    PtpDevInstances_t tDevInst = PtpInvalidDevInst;

    /*Check Parameters*/
    if((NULL == PtpDevHndl)||(NULL == pImageSize)||(NULL == pImage))
    {
        PTP_DRV_ERR("\nInvalid params\n");
        return PTPAPI_ERROR;

    }

    /*Check for Init*/
    if(False == gPtpInitFlag)
    {
        PTP_DRV_ERR("\nPTP Drv Not Initialized\n");
        return PTPAPI_ERROR;
    }

    pPtpInstHndl = (PTPInstHndl *)PtpDevHndl;

    if(!PTP_CHECKHNDL(pPtpInstHndl->u32Valid))
    {
        PTP_DRV_ERR("\nHandle Not Valid\n");
        return PTPAPI_ERROR;
    }

    /*Masking for Device Instance*/
    tDevInst = (PtpDevInstances_t)(pPtpInstHndl->u32Valid & 0xf);

    /*Lock Device Mutex*/
    PtpDevLock(tDevInst);

    /*Check Device Detection*/
    if(False == gPTPDriverHandle[tDevInst].IsDeviceDetect)
    {
        PTP_DRV_ERR("\nDevice[%d] Not Detected\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }


    /*Check Device Context*/
    if(NULL ==	gPTPDriverHandle[tDevInst].pPTPDevCtxt)
    {
        PTP_DRV_ERR("\nDevice[%d] Context NULL\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }


    /*Check Device Exists or not*/
    if(True != staPtpChkDevExist(gPTPDriverHandle[tDevInst].pPTPDevCtxt))
    {
	PTP_DRV_ERR("\nDevice[%d] Not Exist\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR; 
    }

    /*Device Context*/
    pPtpDevCtxt = gPTPDriverHandle[tDevInst].pPTPDevCtxt;

#ifndef OPTIMIZED_PTP
    if (!PTP_API_Is_Handle_Vaild_MPTP(handle,PtpDevHndl))
    {
 	 PTP_DRV_ERR("\nObject Handle Not Found in Device\n");
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR; //just representing handle error		
    }
#endif

    PtpPrivateInitTransaction (pPtpDevCtxt);

    PTP_API_Init_ObjectInfo (&PtpObjInfo);

    PTP_DRV_INFO("\nptp_getobjectinfo : start(H:%lu)\n", uiHndl);
    uiResult = ptp_getobjectinfo(&(pPtpDevCtxt->params), uiHndl, &PtpObjInfo);
    if ( uiResult != PTP_RC_OK )
    {
        PTP_DRV_ERR( "ptp_getobjectinfo return(0x%04x)\n", uiResult);
        PtpDevUnLock(tDevInst);
        return uiResult;
    }

    if (PtpObjInfo.ObjectFormat != PTP_OFC_EXIF_JPEG) 
    {
	PTP_API_Clear_ObjectInfo(&PtpObjInfo);
        PTP_DRV_ERR("\nhandle(%lu) is not jpeg image.\n", uiHndl);
        PtpDevUnLock(tDevInst);
        return uiResult;
    }         

    if (PTP_Debug_Level >=  PTP_DBG_LVL2)
    {
        stTick = Tick() ;
    }

    uiResult = ptp_getthumb(&(pPtpDevCtxt->params), uiHndl, pImage);

    if (PTP_Debug_Level >= PTP_DBG_LVL2)
    {
        edTick = Tick() ;

        PTP_DRV_INFO("\n0x%lu: %9i\t%16s\t0x%08x\t0x%04x\t0x%08x "
            "speed:%lu (ms)\n", uiHndl,	\
            PtpObjInfo.ObjectCompressedSize, PtpObjInfo.Filename, 										\
            PtpObjInfo.StorageID, PtpObjInfo.ObjectFormat, PtpObjInfo.ParentObject, (edTick - stTick));
    }
    else
    {
        PTP_DRV_INFO ("0x%lu: %9i\t%16s\t0x%08x\t0x%04x\t0x%08x\n", uiHndl,		\
            PtpObjInfo.ObjectCompressedSize, PtpObjInfo.Filename, 										\
            PtpObjInfo.StorageID, PtpObjInfo.ObjectFormat, PtpObjInfo.ParentObject);
    }

    if (uiResult != PTP_RC_OK) 
    {
        PTP_DRV_WARN ("ptp_getthumb return(0x%04x)\n", uiResult);
        /* 
        if (uiResult == PTP_ERROR_IO) 
        {
        stausbclearstall(pPtpDevCtxt);
        } 
        */
        //*pImageSize = -1;
        *pImageSize = 0;//Niraj	
    } 
    else 
    {
        *pImageSize = PtpObjInfo.ThumbCompressedSize;
    }

    PTP_API_Clear_ObjectInfo(&PtpObjInfo);


    PTP_DRV_INFO("\nptp_getobjectinfo : end(%x)\n\n", uiResult);
    PTP_DRV_INFO("\nSUCCESS\n");

    PtpDevUnLock(tDevInst);
    return uiResult;
}
/**
* @fn	  PTP_API_Get_JpegImage_MPTP
* @brief	  Get Jpeg pImage
* @param (in)	 PtpDevHndl
*		   (in) 	handle
*		   (out) 	pImage
*		   (out) 	pImageSize
* @return	  Error code
*		 PTPAPI_ERROR : in case of error.
*		 PTP_RC_OK : in case of success.
*/

PtpErr_t PTP_API_Get_JpegImage_MPTP (uint32 uiHndl, char **pImage, uint32 *pImageSize,PTPDeviceHandle  PtpDevHndl )
{
    PTPObjectInfo		PtpObjInfo;
    unsigned long stTick = 0;
    unsigned long edTick = 0;
    uint16	uiResult = PTP_RC_OK;
    PTPDevContext *pPtpDevCtxt = NULL;
    PTPInstHndl *pPtpInstHndl=NULL;
    PtpDevInstances_t tDevInst = PtpInvalidDevInst;

    /*Check Parameters*/
    if((NULL == PtpDevHndl)||(NULL == pImageSize)||(NULL == pImage))
    {
        PTP_DRV_ERR("\nInvalid params\n");
        return PTPAPI_ERROR;
    }

    /*Check for Init*/
    if(False == gPtpInitFlag)
    {
        PTP_DRV_ERR("\nPTP Drv Not Initialized\n");
        return PTPAPI_ERROR;
    }

    pPtpInstHndl = (PTPInstHndl *)PtpDevHndl;
    if(!PTP_CHECKHNDL(pPtpInstHndl->u32Valid))
    {
        PTP_DRV_ERR("\nHandle Not Valid\n");
        return PTPAPI_ERROR;
    }		

    /*Masking for Device Instance*/
    tDevInst = (PtpDevInstances_t)(pPtpInstHndl->u32Valid & 0xf);
    /*Lock Device Mutex*/
    PtpDevLock(tDevInst);

    /*Check Device Detection*/
    if(False == gPTPDriverHandle[tDevInst].IsDeviceDetect)
    {
        PTP_DRV_ERR("\nDevice[%d] Not Detected\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }

    /*Check Device Context*/
    if(NULL ==	gPTPDriverHandle[tDevInst].pPTPDevCtxt)
    {
        PTP_DRV_ERR("\nDevice[%d] Context NULL\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }


    /*Check Device Exists or not*/
    if(True != staPtpChkDevExist(gPTPDriverHandle[tDevInst].pPTPDevCtxt))
    {
	PTP_DRV_ERR("\nDevice[%d] Not Exist\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR; 
    }

    /*Device Context*/
    pPtpDevCtxt = gPTPDriverHandle[tDevInst].pPTPDevCtxt;

#ifndef OPTIMIZED_PTP
    if (!PTP_API_Is_Handle_Vaild_MPTP(uiHndl,PtpDevHndl))
    {
  	 PTP_DRV_ERR("\nObject Handle Not Found in Device\n");
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR; //just representing handle error		
    }
#endif

    PtpPrivateInitTransaction (pPtpDevCtxt);
    PTP_API_Init_ObjectInfo (&PtpObjInfo);

    PTP_DRV_INFO("\nptp_getobjectinfo : start(H:%lu)\n", uiHndl);
    uiResult = ptp_getobjectinfo(&(pPtpDevCtxt->params), uiHndl, &PtpObjInfo);

    if ( uiResult != PTP_RC_OK )
    {
        PTP_DRV_ERR("\nptp_getobjectinfo "
            "return(0x%04x)\n", uiResult);
        PtpDevUnLock(tDevInst);
        return uiResult;
    }

    if (PtpObjInfo.ObjectFormat != PTP_OFC_EXIF_JPEG) 
    {
        PTP_DRV_ERR("\nhandle(%lu) is not"
            "jpeg image.\n", uiHndl);
        PTP_API_Clear_ObjectInfo(&PtpObjInfo);		
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR; // just representing handle error
    }         

    if (PTP_Debug_Level >=  PTP_DBG_LVL2)
    {
        stTick = Tick() ;
    }


    /*Sync Implementation*/		
    uiResult = ptp_getobject(&(pPtpDevCtxt->params),uiHndl, pImage);

    if (PTP_Debug_Level >= PTP_DBG_LVL2)
    {
        edTick = Tick() ;
        PTP_DRV_INFO("\n0x%lu: %9i\t%16s\t0x%08x\t0x%04x\t0x%08x "
            "speed:%lu(ms)\n", uiHndl,	\
            PtpObjInfo.ObjectCompressedSize, PtpObjInfo.Filename, 										\
            PtpObjInfo.StorageID, PtpObjInfo.ObjectFormat, PtpObjInfo.ParentObject, (edTick -stTick)  );
    }
    else
    {
        PTP_DRV_INFO ("0x%lu: %9i\t%16s\t0x%08x\t0x%04x\t0x%08x\n", uiHndl,		\
            PtpObjInfo.ObjectCompressedSize, PtpObjInfo.Filename, 										\
            PtpObjInfo.StorageID, PtpObjInfo.ObjectFormat, PtpObjInfo.ParentObject);
    }

    if (uiResult != PTP_RC_OK) 
    {
        PTP_DRV_INFO("\n[Error] PTP_API_Get_JpegImage : ptp_getobject return(0x%04x)\n", uiResult);
        //*pImageSize = -1;
        *pImageSize=0;//Niraj
    } 
    else 
    {	
        *pImageSize = PtpObjInfo.ObjectCompressedSize;

    }

    PTP_API_Clear_ObjectInfo(&PtpObjInfo);	
    PTP_DRV_INFO("\nptp_getobject : end(%x)\n\n", uiResult);
    PtpDevUnLock(tDevInst);
    return uiResult;	
}

/**
* @fn	  PTP_API_Get_DevPath_MPTP
* @brief	  Get Device Path
* @param (in)	 PtpDevHndl
*		   (out) 	pDevpath
* @return	  Error code
*		  PTPAPI_ERROR : in case of error.
*		  PTP_RC_OK : in case of success.
*/

PtpErr_t PTP_API_Get_DevPath_MPTP(char *pDevpath,PTPDeviceHandle  PtpDevHndl)
{

    uint16 uiResult = PTP_RC_OK;
    char strTemp[PATH_MAX + 1];

    PTPDevContext *pPtpDevCtxt = NULL;
    PTPInstHndl *pPtpInstHndl=NULL;
    PtpDevInstances_t tDevInst = PtpInvalidDevInst;

    /*Check Parameters*/
    if((NULL == PtpDevHndl)||(NULL == pDevpath))
    {
        PTP_DRV_ERR("\nInvalid params\n");
        return PTPAPI_ERROR;
    }

    /*Check for Init*/
    if(False == gPtpInitFlag)
    {
        PTP_DRV_ERR("\nPTP Drv Not Initialized\n");
        return PTPAPI_ERROR;
    }
    pPtpInstHndl = (PTPInstHndl *)PtpDevHndl;
    if(!PTP_CHECKHNDL(pPtpInstHndl->u32Valid))
    {
        PTP_DRV_ERR("\nHandle Not Valid\n");
        return PTPAPI_ERROR;
    }	

    /*Masking for Device Instance*/
    tDevInst = (PtpDevInstances_t)(pPtpInstHndl->u32Valid & 0xf);
    /*Lock Device Mutex*/
    PtpDevLock(tDevInst);

    /*Check Device Detection*/
    if(False == gPTPDriverHandle[tDevInst].IsDeviceDetect)
    {
        PTP_DRV_ERR("\nDevice[%d] Not Detected\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }
    /*Check Device Context*/
    if(NULL ==	gPTPDriverHandle[tDevInst].pPTPDevCtxt)
    {
        PTP_DRV_ERR("\nDevice[%d] Context NULL\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }
    /*Check Device Exists or not*/
    if(True != staPtpChkDevExist(gPTPDriverHandle[tDevInst].pPTPDevCtxt))
    {
	PTP_DRV_ERR("\nDevice[%d] Not Exist\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR; 
    }

    /*Device Context*/
    pPtpDevCtxt = gPTPDriverHandle[tDevInst].pPTPDevCtxt;
    snprintf(strTemp,(PATH_MAX + 1),"/%s/%s",pPtpDevCtxt->dev->dirPath,pPtpDevCtxt->dev->filePath);

    //              strcpy(pDevpath,"/dev/bus/usb");
    //              pDevpath[0]= '\0';
    //                strcat(pDevpath, strTemp);
    strncpy(pDevpath,strTemp,PATH_MAX);
    pDevpath[PATH_MAX]= '\0';
    PTP_DRV_INFO("\ndevice path is %s strTemp is %s\n", pDevpath,strTemp);
    PTP_DRV_INFO("\nSUCCESS\n");

    PtpDevUnLock(tDevInst);
    return uiResult;

}

/**
* @fn	  PTP_API_Get_FilesList_MPTP
* @brief	  Get File List
* @param (in)	 PtpDevHndl
*		   (in)	uiStorageID
*		   (in)	uiParentObjectHandle
*		   (out)   pFileInfo
*		   (out)	pnCount
* @return	  Error code
*		  PTPAPI_ERROR: in case of error.
*		 PTP_RC_OK : in case of success.
*/

PtpErr_t PTP_API_Get_FilesList_MPTP(uint32 uiStorageID, uint32 uiParentObjectHandle, PTPFileListInfo **pFileInfo, uint32 *pnCount,PTPDeviceHandle  PtpDevHndl)
{

    uint32	n = 0;
    uint32	nIndex = 0;
    uint16	uiResult = PTP_RC_OK;
    struct tm* tmPtr = 0;
    struct tm lt;
    PTPObjectInfo		PtpObjInfo;
    PTPObjectHandles	PtpObjHndl, PtpObjHndl2;
    uint32 i = 0;
    uint32 numdir = 0;	
    uint32 numjpg = 0;
    unsigned long stTick = 0;
    unsigned long edTick = 0;
    PTPDevContext *pPtpDevCtxt;
    PTPInstHndl *pPtpInstHndl=NULL;
    PtpDevInstances_t tDevInst = PtpInvalidDevInst;
    uint16 len =0;

    /*Check Parameters*/
    if((NULL == PtpDevHndl)||(NULL == pnCount)||(NULL == pFileInfo))
    {
        PTP_DRV_ERR("\nInvalid params\n");
        return PTPAPI_ERROR;
    }

    /*Check for Init*/
    if(False == gPtpInitFlag)
    {
        PTP_DRV_ERR("\nPTP Drv Not Initialized\n");
        return PTPAPI_ERROR;
    }

    pPtpInstHndl = (PTPInstHndl *)PtpDevHndl;
    if(!PTP_CHECKHNDL(pPtpInstHndl->u32Valid))
    {
        PTP_DRV_ERR("\nHandle Not Valid\n");
        return PTPAPI_ERROR;
    }

    /*Masking for Device Instance*/
    tDevInst = (PtpDevInstances_t)(pPtpInstHndl->u32Valid & 0xf);

    /*Lock Device Mutex*/
    PtpDevLock(tDevInst);


    /*Check Device Detection*/
    if(False == gPTPDriverHandle[tDevInst].IsDeviceDetect)
    {
        PTP_DRV_ERR("\nDevice[%d] Not Detected\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }

    /*Check Device Context*/
    if(NULL ==	gPTPDriverHandle[tDevInst].pPTPDevCtxt)
    {
        PTP_DRV_ERR("\nDevice[%d] Context NULL\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }


    /*Check Device Exists or not*/
    if(True != staPtpChkDevExist(gPTPDriverHandle[tDevInst].pPTPDevCtxt))
    {
	PTP_DRV_ERR("\nDevice[%d] Not Exist\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR; 
    }

    /*Device Context*/
    pPtpDevCtxt = gPTPDriverHandle[tDevInst].pPTPDevCtxt;

    PtpPrivateInitTransaction (pPtpDevCtxt);

    PTP_API_Init_ObjectHandles(&PtpObjHndl);
    PTP_API_Init_ObjectHandles(&PtpObjHndl2);

    PTP_DRV_INFO("\nptp_getobjecthandles : start(H:%lu)\n", uiParentObjectHandle);
    stTick = Tick() ;
    //Find All list from the uiParentObjectHandle
    uiResult = ptp_getobjecthandles (&(pPtpDevCtxt->params),uiStorageID, 0x00000000, \
        uiParentObjectHandle, &(pPtpDevCtxt->params.handles));
    if (uiResult != PTP_RC_OK)
    {
    	 PTP_DRV_ERR("\nptp_getobjecthandles:return(%x)\n",uiResult);
        PtpDevUnLock(tDevInst);
        return uiResult;
    }

    PTP_DRV_INFO("\nThe # of handles : %d\n", pPtpDevCtxt->params.handles.n);
    PTP_DRV_INFO("\nHandler:        size: \tname:\n");
    for (i = 0; i < pPtpDevCtxt->params.handles.n; i++) 
    {
        PTP_API_Init_ObjectInfo(&PtpObjInfo);
        uiResult = ptp_getobjectinfo(&(pPtpDevCtxt->params),\
            pPtpDevCtxt->params.handles.Handler[i], &PtpObjInfo);
        if (uiResult != PTP_RC_OK)
        {
		PTP_DRV_ERR("\nptp_getobjectinfo:return(%x)\n",uiResult);
            PTPFreeMem((pPtpDevCtxt->params.handles.Handler));
            PtpDevUnLock(tDevInst);
            return uiResult;
        }

        if (PtpObjInfo.ObjectFormat == PTP_OFC_Association)
        {
            numdir++;
            PTP_API_Clear_ObjectInfo (&PtpObjInfo);
            continue;
        }
        if (PtpObjInfo.ObjectFormat == PTP_OFC_EXIF_JPEG)
        {
            numjpg++;
        }
        PTP_DRV_INFO("\n0x%08x: % 9i\t%s\n",pPtpDevCtxt->params.handles.Handler[i],
            PtpObjInfo.ObjectCompressedSize, PtpObjInfo.Filename);

        PTP_API_Clear_ObjectInfo (&PtpObjInfo);
    }

    PTPFreeMem((pPtpDevCtxt->params.handles.Handler));

    PTP_DRV_INFO("\nthe num of dir : %lu \n", numdir);
    PTP_DRV_INFO("\nthe num of jpg : %lu \n", numjpg);

    // SONY DSC T20Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½, PTP_OFC_Association cmdÃ¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½  handle list Ã¯Â¿Â½Ã¯Â¿Â½ÃƒÂ»Ã¯Â¿Â½Ã¯Â¿Â½ deviceÃ¯Â¿Â½Ã¯Â¿Â½ STALLÃ¯Â¿Â½Ã¯Â¿Â½
    // Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½,Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã†Â® Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ NULL Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ ÃƒÂ¼Ã…Â©Ã¯Â¿Â½Ã˜Â¼Ã¯Â¿Â½ handle list Ã¯Â¿Â½Ã¯Â¿Â½ÃƒÂ»
    if(numdir != 0)
    {
        uiResult = ptp_getobjecthandles (&(pPtpDevCtxt->params),\
            uiStorageID, PTP_OFC_Association, uiParentObjectHandle, &(PtpObjHndl));
        if( uiResult != PTP_RC_OK) 
        {
            PTP_DRV_ERR("\nptp_getobjecthandles(folder) return (%x)\n", uiResult);
            PTP_API_Clear_ObjectHandles(&PtpObjHndl);
            PtpDevUnLock(tDevInst);
            return uiResult;
        }	
    }

    if(numjpg != 0)
    {
        uiResult = ptp_getobjecthandles (&(pPtpDevCtxt->params), uiStorageID, \
            PTP_OFC_EXIF_JPEG, uiParentObjectHandle, &(PtpObjHndl2));
        if( uiResult != PTP_RC_OK) 
        {
            PTP_DRV_ERR("\nptp_getobjecthandles(file) return (%x)\n", uiResult);
            PTP_API_Clear_ObjectHandles(&PtpObjHndl);
            PTP_API_Clear_ObjectHandles(&PtpObjHndl2);
            PtpDevUnLock(tDevInst);
            return uiResult;		
        }	
    }


    *pnCount = numdir + numjpg;/*PtpObjHndl.n + PtpObjHndl2.n;*/
    if (*pnCount <= 0)
    {
        PTP_DRV_ERR("\nPtpObjHndl.n + PtpObjHndl2.n is less than 0\n");
        PTP_API_Clear_ObjectHandles(&PtpObjHndl);
        PTP_API_Clear_ObjectHandles(&PtpObjHndl2);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR; // just representing wrong count error		
    }

    *pFileInfo = (PTPFileListInfo *)calloc (sizeof(PTPFileListInfo), *pnCount);
    if (*pFileInfo == NULL)
    {
        PTP_DRV_ERR("\nmemory alloc error(PTPFileListInfo)\n");
        PTP_API_Clear_ObjectHandles(&PtpObjHndl);
        PTP_API_Clear_ObjectHandles(&PtpObjHndl2);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR; // just representing memory allocation error		
    }


    n = 0;
    for (nIndex = 0; nIndex <numdir /*PtpObjHndl.n*/; nIndex++) 
    {
        PTP_API_Init_ObjectInfo(&PtpObjInfo);
        uiResult = ptp_getobjectinfo(&(pPtpDevCtxt->params),\
            PtpObjHndl.Handler[nIndex], &PtpObjInfo);
        if ( uiResult != PTP_RC_OK )
        {
            PTP_DRV_WARN("\nptp_getobjectinfo return(%x)\n", uiResult);
            continue;
        }

        if (PtpObjInfo.ObjectFormat != PTP_OFC_Association) 
        {
            PTP_API_Clear_ObjectInfo (&PtpObjInfo);
            continue;
        }

        // 2007.08.06,  Kyungsik
        // Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ : Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½(*.MOV) Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ folderÃ¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½ÃÂ·Ã¯Â¿Â½Ã¯Â¿Â½ÃÂ¿Ã¯Â¿Â½ ObjectFormatÃ¯Â¿Â½ folderÃ¯Â¿Â½Ã¯Â¿Â½ return 
        // Ã¯Â¿Â½Ã¯Â¿Â½ÃƒÂ¥ : ObjectFormat(folder,0x3001)Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½, objectinfoÃ¯Â¿Â½Ã¯Â¿Â½  AssociationType(folder,0x0001) Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ÃŽÂ¸Ã¯Â¿Â½  ÃƒÂ¼Ã…Â©Ã¯Â¿Â½Ã‘Â´Ã¯Â¿Â½.
        //		Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ AssociationTypeÃ¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½ÃŠÂ´Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã‡Â·Ã¯Â¿Â½, AssociationType(undef, 0x0000)Ã¯Â¿Â½Ã¯Â¿Â½ ÃˆÂ®Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã˜Â¾Ã¯Â¿Â½ Ã¯Â¿Â½Ã‘Â´Ã¯Â¿Â½.
        // Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ :  Panasonic FX12 
        if (PtpObjInfo.AssociationType > PTP_AT_GenericFolder)
        {
            PTP_DRV_WARN("\nThis file is not folder \n");
            PTP_API_Clear_ObjectInfo (&PtpObjInfo);		
            continue;
        }
        len = strlen(PtpObjInfo.Filename);
        strncpy ((*pFileInfo)[n].filename, PtpObjInfo.Filename,len);
        (*pFileInfo)[n].filename[len] = '\0';

        (*pFileInfo)[n].filetype = DIR_TYPE;
        (*pFileInfo)[n].storageId = PtpObjInfo.StorageID;
        (*pFileInfo)[n].ParentObject = PtpObjInfo.ParentObject;
        (*pFileInfo)[n].handle = PtpObjHndl.Handler[nIndex];

        PTP_API_Clear_ObjectInfo (&PtpObjInfo);
        ++n;
    }


    for (nIndex = 0; nIndex < numjpg/*PtpObjHndl2.n*/; nIndex++)
    {
        PTP_API_Init_ObjectInfo(&PtpObjInfo);
        uiResult = ptp_getobjectinfo(&(pPtpDevCtxt->params), PtpObjHndl2.Handler[nIndex],&PtpObjInfo);
        if ( uiResult != PTP_RC_OK )
        {
            PTP_DRV_WARN("\nptp_getobjectinfo return(%x)\n", uiResult);
            continue;
        }

        if (PtpObjInfo.ObjectFormat != PTP_OFC_EXIF_JPEG)
        {
            PTP_API_Clear_ObjectInfo (&PtpObjInfo);		
            continue;
        }

        tmPtr = NULL;
        tmPtr = localtime_r ( (const time_t*)&PtpObjInfo.CaptureDate, &lt);
        if (tmPtr != NULL)
        {
            (*pFileInfo)[n].year = tmPtr->tm_year + 1900;
            (*pFileInfo)[n].month = tmPtr->tm_mon + 1;
            (*pFileInfo)[n].day = tmPtr->tm_mday ;
            (*pFileInfo)[n].hour = tmPtr->tm_hour;
            (*pFileInfo)[n].min = tmPtr->tm_min;
            (*pFileInfo)[n].sec = tmPtr->tm_sec;
        }
        len = strlen(PtpObjInfo.Filename);
        strncpy ((*pFileInfo)[n].filename, PtpObjInfo.Filename,len);
        (*pFileInfo)[n].filename[len] = '\0';

        (*pFileInfo)[n].filetype = FILE_TYPE;
        (*pFileInfo)[n].storageId = PtpObjInfo.StorageID;
        (*pFileInfo)[n].ParentObject = PtpObjInfo.ParentObject;
        (*pFileInfo)[n].handle = PtpObjHndl2.Handler[nIndex];

        PTP_API_Clear_ObjectInfo (&PtpObjInfo);
        ++n;
    }

    // 2007.08.06,  ssu
    // For the error case
    // If any file or folder's association type is not proper, at that time we should count again the real number of files.
    if (*pnCount != n)
    {
        *pnCount = n;
    }

    edTick = Tick() ;
    PTP_DRV_INFO("\nPTP_API_Get_FilesList : PtpObjHndl.n = %d,"
        "PtpObjHndl2.n = %d, storageId= %lu, parent handle = %lu, speed = %lu(ms)\n", 
        PtpObjHndl.n, PtpObjHndl2.n, uiStorageID, uiParentObjectHandle, (edTick -stTick ));

    PTP_API_Clear_ObjectHandles(&PtpObjHndl);
    PTP_API_Clear_ObjectHandles(&PtpObjHndl2);

    PTP_DRV_INFO("\nptp_getobjectinfo : end (%x)\n\n", uiResult);
    PTP_DRV_INFO("\nSUCCESS\n");
	 
    PtpDevUnLock(tDevInst);
    return uiResult;	
}
/**
* @fn	  PTP_API_Get_JpegImageInfo_MPTP
* @brief	  Get Jpeg pImage Info
* @param (in)	 PtpDevHndl
*		   (in)	uiHndl
*		   (out)	PtpObjInfo
* @return	  Error code
*		 PTPAPI_ERROR: in case of error.
*		  PTP_RC_OK : in case of success.
*/

PtpErr_t PTP_API_Get_JpegImageInfo_MPTP (uint32 uiHndl, PTPObjectInfo *pPtpObjInfo,PTPDeviceHandle  PtpDevHndl)
{
    uint16	uiResult = PTP_RC_OK;
    PTPDevContext *pPtpDevCtxt = NULL;
    PTPInstHndl *pPtpInstHndl=NULL;
    PtpDevInstances_t tDevInst = PtpInvalidDevInst;
    /*Check Parameters*/
    if((NULL == PtpDevHndl)||(NULL == pPtpObjInfo))
    {
        PTP_DRV_ERR("\nInvalid params\n");
        return PTPAPI_ERROR;
    }

    /*Check for Init*/
    if(False == gPtpInitFlag)
    {
        PTP_DRV_ERR("\nPTP Drv Not Initialized\n");
        return PTPAPI_ERROR;
    }
    pPtpInstHndl = (PTPInstHndl *)PtpDevHndl;

    if(!PTP_CHECKHNDL(pPtpInstHndl->u32Valid))
    {
        PTP_DRV_ERR("\nHandle Not Valid\n");
        return PTPAPI_ERROR;
    }

    /*Masking for Device Instance*/
    tDevInst = (PtpDevInstances_t)(pPtpInstHndl->u32Valid & 0xf);

    /*Lock Device Mutex*/
    PtpDevLock(tDevInst);


    /*Check Device Detection*/
    if(False == gPTPDriverHandle[tDevInst].IsDeviceDetect)
    {
        PTP_DRV_ERR("\nDevice[%d] Not Detected\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }

    /*Check Device Context*/
    if(NULL ==	gPTPDriverHandle[tDevInst].pPTPDevCtxt)
    {
        PTP_DRV_ERR("\nDevice[%d] Context NULL\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }


    /*Check Device Exists or not*/
    if(True != staPtpChkDevExist(gPTPDriverHandle[tDevInst].pPTPDevCtxt))
    {
	PTP_DRV_ERR("\nDevice[%d] Not Exist\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR; 
    }

    /*Device Context*/
    pPtpDevCtxt = gPTPDriverHandle[tDevInst].pPTPDevCtxt;

    PtpPrivateInitTransaction (pPtpDevCtxt);

    PTP_DRV_INFO("\nptp_getobjectinfo : start(H:%lu)\n", uiHndl);

    uiResult = ptp_getobjectinfo(&(pPtpDevCtxt->params), uiHndl, pPtpObjInfo); //modified by Niraj to get object info directly in PtpObjInfo
    if ( uiResult != PTP_RC_OK )
    {
        PTP_DRV_ERR("\nptp_getobjectinfo return(%x)\n", uiResult);
        PtpDevUnLock(tDevInst);
        return uiResult;
    }
    if (pPtpObjInfo->ObjectFormat != PTP_OFC_EXIF_JPEG) //modified by Niraj
    {
        PTP_DRV_ERR("\nhandle(%lu) is not jpeg image.\n", uiHndl);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR; //just representing handle error
    }         

    //added by Niraj to allocate space for objectinfo which is NULL before this point
    if(pPtpDevCtxt->params.objectinfo==NULL)
    {
        pPtpDevCtxt->params.objectinfo=(PTPObjectInfo*)malloc(sizeof(PTPObjectInfo));//vishal g++ typecasting
        if(pPtpDevCtxt->params.objectinfo==NULL)
        {
            PTP_DRV_ERR("\nMemory Allocation Error");
            PtpDevUnLock(tDevInst);
            return PTPAPI_ERROR;
        }

        memset(pPtpDevCtxt->params.objectinfo,0,sizeof(PTPObjectInfo));
    }

    ptp_objectinfo_copy(pPtpDevCtxt->params.objectinfo,pPtpObjInfo);
    PTP_DRV_INFO("\n0x%lu: % 9i\t%16s\t0x%08x\t0x%04x\t0x%08x\n",uiHndl,         \
        pPtpDevCtxt->params.objectinfo->ObjectCompressedSize,\
        pPtpDevCtxt->params.objectinfo->Filename,                                   \
        pPtpDevCtxt->params.objectinfo->StorageID, pPtpDevCtxt->params.objectinfo->ObjectFormat, \
        pPtpDevCtxt->params.objectinfo->ParentObject);


    // ÀÌ ÇÔ¼ö¸¦ È£ÃâÇÑ ÂÊ¿¡¼­ objectinfoÀÇ ¸Þ¸ð¸®¸¦ ÇØÁ¦ÇØÁÖ¾î¾ß ÇÑ´Ù.	


    PTP_DRV_INFO("\nptp_getobjectinfo : end(%x)\n\n", uiResult);
    PTP_DRV_INFO("\nSUCCESS\n");
    PtpDevUnLock(tDevInst);
    return uiResult;	
}

/**
* @fn	  PTP_API_Get_MP3FilesList_MPTP
* @brief	  Get MP3 File List
* @param (in)	 PtpDevHndl
*		   (in)	uiStorageID
*		   (in)	uiParentObjectHandle
*		   (out)   pFileInfo
*		   (out)	pnCount
* @return	  Error code
*		  PTPAPI_ERROR: in case of error.
*		  PTP_RC_OK : in case of success.
*/

PtpErr_t PTP_API_Get_MP3FilesList_MPTP (uint32 uiStorageID, uint32 uiParentObjectHandle, PTPFileListInfo **pFileInfo, uint32 *pnCount,PTPDeviceHandle  PtpDevHndl)
{
    uint32	n = 0;
    uint32	nIndex = 0;
    uint16	uiResult = PTP_RC_OK;
    struct tm* tmPtr = 0;
    struct tm lt;
    PTPObjectInfo		PtpObjInfo;
    PTPObjectHandles	PtpObjHndl, PtpObjHndl2;
    uint32 i = 0;
    uint32 nummp3 = 0;
    uint32 numdir = 0;
    PTPDevContext *pPtpDevCtxt = NULL;
    PTPInstHndl *pPtpInstHndl=NULL;
    PtpDevInstances_t tDevInst = PtpInvalidDevInst;
    uint16 len = 0;

    /*Check Parameters*/
    if((NULL == PtpDevHndl)||(NULL == pFileInfo)||(NULL == pnCount))
    {
        PTP_DRV_ERR("\nInvalid params\n");
        return PTPAPI_ERROR;

    }

    /*Check for Init*/
    if(False == gPtpInitFlag)
    {
        PTP_DRV_ERR("\nPTP Drv Not Initialized\n");
        return PTPAPI_ERROR;
    }

    pPtpInstHndl = (PTPInstHndl *)PtpDevHndl;

    if(!PTP_CHECKHNDL(pPtpInstHndl->u32Valid))
    {
        PTP_DRV_ERR("\nHandle Not Valid\n");
        return PTPAPI_ERROR;
    }

    /*Masking for Device Instance*/
    tDevInst = (PtpDevInstances_t)(pPtpInstHndl->u32Valid & 0xf);


    /*Lock Device Mutex*/
    PtpDevLock(tDevInst);

    /*Check Device Detection*/
    if(False == gPTPDriverHandle[tDevInst].IsDeviceDetect)
    {
        PTP_DRV_ERR("\nDevice[%d] Not Detected\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }

    /*Check Device Context*/
    if(NULL ==	gPTPDriverHandle[tDevInst].pPTPDevCtxt)
    {
        PTP_DRV_ERR("\nDevice[%d] Context NULL\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }


    /*Check Device Exists or not*/
    if(True != staPtpChkDevExist(gPTPDriverHandle[tDevInst].pPTPDevCtxt))
    {
	PTP_DRV_ERR("\nDevice[%d] Not Exist\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR; 
    }

    /*Device Context*/
    pPtpDevCtxt = gPTPDriverHandle[tDevInst].pPTPDevCtxt;

    PtpPrivateInitTransaction (pPtpDevCtxt);
    PTP_API_Init_ObjectHandles(&PtpObjHndl);
    PTP_API_Init_ObjectHandles(&PtpObjHndl2);
    PTP_DRV_INFO("\nptp_getobjecthandles : start(H:%lu)\n",\
        uiParentObjectHandle);

    //Find All list from the uiParentObjectHandle
    uiResult = ptp_getobjecthandles (&(pPtpDevCtxt->params),uiStorageID,\
        0x00000000, uiParentObjectHandle, &(pPtpDevCtxt->params.handles));
    if (uiResult != PTP_RC_OK)
    {
       PTP_DRV_ERR("\nptp_getobjecthandles returns (%x)\n",uiResult);
        PtpDevUnLock(tDevInst);
        return uiResult;
    }

    PTP_DRV_INFO("\nThe # of handles : %d\n", pPtpDevCtxt->params.handles.n);
    PTP_DRV_INFO("\nHandler:        size: \tname:\n");
    for (i = 0; i < pPtpDevCtxt->params.handles.n; i++) 
    {
        PTP_API_Init_ObjectInfo(&PtpObjInfo);
        uiResult = ptp_getobjectinfo(&(pPtpDevCtxt->params),\
            pPtpDevCtxt->params.handles.Handler[i], &PtpObjInfo);
        if (uiResult != PTP_RC_OK)
        {
            PtpDevUnLock(tDevInst);
            return uiResult;
        }

        if (PtpObjInfo.ObjectFormat == PTP_OFC_Association)
        {
            PTP_API_Clear_ObjectInfo (&PtpObjInfo);
            numdir++;
            continue;
        }
        if (PtpObjInfo.ObjectFormat == PTP_OFC_MP3)
        {    
            nummp3++;
        }
        PTP_DRV_INFO("\n0x%08x: % 9i\t%s\n",pPtpDevCtxt->params.handles.Handler[i],
            PtpObjInfo.ObjectCompressedSize, PtpObjInfo.Filename);

        /*Memory Leak Check*/
        PTP_API_Clear_ObjectInfo (&PtpObjInfo);
    }


    PTPFreeMem((pPtpDevCtxt->params.handles.Handler));


    PTP_DRV_INFO("\nthe num of dir : %lu \n", numdir);
    PTP_DRV_INFO("\nthe num of mp3 : %lu \n", nummp3);	


    // SONY DSC T20Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½, PTP_OFC_Association cmdÃ¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½  handle list Ã¯Â¿Â½Ã¯Â¿Â½ÃƒÂ»Ã¯Â¿Â½Ã¯Â¿Â½ deviceÃ¯Â¿Â½Ã¯Â¿Â½ STALLÃ¯Â¿Â½Ã¯Â¿Â½
    // Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½,Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã†Â® Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ NULL Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ ÃƒÂ¼Ã…Â©Ã¯Â¿Â½Ã˜Â¼Ã¯Â¿Â½ handle list Ã¯Â¿Â½Ã¯Â¿Â½ÃƒÂ»
    if(numdir != 0)
    {
        uiResult = ptp_getobjecthandles (&(pPtpDevCtxt->params), \
            uiStorageID, PTP_OFC_Association, uiParentObjectHandle, &(PtpObjHndl));
        if( uiResult != PTP_RC_OK) 
        {
            PTP_DRV_ERR("\nptp_getobjecthandles(folder) return (%x)\n", uiResult);
            PtpDevUnLock(tDevInst);
            return uiResult;
        }	
    }


    if(nummp3 != 0)
    {
        uiResult = ptp_getobjecthandles (&(pPtpDevCtxt->params), uiStorageID,\
            PTP_OFC_MP3, uiParentObjectHandle, &(PtpObjHndl2));
        if( uiResult != PTP_RC_OK) 
        {
	      PTP_API_Clear_ObjectHandles(&PtpObjHndl);
            PTP_DRV_ERR("\nptp_getobjecthandles(file) return (%x)\n", uiResult);
            PtpDevUnLock(tDevInst);
            return uiResult;
        }	
    }

    *pnCount = numdir + nummp3; /*PtpObjHndl.n + PtpObjHndl2.n*/

    /*Mp3 Bug Resolved*/
    if ((*pnCount <= 0)||(nummp3 <=0))
    {
	 PTP_API_Clear_ObjectHandles(&PtpObjHndl);
	 PTP_API_Clear_ObjectHandles(&PtpObjHndl2);
        PTP_DRV_ERR("\nPtpObjHndl.n + PtpObjHndl2.n is less than equal to 0 \n");
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR; // just representing wrong count error				
    }

    PTP_DRV_INFO (" PtpObjHndl.n = %d, PtpObjHndl2.n = %d, storageId= %lu, parent handle = %lu\n", PtpObjHndl.n, \
        PtpObjHndl2.n, uiStorageID, uiParentObjectHandle);

    *pFileInfo = (PTPFileListInfo *)calloc (sizeof(PTPFileListInfo), *pnCount);
    if (*pFileInfo == NULL)
    {
        PTP_DRV_ERR( "  memory alloc error(PTPFileListInfo)\n");
        PTP_API_Clear_ObjectHandles(&PtpObjHndl);
	  PTP_API_Clear_ObjectHandles(&PtpObjHndl2);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR; // just representing memory allocation error				
    }

    n = 0;
    for (nIndex = 0; nIndex < numdir/*PtpObjHndl.n*/; nIndex++) 
    {
        PTP_API_Init_ObjectInfo(&PtpObjInfo);
        uiResult = ptp_getobjectinfo(&(pPtpDevCtxt->params),\
            PtpObjHndl.Handler[nIndex], &PtpObjInfo);
        if ( uiResult != PTP_RC_OK )
        {
            PTP_DRV_WARN("\nptp_getobjectinfo return(%x)\n", uiResult);
            continue;
        }

        if (PtpObjInfo.ObjectFormat != PTP_OFC_Association) 
        {
            PTP_API_Clear_ObjectInfo (&PtpObjInfo);
            continue;
        }

        // 2007.08.06,  Kyungsik
        // Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ : Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½(*.MOV) Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ folderÃ¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½ÃÂ·Ã¯Â¿Â½Ã¯Â¿Â½ÃÂ¿Ã¯Â¿Â½ ObjectFormatÃ¯Â¿Â½ folderÃ¯Â¿Â½Ã¯Â¿Â½ return 
        // Ã¯Â¿Â½Ã¯Â¿Â½ÃƒÂ¥ : ObjectFormat(folder,0x3001)Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½, objectinfoÃ¯Â¿Â½Ã¯Â¿Â½  AssociationType(folder,0x0001) Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ÃŽÂ¸Ã¯Â¿Â½  ÃƒÂ¼Ã…Â©Ã¯Â¿Â½Ã‘Â´Ã¯Â¿Â½.
        //		Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ AssociationTypeÃ¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½ÃŠÂ´Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã‡Â·Ã¯Â¿Â½, AssociationType(undef, 0x0000)Ã¯Â¿Â½Ã¯Â¿Â½ ÃˆÂ®Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã˜Â¾Ã¯Â¿Â½ Ã¯Â¿Â½Ã‘Â´Ã¯Â¿Â½.
        // Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ :  Panasonic FX12 
        if (PtpObjInfo.AssociationType > PTP_AT_GenericFolder)
        {
            PTP_DRV_WARN("\nThis file is not folder \n");
            PTP_API_Clear_ObjectInfo (&PtpObjInfo);	
            continue;
        }
		
        len = strlen(PtpObjInfo.Filename);
        strncpy ((*pFileInfo)[n].filename, PtpObjInfo.Filename,len);
        (*pFileInfo)[n].filename[len] = '\0';

        (*pFileInfo)[n].filetype = DIR_TYPE;
        (*pFileInfo)[n].storageId = PtpObjInfo.StorageID;
        (*pFileInfo)[n].ParentObject = PtpObjInfo.ParentObject;
        (*pFileInfo)[n].handle = PtpObjHndl.Handler[nIndex];

        PTP_API_Clear_ObjectInfo (&PtpObjInfo);
        ++n;
    }

    for (nIndex = 0; nIndex < nummp3/*PtpObjHndl2.n*/; nIndex++)
    {
        PTP_API_Init_ObjectInfo(&PtpObjInfo);
        uiResult = ptp_getobjectinfo(&(pPtpDevCtxt->params), PtpObjHndl2.Handler[nIndex],&PtpObjInfo);
        if ( uiResult != PTP_RC_OK )
        {
            PTP_DRV_WARN("\nptp_getobjectinfo return(%x)\n", uiResult);
            continue;
        }

        if (PtpObjInfo.ObjectFormat != PTP_OFC_MP3) 
        {
            PTP_API_Clear_ObjectInfo (&PtpObjInfo);
            continue;
        }

        tmPtr = NULL;
        tmPtr = localtime_r ( (const time_t*)&PtpObjInfo.CaptureDate, &lt);
        if (tmPtr != NULL)
        {
            (*pFileInfo)[n].year = tmPtr->tm_year + 1900;
            (*pFileInfo)[n].month = tmPtr->tm_mon + 1;
            (*pFileInfo)[n].day = tmPtr->tm_mday ;
            (*pFileInfo)[n].hour = tmPtr->tm_hour;
            (*pFileInfo)[n].min = tmPtr->tm_min;
            (*pFileInfo)[n].sec = tmPtr->tm_sec;
        }

        len = strlen(PtpObjInfo.Filename);
        strncpy ((*pFileInfo)[n].filename, PtpObjInfo.Filename,len);
        (*pFileInfo)[n].filename[len] = '\0';

        (*pFileInfo)[n].filetype = FILE_TYPE;
        (*pFileInfo)[n].storageId = PtpObjInfo.StorageID;
        (*pFileInfo)[n].ParentObject = PtpObjInfo.ParentObject;
        (*pFileInfo)[n].handle = PtpObjHndl2.Handler[nIndex];

        PTP_API_Clear_ObjectInfo (&PtpObjInfo);
        ++n;
    }

    // 2007.08.06,  ssu
    // For the error case
    // If any file or folder's association type is not proper, at that time we should count again the real number of files.
    if (*pnCount != n)
    {
        *pnCount = n;
    }

    PTP_API_Clear_ObjectHandles(&PtpObjHndl);
    PTP_API_Clear_ObjectHandles(&PtpObjHndl2);


    PTP_DRV_INFO("\nptp_getobjectinfo : end (%x)\n\n", uiResult);
    PTP_DRV_INFO("\nSUCCESS\n");
	
    PtpDevUnLock(tDevInst);
    return uiResult;	
}

/**
* @fn	  PTP_API_Get_MP3File_MPTP
* @brief	  Get MP3 Files 
* @param (in)	 PtpDevHndl
*		   (in)	uiHndl
*		   (out)   pImage
*		   (out)	pImageSize
* @return	  Error code
*		  PTPAPI_ERROR : in case of error.
*		  PTP_RC_OK : in case of success.
* @see 	   
*/

PtpErr_t PTP_API_Get_MP3File_MPTP(uint32 uiHndl, char **pImage, uint32 *pImageSize,PTPDeviceHandle  PtpDevHndl)
{
    PTPObjectInfo		PtpObjInfo;
    uint16	uiResult = PTP_RC_OK;
    PTPDevContext *pPtpDevCtxt = NULL;
    PTPInstHndl *pPtpInstHndl=NULL;
    PtpDevInstances_t tDevInst = PtpInvalidDevInst;

    /*Check Parameters*/
    if((NULL == PtpDevHndl)||(NULL == pImageSize)||(NULL == pImage))
    {
        PTP_DRV_ERR("\nInvalid params\n");
        return PTPAPI_ERROR;

    }

    /*Check for Init*/
    if(False == gPtpInitFlag)
    {
        PTP_DRV_ERR("\nPTP Drv Not Initialized\n");
        return PTPAPI_ERROR;
    }
    pPtpInstHndl = (PTPInstHndl *)PtpDevHndl;
    if(!PTP_CHECKHNDL(pPtpInstHndl->u32Valid))
    {
        PTP_DRV_ERR("\nHandle Not Valid\n");
        return PTPAPI_ERROR;
    }

    /*Masking for Device Instance*/
    tDevInst = (PtpDevInstances_t)(pPtpInstHndl->u32Valid & 0xf);
    /*Lock Device Mutex*/
    PtpDevLock(tDevInst);
    /*Check Device Detection*/
    if(False == gPTPDriverHandle[tDevInst].IsDeviceDetect)
    {
        PTP_DRV_ERR("\nDevice[%d] Not Detected\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }

    /*Check Device Context*/
    if(NULL ==	gPTPDriverHandle[tDevInst].pPTPDevCtxt)
    {
        PTP_DRV_ERR("\nDevice[%d] Context NULL\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }


    /*Check Device Exists or not*/
    if(True != staPtpChkDevExist(gPTPDriverHandle[tDevInst].pPTPDevCtxt))
    {
	PTP_DRV_ERR("\nDevice[%d] Not Exist\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR; 
    }

    /*Device Context*/
    pPtpDevCtxt = gPTPDriverHandle[tDevInst].pPTPDevCtxt;

#ifndef OPTIMIZED_PTP
    if (!PTP_API_Is_Handle_Vaild_MPTP(uiHndl,PtpDevHndl))
    {
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR; //just representing handle error		
    }
#endif		
    PtpPrivateInitTransaction (pPtpDevCtxt);

    PTP_API_Init_ObjectInfo (&PtpObjInfo);

    PTP_DRV_INFO("\nptp_getobjectinfo : start(H:%lu)\n", uiHndl);
    uiResult = ptp_getobjectinfo(&(pPtpDevCtxt->params), uiHndl, &PtpObjInfo);
    if ( uiResult != PTP_RC_OK )
    {
        PTP_DRV_ERR("\nptp_getobjectinfo return(0x%04x)\n", uiResult);
        PtpDevUnLock(tDevInst);
        return uiResult;
    }

    if (PtpObjInfo.ObjectFormat != PTP_OFC_MP3) 
    {
        PTP_DRV_ERR("\n[handle(%lu) is not jpeg image.\n", uiHndl);
        PTP_API_Clear_ObjectInfo(&PtpObjInfo);	
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR; //just representing handle error
    }         

    uiResult = ptp_getobject(&(pPtpDevCtxt->params),uiHndl, pImage);

    PTP_DRV_INFO ("0x%lu: %9i\t%16s\t0x%08x\t0x%04x\t0x%08x\n", uiHndl,		\
        PtpObjInfo.ObjectCompressedSize, PtpObjInfo.Filename, 										\
        PtpObjInfo.StorageID, PtpObjInfo.ObjectFormat, PtpObjInfo.ParentObject);

    if (uiResult != PTP_RC_OK) 
    {
        PTP_DRV_WARN ("ptp_getobject return(0x%04x)\n", uiResult);
        //*pImageSize = -1;
        *pImageSize=0;//Niraj
    } 
    else 
    {
        *pImageSize = PtpObjInfo.ObjectCompressedSize;
    }

    PTP_API_Clear_ObjectInfo(&PtpObjInfo);		


    PTP_DRV_INFO("\nptp_getobject : end(%x)\n\n", uiResult);
    PTP_DRV_INFO("\nSUCCESS\n");	

    PtpDevUnLock(tDevInst);
    return uiResult;	
}

/**
* @fn	  PTP_API_Send_Reset_MPTP
* @brief	  Resets Ptp Device
* @param (in)	 PtpDevHndl
* @return	  Error code
*		  PTPAPI_ERROR : in case of error.
*		  PTP_RC_OK: in case of success.
* @see 	   
*/

PtpErr_t PTP_API_Send_Reset_MPTP(PTPDeviceHandle  PtpDevHndl)
{

    uint16	uiResult = PTP_RC_OK;
    uint16	uiStatus = 0;
    uint16	/*uiDevStatus[2]*/uiDevStatus[4] = {0,0};
    PTPDevContext *pPtpDevCtxt = NULL;
    PTPInstHndl *pPtpInstHndl=NULL;
    PtpDevInstances_t tDevInst = PtpInvalidDevInst;

    /*Check Parameters*/
    if((NULL == PtpDevHndl))
    {
        PTP_DRV_ERR("\nInvalid params\n");
        return PTPAPI_ERROR;

    }

    /*Check for Init*/
    if(False == gPtpInitFlag)
    {
        PTP_DRV_ERR("\nPTP Drv Not Initialized\n");
        return PTPAPI_ERROR;
    }

    pPtpInstHndl = (PTPInstHndl *)PtpDevHndl;
    if(!PTP_CHECKHNDL(pPtpInstHndl->u32Valid))
    {
        PTP_DRV_ERR("\nHandle Not Valid\n");
        return PTPAPI_ERROR;
    }

    /*Masking for Device Instance*/
    tDevInst = (PtpDevInstances_t)(pPtpInstHndl->u32Valid & 0xf);
    /*Lock Device Mutex*/
    PtpDevLock(tDevInst);

    /*Check Device Detection*/
    if(False == gPTPDriverHandle[tDevInst].IsDeviceDetect)
    {
        PTP_DRV_ERR("\nDevice[%d] Not Detected\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }

    /*Check Device Context*/
    if(NULL ==	gPTPDriverHandle[tDevInst].pPTPDevCtxt)
    {
        PTP_DRV_ERR("\nDevice[%d] Context NULL\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }


    /*Check Device Exists or not*/
    if(True != staPtpChkDevExist(gPTPDriverHandle[tDevInst].pPTPDevCtxt))
    {
       PTP_DRV_ERR("\nDevice[%d] Not Exist\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR; 
    }

    /*Device Context*/
    pPtpDevCtxt = gPTPDriverHandle[tDevInst].pPTPDevCtxt;

    PtpPrivateInitTransaction (pPtpDevCtxt);

    uiResult = stagetusbendpointstatus(pPtpDevCtxt, &uiStatus);
    if (uiResult != PTP_RC_OK)
    {
        PTP_DRV_ERR("\nusb_get_endpoint_status return (%x)\n", uiResult);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR; //just representing status error
    }
    else
    {
        if (uiStatus) 
        {
            uiResult = stausbclearstall(pPtpDevCtxt);
            if (uiResult != PTP_RC_OK)
            {
                PTP_DRV_ERR("\nusb_clear_stall_feature return (%x)\n", uiResult);
                PtpDevUnLock(tDevInst);
                return PTPAPI_ERROR; //just representing status error
            }
        }
    }	

    uiResult = stagetusbdevicestatus(pPtpDevCtxt, uiDevStatus);
    if (uiResult != PTP_RC_OK )
    {
        PTP_DRV_ERR("\nstagetusbdevicestatus return fail.\n");
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR; //just representing status error
    }
    else
    {
        PTP_DRV_WARN("\nstagetusbdevicestatus return(0x%04x/0x%04x)\n", uiDevStatus[0], uiDevStatus[1]);
        if (/*uiDevStatus[0]*/uiDevStatus[3] == PTP_RC_OK) //vishal
        {
            PTP_DRV_INFO("\nDevice status OK\n");
        }
        else
        {
            PtpDevUnLock(tDevInst);
            return PTPAPI_ERROR; //just representing status error
        }
    }

    uiResult = stasetusbdevicereset(pPtpDevCtxt);
    if (uiResult != PTP_RC_OK)
    {
        PTP_DRV_ERR("\nset_ptp_device_reset return(0x%4x)\n", uiResult);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR; //just representing status error
    }

    uiResult = stagetusbdevicestatus(pPtpDevCtxt, uiDevStatus);
    if (uiResult != PTP_RC_OK )
    {
        PTP_DRV_ERR("\nstagetusbdevicestatus return(%x)\n", uiDevStatus[0]);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR; //just representing status error
    }
    else
    {
        if (/*uiDevStatus[0]*/uiDevStatus[3] == PTP_RC_OK) 
        {
            PTP_DRV_INFO("\ndevice status OK\n");
        }
    }

    PTP_DRV_INFO("\nSUCCESS\n");
	
    PtpDevUnLock(tDevInst);
    return uiResult;
}


/**
* @fn	  PTP_API_PartialRead_MPTP(uint32 uiHndl, char** pBuf, uint32 uiLength, uint32 uiStartOffset, uint32 *pReturnCurOffset,PTPDeviceHandle  PtpDevHndl)
* @brief	 Partial Read operation for the given PTP device,
* @param (in)	  PtpDevHndl
*		   (in)	uiHndl
*		   (in)	length
*		   (in)	uiStartOffset
*		   (out)	buf
*		   (out)    pReturnCurOffset
* @return	  Error code
*		  PTPAPI_ERROR : in case of error.
*		  PTP_RC_OK : in case of success.
*/
PtpErr_t PTP_API_PartialRead_MPTP(uint32 uiHndl, char** pBuf, uint32 uiLength, uint32 uiStartOffset, uint32 *pReturnCurOffset,PTPDeviceHandle  PtpDevHndl)
{
    uint16 ret=0;
    uint16 i = 0;
    PTPObjectInfo objinfo;  
    PTPDevContext *pPtpDevCtxt = NULL;
    PTPInstHndl *pPtpInstHndl=NULL;
    PtpDevInstances_t tDevInst = PtpInvalidDevInst;

    /*Check Parameters*/
    if((NULL == PtpDevHndl)||(NULL == pBuf)||(NULL == pReturnCurOffset))
    {
        PTP_DRV_ERR("\nInvalid params\n");
        return PTPAPI_ERROR;

    }
    if(uiLength < 512)
    {
        *pReturnCurOffset = 0;
        PTP_DRV_ERR("\n\nlength< 512\n");
        return  PTP_PARTREAD_ERR;
    }

    /*Check for Init*/
    if(False == gPtpInitFlag)
    {
        PTP_DRV_ERR("\nPTP Drv Not Initialized\n");
        return PTPAPI_ERROR;
    }

    pPtpInstHndl = (PTPInstHndl *)PtpDevHndl;

    if(!PTP_CHECKHNDL(pPtpInstHndl->u32Valid))
    {
        PTP_DRV_ERR("\nHandle Not Valid\n");
        return PTPAPI_ERROR;
    }

    /*Masking for Device Instance*/
    tDevInst = (PtpDevInstances_t)(pPtpInstHndl->u32Valid & 0xf);


    /*Lock Device Mutex*/
    PtpDevLock(tDevInst);

    /*Check Device Detection*/
    if(False == gPTPDriverHandle[tDevInst].IsDeviceDetect)
    {
        PTP_DRV_ERR("\nDevice[%d] Not Detected\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }

    /*Check Device Context*/
    if(NULL ==	gPTPDriverHandle[tDevInst].pPTPDevCtxt)
    {
        PTP_DRV_ERR("\nDevice[%d] Context NULL\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }


    /*Check Device Exists or not*/
    if(True != staPtpChkDevExist(gPTPDriverHandle[tDevInst].pPTPDevCtxt))
    {
	PTP_DRV_ERR("\nDevice[%d] Not Exist\n",tDevInst);
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR; 
    }

    /*Device Context*/
    pPtpDevCtxt = gPTPDriverHandle[tDevInst].pPTPDevCtxt;

    if(uiStartOffset>0)
    {
        PTP_DRV_INFO("\n~ ING\n");
        ret = ptp_getpartialobjectNS( &(pPtpDevCtxt->params), \
            uiHndl, uiStartOffset, uiLength, pBuf);//modified by Niraj
        /* india org, bug fix*/
        *pReturnCurOffset = pPtpDevCtxt->params.getObjRetSize;	//modified by Niraj
        PTP_DRV_INFO("\nPartial read Main:uiStartOffset = %lu \n",\
            uiStartOffset);
        PTP_DRV_INFO("\nPartial read Main:pReturnCurOffset = %d \n", \
            (pPtpDevCtxt->params.getObjRetSize));//modified by Niraj
        PTP_DRV_INFO("\nPartial read Main:required size = %lu, readSize = %d \n \n",\
            uiLength, (pPtpDevCtxt->params.getObjRetSize));//modified by Niraj
        PtpDevUnLock(tDevInst);
        return ret;
    }

    PTP_API_Init_ObjectInfo(&objinfo);	  
    for(i = 0; i < RETRY_CNT; i++)
    {                
        ret = ptp_getobjectinfo(&(pPtpDevCtxt->params), uiHndl, &objinfo);
        if (ret ==PTP_RC_OK)
        {
            PTP_DRV_INFO("\nSUCCESS in getting the object info (MAX retry conut"
                ": %d, current count: %d)\r\n",RETRY_CNT, i+1);
            PTP_DRV_INFO("\nobjinfo.ObjectCompressedSize = %d\n", \
                objinfo.ObjectCompressedSize);

            break;
        }
        else
        {
            PTP_DRV_WARN("\nRETRY in getting the object info (MAX retry conut"
                ": %d, current count: %d)\r\n",RETRY_CNT, i+1);
            continue;			
        }
    }
    if(ret != PTP_RC_OK)
    {			
        PTP_DRV_ERR("\nCould not GetObjectInfo\r\n");
        PTP_API_Clear_ObjectInfo(&objinfo);
        PtpDevUnLock(tDevInst);
        return ret;				
    }	

    if(uiStartOffset >= objinfo.ObjectCompressedSize)
    {
        PTP_DRV_ERR("\n\n Invalid uiStartOffset  --- Greater "
            "than the OBJECT SIZE\r\n");
        PTP_API_Clear_ObjectInfo(&objinfo);
        PtpDevUnLock(tDevInst);
        return PTP_PARTREAD_ERR;	
    }
    if(uiLength > (objinfo.ObjectCompressedSize -uiStartOffset))
    {
        uiLength = objinfo.ObjectCompressedSize -uiStartOffset;
    }

    PTP_DRV_INFO("\n\nbefore AlGetPartialObject_NS\n");
    PTP_DRV_INFO("\n~ ING\n");
    ret = ptp_getpartialobjectNS( &(pPtpDevCtxt->params), \
        uiHndl, uiStartOffset, uiLength, pBuf);	//modified by Niraj
    /* india org, bug fix*/
    *pReturnCurOffset= (pPtpDevCtxt->params.getObjRetSize);//modified by Niraj
    PTP_DRV_INFO("\nPartial read Main:uiStartOffset = %lu \n",\
        uiStartOffset);
    PTP_DRV_INFO("\nPartial read Main:pReturnCurOffset = %d \n", \
        (pPtpDevCtxt->params.getObjRetSize));//modified by Niraj
    PTP_DRV_INFO("\nPartial read Main:required size = %lu, readSize = %d \n \n",\
        uiLength, (pPtpDevCtxt->params.getObjRetSize));//modified by Niraj

    PTP_API_Clear_ObjectInfo(&objinfo);	
    PTP_DRV_INFO("\nSUCCESS\n");
	
    PtpDevUnLock(tDevInst);
    return ret;
}

/**
* @fn	  	PTP_API_PollDev_MPTP
* @brief	  	Polls PTP Device
* @param 	(in)	pDevChange
* @return	  Error code
*		  	PTP_RC_OK:
* @see 	  	New API
* 			Wiselink or any other application has to call this API for Device Polling
*			Calling of This API is essential
*			This API is Mandatory for the Application
*/   

PtpErr_t   PTP_API_PollDev_MPTP( int16 *pDevChange)
{
    if(NULL == pDevChange)
    {
		PTP_DRV_ERR("\nNULL Device Poll Pointer\n");
		return PTPAPI_ERROR;
    }
	
    *pDevChange =  (int16 )PTPDevFindPTPDevices();

//    PTP_DRV_INFO("\nSUCCESS\n");	

    return PTP_RC_OK;
}

/**
* @fn	  	void PTPDriverLog(PtpDrvLogLevel_t PtpDrvLogLevel,const char *Function, const char *Format, ...)
* @brief	  	Prints PTP Driver Logs for Various Levels
* @param 	(in)	PtpDrvLogLevel
* @param	(in) Function
* @param	(int) Format
* @return	None
*/
void PTPDriverLog(PtpDrvLogLevel_t PtpDrvLogLevel,const char *Function, const char *Format, ...)
{
	va_list args;
//	FILE *stream = stdout;
	const char *PREFIX;

#ifndef ENABLE_PTP_DRV_DEBUG_LOG
	if (PtpDrvLogLevel == PtpDrvLogLevelWarning)
	{
		return;
	}
	if (PtpDrvLogLevel == PtpDrvLogLevelInfo)
	{
		return;
	}
#endif
	
	switch (PtpDrvLogLevel) 
	{
	    case PtpDrvLogLevelInfo:
		    PREFIX = "INFO";
//		    stream = stdout;
		    break;
	    case PtpDrvLogLevelWarning:
//		    stream = stderr;
		    PREFIX = "WARNING";
		    break;
	    case PtpDrvLogLevelError:
//		    stream = stderr;
		    PREFIX = "ERROR";
		    break;
	    case PtpDrvLogLevelDebug:
//		    stream = stderr;
		    PREFIX = "DEBUG";
		    break;
	    default:
//		    stream = stderr;
		    PREFIX = "UNKNOWN";
		    break;
	}

	fprintf(stdout, "PTPDRIVER:%s [%s] ", PREFIX, Function);
	va_start (args, Format);
	vfprintf(stdout,Format, args);
	va_end (args);
	fprintf(stdout, "\n");
}
#else
/**
*  @mainpage PTP project
*  @section Description
* - º» °úÁ¦´Â WiseLink¿¡¼­ Sony¿Í Canon°ú °°ÀÌ PTP¸¦ Áö¿øÇÏ´Â µðÁöÅÐ Ä«¸Þ¶ó¿¡¼­ 
	jpeg & thumbnail imageµéÀ» °¡Á®¿À±â À§ÇØ PTP API¸¦ ±¸ÇöÇÑ´Ù.
*  @section Information
*  - ÀÛ ¼º ÀÚ : ÀÌ½ÂÈÆ Ã¥ÀÓ(VD/SW1)
*  - ÀÛ¼ºÀÏÀÚ : 2006/04/14
*  @section MODIFYINFO
* - Author	: SANG-U, SHIM (VD)
* - DATE	: Nov 2006
*/

/** 
* @File Description
* @file name		ptpapi.c
* @brief			implementation of apis for PTP
  * @author			ÀÌ½ÂÈÆ Ã¥ÀÓ(VD/SW1)
* @DATE			2006/04/14
* @MODIFYINFO	      Modified for Linux 2.6
*					Sheetal,SISC and Niraj,SISC - Modified for supporting multiple PTP devices at the same time.
* @Author               Sang U Shim (VD)
* @Date		      Nov 2006
*/
/***********************************************************************/
/* Header Files */
/***********************************************************************/

#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>
#include "ptpapi.h"

#include "PTPDeviceListUpdate.h"

// global variables
static pthread_mutex_t __mutex_avh = PTHREAD_MUTEX_INITIALIZER; /*Function entry critical section*/

/////////////////////////////////////////////
//Global dc : Using in PTP API
PTPDevContext gdc; /* Global variable */
///////////////////////////////////////////////
static PTPDevContext *getDc(PTPDeviceHandle  ptpHandle); /* Global variable */
///////////////////////////////////////////////
//added by sheetal -start
//#define MAX_PTP_DEV 5

#define ENABLE_MPTP_DEBUG_PRINT 0
#define ENABLE_MPTP_INFO_PRINT 1

//added by sheetal - end

#define ENABLE_MPTP_PRINT 0 //VISHAL
//#define	ENDIAN_PATCH_BY_SISC	1 /* 20090724  SISC by Ajeet*/
///////////////////////////////////////////////

#if defined(ENDIAN_PATCH_BY_SISC)
static inline uint16_t
dtoh16ap (PTPParams *params, unsigned char *a)
{
	uint16_t var=0;
	memcpy(&var, a, 2);
	return ((params->byteorder==PTP_DL_LE) ? var : swap16(var));
}
#endif
/*******************************************************************************
MACRO CONSTANT DEFINITIONS 
*******************************************************************************/  
#if 1
#define PTP_BEGIN		pthread_mutex_lock(&__mutex_avh)
#define PTP_END			pthread_mutex_unlock(&__mutex_avh)
#else
#define PTP_BEGIN
#define PTP_END

#endif

#define PTP_PRINT		printf
//only to test svn 


static PTPDevContext *getDc(PTPDeviceHandle  ptpHandle); /* Global variable */

typedef enum PTP_DEBUG_LEVEL_
{
	PTP_DBG_LVL0,
		PTP_DBG_LVL1,
		PTP_DBG_LVL2,
}PTP_DEBUG_LEVEL;

PTP_DEBUG_LEVEL PTP_Debug_Level = PTP_DBG_LVL0; /* Debug level */



///////////////////////////////////////////////
//added by sheetal -start
void MPTP_Info_Print(const char *fmt, ...)
{
	va_list	    ap;
	
	if (ENABLE_MPTP_INFO_PRINT)
	{
		va_start(ap, fmt);
		vfprintf (stdout, fmt, ap);
		va_end(ap);
	}
}

void MPTP_Debug_Print(const char *fmt, ...)
{
	va_list	    ap;
	
	if (ENABLE_MPTP_DEBUG_PRINT)
	{
		va_start(ap, fmt);
		vfprintf (stdout, fmt, ap);
		va_end(ap);
	}
}
//added by sheetal - end

void MPTP_Print(const char *fmt, ...) //VISHAL
{
	va_list	    ap;
	
	if (ENABLE_MPTP_PRINT)
	{
		va_start(ap, fmt);
		vfprintf (stdout, fmt, ap);
		va_end(ap);
	}
}
///////////////////////////////////////////////


/**
* @brief 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @remarks 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         di ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @return
          ¾øÀ½.
* @see
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
*/
void PTP_Debug_Print(const char *fmt, ...)
{
	va_list	    ap;
	
	if (PTP_Debug_Level >= PTP_DBG_LVL1)
	{
		va_start(ap, fmt);
		vfprintf (stdout, fmt, ap);
		va_end(ap);
	}
}

unsigned long Tick(void)
{
	struct tms buf;
	
	return (unsigned long)(times(&buf) * 10);
	
}


/**
* @brief 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @remarks 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         di ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @return
          ¾øÀ½.
* @see
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
*/
void PTP_API_Init_DeviceInfo(PTPDeviceInfo *di)
{
	di->VendorExtensionDesc = NULL;
	di->OperationsSupported = NULL;
	di->EventsSupported = NULL;
	di->DevicePropertiesSupported = NULL;
	di->CaptureFormats = NULL;
	di->ImageFormats = NULL;
	di->Manufacturer = NULL;
	di->Model = NULL;
	di->DeviceVersion = NULL;
	di->SerialNumber = NULL;
}

/**
* @brief 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @remarks 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         oh ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @return
          ¾øÀ½.
* @see
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
*/
void PTP_API_Init_ObjectHandles(PTPObjectHandles *oh)
{
	oh->n = 0;
	oh->Handler = NULL;
}

/**
* @brief 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @remarks 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         storageids ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @return
          ¾øÀ½.
* @see
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
*/
void PTP_API_Init_StorageIDs(PTPStorageIDs *storageids)
{
	storageids->n = 0;
	storageids->Storage = NULL;
}

/**
* @brief 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @remarks 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         oi ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @return
          ¾øÀ½.
* @see
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
*/
void PTP_API_Init_ObjectInfo(PTPObjectInfo *oi)
{
	oi->Filename = NULL;
	oi->Keywords = NULL;
}

/**
* @brief 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @remarks 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         fileinfo ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @return
          ¾øÀ½.
* @see
Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½.
*/
void PTP_API_Init_PTPFileListInfo(PTPFileListInfo **fileinfo)
{
	*fileinfo = NULL;
}

/**
* @brief 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @remarks 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         storageInfo ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @return
          ¾øÀ½.
* @see
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
*/
void PTP_API_Init_PTPStorageInfo(PTPStorageInfo *storageInfo)
{
	storageInfo->StorageDescription = NULL;
	storageInfo->VolumeLabel = NULL;
}

/**
* @brief 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @remarks 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         dc ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @return
          ¾øÀ½.
* @see
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
*/
void PTP_Init_Transaction(PTPDevContext *dc)
{
	PTP_API_Init_DeviceInfo (&(dc->params.deviceinfo));
	PTP_API_Init_ObjectHandles (&(dc->params.handles));
	dc->params.objectinfo = NULL;
}

/**
* @brief 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @remarks 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         storageids ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @return
          ¾øÀ½.
* @see
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
*/
void PTP_API_Clear_StorageIDs(PTPStorageIDs *storageids)
{
	storageids->n = 0;
	if (storageids->Storage != NULL)
	{
		free (storageids->Storage);
		storageids->Storage = NULL;
	}
}

/**
* @brief 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @remarks 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         di ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @return
          ¾øÀ½.
* @see
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
*/
void PTP_API_Clear_DeviceInfo(PTPDeviceInfo *di)
{
	if ( di->VendorExtensionDesc != NULL)
	{
		free (di->VendorExtensionDesc);
		di->VendorExtensionDesc = NULL;
	}
	
	if ( di->OperationsSupported != NULL)
	{
		free (di->OperationsSupported);
		di->OperationsSupported = NULL;
	}
	
	if ( di->EventsSupported != NULL)
	{
		free (di->EventsSupported);
		di->EventsSupported = NULL;
	}
	
	if ( di->DevicePropertiesSupported != NULL)
	{
		free (di->DevicePropertiesSupported);
		di->DevicePropertiesSupported = NULL;
	}
	
	if ( di->CaptureFormats != NULL)
	{
		free (di->CaptureFormats);
		di->CaptureFormats = NULL;
	}
	
	if ( di->ImageFormats != NULL)
	{
		free (di->ImageFormats);
		di->ImageFormats = NULL;
	}
	
	if ( di->Manufacturer != NULL)
	{
		free (di->Manufacturer);
		di->Manufacturer = NULL;
	}
	
	if ( di->Model != NULL)
	{
		free (di->Model);
		di->Model = NULL;
	}
	
	if ( di->DeviceVersion != NULL)
	{
		free (di->DeviceVersion);
		di->DeviceVersion = NULL;
	}
	
	if ( di->SerialNumber != NULL)
	{
		free (di->SerialNumber);
		di->SerialNumber = NULL;
	}
}

/**
* @brief 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @remarks 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         oh ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @return
          ¾øÀ½.
* @see
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
*/
void PTP_API_Clear_ObjectHandles(PTPObjectHandles *oh)
{

	if  (oh->n > 0)
	{
		free (oh->Handler);
		oh->Handler = NULL;
	}
	
	oh->n = 0;
}

/**
* @brief 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @remarks 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         oi ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @return
          ¾øÀ½.
* @see
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
*/
void PTP_API_Clear_ObjectInfo(PTPObjectInfo *oi)
{
	if (oi == NULL)
	{
		return;
	}
	
	if ( oi->Filename != NULL)
	{
		free (oi->Filename);
		oi->Filename = NULL;
	}
	
	if ( oi->Keywords != NULL)
	{
		free (oi->Keywords);
		oi->Keywords = NULL;
	}
}

/**
* @brief 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @remarks 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         fileinfo ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @return
          ¾øÀ½.
* @see
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
*/
void PTP_API_Clear_PTPFileListInfoInfo(PTPFileListInfo **fileinfo)
{
	if (*fileinfo != NULL)
	{
		free (*fileinfo);
		*fileinfo = NULL;
	}
}

/**
* @brief 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @remarks 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         storageInfo ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @return
Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½.
* @see
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
*/
void PTP_API_Clear_PTPStorageInfo(PTPStorageInfo *storageInfo)
{
	if (storageInfo->StorageDescription != NULL)
	{
		free (storageInfo->StorageDescription);
		storageInfo->StorageDescription = NULL;
	}
	
	if (storageInfo->VolumeLabel != NULL)
	{
		free (storageInfo->VolumeLabel);
		storageInfo->VolumeLabel = NULL;
	}
}

/**
* @brief 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @remarks 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         dc ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @return
          ¾øÀ½.
* @see
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
*/
void PTP_Terminate_Transaction(PTPDevContext *dc)
{
	if (dc->params.data != NULL)
	{
        //FREE_MEMORY (dc->params.data);
		dc->params.data = NULL;
	}
	
	PTP_API_Clear_DeviceInfo(&dc->params.deviceinfo);
	PTP_API_Clear_ObjectHandles(&dc->params.handles);
	PTP_API_Clear_ObjectInfo(dc->params.objectinfo);
}


/**
*@brief		This function Initialises the PTPDevice list
*@remarks	Initialise and register device handler for PTP with thread monitoring for 
*	 		new PTP device connection or disconnection
*@param		void
*@return	uint16
PTP_RC_OK : success, else : failure
*/
uint16 PTP_API_Init_DeviceList (void)
{
	uint16 result = PTP_RC_Undefined;

	// Initialise and register device handler for PTP with thread monitoring for 
	// new PTP device connection or disconnection
	//
	if (CPTPDeviceListUpdate::Get().Initialize() == 0)
    {
		return PTP_RC_OK;
    }

	return result;

}

/**
*@brief		This function opens the given device and updates the DC
*@ remarks	Endpoints are updated and ports are intialized
*@param		PTPDevContext *dc:	Device context of the given device
*@return		uint16
PTP_RC_OK : success, else : failure
*/
uint16 PTP_Open_Device_MPTP(PTPDevContext *dc)
{
	
	uint16 result = PTP_RC_Undefined;
	Bool bRet = false;
	
	PTP_Debug_Print("PTP_Open_Device : start\n");
	if(dc==NULL)
	{
		PTP_Debug_Print("[ERROR] PTP_Open_Device_MPTP : PTPDevContext is null.\n");
		return PTPAPI_ERROR;//added by Niraj   
	}
	bRet = usb_find_device_MPTP(dc);
	  
	if (bRet == false)
	{
		PTP_Debug_Print("[ERROR] PTP_Open_Device : Can't find devices\n");
		MPTP_Print("[ERROR] PTP_Open_Device : Can't find devices\n");
		return result;
	}
	
	result = usb_find_endpoints(dc);
	if (result != PTP_RC_OK)
	{
		PTP_Debug_Print("[ERROR] PTP_Open_Device : Can't find endpoint (0x%4x)\n", result);
		MPTP_Print("[ERROR] PTP_Open_Device : Can't find endpoint (0x%4x)\n", result);
		return result;
	}
  
	result = usb_initialize_port(dc);
	if (result != PTP_RC_OK)
	{
		PTP_Debug_Print("[ERROR] PTP_Open_Device : Can't open usb port (0x%4x)\n", result);
		MPTP_Print("[ERROR] PTP_Open_Device : Can't open usb port (0x%4x)\n", result);
		return result;
	}
	
	PTP_Debug_Print("PTP_Open_Device : end (ret : %x)\n", result);
  
	return PTP_RC_OK;
}

/**
* @brief 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @remarks 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         dc ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @return
          ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @see
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
*/
 uint16 PTP_Open_Device(PTPDevContext *dc)
 {
#if 0
 	uint16 result = PTP_RC_Undefined;
 	Bool bRet = false;
 	
 	PTP_Debug_Print("PTP_Open_Device : start\n");
 	
 	bRet = usb_find_device(dc);
 	if (bRet == false) 
 	{
 		PTP_Debug_Print("[ERROR] PTP_Open_Device : Can't find devices\n");
 		return result;
 	}
 	
 	result = usb_find_endpoints(dc);
 	if (result != PTP_RC_OK) 
 	{
 		PTP_Debug_Print("[ERROR] PTP_Open_Device : Can't find endpoint (0x%4x)\n", result);
 		return result;
 	}
 	
 	result = usb_initialize_port(dc);
 	if (result != PTP_RC_OK) 
 	{
 		PTP_Debug_Print("[ERROR] PTP_Open_Device : Can't open usb port (0x%4x)\n", result);
 		return result;
 	}
 	
 	PTP_Debug_Print("PTP_Open_Device : end (ret : %x)\n", result);
 	
 	return PTP_RC_OK;
#endif
		
	dc = NULL;//intialising unused variable
	return PTPAPI_ERROR;
 }


/**
* @brief 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @remarks 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         dc ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @return
          ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @see
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
*/
uint16 PTP_Close_Device(PTPDevContext *dc)
{
	uint16	result = PTP_RC_OK;
	int16	nRet = 0;
	
	PTP_Debug_Print ("PTP_Close_Device : start\n");
	
	result = usb_clear_stall(dc);	// defined in libusb
	
	if (dc->dev != NULL)
	{
		usb_release_interface(dc->ptp_usb.handle, \
			dc->dev->config->interface->altsetting->bInterfaceNumber);
	}
	else
	{
		PTP_Debug_Print("[ERROR] PTP_Close_Device : PTPDevContext dev is null.\n");
	}
	
	nRet = usb_close(dc->ptp_usb.handle);	
	
	PTP_Debug_Print ("PTP_Close_Device : end(nRet:0x%x)\n", nRet);
	
	return result;
}

/**
* @brief 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @remarks 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         dc ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         status ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @return
          ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @see
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
*/
uint16 get_usb_endpoint_status(PTPDevContext *dc, uint16 *status)
{
	uint16	result = PTP_RC_OK;
	int16	nRet = 0;
	
	if (dc == NULL)
	{
		PTP_Debug_Print("[ERROR] get_usb_endpoint_status: PTPDevContext dc is null.\n"); 
		return PTP_RC_Undefined;
	}
	
	nRet = usb_control_msg(dc->ptp_usb.handle, USB_DP_DTH|USB_RECIP_ENDPOINT, USB_REQ_GET_STATUS, \
		USB_FEATURE_HALT, dc->ptp_usb.inep, (char *)status, 2, PTP_TIMEOUT);
#if defined(ENDIAN_PATCH_BY_SISC)
	*status = dtoh16ap (&dc->params, (unsigned char *)status);
#endif
	if (nRet < 0)
	{
		PTP_Debug_Print("[ERROR] get_usb_endpoint_status: usb_control_msg return error(%x)\n", nRet); 
		result = PTP_RC_Undefined;
	}
	//else
	{
		if (*status)
		{
			PTP_Debug_Print("get_usb_endpoint_status: usb_control_msg - IN endpoint status(%x)\n", *status); 
			nRet = clear_usb_stall_feature(dc, dc->ptp_usb.inep);
			PTP_Debug_Print("get_usb_endpoint_status: usb_control_msg - IN endpoint Request Clear Feature(%x)\n", nRet);
		}
	}
	
	*status = 0;
	nRet = usb_control_msg(dc->ptp_usb.handle, USB_DP_DTH|USB_RECIP_ENDPOINT, USB_REQ_GET_STATUS,	\
		USB_FEATURE_HALT, dc->ptp_usb.outep, (char *)status, 2, PTP_TIMEOUT);
#if defined(ENDIAN_PATCH_BY_SISC)
	*status = dtoh16ap (&dc->params, (unsigned char *)status);
#endif
	if (nRet < 0)
	{
		PTP_Debug_Print("[ERROR] get_usb_endpoint_status: usb_control_msg return error(%x)\n", nRet); 
		result = PTP_RC_Undefined;
	}
	//else
	{
		if (*status)
		{
			PTP_Debug_Print("get_usb_endpoint_status: usb_control_msg - OUT endpoint status(%x)\n", *status); 
			nRet = clear_usb_stall_feature(dc, dc->ptp_usb.outep);
			PTP_Debug_Print("get_usb_endpoint_status: usb_control_msg - OUT endpoint Request Clear Feature(%x)\n", nRet);
		}
	}
	
	*status = 0;
	nRet = usb_control_msg(dc->ptp_usb.handle, USB_DP_DTH|USB_RECIP_ENDPOINT, USB_REQ_GET_STATUS,	\
		USB_FEATURE_HALT, dc->ptp_usb.intep, (char *)status, 2, PTP_TIMEOUT);
#if defined(ENDIAN_PATCH_BY_SISC)
	*status = dtoh16ap (&dc->params, (unsigned char *)status);
#endif
	if (nRet < 0)
	{
		PTP_Debug_Print("[ERROR] get_usb_endpoint_status: usb_control_msg return error(%x)\n", nRet); 
		result = PTP_RC_Undefined;
	}
	//else
	{
		if (*status)
		{
			PTP_Debug_Print("get_usb_endpoint_status: usb_control_msg - INTERRUPT endpoint status(%x)\n", *status); 
			nRet = clear_usb_stall_feature(dc, dc->ptp_usb.intep);
			PTP_Debug_Print("get_usb_endpoint_status: usb_control_msg - INTERRUPT endpoint Request Clear Feature(%x)\n", nRet);
		}
	}
	
	
	return result;
}

/**
* @brief 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @remarks 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         dc ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         ep ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @return
          ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @see
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
*/
uint16 clear_usb_stall_feature(PTPDevContext *dc, int ep)
{
	uint16	result = PTP_RC_OK;
	int16	nRet = 0;
	
	nRet = usb_control_msg(dc->ptp_usb.handle, USB_RECIP_ENDPOINT, USB_REQ_CLEAR_FEATURE,	\
		USB_FEATURE_HALT, ep, NULL, 0, PTP_TIMEOUT);
	if (nRet < 0)
	{
		result = PTP_RC_Undefined;
	}
	
	return result;
}

/**
* @brief 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @remarks 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         dc ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         device_status ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @return
          ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @see
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
*/
uint16 get_usb_device_status(PTPDevContext *dc, uint16 *device_status)
{
	uint16	result = PTP_RC_OK;
	int16	nRet = 0;

#if defined(ENDIAN_PATCH_BY_SISC)
	unsigned char status[8] = {0,};
	nRet = usb_control_msg(dc->ptp_usb.handle, USB_DP_DTH|USB_TYPE_CLASS|USB_RECIP_INTERFACE, \
		USB_REQ_GET_DEVICE_STATUS, 0, 0, (char *)status, 8, PTP_TIMEOUT);
	*device_status = dtoh16ap (&dc->params, &status[2]);	

	if (nRet < 0)
	{
		result = PTP_RC_Undefined;
	}
	
	PTP_Debug_Print(" get_usb_device_status ===>> 0x%0x\n", *device_status);
#else
	uint16	status[4] = {0,};
	nRet = usb_control_msg(dc->ptp_usb.handle, USB_DP_DTH|USB_TYPE_CLASS|USB_RECIP_INTERFACE, \
		USB_REQ_GET_DEVICE_STATUS, 0, 0, (char *)device_status/*status*/, 4, PTP_TIMEOUT);

	if (nRet < 0)
	{
		result = PTP_RC_Undefined;
	}
	
	PTP_Debug_Print(" get_usb_device_status ===>> 0x%0x\n", *device_status);

	*device_status = status[3];
#endif	
	return result;
}

/**
* @brief 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @remarks 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         dc ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @return
          ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @see
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
*/
uint16 set_usb_device_reset(PTPDevContext *dc)
{
	uint16	result = PTP_RC_OK;
	int16	nRet = 0;
	
	PTP_Debug_Print ("=====>>>set_usb_device_reset use a handle(%d)\n", dc->ptp_usb.handle);
	
	nRet = usb_control_msg(dc->ptp_usb.handle, USB_TYPE_CLASS|USB_RECIP_INTERFACE, \
		USB_REQ_DEVICE_RESET, 0, 0, NULL, 0, PTP_TIMEOUT);
	if (nRet < 0)
	{
		PTP_Debug_Print ("[Error] : set_usb_device_reset : usb_control_msg return (%x)\n", nRet);
		result = PTP_RC_Undefined;
	}
	
	return result;
	
}


/**
*@brief		This function finds  the given device and rechecks the device connnectivity
*@ remarks	bus number, device number and product id are matched
*@param		PTPDevContext *dc:	Device context of the given ptp device
*@return		Bool 
true : device present, false : device absent
*/
Bool  usb_find_device_MPTP (PTPDevContext *dc)
{
	Bool bReturn = false;
	
	char busNum[10],devNum[10];
	
	if (dc == NULL)
	{
		PTP_Debug_Print("[ERROR] usb_find_device : PTPDevContext is null.\n");
		return false;
	}
// 	uint16 ProdID = dc->dev->descriptor.idProduct;
	strncpy(busNum,dc->bus->dirname,10);
   
	strncpy(devNum,dc->dev->filename,10);
   	
	usb_init();
	usb_find_busses();
// 	usb_find_devices();
	dc->bus = usb_get_busses();
	if (dc->bus == NULL)
	{
		PTP_Debug_Print("[ERROR] usb_find_device : dc->bus is null.\n");
		return false;
	}
	
	
	for (;dc->bus; dc->bus = dc->bus->next)
	{
		for (dc->dev = dc->bus->devices; dc->dev; dc->dev = dc->dev->next)
		{
			if ((dc->dev->config->interface->altsetting->bInterfaceClass == USB_CLASS_PTP)&& (dc->dev->descriptor.bDeviceClass!=USB_CLASS_HUB))
			{
				if ((strcmp(busNum,dc->bus->dirname)==0) && (strcmp(devNum,dc->dev->filename)==0) /*&& (dc->dev->descriptor.idProduct == ProdID)*/)
				{
					
					bReturn = true;
					return bReturn;
					
					
				}
			}
		}
	}
	
	return bReturn;
	
}

/**
* @brief 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @remarks 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         dc ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @return
          ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @see
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
*/
 Bool  usb_find_device (PTPDevContext *dc)
 //struct usb_device* usb_find_device (int busn, int devn,PTPDevContext *dc)
 {
#if 0
 	Bool bReturn = false;
 	
 	if (dc == NULL)
 	{
 		PTP_Debug_Print("[ERROR] usb_find_device : PTPDevContext is null.\n");
 		return false;
 	}
 	
 	usb_init();
 	usb_find_busses();
 	usb_find_devices();
 	dc->bus = usb_get_busses();
 	if (dc->bus == NULL)
 	{
 		PTP_Debug_Print("[ERROR] usb_find_device : dc->bus is null.\n");
 		return false;
 	}
 	
 	for (;dc->bus; dc->bus = dc->bus->next)
 	{
 		for (dc->dev = dc->bus->devices; dc->dev; dc->dev = dc->dev->next)
 		{
 			if ((dc->dev->config->interface->altsetting->bInterfaceClass == USB_CLASS_PTP))
 			{
 				if (dc->dev->descriptor.bDeviceClass!=USB_CLASS_HUB)
 				{
 					
 					bReturn = true;
 					return bReturn;
 					
 				}
 			}
 		}
 	}
 	
 	return bReturn;
#endif
	return false;
 }


/**
* @brief 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @remarks 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         dc ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @return
          ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @see
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
*/
uint16 usb_find_endpoints(PTPDevContext *dc)
{
	uint16 nIndex = 0;
	uint16 nNumEp = 0;
	uint16 result = PTP_RC_OK;
	struct usb_endpoint_descriptor *ep = NULL;
	
	if (dc == NULL)
	{
		PTP_Debug_Print("[ERROR] usb_find_endpoints : PTPDevContext is null.\n");
		return PTP_RC_Undefined;
	}
	
	if (dc->dev == NULL)
	{
		PTP_Debug_Print("[ERROR] usb_find_endpoints : usb device is null.\n");
		return PTP_RC_Undefined;
	}
	
	ep = dc->dev->config->interface->altsetting->endpoint;
	if (ep == NULL)
	{
		PTP_Debug_Print("[ERROR] usb_find_endpoints : endpoint descriptor is null.\n");
		return PTP_RC_Undefined;
	}
	
	nNumEp = dc->dev->config->interface->altsetting->bNumEndpoints;
	if (nNumEp < 3 || nNumEp > 3)
	{
		PTP_Debug_Print("[ERROR] usb_find_endpoints : endpoint number (%x) is not matched.\n", nNumEp);
		return PTP_RC_Undefined;
	}
	
	for (nIndex = 0; nIndex < nNumEp; nIndex++)
	{
		switch (ep[nIndex].bmAttributes)
		{
		    case USB_ENDPOINT_TYPE_INTERRUPT:
			    if ((ep[nIndex].bEndpointAddress & USB_ENDPOINT_DIR_MASK)== USB_ENDPOINT_DIR_MASK)
			    {
				    dc->ptp_usb.intep = ep[nIndex].bEndpointAddress;
				    PTP_Debug_Print("usb_find_endpoints : find a interrupt in endpoint at 0x%02x\n", dc->ptp_usb.intep);
			    }
			    break;
		    case USB_ENDPOINT_TYPE_BULK:
			    if ((ep[nIndex].bEndpointAddress & USB_ENDPOINT_DIR_MASK)== USB_ENDPOINT_DIR_MASK)
			    {
				    dc->ptp_usb.inep = ep[nIndex].bEndpointAddress;
				    PTP_Debug_Print("usb_find_endpoints : find a Bulk(IN) in endpoint at 0x%02x\n", dc->ptp_usb.inep);
			    }
			    else if ((ep[nIndex].bEndpointAddress&USB_ENDPOINT_DIR_MASK)==0)
			    {
				    dc->ptp_usb.outep = ep[nIndex].bEndpointAddress;
				    PTP_Debug_Print("usb_find_endpoints : find a Bulk(OUT) in endpoint at 0x%02x\n", dc->ptp_usb.outep);
			    }
			    break;
		    default:
			    PTP_Debug_Print("usb_find_endpoints : undefind endpoint type (0x%02x)\n", ep[nIndex].bmAttributes);
		}
	}
	
	
	return result;
	
}

/**
* @brief 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @remarks 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         args ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         data ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         format ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @return
          ¾øÀ½.
* @see
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
*/
void PTP_print_debug (void *data, const char *format, va_list args)
{
	data = NULL; 					//intialising unused variable
	PTP_PRINT (format, args);
}

/**
* @brief 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @remarks 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         args ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         data ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         format ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @return
          ¾øÀ½.
* @see
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
*/
void PTP_print_error (void *data, const char *format, va_list args)
{
	data = NULL; 					//intialising unused variable
	PTP_PRINT (format, args);
}

/**
* @brief 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @remarks 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         bytes ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         data ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         readbytes ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         size ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @return
          ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @see
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
*/
static short ptp_read_func (unsigned char *bytes, unsigned int size, void *data, unsigned int *readbytes)
{
	int result=-1;
	PTP_USB *ptp_usb=(PTP_USB *)data;
	int toread=0;
	signed long int rbytes=size;
	
	 
	readbytes = NULL;					//intialising unused variable  
	
	do {
		bytes += toread;
		if (rbytes > PTP_USB_URB) 
		{
			toread = PTP_USB_URB;
		}
		else
		{
			toread = rbytes;
		}
		
		result = USB_BULK_READ(ptp_usb->handle, ptp_usb->inep,(char *)bytes, toread, PTP_TIMEOUT);
		
		if (result==0)
		{
			result=USB_BULK_READ(ptp_usb->handle, ptp_usb->inep,(char *)bytes, toread, PTP_TIMEOUT);
		}
		
		if (result < 0)
		{
			break;
		}
		
		rbytes-=PTP_USB_URB;
	} while (rbytes > 0);
	
	if (result >= 0) 
	{
		return (PTP_RC_OK);
	}
	else 
	{
		return PTP_ERROR_IO;
	}
}

/**
* @brief 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @remarks 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         bytes ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         data ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         size ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @return
          ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @see
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
*/
static short ptp_write_func (unsigned char *bytes, unsigned int size, void *data)
{
	int result;
	PTP_USB *ptp_usb=(PTP_USB *)data;
	
	result=USB_BULK_WRITE(ptp_usb->handle,ptp_usb->outep,(char *)bytes,size,PTP_TIMEOUT);
	if (result >= 0)
	{
		return (PTP_RC_OK);
	}
	else 
	{
		  
        PTP_PRINT ("\n\tERROR IN SENDING COMMAND TO PTP CAMERA\n");
		return PTP_ERROR_IO;
	}
}

/**
* @brief 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @remarks 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         bytes ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         data ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         rlen ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         size ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @return
          ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @see
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
*/
static short ptp_check_int (unsigned char *bytes, unsigned int size, void *data, unsigned int *rlen)
{
	int result;
	
	rlen = NULL;//intialising unused variable
	PTP_USB *ptp_usb=(PTP_USB *)data;
	
	result = USB_BULK_READ(ptp_usb->handle, ptp_usb->intep,(char *)bytes,size,PTP_TIMEOUT);
	if (result == 0)
	{
		result = USB_BULK_READ(ptp_usb->handle, ptp_usb->intep,(char *) bytes, size, PTP_TIMEOUT);
	}
	
	if (result >= 0) 
	{
		return PTP_RC_OK;
	} 
	else 
	{
		return PTP_ERROR_IO;
	}
}

/**
* @brief 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @remarks 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         dc ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @return
          ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @see
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
*/
uint16 usb_initialize_port (PTPDevContext *dc)
{
	uint16	result = PTP_RC_OK;
	int16	nRet = 0;
	
	if (dc == NULL)
	{
		PTP_Debug_Print("[ERROR] usb_initialize_port : PTPDevContext is null\n");
		MPTP_Print("[ERROR] usb_initialize_port : PTPDevContext is null\n");
		return PTP_RC_Undefined;
	}
	
	dc->params.write_func = ptp_write_func;
	dc->params.read_func = ptp_read_func;
	dc->params.check_int_func = ptp_check_int;
	dc->params.check_int_fast_func = ptp_check_int;
	dc->params.error_func = PTP_print_error;
	dc->params.debug_func = PTP_print_debug;
	dc->params.sendreq_func = ptp_usb_sendreq;
	dc->params.senddata_func = ptp_usb_senddata;
	dc->params.getresp_func = ptp_usb_getresp;
	dc->params.getdata_func = ptp_usb_getdata;
	dc->params.data = (void *)(&(dc->ptp_usb));
	dc->params.transaction_id = 0;
	//dc->params.byteorder = PTP_DL_BE;
	//Test to check byteorder by Ajeet Yadav
	result = 0xBB11;	
	dc->params.byteorder = (*(unsigned char *)(&result) == '\x11')?
							PTP_DL_LE : PTP_DL_BE;
	result = PTP_RC_OK;
	
	if (dc->dev < 0)
	{
		PTP_Debug_Print("[ERROR] usb_initialize_port : dc->dev is null\n");
		MPTP_Print("[ERROR] usb_initialize_port : dc->dev is null\n");
		return PTP_RC_Undefined;
	}
	
	if ((dc->ptp_usb.handle=usb_open(dc->dev)))
	{
		if ( !dc->ptp_usb.handle ) 
		{
			PTP_Debug_Print("[ERROR] usb_initialize_port : usb_open (%x)\n", dc->ptp_usb.handle);
			MPTP_Print("[ERROR] usb_initialize_port : usb_open (%x)\n", dc->ptp_usb.handle);
			result = PTP_RC_Undefined;
		}
		else
		{
			nRet = usb_claim_interface(dc->ptp_usb.handle, \
				dc->dev->config->interface->altsetting->bInterfaceNumber);
			if (nRet < 0)
			{
				PTP_Debug_Print("[ERROR] usb_initialize_port : usb_claim_interface (%x)\n", nRet);
				PTP_Debug_Print("[ERROR] usb_initialize_port : usb_claim_interface (%x)\n", dc->dev->config->interface->altsetting->bInterfaceNumber);
				MPTP_Print("[ERROR] usb_initialize_port : usb_claim_interface (%x)\n", nRet);
				result = PTP_RC_Undefined;
				PTP_Close_Device (dc);
			}
		}
	}
	
	return result;	
}

/**
* @brief 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @remarks 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         dc ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @return
          ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @see
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
*/
uint16 usb_clear_stall(PTPDevContext *dc)
{
	uint16	result = PTP_RC_OK;
	int16	nRet = 0;
	int16	status = 0;
	
	nRet = usb_control_msg(dc->ptp_usb.handle, USB_DP_DTH|USB_RECIP_ENDPOINT, \
		USB_REQ_GET_STATUS, USB_FEATURE_HALT, dc->ptp_usb.inep, (char *)&status, 2, PTP_TIMEOUT);
#if defined(ENDIAN_PATCH_BY_SISC)
	status = dtoh16ap (&dc->params, (unsigned char *)&status);
#endif
	if ( nRet < 0)
	{
		PTP_Debug_Print("[ERROR] usb_clear_stall: usb_control_msg(in ep) return error(%x)\n", nRet); 
		result = PTP_RC_Undefined;
	}
	else
	{
		if (status)
		{
			PTP_Debug_Print("usb_clear_stall: usb_control_msg - IN endpoint status(%x)\n", status); 
			result = clear_usb_stall_feature(dc, dc->ptp_usb.inep);
			PTP_Debug_Print("usb_clear_stall: usb_control_msg - IN endpoint Request Clear Feature(%x)\n", result);
		}
	}
	
	nRet  = usb_control_msg(dc->ptp_usb.handle, USB_DP_DTH|USB_RECIP_ENDPOINT, \
		USB_REQ_GET_STATUS, USB_FEATURE_HALT, dc->ptp_usb.outep, (char *)&status, 2, PTP_TIMEOUT);
#if defined(ENDIAN_PATCH_BY_SISC)
	status = dtoh16ap (&dc->params, (unsigned char *)&status);
#endif
	if (nRet < 0)
	{
		PTP_Debug_Print("[ERROR] usb_clear_stall: usb_control_msg(out ep) return error(%x)\n", nRet); 
		result = PTP_RC_Undefined;
	}
	else
	{
		if (status)
		{
			PTP_Debug_Print("usb_clear_stall: usb_control_msg - OUT endpoint status(%x)\n", status); 
			result = clear_usb_stall_feature(dc, dc->ptp_usb.outep);
			PTP_Debug_Print("usb_clear_stall: usb_control_msg - OUT endpoint Request Clear Feature(%x)\n", result);
		}
	}
	
	nRet = usb_control_msg(dc->ptp_usb.handle, USB_DP_DTH|USB_RECIP_ENDPOINT, \
		USB_REQ_GET_STATUS, USB_FEATURE_HALT, dc->ptp_usb.intep, (char *)&status, 2, PTP_TIMEOUT);
#if defined(ENDIAN_PATCH_BY_SISC)
	status = dtoh16ap (&dc->params, (unsigned char *)&status);
#endif
	if (nRet < 0)
	{
		PTP_Debug_Print("[ERROR] usb_clear_stall: usb_control_msg(interrup ep) return error(%x)\n", result); 
		result = PTP_RC_Undefined;
	}
	else
	{
		if (status)
		{
			PTP_Debug_Print("usb_clear_stall: usb_control_msg - INTERRUPT endpoint status(%x)\n", status); 
			result = clear_usb_stall_feature(dc, dc->ptp_usb.intep);
			PTP_Debug_Print("usb_clear_stall: usb_control_msg - INTERRUPT endpoint Request Clear Feature(%x)\n", result);
		}
	}
	
	return result;
}

/**
* @brief 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @remarks 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         dest ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         src ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @return
          ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @see
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
*/
uint16 ptp_deviceinfo_copy(PTPDeviceInfo *dest, PTPDeviceInfo *src)
{
	int16	result = PTP_RC_OK;
	
	dest->StaqndardVersion = src->StaqndardVersion;
	dest->VendorExtensionID = src->VendorExtensionID;
	dest->VendorExtensionVersion = src->VendorExtensionVersion;
	
	if (src->VendorExtensionDesc != NULL)
	{
		dest->VendorExtensionDesc = (char*)calloc(strlen(src->VendorExtensionDesc) + 1, sizeof (char));	//typecasting g++ vishal
		memcpy (dest->VendorExtensionDesc, src->VendorExtensionDesc, strlen(src->VendorExtensionDesc));
	}
	
	dest->FunctionalMode = src->FunctionalMode;
	
	dest->OperationsSupported_len = src->OperationsSupported_len;
	if (src->OperationsSupported_len > 0)
	{
		dest->OperationsSupported = (uint16_t*)calloc (src->OperationsSupported_len, sizeof (char));//typecasting g++ vishal
		memcpy (dest->OperationsSupported, src->OperationsSupported, src->OperationsSupported_len);
	}
	
	dest->EventsSupported_len = src->EventsSupported_len;
	if (src->EventsSupported_len > 0)
	{
		dest->EventsSupported = (uint16_t*)calloc (src->EventsSupported_len, sizeof (char)) ;//typecasting g++ vishal
		memcpy (dest->EventsSupported, src->EventsSupported, src->EventsSupported_len);
	}
	
	dest->DevicePropertiesSupported_len = src->DevicePropertiesSupported_len;
	if (src->DevicePropertiesSupported_len > 0)
	{
		dest->DevicePropertiesSupported = (uint16_t*)calloc (src->DevicePropertiesSupported_len, sizeof(char));//typecasting g++ vishal
		memcpy (dest->DevicePropertiesSupported, src->DevicePropertiesSupported, src->DevicePropertiesSupported_len);
	}
	
	dest->CaptureFormats_len = src->CaptureFormats_len;
	if (src->CaptureFormats_len > 0)
	{
		dest->CaptureFormats = (uint16_t*)calloc (src->CaptureFormats_len, sizeof (char));//typecasting g++ vishal
		memcpy (dest->CaptureFormats, src->CaptureFormats, src->CaptureFormats_len);
	}
	
	dest->ImageFormats_len = src->ImageFormats_len;
	if (src->ImageFormats_len > 0)
	{
		src->ImageFormats = (uint16_t*)calloc (dest->ImageFormats_len, sizeof (char));//typecasting g++ vishal
		dest->ImageFormats = dest->ImageFormats;
	}
	
	if (src->Manufacturer != NULL)
	{
		dest-> Manufacturer = (char*)calloc (strlen(src->Manufacturer) + 1, sizeof(char));//typecasting g++ vishal
		strcpy (dest->Manufacturer, src->Manufacturer);
	}
	
	if (src->Model != NULL)
	{
		dest->Model = (char*)calloc (strlen (src->Model) + 1, sizeof (char));//typecasting g++ vishal
		strcpy (dest->Model, src->Model);
	}
	
	if (src->DeviceVersion != NULL)
	{
		dest->DeviceVersion = (char*)calloc (strlen (src->DeviceVersion) + 1, sizeof (char));//typecasting g++ vishal
		strcpy(dest->DeviceVersion, src->DeviceVersion);
	}
	
	if (src->SerialNumber != NULL)
	{
		dest-> SerialNumber = (char*)calloc (strlen(src-> SerialNumber) + 1, sizeof(char));//typecasting g++ vishal
		strcpy(dest->SerialNumber, src->SerialNumber);
	}
	
	return result;
}


/**
* @brief 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @remarks 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         dest ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         src ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @return
          ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @see
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
*/
uint16 ptp_objectinfo_copy(PTPObjectInfo *dest, PTPObjectInfo *src)
{
	int16	result = PTP_RC_OK;
	
	dest->StorageID = src->StorageID;
	dest->ObjectFormat = src->ObjectFormat;
	dest->ProtectionStatus = src->ProtectionStatus;
	dest->ObjectCompressedSize = src->ObjectCompressedSize;
	dest->ThumbFormat = src->ThumbFormat;
	dest->ThumbCompressedSize = src->ThumbCompressedSize;
	dest->ThumbPixWidth = src->ThumbPixWidth;
	dest->ThumbPixHeight = src->ThumbPixHeight;
	dest->ImagePixWidth = src->ImagePixWidth;
	dest->ImagePixHeight = src->ImagePixHeight;
	dest->ImageBitDepth = src->ImageBitDepth;
	dest->ParentObject = src->ParentObject;
	dest->AssociationType = src->AssociationType;
	dest->AssociationDesc = src->AssociationDesc;
	dest->SequenceNumber = src->SequenceNumber;
	dest->CaptureDate = src->CaptureDate;
	dest->ModificationDate = src->ModificationDate;
	
	if (src->Filename != NULL)
	{
		if (strlen (src->Filename) > 0)
		{
			dest->Filename = (char*)calloc (strlen(src->Filename) + 1, sizeof (char));//typecasting g++ vishal
			strcpy (dest->Filename, src->Filename);
		}
	}
	
	if (src->Keywords != NULL)
	{
		if (strlen(src->Keywords) > 0)
		{
			dest->Keywords = (char*)calloc (strlen(src->Keywords) + 1, sizeof (char));//typecasting g++ vishal
			strcpy (dest->Keywords, src->Keywords);
		}
	}
	
	return result;
	
}

/**
* @brief 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @remarks 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         dest ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
         src ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @return
          ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @see
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
*/
uint16 ptp_handles_copy(PTPObjectHandles *dest , PTPObjectHandles *src)
{
	int16	result = PTP_RC_OK;
	uint32	nIndex = 0;
	
	dest->n = src->n;
	dest->Handler = (uint32_t*)calloc (sizeof (uint32), src->n);//typecasting g++ vishal
	
	for (nIndex = 0; nIndex < src->n; nIndex++)
	{
		dest->Handler[nIndex] = src->Handler[nIndex];
	}
	
	return result;
}


/**
* @brief 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @remarks 
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @param
          ¾øÀ½.
* @return
          ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
* @see
         ³»¿ëÀ» ³ÖÀ¸¼¼¿ä.
*/

 Bool PTP_API_Check_Devices(void)
 {
#if 0
 	struct usb_bus *usb_bus;
 	struct usb_device *usb_dev;
 	Bool bReturn = false;
 	
 	PTP_BEGIN;
 	
 	usb_init();
 	usb_find_busses();
 	usb_find_devices();
 	usb_bus = usb_get_busses();
 	
 	for (; usb_bus; usb_bus = usb_bus->next)
 	{
 		for (usb_dev = usb_bus->devices; usb_dev; usb_dev = usb_dev->next)
 		{
 			if ((usb_dev->config->interface->altsetting->bInterfaceClass == USB_CLASS_PTP))
 			{
 				if (usb_dev->descriptor.bDeviceClass!=USB_CLASS_HUB)
 				{
 					bReturn = true;
 				}
 			}
 		}
 	}
 	
 	PTP_END;
 	
 	return bReturn;
#endif
	return false;
 	
 }






/**
* @brief	      checks whether new devices are connected to the system
* @remarks		counts the number of ptp devices connected to the system
* @param		uint16 *ptp_dev_cnt 			: the num of PTP Devices  
* @return		Bool
True:PTP device present  False:PTP Device absent

*/ 
Bool PTP_API_Check_Devices_MPTP (uint16 *ptp_dev_cnt)
{
	struct usb_bus *usb_bus;
	struct usb_device *usb_dev;
	Bool bReturn = false;
		
	PTP_BEGIN;
	
	MPTP_Debug_Print("Sheetal - PTP_API_Check_Devices : start\n");
	usb_init();
	usb_find_busses();
	usb_find_devices();
	usb_bus = usb_get_busses();
	
	MPTP_Debug_Print("Sheetal - PTP_API_Check_Devices : initialised usb buses\n");
	
	// allocate array for dcs of all devices..for the moment max ptp devices
	for (; usb_bus; usb_bus = usb_bus->next)
	{
		for (usb_dev = usb_bus->devices; usb_dev; usb_dev = usb_dev->next)
		{
			
			if ((usb_dev->config->interface->altsetting->bInterfaceClass == USB_CLASS_PTP))
			{
				if (usb_dev->descriptor.bDeviceClass!=USB_CLASS_HUB)
				{
					(*ptp_dev_cnt)++;
					bReturn = true;
				}
			}
		}
	}
	
	
	PTP_END;
	
	//MPTP_Info_Print("\nSheetal PTP_API_CHECK_Devices : Total USB Devices found \t%d\n",count);
	//MPTP_Info_Print("Sheetal PTP_API_CHECK_Devices : PTP Devices found \t%d\n",*ptp_dev_cnt);
	//MPTP_Debug_Print("Sheetal PTP_API_CHECK_Devices : end\n");
	
	return bReturn;
}


////////////////////////////////////////////////////////////////////////////////////////////
// Newly updated     : ¸®´ª½º 2.6¿¡¼­ PTP µ¿ÀÛÀ» Áö¿øÇÏ±â À§ÇØ »õ·Î ±¸¼ºÇÔ 
//                            Below apis are newly updated for supporting the platform in linux2.6
// Date 	 : Nov 2006
// Author : Sang U Shim
////////////////////////////////////////////////////////////////////////////////////////////
/**
* @brief	      Check whether the file is existing or not
* @remarks		none
* @param		const char *fileName
* @return		Bool
true : success, false : failure
*/ 
Bool t_FileExist(const char* fileName)
{
	FILE *file = NULL;
	Bool ret = false;
	if ((file = fopen(fileName, "r")))
	{
		ret = true;
		fclose(file);
	}
	else
	{
		ret = false;
	}
	return ret;
}

/**
* @brief	      Check whether the string is existing in the source string or not
* @remarks		none
* @param		 char *pStr
* @param		const char *pFindStr
* @param		int bContinue    
* @return		Bool
true : success, false : failure
*/ 
Bool t_FindString(char *pStr, const char *pFindStr, int bContinue)
{
	int strIndex = 0;
	static int findIndex = 0;
	static char* pOldStr = 0x0;
	
	if(!bContinue || pStr != pOldStr)
	{
		findIndex = 0;		
		pOldStr = (char*)pStr;
	}
	
	int len = strlen(pStr);
	
	int findLen = strlen(pFindStr);
	for(strIndex = 0; strIndex < len; strIndex++)
	{
		if(*(pStr+strIndex) == *(pFindStr+findIndex) )
		{
			findIndex++;
			
		}
		else
		{
			findIndex = 0;
		}
		if(findIndex == findLen)
		{	
			return true;
		}
	}
	
	return false;	
}

/**
* @brief	      Check whether the ptp device is existing or not
* @remarks		none
* @param		none
* @return		Bool
true : success, false : failure
*/ 
 Bool PTP_API_CHKDEV(void)
 {
#if 0
#define USB_PROC_BUS_USB_DEVICES_FILE "/proc/bus/usb/devices"
#define PTP_ATTACHED_STRING1 "Cls=06"
#define PTP_ATTACHED_STRING2 "still"
#define PTP_MANUFACTURER "Manufacturer="
#define PTP_PRODUCT "Product="
 	
 	FILE * fp;
 	char pDataLine[1024];
 	int byteCount = 0;
 	
 	int finish = false;
 	unsigned long count;
 	char *pDest = NULL;
 	char product[50] = {0, };
 	
 	
 	if (!t_FileExist(USB_PROC_BUS_USB_DEVICES_FILE))
 	{
 		system("mount -t usbfs none /proc/bus/usb/");
 	}
 	
 	if(!(fp = fopen(USB_PROC_BUS_USB_DEVICES_FILE, "r")))
 	{
 		printf("[ FILE ]file open error : %s\n", USB_PROC_BUS_USB_DEVICES_FILE);
 		return  false;
 	}	
 	
 	
 	while (1) {
 		count = 500;
 		
 		memset (pDataLine, 0, sizeof (pDataLine));
 		byteCount = fread(pDataLine, sizeof(char), 1024, fp);
 		if (byteCount == 0)
 		{
 			finish = true;
 		}
 		else
 		{
 			if ( t_FindString(pDataLine,PTP_ATTACHED_STRING1, true))
 			{	
 				fclose(fp);	
 				if(!(fp = fopen(USB_PROC_BUS_USB_DEVICES_FILE, "r")))
 				{
 					printf("[ FILE ]file open error : %s\n", USB_PROC_BUS_USB_DEVICES_FILE);
 					return  false;
 				}		
 				while(fread(pDataLine, sizeof(char), 1024, fp))	
 				{	
 					pDest = strstr (pDataLine,PTP_PRODUCT);
 					
 					
 					if (pDest != NULL)
 					{
 						pDest = strchr (pDest, '=');
 						if (pDest != NULL)
 						{
 							++pDest;
 							uint16 nIndex = 0;
 							do 
 							{
 								product[nIndex] = *pDest;
 								++nIndex;
 								++pDest;
 							} while (*pDest != '\n' && *pDest != '\0');
 							//printf("\n ProdID=%s \n",product);
 							fclose(fp);
 							return true;
 						}
 					}
 					
 					else	
 						printf("\n pDest = NULL\n"); //vishal
 				}
 				
 				fclose(fp);
 				return true;
 			}
 		}
 		
 		if(finish) break;
 	}
 	
 	fclose(fp);
 	return false;
#endif
	return false;
 }
 


/**
* @brief	      Check whether the ptp device is existing or not
* @remarks		none
* @param		PTPDeviceHandle ptpHandle:    Handle for the given ptp device
* @return		Bool
true : success, false : failure
*/ 

Bool PTP_API_CHKDEV_MPTP(PTPDeviceHandle  ptpHandle)	
{
	
#define USB_PROC_BUS_USB_DEVICES_FILE "/proc/bus/usb/devices"
#define PTP_ATTACHED_STRING1 "Cls=06"
#define PTP_ATTACHED_STRING2 "still"
#define PTP_MANUFACTURER "Manufacturer="
#define PTP_PRODUCT "Product="

	int busNum;/*=(int)dc->bus->location;*/
	int devNum;
	FILE *fp;
	char pDataLine[1024];
	int byteCount = 0;
	
	int finish = false;
	
	char *pDest = NULL;
	char *pDest1=NULL;
	//char product[50] = {0, };
	int len;

	PTPDevContext *dc;
	dc=getDc(ptpHandle);

	if(dc==NULL)
	{
		PTP_Debug_Print("[ERROR] PTP_API_CHKDEV_MPTP : PTPDevContext is null.\n");
		return false;//added by Niraj   
	}
	if (!t_FileExist(USB_PROC_BUS_USB_DEVICES_FILE))
	{
		system("mount -t usbfs none /proc/bus/usb/");
	}
	
	if(!(fp = fopen(USB_PROC_BUS_USB_DEVICES_FILE, "r")))
	{
		printf("[ FILE ]file open error : %s\n", USB_PROC_BUS_USB_DEVICES_FILE);
		return  false;
	}	
	
	char dest1[25];
	memset(dest1,0,25);
	busNum=atoi((dc->bus)->dirname);
	
	if(busNum<9)
	{
		sprintf(dest1,"Bus=0%d",busNum);
	}
	else	
	{
		sprintf(dest1,"Bus=%d",busNum);
	}
	char dest2[25];
	memset(dest2,0,25);
	//devNum=(int)dc->dev->devnum;
	devNum=atoi((dc->dev)->filename);
	
	sprintf(dest2,"Dev#=%3d",devNum);
	
    while (1) 
    {
		memset (pDataLine, 0, sizeof (pDataLine));
		byteCount = fread(pDataLine, sizeof(char), 1024, fp);

		if (byteCount == 0)
		{
			finish = true;
		}
		else
		{
			if ( t_FindString(pDataLine,PTP_ATTACHED_STRING1, true))
			{	
				
				fclose(fp);	
				if(!(fp = fopen(USB_PROC_BUS_USB_DEVICES_FILE, "r")))
				{
					printf("[ FILE ]file open error : %s\n", USB_PROC_BUS_USB_DEVICES_FILE);
					return  false;
				}	
				
				while(fread(pDataLine, sizeof(char),1024 , fp))
				{	
					pDest = strstr (pDataLine,dest1);
					
					if (pDest != NULL)
					{
						if((len=strlen(pDest))<50) //actual value is 45
						{
							fseek(fp,-(len+4),SEEK_CUR);
							continue;
						}
						
						pDest1 = strstr (pDest, dest2);
						if(pDest1!=NULL)
						{
							
							fclose(fp);
							return true;
						}
						else
						{
							pDest=pDest+6;//increment the pointer to point after string dest1
							pDest1 = strstr(pDest,dest1);
							if(pDest1!=NULL)
							{
								len=strlen(pDest1);
								fseek(fp,-(len+4),SEEK_CUR);
							}	
							continue;
						}

					}			
				}
			
				fclose(fp);
				return false;
			}
		}
		
		if(finish) break;
	}
	
	fclose(fp);
	return false;
}

/**
* @brief	      open session for ptp
* @remarks		none
* @param		none
* @return		uint16
PTP_RC_OK : success, else : failure
*/ 
 uint16 PTP_API_OPENSESSION(void)
 {
#if 0
 	uint16 result;	
 	if (PTP_API_CHKDEV() != true)
    {
 		return PTPAPI_ERROR; //just representing status error
    }
 	
 	result = ptp_opensession(&(gdc.params), PTP_START_INDEX);
 	if (result != PTP_RC_OK) 
 	{               
 		PTP_Debug_Print("[ERROR] PTP_API_OPENSESSION : Can't open PTP session (%x)\n", result);
 	}
 	else
 	{
 		PTP_Debug_Print("PTP_API_OPENSESSION is ok\n");		
 	}	
 	return result;
#endif
	return PTPAPI_ERROR;
 }

/**
* @brief	      open session for the given ptp
* @remarks		none
* @param		PTPDeviceHandle ptpHandle:    Handle for the given ptp device
* @return		uint16
PTP_RC_OK : success, else : failure
*/ 
uint16 PTP_API_OPENSESSION_MPTP(PTPDeviceHandle  ptpHandle)
{	
	uint16 result;	

	PTPDevContext *dc;
	dc=getDc(ptpHandle);
	if(dc==NULL)
	{
		PTP_Debug_Print("[ERROR] PTP_API_OPENSESSION_MPTP : PTPDevContext is null.\n");
		return PTPAPI_ERROR;//added by Niraj   
	}
	if (PTP_API_CHKDEV_MPTP(ptpHandle) != true)
	{
		  
		return PTPAPI_ERROR; //just representing status error
	}
	
	result = ptp_opensession(&(dc->params), PTP_START_INDEX);
	if (result != PTP_RC_OK) 
	{        
		  
	    //printf("[ERROR] PTP_API_OPENSESSION : Can't open PTP session (%x)\n", result);
		PTP_Debug_Print("[ERROR] PTP_API_OPENSESSION : Can't open PTP session (%x)\n", result);
	}
	else
	{
		PTP_Debug_Print("PTP_API_OPENSESSION is ok\n");		
	}	

/// added for panasonic - to chk
	if (result == PTP_RC_SessionAlreadyOpened)
	{
		result = ptp_closesession(&(dc->params)); 	
		if (result != PTP_RC_OK) 
		{               
			PTP_Debug_Print("[ERROR] PTP_API_CLOSESESSION : Can't close PTP session (%x)\n", result);
		}
		else
		{
			PTP_Debug_Print("PTP_API_CLOSESESSION is ok\n");		

			result = ptp_opensession(&(dc->params), PTP_START_INDEX);
			if (result != PTP_RC_OK) 
			{        
				//printf("[ERROR] PTP_API_OPENSESSION : Can't open PTP session (%x)\n", result);
				PTP_Debug_Print("[ERROR] PTP_API_OPENSESSION : Can't open PTP session (%x)\n", result);
			}
			else
			{
				PTP_Debug_Print("PTP_API_OPENSESSION is ok\n");		
			}	
		}	
	}
	return result;
}

/**
* @brief	      close session for ptp
* @remarks		none
* @param		none
* @return		uint16
PTP_RC_OK : success, else : failure
*/ 
 uint16 PTP_API_CLOSESESSION(void)
 {
#if 0
 	uint16 result;	
 	if (PTP_API_CHKDEV() != true)
 		return PTPAPI_ERROR; //just representing status error
 	
 	result = ptp_closesession(&(gdc.params)); 	
 	if (result != PTP_RC_OK) 
 	{               
 		PTP_Debug_Print("[ERROR] PTP_API_CLOSESESSION : Can't close PTP session (%x)\n", result);
 	}
 	else
 	{
 		PTP_Debug_Print("PTP_API_CLOSESESSION is ok\n");		
 	}	
 	return result;
#endif
	return PTPAPI_ERROR;
 }


/**
* @brief	      close session for the given ptp device
* @remarks		none
* @param		PTPDeviceHandle ptpHandle:    Handle for the given ptp device
* @return		uint16
PTP_RC_OK : success, else : failure
*/ 
uint16 PTP_API_CLOSESESSION_MPTP(PTPDeviceHandle  ptpHandle)
{	
	uint16 result;	

	PTPDevContext *dc;
	dc=getDc(ptpHandle);

	if(dc==NULL)
	{
		PTP_Debug_Print("[ERROR] PTP_API_CLOSESESSION_MPTP : PTPDevContext is null.\n");
		return PTPAPI_ERROR;//added by Niraj   
	}
    if (PTP_API_CHKDEV_MPTP(ptpHandle) != true)
    {
		return PTPAPI_ERROR; //just representing status error
	}
		result = ptp_closesession(&(dc->params)); 	
	if (result != PTP_RC_OK) 
	{               
		PTP_Debug_Print("[ERROR] PTP_API_CLOSESESSION : Can't close PTP session (%x)\n", result);
	}
	else
	{
		PTP_Debug_Print("PTP_API_CLOSESESSION is ok\n");		
	}	
	return result;
}

/**
* @brief	      open device for ptp
* @remarks		none
  * @param		Bool isgetdeviceinfo - 1 ÀÌ¸é device info¸¦ °¡Á®¿À°í, ±×·¸Áö ¾ÊÀ¸¸é °¡Á®¿ÀÁö ¾Ê´Â´Ù. 
* @return		uint16
PTP_RC_OK : success, else : failure
*/ 
 uint16 PTP_API_OPENDEVICE(Bool isgetdeviceinfo)
 {
#if 0
 	uint16 result;	
 	int i = 0;
 	if (PTP_API_CHKDEV() != true)
 		return PTPAPI_ERROR; //just representing status error
 	
 	PTP_Init_Transaction (&gdc);
 	result = PTP_Open_Device(&gdc);
 	
 	if (result != PTP_RC_OK)
 	{       
 		PTP_Debug_Print("[Error] PTP_API_OPENDEVICE : PTP_API_OPENDEVICE return (%x)\n", result);
 		return result;
 	}
 	
 	if (isgetdeviceinfo)
 	{
 		result = ptp_getdeviceinfo (&(gdc.params), &(gdc.params.deviceinfo));
 		if( result != PTP_RC_OK) 
 		{
 			PTP_Debug_Print("[Error] ptp_getdeviceinfo : ptp_getdeviceinfo return (%x)\n", result);
 			PTP_Close_Device(&gdc);
 			return result;
 		}			
 		else
 		{
 			PTP_Debug_Print("ptp_getdeviceinfo is OK\n");
 			PTP_Debug_Print("The Model is : %s\n\n", gdc.params.deviceinfo.Model);
 			PTP_Debug_Print("The supported operations are \n");
 			for (i = 0; i < gdc.params.deviceinfo.OperationsSupported_len; i++)
 			{				
 				PTP_Debug_Print ("%x\n", gdc.params.deviceinfo.OperationsSupported[i]);
 			}
 		}
 	}
 	
 	return result;	
#endif
	isgetdeviceinfo=0;
	return PTPAPI_ERROR;
 }



/**
* @brief	      opens the given ptp device
* @remarks		none
* @param		Bool isgetdeviceinfo	:flag whether device info is to fetched or not
* @param		PTPDevContext *dc	:Device context of the given device
* @return		uint16
PTP_RC_OK : success, else : failure
*/ 
uint16 PTP_API_OPENDEVICE_MPTP(Bool isgetdeviceinfo, PTPDeviceHandle  ptpHandle)
{
	
	uint16 result;	
	uint32 i = 0;

	PTPDevContext *dc;
	dc=getDc(ptpHandle);

	if(dc==NULL)
	{
		PTP_Debug_Print("[ERROR] PTP_API_OPENDEVICE_MPTP : PTPDevContext is null.\n");
		return PTPAPI_ERROR;//added by Niraj   
	}
	//if (PTP_API_CHKDEV_MPTP(ptpHandle) != true)//vishal
	result=PTP_API_CHKDEV_MPTP(ptpHandle);
	if (result != true)
	{	
		MPTP_Print("\nInside PTP_API_OPENDEVICE_MPTP : PTP_API_CHKDEV_MPTP FAIL result=%x\n",result);
		return PTPAPI_ERROR; //just representing status error
	}
	MPTP_Print("\nInside PTP_API_OPENDEVICE_MPTP : PTP_API_CHKDEV_MPTP success\n");
	PTP_Init_Transaction (dc);
	MPTP_Print("\nInside PTP_API_OPENDEVICE_MPTP : Before PTP_Open_Device_MPTP\n");
	result = PTP_Open_Device_MPTP(dc);
	
	if (result != PTP_RC_OK)
	{       
		PTP_Debug_Print("[Error] PTP_API_OPENDEVICE : PTP_API_OPENDEVICE return (%x)\n", result);
		MPTP_Print("[Error] PTP_API_OPENDEVICE : PTP_API_OPENDEVICE return (%x)\n", result);
		return result;
	}
	
	if (isgetdeviceinfo)
	{
		result = ptp_getdeviceinfo (&(dc->params), &(dc->params.deviceinfo));
		if( result != PTP_RC_OK) 
		{
			PTP_Debug_Print("[Error] ptp_getdeviceinfo : ptp_getdeviceinfo return (%x)\n", result);
		//	printf("\n[Error] ptp_getdeviceinfo : ptp_getdeviceinfo return (%x)\n", result);
			PTP_Close_Device(dc);
			return result;
		}			
		else
		{
			PTP_Debug_Print("ptp_getdeviceinfo is OK\n");
			PTP_Debug_Print("The Model is : %s\n\n", dc->params.deviceinfo.Model);
			PTP_Debug_Print("The supported operations are \n");
			for (i = 0; i < dc->params.deviceinfo.OperationsSupported_len; i++)
			{				
				PTP_Debug_Print ("%x\n", dc->params.deviceinfo.OperationsSupported[i]);
			}
		}
	}
	
	return result;	
}


/**
* @brief	      close device for ptp
* @remarks		none
* @param		none
* @return		uint16
PTP_RC_OK : success, else : failure
*/ 
 uint16 PTP_API_CLOSEDEVICE(void)
 {
#if 0
 	uint16 result;
 	if (PTP_API_CHKDEV() != true)
 		return PTPAPI_ERROR; //just representing status error
 	
 	result = PTP_Close_Device(&gdc);
 	if (result != PTP_RC_OK)
 	{       
 		PTP_Debug_Print("[Error] PTP_API_CLOSEDEVICE : PTP_API_CLOSEDEVICE return (%x)\n", result);
 	}
 	return result;
#endif
	return PTPAPI_ERROR;
 }

/**
* @brief	      close device for the given ptp device
* @remarks		none
* @param		PTPDeviceHandle ptpHandle:    Handle for the given ptp device
* @return		uint16
PTP_RC_OK : success, else : failure
*/ 
uint16 PTP_API_CLOSEDEVICE_MPTP(PTPDeviceHandle  ptpHandle)
{
	
	uint16 result;
	
	PTPDevContext *dc;
	dc=getDc(ptpHandle);

	if(dc==NULL)
	{
		PTP_Debug_Print("[ERROR] PTP_API_CLOSEDEVICE_MPTP : PTPDevContext is null.\n");
		return PTPAPI_ERROR;//added by Niraj   
	}
	if (PTP_API_CHKDEV_MPTP(ptpHandle) != true)
		return PTPAPI_ERROR; //just representing status error
	
	result = PTP_Close_Device(dc);
	if (result != PTP_RC_OK)
	{       
		PTP_Debug_Print("[Error] PTP_API_CLOSEDEVICE : PTP_API_CLOSEDEVICE return (%x)\n", result);
	}
	return result;
}

/**
* @brief	      Initialization of communication with ptp device
* @remarks		this function must be called onetime when the device is attached
* @param		none
* @return		uint16
PTP_RC_OK : success, else : failure
*/ 
 uint16 PTP_API_INIT_COMM(void)
 {
 #if 0
 	uint16 result;
 	
 	if (PTP_API_CHKDEV() != true)
 		return PTPAPI_ERROR; //just representing status error
 	
 	result = PTP_API_OPENDEVICE(DO_GET_DEVICE_INFO);
 	if (result == PTP_RC_OK)
 	{
 		PTP_Debug_Print("PTP_API_OPENDEVICE is ok\n");		
 		result = PTP_API_OPENSESSION();	
 	}
 	else
 	{
 		PTP_Debug_Print("[ERROR] PTP_API_INIT_COMM is not ok\n");
 	}
 	
 	return result;
 #endif
 	
 	return PTPAPI_ERROR;
 }

/**
* @brief	      Initialization of communication with the given ptp device
* @remarks		this function must be called onetime when the device is attached
* @param		PTPDeviceHandle ptpHandle:    Handle for the given ptp device
* @return		uint16
PTP_RC_OK : success, else : failure
*/
uint16 PTP_API_INIT_COMM_MPTP(PTPDeviceHandle  ptpHandle)
{
	
	uint16 result;
	PTPDevContext *dc;
	dc=getDc(ptpHandle);

	if(dc==NULL)
	{
		PTP_Debug_Print("[ERROR] PTP_API_INIT_COMM_MPTP : PTPDevContext is null.\n");
// printf("[ERROR] PTP_API_INIT_COMM_MPTP : PTPDevContext is null.\n ptpHandle=%s",ptpHandle);
		return PTPAPI_ERROR;//added by Niraj   
	}
	MPTP_Print("\nBefore PTP_API_CHKDEV_MPTP\n");
	if (PTP_API_CHKDEV_MPTP(ptpHandle) != true)
	{
//printf("\n PTP_API_CHKDEV_MPTP failed");
		return PTPAPI_ERROR; //just representing status error
	}
	
	//MPTP_Print("\nBefore PTP_API_OPENDEVICE_MPTP\n");
	result = PTP_API_OPENDEVICE_MPTP(DO_GET_DEVICE_INFO,ptpHandle);
	if (result == PTP_RC_OK)
	{
		PTP_Debug_Print("PTP_API_OPENDEVICE is ok\n");	
		MPTP_Print("\nPTP_API_OPENDEVICE_MPTP is ok\n");
		result = PTP_API_OPENSESSION_MPTP(ptpHandle);	
		if(result == PTP_RC_OK)//vishal
			MPTP_Print("\nPTP_API_OPENSESSION_MPTPis ok\n");
		else
		{
		  
//			printf("\nPTP_API_OPENSESSION_MPTPis NOT ok\n");
		}
	}
	else
	{	
		  
//		printf("\nPTP_API_OPENDEVICE_MPTP is not ok\n");
		PTP_Debug_Print("[ERROR] PTP_API_INIT_COMM is not ok\n");
	}
	
	return result;
}

/**
* @brief	      finishing process of communication with the ptp device
* @remarks		none
* @param		none
* @return		uint16
PTP_RC_OK : success, else : failure
*/ 
 uint16 PTP_API_FINISH_COMM(void)
 {
#if 0
 	uint16 result;
 	result = PTP_API_CLOSESESSION();
 	if (result == PTP_RC_OK)
 		result = PTP_API_CLOSEDEVICE();
 	return result;
#endif
	return PTPAPI_ERROR;
 }

/**
* @brief	      finishing process of communication with the given ptp device
* @remarks		none
* @param		PTPDeviceHandle ptpHandle:    Handle for the given ptp device
* @return		uint16
PTP_RC_OK : success, else : failure
*/
uint16 PTP_API_FINISH_COMM_MPTP(PTPDeviceHandle  ptpHandle)
{
	uint16 result;

	result = PTP_API_CLOSESESSION_MPTP(ptpHandle);

	if (result == PTP_RC_OK)
	{
		MPTP_Print("\nPTP_API_CLOSESESSION_MPTP is OK\n");

		result = PTP_API_CLOSEDEVICE_MPTP(ptpHandle);
		if(result == PTP_RC_OK)
			MPTP_Print("\nPTP_API_CLOSEDEVICE_MPTP is OK\n");
		else
			MPTP_Print("\nPTP_API_CLOSEDEVICE_MPTP is not OK\n");
	}
	else
		MPTP_Print("\nPTP_API_CLOSESESSION_MPTP is not OK\n");

	return result;
}

/**
* @brief	      check the handle whether it's valid or not
* @remarks		none
* @param		none
* @return		Bool
true : success, false : failure
*/ 
 Bool PTP_API_Is_Handle_Vaild (uint32 ObjectHandle)
 {
#if 0
 /*
 *
	  * 2007-11-14 (KKS) : PTP ¸®½ºÆ® ÃßÃâ¼º´É °³¼±À» À§ÇØ »èÁ¦ÇÔ
	  * PTP device¿¡ ÆÄÀÏÀÌ ¸¹À»¼ö·Ï handleÀÇ À¯È¿¼º °Ë»ç·Î ÀÎÇØ µ¿ÀÛ½Ã°£ÀÌ ´À·ÁÁö´Â ¹®Á¦°¡ ¹ß»ýÇÔ
	  * ÇÚµéÀÇ À¯È¿¼º ÆÇ´ÜÀº API È£Ãâ½Ã ptp commandÀÇ return value Ã¼Å©·Î Ã³¸®ÇØ¾ßÇÔ
 	*/
 	PTPObjectHandles objecthandles;
 	Bool Is_objecthandle_found = false;
 	int i = 0;
 	
 	PTP_BEGIN;
 	
 	//HANDLE CHECK (All handles in device)
 	if (ptp_getobjecthandles (&gdc.params, 0xffffffff,
 		/*uint32_t objectformatcode -> optional*/0, /*uint32_t associationOH -> optional*/0,
 		&objecthandles) != PTP_RC_OK)
 	{
 		PTP_END;
 		return false;
 	}
 	
 	//Find a handle matched with the input handle
 	for (i = 0; i < objecthandles.n; i++)
 	{
 		PTP_Debug_Print("handle [%d] : %x ", i, objecthandles.Handler[i]);
 		if (ObjectHandle == objecthandles.Handler[i])
 		{
 			PTP_Debug_Print("\nHandle is found\n");
 			Is_objecthandle_found = true;
 			break;
 		}
 	}
 	
 	if (Is_objecthandle_found == false)
 	{
 		PTP_Debug_Print("\nHandle is not found\n");
 	}
 	
 	PTP_END;
 	
 	return Is_objecthandle_found;
#endif
	ObjectHandle = 0;
	return false;
 	
 }

/**
* @brief	      check the handle whether it's valid or not for the given ptp device
* @remarks		none
* @param		PTPDeviceHandle ptpHandle:    Handle for the given ptp device
* @param		uint32 ObjectHandle :handle of the object in the ptp device
* @return		Bool
true : success, false : failure
*/ 
Bool PTP_API_Is_Handle_Vaild_MPTP (uint32 ObjectHandle, PTPDeviceHandle  ptpHandle)
{

	
	/*
	  *
	  * 2007-11-14 (KKS) : PTP ¸®½ºÆ® ÃßÃâ¼º´É °³¼±À» À§ÇØ »èÁ¦ÇÔ
	  * PTP device¿¡ ÆÄÀÏÀÌ ¸¹À»¼ö·Ï handleÀÇ À¯È¿¼º °Ë»ç·Î ÀÎÇØ µ¿ÀÛ½Ã°£ÀÌ ´À·ÁÁö´Â ¹®Á¦°¡ ¹ß»ýÇÔ
	  * ÇÚµéÀÇ À¯È¿¼º ÆÇ´ÜÀº API È£Ãâ½Ã ptp commandÀÇ return value Ã¼Å©·Î Ã³¸®ÇØ¾ßÇÔ
	  */
	PTPObjectHandles objecthandles;
	Bool Is_objecthandle_found = false;
	uint32 i = 0;

	PTPDevContext *dc;
	dc=getDc(ptpHandle);
	if(dc==NULL)
	{
		PTP_Debug_Print("[ERROR] PTP_API_Is_Handle_Valild_MPTP : PTPDevContext is null.\n");
		return false;//added by Niraj   
	}
	
	PTP_BEGIN;	
		
	//HANDLE CHECK (All handles in device)
	//if (ptp_getobjecthandles (&gdc.params, 0xffffffff,
	if (ptp_getobjecthandles (&(dc->params), 0xffffffff,
		/*uint32_t objectformatcode -> optional*/ 0, /*uint32_t associationOH -> optional*/0,
		&objecthandles) != PTP_RC_OK)
	{
		PTP_END;
		return false;
	}
		
	//Find a handle matched with the input handle
	
	for (i = 0; i < objecthandles.n; i++)
	{
		PTP_Debug_Print("handle [%d] : %x ", i, objecthandles.Handler[i]);
		if (ObjectHandle == objecthandles.Handler[i])
		{
			PTP_Debug_Print("\nHandle is found\n");								
			Is_objecthandle_found = true;
			break;
		}
	}
		
	if (Is_objecthandle_found == false)
	{		
		PTP_Debug_Print("\nHandle is not found\n");		
	}	
	
	PTP_END;	
		
	return Is_objecthandle_found;
	
}

/**
* @brief	      check the existence of ptp device
* @remarks		none
* @param		PTPDeviceInfo *deviceinfo 
* @return		Bool
true : success, false : failure
*/ 
 Bool Is_PTP_device(PTPDeviceInfo *deviceinfo)
 {
#if 0
 	uint16  result = PTP_RC_OK;
 	Bool ret = false;
 	
 	PTP_Init_Transaction (&gdc);
 	
 	if (PTP_API_CHKDEV() != true)
 		return false; //just representing status error
 	
 	PTP_BEGIN;
 	
 	result = ptp_getdeviceinfo (&(gdc.params), &(gdc.params.deviceinfo));
 	if( result != PTP_RC_OK)
 	{
 		PTP_Debug_Print("[Error] Is_PTP_device : ptp_getdeviceinfo return (%x)\n", result);
 		ret = false;
 	}
 	else
 	{
 		PTP_Debug_Print("Is_PTP_device is OK\n");
 		ret = true;
 	}
 	
 	PTP_END;
 	
 	return ret;
#endif
	
	deviceinfo = NULL;   //intialising unused variable
	return false;
 	
 }

/**
* @brief	      check the existence of the given ptp device
* @remarks		none
* @param		PTPDeviceHandle ptpHandle:    Handle for the given ptp device
* @return		Bool
true : success, false : failure
*/ 
Bool Is_PTP_device_MPTP(PTPDeviceHandle  ptpHandle)
{
	
	uint16  result = PTP_RC_OK;
	Bool ret = false;
	
	PTPDevContext *dc;
	dc=getDc(ptpHandle);
	if(dc==NULL)
	{
		PTP_Debug_Print("[ERROR] Is_PTP_device_MPTP : PTPDevContext is null.\n");
		return false;//added by Niraj   
	}
	PTP_Init_Transaction (dc);
	
	if (PTP_API_CHKDEV_MPTP(ptpHandle) != true)
		return false; //just representing status error
	
	PTP_BEGIN;
	
	result = ptp_getdeviceinfo (&(dc->params), &(dc->params.deviceinfo));
	if( result != PTP_RC_OK)
	{
		PTP_Debug_Print("[Error] Is_PTP_device : ptp_getdeviceinfo return (%x)\n", result);
		ret = false;
	}
	else
	{
		PTP_Debug_Print("Is_PTP_device is OK\n");		
		ret = true;
	}
	
	PTP_END;
	
	return ret;
	
}

/**
* @brief	      Get file list from the handle
* @remarks		file list is including folders and jpg files
* @param		uint32 storageID			: storage id
* @param		uint32 ParentObjectHandle 	: handle 
* @param		PTPFileListInfo **fileinfo 	: store of filelist   
* @param		uint32 *nCount 			: the num of files(including folders)   
* @return		uint16
PTP_RC_OK : success, else : failure
*/ 
 uint16 PTP_API_Get_FilesList (uint32 storageID, uint32 ParentObjectHandle, PTPFileListInfo **fileinfo, uint32 *nCount)
 {
#if 0
 	uint32	n = 0;
 	uint32	nIndex = 0;
 	uint16	result = PTP_RC_OK;
 	
 	int		nRet = 0;
 	char		foldername[MAX_FILENAME] = {'0'};
 	char		filename[MAX_FILENAME] = {'0'};
 	
 	struct tm* tmPtr = 0;
 	struct tm lt;
 	
 	PTPObjectInfo		oi;
 	PTPObjectHandles	oh1, oh2;
 	
 	int i = 0;
 	uint32 numdir = 0;	
 	uint32 numjpg = 0;
 	
 	unsigned long stTick = 0;
 	unsigned long edTick = 0;
 	
 	if (PTP_API_CHKDEV() != true)
 		return PTPAPI_ERROR; //just representing status error
 	
 	PTP_Init_Transaction (&gdc);
 	
 	PTP_API_Init_ObjectHandles(&oh1);
 	PTP_API_Init_ObjectHandles(&oh2);
 	
 	PTP_Debug_Print("PTP_API_Get_FilesList : start(H:%x)\n", ParentObjectHandle);
 	
 	PTP_BEGIN;		
 	
 	stTick = Tick() ;
 	
 	//Find All list from the ParentObjectHandle
 	result = ptp_getobjecthandles (&(gdc.params),storageID, 0x00000000, ParentObjectHandle, &(gdc.params.handles));
 	if (result != PTP_RC_OK)
 	{
 		PTP_END;
 		return result;
 	}
 	
 	PTP_Debug_Print("The # of handles : %d\n", gdc.params.handles.n);
 	PTP_Debug_Print("Handler:        size: \tname:\n");
 	for (i = 0; i < gdc.params.handles.n; i++) {
 		result = ptp_getobjectinfo(&(gdc.params),gdc.params.handles.Handler[i], &oi);
 		if (result != PTP_RC_OK)
 		{
 			PTP_END;
 			return result;
 		}
 		
 		if (oi.ObjectFormat == PTP_OFC_Association)
 		{
 			numdir++;
 			continue;
 		}
 		if (oi.ObjectFormat == PTP_OFC_EXIF_JPEG)
 			numjpg++;
 		PTP_Debug_Print("0x%08x: % 9i\t%s\n",gdc.params.handles.Handler[i],
 			oi.ObjectCompressedSize, oi.Filename);
 	}
 	
 	PTP_Debug_Print("the num of dir : %d \n", numdir);
 	PTP_Debug_Print("the num of jpg : %d \n", numjpg);
 	
	// SONY DSC T20ÀÇ °æ¿ì, PTP_OFC_Association cmd·Î Æú´õ¿¡ ´ëÇÑ  handle list ¿äÃ»½Ã device°¡ STALLµÊ
	// ÆÄÀÏ,Æú´õ ¸®½ºÆ® ±¸¼º½Ã NULL °ªÀ» Ã¼Å©ÇØ¼­ handle list ¿äÃ»
 	if(numdir != 0)
 	{
 		result = ptp_getobjecthandles (&(gdc.params), storageID, PTP_OFC_Association, ParentObjectHandle, &(oh1));
 		if( result != PTP_RC_OK) 
 		{
 			PTP_Debug_Print("[Error] PTP_API_Get_FilesList : ptp_getobjecthandles(folder) return (%x)\n", result);
 			PTP_END;
 			return result;
 		}	
 	}
 	
 	if(numjpg != 0)
 	{
 		result = ptp_getobjecthandles (&(gdc.params), storageID, PTP_OFC_EXIF_JPEG, ParentObjectHandle, &(oh2));
 		if( result != PTP_RC_OK) 
 		{
 			PTP_Debug_Print("[Error] PTP_API_Get_FilesList : ptp_getobjecthandles(file) return (%x)\n", result);
 			PTP_END;		
 			return result;		
 		}	
 	}
 	
 	
 	*nCount = numdir + numjpg;/*oh1.n + oh2.n;*/
 	if (*nCount <= 0)
 	{
 		PTP_Debug_Print("[Error] PTP_API_Get_FilesList : oh1.n + oh2.n is less than 0\n");
 		PTP_END;		
 		return PTPAPI_ERROR; // just representing wrong count error		
 	}
 	
 	*fileinfo = (PTPFileListInfo *)calloc (sizeof(PTPFileListInfo), *nCount);
 	if (*fileinfo == NULL)
 	{
 		PTP_Debug_Print("[Error] PTP_API_Get_FilesList : memory alloc error(PTPFileListInfo)\n");
 		PTP_END;		
 		return PTPAPI_ERROR; // just representing memory allocation error		
 	}
 	
 	
 	n = 0;
 	for (nIndex = 0; nIndex <numdir /*oh1.n*/; nIndex++) 
 	{
 		PTP_API_Init_ObjectInfo(&oi);
 		result = ptp_getobjectinfo(&(gdc.params), oh1.Handler[nIndex], &oi);
 		if ( result != PTP_RC_OK )
 		{
 			PTP_Debug_Print("[Error] PTP_API_Get_FilesList : ptp_getobjectinfo return(%x)\n", result);
 			continue;
 		}
 		
 		if (oi.ObjectFormat != PTP_OFC_Association) continue;
 		
 		// 2007.08.06,  Kyungsik
// ¹®Á¦Á¡ : µ¿¿µ»ó(*.MOV) ÆÄÀÏÀ» folder·Î ºÐ·ùÇÏ¿© ObjectFormatÀ» folder·Î return 
// ´ëÃ¥ : ObjectFormat(folder,0x3001)ÀÎ °æ¿ì, objectinfoÀÇ  AssociationType(folder,0x0001) ¿©ºÎ¸¦  Ã¼Å©ÇÑ´Ù.
//		ÀÌ °æ¿ì AssociationTypeÀ» Áö¿øÇÏÁö ¾Ê´Â Àåºñ°¡ Á¸ÀçÇÒ ¼ö ÀÖÀ¸¹Ç·Î, AssociationType(undef, 0x0000)µµ È®ÀÎÇØ¾ß ÇÑ´Ù.
// Àåºñ¸í :  Panasonic FX12 
 		if (oi.AssociationType > PTP_AT_GenericFolder)
 		{
 			PTP_Debug_Print("[Error] This file is not folder \n");
 			continue;
 		}
 		
 		strcpy ((*fileinfo)[n].filename, oi.Filename);
 		(*fileinfo)[n].filetype = DIR_TYPE;
 		(*fileinfo)[n].storageId = oi.StorageID;
 		(*fileinfo)[n].ParentObject = oi.ParentObject;
 		(*fileinfo)[n].handle = oh1.Handler[nIndex];
 		
 		PTP_API_Clear_ObjectInfo (&oi);
 		++n;
 	}
 	
 	
 	for (nIndex = 0; nIndex < numjpg/*oh2.n*/; nIndex++)
 	{
 		PTP_API_Init_ObjectInfo(&oi);
 		result = ptp_getobjectinfo(&(gdc.params), oh2.Handler[nIndex],&oi);
 		if ( result != PTP_RC_OK )
 		{
 			PTP_Debug_Print("[Error] PTP_API_Get_FilesList : ptp_getobjectinfo return(%x)\n", result);
 			continue;
 		}
 		
 		if (oi.ObjectFormat != PTP_OFC_EXIF_JPEG) continue;
 		
 		tmPtr = NULL;
 		tmPtr = localtime_r ( (const time_t*)&oi.CaptureDate, &lt);
 		if (tmPtr != NULL)
 		{
 			(*fileinfo)[n].year = tmPtr->tm_year + 1900;
 			(*fileinfo)[n].month = tmPtr->tm_mon + 1;
 			(*fileinfo)[n].day = tmPtr->tm_mday ;
 			(*fileinfo)[n].hour = tmPtr->tm_hour;
 			(*fileinfo)[n].min = tmPtr->tm_min;
 			(*fileinfo)[n].sec = tmPtr->tm_sec;
 		}
 		
 		strcpy ((*fileinfo)[n].filename, oi.Filename);
 		(*fileinfo)[n].filetype = FILE_TYPE;
 		(*fileinfo)[n].storageId = oi.StorageID;
 		(*fileinfo)[n].ParentObject = oi.ParentObject;
 		(*fileinfo)[n].handle = oh2.Handler[nIndex];
 		
 		PTP_API_Clear_ObjectInfo (&oi);
 		++n;
 	}
 	
 	// 2007.08.06,  ssu
 	// For the error case
 	// If any file or folder's association type is not proper, at that time we should count again the real number of files.
 	if (*nCount != n)
 		*nCount = n;
 	
 	edTick = Tick() ;
 	PTP_Debug_Print ("PTP_API_Get_FilesList : oh1.n = %d, oh2.n = %d, storageId= %x, parent handle = %x, speed = %d(ms)\n", 
 		oh1.n, oh2.n, storageID, ParentObjectHandle, (edTick -stTick ));
 	
 	PTP_API_Clear_ObjectHandles(&oh1);
 	PTP_API_Clear_ObjectHandles(&oh2);
 	
 	PTP_END;
 	
 	PTP_Debug_Print("PTP_API_Get_FilesList : end (%x)\n\n", result);
 	
 	return result;	
#endif

	storageID = 0; //intialising unused variable
	ParentObjectHandle = 0;
	fileinfo = NULL;
	*nCount = 0;

	return PTPAPI_ERROR;
 }


/**
* @brief	      Get file list from the handle
* @remarks		file list is including folders and jpg files
* @param		uint32 storageID			: storage id
* @param		uint32 ParentObjectHandle 	: handle 
* @param		PTPFileListInfo **fileinfo 	: store of filelist   
* @param		uint32 *nCount 			: the num of files(including folders)   
* @param		PTPDevContext *dc		:Device context of the given device
* @return		uint16
PTP_RC_OK : success, else : failure
*/
uint16 PTP_API_Get_FilesList_MPTP (uint32 storageID, uint32 ParentObjectHandle, PTPFileListInfo **fileinfo, uint32 *nCount,PTPDeviceHandle  ptpHandle)
{
	
	uint32	n = 0;
	uint32	nIndex = 0;
	uint16	result = PTP_RC_OK;
	
	//	int		nRet = 0;
	//	char		foldername[MAX_FILENAME] = {'0'};
	//	char		filename[MAX_FILENAME] = {'0'};
	
	struct tm* tmPtr = 0;
	struct tm lt;
	
	PTPObjectInfo		oi;
	PTPObjectHandles	oh1, oh2;
	
	uint32 i = 0;
	uint32 numdir = 0;	
	uint32 numjpg = 0;
	
	unsigned long stTick = 0;
	unsigned long edTick = 0;

	PTPDevContext *dc;
	dc=getDc(ptpHandle);
	
	if(dc==NULL)
	{
		PTP_Debug_Print("[ERROR] PTP_API_Get_FilesList_MPTP : PTPDevContext is null.\n");
		return PTPAPI_ERROR;//added by Niraj   
	}
	if (PTP_API_CHKDEV_MPTP(ptpHandle) != true)
		return PTPAPI_ERROR; //just representing status error
	
	PTP_Init_Transaction (dc);
	
	PTP_API_Init_ObjectHandles(&oh1);
	PTP_API_Init_ObjectHandles(&oh2);
	
	PTP_Debug_Print("PTP_API_Get_FilesList : start(H:%x)\n", ParentObjectHandle);
	
	PTP_BEGIN;		
	
	stTick = Tick() ;
	
	//Find All list from the ParentObjectHandle
	result = ptp_getobjecthandles (&(dc->params),storageID, 0x00000000, ParentObjectHandle, &(dc->params.handles));
	if (result != PTP_RC_OK)
	{
		PTP_END;
		return result;
	}
	
	PTP_Debug_Print("The # of handles : %d\n", dc->params.handles.n);
	PTP_Debug_Print("Handler:        size: \tname:\n");
    for (i = 0; i < dc->params.handles.n; i++) 
    {
		result = ptp_getobjectinfo(&(dc->params),dc->params.handles.Handler[i], &oi);
		if (result != PTP_RC_OK)
		{
			PTP_END;
			return result;
		}
		
		if (oi.ObjectFormat == PTP_OFC_Association)
		{
			numdir++;
			continue;
		}
		if (oi.ObjectFormat == PTP_OFC_EXIF_JPEG)
        {
			numjpg++;
        }
		PTP_Debug_Print("0x%08x: % 9i\t%s\n",dc->params.handles.Handler[i],
			oi.ObjectCompressedSize, oi.Filename);
	}
	
	PTP_Debug_Print("the num of dir : %d \n", numdir);
	PTP_Debug_Print("the num of jpg : %d \n", numjpg);
	
	
	// SONY DSC T20Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½, PTP_OFC_Association cmdÃ¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½  handle list Ã¯Â¿Â½Ã¯Â¿Â½ÃƒÂ»Ã¯Â¿Â½Ã¯Â¿Â½ deviceÃ¯Â¿Â½Ã¯Â¿Â½ STALLÃ¯Â¿Â½Ã¯Â¿Â½
	// Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½,Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã†Â® Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ NULL Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ ÃƒÂ¼Ã…Â©Ã¯Â¿Â½Ã˜Â¼Ã¯Â¿Â½ handle list Ã¯Â¿Â½Ã¯Â¿Â½ÃƒÂ»
	if(numdir != 0)
	{
		result = ptp_getobjecthandles (&(dc->params), storageID, PTP_OFC_Association, ParentObjectHandle, &(oh1));
		if( result != PTP_RC_OK) 
		{
			PTP_Debug_Print("[Error] PTP_API_Get_FilesList : ptp_getobjecthandles(folder) return (%x)\n", result);
			PTP_END;
			return result;
		}	
	}
	
	if(numjpg != 0)
	{
		result = ptp_getobjecthandles (&(dc->params), storageID, PTP_OFC_EXIF_JPEG, ParentObjectHandle, &(oh2));
		if( result != PTP_RC_OK) 
		{
			PTP_Debug_Print("[Error] PTP_API_Get_FilesList : ptp_getobjecthandles(file) return (%x)\n", result);
			PTP_END;		
			return result;		
		}	
	}
	
	
	*nCount = numdir + numjpg;/*oh1.n + oh2.n;*/
	if (*nCount <= 0)
	{
		PTP_Debug_Print("[Error] PTP_API_Get_FilesList : oh1.n + oh2.n is less than 0\n");
		PTP_END;		
		return PTPAPI_ERROR; // just representing wrong count error		
	}
	
	*fileinfo = (PTPFileListInfo *)calloc (sizeof(PTPFileListInfo), *nCount);
	if (*fileinfo == NULL)
	{
		PTP_Debug_Print("[Error] PTP_API_Get_FilesList : memory alloc error(PTPFileListInfo)\n");
		PTP_END;		
		return PTPAPI_ERROR; // just representing memory allocation error		
	}
	
	
	n = 0;
	for (nIndex = 0; nIndex <numdir /*oh1.n*/; nIndex++) 
	{
		PTP_API_Init_ObjectInfo(&oi);
		result = ptp_getobjectinfo(&(dc->params), oh1.Handler[nIndex], &oi);
		if ( result != PTP_RC_OK )
		{
			PTP_Debug_Print("[Error] PTP_API_Get_FilesList : ptp_getobjectinfo return(%x)\n", result);
			continue;
		}
		
		if (oi.ObjectFormat != PTP_OFC_Association) continue;
		
		// 2007.08.06,  Kyungsik
		// Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ : Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½(*.MOV) Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ folderÃ¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½ÃÂ·Ã¯Â¿Â½Ã¯Â¿Â½ÃÂ¿Ã¯Â¿Â½ ObjectFormatÃ¯Â¿Â½ folderÃ¯Â¿Â½Ã¯Â¿Â½ return 
		// Ã¯Â¿Â½Ã¯Â¿Â½ÃƒÂ¥ : ObjectFormat(folder,0x3001)Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½, objectinfoÃ¯Â¿Â½Ã¯Â¿Â½  AssociationType(folder,0x0001) Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ÃŽÂ¸Ã¯Â¿Â½  ÃƒÂ¼Ã…Â©Ã¯Â¿Â½Ã‘Â´Ã¯Â¿Â½.
		//		Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ AssociationTypeÃ¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½ÃŠÂ´Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã‡Â·Ã¯Â¿Â½, AssociationType(undef, 0x0000)Ã¯Â¿Â½Ã¯Â¿Â½ ÃˆÂ®Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã˜Â¾Ã¯Â¿Â½ Ã¯Â¿Â½Ã‘Â´Ã¯Â¿Â½.
		// Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ :  Panasonic FX12 
		if (oi.AssociationType > PTP_AT_GenericFolder)
		{
			PTP_Debug_Print("[Error] This file is not folder \n");
			continue;
		}
		
		strcpy ((*fileinfo)[n].filename, oi.Filename);
		(*fileinfo)[n].filetype = DIR_TYPE;
		(*fileinfo)[n].storageId = oi.StorageID;
		(*fileinfo)[n].ParentObject = oi.ParentObject;
		(*fileinfo)[n].handle = oh1.Handler[nIndex];
		
		PTP_API_Clear_ObjectInfo (&oi);
		++n;
	}
	
	
	for (nIndex = 0; nIndex < numjpg/*oh2.n*/; nIndex++)
	{
		PTP_API_Init_ObjectInfo(&oi);
		result = ptp_getobjectinfo(&(dc->params), oh2.Handler[nIndex],&oi);
		if ( result != PTP_RC_OK )
		{
			PTP_Debug_Print("[Error] PTP_API_Get_FilesList : ptp_getobjectinfo return(%x)\n", result);
			continue;
		}
		
		if (oi.ObjectFormat != PTP_OFC_EXIF_JPEG) continue;
		
		tmPtr = NULL;
		tmPtr = localtime_r ( (const time_t*)&oi.CaptureDate, &lt);
		if (tmPtr != NULL)
		{
			(*fileinfo)[n].year = tmPtr->tm_year + 1900;
			(*fileinfo)[n].month = tmPtr->tm_mon + 1;
			(*fileinfo)[n].day = tmPtr->tm_mday ;
			(*fileinfo)[n].hour = tmPtr->tm_hour;
			(*fileinfo)[n].min = tmPtr->tm_min;
			(*fileinfo)[n].sec = tmPtr->tm_sec;
		}
		
		strcpy ((*fileinfo)[n].filename, oi.Filename);
		(*fileinfo)[n].filetype = FILE_TYPE;
		(*fileinfo)[n].storageId = oi.StorageID;
		(*fileinfo)[n].ParentObject = oi.ParentObject;
		(*fileinfo)[n].handle = oh2.Handler[nIndex];
		//printf("\nfilename= %s\n",oi.Filename);//vishal
		PTP_API_Clear_ObjectInfo (&oi);
		++n;
	}
	
	// 2007.08.06,  ssu
	// For the error case
	// If any file or folder's association type is not proper, at that time we should count again the real number of files.
	if (*nCount != n)
		*nCount = n;
	
	edTick = Tick() ;
	PTP_Debug_Print ("PTP_API_Get_FilesList : oh1.n = %d, oh2.n = %d, storageId= %x, parent handle = %x, speed = %d(ms)\n", 
		oh1.n, oh2.n, storageID, ParentObjectHandle, (edTick -stTick ));
	
	PTP_API_Clear_ObjectHandles(&oh1);
	PTP_API_Clear_ObjectHandles(&oh2);
	
	PTP_END;
	
	PTP_Debug_Print("PTP_API_Get_FilesList : end (%x)\n\n", result);
	
	return result;	
}



/**
* @brief	      Get JPG file list from the handle (same with PTP_API_Get_FileList)
* @remarks		file list is including folders and jpg files
* @param		uint32 storageID			: storage id
* @param		uint32 ParentObjectHandle 	: handle 
* @param		PTPFileListInfo **fileinfo 	: store of filelist   
* @param		uint32 *nCount 			: the num of files(including folders)   
* @return		uint16
PTP_RC_OK : success, else : failure
*/ 
 uint16 PTP_API_Get_JpegFilesList (uint32 storageID, uint32 ParentObjectHandle, PTPFileListInfo **fileinfo, uint32 *nCount)
 {
#if 0
 	uint32	n = 0;
 	uint32	nIndex = 0;
 	uint16	result = PTP_RC_OK;
 	
 	int	nRet = 0;
 	char	foldername[MAX_FILENAME] = {'0'};
 	char	filename[MAX_FILENAME] = {'0'};
 	
 	struct tm* tmPtr = 0;
 	struct tm lt;
 	
 	PTPObjectInfo		oi;
 	PTPObjectHandles	oh1, oh2;
 	
 	int i = 0;
 	uint32 numjpg = 0;
 	uint32 numdir = 0;
 	
 	if (PTP_API_CHKDEV() != true)
 		return PTPAPI_ERROR; //just representing status error
 	
 	PTP_Init_Transaction (&gdc);
 	
 	PTP_API_Init_ObjectHandles(&oh1);
 	PTP_API_Init_ObjectHandles(&oh2);
 	
 	PTP_Debug_Print("PTP_API_Get_JpegFilesList : start(H:%x)\n", ParentObjectHandle);	
 	
 	PTP_BEGIN;
 	
 	//Find All list from the ParentObjectHandle
 	result = ptp_getobjecthandles (&(gdc.params),storageID, 0x00000000, ParentObjectHandle, &(gdc.params.handles));
 	if (result != PTP_RC_OK)
 	{
 		PTP_END;		
 		return result;
 	}
 	
 	PTP_Debug_Print("The # of handles : %d\n", gdc.params.handles.n);
 	PTP_Debug_Print("Handler:        size: \tname:\n");
 	for (i = 0; i < gdc.params.handles.n; i++) {
 		result = ptp_getobjectinfo(&(gdc.params),gdc.params.handles.Handler[i], &oi);
 		if (result != PTP_RC_OK)
 		{
 			PTP_END;			
 			return result;
 		}
 		
 		if (oi.ObjectFormat == PTP_OFC_Association)
 		{
 			numdir++;
 			continue;
 		}
 		if (oi.ObjectFormat == PTP_OFC_EXIF_JPEG)
 			numjpg++;
 		PTP_Debug_Print("0x%08x: % 9i\t%s\n",gdc.params.handles.Handler[i],
 			oi.ObjectCompressedSize, oi.Filename);
 	}
 	
 	PTP_Debug_Print("the num of dir : %d \n", numdir);
 	PTP_Debug_Print("the num of jpg : %d \n", numjpg);
 	
 	
	// SONY DSC T20ÀÇ °æ¿ì, PTP_OFC_Association cmd·Î Æú´õ¿¡ ´ëÇÑ  handle list ¿äÃ»½Ã device°¡ STALLµÊ
	// ÆÄÀÏ,Æú´õ ¸®½ºÆ® ±¸¼º½Ã NULL °ªÀ» Ã¼Å©ÇØ¼­ handle list ¿äÃ»
 	if(numdir != 0)
 	{
 		result = ptp_getobjecthandles (&(gdc.params), storageID, PTP_OFC_Association, ParentObjectHandle, &(oh1));
 		if( result != PTP_RC_OK) 
 		{
 			PTP_Debug_Print("[Error] PTP_API_Get_JpegFilesList : ptp_getobjecthandles(folder) return (%x)\n", result);
 			PTP_END;		
 			return result;
 		}
 	}
 	
 	if(numjpg != 0)
 	{
 		result = ptp_getobjecthandles (&(gdc.params), storageID, PTP_OFC_EXIF_JPEG, ParentObjectHandle, &(oh2));
 		if( result != PTP_RC_OK) 
 		{
 			PTP_Debug_Print("[Error] PTP_API_Get_JpegFilesList : ptp_getobjecthandles(file) return (%x)\n", result);
 			PTP_END;
 			return result;
 		}	
 	}
 	
 	*nCount = numdir + numjpg; /*oh1.n + oh2.n;*/
 	if (*nCount <= 0)
 	{
 		PTP_Debug_Print("[Error] PTP_API_Get_JpegFilesList : oh1.n + oh2.n is less than 0\n");
 		PTP_END;
 		return PTPAPI_ERROR; // just representing wrong count error		
 	}
 	
 	PTP_Debug_Print ("PTP_API_Get_JpegFilesList : oh1.n = %d, oh2.n = %d, storageId= %x, parent handle = %x\n", oh1.n, oh2.n, storageID, ParentObjectHandle);
 	
 	*fileinfo = (PTPFileListInfo *)calloc (sizeof(PTPFileListInfo), *nCount);
 	if (*fileinfo == NULL)
 	{
 		PTP_Debug_Print("[Error] PTP_API_Get_JpegFilesList : memory alloc error(PTPFileListInfo)\n");
 		PTP_END;
 		return PTPAPI_ERROR; // just representing memory allocation error				
 	}
 	
 	n = 0;
 	for (nIndex = 0; nIndex < numdir/*oh1.n*/; nIndex++) 
 	{
 		PTP_API_Init_ObjectInfo(&oi);
 		result = ptp_getobjectinfo(&(gdc.params), oh1.Handler[nIndex], &oi);
 		if ( result != PTP_RC_OK )
 		{
 			PTP_Debug_Print("[Error] PTP_API_Get_JpegFilesList : ptp_getobjectinfo return(%x)\n", result);
 			continue;
 		}
 		
 		if (oi.ObjectFormat != PTP_OFC_Association) continue;
 		
 		
 		// 2007.08.06,  Kyungsik
// ¹®Á¦Á¡ : µ¿¿µ»ó(*.MOV) ÆÄÀÏÀ» folder·Î ºÐ·ùÇÏ¿© ObjectFormatÀ» folder·Î return 
// ´ëÃ¥ : ObjectFormat(folder,0x3001)ÀÎ °æ¿ì, objectinfoÀÇ  AssociationType(folder,0x0001) ¿©ºÎ¸¦  Ã¼Å©ÇÑ´Ù.
//		ÀÌ °æ¿ì AssociationTypeÀ» Áö¿øÇÏÁö ¾Ê´Â Àåºñ°¡ Á¸ÀçÇÒ ¼ö ÀÖÀ¸¹Ç·Î, AssociationType(undef, 0x0000)µµ È®ÀÎÇØ¾ß ÇÑ´Ù.
// Àåºñ¸í :  Panasonic FX12 
 		if (oi.AssociationType > PTP_AT_GenericFolder)
 		{
 			PTP_Debug_Print("[Error] This file is not folder \n");
 			continue;
 		}
 		
 		strcpy ((*fileinfo)[n].filename, oi.Filename);
 		(*fileinfo)[n].filetype = DIR_TYPE;
 		(*fileinfo)[n].storageId = oi.StorageID;
 		(*fileinfo)[n].ParentObject = oi.ParentObject;
 		(*fileinfo)[n].handle = oh1.Handler[nIndex];
 		
 		PTP_API_Clear_ObjectInfo (&oi);
 		++n;
 	}
 	
 	
 	for (nIndex = 0; nIndex < numjpg/*oh2.n*/; nIndex++)
 	{
 		PTP_API_Init_ObjectInfo(&oi);
 		result = ptp_getobjectinfo(&(gdc.params), oh2.Handler[nIndex],&oi);
 		if ( result != PTP_RC_OK )
 		{
 			PTP_Debug_Print("[Error] PTP_API_Get_JpegFilesList : ptp_getobjectinfo return(%x)\n", result);
 			continue;
 		}
 		
 		if (oi.ObjectFormat != PTP_OFC_EXIF_JPEG) continue;
 		
 		tmPtr = NULL;
 		tmPtr = localtime_r ( (const time_t*)&oi.CaptureDate, &lt);
 		if (tmPtr != NULL)
 		{
 			(*fileinfo)[n].year = tmPtr->tm_year + 1900;
 			(*fileinfo)[n].month = tmPtr->tm_mon + 1;
 			(*fileinfo)[n].day = tmPtr->tm_mday ;
 			(*fileinfo)[n].hour = tmPtr->tm_hour;
 			(*fileinfo)[n].min = tmPtr->tm_min;
 			(*fileinfo)[n].sec = tmPtr->tm_sec;
 		}
 		
 		strcpy ((*fileinfo)[n].filename, oi.Filename);
 		(*fileinfo)[n].filetype = FILE_TYPE;
 		(*fileinfo)[n].storageId = oi.StorageID;
 		(*fileinfo)[n].ParentObject = oi.ParentObject;
 		(*fileinfo)[n].handle = oh2.Handler[nIndex];
 		
 		PTP_API_Clear_ObjectInfo (&oi);
 		++n;
 	}
 	
 	// 2007.08.06,  ssu
 	// For the error case
 	// If any file or folder's association type is not proper, at that time we should count again the real number of files.
 	if (*nCount != n)
 		*nCount = n;
 	
 	PTP_API_Clear_ObjectHandles(&oh1);
 	PTP_API_Clear_ObjectHandles(&oh2);
 	
 	PTP_END;
 	
 	PTP_Debug_Print("PTP_API_Get_JpegFilesList : end (%x)\n\n", result);
 	
 	return result;	
#endif

	//intialising to unused variable
	storageID = 0;
	ParentObjectHandle = 0;
	fileinfo = NULL;
	*nCount = 0;

	return PTPAPI_ERROR;
 }

/**
* @brief	      Get JPG file list from the handle (same with PTP_API_Get_FileList)
* @remarks		file list is including folders and jpg files
* @param		uint32 storageID			: storage id
* @param		uint32 ParentObjectHandle 	: handle 
* @param		PTPFileListInfo **fileinfo 	: store of filelist   
* @param		uint32 *nCount 			: the num of files(including folders)
* @param	      PTPDeviceHandle ptpHandle:	Handle for the given ptp device
* @return		uint16
PTP_RC_OK : success, else : failure
*/ 
uint16 PTP_API_Get_JpegFilesList_MPTP (uint32 storageID, uint32 ParentObjectHandle, PTPFileListInfo **fileinfo, uint32 *nCount,PTPDeviceHandle  ptpHandle)
{
	
	uint32	n = 0;
	uint32	nIndex = 0;
	uint16	result = PTP_RC_OK;
	
	//int	nRet = 0;
	//char	foldername[MAX_FILENAME] = {'0'};
	//char	filename[MAX_FILENAME] = {'0'};
	
	struct tm* tmPtr = 0;
	struct tm lt;
	
	PTPObjectInfo		oi;
	PTPObjectHandles	oh1, oh2;
	
	uint32 i = 0;
	uint32 numjpg = 0;
	uint32 numdir = 0;


	PTPDevContext *dc;
	dc=getDc(ptpHandle);
	if(dc==NULL)
	{
		PTP_Debug_Print("[ERROR] PTP_API_Get_JpegFilesList_MPTP : PTPDevContext is null.\n");
		return PTPAPI_ERROR;//added by Niraj   
	}
	if (PTP_API_CHKDEV_MPTP(ptpHandle) != true)
		return PTPAPI_ERROR; //just representing status error
	
	PTP_Init_Transaction (dc);
	
	PTP_API_Init_ObjectHandles(&oh1);
	PTP_API_Init_ObjectHandles(&oh2);
	
	PTP_Debug_Print("PTP_API_Get_JpegFilesList : start(H:%x)\n", ParentObjectHandle);	
	
	PTP_BEGIN;
	
	//Find All list from the ParentObjectHandle
	result = ptp_getobjecthandles (&(dc->params),storageID, 0x00000000, ParentObjectHandle, &(dc->params.handles));
	
	if (result != PTP_RC_OK)
	{
		PTP_END;		
		return result;
	}
	
	PTP_Debug_Print("The # of handles : %d\n", dc->params.handles.n);
	PTP_Debug_Print("Handler:        size: \tname:\n");
    for (i = 0; i < dc->params.handles.n; i++) 
    {
		result = ptp_getobjectinfo(&(dc->params),dc->params.handles.Handler[i], &oi);
		if (result != PTP_RC_OK)
		{
			PTP_END;			
			return result;
		}
		
		if (oi.ObjectFormat == PTP_OFC_Association)
		{
			numdir++;
			continue;
		}
		if (oi.ObjectFormat == PTP_OFC_EXIF_JPEG)
			numjpg++;
		PTP_Debug_Print("0x%08x: % 9i\t%s\n",dc->params.handles.Handler[i],
			oi.ObjectCompressedSize, oi.Filename);
	}
	
	PTP_Debug_Print("the num of dir : %d \n", numdir);
	PTP_Debug_Print("the num of jpg : %d \n", numjpg);

	
	// SONY DSC T20Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½, PTP_OFC_Association cmdÃ¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½  handle list Ã¯Â¿Â½Ã¯Â¿Â½ÃƒÂ»Ã¯Â¿Â½Ã¯Â¿Â½ deviceÃ¯Â¿Â½Ã¯Â¿Â½ STALLÃ¯Â¿Â½Ã¯Â¿Â½
	// Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½,Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã†Â® Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ NULL Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ ÃƒÂ¼Ã…Â©Ã¯Â¿Â½Ã˜Â¼Ã¯Â¿Â½ handle list Ã¯Â¿Â½Ã¯Â¿Â½ÃƒÂ»
	if(numdir != 0)
	{
		result = ptp_getobjecthandles (&(dc->params), storageID, PTP_OFC_Association, ParentObjectHandle, &(oh1));
		if( result != PTP_RC_OK) 
		{
			PTP_Debug_Print("[Error] PTP_API_Get_JpegFilesList : ptp_getobjecthandles(folder) return (%x)\n", result);
			PTP_END;		
			return result;
		}
	}
	
	if(numjpg != 0)
	{
		result = ptp_getobjecthandles (&(dc->params), storageID, PTP_OFC_EXIF_JPEG, ParentObjectHandle, &(oh2));
		if( result != PTP_RC_OK) 
		{
			PTP_Debug_Print("[Error] PTP_API_Get_JpegFilesList : ptp_getobjecthandles(file) return (%x)\n", result);
			PTP_END;
			return result;
		}	
	}
	
	*nCount = numdir + numjpg; /*oh1.n + oh2.n;*/
	if (*nCount <= 0)
	{
		PTP_Debug_Print("[Error] PTP_API_Get_JpegFilesList : oh1.n + oh2.n is less than 0\n");
		PTP_END;
		return PTPAPI_ERROR; // just representing wrong count error		
	}
	
	PTP_Debug_Print ("PTP_API_Get_JpegFilesList : oh1.n = %d, oh2.n = %d, storageId= %x, parent handle = %x\n", oh1.n, oh2.n, storageID, ParentObjectHandle);
	
	*fileinfo = (PTPFileListInfo *)calloc (sizeof(PTPFileListInfo), *nCount);
	if (*fileinfo == NULL)
	{
		PTP_Debug_Print("[Error] PTP_API_Get_JpegFilesList : memory alloc error(PTPFileListInfo)\n");
		PTP_END;
		return PTPAPI_ERROR; // just representing memory allocation error				
	}
	
	n = 0;
	for (nIndex = 0; nIndex < numdir/*oh1.n*/; nIndex++) 
	{
		PTP_API_Init_ObjectInfo(&oi);
		result = ptp_getobjectinfo(&(dc->params), oh1.Handler[nIndex], &oi);
		if ( result != PTP_RC_OK )
		{
			PTP_Debug_Print("[Error] PTP_API_Get_JpegFilesList : ptp_getobjectinfo return(%x)\n", result);
			continue;
		}
		
		if (oi.ObjectFormat != PTP_OFC_Association) continue;
		
		
		// 2007.08.06,  Kyungsik
		// Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ : Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½(*.MOV) Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ folderÃ¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½ÃÂ·Ã¯Â¿Â½Ã¯Â¿Â½ÃÂ¿Ã¯Â¿Â½ ObjectFormatÃ¯Â¿Â½ folderÃ¯Â¿Â½Ã¯Â¿Â½ return 
		// Ã¯Â¿Â½Ã¯Â¿Â½ÃƒÂ¥ : ObjectFormat(folder,0x3001)Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½, objectinfoÃ¯Â¿Â½Ã¯Â¿Â½  AssociationType(folder,0x0001) Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ÃŽÂ¸Ã¯Â¿Â½  ÃƒÂ¼Ã…Â©Ã¯Â¿Â½Ã‘Â´Ã¯Â¿Â½.
		//		Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ AssociationTypeÃ¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½ÃŠÂ´Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã‡Â·Ã¯Â¿Â½, AssociationType(undef, 0x0000)Ã¯Â¿Â½Ã¯Â¿Â½ ÃˆÂ®Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã˜Â¾Ã¯Â¿Â½ Ã¯Â¿Â½Ã‘Â´Ã¯Â¿Â½.
		// Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ :  Panasonic FX12 
		if (oi.AssociationType > PTP_AT_GenericFolder)
		{
			PTP_Debug_Print("[Error] This file is not folder \n");
			continue;
		}
		
		strcpy ((*fileinfo)[n].filename, oi.Filename);
		(*fileinfo)[n].filetype = DIR_TYPE;
		(*fileinfo)[n].storageId = oi.StorageID;
		(*fileinfo)[n].ParentObject = oi.ParentObject;
		(*fileinfo)[n].handle = oh1.Handler[nIndex];
		
		PTP_API_Clear_ObjectInfo (&oi);
		++n;
	}
	
	
	for (nIndex = 0; nIndex < numjpg/*oh2.n*/; nIndex++)
	{
		PTP_API_Init_ObjectInfo(&oi);
		result = ptp_getobjectinfo(&(dc->params), oh2.Handler[nIndex],&oi);
		if ( result != PTP_RC_OK )
		{
			PTP_Debug_Print("[Error] PTP_API_Get_JpegFilesList : ptp_getobjectinfo return(%x)\n", result);
			continue;
		}
		
		if (oi.ObjectFormat != PTP_OFC_EXIF_JPEG) continue;
		
		tmPtr = NULL;
		tmPtr = localtime_r ( (const time_t*)&oi.CaptureDate, &lt);
		if (tmPtr != NULL)
		{
			(*fileinfo)[n].year = tmPtr->tm_year + 1900;
			(*fileinfo)[n].month = tmPtr->tm_mon + 1;
			(*fileinfo)[n].day = tmPtr->tm_mday ;
			(*fileinfo)[n].hour = tmPtr->tm_hour;
			(*fileinfo)[n].min = tmPtr->tm_min;
			(*fileinfo)[n].sec = tmPtr->tm_sec;
		}
		
		strcpy ((*fileinfo)[n].filename, oi.Filename);
		(*fileinfo)[n].filetype = FILE_TYPE;
		(*fileinfo)[n].storageId = oi.StorageID;
		(*fileinfo)[n].ParentObject = oi.ParentObject;
		(*fileinfo)[n].handle = oh2.Handler[nIndex];
		

		PTP_API_Clear_ObjectInfo (&oi);
		++n;
	}
	
	// 2007.08.06,  ssu
	// For the error case
	// If any file or folder's association type is not proper, at that time we should count again the real number of files.
	if (*nCount != n)
		*nCount = n;
	
	PTP_API_Clear_ObjectHandles(&oh1);
	PTP_API_Clear_ObjectHandles(&oh2);
	
	PTP_END;
	
	PTP_Debug_Print("PTP_API_Get_JpegFilesList : end (%x)\n\n", result);
	
	return result;	
}


/**
* @brief	      Get  jpeg file list from the PTP device (same with PTP_API_Get_FileList)
* @remarks		file list is including folders and jpg files, or jpg files only
* @param		uint32 storageID			: (in) storage id
* @param		PTPObjectHandles *oh1	 	: (out) pointer indicating object handles
* @return		uint16
PTP_RC_OK : success, else : failure
*/
 uint16 PTP_API_Get_JpegObjectHandles (uint32 storageID, PTPObjectHandles *oh1 )
 {
#if 0
 	uint16  result = PTP_RC_OK;
 	int     nRet = 0, i;
 	
 	if (PTP_API_CHKDEV() != true)
 		return PTPAPI_ERROR; //just representing status error
 	
 	PTP_Debug_Print("PTP_API_Get_JPEGObjectHandles : start\n");
 	
 	PTP_BEGIN;
 	
 	//Find all jpeg list from the PTP device
	// ÀÏºÎ PTP ÀåºñÀÇ °æ¿ì(panasonic FX12), jpeg handle ÀÌ¿ÜÀÇ handle À» return ÇÏ¹Ç·Î GetObjectinfo¸¦ 
	// object typeÀ» Ã¼Å©ÇØ¼­ filtering ÇØ¾ßÇÑ´Ù. (by kks)
 	result = ptp_getobjecthandles (&(gdc.params),storageID, PTP_OFC_EXIF_JPEG, PTP_HANDLER_ROOT, oh1);
 	if (result != PTP_RC_OK)
 	{
 		PTP_END;
 		PTP_Debug_Print("PTP_API_Get_JPEGObjectHandles : fail (0x%x)\n",result);
 		return result;
 	}
 	
 	PTP_Debug_Print("PTP_API_Get_JPEGObjectHandles : handles num %d\n", oh1->n);
 	for (i = 0; i < oh1->n; i++)
 	{
 		PTP_Debug_Print("handle [%d] : %x \n", i,oh1->Handler[i]);
 	}
 	
 	PTP_END;
 	
 	PTP_Debug_Print("PTP_API_Get_JPEGObjectHandles : succes (0x%x)\n\n", result);
 	
 	return result;
#endif
	storageID=0;
	oh1=NULL;
	return PTPAPI_ERROR;
 }

/**
* @brief	      Get  list of handles of jpeg objects from the given ptp device
* @remarks		handle is only of jpeg objects and not of folders
* @param		uint32 storageID			: (in) storage id
* @param		PTPObjectHandles *oh1	 	: (out) pointer indicating object handles
* @param	      PTPDeviceHandle ptpHandle:	Handle for the given ptp device
* @return		uint16
PTP_RC_OK : success, else : failure
*/
uint16 PTP_API_Get_JpegObjectHandles_MPTP (uint32 storageID, PTPObjectHandles *oh1, PTPDeviceHandle  ptpHandle)
{
	
	uint16	result = PTP_RC_OK;
	//int	nRet = 0, i;
	uint32 i=0;

	PTPDevContext *dc;
	dc=getDc(ptpHandle);
	if(dc==NULL)
	{
		PTP_Debug_Print("[ERROR] PTP_API_JpegObjectHandles_MPTP : PTPDevContext is null.\n");
		return PTPAPI_ERROR;//added by Niraj   
	}
	if (PTP_API_CHKDEV_MPTP(ptpHandle) != true)
		return PTPAPI_ERROR; //just representing status error
	
	PTP_Debug_Print("PTP_API_Get_JPEGObjectHandles : start\n");	
	
	PTP_BEGIN;
	
 	//Find all jpeg list from the PTP device
	// ÀÏºÎ PTP ÀåºñÀÇ °æ¿ì(panasonic FX12), jpeg handle ÀÌ¿ÜÀÇ handle À» return ÇÏ¹Ç·Î GetObjectinfo¸¦ 
	// object typeÀ» Ã¼Å©ÇØ¼­ filtering ÇØ¾ßÇÑ´Ù. (by kks)	
	result = ptp_getobjecthandles (&(dc->params),storageID, PTP_OFC_EXIF_JPEG, PTP_HANDLER_ROOT, oh1);
	if (result != PTP_RC_OK)
	{
		PTP_END;
		PTP_Debug_Print("PTP_API_Get_JPEGObjectHandles : fail (0x%x)\n",result);
		return result;
	}
	
	PTP_Debug_Print("PTP_API_Get_JPEGObjectHandles : handles num %d\n", oh1->n);
	for (i = 0; i < oh1->n; i++)
	{
		PTP_Debug_Print("handle [%d] : %x \n", i,oh1->Handler[i]);
	}
	
	PTP_END;
	
	PTP_Debug_Print("PTP_API_Get_JPEGObjectHandles : succes (0x%x)\n\n", result);
	
	return result;	
}



/**
* @brief	      Get MP3 file list from the handle
* @remarks		file list is including folders and MP3 files
* @param		uint32 storageID			: storage id
* @param		uint32 ParentObjectHandle 	: handle 
* @param		PTPFileListInfo **fileinfo 	: store of filelist   
* @param		uint32 *nCount 			: the num of files(including folders)   
* @return		uint16
PTP_RC_OK : success, else : failure
*/ 
 uint16 PTP_API_Get_MP3FilesList (uint32 storageID, uint32 ParentObjectHandle, PTPFileListInfo **fileinfo, uint32 *nCount)
 {
#if 0
 	uint32	n = 0;  
 	uint32  nIndex = 0;
 	uint16  result = PTP_RC_OK;
 	
 	int     nRet = 0;
 	char    foldername[MAX_FILENAME] = {'0'};
 	char    filename[MAX_FILENAME] = {'0'};
 	
 	struct tm* tmPtr = 0;
 	struct tm lt;
 	
 	PTPObjectInfo           oi;
 	PTPObjectHandles        oh1, oh2;
 	
 	int i = 0;
 	uint32 nummp3 = 0;
 	uint32 numdir = 0;
 	
 	
 	if (PTP_API_CHKDEV() != true)
 		return PTPAPI_ERROR; //just representing status error
 	
 	PTP_Init_Transaction (&gdc);
 	
 	PTP_API_Init_ObjectHandles(&oh1);
 	PTP_API_Init_ObjectHandles(&oh2);
 	
 	PTP_Debug_Print("PTP_API_Get_MP3FilesList : start(H:%x)\n", ParentObjectHandle);
 	
 	PTP_BEGIN;
 	
 	//Find All list from the ParentObjectHandle
 	result = ptp_getobjecthandles (&(gdc.params),storageID, 0x00000000, ParentObjectHandle, &(gdc.params.handles));
 	if (result != PTP_RC_OK)
 	{
 		PTP_END;
 		return result;
 	}
 	
 	PTP_Debug_Print("The # of handles : %d\n", gdc.params.handles.n);
 	PTP_Debug_Print("Handler:        size: \tname:\n");
 	for (i = 0; i < gdc.params.handles.n; i++) {
 		result = ptp_getobjectinfo(&(gdc.params),gdc.params.handles.Handler[i], &oi);
 		if (result != PTP_RC_OK)
 		{
 			PTP_END;
 			return result;
 		}
 		
 		if (oi.ObjectFormat == PTP_OFC_Association)
 		{
 			numdir++;
 			continue;
 		}
 		
 		if (oi.ObjectFormat == PTP_OFC_MP3)
        {
 			nummp3++;
        }
 		PTP_Debug_Print("0x%08x: % 9i\t%s\n",gdc.params.handles.Handler[i],
 			oi.ObjectCompressedSize, oi.Filename);
 	}
 	
 	PTP_Debug_Print("the num of dir : %d \n", numdir);
 	PTP_Debug_Print("the num of mp3 : %d \n", nummp3);
 	
	// SONY DSC T20ÀÇ °æ¿ì, PTP_OFC_Association cmd·Î Æú´õ¿¡ ´ëÇÑ  handle list ¿äÃ»½Ã device°¡ STALLµÊ
	// ÆÄÀÏ,Æú´õ ¸®½ºÆ® ±¸¼º½Ã NULL °ªÀ» Ã¼Å©ÇØ¼­ handle list ¿äÃ»
 	if(numdir != 0)
 	{
 		result = ptp_getobjecthandles (&(gdc.params), storageID, PTP_OFC_Association, ParentObjectHandle, &(oh1));
 		if( result != PTP_RC_OK)
 		{
 			PTP_Debug_Print("[Error] PTP_API_Get_MP3FilesList : ptp_getobjecthandles(folder) return (%x)\n", result);
 			PTP_END;
 			return result;
 		}
 	}
 	
 	if(nummp3 != 0)
 	{
 		result = ptp_getobjecthandles (&(gdc.params), storageID, PTP_OFC_MP3, ParentObjectHandle, &(oh2));
 		if( result != PTP_RC_OK)
 		{
 			PTP_Debug_Print("[Error] PTP_API_Get_MP3FilesList : ptp_getobjecthandles(file) return (%x)\n", result);
 			PTP_END;
 			return result;
 		}
 	}
 	
 	*nCount = numdir + nummp3; /*oh1.n + oh2.n*/
 	if (*nCount <= 0)
 	{
 		PTP_Debug_Print("[Error] PTP_API_Get_MP3FilesList : oh1.n + oh2.n is less than 0\n");
 		PTP_END;
 		return PTPAPI_ERROR; // just representing wrong count error                             
 	}
 	
 	PTP_Debug_Print ("PTP_API_Get_MP3FilesList : oh1.n = %d, oh2.n = %d, storageId= %x, parent handle = %x\n", oh1.n, oh2.n, storageID, ParentObjectHandle);
 	
 	*fileinfo = (PTPFileListInfo *)calloc (sizeof(PTPFileListInfo), *nCount);
 	if (*fileinfo == NULL)
 	{
 		PTP_Debug_Print("[Error] PTP_API_Get_MP3FilesList : memory alloc error(PTPFileListInfo)\n");
 		PTP_END;
 		return PTPAPI_ERROR; // just representing memory allocation error                               
 	}
 	   n = 0;
 	   for (nIndex = 0; nIndex < numdir/*oh1.n*/; nIndex++)
 	   {
 		   PTP_API_Init_ObjectInfo(&oi);
 		   result = ptp_getobjectinfo(&(gdc.params), oh1.Handler[nIndex], &oi);
 		   if ( result != PTP_RC_OK )
 		   {
 			   PTP_Debug_Print("[Error] PTP_API_Get_MP3FilesList : ptp_getobjectinfo return(%x)\n", result);
 			   continue;
 		   }
 		   
 		   if (oi.ObjectFormat != PTP_OFC_Association) continue;
 		   
 		   // 2007.08.06,  Kyungsik
// ¹®Á¦Á¡ : µ¿¿µ»ó(*.MOV) ÆÄÀÏÀ» folder·Î ºÐ·ùÇÏ¿© ObjectFormatÀ» folder·Î return 
// ´ëÃ¥ : ObjectFormat(folder,0x3001)ÀÎ °æ¿ì, objectinfoÀÇ  AssociationType(folder,0x0001) ¿©ºÎ¸¦  Ã¼Å©ÇÑ´Ù.
//		ÀÌ °æ¿ì AssociationTypeÀ» Áö¿øÇÏÁö ¾Ê´Â Àåºñ°¡ Á¸ÀçÇÒ ¼ö ÀÖÀ¸¹Ç·Î, AssociationType(undef, 0x0000)µµ È®ÀÎÇØ¾ß ÇÑ´Ù.
// Àåºñ¸í :  Panasonic FX12 
 		   if (oi.AssociationType > PTP_AT_GenericFolder)
 		   {
 			   PTP_Debug_Print("[Error] This file is not folder \n");
 			   continue;
 		   }
 		   
 		   strcpy ((*fileinfo)[n].filename, oi.Filename);
 		   (*fileinfo)[n].filetype = DIR_TYPE;
 		   (*fileinfo)[n].storageId = oi.StorageID;
 		   (*fileinfo)[n].ParentObject = oi.ParentObject;
 		   (*fileinfo)[n].handle = oh1.Handler[nIndex];
 		   
 		   PTP_API_Clear_ObjectInfo (&oi);
 		   ++n;
 	   }
 	   
 	   for (nIndex = 0; nIndex < nummp3/*oh2.n*/; nIndex++)
 	   {
 		   PTP_API_Init_ObjectInfo(&oi);
 		   result = ptp_getobjectinfo(&(gdc.params), oh2.Handler[nIndex],&oi);
 		   if ( result != PTP_RC_OK )
 		   {
 			   PTP_Debug_Print("[Error] PTP_API_Get_MP3FilesList : ptp_getobjectinfo return(%x)\n", result);
 			   continue;
 		   }
 		   
 		   if (oi.ObjectFormat != PTP_OFC_MP3) continue;
 		   
 		   tmPtr = NULL;
 		   tmPtr = localtime_r ( (const time_t*)&oi.CaptureDate, &lt);
 		   if (tmPtr != NULL)
 		   {
 			   (*fileinfo)[n].year = tmPtr->tm_year + 1900;
 			   (*fileinfo)[n].month = tmPtr->tm_mon + 1;
 			   (*fileinfo)[n].day = tmPtr->tm_mday ;
 			   (*fileinfo)[n].hour = tmPtr->tm_hour;
 			   (*fileinfo)[n].min = tmPtr->tm_min;
 			   (*fileinfo)[n].sec = tmPtr->tm_sec;
 		   }
 		   
 		   strcpy ((*fileinfo)[n].filename, oi.Filename);
 		   (*fileinfo)[n].filetype = FILE_TYPE;
 		   (*fileinfo)[n].storageId = oi.StorageID;
 		   (*fileinfo)[n].ParentObject = oi.ParentObject;
 		   (*fileinfo)[n].handle = oh2.Handler[nIndex];
 		   
 		   PTP_API_Clear_ObjectInfo (&oi);
 		   ++n;
 	   }
 	   
 	   // 2007.08.06,  ssu
 	   // For the error case
 	   // If any file or folder's association type is not proper, at that time we should count again the real number of files.
 	   if (*nCount != n)
 		   *nCount = n;
 	   
 	   PTP_API_Clear_ObjectHandles(&oh1);
 	   PTP_API_Clear_ObjectHandles(&oh2);
 	   
 	   PTP_END;
 	   
 	   PTP_Debug_Print("PTP_API_Get_MP3FilesList : end (%x)\n\n", result);
 	   
 	   return result;
   #endif

	//intialising unused variable
	storageID = 0;	
	ParentObjectHandle = 0;
	fileinfo = NULL;
	*nCount = 0;

	return PTPAPI_ERROR;
 }


/**
* @brief	      Get MP3 file list from the given ptp device
* @remarks		file list is including folders and MP3 files
* @param		uint32 storageID			: storage id
* @param		uint32 ParentObjectHandle 	: handle 
* @param		PTPFileListInfo **fileinfo 	: store of filelist   
* @param		uint32 *nCount 			: the num of files(including folders)   
* @param	      PTPDeviceHandle ptpHandle:	Handle for the given ptp device
* @return		uint16
PTP_RC_OK : success, else : failure
*/ 
uint16 PTP_API_Get_MP3FilesList_MPTP (uint32 storageID, uint32 ParentObjectHandle, PTPFileListInfo **fileinfo, uint32 *nCount, PTPDeviceHandle  ptpHandle)

{
	
	uint32	n = 0;
	uint32	nIndex = 0;
	uint16	result = PTP_RC_OK;
	
	//int	nRet = 0;
	//char	foldername[MAX_FILENAME] = {'0'};
	//char	filename[MAX_FILENAME] = {'0'};
	
	struct tm* tmPtr = 0;
	struct tm lt;
	
	PTPObjectInfo		oi;
	PTPObjectHandles	oh1, oh2;
	
	uint32 i = 0;
	uint32 nummp3 = 0;
	uint32 numdir = 0;

	PTPDevContext *dc;
	dc=getDc(ptpHandle);
	if(dc==NULL)
	{
		PTP_Debug_Print("[ERROR] PTP_API_Get_MPTFilesList_MPTP : PTPDevContext is null.\n");
		return PTPAPI_ERROR;//added by Niraj   
	}
	if (PTP_API_CHKDEV_MPTP(ptpHandle) != true)
		return PTPAPI_ERROR; //just representing status error
	
	PTP_Init_Transaction (dc);
	
	PTP_API_Init_ObjectHandles(&oh1);
	PTP_API_Init_ObjectHandles(&oh2);
	
	PTP_Debug_Print("PTP_API_Get_MP3FilesList : start(H:%x)\n", ParentObjectHandle);
	
	PTP_BEGIN;
	
	//Find All list from the ParentObjectHandle
	result = ptp_getobjecthandles (&(dc->params),storageID, 0x00000000, ParentObjectHandle, &(dc->params.handles));
	if (result != PTP_RC_OK)
	{
		PTP_END;
		return result;
	}
	
	PTP_Debug_Print("The # of handles : %d\n", dc->params.handles.n);
	PTP_Debug_Print("Handler:        size: \tname:\n");
    for (i = 0; i < dc->params.handles.n; i++) 
    {
		result = ptp_getobjectinfo(&(dc->params),dc->params.handles.Handler[i], &oi);
		if (result != PTP_RC_OK)
		{
			PTP_END;
			return result;
		}
		
		if (oi.ObjectFormat == PTP_OFC_Association)
		{
			numdir++;
			continue;
		}
		if (oi.ObjectFormat == PTP_OFC_MP3)
        {
			nummp3++;
        }
		PTP_Debug_Print("0x%08x: % 9i\t%s\n",dc->params.handles.Handler[i],
			oi.ObjectCompressedSize, oi.Filename);
	}
	
	PTP_Debug_Print("the num of dir : %d \n", numdir);
	PTP_Debug_Print("the num of mp3 : %d \n", nummp3);	
	
	// SONY DSC T20Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½, PTP_OFC_Association cmdÃ¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½  handle list Ã¯Â¿Â½Ã¯Â¿Â½ÃƒÂ»Ã¯Â¿Â½Ã¯Â¿Â½ deviceÃ¯Â¿Â½Ã¯Â¿Â½ STALLÃ¯Â¿Â½Ã¯Â¿Â½
	// Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½,Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã†Â® Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ NULL Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ ÃƒÂ¼Ã…Â©Ã¯Â¿Â½Ã˜Â¼Ã¯Â¿Â½ handle list Ã¯Â¿Â½Ã¯Â¿Â½ÃƒÂ»
	if(numdir != 0)
	{
		result = ptp_getobjecthandles (&(dc->params), storageID, PTP_OFC_Association, ParentObjectHandle, &(oh1));
		if( result != PTP_RC_OK) 
		{
			PTP_Debug_Print("[Error] PTP_API_Get_MP3FilesList : ptp_getobjecthandles(folder) return (%x)\n", result);
			PTP_END;
			return result;
		}	
	}
	
	if(nummp3 != 0)
	{
		result = ptp_getobjecthandles (&(dc->params), storageID, PTP_OFC_MP3, ParentObjectHandle, &(oh2));
		if( result != PTP_RC_OK) 
		{
			PTP_Debug_Print("[Error] PTP_API_Get_MP3FilesList : ptp_getobjecthandles(file) return (%x)\n", result);
			PTP_END;
			return result;
		}	
	}
	
	*nCount = numdir + nummp3; /*oh1.n + oh2.n*/
	if (*nCount <= 0)
	{
		PTP_Debug_Print("[Error] PTP_API_Get_MP3FilesList : oh1.n + oh2.n is less than 0\n");
		PTP_END;
		return PTPAPI_ERROR; // just representing wrong count error				
	}
	
	PTP_Debug_Print ("PTP_API_Get_MP3FilesList : oh1.n = %d, oh2.n = %d, storageId= %x, parent handle = %x\n", oh1.n, oh2.n, storageID, ParentObjectHandle);
	
	*fileinfo = (PTPFileListInfo *)calloc (sizeof(PTPFileListInfo), *nCount);
	if (*fileinfo == NULL)
	{
		PTP_Debug_Print("[Error] PTP_API_Get_MP3FilesList : memory alloc error(PTPFileListInfo)\n");
		PTP_END;
		return PTPAPI_ERROR; // just representing memory allocation error				
	}
	
	n = 0;
	for (nIndex = 0; nIndex < numdir/*oh1.n*/; nIndex++) 
	{
		PTP_API_Init_ObjectInfo(&oi);
		result = ptp_getobjectinfo(&(dc->params), oh1.Handler[nIndex], &oi);
		if ( result != PTP_RC_OK )
		{
			PTP_Debug_Print("[Error] PTP_API_Get_MP3FilesList : ptp_getobjectinfo return(%x)\n", result);
			continue;
		}
		
		if (oi.ObjectFormat != PTP_OFC_Association) continue;
		
		// 2007.08.06,  Kyungsik
		// Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ : Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½(*.MOV) Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ folderÃ¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½ÃÂ·Ã¯Â¿Â½Ã¯Â¿Â½ÃÂ¿Ã¯Â¿Â½ ObjectFormatÃ¯Â¿Â½ folderÃ¯Â¿Â½Ã¯Â¿Â½ return 
		// Ã¯Â¿Â½Ã¯Â¿Â½ÃƒÂ¥ : ObjectFormat(folder,0x3001)Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½, objectinfoÃ¯Â¿Â½Ã¯Â¿Â½  AssociationType(folder,0x0001) Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ÃŽÂ¸Ã¯Â¿Â½  ÃƒÂ¼Ã…Â©Ã¯Â¿Â½Ã‘Â´Ã¯Â¿Â½.
		//		Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ AssociationTypeÃ¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½ÃŠÂ´Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½ Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã‡Â·Ã¯Â¿Â½, AssociationType(undef, 0x0000)Ã¯Â¿Â½Ã¯Â¿Â½ ÃˆÂ®Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã˜Â¾Ã¯Â¿Â½ Ã¯Â¿Â½Ã‘Â´Ã¯Â¿Â½.
		// Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½Ã¯Â¿Â½ :  Panasonic FX12 
		if (oi.AssociationType > PTP_AT_GenericFolder)
		{
			PTP_Debug_Print("[Error] This file is not folder \n");
			continue;
		}
		
		strcpy ((*fileinfo)[n].filename, oi.Filename);
		(*fileinfo)[n].filetype = DIR_TYPE;
		(*fileinfo)[n].storageId = oi.StorageID;
		(*fileinfo)[n].ParentObject = oi.ParentObject;
		(*fileinfo)[n].handle = oh1.Handler[nIndex];
		
		PTP_API_Clear_ObjectInfo (&oi);
		++n;
	}
	
	for (nIndex = 0; nIndex < nummp3/*oh2.n*/; nIndex++)
	{
		PTP_API_Init_ObjectInfo(&oi);
		result = ptp_getobjectinfo(&(dc->params), oh2.Handler[nIndex],&oi);
		if ( result != PTP_RC_OK )
		{
			PTP_Debug_Print("[Error] PTP_API_Get_MP3FilesList : ptp_getobjectinfo return(%x)\n", result);
			continue;
		}
		
		if (oi.ObjectFormat != PTP_OFC_MP3) continue;
		
		tmPtr = NULL;
		tmPtr = localtime_r ( (const time_t*)&oi.CaptureDate, &lt);
		if (tmPtr != NULL)
		{
			(*fileinfo)[n].year = tmPtr->tm_year + 1900;
			(*fileinfo)[n].month = tmPtr->tm_mon + 1;
			(*fileinfo)[n].day = tmPtr->tm_mday ;
			(*fileinfo)[n].hour = tmPtr->tm_hour;
			(*fileinfo)[n].min = tmPtr->tm_min;
			(*fileinfo)[n].sec = tmPtr->tm_sec;
		}
		
		strcpy ((*fileinfo)[n].filename, oi.Filename);
		(*fileinfo)[n].filetype = FILE_TYPE;
		(*fileinfo)[n].storageId = oi.StorageID;
		(*fileinfo)[n].ParentObject = oi.ParentObject;
		(*fileinfo)[n].handle = oh2.Handler[nIndex];
		
		PTP_API_Clear_ObjectInfo (&oi);
		++n;
	}
	
	// 2007.08.06,  ssu
	// For the error case
	// If any file or folder's association type is not proper, at that time we should count again the real number of files.
	if (*nCount != n)
		*nCount = n;
	
	PTP_API_Clear_ObjectHandles(&oh1);
	PTP_API_Clear_ObjectHandles(&oh2);
	
	PTP_END;
	
	PTP_Debug_Print("PTP_API_Get_MP3FilesList : end (%x)\n\n", result);
	
	return result;	
}

/**
* @brief	      Get parent handle 
* @remarks		none
* @param		uint32 ObjectHandle		: handle
* @param		uint32 *ParentHandle	 	: pointer indicating parent handle 
* @return		uint16
PTP_RC_OK : success, else : failure
*/ 
 uint16 PTP_API_Get_ParentHandle (uint32 ObjectHandle, uint32 *ParentHandle)
 {
#if 0
 	PTPObjectInfo   oi;
 	uint16  result = PTP_RC_OK;
 	
 	if (PTP_API_CHKDEV() != true)
 		return PTPAPI_ERROR; //just representing status error
 #ifndef OPTIMIZED_PTP
 	if (!PTP_API_Is_Handle_Vaild(ObjectHandle))
 		return PTPAPI_ERROR; //just representing handle error           
 #endif
 	PTP_Init_Transaction (&gdc);
 	
 	PTP_API_Init_ObjectInfo(&oi);
 	PTP_Debug_Print("PTP_API_Get_ParentHandle : start\n");
 	
 	PTP_BEGIN;
 	
 	result = ptp_getobjectinfo(&(gdc.params), ObjectHandle, &oi );
 	if ( result != PTP_RC_OK )
 	{
 		PTP_Debug_Print("[Error] PTP_API_Get_ParentHandle : ptp_getobjectinfo return(%x)\n", result);
 		PTP_END;
 		return result;
 	}
 	
 	*ParentHandle = oi.ParentObject;
 	PTP_Debug_Print (" PTP_API_Get_ParentHandle: handle = %x,  parent handle = %x\n", ObjectHandle, *ParentHandle);
 	PTP_API_Clear_ObjectInfo(&oi);
 	
 	PTP_END;
 	
 	PTP_Debug_Print("PTP_API_Get_ParentHandle : end(%x)\n\n", result);
 	
 	return result;
#endif

	// intialising unused arguments
	ObjectHandle = 0;
	*ParentHandle = 0;

	return PTPAPI_ERROR;
 	
 }

/**
* @brief	      Get parent handle of given object of the given ptp device
* @remarks		none
* @param		uint32 ObjectHandle		: handle
* @param		uint32 *ParentHandle	 	: pointer indicating parent handle 
* @param	      PTPDeviceHandle ptpHandle:	Handle for the given ptp device
* @return		uint16
PTP_RC_OK : success, else : failure
*/ 
uint16 PTP_API_Get_ParentHandle_MPTP (uint32 ObjectHandle, uint32 *ParentHandle, PTPDeviceHandle  ptpHandle)
{
	
	PTPObjectInfo 	oi;
	uint16	result = PTP_RC_OK;

	PTPDevContext *dc;
	dc=getDc(ptpHandle);
	if(dc==NULL)
	{
		PTP_Debug_Print("[ERROR] PTP_API_Get_ParentHandle_MPTP : PTPDevContext is null.\n");
		return PTPAPI_ERROR;//added by Niraj   
	}
	if (PTP_API_CHKDEV_MPTP(ptpHandle) != true)
		return PTPAPI_ERROR; //just representing status error
#ifndef OPTIMIZED_PTP
	if (!PTP_API_Is_Handle_Vaild_MPTP(ObjectHandle, ptpHandle))
		return PTPAPI_ERROR; //just representing handle error		
#endif
	PTP_Init_Transaction (dc);
	
	PTP_API_Init_ObjectInfo(&oi);
	PTP_Debug_Print("PTP_API_Get_ParentHandle : start\n");
	
	PTP_BEGIN;	
	
	result = ptp_getobjectinfo(&(dc->params), ObjectHandle, &oi );
	if ( result != PTP_RC_OK )
	{
		PTP_Debug_Print("[Error] PTP_API_Get_ParentHandle : ptp_getobjectinfo return(%x)\n", result);
		PTP_END;
		return result;
	}
	
	*ParentHandle = oi.ParentObject;
	PTP_Debug_Print (" PTP_API_Get_ParentHandle: handle = %x,  parent handle = %x\n", ObjectHandle, *ParentHandle);
	PTP_API_Clear_ObjectInfo(&oi);
	
	PTP_END;
	
	PTP_Debug_Print("PTP_API_Get_ParentHandle : end(%x)\n\n", result);
	
	return result;	
}

/**
* @brief	      Get Directory name from a handle
* @remarks		none
* @param		uint32 ObjectHandle		: handle
* @param		char *ParentDir		 	: pointer indicating parent directory
* @return		uint16
PTP_RC_OK : success, else : failure
*/ 
 uint16 PTP_API_Get_DirName (uint32 ObjectHandle, char *ParentDir)
 {
#if 0
 	PTPObjectInfo   oi;
 	uint16  result = PTP_RC_OK;
 	
 	if (PTP_API_CHKDEV() != true)
 		return PTPAPI_ERROR; //just representing status error
 #ifndef OPTIMIZED_PTP
 	if (!PTP_API_Is_Handle_Vaild(ObjectHandle))
 		return PTPAPI_ERROR; //just representing handle error           
 #endif
 	PTP_Init_Transaction (&gdc);
 	
 	PTP_API_Init_ObjectInfo(&oi);
 	PTP_Debug_Print("PTP_API_Get_DirName : start(H:%x)\n", ObjectHandle);
 	
 	PTP_BEGIN;
 	
 	result = ptp_getobjectinfo(&(gdc.params), ObjectHandle, &oi );
 	if ( result != PTP_RC_OK )
 	{
 		PTP_Debug_Print("[Error] PTP_API_Get_ParentHandle : ptp_getobjectinfo return(%x)\n", result);
 		PTP_END;
 		return result;
 	}
 	
 	strcpy(ParentDir, oi.Filename);
 	
 	PTP_Debug_Print (" PTP_API_Get_ParentHandle : handle = %x,  parent name = %s\n", ObjectHandle, ParentDir);
 	PTP_API_Clear_ObjectInfo(&oi);
 	
 	PTP_END;
 	
 	PTP_Debug_Print("PTP_API_Get_DirName : end(%x)\n\n", result);
 	
 	return result;
#endif

	// intialising unused arguments
	ObjectHandle = 0;
	*ParentDir = '\0';
	
	return PTPAPI_ERROR;
 	
 }

/**
* @brief	      Get Directory name from a handle for the given ptp device
* @remarks		none
* @param		uint32 ObjectHandle		: handle
* @param		char *ParentDir		 	: pointer indicating parent directory
* @param	      PTPDeviceHandle ptpHandle:	Handle for the given ptp device
* @return		uint16
PTP_RC_OK : success, else : failure
*/ 
uint16 PTP_API_Get_DirName_MPTP (uint32 ObjectHandle, char *ParentDir, PTPDeviceHandle  ptpHandle)
{
	
	PTPObjectInfo 	oi;
	uint16	result = PTP_RC_OK;

	PTPDevContext *dc;
	dc=getDc(ptpHandle);
	if(dc==NULL)
	{
		PTP_Debug_Print("[ERROR] PTP_API_Get_DirName_MPTP : PTPDevContext is null.\n");
		return PTPAPI_ERROR;//added by Niraj   
	}
	if (PTP_API_CHKDEV_MPTP(ptpHandle)  != true)
		return PTPAPI_ERROR; //just representing status error
#ifndef OPTIMIZED_PTP
	if (!PTP_API_Is_Handle_Vaild_MPTP(ObjectHandle, ptpHandle))
		return PTPAPI_ERROR; //just representing handle error		
#endif
	PTP_Init_Transaction (dc);
	
	PTP_API_Init_ObjectInfo(&oi);
	PTP_Debug_Print("PTP_API_Get_DirName : start(H:%x)\n", ObjectHandle);
	
	PTP_BEGIN;
	
	result = ptp_getobjectinfo(&(dc->params), ObjectHandle, &oi );
	if ( result != PTP_RC_OK )
	{
		PTP_Debug_Print("[Error] PTP_API_Get_ParentHandle : ptp_getobjectinfo return(%x)\n", result);
		PTP_END;
		return result;
	}
	
	strcpy(ParentDir, oi.Filename);
	
	PTP_Debug_Print (" PTP_API_Get_ParentHandle : handle = %x,  parent name = %s\n", ObjectHandle, ParentDir);
	PTP_API_Clear_ObjectInfo(&oi);
	
	PTP_END;
	
	PTP_Debug_Print("PTP_API_Get_DirName : end(%x)\n\n", result);
	
	return result;	
}

/**
* @brief	      Get object information from a handle
* @remarks		none
* @param		uint32 ObjectHandle		: handle
* @param		PTPObjectInfo *oi		 	: pointer indicating object information
* @return		uint16
PTP_RC_OK : success, else : failure
*/ 
 uint16 PTP_API_Get_ObjectInfo (uint32 ObjectHandle, PTPObjectInfo *oi)
 {
#if 0	
 	uint16	result = PTP_RC_OK;
 	
 	if (PTP_API_CHKDEV() != true)
 		return PTPAPI_ERROR; //just representing status error
 #ifndef OPTIMIZED_PTP
 	if (!PTP_API_Is_Handle_Vaild(ObjectHandle))
 		return PTPAPI_ERROR; //just representing handle error
 #endif
 	PTP_Init_Transaction (&gdc);
 	
 	PTP_Debug_Print("PTP_API_Get_ObjectInfo : start (H:%x)\n", ObjectHandle);
 	
 	PTP_BEGIN;
 	
 	result = ptp_getobjectinfo(&(gdc.params), ObjectHandle, oi );
 	if ( result != PTP_RC_OK )
 	{
 		PTP_Debug_Print("[Error] PTP_API_Get_ObjectInfo : ptp_getobjectinfo return(%x)\n", result);
 		PTP_END;
 		return result;
 	}
 	
 	if ((oi->ObjectFormat != PTP_OFC_Association) && (oi->ObjectFormat != PTP_OFC_EXIF_JPEG)) 
 	{
 		PTP_Debug_Print("[Error] PTP_API_Get_ObjectInfo : invalid object format (%x)\n", oi->ObjectFormat); 
 		PTP_END;
 		return PTPAPI_ERROR; //just representing format error
 	}
 	
 	PTP_Debug_Print("0x%08x: % 9i\t%16s\t0x%08x\t0x%04x\t0x%08x\n", ObjectHandle,	\
 		oi->ObjectCompressedSize, oi->Filename,			\
 		oi->StorageID, oi->ObjectFormat, oi->ParentObject);
 	
 	PTP_END;
 	
 	PTP_Debug_Print("PTP_API_Get_ObjectInfo : end(%x)\n\n", result);
 	
 	return result;	
#endif

	// intialising unused arguments
	ObjectHandle = 0;
	oi = NULL;

	return PTPAPI_ERROR;
 	
 }

/**
* @brief	      Get object information from a handle for the given ptp device
* @remarks		none
* @param		uint32 ObjectHandle		: handle
* @param		PTPObjectInfo *oi		 	: pointer indicating object information
* @param	      PTPDeviceHandle ptpHandle:	Handle for the given ptp device
* @return		uint16
PTP_RC_OK : success, else : failure
*/ 
uint16 PTP_API_Get_ObjectInfo_MPTP (uint32 ObjectHandle, PTPObjectInfo *oi, PTPDeviceHandle  ptpHandle)
{	
	uint16	result = PTP_RC_OK;
	PTPDevContext *dc;

	dc = getDc(ptpHandle);
	if(dc == NULL)
	{
		PTP_Debug_Print("[ERROR] PTP_API_Get_ObjectInfo_MPTP : PTPDevContext is null.\n");
		return PTPAPI_ERROR;
	}

	if (PTP_API_CHKDEV_MPTP(ptpHandle) != true)
		return PTPAPI_ERROR; //just representing status error

#ifndef OPTIMIZED_PTP
	if (!PTP_API_Is_Handle_Vaild_MPTP(ObjectHandle, ptpHandle)) ///TBD: to be reflected 
		return PTPAPI_ERROR; //just representing handle error
#endif

	PTP_Init_Transaction (dc);
	
	PTP_Debug_Print("PTP_API_Get_ObjectInfo : start (H:%x)\n", ObjectHandle);
	
	PTP_BEGIN;
	
	result = ptp_getobjectinfo(&(dc->params), ObjectHandle, oi );
	if ( result != PTP_RC_OK )
	{
		PTP_Debug_Print("[Error] PTP_API_Get_ObjectInfo : ptp_getobjectinfo return(%x)\n", result);
		PTP_END;
		return result;
	}
	
	if ((oi->ObjectFormat != PTP_OFC_Association) && (oi->ObjectFormat != PTP_OFC_EXIF_JPEG)) 
	{
		PTP_Debug_Print("[Error] PTP_API_Get_ObjectInfo : invalid object format (%x)\n", oi->ObjectFormat); 
		PTP_END;
		return PTPAPI_ERROR; //just representing format error
	}
	
	PTP_Debug_Print("0x%08x: % 9i\t%16s\t0x%08x\t0x%04x\t0x%08x\n", ObjectHandle,	\
		oi->ObjectCompressedSize, oi->Filename,			\
		oi->StorageID, oi->ObjectFormat, oi->ParentObject);
	
	PTP_END;
	
	PTP_Debug_Print("PTP_API_Get_ObjectInfo : end(%x)\n\n", result);
	
	return result;	
}

/**
* @brief	      Get numbers of objects below than current level (handle)
* @remarks		extingushed by object format
* @param		uint32 storageID			: stroage id
* @param		uint32 ParentObjectHandle	: handle
* @param		uint32 ObjectFormat		: ObjectFormat  
* @param		uint32 *nNum				: number of objects
* @return		uint16
PTP_RC_OK : success, else : failure
*/ 
 uint16 PTP_API_Get_NumOfObjects(uint32 storageID, uint32 ParentObjectHandle, uint32 ObjectFormat, uint32 *nNum)
 {
#if 0
 	PTPObjectInfo           oi;
 	
 	uint16  result = PTP_RC_OK;
 	uint32  nIndex = 0;
 	uint32  nRecordIndex = 0;
 	
 	if (PTP_API_CHKDEV() != true)
 		return PTPAPI_ERROR; //just representing status error
 	
 	PTP_Init_Transaction (&gdc);
 	
 	*nNum = 0;
 	
 	PTP_Debug_Print("PTP_API_Get_NumOfObjects : start(H:%d)\n", ParentObjectHandle);
 	
 	PTP_BEGIN;
 	
 	result = ptp_GetNumObjects (&(gdc.params), storageID, ObjectFormat, ParentObjectHandle, (uint32_t*)nNum);
 	if( result != PTP_RC_OK)
 	{
 		PTP_Debug_Print("[Error] PTP_API_Get_NumOfObjects : ptp_GetNumObjects return (0x%04x)\n", result);
 		PTP_END;
 		return result;
 	}
 	PTP_Debug_Print("PTP_API_Get_NumOfObjects : ptp_GetNumObjects return(%x)\n", *nNum);
 	
 	PTP_END;
 	
 	PTP_Debug_Print("PTP_API_Get_NumOfObjects : end(%x)\n\n", result);
 	
 	return result;
#endif

	// intialising unused arguments
	storageID = 0;
	ParentObjectHandle = 0;
	ObjectFormat = 0;
	*nNum = 0;

	return PTPAPI_ERROR;
 	
 }

/**
* @brief	      Get numbers of objects below than current level (handle) for the given ptp device
* @remarks		objects of the given format will be selected
* @param		uint32 storageID			: stroage id
* @param		uint32 ParentObjectHandle	: handle
* @param		uint32 ObjectFormat		: ObjectFormat  
* @param		uint32 *nNum				: number of objects
* @param	      PTPDeviceHandle ptpHandle:	Handle for the given ptp device
* @return		uint16
PTP_RC_OK : success, else : failure
*/
uint16 PTP_API_Get_NumOfObjects_MPTP (uint32 storageID, uint32 ParentObjectHandle, uint32 ObjectFormat, uint32 *nNum, PTPDeviceHandle  ptpHandle)
{
	uint16	result = PTP_RC_OK;

	PTPDevContext *dc;
	dc=getDc(ptpHandle);
	if(dc==NULL)
	{
		PTP_Debug_Print("[ERROR] PTP_API_Get_NumOfObjects_MPTP : PTPDevContext is null.\n");
		return PTPAPI_ERROR;//added by Niraj   
	}
	if (PTP_API_CHKDEV_MPTP(ptpHandle) != true)
		return PTPAPI_ERROR; //just representing status error
	
	PTP_Init_Transaction (dc);
	
	*nNum = 0;
	
	PTP_Debug_Print("PTP_API_Get_NumOfObjects : start(H:%d)\n", ParentObjectHandle);
	
	PTP_BEGIN;
	
	result = ptp_GetNumObjects (&(dc->params), storageID, ObjectFormat, ParentObjectHandle, (uint32_t*)nNum);
	
	if( result != PTP_RC_OK) 
	{
		PTP_Debug_Print("[Error] PTP_API_Get_NumOfObjects : ptp_GetNumObjects return (0x%04x)\n", result);
		PTP_END;
		return result;
	}
	PTP_Debug_Print("PTP_API_Get_NumOfObjects : ptp_GetNumObjects return(%x)\n", *nNum);
	
	PTP_END;
	
	PTP_Debug_Print("PTP_API_Get_NumOfObjects : end(%x)\n\n", result);
	
	return result;	
}

/**
* @brief	      Get numbers of objects below than current level (handle)
* @remarks		extingushed by format (folder, jpg)
* @param		uint32 storageID			: stroage id
* @param		uint32 ParentObjectHandle	: handle
* @param		uint32 *nNum				: number of objects
* @return		uint16
PTP_RC_OK : success, else : failure
*/ 
 uint16 PTP_API_Get_NumOfList(uint32 storageID, uint32 ParentObjectHandle, uint32 *nNum)
 {
#if 0
 	PTPObjectInfo           oi;
 	
 	uint16  result = PTP_RC_OK;
 	uint32  nIndex = 0;
 	uint32  nNum1 =0, nNum2 =0;
 	uint32  nRecordIndex = 0;
 	
 	if (PTP_API_CHKDEV() != true)
 		return PTPAPI_ERROR; //just representing status error
 	
 	PTP_Init_Transaction (&gdc);
 	
 	*nNum = 0;
 	
 	PTP_Debug_Print("PTP_API_Get_NumOfList : start(H:%x)\n", ParentObjectHandle);
 	
 	PTP_BEGIN;
 	
 	
 	// 0xFFFFFFFF : root
 	result = ptp_GetNumObjects (&(gdc.params), storageID, PTP_OFC_Association, ParentObjectHandle,(uint32_t * )&nNum1);
 	if( result != PTP_RC_OK)
 	{
 		PTP_Debug_Print("[Error] PTP_API_Get_NumOfList : ptp_GetNumObjects(dir) return (0x%04x)\n", result);
 		PTP_END;
 		return result;
 	}
 	PTP_Debug_Print("PTP_API_Get_NumOfList : ptp_GetNumObjects return(dir:%x)\n", nNum1);
 	
 	result = ptp_GetNumObjects (&(gdc.params), storageID, PTP_OFC_EXIF_JPEG, ParentObjectHandle, (uint32_t * )&nNum2);
 	if( result != PTP_RC_OK)
 	{
 		PTP_Debug_Print("[Error] PTP_API_Get_NumOfList : ptp_GetNumObjects(jpeg) return (0x%04x)\n", result);
 		PTP_END;
 		return result;
 	}
 	PTP_Debug_Print("PTP_API_Get_NumOfList : ptp_GetNumObjects return(file:%x)\n", nNum2);
 	*nNum = nNum1 + nNum2;
 	
 	PTP_END;
 	
 	PTP_Debug_Print("PTP_API_Get_NumOfList : end(%x), sum(%x)\n\n", result, *nNum);
 	
 	return result;
#endif

	// intialising unused arguments
	storageID = 0;
	ParentObjectHandle = 0;
	*nNum = 0;

	return PTPAPI_ERROR; 	
 }

/**
* @brief	      Get numbers of objects below than current level (handle) for the given ptp device
* @remarks		all the objects irrespective of format will be selected
* @param		uint32 storageID			: stroage id
* @param		uint32 ParentObjectHandle	: handle
* @param		uint32 *nNum				: number of objects
* @param	      PTPDeviceHandle ptpHandle:	Handle for the given ptp device
* @return		uint16
PTP_RC_OK : success, else : failure
*/ 
uint16 PTP_API_Get_NumOfList_MPTP(uint32 storageID, uint32 ParentObjectHandle, uint32 *nNum, PTPDeviceHandle  ptpHandle)
{
	uint16	result = PTP_RC_OK;

	uint32_t	nNum1 =0, nNum2 =0;

	PTPDevContext *dc;

	dc=getDc(ptpHandle);
	if(dc==NULL)
	{
		PTP_Debug_Print("[ERROR] PTP_API_Get_NumOfList_MPTP : PTPDevContext is null.\n");
		return PTPAPI_ERROR;//added by Niraj   
	}
	if (PTP_API_CHKDEV_MPTP(ptpHandle) != true)
		return PTPAPI_ERROR; //just representing status error
	
	PTP_Init_Transaction (dc);
	
	*nNum = 0;
	
	PTP_Debug_Print("PTP_API_Get_NumOfList : start(H:%x)\n", ParentObjectHandle);
	
	PTP_BEGIN;
	
	
	// 0xFFFFFFFF : root
	result = ptp_GetNumObjects (&(dc->params), storageID, PTP_OFC_Association, ParentObjectHandle,(uint32_t * )&nNum1);
	
	if( result != PTP_RC_OK) 
	{
		PTP_Debug_Print("[Error] PTP_API_Get_NumOfList : ptp_GetNumObjects(dir) return (0x%04x)\n", result);
		PTP_END;
		return result;
	}	
	PTP_Debug_Print("PTP_API_Get_NumOfList : ptp_GetNumObjects return(dir:%x)\n", nNum1);
	
	result = ptp_GetNumObjects (&(dc->params), storageID, PTP_OFC_EXIF_JPEG, ParentObjectHandle, (uint32_t * )&nNum2);
	
	if( result != PTP_RC_OK) 
	{
		PTP_Debug_Print("[Error] PTP_API_Get_NumOfList : ptp_GetNumObjects(jpeg) return (0x%04x)\n", result);
		PTP_END;		
		return result;
	}	
	PTP_Debug_Print("PTP_API_Get_NumOfList : ptp_GetNumObjects return(file:%x)\n", nNum2);
	*nNum = nNum1 + nNum2;
	
	PTP_END;
	
	PTP_Debug_Print("PTP_API_Get_NumOfList : end(%x), sum(%x)\n\n", result, *nNum);
	
	return result;	
}

/**
* @brief	      Get thumbnail image
* @remarks		none
* @param		uint32 handle				: handle
* @param		char **Image				: the store of an image
* @param		uint32 *uint32 *ImageSize	: Compressed size of an image
* @return		uint16
PTP_RC_OK : success, else : failure
*/ 
 uint16 PTP_API_Get_Thumbnail(uint32 handle, char **Image, uint32 *ImageSize)
 {
#if 0
 	PTPObjectInfo           oi;
 	unsigned long stTick = 0;
 	unsigned long edTick = 0;
 	
 	uint16  result = PTP_RC_OK;
 	
 	if (PTP_API_CHKDEV() != true)
 		return PTPAPI_ERROR; //just representing status error
 #ifndef OPTIMIZED_PTP
 	if (!PTP_API_Is_Handle_Vaild(handle))
 		return PTPAPI_ERROR; //just representing handle error           
 #endif
 	PTP_Init_Transaction (&gdc);
 	
 	PTP_API_Init_ObjectInfo (&oi);
 	
 	PTP_Debug_Print("PTP_API_Get_Thumbnail : start(H:%x)\n", handle);
 	
 	PTP_BEGIN;
 	
 	result = ptp_getobjectinfo(&(gdc.params), handle, &oi);
 	if ( result != PTP_RC_OK )
 	{
 		PTP_Debug_Print("[Error] PTP_API_Get_thumbnail : ptp_getobjectinfo return(0x%04x)\n", result);
 		PTP_END;
 		return result;
 	}
 	
 	if (oi.ObjectFormat != PTP_OFC_EXIF_JPEG)
 	{
 		PTP_Debug_Print("[Error] PTP_API_Get_thumbnail : handle(%d) is not jpeg image.\n", handle);
 		PTP_END;
 		return result;
 	}
 	
 	
 	if (PTP_Debug_Level >=  PTP_DBG_LVL2)
 	{
 		stTick = Tick() ;
 	}
 	
 	result = ptp_getthumb(&(gdc.params), handle, Image);
 	
 	if (PTP_Debug_Level >= PTP_DBG_LVL2)
 	{
 		edTick = Tick() ;
 		
 		PTP_Debug_Print ("0x%08x: %9i\t%16s\t0x%08x\t0x%04x\t0x%08x speed:%d (ms)\n", handle,   \
 			oi.ObjectCompressedSize, oi.Filename,               oi.StorageID, oi.ObjectFormat, oi.ParentObject, edTick - stTick);
 	}
 	else
 	{
 		PTP_Debug_Print ("0x%08x: %9i\t%16s\t0x%08x\t0x%04x\t0x%08x\n", handle,         \
 			oi.ObjectCompressedSize, oi.Filename,                                                                           \
 			oi.StorageID, oi.ObjectFormat, oi.ParentObject);
 	}
 	
 	if (result != PTP_RC_OK)
 	{
 		PTP_Debug_Print ("[Error] PTP_API_Get_thumbnail : ptp_getthumb return(0x%04x)\n", result);
 		/* 
 		if (result == PTP_ERROR_IO) 
 		{
 		usb_clear_stall(dc);
 		} 
 		*/
 		*ImageSize = -1;
 	}
 	else
 	{
 		*ImageSize = oi.ThumbCompressedSize;
 	}
 	
 	PTP_API_Clear_ObjectInfo(&oi);
 	
 	PTP_END;
 	
 	PTP_Debug_Print("PTP_API_Get_Thumbnail : end(%x)\n\n", result);
 	
 	return result;
#endif

	// intialising unused arguments
	handle = 0;
	Image = NULL;
	*ImageSize = 0;

	return PTPAPI_ERROR;
 	
 }

/**
* @brief	      Get thumbnail image from the given ptp device
* @remarks		none
* @param		uint32 handle				: handle
* @param		char **Image				: the store of an image
* @param		uint32 *uint32 *ImageSize	: Compressed size of an image
* @param	      PTPDeviceHandle ptpHandle:	Handle for the given ptp device
* @return		uint16
PTP_RC_OK : success, else : failure
*/ 
uint16 PTP_API_Get_Thumbnail_MPTP(uint32 handle, char **Image, uint32 *ImageSize,PTPDeviceHandle  ptpHandle)
{
	
	PTPObjectInfo		oi;
	unsigned long stTick = 0;
	unsigned long edTick = 0;
	
	uint16	result = PTP_RC_OK;

	PTPDevContext *dc;
	dc=getDc(ptpHandle);
	if(dc==NULL)
	{
		PTP_Debug_Print("[ERROR] PTP_API_Get_Thumbnail_MPTP : PTPDevContext is null.\n");
		return PTPAPI_ERROR;//added by Niraj   
	}
	if (PTP_API_CHKDEV_MPTP(ptpHandle) != true)
		return PTPAPI_ERROR; //just representing status error
#ifndef OPTIMIZED_PTP
	if (!PTP_API_Is_Handle_Vaild_MPTP(handle, ptpHandle))
		return PTPAPI_ERROR; //just representing handle error		
#endif
	PTP_Init_Transaction (dc);
	
	PTP_API_Init_ObjectInfo (&oi);
	
	PTP_Debug_Print("PTP_API_Get_Thumbnail : start(H:%x)\n", handle);
	
	PTP_BEGIN;
	
	result = ptp_getobjectinfo(&(dc->params), handle, &oi);
	if ( result != PTP_RC_OK )
	{
		PTP_Debug_Print("[Error] PTP_API_Get_thumbnail : ptp_getobjectinfo return(0x%04x)\n", result);
		PTP_END;
		return result;
	}
	
	if (oi.ObjectFormat != PTP_OFC_EXIF_JPEG) 
	{
		PTP_Debug_Print("[Error] PTP_API_Get_thumbnail : handle(%d) is not jpeg image.\n", handle);
		PTP_END;
		return result;
	}         
	
	
	if (PTP_Debug_Level >=  PTP_DBG_LVL2)
	{
		stTick = Tick() ;
	}
	
	result = ptp_getthumb(&(dc->params), handle, Image);
	
	if (PTP_Debug_Level >= PTP_DBG_LVL2)
	{
		edTick = Tick() ;
		
		PTP_Debug_Print ("0x%08x: %9i\t%16s\t0x%08x\t0x%04x\t0x%08x speed:%d (ms)\n", handle,	\
			oi.ObjectCompressedSize, oi.Filename, 										\
			oi.StorageID, oi.ObjectFormat, oi.ParentObject, edTick - stTick);
	}
	else
	{
		PTP_Debug_Print ("0x%08x: %9i\t%16s\t0x%08x\t0x%04x\t0x%08x\n", handle,		\
			oi.ObjectCompressedSize, oi.Filename, 										\
			oi.StorageID, oi.ObjectFormat, oi.ParentObject);
	}
	
	if (result != PTP_RC_OK) 
	{
		PTP_Debug_Print ("[Error] PTP_API_Get_thumbnail : ptp_getthumb return(0x%04x)\n", result);
		/* 
		if (result == PTP_ERROR_IO) 
		{
		usb_clear_stall(dc);
		} 
		*/
		//*ImageSize = -1;
		*ImageSize = 0;//Niraj	
	} 
	else 
	{
		*ImageSize = oi.ThumbCompressedSize;
	}
	
	PTP_API_Clear_ObjectInfo(&oi);
	
	PTP_END;
	
	PTP_Debug_Print("PTP_API_Get_Thumbnail : end(%x)\n\n", result);
	
	return result;
}

void PTP_API_PartialReadStart(void)
{
	PTP_BEGIN;
	return;
}

void PTP_API_PartialReadStop(void)
{
	PTP_END;
	return;
}

/**
* @brief  
: Partial Read operation of PTP device, reads up to length bytes from StartOffset into
the buffer starting at buf
* @remarks 
* @param 
StartOffset : read starting point
ReturnCurOffset : return value of current file offset after read operation
* @return	On success, the number of bytes read  are  returned,  
0 : indicates  end  of  file, -1 : failure,            

  * @see	Using other method. This will do the Partial read for all cameras. 
  This API is useful if only single camera is available for testing & that camera supports partial read.
*/
 uint16 PTP_API_PartialRead(uint32 handle, char** buf, uint32 length, uint32 StartOffset, uint32 *ReturnCurOffset)
 {
#if 0
 	uint16 ret=0, i = 0;
 	PTPObjectInfo objinfo;  
 	
 	PTP_Debug_Print(" PTP_API_PartialRead Entering\n");
 	
 	if(length < 512)
 	{
 		*ReturnCurOffset = 0;
 		PTP_Debug_Print("\nERROR in PTP_API_PartialRead; length< 512\n");
 		return  PTP_PARTREAD_ERR;
 	}
 	if(StartOffset>0)
 	{
 		PTP_Debug_Print("~ ING\n");
 		ret = ptp_getpartialobjectNS( &(gdc.params), handle, StartOffset, length, buf);
 		/* india org, bug fix*/
 		*ReturnCurOffset = gdc.params.getObjRetSize;	
 		PTP_Debug_Print("Partial read Main:StartOffset = %d \n", StartOffset);
 		PTP_Debug_Print("Partial read Main:ReturnCurOffset = %d \n", (gdc.params.getObjRetSize));
 		PTP_Debug_Print("Partial read Main:required size = %d, readSize = %d \n \n",length, (gdc.params.getObjRetSize));
 		return ret;
 	}
 	
 	if (PTP_API_CHKDEV() != true)
 		return PTP_PARTREAD_ERR;
 	
 	PTP_API_Init_ObjectInfo(&objinfo);	  
 	for(i = 0; i < RETRY_CNT; i++)
 	{                
 		ret = ptp_getobjectinfo(&(gdc.params), handle, &objinfo);
 		if (ret ==PTP_RC_OK)
 		{
 			PTP_Debug_Print("SUCCESS in getting the object info (MAX retry conut : %d, current count: %d)\r\n",RETRY_CNT, i+1);
 			PTP_Debug_Print("objinfo.ObjectCompressedSize = %d\n", objinfo.ObjectCompressedSize);
 			
 			break;
 		}
 		else
 		{
 			PTP_Debug_Print("RETRY in getting the object info (MAX retry conut : %d, current count: %d)\r\n",RETRY_CNT, i+1);
 			continue;			
 		}
 	}
 	if(ret != PTP_RC_OK)
 	{			
 		PTP_Debug_Print("ERROR: Could not GetObjectInfo\r\n");
 		return ret;				
 	}	
 	
 	if(StartOffset >= objinfo.ObjectCompressedSize)
 	{
 		PTP_Debug_Print("\n Invalid StartOffset  --- Greater than the OBJECT SIZE\r\n");
 		PTP_API_Clear_ObjectInfo(&objinfo);
 		return PTP_PARTREAD_ERR;	
 	}
 	
 	if(length > (objinfo.ObjectCompressedSize -StartOffset))
 	{
 		length = objinfo.ObjectCompressedSize -StartOffset;
 	}
 	
 	PTP_Debug_Print("\nbefore AlGetPartialObject_NS\n");
 	PTP_Debug_Print("~ ING\n");
 	ret = ptp_getpartialobjectNS( &(gdc.params), handle, StartOffset, length, buf);	
 	/* india org, bug fix*/
 	*ReturnCurOffset= (gdc.params.getObjRetSize);
 	PTP_Debug_Print("Partial read Main:StartOffset = %d \n", StartOffset);
 	PTP_Debug_Print("Partial read Main:ReturnCurOffset = %d \n", (gdc.params.getObjRetSize));
 	PTP_Debug_Print("Partial read Main:required size = %d, readSize = %d \n \n",length, (gdc.params.getObjRetSize));
 	
 	PTP_API_Clear_ObjectInfo(&objinfo);	
 	
 	return ret;
#endif

	// intialising unused arguments
	handle = 0;
	buf = NULL;
	length = 0;
	StartOffset = 0;
	*ReturnCurOffset = 0;

	 return  PTP_PARTREAD_ERR;
 }

/**
* @brief		Partial Read operation for the given PTP device,
* @remarks	reads up to length bytes from StartOffset into
the buffer starting at buf
* @param		StartOffset : read starting point
* @param      	ReturnCurOffset : return value of current file offset after read operation
* @param		uint32 handle:  handle of the particular object
* @param	 	char** buf: buffer for fetching the partial data
* @param		uint32 length: number of bytes of data to fetched
* @param	       PTPDeviceHandle ptpHandle:    Handle for the given ptp device
* @return		uint16
1 :indicates  end  of  file,  2 : failure,  0:continue            
*/
uint16 PTP_API_PartialRead_MPTP(uint32 handle, char** buf, uint32 length, uint32 StartOffset, uint32 *ReturnCurOffset, PTPDeviceHandle  ptpHandle)
{
	uint16 ret=0, i = 0;
	PTPObjectInfo objinfo;  

	PTPDevContext *dc;
	dc=getDc(ptpHandle);
	if(dc==NULL)
	{
		PTP_Debug_Print("[ERROR] PTP_API_PartialRead_MPTP : PTPDevContext is null.\n");
		return  PTP_PARTREAD_ERR;//added by Niraj   
	}
	PTP_Debug_Print(" PTP_API_PartialRead Entering\n");
	
	if(length < 512)
	{
		*ReturnCurOffset = 0;
		PTP_Debug_Print("\nERROR in PTP_API_PartialRead; length< 512\n");
		return  PTP_PARTREAD_ERR;
	}
	if(StartOffset>0)
	{
		PTP_Debug_Print("~ ING\n");
		ret = ptp_getpartialobjectNS( &(dc->params), handle, StartOffset, length, buf);//modified by Niraj
		/* india org, bug fix*/
		*ReturnCurOffset = dc->params.getObjRetSize;	//modified by Niraj
		PTP_Debug_Print("Partial read Main:StartOffset = %d \n", StartOffset);
		PTP_Debug_Print("Partial read Main:ReturnCurOffset = %d \n", (dc->params.getObjRetSize));//modified by Niraj
		PTP_Debug_Print("Partial read Main:required size = %d, readSize = %d \n \n",length, (dc->params.getObjRetSize));//modified by Niraj
		return ret;
	}
	
	if (PTP_API_CHKDEV_MPTP(ptpHandle) != true)
		return PTP_PARTREAD_ERR;
	
	PTP_API_Init_ObjectInfo(&objinfo);	  
	for(i = 0; i < RETRY_CNT; i++)
	{                
		ret = ptp_getobjectinfo(&(dc->params), handle, &objinfo);
		if (ret ==PTP_RC_OK)
		{
			PTP_Debug_Print("SUCCESS in getting the object info (MAX retry conut : %d, current count: %d)\r\n",RETRY_CNT, i+1);
			PTP_Debug_Print("objinfo.ObjectCompressedSize = %d\n", objinfo.ObjectCompressedSize);
			
			break;
		}
		else
		{
			PTP_Debug_Print("RETRY in getting the object info (MAX retry conut : %d, current count: %d)\r\n",RETRY_CNT, i+1);
			continue;			
		}
	}
	if(ret != PTP_RC_OK)
	{			
		PTP_Debug_Print("ERROR: Could not GetObjectInfo\r\n");
//		PTP_API_Clear_ObjectInfo(&objinfo);
		return ret;				
	}	
	
	if(StartOffset >= objinfo.ObjectCompressedSize)
	{
		PTP_Debug_Print("\n Invalid StartOffset  --- Greater than the OBJECT SIZE\r\n");
		PTP_API_Clear_ObjectInfo(&objinfo);
		return PTP_PARTREAD_ERR;	
	}
	
	if(length > (objinfo.ObjectCompressedSize -StartOffset))
	{
		length = objinfo.ObjectCompressedSize -StartOffset;
	}
	
	PTP_Debug_Print("\nbefore AlGetPartialObject_NS\n");
	PTP_Debug_Print("~ ING\n");
	ret = ptp_getpartialobjectNS( &(dc->params), handle, StartOffset, length, buf);	//modified by Niraj
	/* india org, bug fix*/
	*ReturnCurOffset= (dc->params.getObjRetSize);//modified by Niraj
	PTP_Debug_Print("Partial read Main:StartOffset = %d \n", StartOffset);
	PTP_Debug_Print("Partial read Main:ReturnCurOffset = %d \n", (dc->params.getObjRetSize));//modified by Niraj
	PTP_Debug_Print("Partial read Main:required size = %d, readSize = %d \n \n",length, (dc->params.getObjRetSize));//modified by Niraj
	
	PTP_API_Clear_ObjectInfo(&objinfo);	
	
	return ret;
}


/**
* @brief	      Get jpeg image
* @remarks		none
* @param		uint32 handle				: handle
* @param		char **Image				: the store of an image
* @param		uint32 *uint32 *ImageSize	: Compressed size of an image
* @return		uint16
PTP_RC_OK : success, else : failure
*/ 
 uint16 PTP_API_Get_JpegImage(uint32 handle, char **Image, uint32 *ImageSize)
 {
#if 0
 	PTPObjectInfo           oi;
 	unsigned long stTick = 0;
 	unsigned long edTick = 0;
 	
 	uint16  result = PTP_RC_OK;
 	
 	if (PTP_API_CHKDEV() != true)
 		return PTPAPI_ERROR; //just representing status error
 #ifndef OPTIMIZED_PTP
 	if (!PTP_API_Is_Handle_Vaild(handle))
 		return PTPAPI_ERROR; //just representing handle error           
 #endif
 	PTP_Init_Transaction (&gdc);
 	
 	PTP_API_Init_ObjectInfo (&oi);
 	
 	PTP_Debug_Print("PTP_API_Get_JpegImage : start(H:%x)\n", handle);
 	
 	PTP_BEGIN;
 	
 	result = ptp_getobjectinfo(&(gdc.params), handle, &oi);
 	if ( result != PTP_RC_OK )
 	{
 		PTP_Debug_Print("[Error] PTP_API_Get_JpegImage : ptp_getobjectinfo return(0x%04x)\n", result);
 		PTP_END;
 		return result;
 	}
 	
 	if (oi.ObjectFormat != PTP_OFC_EXIF_JPEG)
 	{
 		PTP_Debug_Print("[Error] PTP_API_Get_JpegImage : handle(%d) is not jpeg image.\n", handle);
 		PTP_END;
 		return PTPAPI_ERROR; // just representing handle error
 	}
 	
 	if (PTP_Debug_Level >=  PTP_DBG_LVL2)
 	{
 		stTick = Tick() ;
 	}
 	
 	result = ptp_getobject(&(gdc.params),handle, Image);
 	
 	if (PTP_Debug_Level >= PTP_DBG_LVL2)
 	{
 		edTick = Tick() ;
 		PTP_Debug_Print ("0x%08x: %9i\t%16s\t0x%08x\t0x%04x\t0x%08x speed:%d(ms)\n", handle,    \
 			oi.ObjectCompressedSize, oi.Filename,                                                                           \
 			oi.StorageID, oi.ObjectFormat, oi.ParentObject, edTick -stTick  );
 	}
 	else
 	{
 		PTP_Debug_Print ("0x%08x: %9i\t%16s\t0x%08x\t0x%04x\t0x%08x\n", handle,         \
 			oi.ObjectCompressedSize, oi.Filename,                                                                           \
 			oi.StorageID, oi.ObjectFormat, oi.ParentObject);
 	}
 	
 	if (result != PTP_RC_OK)
 	{
 		PTP_Debug_Print ("[Error] PTP_API_Get_JpegImage : ptp_getobject return(0x%04x)\n", result);
 		*ImageSize = -1;
 	}
 	else
 	{
 		*ImageSize = oi.ObjectCompressedSize;
 	}
 	
 	PTP_API_Clear_ObjectInfo(&oi);
 	
 	PTP_END;
 	
 	PTP_Debug_Print("PTP_API_Get_JpegImage : end(%x)\n\n", result);
 	
 	return result;
#endif

	// intialising unused arguments
	handle = 0;
	Image = NULL;
	*ImageSize = 0;

	return PTPAPI_ERROR;
 	
 }

/**
* @brief	      Get jpeg image from the given device
* @remarks		none
* @param		uint32 handle				: handle
* @param		char **Image				: the store of an image
* @param		uint32 *uint32 *ImageSize	: Compressed size of an image
* @param	      PTPDeviceHandle ptpHandle:	Handle for the given ptp device
* @return		uint16
PTP_RC_OK : success, else : failure
*/ 
uint16 PTP_API_Get_JpegImage_MPTP (uint32 handle, char **Image, uint32 *ImageSize, PTPDeviceHandle  ptpHandle)
{
	
	PTPObjectInfo		oi;
	unsigned long stTick = 0;
	unsigned long edTick = 0;
	
	uint16	result = PTP_RC_OK;

	PTPDevContext *dc;
	dc=getDc(ptpHandle);
	if(dc==NULL)
	{
		PTP_Debug_Print("[ERROR] PTP_API_Get_JpegImage_MPTP : PTPDevContext is null.\n");
		return PTPAPI_ERROR;//added by Niraj   
	}
	if (PTP_API_CHKDEV_MPTP(ptpHandle) != true)
		return PTPAPI_ERROR; //just representing status error
	
#ifndef OPTIMIZED_PTP
	if (!PTP_API_Is_Handle_Vaild_MPTP(handle, ptpHandle))
		return PTPAPI_ERROR; //just representing handle error		
#endif		
	PTP_Init_Transaction (dc);
	
	PTP_API_Init_ObjectInfo (&oi);
	
	PTP_Debug_Print("PTP_API_Get_JpegImage : start(H:%x)\n", handle);
	
	PTP_BEGIN;
	
	
	result = ptp_getobjectinfo(&(dc->params), handle, &oi);
	
	if ( result != PTP_RC_OK )
	{
		PTP_Debug_Print("[Error] PTP_API_Get_JpegImage : ptp_getobjectinfo return(0x%04x)\n", result);
		PTP_END;
		return result;
	}
	
	if (oi.ObjectFormat != PTP_OFC_EXIF_JPEG) 
	{
		PTP_Debug_Print("[Error] PTP_API_Get_JpegImage : handle(%d) is not jpeg image.\n", handle);
		PTP_END;		
		return PTPAPI_ERROR; // just representing handle error
	}         
	
	if (PTP_Debug_Level >=  PTP_DBG_LVL2)
	{
		stTick = Tick() ;
	}
	
	result = ptp_getobject(&(dc->params),handle, Image);
	
	if (PTP_Debug_Level >= PTP_DBG_LVL2)
	{
		edTick = Tick() ;
		PTP_Debug_Print ("0x%08x: %9i\t%16s\t0x%08x\t0x%04x\t0x%08x speed:%d(ms)\n", handle,	\
			oi.ObjectCompressedSize, oi.Filename, 										\
			oi.StorageID, oi.ObjectFormat, oi.ParentObject, edTick -stTick  );
	}
	else
	{
		PTP_Debug_Print ("0x%08x: %9i\t%16s\t0x%08x\t0x%04x\t0x%08x\n", handle,		\
			oi.ObjectCompressedSize, oi.Filename, 										\
			oi.StorageID, oi.ObjectFormat, oi.ParentObject);
	}
	
	if (result != PTP_RC_OK) 
	{
		PTP_Debug_Print ("[Error] PTP_API_Get_JpegImage : ptp_getobject return(0x%04x)\n", result);
		//*ImageSize = -1;
		*ImageSize=0;//Niraj
	} 
	else 
	{	
		*ImageSize = oi.ObjectCompressedSize;
		
	}
	
	PTP_API_Clear_ObjectInfo(&oi);	
	
	PTP_END;	
	
	PTP_Debug_Print("PTP_API_Get_JpegImage : end(%x)\n\n", result);
	
	return result;	
}

/**
* @brief	      Get MP3 file
* @remarks		none
* @param		uint32 handle				: handle
* @param		char **Image				: the store of an mp3
* @param		uint32 *uint32 *ImageSize	: Compressed size of an mp3
* @return		uint16
PTP_RC_OK : success, else : failure
*/ 
 uint16 PTP_API_Get_MP3File(uint32 handle, char **Image, uint32 *ImageSize)
 {
#if 0
 	PTPObjectInfo           oi;
 	
 	char    sztemp[100]  = {0};
 	long    start_sec = 0, end_sec = 0;
 	double  start_msec = 0, end_msec = 0;
 	
 	uint16  result = PTP_RC_OK;
 	
 	if (PTP_API_CHKDEV() != true)
 		return PTPAPI_ERROR; //just representing status error
 #ifndef OPTIMIZED_PTP
 	if (!PTP_API_Is_Handle_Vaild(handle))
 		return PTPAPI_ERROR; //just representing handle error           
 #endif
 	PTP_Init_Transaction (&gdc);
 	
 	PTP_API_Init_ObjectInfo (&oi);
 	
 	PTP_Debug_Print("PTP_API_Get_MP3 : start(H:%x)\n", handle);
 	
 	PTP_BEGIN;
 	
 	result = ptp_getobjectinfo(&(gdc.params), handle, &oi);
 	if ( result != PTP_RC_OK )
 	{
 		PTP_Debug_Print("[Error] PTP_API_Get_MP3 : ptp_getobjectinfo return(0x%04x)\n", result);
 		PTP_END;
 		return result;
 	}
 	
 	if (oi.ObjectFormat != PTP_OFC_MP3)
 	{
 		PTP_Debug_Print("[Error] PTP_API_Get_MP3 : handle(%d) is not jpeg image.\n", handle);
 		PTP_END;
 		return PTPAPI_ERROR; //just representing handle error
 	}
 	
 	result = ptp_getobject(&(gdc.params),handle, Image);
 	
 	PTP_Debug_Print ("0x%08x: %9i\t%16s\t0x%08x\t0x%04x\t0x%08x\n", handle,         \
 		oi.ObjectCompressedSize, oi.Filename,                                                                           \
 		oi.StorageID, oi.ObjectFormat, oi.ParentObject);
 	
 	if (result != PTP_RC_OK)
 	{
 		PTP_Debug_Print ("[Error] PTP_API_Get_MP3 : ptp_getobject return(0x%04x)\n", result);
 		*ImageSize = -1;
 	}
 	else
 	{
 		*ImageSize = oi.ObjectCompressedSize;
 	}
 	
 	PTP_API_Clear_ObjectInfo(&oi);
 	
 	PTP_END;
 	
 	PTP_Debug_Print("PTP_API_Get_MP3 : end(%x)\n\n", result);
 	
 	return result;
#endif

	// intialising unused arguments
	handle = 0;
	Image = NULL;
	*ImageSize = 0;

	return PTPAPI_ERROR; 	
 }

/**
* @brief	      Get MP3 file for the given ptp device
* @remarks		none
* @param		uint32 handle				: handle
* @param		char **Image				: the store of an mp3
* @param		uint32 *uint32 *ImageSize	: Compressed size of an mp3
* @param	      PTPDeviceHandle ptpHandle:	Handle for the given ptp device
* @return		uint16
PTP_RC_OK : success, else : failure
*/ 
uint16 PTP_API_Get_MP3File_MPTP(uint32 handle, char **Image, uint32 *ImageSize, PTPDeviceHandle  ptpHandle)
{
	PTPObjectInfo		oi;
	
	//char 	sztemp[100]  = {0};
	//long 	start_sec = 0, end_sec = 0;
	//double 	start_msec = 0, end_msec = 0;
	
	uint16	result = PTP_RC_OK;

	PTPDevContext *dc;
	dc=getDc(ptpHandle);
	if(dc==NULL)
	{
		PTP_Debug_Print("[ERROR] PTP_API_Get_MP3File_MPTP : PTPDevContext is null.\n");
		return PTPAPI_ERROR;//added by Niraj   
	}
	if (PTP_API_CHKDEV_MPTP(ptpHandle) != true)
		return PTPAPI_ERROR; //just representing status error
#ifndef OPTIMIZED_PTP
	if (!PTP_API_Is_Handle_Vaild_MPTP(handle, ptpHandle))
		return PTPAPI_ERROR; //just representing handle error		
#endif		
	PTP_Init_Transaction (dc);
	
	PTP_API_Init_ObjectInfo (&oi);
	
	PTP_Debug_Print("PTP_API_Get_MP3 : start(H:%x)\n", handle);
	
	PTP_BEGIN;	
	
	result = ptp_getobjectinfo(&(dc->params), handle, &oi);
	if ( result != PTP_RC_OK )
	{
		PTP_Debug_Print("[Error] PTP_API_Get_MP3 : ptp_getobjectinfo return(0x%04x)\n", result);
		PTP_END;
		return result;
	}
	
	if (oi.ObjectFormat != PTP_OFC_MP3) 
	{
		PTP_Debug_Print("[Error] PTP_API_Get_MP3 : handle(%d) is not jpeg image.\n", handle);
		PTP_END;		
		return PTPAPI_ERROR; //just representing handle error
	}         
	
	result = ptp_getobject(&(dc->params),handle, Image);
	
	PTP_Debug_Print ("0x%08x: %9i\t%16s\t0x%08x\t0x%04x\t0x%08x\n", handle,		\
		oi.ObjectCompressedSize, oi.Filename, 										\
		oi.StorageID, oi.ObjectFormat, oi.ParentObject);
	
	if (result != PTP_RC_OK) 
	{
		PTP_Debug_Print ("[Error] PTP_API_Get_MP3 : ptp_getobject return(0x%04x)\n", result);
		//*ImageSize = -1;
		*ImageSize=0;//Niraj
	} 
	else 
	{
		*ImageSize = oi.ObjectCompressedSize;
	}
	
	PTP_API_Clear_ObjectInfo(&oi);		
	
	PTP_END;	
	
	PTP_Debug_Print("PTP_API_Get_MP3 : end(%x)\n\n", result);
	
	return result;	
}

/**
* @brief	      Get information of a jpeg image
* @remarks		none
* @param		uint32 handle				: handle
* @param		PTPObjectInfo *oi			: the store of an object information
* @return		uint16
PTP_RC_OK : success, else : failure
*/ 
 uint32 PTP_API_Get_JpegImageInfo(uint32 handle, PTPObjectInfo *oi)
 {
#if 0
 	uint16  result = PTP_RC_OK;
 	
 	if (PTP_API_CHKDEV() != true)
 		return PTPAPI_ERROR; //just representing status error
 #ifndef OPTIMIZED_PTP
 	if (!PTP_API_Is_Handle_Vaild(handle))
 		return PTPAPI_ERROR; //just representing handle error                           
 #endif
 	PTP_Init_Transaction (&gdc);
 	
 	PTP_Debug_Print("PTP_API_Get_JpegImageInfo : start(H:%x)\n", handle);
 	
 	PTP_BEGIN;
 	
 	result = ptp_getobjectinfo(&(gdc.params), handle, gdc.params.objectinfo);
 	if ( result != PTP_RC_OK )
 	{
 		PTP_Debug_Print("[Error] PTP_API_Get_JpegImageInfo : ptp_getobjectinfo return(%x)\n", result);
 		PTP_END;
 		return result;
 	}
 	
 	if (gdc.params.objectinfo->ObjectFormat != PTP_OFC_EXIF_JPEG)
 	{
 		PTP_Debug_Print("[Error] PTP_API_Get_JpegImageInfo : handle(%x) is not jpeg image.\n", handle);
 		PTP_END;
 		return PTPAPI_ERROR; //just representing handle error
 	}
 	
 	PTP_Debug_Print ("0x%08x: % 9i\t%16s\t0x%08x\t0x%04x\t0x%08x\n",handle,         \
 		gdc.params.objectinfo->ObjectCompressedSize, gdc.params.objectinfo->Filename,                                   \
 		gdc.params.objectinfo->StorageID, gdc.params.objectinfo->ObjectFormat, gdc.params.objectinfo->ParentObject);
 	
	// ÀÌ ÇÔ¼ö¸¦ È£ÃâÇÑ ÂÊ¿¡¼­ objectinfoÀÇ ¸Þ¸ð¸®¸¦ ÇØÁ¦ÇØÁÖ¾î¾ß ÇÑ´Ù.
 	ptp_objectinfo_copy(oi, gdc.params.objectinfo);
 	
 	PTP_END;
 	
 	PTP_Debug_Print("PTP_API_Get_JpegImageInfo : end(%x)\n\n", result);
 	
 	return result;
#endif

	// intialising unused arguments
	handle = 0;
	oi = NULL;

	return PTPAPI_ERROR;
 }


/**
* @brief	      Get information of a jpeg image for the given ptp device
* @remarks		none
* @param		uint32 handle				: handle
* @param		PTPObjectInfo *oi			: the store of an object information
* @param	      PTPDeviceHandle ptpHandle:	Handle for the given ptp device
* @return		uint16
PTP_RC_OK : success, else : failure
*/
uint32 PTP_API_Get_JpegImageInfo_MPTP (uint32 handle, PTPObjectInfo *oi,PTPDeviceHandle  ptpHandle)
{
	uint16	result = PTP_RC_OK;

	PTPDevContext *dc;
	dc=getDc(ptpHandle);
	if(dc==NULL)
	{
		PTP_Debug_Print("[ERROR] PTP_API_Get_JpegImageInfo_MPTP : PTPDevContext is null.\n");
		return PTPAPI_ERROR;//added by Niraj   
	}
	if (PTP_API_CHKDEV_MPTP(ptpHandle)  != true)
		return PTPAPI_ERROR; //just representing status error
#ifndef OPTIMIZED_PTP
	if (!PTP_API_Is_Handle_Vaild_MPTP(handle, ptpHandle))
		return PTPAPI_ERROR; //just representing handle error				
#endif
	PTP_Init_Transaction (dc);
	
	PTP_Debug_Print("PTP_API_Get_JpegImageInfo : start(H:%x)\n", handle);
	
	PTP_BEGIN;	
	
	
	result = ptp_getobjectinfo(&(dc->params), handle, oi); //modified by Niraj to get object info directly in oi
	if ( result != PTP_RC_OK )
	{
		PTP_Debug_Print("[Error] PTP_API_Get_JpegImageInfo : ptp_getobjectinfo return(%x)\n", result);
		PTP_END;		
	      	return result;
	}
	
	
	
	if (oi->ObjectFormat != PTP_OFC_EXIF_JPEG) //modified by Niraj
	{
		PTP_Debug_Print("[Error] PTP_API_Get_JpegImageInfo : handle(%x) is not jpeg image.\n", handle);
		PTP_END;				
		return PTPAPI_ERROR; //just representing handle error
	}         
	
	
	
	//added by Niraj to allocate space for objectinfo which is NULL before this point
	if(dc->params.objectinfo==NULL)
	{
		dc->params.objectinfo=(PTPObjectInfo*)malloc(sizeof(PTPObjectInfo));//vishal g++ typecasting
	}
	
	ptp_objectinfo_copy(dc->params.objectinfo,oi);
	//added by Niraj..end
	
	PTP_Debug_Print ("0x%08x: % 9i\t%16s\t0x%08x\t0x%04x\t0x%08x\n",handle,         \
		dc->params.objectinfo->ObjectCompressedSize, dc->params.objectinfo->Filename,                                   \
		dc->params.objectinfo->StorageID, dc->params.objectinfo->ObjectFormat, dc->params.objectinfo->ParentObject);
	
	
// ÀÌ ÇÔ¼ö¸¦ È£ÃâÇÑ ÂÊ¿¡¼­ objectinfoÀÇ ¸Þ¸ð¸®¸¦ ÇØÁ¦ÇØÁÖ¾î¾ß ÇÑ´Ù.	
	PTP_END;			
	
	PTP_Debug_Print("PTP_API_Get_JpegImageInfo : end(%x)\n\n", result);
	
	return result;	
}


/**
* @brief	      Get information of a device
* @remarks		none
* @param		PTPDeviceInfo *deviceinfo	: the store of a device information
* @return		uint16
PTP_RC_OK : success, else : failure
*/ 
 uint16 PTP_API_Get_DeviceInfo(PTPDeviceInfo *deviceinfo)
 {
#if 0
 	uint16 result = PTP_RC_OK;
 	
 	if (PTP_API_CHKDEV() != true)
 		return PTPAPI_ERROR; //just representing status error
 	
 	PTP_Init_Transaction (&gdc);
 	
 	PTP_Debug_Print("PTP_API_Get_DeviceInfo : start\n");
 	
 	PTP_BEGIN;
 	
 	result = ptp_getdeviceinfo (&(gdc.params), &(gdc.params.deviceinfo));
 	if( result != PTP_RC_OK)
 	{
 		PTP_Debug_Print("[Error] PTP_API_Get_DeviceInfo : ptp_getdeviceinfo return (%x)\n", result);
 		PTP_END;
 		return result;
 	}
 	
	// ÀÌ ÇÔ¼ö¸¦ È£ÃâÇÑ ÂÊ¿¡¼­ deviceinfoÀÇ ¸Þ¸ð¸®¸¦ ÇØÁ¦ÇØÁÖ¾î¾ß ÇÑ´Ù.
 	ptp_deviceinfo_copy(deviceinfo, &(gdc.params.deviceinfo));
 	
 	PTP_END;
 	
 	PTP_Debug_Print("PTP_API_Get_DeviceInfo : end(%x)\n\n", result);
 	
 	return result;
 #endif

	// intialising unused arguments
	deviceinfo = NULL;

	return PTPAPI_ERROR;	
 }

/**
* @brief	      Get information of the given ptp device
* @remarks		none
* @param		PTPDeviceInfo *deviceinfo	: the store of a device information
* @param	      PTPDevContext *dc		:Device context of the given device
* @return		uint16
PTP_RC_OK : success, else : failure
*/ 
uint16 PTP_API_Get_DeviceInfo_MPTP(PTPDeviceInfo *deviceinfo, PTPDeviceHandle  ptpHandle)
{
	uint16 result = PTP_RC_OK;

	PTPDevContext *dc;
	dc=getDc(ptpHandle);
	if(dc==NULL)
	{
		PTP_Debug_Print("[ERROR] PTP_API_Get_DeviceInfo_MPTP : PTPDevContext is null.\n");
//printf("[ERROR] PTP_API_Get_DeviceInfo_MPTP : PTPDevContext is null.\n");
		return PTPAPI_ERROR;//added by Niraj   
	}
	if (PTP_API_CHKDEV_MPTP(ptpHandle)  != true)
	{
//printf("[ERROR] PTP_API_Get_DeviceInfo_MPTP : PTP_API_CHKDEV_MPTP failed....\n");
		return PTPAPI_ERROR; //just representing status error
	}
	
	PTP_Init_Transaction (dc);
	
	PTP_Debug_Print("PTP_API_Get_DeviceInfo : start\n");
	
	PTP_BEGIN;	
	
	result = ptp_getdeviceinfo (&(dc->params), &(dc->params.deviceinfo));
	if( result != PTP_RC_OK) 
	{
		PTP_Debug_Print("[Error] PTP_API_Get_DeviceInfo : ptp_getdeviceinfo return (%x)\n", result);
//printf("[ERROR] PTP_API_Get_DeviceInfo_MPTP :ptp_getdeviceinfo return (%x)\n", result);
		PTP_END;
		return result;
	}
	
// ÀÌ ÇÔ¼ö¸¦ È£ÃâÇÑ ÂÊ¿¡¼­ objectinfoÀÇ ¸Þ¸ð¸®¸¦ ÇØÁ¦ÇØÁÖ¾î¾ß ÇÑ´Ù.	ptp_deviceinfo_copy(deviceinfo, &(dc->params.deviceinfo));
	ptp_deviceinfo_copy(deviceinfo, &(dc->params.deviceinfo));

	PTP_END;
	
	PTP_Debug_Print("PTP_API_Get_DeviceInfo : end(%x)\n\n", result);
	
	return result;	
	
}

/**
* @brief	      Get devpath of a device
* @remarks		none
* @param		PTPDeviceHandle  ptpHandle	: the store of a device path
* @return		uint16
PTP_RC_OK : success, else : failure
*/ 
 uint16 PTP_API_Get_DevPath(char *devpath)
 {
#if 0
 	uint16 result = PTP_RC_OK;
 	
 	if (PTP_API_CHKDEV() != true)
 		return PTPAPI_ERROR; //just representing status error
 	
 	PTP_Debug_Print("PTP_API_Get_DevPath : start\n");
 	
 	PTP_BEGIN;
 	
 	if(gdc.dev)
 		strcpy(devpath, gdc.dev->devpath);
 	else
 		result = PTP_RC_GeneralError;
 	
 	PTP_END;
 	
 	PTP_Debug_Print("PTP_API_Get_DevPath : end(%x)\n\n", result);
 	
 	return result;
#endif

	// intialising unused arguments
	devpath = NULL;

	return PTPAPI_ERROR;
 	
 }

/**
* @brief	      Get devpath of the given device
* @remarks		none
* @param		PTPDeviceHandle *ptpHandle	: the store of a device path
* @param	      PTPDevContext *dc		:Device context of the given device
* @return		uint16
PTP_RC_OK : success, else : failure
*/ 
#if 0
uint16 PTP_API_Get_DevPath_MPTP(char *devpath, struct usb_device* dev)
{
	
	uint16 result = PTP_RC_OK;
	
// 	if(dc==NULL)
// 	{
// 		PTP_Debug_Print("[ERROR] PTP_API_Get_DevPath_MPTP : PTPDevContext is null.\n");
// 		return PTPAPI_ERROR;//added by Niraj   
// 	}
// 	if (PTP_API_CHKDEV_MPTP(devpath)  != true)
// 		return PTPAPI_ERROR; //just representing status error
	
	PTP_Debug_Print("PTP_API_Get_DevPath : start\n");
	
	PTP_BEGIN;	
	
	if(dev)
	{
		sprintf(dev->devpath,"/%s/%s",dev->bus->dirname,dev->filename);
		strcpy(devpath,"/dev/bus/usb");
// 		devpath[0]= '\0';
		strcat(devpath, dev->devpath);
		MPTP_Debug_Print ("PTP_API_Get_DevPath_MPTP: device path is %s\n", dev->devpath);
	}
	else
		result = PTP_RC_GeneralError;
	
	PTP_END;
	
	PTP_Debug_Print("PTP_API_Get_DevPath : end(%x)\n\n", result);
	
	return result;	
	
}
#endif

uint16 PTP_API_Get_DevPath_MPTP(char *devpath, PTPDeviceHandle  ptpHandle)
{

        uint16 result = PTP_RC_OK;
        char strTemp[15];

        PTPDevContext *dc;
        dc=getDc(ptpHandle);
        if(dc==NULL)
        {
                PTP_Debug_Print("[ERROR] PTP_API_Get_DevPath_MPTP : PTPDevContext is null.\n");
                return PTPAPI_ERROR;//added by Niraj   
        }
        if (PTP_API_CHKDEV_MPTP(ptpHandle) != true)
                return PTPAPI_ERROR;

        PTP_Debug_Print("PTP_API_Get_DevPath : start\n");

        PTP_BEGIN;

                sprintf(strTemp,"/%s/%s",dc->bus->dirname,dc->dev->filename);
  //              strcpy(devpath,"/dev/bus/usb");
//              devpath[0]= '\0';
//                strcat(devpath, strTemp);
                strcpy(devpath,strTemp);
                MPTP_Debug_Print ("PTP_API_Get_DevPath_MPTP: device path is %s\n", devpath);
			//	printf("\nPTP_API_Get_DevPath_MPTP: device path is %s strTemp is %s\n", devpath,strTemp);


        PTP_END;

        PTP_Debug_Print("PTP_API_Get_DevPath : end(%x)\n\n", result);

        return result;

}



/**
    * @brief			PTP_API_Get_StorageIDs´Â ÇØ´ç µðÁöÅÐ Ä«¸Þ¶ó¿¡¼­ °ü¸®ÇÏ°í ÀÖ´Â storage id µé ¸®½ºÆ®¸¦ ¹Þ¾Æ¿Â´Ù.
    * @remarks		PTP_API_Get_StorageInfos ÇÔ¼ö¸¦ È£ÃâÇÏ±â Àü¿¡ ÀÌ ÇÔ¼ö°¡ ¸ÕÀú È£ÃâµÇ¾î¾ß ÇÔ.
    					PTP ½ºÆå¿¡¼­ getstorageids API ¸¦ Á¤ÀÇÇÔ.

    					ÁÖÀÇ) ÀÌ ÇÔ¼ö¸¦ È£ÃâÇÏ±â Àü¿¡ PTP_API_Init_StorageIDs ÇÔ¼ö¸¦ È£ÃâÇØ¾ß ÇÔ.
    					ÀÌ ÇÔ¼ö°¡ È£ÃâµÇ¸é PTPStorageIDs¿¡ Id  µéÀÌ 1Â÷¿ø ¹è¿­·Î ÀúÀåµÊ.
    					PTPStorageIDµéÀ» »ç¿ëÇÑ ÈÄ PTP_API_Clear_StorageIDs () ¸¦ È£ÃâÇØ ¸Þ¸ð¸®¸¦ ÇØÁ¦ÇØ¾ß ÇÔ.
  
				-----------------
				| usb handle open  |
				-----------------
				|
				V
				-----------------
				| ptp session open  |
				-----------------
				|
				V
				-----------------------
				| send ptp_getstorageids   |
				-----------------------
				|
				V
				-----------------
				| ptp session close |
				-----------------
				|
				V
				-----------------
				| usb handle close  |
				-----------------
				
    * @param         storage id ÀÇ ¹è¿­ ±¸Á¶Ã¼.
    * @return		¾Æ·¡ ÇÔ¼ö°¡ ¼º°øÀûÀ¸·Î ¼öÇàµÇ¸é PTP_RC_OK ¸¦ ¸®ÅÏÇÏ°í,
    				¼öÇà µµÁß¿¡ ¿¡·¯¸¦ ¸¸³ª¸é PTP_RC_Undefined ¸¦ ¸®ÅÏÇÔ.
    * @see		ptp_opensession ´Â usb handleÀ» openÇÑ ÈÄ ptp sessionÀ» openÇÒ¶§ »ç¿ëÇÔ.
    				ptp_getstorageids´Â PTP ½ºÆå¿¡¼­ Á¤ÀÇÇÑ storage idµéÀ» °¡Á®¿À±â À§ÇÑ API.
    				ptp_closesession ´Â openµÈ ptp sessionÀ» closeÇÒ¶§ »ç¿ëÇÔ.
*/
 uint16 PTP_API_Get_StorageIDs(PTPStorageIDs *storageids)
 {
#if 0
 	uint16  result = PTP_RC_OK;
 	
 	if (PTP_API_CHKDEV() != true)
 		return PTPAPI_ERROR; //just representing status error
 	
 	PTP_Init_Transaction (&gdc);
 	
 	PTP_Debug_Print("PTP_API_Get_StorageIDs : start\n");
 	
 	PTP_BEGIN;
 	
 	result = ptp_getstorageids (&(gdc.params), storageids);
 	if( result != PTP_RC_OK)
 	{
 		PTP_Debug_Print("[Error] PTP_API_Get_StorageIDs : ptp_getstorageids return (%x)\n", result);
 		PTP_END;
 		return result;
 	}
 	
 	PTP_END;
 	
 	PTP_Debug_Print("PTP_API_Get_StorageIDs : end(%x)\n\n", result);
 	
 	return result;
#endif

	// intialising unused arguments
	storageids = NULL;	

	return PTPAPI_ERROR;
 	
 }

/**
* @brief	      Get list of storageIds from the given ptp device
* @remarks		none
* @param		PTPStorageIDs *storageids:		list of storage ids
* @param	      PTPDeviceHandle ptpHandle:	Handle for the given ptp device
* @return		uint16
PTP_RC_OK : success, else : failure
*/ 
uint16 PTP_API_Get_StorageIDs_MPTP(PTPDeviceHandle  ptpHandle,PTPStorageIDs *storageids)
{
	uint16  result = PTP_RC_OK;

	PTPDevContext *dc;

	dc = getDc(ptpHandle);
	if(dc == NULL)
	{
		PTP_Debug_Print("[ERROR] PTP_API_Get_StorageIDs_MPTP : PTPDevContext is null.\n");
				printf("[ERROR] PTP_API_Get_StorageIDs_MPTP : PTPDevContext is null.\n");

		return PTPAPI_ERROR;
	}

	

	if (PTP_API_CHKDEV_MPTP(ptpHandle) != true)
	{
		PTP_Debug_Print("[ERROR] PTP_API_Get_StorageIDs_MPTP :PTP_API_CHKDEV_MPTP failed \n");

		printf("[ERROR] PTP_API_Get_StorageIDs_MPTP :PTP_API_CHKDEV_MPTP failed \n");
		return PTPAPI_ERROR;
	}
	
	//	PTP_Init_Transaction (&gdc);
	PTP_Init_Transaction (dc);
	   
	PTP_Debug_Print("PTP_API_Get_StorageIDs : start\n");
	
	PTP_BEGIN;	

	  	
	//	result = ptp_getstorageids (&(gdc.params), storageids);
	result = ptp_getstorageids (&(dc->params), storageids);
	
	if( result != PTP_RC_OK)
	{
		PTP_Debug_Print("[Error] PTP_API_Get_StorageIDs : ptp_getstorageids return (%x)\n", result);
		printf("[Error] PTP_API_Get_StorageIDs : ptp_getstorageids return (%x)\n", result);
		PTP_END;
		return result;
	}       
	
	PTP_END;
	
	PTP_Debug_Print("PTP_API_Get_StorageIDs : end(%x)\n\n", result);
	
	return result;
}       

/**
    * @brief		PTP_API_Get_StorageInfos ÇÔ¼ö´Â ptp ÀåºñÀÇ Æ¯Á¤ storage id ¿¡ ´ëÇÑ Á¤º¸¸¦ ¾ò¾î¿È.
    * @remarks	PTP_API_Get_StorageInfos ÇÔ¼ö´Â µðÁöÅÐ Ä«¸Þ¶ó¿¡ ¿¬°áµÈ ÀúÀåÀåÄ¡ÀÇ Á¤º¸¸¦ °¡Á®¿Ã¶§ »ç¿ëÇÔ.
    				PTP ½ºÆå¿¡¼­ getstorageinfo API ¸¦ »ç¿ëÇÔ.
    				
    				ÁÖÀÇ) ÀÌ ÇÔ¼ö¸¦ È£ÃâÇÏ±â Àü¿¡ PTP_API_Init_PTPStorageInfo () ÇÔ¼ö¸¦ È£ÃâÇØ¾ß ÇÔ. 
    					PTPStorageInfo ÀÇ StorageDescription, VolumeLabelÀº »ç¿ëÈÄ ¸Þ¸ð¸®¸¦ ÇØÁ¦ÇØ¾ß ÇÔ.
    					Áï, PTP_API_Clear_PTPStorageInfo () ÇÔ¼ö¸¦ È£ÃâÇÏ¸é µÊ.
  
				-----------------
				| usb handle open  |
				-----------------
				|
				V
				-----------------
				| ptp session open  |
				-----------------
				|
				V
				------------------------
				| send ptp_getstorageinfo   |
				------------------------
				|
				V
				-----------------
				| ptp session close |
				-----------------
				|
				V
				-----------------
				| usb handle close  |
				-----------------
				
    * @param		storageid	µðÁöÅÐ Ä«¸Þ¶ó¿¡¼­ °ü¸®ÇÏ´Â ÀúÀåÀåÄ¡ÀÇ id. PTP ½ºÆå¿¡¼­ Á¤ÀÇÇÑ 4¹ÙÀÌÆ® Á¤¼ö.
    * @param		storageinfo	PTP ½ºÆå¿¡¼­ Á¤ÀÇÇÑ ÀúÀåÀåÄ¡ ±¸Á¶Ã¼·Î ¿ë·®, ¿©À¯°ø°£ Å©±â, º¼·ý¸í ÆÄÀÏ½Ã½ºÅÛ Å¸ÀÔµîÀÇ Á¤º¸¸¦ ÀúÀåÇÔ.
    * @return		¾Æ·¡ ÇÔ¼ö°¡ ¼º°øÀûÀ¸·Î ¼öÇàµÇ¸é PTP_RC_OK ¸¦ ¸®ÅÏÇÏ°í,
    				¼öÇà µµÁß¿¡ ¿¡·¯¸¦ ¸¸³ª¸é PTP_RC_Undefined ¸¦ ¸®ÅÏÇÔ.
    * @see		ptp_opensession ´Â usb handleÀ» openÇÑ ÈÄ ptp sessionÀ» openÇÒ¶§ »ç¿ëÇÔ.
    				ptp_getstorageinfo´Â PTP ½ºÆå¿¡¼­ Á¤ÀÇÇÑ storageinfo Á¤º¸¸¦ °¡Á®¿À±â À§ÇÑ API.
    				ptp_closesession ´Â openµÈ ptp sessionÀ» closeÇÒ¶§ »ç¿ëÇÔ.
*/

 uint16 PTP_API_Get_StorageInfos(uint32_t storageid, PTPStorageInfo* storageinfo)
 { 
#if 0
 	uint16  result = PTP_RC_OK;
 	
 	if (PTP_API_CHKDEV() != true)
 		return PTPAPI_ERROR; //just representing status error
 	
 	PTP_Init_Transaction (&gdc);
 	
 	PTP_Debug_Print("PTP_API_Get_StorageInfos : start\n");
 	
 	PTP_BEGIN;
 	
 	result = ptp_getstorageinfo (&(gdc.params), storageid, storageinfo);
 	if( result != PTP_RC_OK)
 	{
 		PTP_Debug_Print("[Error] PTP_API_Get_StorageInfos : ptp_getstorageinfo return (%x)\n", result);
 		PTP_END;
 		return result;
 	}
 	
 	PTP_END;
 	
 	PTP_Debug_Print("PTP_API_Get_StorageInfos : end(%x)\n\n", result);
 	
 	return result;
#endif
	return PTPAPI_ERROR;
 }

/**
* @brief	      Get storage information of the given storage id from the given  device
* @remarks		none
* @param		PTPStorageInfo* storageinfo:	structure for storage infomation		
* @param	      PTPDeviceHandle ptpHandle:	Handle for the given ptp device
* @return		uint16
PTP_RC_OK : success, else : failure
*/
uint16 PTP_API_Get_StorageInfos_MPTP(PTPDeviceHandle  ptpHandle, uint32_t storageid, PTPStorageInfo* storageinfo)
{
	uint16  result = PTP_RC_OK;
	PTPDevContext *dc = NULL;

	dc = getDc(ptpHandle);
	if(dc == NULL)
	{
		PTP_Debug_Print("[ERROR] PTP_API_Get_StorageInfos_MPTP : PTPDevContext is null.\n");
		return PTPAPI_ERROR;//added by Niraj   
	}

	if (PTP_API_CHKDEV_MPTP(ptpHandle) != true)
		return PTPAPI_ERROR; //just representing status error
	
	PTP_Init_Transaction (dc);
	
	PTP_Debug_Print("PTP_API_Get_StorageInfos : start\n");
	
	PTP_BEGIN;	
	
	if(PTP_API_CheckValidStorageID(storageid) ==PTP_RC_OK)
	{
		result = ptp_getstorageinfo (&(dc->params), storageid, storageinfo);
		if( result != PTP_RC_OK)
		{
			  
			PTP_Debug_Print("[Error] PTP_API_Get_StorageInfos : ptp_getstorageinfo return (%x)\n", result);
			PTP_END;
			return result;
		}
	}
	else
	{
		PTP_END;
		return PTP_RC_Undefined;
	}
	
	PTP_END;
	
	PTP_Debug_Print("PTP_API_Get_StorageInfos : end(%x)\n\n", result);
	
	return result;
}

/**
    * @brief	PTP_API_Send_Reset ÇÔ¼ö´Â usb device ¿¡°Ô Reset ¸í·ÉÀ» º¸³¾¶§ »ç¿ëÇÔ.
    * @remarks	usb device ÀåºñµéÀÌ host ¿¡ ¿¬°áµÇ¸é host´Â ÀÓÀÇ ½ÃÁ¡¿¡ Reset ¸í·ÉÀ» º¸³»¼­
    				usb device ¿¡°Ô ÃÊ±âÈ­ ÇÒ  °ÍÀ» ¸í·ÉÇÔ.

  -----------------
  | usb handle open  |
  -----------------
  |
  V
  -----------------
  | send reset cmd    |
  -----------------
  |
  V
  -----------------
  | usb handle close  |
  -----------------
  
    * @param	¾øÀ½.
    * @return	¾Æ·¡ ÇÔ¼ö°¡ ¼º°øÀûÀ¸·Î ¼öÇàµÇ¸é PTP_RC_OK ¸¦ ¸®ÅÏÇÏ°í,
    		¼öÇà µµÁß¿¡ ¿¡·¯¸¦ ¸¸³ª¸é PTP_RC_Undefined ¸¦ ¸®ÅÏÇÔ.
    * @see	PTP_Init_Transaction ÇÔ¼ö´Â usb/ptp Åë½Åµ¿¾È »ç¿ëÇÑ º¯¼ö ÃÊ±âÈ­.
    		PTP_Open_Device ÇÔ¼ö´Â usb handleÀ» open ÇÔ.
    		get_usb_endpoint_status ÇÔ¼ö´Â bulk in/bulk out/interrupt endpointÀÇ »óÅÂ¸¦ ÀÐ¾î¿È.
    		usb_clear_stall ÇÔ¼ö´Â bulk in/bulk out/interrupt endpointÀÇ ¿¬°áÀ» ²÷À½.
    		set_usb_device_reset ÇÔ¼ö´Â usb device¿¡°Ô Reset ¸í·ÉÀ» º¸³¿.
    		PTP_Close_Device ÇÔ¼ö´Â usb handleÀ» ´ÝÀ½.
    		PTP_Terminate_Transaction ÇÔ¼ö´Â usb/ptp Åë½Åµ¿¾È »ç¿ëÇÑ ¸Þ¸ð¸® ÇØÁ¦.
*/
 uint16 PTP_API_Send_Reset(void)
 {
#if 0
 	
 	uint16  result = PTP_RC_OK;
 	uint16  status = 0;
 	uint16  device_status[2] = {0,0};
 	
 	PTP_Init_Transaction (&gdc);
 	PTP_Debug_Print("PTP_API_Send_Reset : start\n");
 	
 	PTP_BEGIN;
 	
 	result = get_usb_endpoint_status(&gdc, &status);
 	if (result != PTP_RC_OK)
 	{
 		PTP_Debug_Print("[Error] PTP_API_Send_Reset : usb_get_endpoint_status return (%x)\n", result);
 		PTP_END;
 		return PTPAPI_ERROR; //just representing status error
 	}
 	else
 	{
 		if (status)
 		{
 			result = usb_clear_stall(&gdc);
 			if (result != PTP_RC_OK)
 			{
 				PTP_Debug_Print("[Error] PTP_API_Send_Reset : usb_clear_stall_feature return (%x)\n", result);
 				PTP_END;
 				return PTPAPI_ERROR; //just representing status error
 			}
 		}
 	}
 	
 	result = get_usb_device_status(&gdc, device_status);
 	if (result != PTP_RC_OK )
 	{
 		PTP_Debug_Print ("[Error] PTP_API_Send_Reset : get_usb_device_status return fail.\n");
 		PTP_END;
 		return PTPAPI_ERROR; //just representing status error
 	}
 	else
 	{
 		PTP_Debug_Print ("PTP_API_Send_Reset : get_usb_device_status return(0x%04x/0x%04x)\n", device_status[0], device_status[1]);
 		if (device_status[0] == PTP_RC_OK)
 		{
 			PTP_Debug_Print ("PTP_API_Send_Reset : device status OK\n");
 		}
 		else
 		{
 			PTP_Debug_Print ("[Error] PTP_API_Send_Reset : device status(0x%04x/0x%04x)\n",device_status[0], device_status[1]);
 			PTP_END;
 			return PTPAPI_ERROR; //just representing status error
 		}
 	}
 	
 	result = set_usb_device_reset(&gdc);
 	if (result != PTP_RC_OK)
 	{
 		PTP_Debug_Print ("[Error] PTP_API_Send_Reset : set_ptp_device_reset return(0x%4x)\n", result);
 		PTP_END;
 		return PTPAPI_ERROR; //just representing status error
 	}
 	
 	result = get_usb_device_status(&gdc, device_status);
 	if (result != PTP_RC_OK )
 	{
 		PTP_Debug_Print ("[Error] PTP_API_Send_Reset : get_usb_device_status return(%x)\n", device_status[0]);
 		PTP_END;
 		return PTPAPI_ERROR; //just representing status error
 	}
 	else
 	{
 		if (device_status[0] == PTP_RC_OK)
 		{
 			PTP_Debug_Print ("PTP_API_Send_Reset : device status OK\n");
 		}
 	}
 	
 	PTP_END;
 	
 	PTP_Debug_Print("PTP_API_Send_Reset : end(%x)\n\n", result);
 	
 	return result;
#endif
	return PTPAPI_ERROR;
 }

/**
* @brief	      sends reset command to the given ptp device
* @remarks		none
* @param	      PTPDeviceHandle ptpHandle:	Handle for the given ptp device
* @return		uint16
PTP_RC_OK : success, else : failure
*/ 
uint16 PTP_API_Send_Reset_MPTP(PTPDeviceHandle  ptpHandle)
{
	
	uint16	result = PTP_RC_OK;
	uint16	status = 0;
	uint16	/*device_status[2]*/device_status[4] = {0,0};

	PTPDevContext *dc;
	dc=getDc(ptpHandle);
	
	if(dc==NULL)
	{
		PTP_Debug_Print("[ERROR] PTP_API_Send_Reset_MPTP : PTPDevContext is null.\n");
		return PTPAPI_ERROR;//added by Niraj   
	}
	PTP_Init_Transaction (dc);
	PTP_Debug_Print("PTP_API_Send_Reset : start\n");
	PTP_BEGIN;		
	      
	result = get_usb_endpoint_status(dc, &status);
	if (result != PTP_RC_OK)
	{
		PTP_Debug_Print("[Error] PTP_API_Send_Reset : usb_get_endpoint_status return (%x)\n", result);
		PTP_END;
		return PTPAPI_ERROR; //just representing status error
	}
	else
	{
		if (status) 
		{
			result = usb_clear_stall(dc);
			if (result != PTP_RC_OK)
			{
				PTP_Debug_Print("[Error] PTP_API_Send_Reset : usb_clear_stall_feature return (%x)\n", result);
				PTP_END;
				return PTPAPI_ERROR; //just representing status error
			}
		}
	}	
	
	result = get_usb_device_status(dc, device_status);
	if (result != PTP_RC_OK )
	{
		PTP_Debug_Print ("[Error] PTP_API_Send_Reset : get_usb_device_status return fail.\n");
		PTP_END;
		return PTPAPI_ERROR; //just representing status error
	}
	else
	{
		PTP_Debug_Print ("PTP_API_Send_Reset : get_usb_device_status return(0x%04x/0x%04x)\n", device_status[0], device_status[1]);
		if (/*device_status[0]*/device_status[3] == PTP_RC_OK) //vishal
		{
			PTP_Debug_Print ("PTP_API_Send_Reset : device status OK\n");
		}
		else
		{
			PTP_Debug_Print ("[Error] PTP_API_Send_Reset : device status(0x%04x/0x%04x)\n",device_status[0], device_status[1]);
			PTP_END;			
			return PTPAPI_ERROR; //just representing status error
		}
	}
	
	result = set_usb_device_reset(dc);
	if (result != PTP_RC_OK)
	{
		PTP_Debug_Print ("[Error] PTP_API_Send_Reset : set_ptp_device_reset return(0x%4x)\n", result);
		PTP_END;		
		return PTPAPI_ERROR; //just representing status error
	}
	
	result = get_usb_device_status(dc, device_status);
	if (result != PTP_RC_OK )
	{
		PTP_Debug_Print ("[Error] PTP_API_Send_Reset : get_usb_device_status return(%x)\n", device_status[0]);
		PTP_END;		
		return PTPAPI_ERROR; //just representing status error
	}
	else
	{
		if (/*device_status[0]*/device_status[3] == PTP_RC_OK) 
		{
			PTP_Debug_Print ("PTP_API_Send_Reset : device status OK\n");
		}
	}
	
	PTP_END;
	
	PTP_Debug_Print("PTP_API_Send_Reset : end(%x)\n\n", result);
	
	return result;
}



/**

  * @brief  
  
         : Storage IDÀÇ À¯È¿¼ºÀ» ÆÇ´ÜÇÑ´Ù (physical,logical masking °ªÀÌ Á¸ÀçÇÏ¸é À¯È¿ÇÑ storageID ÀÌ´Ù)
	
	  * @remarks 
	  
		* @param    AlDevContext *dc
		
		  * @return   
		  
			* @see
			
*/
uint16  PTP_API_CheckValidStorageID(unsigned int storageID)

{
	
    if(storageID & PTP_PHYSICAL_STORAGE_MASK)
    {
        if(storageID & PTP_LOGICAL_STORAGE_MASK)
        {
            PTP_Debug_Print("PTP_LOGICAL_STORAGE existing\n ");
            return PTP_RC_OK;
        }
        else
        {
            PTP_Debug_Print("PTP_LOGICAL_STORAGE not existing\n ");
            return PTPAPI_ERROR; //just representing status error;          
        }
    }
    else
    {
        PTP_Debug_Print("PTP_PHYSICAL_STORAGE not existing\n ");
        return PTPAPI_ERROR; //just representing status error
    }
}


/**
* @brief         This function maps the ptpHandle of a ptp device to its DeviceContext
* @remarks       pDC is set to NULL if the device is not in the list
* @param         PTPDeviceHandle ptpHandle - ptpHandle of the camera  
* @return		 PTPDevContext*:  
				 DeviceContext of the respective Handle
* @see           
*/
static PTPDevContext *getDc(PTPDeviceHandle  ptpHandle)
{ 
	PTPDevContext* pDC;

	CPTPDeviceListUpdate::Get().GetPTPDeviceContext(ptpHandle, &pDC);

	return pDC;
}

/**
* @brief	    fetches the DeviceList of the connected PTP Devices
* @remarks		none
* @param		UsbDev* deviceList[]    :Device List
* @param		uint32_t* count:	number of ptp Devices
* @return		void
*/ 
void PTP_API_GetDeviceList(UsbDev* deviceList[],uint32_t* count)
{
#define dev_max_cnt 20

	CPTPDeviceListUpdate::Get().GetPTPDeviceList(dev_max_cnt, deviceList, count);
}	
	
    

#endif
