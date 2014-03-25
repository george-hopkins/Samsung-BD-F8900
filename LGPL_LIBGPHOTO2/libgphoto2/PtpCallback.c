/** 
* @file 	PtpCallback.c
* @brief	This source file is used for Callback Notification 
*			to Application and Device Communication to check
*			Device Connect Disconnect Status and Updates  PTP Driver
*			Structure in Driver Interface Module
* @author	Sandeep Kumar(SISC)
* @Date	    	January 8, 2010

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
#include <signal.h>
#include <semaphore.h>
#include "PtpTypes.h"
#include "PtpDeviceAccess.h"
#include "ptpapi.h"
#include "ptp.h"
#include "PtpCallback.h"
#include "PTPDeviceListUpdate.h"

#ifdef CSP_LAYER_USAGE
#include "PCThread.h"
#include "PCDebug.h"
#else
#include "assert.h"
#endif

/* MACROS */
#ifndef CSP_LAYER_USAGE
static pthread_mutex_t __mutex_avh = PTHREAD_MUTEX_INITIALIZER; 	/*Function entry critical section*/
#define PTP_BEGIN			pthread_mutex_lock(&__mutex_avh)			/*Mutex Lock*/
#define PTP_END				pthread_mutex_unlock(&__mutex_avh)	/*Mutex UnLock*/
#ifndef ASSERT
#define ASSERT(x) 			(assert(x))							/*Assert Macro*/
#endif
/*CSP Layer Implementation To be used in Samsung Platform i.e. Valencia or Trident */
#else
static PCMutex 				__mutex_avh ; 			/*Mutex*/
#define PTP_CREATE			__mutex_avh.Create();		/*Create Mutex*/
#define PTP_DESTROY			__mutex_avh.Destroy();	/*Destroy Mutex*/
#define PTP_BEGIN				__mutex_avh.Lock();		/*Mutex Lock*/
#define PTP_END				__mutex_avh.UnLock();		/*Mutex UnLock*/
#endif
#define USB_PROC_BUS_USB_DEVICES_FILE 	"/proc/bus/usb/devices"	/*Usb Device File Location*/
#define PTP_ATTACHED_STRING1 			"Cls=06"					/*String1*/
#define PTP_ATTACHED_STRING2 			"still"					/*String2*/
#define PTP_MANUFACTURER 				"Manufacturer="			/*Manufacture*/
#define PTP_PRODUCT 						"Product="			/*Ptp Product*/
#define ONEKB							1024				/*1 KB*/
#define PTP_LENGTH						25					/*Ptp Length*/


/*ETERN VARIABLES*/
extern PTPDriverHandle		gPTPDriverHandle[MAX_DEV_SUPPORTED]; /* PTP Driver Structure*/

/*GLOBAL VARIABLES*/


/*STATIC FUNCTIONS*/  

/**
* @fn		static PTPDevContext *getDc(PtpDevHndl  ptpHandle)
* @brief  	This function maps the ptpHandle of a ptp device to its DeviceContext
* @remarks 	pDC is set to NULL if the device is not in the list
* @param	PtpDevHndl ptpHandle - ptpHandle of the camera  
* @return	PTPDevContext*:  
*			DeviceContext of the respective Handle
* @see 	
*/
static PTPDevContext *getDc(PtpDevHndl  ptpHandle)
{ 
    PTPDevContext* pDC = NULL;
    /*Gets Device Context*/
    CPTPDeviceListUpdate::Get().GetPTPDeviceContext(ptpHandle, &pDC);
    return pDC;
}

/**
* @fn		static  Bool PtpFileExist(const char* pFileName)
* @brief  	       This function  Check whether the file is existing or not
* @param	const char *fileName
* @return	Error Code
*		 	PTPAPI_ERROR : in case of error.
*		  	PTP_RC_OK : in case of success.
*/
static  Bool PtpFileExist(const char* pFileName)
{

    Bool bRet = false;
    struct stat stStatBuf;
	/*Checks File Status*/
    if(0 == stat(pFileName,&stStatBuf))
    {
        /*Check file is regular*/
        if(	S_ISREG(stStatBuf.st_mode))
        {
            bRet = True;						
        }
        else
        {
            PTP_DRV_INFO("\nPtpFileExist ERRRRRRRRRRRRRRRRRRRRRR\n");
        }
    }
    return bRet;
}

/**
* @fn	       Bool PtpCheckDevHndl(PtpDevHndl  ptpHandle)	
* @brief	       Check whether the ptp device is existing or not
* @remarks	none
* @param	PtpDevHndl ptpHandle:    Handle for the given ptp device
* @return	Error Code
*		 	PTPAPI_ERROR : in case of error.
*		  	PTP_RC_OK : in case of success.
*/ 
static Bool PtpCheckDevHndl(PtpDevHndl  ptpHandle)	
{
    int busNum = -1 ;
    int devNum = -1;
    FILE *fp = NULL;
    char pDataLine[ONEKB +1];
    int byteCount = 0;
    int finish = False;
    char *pDest = NULL;
    char *pDest1=NULL;
    int len = -1;
    char dest1[PTP_LENGTH];
    char dest2[PTP_LENGTH];
    PTPDevContext *pDevCtxt = NULL;
	/*Gets Device Context*/
    pDevCtxt=getDc(ptpHandle);
	/*Check Device Context is NULL*/
    if(pDevCtxt==NULL)
    {
        PTP_DRV_WARN("\nDevContext is null.\n");
        return false;	
    }
    /*Checks Ptp Device Exists*/	
    if (!PtpFileExist(USB_PROC_BUS_USB_DEVICES_FILE))
    {
        system("mount -t usbfs none /proc/bus/usb/");
    }
    /*Opens Ptp Device File*/
    if(!(fp = fopen(USB_PROC_BUS_USB_DEVICES_FILE, "r")))
    {
        PTP_DRV_WARN("\n[FILE]file open error %s\n",\
            USB_PROC_BUS_USB_DEVICES_FILE);
        return	false;
    }	
    memset(dest1,0,PTP_LENGTH);
    busNum=atoi(pDevCtxt->dev->dirPath);
    if(busNum<9)
    {
        snprintf(dest1,PTP_LENGTH,"Bus=0%d",busNum);
    }
    else	
    {
        snprintf(dest1,PTP_LENGTH,"Bus=%d",busNum);
    }
    /*Set Values to 0*/
    memset(dest2,0,PTP_LENGTH);
    devNum=atoi((pDevCtxt->dev)->filePath);
    snprintf(dest2,PTP_LENGTH,"Dev#=%3d",devNum);
    while (1) 
    {
       /*Reads Data*/
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
                            pDest=pDest+6;//increment the pointer  to point after string dest1
                            pDest1 = strstr(pDest,dest1);
                            if(pDest1!=NULL)
                            {
                                len=strlen(pDest1);
                                if(0 != fseek(fp,-(len+4),SEEK_CUR))
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


/**
* @fn	       static uint16 PtpOpenSession(PtpDevHndl  ptpHandle)
* @brief	       Open session for the given ptp
* @remarks	none
* @param	PtpDevHndl ptpHandle:    Handle for the given ptp device
* @return	Error Code
*		 	PTPAPI_ERROR : in case of error.
*		  	PTP_RC_OK : in case of success.e
*/ 
static uint16 PtpOpenSession(PtpDevHndl  ptpHandle)
{	
    uint16 uiResult = PTPAPI_ERROR;	
    PTPDevContext *pDevCtxt = NULL;
    /*Gets Device Context*/
   pDevCtxt=getDc(ptpHandle);
   /*Check Device Context is NULL*/
    if(pDevCtxt==NULL)
    {
        PTP_DRV_ERR("\nDevContext is null.\n");
        return PTPAPI_ERROR;   
    }
    /*Checks Device Handle*/	
    if (PtpCheckDevHndl(ptpHandle) != True)
    {
	 PTP_DRV_ERR("\nPtpHandle is null.\n");
        return PTPAPI_ERROR; 
    }
    /*Added for Device Libusb Reset*/
    /*Better Performance no delay */
    uiResult = (uint16) PTPDevUsbReset(pDevCtxt->ptp_usb.handle);
    if(uiResult != 0 )
    {	 
        PTP_DRV_WARN("\n############## Reset Failed \n");
    }
    else
    {
        PTP_DRV_INFO("\nPTPDevUsbReset  is ok\n"); 	
    }
    /*Opens Ptp Session*/
    uiResult = ptp_opensession(&(pDevCtxt->params), PTP_START_INDEX);
    if (uiResult != PTP_RC_OK) 
    {        
        PTP_DRV_WARN("\n: Can't open PTP session(%x)\n",\
             uiResult);
    }
    else
    {
        PTP_DRV_INFO("\nptp_opensession is ok\n");		
    }	
    /*Added for panasonic - to chk*/
    if (uiResult == PTP_RC_SessionAlreadyOpened)
    {
        uiResult = ptp_closesession(&(pDevCtxt->params)); 	
        if (uiResult != PTP_RC_OK) 
        {               
            PTP_DRV_WARN("\n : Can't close PTP session (%x)\n",uiResult);
        }
        else
        {
            uiResult = ptp_opensession(&(pDevCtxt->params), PTP_START_INDEX);
            if (uiResult != PTP_RC_OK) 
            {        
                PTP_DRV_WARN("\n: Can't open PTP session (%x)\n",\
                     uiResult);
            }
            else
            {
			PTP_DRV_INFO("\nDONE\n");
            }	
        }	
    }
    return uiResult;
}

/**
* @fn	       static uint16 PtpCloseSession(PtpDevHndl  ptpHandle)
* @brief	       Close session for the given ptp device
* @remarks	none
* @param	PtpDevHndl ptpHandle:    Handle for the given ptp device
* @return	Error Code
*		 	PTPAPI_ERROR : in case of error.
*		  	PTP_RC_OK : in case of success.
*/ 
static uint16 PtpCloseSession(PtpDevHndl  ptpHandle)
{	
    uint16 uiResult = PTPAPI_ERROR;	
    PTPDevContext *pDevCtxt = NULL;
    /*Gets Device Context*/
    pDevCtxt=getDc(ptpHandle);
	/*Check Device Context is NULL*/
    if(pDevCtxt==NULL)
    {
        PTP_DRV_ERR("\nDevContext is null.\n");
        return PTPAPI_ERROR;   
    }
   /*Check Device Handle*/  
    if (PtpCheckDevHndl(ptpHandle) != True)
    {
	PTP_DRV_ERR("\nPtpHandle is null.\n");
	return PTPAPI_ERROR; 
    }
    /*Close Ptp Session*/
    uiResult = ptp_closesession(&(pDevCtxt->params)); 	
    if (uiResult != PTP_RC_OK) 
    {               
        PTP_DRV_WARN("\n[: Can't close PTP session (%x)\n",uiResult);
    }
    else
    {
		PTP_DRV_INFO("\nDONE\n");
    }	
    return uiResult;
}



/**
* @fn	       static uint16 PtpOpenDevice(Bool bIsgetdeviceinfo, 
PtpDevHndl  ptpHandle)
* @brief	Opens the given ptp device
* @remarks	none
* @param	Bool isgetdeviceinfo	:flag whether device info is to fetched or not
* @param	PTPDevContext *pdc	:Device context of the given device
* @return	Error Code
*		 	PTPAPI_ERROR : in case of error.
*		  	PTP_RC_OK : in case of success.
*/ 
static uint16 PtpOpenDevice(Bool bIsgetdeviceinfo, PtpDevHndl  ptpHandle)
{
    uint16 uiResult = PTPAPI_ERROR;
    uint32 i = 0;
    PTPDevContext *pDevCtxt = NULL;
    /*Gets Device Context*/
    pDevCtxt=getDc(ptpHandle);
   /*Check Device Context is NULL*/
    if(pDevCtxt==NULL)
    {
        PTP_DRV_ERR("\nDevContext is null.\n");
        return PTPAPI_ERROR;
    }

    /*Initially ObjectInfo to be NULL*/
    pDevCtxt->params.objectinfo = NULL;
   /*Check Device Handle*/	
    uiResult=PtpCheckDevHndl(ptpHandle);
    if (uiResult != True)
    {	
	PTP_DRV_ERR("\nPtpHandle is null.\n");
        return PTPAPI_ERROR; 
    }
    /*Initialize Device Context*/
    PtpPrivateInitTransaction (pDevCtxt);
   /*Opens Ptp Device Context*/
    uiResult = PtpPrivateOpenDeviceContext(pDevCtxt);
    if (uiResult != PTP_RC_OK)
    {       
        PTP_DRV_ERR("\nPtpPrivateOpenDeviceContext :  return (%x)\n", uiResult);
        return uiResult;
    }
    if (bIsgetdeviceinfo)
    {
	/*Gets Device Info*/
        uiResult = ptp_getdeviceinfo (&(pDevCtxt->params),\
            &(pDevCtxt->params.deviceinfo));
        if( uiResult != PTP_RC_OK) 
        {
            PTP_DRV_WARN("\nptp_getdeviceinfo : return (%x)\n", uiResult);
            PtpPrivateCloseDeviceContext(pDevCtxt);
            return uiResult;
        }			
        else
        {
            PTP_DRV_INFO("\nThe Model is : %s\n",pDevCtxt->params.deviceinfo.Model);

            for (i = 0; i < pDevCtxt->params.deviceinfo.OperationsSupported_len; i++)
            {				
                PTP_DRV_INFO("\nOperation Supported[%x]\n", pDevCtxt->params.deviceinfo.OperationsSupported[i]);
            }
        }
    }
    else
    {
        PTP_DRV_WARN("\nIsget Info not there\n");
    }
    PTP_DRV_INFO("\nDONE\n");
    return uiResult;	
}

/**
* @fn          	static uint16 PtpCloseDevice(PtpDevHndl  ptpHandle)
* @brief	       close device for the given ptp device
* @remarks	none
* @param	PtpDevHndl ptpHandle:    Handle for the given ptp device
* @return	Error Code
*		 	PTPAPI_ERROR : in case of error.
*		  	PTP_RC_OK : in case of success.
*/ 
static uint16 PtpCloseDevice(PtpDevHndl  ptpHandle)
{
    uint16 uiResult = PTPAPI_ERROR;
    PTPDevContext *pDevCtxt = NULL;
    /*Gets Device Context*/
    pDevCtxt=getDc(ptpHandle);
   /*Check Device Context is NULL*/
    if(pDevCtxt==NULL)
    {
        PTP_DRV_ERR("\nDevContext is null.\n");
        return PTPAPI_ERROR;
    }
    /*Check Device Handle*/
    if (PtpCheckDevHndl(ptpHandle) != True)
    {
	 PTP_DRV_ERR("\nPtpHandle is null.\n");
        return PTPAPI_ERROR; 
    }
    /*Close Ptp Device Context*/
    uiResult = PtpPrivateCloseDeviceContext(pDevCtxt);
    if (uiResult != PTP_RC_OK)
    {       
        PTP_DRV_WARN("\nPtpPrivateCloseDeviceContext :  return (%x)\n", uiResult);
    }
    PTP_DRV_INFO("\nDONE\n");
    return uiResult;
}

/**
* @fn	  	PtpErr_t PtpCallbackInvokeCb(PtpDevInstances_t tDevInst,
PtpCbId_t tAppCbId, 
void *pCbParam)
* @brief	  	Invokes the installed i.e Registered Callback function
* @param 	(in)	 tDevInst,Device Instance Number
*		  	(in)     tAppCbId, Callback ID
*			(in)     pCbParam, Callback param
* @return  	Error code
*		 	PTPAPI_ERROR : in case of error.
*		  	PTP_RC_OK : in case of success.
*/
PtpErr_t PtpCallbackInvokeCb(PtpDevInstances_t tDevInst,
                             PtpCbId_t tAppCbId, 
                             void *pCbParam)
{
    /*Checking for Error Condition*/
    if((tDevInst >= MaxPtpDevSupport)||(tDevInst <= PtpInvalidDevInst)
        ||(tAppCbId <= PtpInvalidCbId)||(tAppCbId > PtpDevDetectStatusCbId)
        ||(NULL == pCbParam))
    {
        PTP_DRV_ERR("\nInvalid Params\n");
        ASSERT(0);
        return PTPAPI_ERROR;
    }

    /*Currently we are supporting only PtpDevDetectStatusCbId*/
    /*Can support more on Application Requirment*/
    gPTPDriverHandle[tDevInst].PtpAppCbFn[tAppCbId](tAppCbId,pCbParam,tDevInst);

    return PTP_RC_OK;
}
/**
* @fn	       uint16 PtpCallbackGetDevInfo(PTPDeviceInfo *deviceinfo, 
PtpDevHndl  ptpHandle)
* @brief	       Get information of the given ptp device
* @param	PTPDeviceInfo *deviceinfo	: the store of a device information
* @param	PTPDevContext *dc		:Device context of the given device
* @param	PtpDevHndl  ptpHandle	:Ptp Device handle
* @return  	Error code
*		  	PTPAPI_ERROR : in case of error.
*		   	PTP_RC_OK : in case of success.
*/ 
uint16 PtpCallbackGetDevInfo(PTPDeviceInfo *pDeviceinfo, PtpDevHndl  ptpHandle)
{
    uint16 result = PTP_RC_OK;
    PTPDevContext *pDevCtxt = NULL;

   /*Mutex Lock*/
    PTP_BEGIN;	
    /*Gets Device Context*/
    pDevCtxt=getDc(ptpHandle);
   /*Check Device Context is NULL*/
    if(pDevCtxt==NULL)
    {
	/*Mutex UnLock*/
        PTP_END;
        PTP_DRV_ERR("\nDevContext is null.\n");
        ASSERT(0);
        return PTPAPI_ERROR;
    }
    /*Check Device Handle*/
    if (PtpCheckDevHndl(ptpHandle)  != True)
    {
	/*Mutex UnLock*/
        PTP_END;
        PTP_DRV_ERR("\nPtpHandle is null.\n");
        ASSERT(0);
        return PTPAPI_ERROR; 
    }
    /*Initialize Device Context*/
    PtpPrivateInitTransaction (pDevCtxt);
   /*Gets Device Information*/
    result = ptp_getdeviceinfo (&(pDevCtxt->params), pDeviceinfo);
    if( result != PTP_RC_OK) 
    {
        PTP_DRV_ERR("\nptp_getdeviceinfo: return (%x)\n", result);
	/*Mutex UnLock*/
        PTP_END;
        return result;
    }
    /*Mutex UnLock*/
    PTP_END;
    return result;	
}

/**
* @fn	       uint16 PtpCallbackGetStorageIDs(PtpDevHndl  ptpHandle,
PTPStorageIDs *storageids)
* @brief	       Get list of storageIds from the given ptp device
* @param	PTPStorageIDs *storageids:		list of storage ids
* @param	PtpDevHndl ptpHandle:	Handle for the given ptp device
* @return  	Error code
*		  	PTPAPI_ERROR : in case of error.
*		   	PTP_RC_OK : in case of success.
*/ 
uint16 PtpCallbackGetStorageIDs(PtpDevHndl  ptpHandle,PTPStorageIDs *pStorageids)
{
    uint16  uiResult = PTP_RC_OK;
    PTPDevContext *pDevCtxt = NULL;
    /*Mutex Lock*/
    PTP_BEGIN;	
    /*Gets Device Context*/
    pDevCtxt = getDc(ptpHandle);

    /*Check Device Context is NULL*/
    if(pDevCtxt == NULL)
    {
	/*Mutex UnLock*/
        PTP_END;
        PTP_DRV_ERR("\nDevContext is null.\n");
        ASSERT(0);
        return PTPAPI_ERROR;
    }
    /*Checks PTP Device Handle*/
    if (PtpCheckDevHndl(ptpHandle) != True)
    {
        PTP_DRV_ERR("\nPtpHandle is null.\n");
	/*Mutex UnLock*/
        PTP_END;
        ASSERT(0);
        return PTPAPI_ERROR;
    }
   /*Initialize Device Context*/
    PtpPrivateInitTransaction (pDevCtxt);
    /*Gets Storage IDs from Device*/
    uiResult = ptp_getstorageids (&(pDevCtxt->params), pStorageids);
    if( uiResult != PTP_RC_OK)
    {
        PTP_DRV_ERR("\nptp_getstorageids: return (%x)\n", uiResult);
	/*Mutex UnLock*/
	PTP_END;
        return uiResult;
    }       
   /*Mutex UnLock*/
    PTP_END;
    return uiResult;
}       

/**
* @fn	    uint16 PtpCallbackGetStorageInfos(PtpDevHndl  ptpHandle, 
uint32_t storageid, 
PTPStorageInfo* storageinfo)
* @brief	Get storage information of the given storage id from the device
* @param	PTPStorageInfo* storageinfo:	structure for storage infomation		
* @param	PtpDevHndl ptpHandle:	Handle for the given ptp device
* @return      	Error code
*		  	PTPAPI_ERROR : in case of error.
*		   	PTP_RC_OK : in case of success.
*/ 
uint16 PtpCallbackGetStorageInfos(PtpDevHndl  ptpHandle, 
                                  uint32_t uiStorageid, 
                                  PTPStorageInfo* pStorageinfo)
{
    uint16  uiResult = PTP_RC_OK;
    PTPDevContext *pDevCtxt = NULL;
   /*Mutex Lock*/
    PTP_BEGIN;	
    /*Gets Device Context*/
    pDevCtxt = getDc(ptpHandle);
    /*Check Device Context is NULL*/
    if(pDevCtxt == NULL)
    {
        PTP_DRV_ERR("\nDevContext is null.\n");
	 /*Mutex UnLock*/
        PTP_END;
        ASSERT(0);
        return PTPAPI_ERROR;
    }
    /*Checks PTP Device Handle*/
    if (PtpCheckDevHndl(ptpHandle) != True)
    {
	 /*Mutex UnLock*/
        PTP_END;
        PTP_DRV_ERR("\nPtpHandle is null.\n");
        ASSERT(0);
        return PTPAPI_ERROR; 
    }
    /*Initialize Device Context*/
    PtpPrivateInitTransaction (pDevCtxt);

    if(uiStorageid & PTP_PHYSICAL_STORAGE_MASK) 
    {
        if(uiStorageid & PTP_LOGICAL_STORAGE_MASK)
        {
		/*Get Storage Info from Device*/
            PTP_DRV_INFO("\nPTP_LOGICAL_STORAGE existing\n ");
            uiResult = ptp_getstorageinfo (&(pDevCtxt->params), \
                uiStorageid, pStorageinfo);
            if( uiResult != PTP_RC_OK)
            {
                PTP_DRV_WARN("\nptp_getstorageinfo:return (%x)\n",uiResult);
		   /*Mutex UnLock*/
                PTP_END;
                return uiResult;
            }
        }
        else
        {
            PTP_DRV_WARN("\nPTP_LOGICAL_STORAGE not existing\n ");
		/*Mutex UnLock*/
            PTP_END;
            return PTP_RC_Undefined;
        }
    }
    else
    {
        PTP_DRV_WARN("\nPTP_PHYSICAL_STORAGE not existing\n ");
	/*Mutex UnLock*/
        PTP_END;
        return PTP_RC_Undefined;
    }
   /*Mutex UnLock*/
    PTP_END;
    return uiResult;
}

/**
* @fn	      	uint16 PtpCallbackInitComm(PtpDevHndl  ptpHandle)
* @brief	      	Initialization of communication with the given ptp device
* @remarks	This function must be called onetime when the device is attached
* @param	PtpDevHndl ptpHandle:    Handle for the given ptp device
* @return  	Error code
*		  	PTPAPI_ERROR : in case of error.
*		   	PTP_RC_OK : in case of success.
*/
uint16 PtpCallbackInitComm(PtpDevHndl  ptpHandle)
{
    uint16 uiResult = PTPAPI_ERROR;
    PTPDevContext *pDevCtxt = NULL;
   /*Gets Device Context*/
    pDevCtxt=getDc(ptpHandle);
   /*Check Device Context is NULL*/
    if(pDevCtxt==NULL)
    {
        PTP_DRV_ERR("\nPTPDevContext is null\n");
        return PTPAPI_ERROR;   
    }
    /*Checks PTP Device Handle*/
    if (PtpCheckDevHndl(ptpHandle) != True)
    {
        PTP_DRV_ERR("\nPtp Handle Not Foundl\n");    
        return PTPAPI_ERROR; 
    }
    /*Opens PTP Device*/
    uiResult = PtpOpenDevice(DO_NOT_GET_DEVICE_INFO,ptpHandle);
    if (uiResult == PTP_RC_OK)
    {
        PTP_DRV_INFO("\nPtpOpenDevice is ok\n");	
	 /*Opens Session if Ptp Device Open is Success*/
        uiResult = PtpOpenSession(ptpHandle);	
        if(uiResult == PTP_RC_OK)
        {
            PTP_DRV_INFO("\nPtpOpenSession ok\n");
        }
        else
        {
            PTP_DRV_WARN("\nPtpOpenSession NOT ok\n");
        }
    }
    else
    {	
    	 /*If PTP Device Open Fails*/
        PTP_DRV_WARN("\nPtpOpenDevice is not ok\n");
    }
    return uiResult;
}

/**
* @fn	      	uint16 PtpCallbackFinishComm(PtpDevHndl  ptpHandle)
* @brief	      	Finishing process of communication with the given ptp device
* @remarks	none
* @param	PtpDevHndl ptpHandle:    Handle for the given ptp device
* @return       Error code
*		       PTPAPI_ERROR : in case of error.
*		       PTP_RC_OK : in case of success.
*/
uint16 PtpCallbackFinishComm(PtpDevHndl  ptpHandle)
{
    uint16 uiResult = PTPAPI_ERROR;
    /*Close PTP Session*/
    uiResult = PtpCloseSession(ptpHandle);
    if (uiResult == PTP_RC_OK)
    {
        PTP_DRV_INFO("\nPtpOpenDevice is OK\n");
	/*If Session Close is Success than Close PTP Device*/
        uiResult = PtpCloseDevice(ptpHandle);
        if(uiResult == PTP_RC_OK)
        {
            PTP_DRV_INFO("\nPtpCloseDevice is OK\n");
        }
        else
        {
            PTP_DRV_WARN("\nPtpCloseDevice is not OK\n");
        }
    }
    else
    {
       /*Close Session is Not Success*/
        PTP_DRV_WARN("\nPtpCloseSession is not OK\n");
    }
    return uiResult;
}

/**
* @fn		PtpCallbackUpdateDevInfo
* @brief 		Updates PTP Driver structure on each device connect and disconect status
* @param 	(in)   bConnectStat,Device Status
*			(in)  ptpDevice, Usb Device Pointer 
*			(in)  pPTPDevCtxt,Device Context Pointer
* @return	Error code
*			PTPAPI_ERROR : in case of error.
*			PTP_RC_OK : in case of success.
* @see		
*/
PtpErr_t PtpCallbackUpdateDevInfo(Bool bConnectStat,
                                  UsbDev *ptpDevice,
                                  PTPDevContext *pPTPDevCtxt)
{
    PtpDevInstances_t  tDevInst = PtpInvalidDevInst;
    UsbDev *tempptpDevice = NULL;
    PTPDevContext *tempPTPDevCtxt=NULL;
    PtpAppDevDetectStatusCbParam 	 devDetectCbParam;
    /*Check Device Pointer is not NULL*/
    if(NULL == ptpDevice)
    {
        PTP_DRV_ERR("\nNULL Pointer to Ptp Device\n");
        return PTPAPI_ERROR;
    }
    /*Device Disconnect Status*/
    if(!bConnectStat)
    {
        tempptpDevice = ptpDevice;
        tempPTPDevCtxt = pPTPDevCtxt;
        PTP_DRV_INFO("\nIn Disconnect Stat\n");
	 /*Search For the Device which is Disconnected*/
        for(tDevInst =PtpDevInst0;tDevInst < MaxPtpDevSupport; tDevInst = \
            (PtpDevInstances_t)(tDevInst +1) )
        {
            /*Lock Device Mutex*/
            PtpDevLock(tDevInst);

            if(NULL != tempptpDevice)
            {
                PTP_DRV_INFO("\nDevice Bus Num[%d]\n",tempptpDevice->ptp_path.bus_num);
            }
            else
            {
                PTP_DRV_WARN("\nNULL PtpDevice\n");
            }
		/*Check for Device/Bus Number */
		/*Both should match in the existing List*/
            if((NULL != gPTPDriverHandle[tDevInst].ptpDevice)&&
                (NULL != tempptpDevice)&&
                (gPTPDriverHandle[tDevInst].ptpDevice->ptp_path.bus_num == \
                tempptpDevice->ptp_path.bus_num)
                &&(gPTPDriverHandle[tDevInst].ptpDevice->ptp_path.dev_num == \
                tempptpDevice->ptp_path.dev_num)
                &&(True == gPTPDriverHandle[tDevInst].IsDeviceDetect))
            {
		   /*Device is found*/
                PTP_DRV_INFO("\nDisconnect::tDevInst Found=%d\n",(int)tDevInst);
		   /*Unlock The Device Mutex*/
                PtpDevUnLock(tDevInst);
		   /*Come out of Loop*/
                break;
            }
            /*UnLock Device Mutex*/
            PtpDevUnLock(tDevInst);
        }

        /*Check for device context*/
        if((tDevInst > PtpInvalidDevInst) && (tDevInst < MAX_DEV_SUPPORTED)&& (NULL !=gPTPDriverHandle[tDevInst].pPTPDevCtxt) 
            &&(NULL != gPTPDriverHandle[tDevInst].pPTPDevCtxt->ptp_usb.handle)
            )
        {
            /*Lock Device Mutex*/
            PtpDevLock(tDevInst);
            if(0!= PTPDevUsbRelInterface(gPTPDriverHandle[tDevInst].pPTPDevCtxt->ptp_usb.handle,gPTPDriverHandle[tDevInst].pPTPDevCtxt->dev->ppConfigDesc[gPTPDriverHandle[tDevInst].pPTPDevCtxt->dev->iLastSelectedConfig]->interface[gPTPDriverHandle[tDevInst].pPTPDevCtxt->dev->iLastClaimedInfterface].altsetting[gPTPDriverHandle[tDevInst].pPTPDevCtxt->dev->iAltSettingNum].bInterfaceNumber))
            {
                PTP_DRV_WARN("\nRel Interface Not OK\n"); 			 
            }
		/*Close Device*/
            PTPDevUsbClose(gPTPDriverHandle[tDevInst].pPTPDevCtxt->ptp_usb.handle);
            gPTPDriverHandle[tDevInst].pPTPDevCtxt = NULL;
            gPTPDriverHandle[tDevInst].ptpDevice = NULL;
            gPTPDriverHandle[tDevInst].IsDeviceDetect= bConnectStat;
            /*UnLock Device Mutex*/
            PtpDevUnLock(tDevInst);
        }
        else
        {
		PTP_DRV_WARN("\n~~~~DISCONNECT STATE -- NULL DEVCNTXT~~~~~\n");
        }
    }
   /*Device Connect Status*/
    else		
    {
        PTP_DRV_INFO("\nIn Connect Stat\n");
        for(tDevInst =PtpDevInst0;tDevInst < PtpDevInst4; 
            tDevInst = (PtpDevInstances_t)(tDevInst +1) )
        {
            /*Lock Device Mutex*/
            PtpDevLock(tDevInst);
		/*If Device is Not Detected*/
            if(False == gPTPDriverHandle[tDevInst].IsDeviceDetect)
            {
                PTP_DRV_INFO("\nConnect::tDevInst Found=%d\n",(int)tDevInst);
                PtpDevUnLock(tDevInst);
                break;
            }
            /*UnLock Device Mutex*/
            PtpDevUnLock(tDevInst);
        }
        gPTPDriverHandle[tDevInst].pPTPDevCtxt = pPTPDevCtxt;	/*Copy Device Context*/
        gPTPDriverHandle[tDevInst].ptpDevice = ptpDevice;		/*Copy Ptp Device Pointer*/
        gPTPDriverHandle[tDevInst].IsDeviceDetect= bConnectStat;	/*Copy Connect Status*/
    }

    /*Lock Device Mutex*/
    PtpDevLock(tDevInst);

    if(
        /*Device Connect/Disconnect Status*/
        ((PtpDevInstances_t)tDevInst > PtpInvalidDevInst)&&
        ((PtpDevInstances_t)tDevInst < MaxPtpDevSupport)&&
        (NULL!= gPTPDriverHandle[tDevInst].PtpAppCbFn[PtpDevDetectStatusCbId])        
        )
    {
	 /*Fill Callback Parameters */
        devDetectCbParam.DevInst = (PtpDevInstances_t)tDevInst;		/*Device Instance*/
        devDetectCbParam.BusNum = ptpDevice->ptp_path.bus_num;	/*Bus Number*/
        devDetectCbParam.DevNum = ptpDevice->ptp_path.dev_num;	/*Device Number*/
        devDetectCbParam.ConnectStat = bConnectStat;				/*Connect Status*/
        PTP_DRV_INFO("\nConnect/Disconnect Devinst=%d\n",(int)tDevInst);
        /*UnLock Device Mutex*/
        PtpDevUnLock(tDevInst);
	/*Invokes Application Callback*/
        PtpCallbackInvokeCb((PtpDevInstances_t)tDevInst,
            PtpDevDetectStatusCbId,(void *)&devDetectCbParam);
        return PTP_RC_OK;
    }	 
    else
    {
        PTP_DRV_INFO("\nDevice[%d] : Status[%d] Callback Not Registered!!!\n",(int)tDevInst,bConnectStat);
        /*UnLock Device Mutex*/
        PtpDevUnLock(tDevInst);
        return PTPAPI_ERROR;
    }
}
