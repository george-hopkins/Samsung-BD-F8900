/**
* @File		PtpTypes.h
* @brief		PTP Driver Typedefines
* @author	Sandeep Kumar(SISC)
* @Date	    	January 8, 2010

 * Copyright 2010 by Samsung Electronics, Inc.,
 * 
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 *  you entered into with Samsung.
 */


 
#ifndef _PTPTYPES_H_
#define _PTPTYPES_H_

/* HEADER FILE INCLUDES */
#include "ptp.h"
//#include "PtpDeviceAccess.h"
#include <stdlib.h>
#include "libusb.h"

/* MACROS */
#define  ENABLE_PTP_DRV_LOG					/*This Macro Enables/Disables All PTP Driver Prints*/
//#define  ENABLE_PTP_DRV_DEBUG_LOG			/*This Macro Enables/Disables Warning/Info Prints*/
#ifdef ENABLE_PTP_DRV_LOG
#define PTP_DRV_LOG(level, fmt...) PTPDriverLog( level, __FUNCTION__, fmt)	/*PTP Driver Log*/
#else
#define PTP_DRV_LOG(level, fmt...) 
#endif

#ifdef ENABLE_PTP_DRV_LOG
#define PTP_DRV_DEBUG(fmt...) PTP_DRV_LOG( PtpDrvLogLevelDebug, fmt)
#else
#define PTP_DRV_DEBUG 
#endif

#define PTP_DRV_INFO(fmt...) PTP_DRV_LOG(PtpDrvLogLevelInfo, fmt)		/*PTP Driver Info Log*/
#define PTP_DRV_WARN(fmt...) PTP_DRV_LOG( PtpDrvLogLevelWarning, fmt)	/*PTP Driver Warning Log*/
//#define PTP_DRV_ERR(fmt...) PTP_DRV_LOG( PtpDrvLogLevelError, fmt)		/*PTP Driver Error Log*/
#define PTP_DRV_ERR printf


#define	OPTIMIZED_PTP									/*Optimized PTP*/
#define PTP_VALID_HNDL(DevInst)		(0x20050820|DevInst)	/*Validating PTP Handle*/
#define PTP_INVALID_HNDL	0x10203040						/*Invalidating PTP Handle*/
/*Checking PTP Handle*/
#define PTP_CHECKHNDL(h)     (((uint32)(h) != (uint32)0) \
                 && ((uint32)(h) != (uint32)0xffffffff)  && \
				(((uint32)(h) >= (uint32)0x20050820)) && \
                 ((uint32)(h) < (0x20050820|MAX_DEV_SUPPORTED)) )

/*Here Max Device Support Means that Simultaneous Access of Devices*/
/*Currently We are supporting Max. of 5 Device Support*/
#define	MAX_DEV_SUPPORTED	5							/*Maximum Device Supported*/
/*Frees the Memory Allocated*/
#define  PTPFreeMem(ptr) \
{\
    if(ptr)\
    {\
    free(ptr);\
    ptr = NULL;\
    }\
}
#define Bool		unsigned char								/*Indicates Bool */
#define False	0											/*Indicates False*/					
#define True	1											/*Indicates True*/
#define PTP_START_INDEX	1								/*Ptp Start Index*/
#define USB_CLASS_PTP		6							/*USB Ptp Class*/
#define USB_DP_HTD		(0x00 << 7)						/*USB DP HTD*/
#define USB_DP_DTH		(0x01 << 7)						/*USB DP DTH*/
#define USB_REQ_DEVICE_RESET			0x66					/*USB Request Device Reset*/
#define USB_REQ_GET_DEVICE_STATUS	0x67					/*USB Request Gets Device Status*/
#define USB_FEATURE_HALT		0x00							/*USB Halt*/
#define USB_TIMEOUT				10000					/*USB Time Out Value*/
#define USB_CAPTURE_TIMEOUT	20000						/*USB Capture Time Out Value*/
#define PTP_TIMEOUT		10000							/*Ptp Time Out Value*/
#define PTP_USB_URB		2097152							/*USB URB*/
#define MAX_FILENAME		255								/*Max File Name Length*/
#define PTPAPI_ERROR		0xffff							/*PTP Error Code*/
#define DO_GET_DEVICE_INFO				1				/*Get Device Information*/
#define DO_NOT_GET_DEVICE_INFO		0					/*No Device Informamtion*/
#define	RETRY_CNT	3									/*Retry Count Value*/
#define PTP_LOGICAL_STORAGE_MASK    0x0000FFFF			/*Logical Storage Mask*/
#define PTP_PHYSICAL_STORAGE_MASK   0xFFFF0000			/*Physical Storage Mask*/
#define GET_NUM_OBJ 999999								/*FIX FOR FUJI CAMERA-Get Number of object Value*/

/*TYPE DEFINES*/

typedef unsigned char uint8;		/*Unsigned Int8*/
typedef unsigned int uint16;			/*Unsigned Int16*/
typedef unsigned long uint32;		/*Unsigned Int32*/
typedef signed char int8;			/*Int8*/
typedef signed int int16;			/*Int16*/
typedef signed long int32;			/*Int32*/
typedef uint16 	PtpErr_t;			/*Ptp Driver Error Code*/


struct stUsbDevice 
{		
    struct stUsbDevice *next, *prev;
    libusb_device_handle *handle;
    void *pvDev;	
    struct libusb_config_descriptor **ppConfigDesc;
    int iLastSelectedConfig;
    int iLastClaimedInfterface;	
    int iAltSettingNum;	
    struct libusb_device_descriptor deviceDesc;
    u_int32_t uiDevNum;    			
    char *filePath;
    u_int32_t uiBusLoc;
    char *dirPath;
};
typedef struct stUsbDevice USB_DEVICE,* PUSB_DEVICE;
typedef void (* PTP_CALLBACK)(PUSB_DEVICE dev, int iDisConnect);
typedef unsigned int size_t;


/*Ptp Device Instances*/
typedef enum
{
	PtpInvalidDevInst = -1,		/*Invalid Device Instance*/
	PtpDevInst0 =0,			/*Device Instance 0*/
	PtpDevInst1,				/*Device Instance 1*/
	PtpDevInst2,				/*Device Instance 2*/
	PtpDevInst3,				/*Device Instance 3*/
	PtpDevInst4,				/*Device Instance 4*/
	/*Add Here*/
	MaxPtpDevSupport			/*Max. Device Suppported*/
}PtpDevInstances_t;

typedef void* PTPDeviceHandle, *pPTPDeviceHandle;	/*Ptp Device Handle*/
typedef enum PTP_DEBUG_LEVEL_		/*Debug Level*/
{
	PTP_DBG_LVL0,
	PTP_DBG_LVL1,
	PTP_DBG_LVL2,
}PTP_DEBUG_LEVEL;
typedef enum 					
{
	FILE_TYPE = 1, 					/*File Type*/
	DIR_TYPE						/*Directory Type*/
}PtpFileType_t;

/*PTP USB Structure*/
typedef struct _PTP_USB 
{
	PUSB_DEVICE handle;				/*Usb Device Handle*/
	USB_DEVICE  DevStruct;			/*USB Device Structure*/
	int inep;							/*In End Point*/
	int outep;						/*Out End Point*/
	int intep;							/*Intr End Point*/
}PTP_USB;

/*PTP Device Context Structure*/
typedef struct _PTPDevContext
{
        PTPParams		params;			/*PTP Parameters*/
        PTP_USB			ptp_usb;		/*PTP Usb Structure*/
        struct stUsbDevice 	*dev;		/*USB Device Access Structure*/
}PTPDevContext;

/*Only For Backward Code Compatibility*/
typedef struct _PtpDevHndl
{
	int bus_num;						/*Bus Number*/
	int dev_num;						/*Device Number*/
}PtpDevHndl;

/*USB Dev Structure*/
typedef struct  _UsbDev
{
	PtpDevHndl ptp_path;				/*PTP Device Handle*/
// 	char devpath[24];					/*Device Path*/
	char vendor[80];					/*Vendor ID*/
	char model[80];					/*Model ID*/
	int storage_id[2];					/*Storage ID*/
	PTPStorageIDs sids;				/*Storage ID Structure*/
	PTPStorageInfo 	storageinfo[2];	/*Storage Info Structure*/
	char	SerialNumber[80];				/*Serial Number*/
	uint16 DeviceId;					/*Device ID*/
}UsbDev;

/*PTP Device Handle Structure*/
typedef struct _PTPDevHndl
{
	int BusNum;	/*-1,Invalid Bus Num*/
	int DevNum;/*-1,Invalid Dev Num*/
	PtpDevInstances_t DevInst;
}PTPDevHndl;
/*File List Structure*/
typedef struct _PTPFileListInfo
{
	char dirname[MAX_FILENAME];	/*Directory Name*/
	char filename[MAX_FILENAME];	/*File Name*/
	char filetype;					/*File Type*/
	uint32 storageId;				/*Storage ID*/
	uint32 handle;					/*Handle*/
	uint32 ParentObject;			/*Parent Object*/
	int year;						/*Year Information*/
	int month;					/*Month Information*/
	int day;						/*Day Information*/
	int hour;						/*Hour Information*/
	int min;						/*Minute Information*/
	int sec;						/*Second Information*/
}PTPFileListInfo;
/*PTP Device Instance Handle*/
/*This Structure will be passed to Application*/
typedef struct _PTPInstHndl
{
	/*Must Come First*/
	uint32 u32Valid;   				/*Valid Handle,Indicates Handle is Valid or Not*/			
	int bus_num;					/*Added bus_num for backward  code compatibility*/
	int dev_num;					/*Added dev_num for backward compatibility*/
}PTPInstHndl;
/*PTP Callback IDs*/
typedef enum
{
	PtpInvalidCbId=-1,				/*Invalid Callback ID*/
	PtpDevDetectStatusCbId,		/*Device Detect Callback ID*/
	/*IMP NOTE : Currently Not Implemented*/
	/*To be Implemented as per Application Requirment*/
	/*for Async Implementation*/
	PtpGetFileListCbId,				
	PtpGetJpgFileListCbId,			
	PtpGetMp3FileListCbId,
	PtpGetParentHndlCbId,
	PtpGetDirNameCbId,
	PtpGetObjInfoCbId,
	PtpGetNumOfObjsCbId,
	PtpGetNumOfListsCbId,
	PtpGetThumbnailCbId,
	PtpGetJpgImgCbId,
	PtpGetMp3imgCbId,
	PtpGetJpgObjHndlsCbId,
	PtpGetJpgImgInfoCbId,
	PtpGetDevInfoCbId,
	PtpGetStorageIDCbId,
	PtpGetStorageInfoCbId,
	PtpResetDevCbId,
	/*Add here*/
	MaxPtpCbId					/*Max Callback ID Support*/
}PtpCbId_t;
/*Application Callback Function*/
typedef void (*PtpAppCbFn_t)(uint32 ,void * ,uint32 );	/*Application should register Function using PtpAppCbFn_t */
/*Device Detect Callback Status Parameters*/
typedef struct _PtpAppDevDetectStatusCbParam
{
	PtpDevInstances_t DevInst;					/*PTP Device Instance*/
	int BusNum;								/*PTP Device Bus Number*/
	int DevNum;								/*PTP Device Device Number*/
	uint32 ConnectStat;						/*Connect Status i.e. Device is Connected or Disconnected*/
}PtpAppDevDetectStatusCbParam;

/*PTP Driver Handle*/
typedef struct _PTPDriverHandle
{
	uint32	IsOpen;							/*Check Device is Open by Application*/
	uint32	IsDeviceDetect;					/*Check Device is Detected*/
	UsbDev *ptpDevice;						/*Pointer to Usb Device Structure*/
	PTPDevContext * pPTPDevCtxt;				/*Pointer to Device Context*/
	PTPDevHndl	PTPHndl;						/*Dev Hndl used internally in Driver Interface File*/
#ifndef CSP_LAYER_USAGE						
	pthread_mutex_t u32MutexSem;				/*Mutex for each PTP Device*/
	/*CSP Layer Implementation To be used in */
	/*Samsung Platform i.e. Valencia or Trident */
#else
	PCMutex	u32MutexSem;					/*Mutex for each PTP Device*/
#endif
	PtpAppCbFn_t PtpAppCbFn[MaxPtpCbId];		/*Appln CallbackList*/
	PTPInstHndl PtpInstHndl;					/*Handle to Application*/
}PTPDriverHandle;

/*PTP Driver Log Level*/
typedef enum  
{
	PtpDrvLogLevelDebug,						/*For Debugging*/
	PtpDrvLogLevelInfo,							/*For Information*/
	PtpDrvLogLevelWarning,						/*For Warning*/
	PtpDrvLogLevelError,						/*For Error Info*/
}PtpDrvLogLevel_t;

/**
* @fn	  	void PTPDriverLog(PtpDrvLogLevel_t PtpDrvLogLevel,const char *Function, const char *Format, ...)
* @brief	  	Prints PTP Driver Logs for Various Levels
* @param 	(in)	PtpDrvLogLevel
* @param	(in) Function
* @param	(int) Format
* @return	None
*/

void PTPDriverLog(PtpDrvLogLevel_t PtpDrvLogLevel,const char *Function, const char *Format, ...);

#endif	/*_PTPTYPES_H_*/

