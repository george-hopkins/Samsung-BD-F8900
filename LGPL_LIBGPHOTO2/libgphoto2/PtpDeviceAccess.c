/** 
* @file	PTPDeviceAccess.c
* @brief	This source file is used for accessing PTP Devices through USB interfac
*
* @author	Sandeep Kumar(SISC)
* @Date	    January 8, 2010

* Copyright 2010 by Samsung Electronics, Inc.,
* 
* This software is the confidential and proprietary information
* of Samsung Electronics, Inc. ("Confidential Information").  You
* shall not disclose such Confidential Information and shall use
* it only in accordance with the terms of the license agreement
* you entered into with Samsung.
*
*
*/

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "PtpTypes.h"
#include "PtpDeviceAccess.h"

#ifdef CSP_LAYER_USAGE
/* For CSP Layer compatibility */
#include "PCThread.h"
#include "PCDebug.h"
/* Mutex for Device Access functions */
static PCMutex 					gLibusbCntxtMutex ; 		/*Mutex*/
#define DEVICE_ACCESS_CREATE		gLibusbCntxtMutex.Create();
#define DEVICE_ACCESS_DESTROY	gLibusbCntxtMutex.Destroy();
#define DEVICE_ACCESS_LOCK		gLibusbCntxtMutex.Lock();
#define DEVICE_ACCESS_UNLOCK	gLibusbCntxtMutex.UnLock();

#else
#include <pthread.h>
#include "assert.h"
#ifndef ASSERT
#define ASSERT(x) 				(assert(x))	
#endif
/* Mutex for Device Access functions */
static pthread_mutex_t 			gLibusbCntxtMutex = PTHREAD_MUTEX_INITIALIZER; /*Function entry critical section*/
#define DEVICE_ACCESS_LOCK		pthread_mutex_lock(&gLibusbCntxtMutex)
#define DEVICE_ACCESS_UNLOCK	pthread_mutex_unlock(&gLibusbCntxtMutex)
#endif


/* MACROS */
#define PTPDEVACC_ERR(e) (-( errno = PTPDevAccConErrNo(e) ) )
#define PTP_SUCCESS 0
#define USBLIBDBG

/* Global Variables */
static libusb_context *pLibUsbCtx = NULL;	/* libusb Context */
static PTP_CALLBACK cbOnDeviceChange = 0;	/* Call back for Device change handling */
static int PTPDevAccConErrNo(int iResult);         /* Function for Error code parsing */

/* Static Functions */

/**
* @fn 	    static void ClearUsbDevice(struct stUsbDevice *pUsbDev)
* @brief	Clears Dev Config descriptor 
* 
* @param	(in) pUsbDev
* @param	(in) iConfigCount :: configurations to be cleared
* @return 	None
*/
static void ClearUsbDevice(struct stUsbDevice *pUsbDev, int iConfigCount)
{
     int iCount = 0;
    if(!pUsbDev)
    {
/* If device pointer is NULL, return */
        PTP_DRV_ERR("\nNo memory \n");
        ASSERT(0);	
        return ;
    }
	 if(!(pUsbDev->ppConfigDesc))
    {
  /* If array is NULL, return */
        ASSERT(0);	
        return ;
    }	
    	
    while(iCount < iConfigCount)
    {
        /* Free all the configuration descriptors */
       	libusb_free_config_descriptor(pUsbDev->ppConfigDesc[iCount] ); 	
		iCount++;
    }
    if(pUsbDev->ppConfigDesc)
    {
        /* Free the array of descriptors */
        PTPFreeMem(pUsbDev->ppConfigDesc);       
        pUsbDev->iLastSelectedConfig = -1;
    }
}

/**
* @fn 		  static int InitUsbDevice(struct stUsbDevice *pUsbDev)
* @brief	  Initializes the Usb device
* 
* @param	  (in) pUsbDev
* @return	  Error code
*/
static int InitUsbDevice(struct stUsbDevice *pUsbDev)
{
     libusb_device *pDevice = NULL;
    int iConfigCount = 0;
    int iRetVal = 0;

    if(!pUsbDev)	
    { 
         /* If device pointer is NULL, return */
        PTP_DRV_ERR("\nNo memory \n");
        ASSERT(0);		
        return -ENOMEM;
    }
    pDevice = (struct libusb_device *)pUsbDev->pvDev;

    iRetVal = libusb_get_device_descriptor(pDevice,
        &pUsbDev->deviceDesc);
    if (iRetVal < 0) 
    {
        return PTPDEVACC_ERR(iRetVal);
    }

    iConfigCount = pUsbDev->deviceDesc.bNumConfigurations;
    /* Allocate the memory for confiration descriptors  */
    pUsbDev->ppConfigDesc = (struct libusb_config_descriptor **) \
        malloc(iConfigCount * sizeof(struct libusb_config_descriptor *)  );

    if(!pUsbDev->ppConfigDesc)
    {
        PTP_DRV_WARN("\nConfig Descriptor Error  \n");
        return ENOMEM;
    }
    /* Initialize the memory with 0 */
	memset(pUsbDev->ppConfigDesc, 0, iConfigCount * sizeof(struct libusb_config_descriptor *));

    int iCount = 0;
    while (iCount < iConfigCount)
    {
        /* Get the configuration descriptors */
        iRetVal= libusb_get_config_descriptor(pDevice, iCount,\
            &(pUsbDev->ppConfigDesc[iCount]) );	
        if (iRetVal < 0)
        {
            break;
        }
        iCount++;
    }

    if(iRetVal >= 0)	
    {
        /* Success case */
        libusb_ref_device(pDevice);
        pUsbDev->iLastSelectedConfig = 0;   	
        return PTP_SUCCESS;
    }
    else
    {
        /* Failure case */
        
        ClearUsbDevice(pUsbDev, iCount);            
        return PTPDEVACC_ERR(iRetVal);
    }
}

/**
* @fn 		 static void FreeUsbDevice(struct stUsbDevice *pUsbDev)
* @brief	 This function is used to free the USB device
* 
* @param	 (in) dev
* @return	 Error code
*/
static void FreeUsbDevice(struct stUsbDevice *pUsbDev)
{
    ASSERT(pUsbDev);
    if(pUsbDev)
    {
        /* If device pointer is not NUL, free all the decriptors */
       	
        ClearUsbDevice(pUsbDev, pUsbDev->deviceDesc.bNumConfigurations);
       libusb_unref_device((struct libusb_device *)pUsbDev->pvDev);
        PTPFreeMem(pUsbDev->dirPath);
        PTPFreeMem(pUsbDev->filePath);
        PTPFreeMem(pUsbDev);        
    }
}
/**
* @fn 		static int DoUsbBulkTransfer(PUSB_DEVICE pDev, int iEp, char *pBytes,
int iSize, int iTimeout)
* @brief	This function is used to transfer the data through Bulk mechanism..
* @param	(in) pDev
*		    (in) iEp
*			(out) pBytes
*			(in) iSize
*			(in) iTimeout
* @return	Error code
*/   
static int DoUsbBulkTransfer(PUSB_DEVICE pDev, int iEp, char *pBytes,
                             int iSize, int iTimeout)
{
    int iActLen = 0;
    int iRetVal = 0;

    if(!pBytes)	
    {  
        /* If pointer to buffer is NULL, return */
        PTP_DRV_ERR("\nNo memory\n");
        ASSERT(0);	
        return -ENOMEM;
    }

    /* call the libusb for bulk transfer */
    iRetVal = libusb_bulk_transfer(pDev->handle, iEp & 0xff, \
        (unsigned char *)pBytes, iSize,
        &iActLen, iTimeout);
    if ((0 == iRetVal) || (iActLen > 0 && LIBUSB_ERROR_TIMEOUT == iRetVal ))
    {
        /* Success case */
        return iActLen;
    }
     if(LIBUSB_ERROR_PIPE == iRetVal || LIBUSB_ERROR_NO_DEVICE == iRetVal || LIBUSB_ERROR_OTHER == iRetVal)
    {
	PTP_DRV_WARN("\n<============= Error =============> n");
        /* 500 milli-second delay*/
        usleep(500*1000); 
    }	

    return PTPDEVACC_ERR(iRetVal);
}
/**
* @fn 		static int DoUsbInterruptTransfer(PUSB_DEVICE pDev, int iEp, char *pBytes,
int iSize, int iTimeout)
* @brief	This function is used to transfer the data through interrupt mechanism.
* @param	(in) pDev
*			(in) iEp
*			(out) pBytes
*			(in) iSize
*			(in) iTimeout
* @return	Error code
*/ 

static int DoUsbInterruptTransfer(PUSB_DEVICE pDev, int iEp, char *pBytes,
                                  int iSize, int iTimeout)
{
    int iActLen = 0;
    int iRetVal = 0;
    if(!pBytes)	
    {
        /* If pointer to buffer is NULL, return */
        PTP_DRV_ERR("\nNo memory \n");
        ASSERT(0);		
        return -ENOMEM;
    }

    /* call the libusb for Interrupt transfer */
    iRetVal = libusb_interrupt_transfer(pDev->handle, iEp & 0xff, \
        (unsigned char *)pBytes, iSize,
        &iActLen, iTimeout);
    if ((0 == iRetVal) || (iActLen > 0 && LIBUSB_ERROR_TIMEOUT == iRetVal ))
    {
        /* Success case */
        return iActLen;
    }
    return PTPDEVACC_ERR(iRetVal);
}

/**
* @fn     	static int PTPDevAccConErrNo(int iResult)
* @brief 	Returns USB Errors
* @param	(in) iResult
* @return     Error code
*/
static int PTPDevAccConErrNo(int iResult)
{
    /* Send the correct code for Libusb error code */
    switch (iResult) 
    {   
        case LIBUSB_SUCCESS:
            return PTP_SUCCESS;
        case LIBUSB_ERROR_NOT_FOUND:
            return ENOENT;
        case LIBUSB_ERROR_NO_DEVICE:
            return ENXIO;		
        case LIBUSB_ERROR_BUSY:
            return EBUSY;
        case LIBUSB_ERROR_PIPE:
            return EPIPE;
        case LIBUSB_ERROR_INTERRUPTED:
            return EINTR;
        case LIBUSB_ERROR_NOT_SUPPORTED:
            return ENOSYS;
        case LIBUSB_ERROR_INVALID_PARAM:
            return EINVAL;
        case LIBUSB_ERROR_OVERFLOW:
            return EOVERFLOW;
        case LIBUSB_ERROR_TIMEOUT:
            return ETIMEDOUT;		
        case LIBUSB_ERROR_NO_MEM:
            return ENOMEM;	
        case LIBUSB_ERROR_IO:
            return EIO;    
        case LIBUSB_ERROR_ACCESS:
            return EACCES;    		
        default:
            return ERANGE;
    }
}

/**
* @fn 		void PTPDevUsbInit(void)
* @brief	Intializes the PTP Device through USB Interface
* @param	None	  
* @return  	None
*/
void PTPDevUsbInit(void)
{
    int iRetVal = 0;
    if (!pLibUsbCtx) 
    {
        /*Initialized only once*/
        iRetVal = libusb_init(&pLibUsbCtx);
        if (iRetVal < 0) 
        {
            PTP_DRV_WARN("LibusbInit Error\n");
            return;
        }

#ifdef USBLIBDBG
        libusb_set_debug(pLibUsbCtx, 3);        
#endif

    }
}

/**
* @fn 		void PTPDevUsbExit(void)
* @brief	Intializes the PTP Device through USB Interface
* @param	None	  
* @return  	None
*/
void PTPDevUsbExit(void)
{
    PUSB_DEVICE* pstUsbDevices = NULL ;
    PUSB_DEVICE pTempDev = NULL;

    pstUsbDevices = PTPDevUsbGetDevices();    

    /* Free the list of devices on the list */
    for( PUSB_DEVICE pUsbDev = *pstUsbDevices; NULL != pUsbDev; )
    {	
        pTempDev = pUsbDev->next;
        DELETE_NODE((*pstUsbDevices), pUsbDev);
            FreeUsbDevice(pUsbDev);		
        pUsbDev = pTempDev;		
    }
    if (pLibUsbCtx) 
    {
        /*DeInitialize*/
	/*2010-11-09, when multi-application should be call libusb_exit, App's owned libusbcontext could be release. */	
        libusb_exit(pLibUsbCtx);
        pLibUsbCtx = NULL;
    }
    
    return;
}

/**
* @fn 		int  PTPDevUsbOpen(void * pvDev , PUSB_DEVICE pUsbDev)
* @brief		Opens the PTP Device through USB Interface.
* @param	(in)pvDev, Pointer to libusb_device Structure
*@param		(out)pUsbDev, Pointer to USB-Dev Structure
* @return       Device Handle
*/
int  PTPDevUsbOpen(void *pvDev , PUSB_DEVICE pUsbDev)
{
    int iRetVal = 0;
    ASSERT(pUsbDev);

    if ((NULL == pUsbDev) || (NULL == pvDev))
    {
        ASSERT(0);        
        return -1;
    }

    /*Initialize the structure to 0, to avoid Garbage values*/
    memset(pUsbDev, 0, sizeof(USB_DEVICE));

    /*call the libusb open for the device */
    iRetVal = libusb_open((libusb_device *) pvDev,&pUsbDev->handle);
    if (iRetVal < 0)
    {
        PTP_DRV_WARN("Libusb Open Error\n");
        errno = PTPDevAccConErrNo(iRetVal);
        return -1;
    }

    pUsbDev->iLastClaimedInfterface = -1;

    return 0;
}

/**
* @fn 		int PTPDevUsbClose(PUSB_DEVICE pUsbDev)
* @brief	Closes the PTP Device through USB Interface.
* @param	(in)pUsbDev,Pointer to USB-Dev Handle
* @return 	Error code
* @see		None
*/
int PTPDevUsbClose(PUSB_DEVICE pUsbDev)
{
    ASSERT(pUsbDev);	
    /*call the libusb close for the device */
    libusb_close(pUsbDev->handle);
    return PTP_SUCCESS;
}

/**
* @fn 		int PTPDevUsbSetCfg(PUSB_DEVICE pUsbDevHandle, int iConfig)
* @brief	Gets PTP Device through USB Interface.
* @param	(in)pdev,Pointer to USB-Dev Handle
* @return 	Pointer to USB Dev structure
*		 	pdev->device
*/
int PTPDevUsbSetCfg(PUSB_DEVICE pUsbDevHandle, int iConfig)
{
    ASSERT(pUsbDevHandle);
    return PTPDEVACC_ERR(libusb_set_configuration(pUsbDevHandle->handle, iConfig));
}

/**
* @fn 		int PTPDevUsbClaimInterface(PUSB_DEVICE pUsbDevHandle, int iIntFace)
* @brief	Claims PTP Device USB-Interface .
* @param	(in)pUsbDevHandle,Pointer to USB-Dev Handle
*			(in)iIntFace, interface number
* @return 	Error code
*/
int PTPDevUsbClaimInterface(PUSB_DEVICE pUsbDevHandle, int iIntFace)
{
    int iRetVal = 0;
    ASSERT(pUsbDevHandle);

    /*call the libusb for claiming the interface */
    iRetVal = libusb_claim_interface(pUsbDevHandle->handle, iIntFace);
    if (0 == iRetVal) 
    {
        pUsbDevHandle->iLastClaimedInfterface = iIntFace;
        return PTP_SUCCESS;
    }

    return PTPDEVACC_ERR(iRetVal);
}

/**
* @fn 		int PTPDevUsbRelInterface(PUSB_DEVICE pUsbDevHandle, int iIntFace)
* @brief	Releases Claimed PTP Device USB-Interface .
* @param	(in) pUsbDevHandle,Pointer to USB-Dev Handle
*			(in)iIntFace,interface number
* @return 	Error code
*/
int PTPDevUsbRelInterface(PUSB_DEVICE pUsbDevHandle, int iIntFace)
{
    int iRetVal = 0;
    ASSERT(pUsbDevHandle);
    /*call the libusb for releasing the interface */
    iRetVal = libusb_release_interface(pUsbDevHandle->handle, iIntFace);
    if (0 == iRetVal)
    {
        pUsbDevHandle->iLastClaimedInfterface = -1;
    }

    return PTPDEVACC_ERR(iRetVal);
}

/**
* @fn 		int PTPDevUsbSetAltInterface(PUSB_DEVICE pUsbDevHandle, int iAltIntFace)
* @brief	Releases Claimed PTP Device USB-Interface .
* @param	(in) pUsbDevHandle,Pointer to USB-Dev Handle
*			(in)iAltIntFace,interface number
* @return 	Error code
*/
int PTPDevUsbSetAltInterface(PUSB_DEVICE pUsbDevHandle, int iAltIntFace)
{
    ASSERT(pUsbDevHandle);
    if (pUsbDevHandle->iLastClaimedInfterface < 0)
    {
        return -(errno=EINVAL);
    }

    return PTPDEVACC_ERR(libusb_set_interface_alt_setting(pUsbDevHandle->handle,
        pUsbDevHandle->iLastClaimedInfterface, iAltIntFace));
}

/**
* @fn 	    int PTPDevUsbResetep(PUSB_DEVICE pUsbDevHandle, unsigned int uiEp)
* @brief	Resets Endpoint PTP Device through USB Interface .
* @param	(in)pUsbDevHandle,Pointer to USB-Dev Handle
*			(in)uiEp, EndPoint Number
* @return 	Error code
*/
int PTPDevUsbResetep(PUSB_DEVICE pUsbDevHandle, unsigned int uiEp)
{
    ASSERT(pUsbDevHandle);
    return PTPDEVACC_ERR(PTPDevUsbClrHalt(pUsbDevHandle, uiEp));
}

/**
* @fn 	     int PTPDevUsbClrHalt(PUSB_DEVICE pUsbDevHandle, unsigned int uiEp)
* @brief	 Resets Endpoint PTP Device through USB Interface .
* @param     (in)pUsbDevHandle,Pointer to USB-Dev Handle
*		     (in)uiEp, EndPoint Number
* @return 	 Error code
*/
int PTPDevUsbClrHalt(PUSB_DEVICE pUsbDevHandle, unsigned int uiEp)
{
    ASSERT(pUsbDevHandle);
    return PTPDEVACC_ERR(libusb_clear_halt(pUsbDevHandle->handle, uiEp & 0xff));
}

/**
* @fn 	    int PTPDevUsbReset(PUSB_DEVICE pUsbDevHandle)
* @brief	Resets PTP -USB Device.
* @param	(in)pUsbDevHandle,Pointer to USB-Dev Handle
* @return 	Error code
*/
int PTPDevUsbReset(PUSB_DEVICE pUsbDevHandle)
{
    ASSERT(pUsbDevHandle);
    return PTPDEVACC_ERR(libusb_reset_device(pUsbDevHandle->handle));
}

/**
* @fn 	    int PTPDevUsbBulkRd(PUSB_DEVICE pUsbDevHandle, int iEp, char *pBytes,int iSize, int iTimeout)
* @brief	Reads PTP-USB Device Bulk data.
* @param	(in)pUsbDevHandle,Pointer to USB-Dev Handle
*			(in)iEp, EndPoint Number
*			(out)pBytes,Data to be Read
*			(in)iSize, Data Size
*			(in)iTimeout, Time Out Value
* @return 	Error code
*/
int PTPDevUsbBulkRd(PUSB_DEVICE pUsbDevHandle, int iEp, char *pBytes,int iSize, int iTimeout)
{
    ASSERT(pUsbDevHandle);
    /* Check if MSb is set or not for Reading from bulk pipe */
    if (!(iEp & LIBUSB_ENDPOINT_IN)) 
    {
        /* Set the bit for Reading from bulk pipe */
        iEp |= LIBUSB_ENDPOINT_IN;
    }

    return DoUsbBulkTransfer(pUsbDevHandle, iEp, pBytes, iSize, iTimeout);
}


/**
* @fn 	    int PTPDevUsbBulkWr(PUSB_DEVICE pUsbDevHandle, int iEp, char *pBytes,int iSize, int iTimeout)
* @brief	Writes PTP-USB Device Bulk data.
* @param	(in)pUsbDevHandle,Pointer to USB-Dev Handle
*			(in)iEp, EndPoint Number
*			(out)pBytes,Data to Write
*			(in)iSize,Data Size
*			(in)iTimeout, Time Out Value
* @return 	Error code
*/
int PTPDevUsbBulkWr(PUSB_DEVICE pUsbDevHandle, int iEp, char *pBytes,int iSize, int iTimeout)
{
    ASSERT(pUsbDevHandle);
    /* Check if MSb is set or not for writing to bulk pipe */
    if (iEp & LIBUSB_ENDPOINT_IN)
    {        
        /* Clear the bit for writing to bulk pipe */
        iEp &= ~LIBUSB_ENDPOINT_IN;
    }
    return DoUsbBulkTransfer(pUsbDevHandle, iEp, pBytes, iSize, iTimeout);
}

/**  
* @fn 	    int PTPDevUsbIntrRd(PUSB_DEVICE pUsbDevHandle, int iEp, char *pBytes,int iSize, int iTimeout)
* @brief	Read PTP-USB Device Interrupt data.
* @param	(in) pUsbDevHandle,Pointer to USB-Dev Handle
*			(in)iEp, EndPoint Number
*			(out)pBytes,Data to read
*			(in)iSize, Data Size
*			(in)iTimeout,Time Out Value
* @return 	Error code
*/
int PTPDevUsbIntrRd(PUSB_DEVICE pUsbDevHandle, int iEp, char *pBytes,int iSize, int iTimeout)
{
    ASSERT(pUsbDevHandle);
    /* Check if MSb is set or not for Reading from Interrupt pipe */
    if (!(iEp & LIBUSB_ENDPOINT_IN)) 
    {        
        /* Set the bit for Reading from Interrupt pipe */
        iEp |= LIBUSB_ENDPOINT_IN;
    }
    return DoUsbInterruptTransfer(pUsbDevHandle, iEp, pBytes, iSize, iTimeout);
}

/**
* @fn 	    int PTPDevUsbIntrWr(PUSB_DEVICE pUsbDevHandle, int iEp, char *pBytes,int iSize, int iTimeout)
* @brief	Writes PTP-USB Device Interrupt data.
* @param	(in) pUsbDevHandle,Pointer to USB-Dev Handle
*			(in)iEp, EndPoint Number
*			(out)pBytes,Data to Write
*			(in)iSize,Data Size
*			(in)iTimeout,Time Out Value
* @return 	Error code
*/
int PTPDevUsbIntrWr(PUSB_DEVICE pUsbDevHandle, int iEp, char *pBytes,int iSize, int iTimeout)
{
    ASSERT(pUsbDevHandle);
    /* Check if MSb is set or not for writing to Interrupt pipe */
    if (iEp & LIBUSB_ENDPOINT_IN) 
    {        
        /* Clear the bit for writing to Interrupt pipe */
        iEp &= ~LIBUSB_ENDPOINT_IN;
    }
    return DoUsbInterruptTransfer(pUsbDevHandle, iEp, pBytes, iSize, iTimeout);
}

/**
* @fn 		int PTPDevUsbSendCtrlMsg(PUSB_DEVICE pUsbDevHandle, int ibmRequestType,
int ibReq, int iwValue, int iwIndex, char *pBytes, int iSize, int iTimeout)
* @brief	Sends PTP-USB DeviceControl Msg.
* @param	(in)pUsbDevHandle,Pointer to USB-Dev Handle
*			(in)ibmRequestType,Request Type
*			(in)ibReq, Request
*			(in)iwValue,Val
*			(in)iwIndex, Index Val
*			(out)pBytes, Data to Rd/Wr
*			(in)iSize,Data Size
*			(in)iTimeout, Time Out Value
* @return 	Error code
*/
int PTPDevUsbSendCtrlMsg(PUSB_DEVICE pUsbDevHandle, int ibmRequestType,\
                         int ibReq, int iwValue, int iwIndex, char *pBytes, int iSize, int iTimeout)
{
    int iRetVal = 0;
     ASSERT(pUsbDevHandle);
    /* Call the libusb for control transfer */
    iRetVal = libusb_control_transfer(pUsbDevHandle->handle, ibmRequestType & 0xff,\
        ibReq & 0xff, iwValue & 0xffff, iwIndex & 0xffff, \
        (unsigned char *)pBytes, iSize & 0xffff,iTimeout);

    if (iRetVal >= 0)
    {
        return iRetVal;
    }
    return PTPDEVACC_ERR(iRetVal);
}

/**
* @fn 		int PTPDevUsbGetStr(PUSB_DEVICE pUsbDevHandle, int iDescIndex, int iLangid,	char *pBuf, size_t BufLen)
* @brief	Gets Descriptor from PTP-USB Device.
* @param	(in) pUsbDevHandle,Pointer to USB-Dev Handle
*			(in)iDescIndex,Index
*			(in)iLangid-> Language ID
*			(out)pBuf,Buffer to Read
*			(in)BufLen, Buffer Length 
* @return 	Error code
*/
int PTPDevUsbGetStr(PUSB_DEVICE pUsbDevHandle, int iDescIndex, int iLangid,	char *pBuf, size_t BufLen)
{
    int iRetVal = 0;
    ASSERT(pUsbDevHandle);	
    /* Call the libusb for getting string descriptor */
    iRetVal = libusb_get_string_descriptor(pUsbDevHandle->handle, iDescIndex & 0xff,
        iLangid & 0xffff,(unsigned char *) pBuf, (int) BufLen);
    if (iRetVal >= 0)
    {
        return iRetVal;
    }
    return PTPDEVACC_ERR(iRetVal);
}

/**
* @fn 		int PTPDevUsbGetStrSimple(PUSB_DEVICE pUsbDevHandle, int iDescIndex,char *pBuf, size_t BufLen)
* @brief	Gets String Descriptor from PTP-USB Device.
* @param	(in)pUsbDevHandle,Pointer to USB-Dev Handle
*			(in)iDescIndex,Index
*			(out)pBuf,Buffer to Read
*			(in)BufLen, Buffer Length 
* @return 	Error code
*/
int PTPDevUsbGetStrSimple(PUSB_DEVICE pUsbDevHandle, int iDescIndex,char *pBuf, size_t BufLen)
{
    int iRetVal = 0;
    ASSERT(pUsbDevHandle);	
    iRetVal = libusb_get_string_descriptor_ascii(pUsbDevHandle->handle, iDescIndex & 0xff,
        (unsigned char *)pBuf, (int) BufLen);
    if (iRetVal >= 0)
    {
        return iRetVal;
    }
    return PTPDEVACC_ERR(iRetVal);
}

/**
* @fn 		int PTPDevUsbGetDesc(PUSB_DEVICE pUsbDevHandle, unsigned char uiType,unsigned char uiDescIndex, void *pBuf, int iSize)
* @brief	Gets Descriptor from PTP-USB Device.
* @param	(in)pUsbDevHandle,Pointer to USB-Dev Handle
*			(in)uiType, Desc. Type
*			(in)uiDescIndex,Index
*			(out)pBuf, Buffer to Read
*			(in)iSize, Buffer Size 
* @return 	Error code
*/
int PTPDevUsbGetDesc(PUSB_DEVICE pUsbDevHandle, unsigned char uiType,unsigned char uiDescIndex, void *pBuf, int iSize)
{
    int iRetVal = 0;
    ASSERT(pUsbDevHandle);	
    /* Call the libusb for getting descriptor */
    iRetVal = libusb_get_descriptor(pUsbDevHandle->handle, uiType, uiDescIndex, \
        (unsigned char *)pBuf, iSize);
    if (iRetVal >= 0)
    {
        return iRetVal;
    }
    return PTPDEVACC_ERR(iRetVal);
}

/**
* @fn 		int PTPDevUsbGetDescEp(PUSB_DEVICE pUsbDevHandle, int iEp,	unsigned char uiType, unsigned char uiDescIndex, void *pBuf, int iSize)
* @brief	Gets Descriptor from PTP-USB  Endpoint Device.
* @param	(in)pUsbDevHandle,Pointer to USB-Dev Handle
*			(in)uiType, Desc. Type
*			(in)uiDescIndex,Index
*			(out)pBuf, Buffer to Read
*			(in)iSize,Buffer Size 
* @return 	Error code
*/
int PTPDevUsbGetDescEp(PUSB_DEVICE pUsbDevHandle, int iEp,	unsigned char uiType, unsigned char uiDescIndex, void *pBuf, int iSize)
{
    int iRetVal = 0;
    ASSERT(pUsbDevHandle);	
    iRetVal= libusb_control_transfer(pUsbDevHandle->handle,
        LIBUSB_ENDPOINT_IN | (iEp & 0xff), LIBUSB_REQUEST_GET_DESCRIPTOR,
        (uiType << 8) | uiDescIndex, 0, (unsigned char *)pBuf, iSize, 1000);
    if (iRetVal >= 0)
    {
        return iRetVal;
    }
    return PTPDEVACC_ERR(iRetVal);
}

/**
* @fn	    int PTPDevUsbGetActiveDriver(PUSB_DEVICE pUsbDevHandle, int iIntFace,	char *pName, unsigned int uiNameLen)
* @brief 	Gets Active Driver PTP-USB Interface.
* @param    (in)pUsbDevHandle -Pointer to USB-Dev Handle
*		    (in)iIntFace -Interface Number
*		    (in)pName -Driver Name
*		    (in)uiNameLen- Name Length
* @return   Error code
*/
int PTPDevUsbGetActiveDriver(PUSB_DEVICE pUsbDevHandle, int iIntFace,	char *pName, unsigned int uiNameLen)
{
    ASSERT(pUsbDevHandle);
    int iRetVal = libusb_kernel_driver_active(pUsbDevHandle->handle, iIntFace);
    if (iRetVal == 1)
    {
        snprintf(pName, uiNameLen, "DUMMY");
        return PTP_SUCCESS;
    }
    else if (iRetVal == 0) 
    {
        return -(errno=ENODATA);
    }
    else
    {
        return PTPDEVACC_ERR(iRetVal);
    }
}

/**
* @fn		int PTPDevUsbDetachKernDrv(PUSB_DEVICE pUsbDevHandle, int iIntFace)
* @brief	Detache Active Kernel Driver through PTP-USB Interface.
* @param	(in) pUsbDevHandle -Pointer to USB-Dev Handle
*		    (in)iIntFace -Interface Number
* @return	Error  code
*/
int PTPDevUsbDetachKernDrv(PUSB_DEVICE pUsbDevHandle, int iIntFace)
{
    ASSERT(pUsbDevHandle);
    return PTPDEVACC_ERR(libusb_detach_kernel_driver(pUsbDevHandle->handle, iIntFace));
}

/**
* @fn	  	int PTPDevUsbAttchKernDriv(PUSB_DEVICE pUsbDevHandle, int iIntFace)
* @brief	Attach Kernel Driver through PTP-USB Interface.
* @param  	(in) pUsbDevHandle,Pointer to USB-Dev Handle
*		    (in) iIntFace,Interface Number
* @return	Error code
*/
int PTPDevUsbAttchKernDriv(PUSB_DEVICE pUsbDevHandle, int iIntFace)
{
    ASSERT(pUsbDevHandle);
    return PTPDEVACC_ERR(libusb_attach_kernel_driver(pUsbDevHandle->handle, iIntFace));
}

/**
* @fn	   	int PTPDevUsbGetDevices(void)
* @brief	Returns PTP Devices List
* @param   	None
* @return   PUSB_DEVICE * : pointer to list of devices
*/
PUSB_DEVICE* PTPDevUsbGetDevices(void)
{
    /* Static varibale for keeping the list of devices */
    /* This avoids the declaring the varibale as global */
    static PUSB_DEVICE g_pstUsbDevices = NULL;
    return &g_pstUsbDevices;
}


/**
* @fn	   	PTPDevFindPTPDevices(void)
* @brief	Finds PTP Devices through USB Interface.
* @param   	void
* @return   Error code
*/
int PTPDevFindPTPDevices(void)
{	
    libusb_device **ppUsbDevList = NULL;
    libusb_device *pLibUsbNewDev = NULL;
    PUSB_DEVICE pUsbDev = NULL;
    PUSB_DEVICE pTempDev = NULL;
    int iDevListLen = 0;
    int iRetVal = -1;
    int iCount = 0;
    int *iArrDevList = NULL;
    uint8_t uiDevNum = 0;
    uint8_t uiBusNum = 0;

    PUSB_DEVICE* pstUsbDevices = NULL;

    DEVICE_ACCESS_LOCK;

    /*If not intialized*/
    if (!pLibUsbCtx)
    {
        DEVICE_ACCESS_UNLOCK;
        return iRetVal;
    }	

    /* Get the current list of device list with PTP driver */
    pstUsbDevices = PTPDevUsbGetDevices();

    /* Get the current conncected device list */
    iDevListLen = libusb_get_device_list(pLibUsbCtx, &ppUsbDevList);
    if (iDevListLen < 0)
    {
        DEVICE_ACCESS_UNLOCK;
	  libusb_free_device_list(ppUsbDevList, 1);
        return PTPDEVACC_ERR(iDevListLen);
    }

    /* Create an array for keeping the status of newly connected devices and disconneced devices */
    iArrDevList = (int *)malloc(iDevListLen*sizeof(int));
    if(!iArrDevList)
    { 
        goto MEMALLOC_FAIL;
    }
    /* Memset the memory with 0 */
    memset(iArrDevList, 0, iDevListLen*sizeof(int));

    /* Compare all the devices on the PTP device list, with the latest device list received from libusb*/
    for(pUsbDev = *pstUsbDevices; NULL != pUsbDev; )
    {		
        int iDevFound = 0;
        /* Store the next device pointer */
        pTempDev = pUsbDev->next;

        for (iCount = 0;iCount < iDevListLen; iCount++)    	    		
        {	

            /* Compare the first device on PTP device list, with all the devices in the list received from libusb*/
            pLibUsbNewDev = ppUsbDevList[iCount];
            uiBusNum = libusb_get_bus_number(pLibUsbNewDev);	
            if(uiBusNum == pUsbDev->uiBusLoc)
            {
                uiDevNum = libusb_get_device_address(pLibUsbNewDev);
                if ( uiDevNum == pUsbDev->uiDevNum )
                {	
                    /* If device is found, means device is still connected, 
                    no need to take any action, break from loop and check for another device*/
                    iDevFound = 1;
                    iArrDevList[iCount] = USB_DEV_DETECTED;
                    break;
                }
            }
        }

        if(!iDevFound)
        {
            /* Device not found on the latest list, so invoke the callback and remove it from our list  */
            if(cbOnDeviceChange)
            {
                cbOnDeviceChange(pUsbDev,1);
                 iRetVal = 0;
            }
            /* Delete the device from list */
            DELETE_NODE((*pstUsbDevices), pUsbDev);			
            FreeUsbDevice(pUsbDev);
        }

        /* Move to next device on the list */
        pUsbDev = pTempDev;
    }

    /* Now check if any new device is found*/
    for (iCount = 0;iCount < iDevListLen; iCount++)    	
    {
		pLibUsbNewDev = ppUsbDevList[iCount];

        if(USB_DEV_DETECTED != iArrDevList[iCount] )
        {
            /* Its a new device, should be added to the usb device list */
            pUsbDev = (struct stUsbDevice *)malloc(sizeof(*pUsbDev));
            if (NULL == pUsbDev)
            {			
                PTP_DRV_WARN("No memory \n");
                PTPDEVACC_ERR(ENOMEM);
                goto ERR;
            }
	      memset(pUsbDev, 0, sizeof(*pUsbDev));
			
            char pTempPath[PATH_MAX + 1];
            memset(pTempPath, 0, PATH_MAX + 1);
	      pUsbDev->iLastSelectedConfig = -1;
            /* Get the device number */
            pUsbDev->uiDevNum = libusb_get_device_address(pLibUsbNewDev);
            snprintf(pTempPath, PATH_MAX,  "%03d", pUsbDev->uiDevNum);
            pTempPath[PATH_MAX] = '\0';
            pUsbDev->filePath = (char *)malloc(strlen(pTempPath)+1);
            if (NULL == pUsbDev->filePath)
            {
		   PTPFreeMem(pUsbDev);	
                PTP_DRV_WARN("No memory \n");
                PTPDEVACC_ERR(ENOMEM);
                goto ERR;
    }
            memset(pUsbDev->filePath, 0, strlen(pTempPath)+1);
            strncpy(pUsbDev->filePath, pTempPath, strlen(pTempPath));
            pUsbDev->filePath[strlen(pTempPath)] = '\0';

	    /* Get the bus number */
            memset(pTempPath, 0, PATH_MAX + 1);           
            pUsbDev->uiBusLoc = libusb_get_bus_number(pLibUsbNewDev);
            snprintf(pTempPath, PATH_MAX, "%03d", pUsbDev->uiBusLoc);  
            pTempPath[PATH_MAX] = '\0';
            
            pUsbDev->dirPath= (char *)malloc(strlen(pTempPath)+1);
            if (NULL == pUsbDev->dirPath)
            {			
                PTPFreeMem(pUsbDev->filePath);
                PTPFreeMem(pUsbDev);		   
                PTP_DRV_WARN("No memory \n");
                PTPDEVACC_ERR(ENOMEM);
                goto ERR;
            }
            memset(pUsbDev->dirPath, 0, strlen(pTempPath)+1);
            strncpy(pUsbDev->dirPath, pTempPath, strlen(pTempPath));
            pUsbDev->dirPath[strlen(pTempPath)] = '\0';

            pUsbDev->pvDev = pLibUsbNewDev;
            /* Initialize the device */            
            iRetVal = InitUsbDevice(pUsbDev);	
            if (iRetVal < 0) 
            {
            	PTPFreeMem(pUsbDev->dirPath);
                PTPFreeMem(pUsbDev->filePath);	
  		   PTPFreeMem(pUsbDev);		
                continue;
            }
			/*Add Node Only in case of Success i.e. Device Found*/
            ADD_NODE((*pstUsbDevices), pUsbDev);
            if(cbOnDeviceChange)
            {
                cbOnDeviceChange(pUsbDev,0);
                iRetVal = 1;				
            }
        }
    }

ERR:
        PTPFreeMem(iArrDevList);        			
    
MEMALLOC_FAIL:
    libusb_free_device_list(ppUsbDevList, 1);
    DEVICE_ACCESS_UNLOCK;
    return iRetVal;
}

/**
* @fn		int TempPTPDevFindPTPDevices(uint8_t uiBusNum, uint8_t uiDevNum);
* @brief		Finds PTP Devices through USB Interface.
* @param   	uint8_t uiBusNum : Bus Number
* @param   	uint8_t uiDevNum : Device Number
* @return   	Error code
*/
int TempPTPDevFindPTPDevices(uint8_t FindBusNum, uint8_t FindDevNum)
{	
    libusb_device **ppUsbDevList = NULL;
    libusb_device *pLibUsbNewDev = NULL;
    int iDevListLen = 0;
    int iRetVal = -1;
    int iCount = 0;
    uint8_t uiDevNum = 0;
    uint8_t uiBusNum = 0;
    /*If not intialized*/
    if (!pLibUsbCtx)
    {
        return iRetVal;
    }	

     /* Get the current conncected device list */
    iDevListLen = libusb_get_device_list(pLibUsbCtx, &ppUsbDevList);
    if (iDevListLen < 0)
    {
	libusb_free_device_list(ppUsbDevList, 1);
        return PTPDEVACC_ERR(iDevListLen);
    }

    for (iCount = 0;iCount < iDevListLen; iCount++)    	    		
    {		
        /* Compare the first device on PTP device list, with all the devices in the list received from libusb*/
        pLibUsbNewDev = ppUsbDevList[iCount];
        uiBusNum = libusb_get_bus_number(pLibUsbNewDev);	
        if(uiBusNum == FindBusNum)
        {
            uiDevNum = libusb_get_device_address(pLibUsbNewDev);
            if ( uiDevNum == FindDevNum )
            {	
                /* If device is found, means device is still connected, 
                no need to take any action, break from loop and check for another device*/
                iRetVal = 1;
                break;
            }
        }
    }

    libusb_free_device_list(ppUsbDevList, 1);
    return iRetVal;
}

/**  
* @fn    	int PTPDevUsbRegisterCbFn(PTP_CALLBACK callback)
* @brief	Registers callback function if any change is there in count of connected devices.
* @param	(in)callback,Fn. Pntr to Registered  Fn.
* @return   Success
*/
int PTPDevUsbRegisterCbFn(PTP_CALLBACK callback)
{
    cbOnDeviceChange = callback;
    return PTP_SUCCESS;
}
