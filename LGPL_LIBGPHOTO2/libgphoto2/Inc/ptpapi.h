/**
* @File		Ptpapi.h
* @brief		PTP Driver Interface Header File
*			This Header file contains API declaration provided by PTP Driver
*			It should be included in Application Source Code to Call PTP Driver APIs
*
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

#ifndef _PTPAPI_H_
#define _PTPAPI_H_


#ifdef __cplusplus
extern "C" {
#endif


#ifdef PTP_THREAD_SAFE

#include "PtpTypes.h"


//#define	OPTIMIZED_PTP		// Not use "PTP_API_Is_Handle_Vaild_MPTP"


/**
* @fn		PTP_API_Init_DeviceList
* @brief		This function Initialises the PTP Driver.
* @param	None
* @return	 Error code
*			PTPAPI_ERROR: in case of error.
*			PTP_RC_OK: in case of success.
* @see		 Added Application Callback Function Parameter
*			If required Application can pass the Callback Function
*			for Device Notification
*/

PtpErr_t PTP_API_Init_DeviceList(PtpAppCbFn_t tAppCbFn);

/**
* @fn		PTP_API_DeInit_DeviceList
* @brief		This function De-Initialises the PTPDriver.
* @param	None
* @return	 Error code
*			PTPAPI_ERROR: in case of error.
*			PTP_RC_OK: in case of success.
* @see		 New API 
*/
PtpErr_t PTP_API_DeInit_DeviceList(void);

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

PtpErr_t   PTP_API_PollDev_MPTP( int16 *pDevChange);

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
PtpErr_t  PTP_API_Get_PTPInstance(int *pPtpDevInst , int BusNum,int DevNum);

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
 PtpErr_t PTP_API_Open_Driver_MPTP(PtpDevInstances_t tDevInst, PTPDeviceHandle  *pPtpDevHndl);
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
 PtpErr_t PTP_API_Close_Driver_MPTP(PtpDevInstances_t tDevInst);



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

PtpErr_t PTP_API_Get_UsbVersion_MPTP(uint8 *uiPackageVer);
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
PtpErr_t PTP_API_InstallCb_MPTP(PTPDeviceHandle  PtpDevHndl ,PtpCbId_t tAppCbId, PtpAppCbFn_t tAppCbFn);


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
PtpErr_t PTP_API_UnInstallCb_MPTP(PTPDeviceHandle  PtpDevHndl, PtpCbId_t tAppCbId);

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

PtpErr_t PTP_API_Get_DeviceInfo_MPTP(PTPDeviceInfo *pDevInfo,PTPDeviceHandle   PtpDevHandle);

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

PtpErr_t PTP_API_Get_StorageIDs_MPTP( PTPStorageIDs *pStorageIds,PTPDeviceHandle   PtpDevHandle);
/**
 * @fn	  	PTP_API_Get_StorageInfos_MPTP
 * @brief	  	Gets Storage IDs from PTP Device .
 * @param 	(in)	 PtpDevHandle
 *		   	(in) 	uiStorageId
 *		   	(out) 	pStorageInfo
 * @return	  Error code
 *		  	PTPAPI_ERROR : in case of error.
 *		  	PTP_RC_OK : in case of success.
 * @see 	   	Old API
 */

PtpErr_t PTP_API_Get_StorageInfos_MPTP(uint32_t uiStorageId, PTPStorageInfo* pStorageInfo,PTPDeviceHandle   PtpDevHandle);
/**
 * @fn	  	Is_PTP_device_MPTP
 * @brief	  	Checks device is PTP or Not
 * @param 	(in)	 ptpHandle
 * @return	  Error code
 *		 	PTPAPI_ERROR : in case of error.
 *		  	PTP_RC_OK : in case of success.
 * @see 	   	Old API
 */

PtpErr_t Is_PTP_device_MPTP(PTPDeviceHandle  PtpDevHndl);

/**
 * @fn	  	PTP_API_Get_JpegObjectHandles_MPTP
 * @brief	  	This function gets Jpeg image object handles
 * @param 	(in)	 PtpDevHndl
 *		   	(in) 	uiStorageID
 *		   	(out) 	pPtpObjHndl
 * @return	  Error code
 *		 	PTPAPI_ERROR : in case of error.
 *		  	PTP_RC_OK : in case of success.
* @see		 Old API
*/

PtpErr_t PTP_API_Get_JpegObjectHandles_MPTP( uint32 uiStorageID, PTPObjectHandles *pPtpObjHndl,PTPDeviceHandle  PtpDevHndl);

/**
 * @fn	 	 PTP_API_Is_Handle_Vaild_MPTP
 * @brief	  	Checks object Handle is valid or not
 * @param 	(in)	 PtpDevHndl
 *		   	(in) 	uiObjectHandle
 * @return	  Error code
 *		  	PTPAPI_ERROR: in case of error.
 *		  	PTP_RC_OK: in case of success.
 * @see 	  	Old API
 */

PtpErr_t PTP_API_Is_Handle_Vaild_MPTP (uint32 uiObjectHandle,PTPDeviceHandle  PtpDevHndl);
/**
 * @fn	  	PTP_API_Get_ParentHandle_MPTP
 * @brief	  	Gets Parent Objects Handle
 * @param 	(in)	 PtpDevHndl
 *		   	(in) 	uiObjectHandle
 *		   	(out) 	pParentHandle
 * @return	  Error code
 *		  	PTPAPI_ERROR : in case of error.
 *		  	PTP_RC_OK : in case of success.
 * @see 	  	Old API
 */

PtpErr_t PTP_API_Get_ParentHandle_MPTP ( uint32 uiObjectHandle, uint32 *pParentHandle,PTPDeviceHandle  PtpDevHndl);
/**
 * @fn	  	PTP_API_Get_DirName_MPTP
 * @brief	  	Gets Directory Name
 * @param 	(in)	 PtpDevHndl
 *		   	(in) 	uiObjectHandle
 *		   	(out) 	pParentDir
 * @return	  Error code
 *		 	PTPAPI_ERROR : in case of error.
 *		  	PTP_RC_OK: in case of success.
 * @see 	  Old API
 */

PtpErr_t PTP_API_Get_DirName_MPTP (uint32 uiObjectHandle, char *pParentDir,PTPDeviceHandle  PtpDevHndl );

/**
 * @fn	  	PTP_API_Get_NumOfObjects_MPTP
 * @brief	  	Get Number of objects
 * @param 	(in)	 PtpDevHndl
 *		   	(in) 	uiStorageID
 *		   	(in) 	uiParentObjectHandle
 *		   	(in) 	ObjectFormat
 *		   	(out) 	pnNum
 * @return	  Error code
 *		  	PTPAPI_ERROR : in case of error.
 *		 	PTP_RC_OK: in case of success.
 * @see 	  	Old API
 */

PtpErr_t PTP_API_Get_NumOfObjects_MPTP( uint32 uiStorageID, uint32 uiParentObjectHandle, uint32 ObjectFormat, uint32 *pnNum,PTPDeviceHandle  PtpDevHndl);
/**
 * @fn	 	PTP_API_Get_ObjectInfo_MPTP
 * @brief	  	Get object info
 * @param 	(in)	 PtpDevHndl
 *		   	(in) 	uiObjectHandle
 *		   	(out) 	pPtpObjInfo
 * @return	  Error code
 *		  	PTPAPI_ERROR : in case of error.
 *		  	PTP_RC_OK : in case of success.
 * @see 	  	Old API
 */

PtpErr_t PTP_API_Get_ObjectInfo_MPTP (uint32 uiObjectHandle, PTPObjectInfo *pPtpObjInfo,PTPDeviceHandle  PtpDevHndl );
/**
 * @fn	  	PTP_API_Get_JpegFilesList_MPTP
 * @brief	 	 Get Jpeg File List
 * @param 	(in)	 PtpDevHndl
 *		   	(in) 	uiStorageID
 *		   	(in) 	uiParentObjectHandle
 *		   	(out) 	pFileInfo
 *		   	(out) 	pnCount
  * @return	  Error code
 *		  	PTPAPI_ERROR : in case of error.
 *		  	PTP_RC_OK : in case of success.
 * @see 	  	Old API
 */

PtpErr_t PTP_API_Get_JpegFilesList_MPTP (uint32 uiStorageID, uint32 uiParentObjectHandle, PTPFileListInfo **pFileInfo, uint32 *pnCount,PTPDeviceHandle  PtpDevHndl );
/**
 * @fn	  	PTP_API_Get_Thumbnail_MPTP
 * @brief	  	Get Jpeg Thumbnail pImage
 * @param 	(in)	 PtpDevHndl
 *		   	(in) 	uiHndl
 *		   	(out) 	pImage
 *		   	(out) 	pImageSize
  * @return	  Error code
 *		  	PTPAPI_ERROR : in case of error.
 *		 	PTP_RC_OK: in case of success.
 * @see 	  	Old API
 */

PtpErr_t PTP_API_Get_Thumbnail_MPTP(uint32 uiHndl, char **pImage, uint32 *pImageSize,PTPDeviceHandle  PtpDevHndl);
/**
 * @fn	  	PTP_API_Get_JpegImage_MPTP
 * @brief	  	Get Jpeg pImage
 * @param 	(in)	 PtpDevHndl
 *		   	(in) 	handle
 *		   	(out) 	pImage
 *		   	(out) 	pImageSize
  * @return	  Error code
 *			 PTPAPI_ERROR : in case of error.
 *		 	PTP_RC_OK : in case of success.
 * @see 	  	Old API
 */

PtpErr_t PTP_API_Get_JpegImage_MPTP (uint32 uiHndl, char **pImage, uint32 *pImageSize,PTPDeviceHandle  PtpDevHndl );
/**
 * @fn	  	PTP_API_Get_DevPath_MPTP
 * @brief	  	Get Device Path
 * @param 	(in)	 PtpDevHndl
 *		   	(out) 	pDevpath
 * @return	  Error code
 *		  	PTPAPI_ERROR : in case of error.
 *		  	PTP_RC_OK : in case of success.
 * @see 	  	Old API
 */

PtpErr_t PTP_API_Get_DevPath_MPTP(char *pDevpath,PTPDeviceHandle  PtpDevHndl);
/**
 * @fn	  	PTP_API_Get_FilesList_MPTP
 * @brief	  	Get File List
 * @param	 (in)	 PtpDevHndl
 *		   	(in)	uiStorageID
 *		   	(in)	uiParentObjectHandle
 *		   	(out)   pFileInfo
 *		   	(out)	pnCount
 * @return	  Error code
 *		 	 PTPAPI_ERROR: in case of error.
 *		 	PTP_RC_OK : in case of success.
 * @see 	  	Old API
 */

PtpErr_t PTP_API_Get_FilesList_MPTP(uint32 uiStorageID, uint32 uiParentObjectHandle, PTPFileListInfo **pFileInfo, uint32 *pnCount,PTPDeviceHandle  PtpDevHndl);
/**
 * @fn	  	PTP_API_Get_JpegImageInfo_MPTP
 * @brief	  	Get Jpeg pImage Info
 * @param 	(in)	 PtpDevHndl
 *		   	(in)	uiHndl
 *		   	(out)	PtpObjInfo
 * @return	  Error code
 *		 	PTPAPI_ERROR: in case of error.
 *		  	PTP_RC_OK : in case of success.
 * @see 	  	Old API
 */

PtpErr_t PTP_API_Get_JpegImageInfo_MPTP (uint32 uiHndl, PTPObjectInfo *pPtpObjInfo,PTPDeviceHandle  PtpDevHndl);

/**
 * @fn	  	PTP_API_Get_MP3FilesList_MPTP
 * @brief	  	Get MP3 File List
 * @param 	(in)	 PtpDevHndl
 *		  	 (in)	uiStorageID
 *		   	(in)	uiParentObjectHandle
 *		   	(out)   pFileInfo
 *		   	(out)	pnCount
 * @return	  Error code
 *		  	PTPAPI_ERROR: in case of error.
 *		  	PTP_RC_OK : in case of success.
 * @see 	  	Old API
 */

PtpErr_t PTP_API_Get_MP3FilesList_MPTP (uint32 uiStorageID, uint32 uiParentObjectHandle, PTPFileListInfo **pFileInfo, uint32 *pnCount,PTPDeviceHandle  PtpDevHndl);

/**
 * @fn	  	PTP_API_Get_MP3File_MPTP
 * @brief	  	Get MP3 Files 
 * @param 	(in)	 PtpDevHndl
 *		   	(in)	uiHndl
 *		   	(out)   pImage
 *		   	(out)	pImageSize
 * @return	  Error code
 *		  	PTPAPI_ERROR : in case of error.
 *		  	PTP_RC_OK : in case of success.
 * @see 	  	Old API
 */

PtpErr_t PTP_API_Get_MP3File_MPTP(uint32 uiHndl, char **pImage, uint32 *pImageSize,PTPDeviceHandle  PtpDevHndl);
/**
 * @fn	  	PTP_API_Send_Reset_MPTP
 * @brief	  	Resets Ptp Device
 * @param 	(in)	 PtpDevHndl
 * @return	  Error code
 *		  	PTPAPI_ERROR : in case of error.
 *		  	PTP_RC_OK: in case of success.
 * @see 	  	Old API
 */

PtpErr_t PTP_API_Send_Reset_MPTP(PTPDeviceHandle  PtpDevHndl);
/**
 * @fn	  	PTP_API_GetDeviceList
 * @brief	  	Get Device List i.e connected Device
 * @param 	(in)	 PtpDevHandle
 *		   	(out)      pDevCnt
 * @return	  Error code
 *		  	PTPAPI_ERROR: in case of error.
 *		  	PTP_RC_OK: in case of success.
 
 * @see 	  	Old API
 * 			Changed 1st parameter in Previous API, i.e.  UsbDev* and swaps the Parameters
 */

PtpErr_t  PTP_API_GetDeviceList(uint32 *pDevCnt,PTPDeviceHandle   PtpDevHandle);
/**
 * @fn	 	PtpPartialRead
 * @brief	 	Partial Read operation for the given PTP device,
 * @brief 	 	reads up to length bytes from uiStartOffset into
 * 		 	the buffer starting at buf
 * @param 	(in)	  PtpDevHndl
 *		   	(in)	uiHndl
 *		   	(in)	uiLength
 *		   	(in)	uiStartOffset
 *		   	(out)	pBuf
 *		   	(out)    pReturnCurOffset
 * @return	  Error code
 *		  	PTPAPI_ERROR : in case of error.
 *		  	PTP_RC_OK : in case of success.
 * @see 	  	Old API
 */
PtpErr_t PTP_API_PartialRead_MPTP(uint32 uiHndl, char** pBuf, uint32 uiLength, uint32 uiStartOffset, uint32 *pReturnCurOffset,PTPDeviceHandle  PtpDevHndl);


/*These below API provided to Application for Memory Leak by Driver Interface*/
/*APIs for Initializing and clearing Structure's*/
/**
* @fn           void PTP_API_Init_DeviceInfo(PTPDeviceInfo *pDevInfo)
* @brief	      Initializes the DeviceInfomation to Null
* @remarks	none
* @param	PTPDeviceInfo
* @return	none
*/ 
void PTP_API_Init_DeviceInfo(PTPDeviceInfo *di);
/**
* @fn           void PTP_API_Init_ObjectHandles(PTPObjectHandles *ohpObjHand)
* @brief	      Initializes the Objecthandles
* @remarks	none
* @param	PTPObjectHandles:  Handle for the given ptp device
* @return	none
*/ 
void PTP_API_Init_ObjectHandles(PTPObjectHandles *oh);

/**
* @fn           void PTP_API_Init_ObjectInfo(PTPObjectInfo *pObjextinfo)
* @brief	     Initializing the ObjectInfo
* @remarks		none
* @param		PTPObjectInfo: ObjectInfo for the given ptp device
* @return		none
*/
void PTP_API_Init_ObjectInfo(PTPObjectInfo *oi);
/**
* @fn           void PTP_API_Init_PTPFileListInfo(PTPFileListInfo **pFileinfo)
* @brief	     Initializing the FileListInfo
* @remarks		none
* @param		PTPFileListInfo: FileListInfo for the given ptp device
* @return		none
*/
void PTP_API_Init_PTPFileListInfo(PTPFileListInfo **fileinfo);	

/**
* @fn           void PTP_API_Init_StorageIDs(PTPStorageIDs *pStorageids)
* @brief	     Initializes the StorageIDs
* @remarks		none
* @param		PTPStorageIDs: StorageIDs for the given ptp device
* @return		none
*/
void PTP_API_Init_StorageIDs(PTPStorageIDs *storageids);

/**
* @fn           void PTP_API_Init_PTPStorageInfo(PTPStorageInfo *pStorageInfo)
* @brief	     Initializing the StorageInfo
* @remarks		none
* @param		PTPStorageInfo 
* @return		none
*/
void PTP_API_Init_PTPStorageInfo(PTPStorageInfo *storageInfo);
/**
* @fn           void PTP_API_Clear_DeviceInfo(PTPDeviceInfo *di)
* @brief	     Clearing the Storage ids of the device
* @remarks		none
* @param		PTPStorageIDs  
* @return		none
*/

void PTP_API_Clear_DeviceInfo(PTPDeviceInfo *di);

/**
* @fn           void PTP_API_Clear_ObjectHandles(PTPObjectHandles *oh)
* @brief	     Clearing the object handles of the device
* @remarks		none
* @param		PTPObjectHandles  
* @return		none
*/

void PTP_API_Clear_ObjectHandles(PTPObjectHandles *oh);

/**
* @fn           void PTP_API_Clear_ObjectInfo(PTPObjectInfo *oi)
* @brief	     Clearing the object info of the device
* @remarks		none
* @param		PTPObjectInfo  
* @return		none
*/
void PTP_API_Clear_ObjectInfo(PTPObjectInfo *oi);

/**
* @fn          void PTP_API_Clear_PTPFileListInfoInfo(PTPFileListInfo **fileinfo)
* @brief	     Clearing the FileListInfo of the device
* @remarks		none
* @param		PTPObjectInfo  
* @return		none
*/
void PTP_API_Clear_PTPFileListInfoInfo(PTPFileListInfo **fileinfo);
/**
* @fn           void PTP_API_Clear_StorageIDs(PTPStorageIDs *pStorageIds)
* @brief	     Clearing the Storage ids of the device
* @remarks		none
* @param		PTPStorageIDs  
* @return		none
*/
void PTP_API_Clear_StorageIDs(PTPStorageIDs *storageids);
/**
* @fn          void PTP_API_Clear_PTPStorageInfo(PTPStorageInfo *storageInfo)
* @brief	     Clearing the StorageInfo of the device
* @remarks		none
* @param		PTPStorageInfo  
* @return		none
*/
void PTP_API_Clear_PTPStorageInfo(PTPStorageInfo *storageInfo);


#else
#include "ptp.h"
#include "usb.h"
#include <pthread.h>

#define	OPTIMIZED_PTP

typedef unsigned char uint8;
typedef unsigned int uint16;
typedef unsigned long uint32;

typedef signed char int8;
typedef signed int int16;
typedef signed long int32;

//#define BOOL		unsigned char
#define Bool		unsigned char
#define true		1
#define false		0

#define PTP_START_INDEX	1

#define USB_CLASS_PTP		6

#define USB_DP_HTD		(0x00 << 7)
#define USB_DP_DTH		(0x01 << 7)

#define USB_REQ_DEVICE_RESET			0x66
#define USB_REQ_GET_DEVICE_STATUS	0x67

#define USB_FEATURE_HALT		0x00

#define USB_TIMEOUT				10000
#define USB_CAPTURE_TIMEOUT	20000

#define PTP_TIMEOUT		10000
#define PTP_USB_URB		2097152

#define MAX_FILENAME		255

#define PTP_PRINT	printf

//newly appended.
#define PTPAPI_ERROR		0xffff
#define DO_GET_DEVICE_INFO				1
#define DO_NOT_GET_DEVICE_INFO		0

#define	RETRY_CNT	3

// PTP 디바이스의 valid storage를 check하기 위한 mask 
#define PTP_LOGICAL_STORAGE_MASK    0x0000FFFF
#define PTP_PHYSICAL_STORAGE_MASK   0xFFFF0000

//#ifdef LINUX_OS
#if 0
	#define USB_BULK_READ ptp_bulk_read
	#define USB_BULK_WRITE ptp_bulk_write
	
	int ptp_bulk_read(usb_dev_handle *dev, int ep, char *bytes, int size,	int timeout);
	int ptp_bulk_write(usb_dev_handle *dev, int ep, char *bytes, int length, int timeout);
#else
	#define USB_BULK_READ usb_bulk_read		/* defined in linux.c */
	#define USB_BULK_WRITE usb_bulk_write		/* defined in linux.c */
#endif


// #define SIZE_VENDORNAME		30
// #define SIZE_PRODUCTNAME	30


	
enum {FILE_TYPE = 1, DIR_TYPE};

struct _PTP_USB {
	usb_dev_handle* handle;
	int inep;
	int outep;
	int intep;
};
typedef struct _PTP_USB PTP_USB;

struct _PTPDevContext
{
        PTPParams		params;
        PTP_USB			ptp_usb;
        struct usb_bus 		*bus;
        struct usb_device 	*dev;
};
typedef struct _PTPDevContext PTPDevContext;


struct _PTPDevHandle
{
	uint32 init_status;
	uint32 device_status;//attached or removed
	PTPDevContext *ptp_dev_ctx;
	struct _PTPDevHandle *next;
};
typedef struct _PTPDevHandle PTPDevHandle;


struct _PTPFileListInfo
{
	char dirname[MAX_FILENAME];
	char filename[MAX_FILENAME];
	char filetype;
	uint32 storageId;
	uint32 handle;
	uint32 ParentObject;
	int year;
	int month;
	int day;
	int hour;
	int min;
	int sec;
};
typedef struct _PTPFileListInfo PTPFileListInfo;


struct _PTPDeviceHandle
{
	int bus_num;
	int dev_num;
};
typedef struct _PTPDeviceHandle PTPDeviceHandle;


typedef struct  _UsbDev
{
	PTPDeviceHandle ptp_path;
// 	char devpath[24];
	char vendor[80];
	char model[80];
	int storage_id[2];
	PTPStorageIDs sids;
	PTPStorageInfo 	storageinfo[2];
	char	SerialNumber[80];
	
	uint16 DeviceId;
	
}UsbDev;



typedef bool (*ptp_monitoring) (void);

uint16 PTP_Open_Device(PTPDevContext *dc);
uint16 PTP_Open_Device_MPTP(PTPDevContext *dc);
uint16 PTP_Close_Device(PTPDevContext *dc);
uint16 get_usb_endpoint_status(PTPDevContext *dc, uint16 *status);
uint16 clear_usb_stall_feature(PTPDevContext *dc, int ep);
uint16 get_usb_device_status(PTPDevContext *dc, uint16 *device_status);
uint16 set_usb_device_reset(PTPDevContext *dc);
Bool usb_find_device (PTPDevContext *dc);
Bool  usb_find_device_MPTP (PTPDevContext *dc);//vishal
uint16 usb_find_endpoints(PTPDevContext *dc);
uint16 usb_initialize_port (PTPDevContext *dc);
uint16 usb_clear_stall(PTPDevContext *dc);

uint16 ptp_deviceinfo_copy(PTPDeviceInfo *dest, PTPDeviceInfo *src);
uint16 ptp_objectinfo_copy(PTPObjectInfo *dest, PTPObjectInfo *src);
uint16 ptp_handles_copy(PTPObjectHandles *dest , PTPObjectHandles *src);



void PTP_API_Init_DeviceInfo(PTPDeviceInfo *di);
void PTP_API_Init_ObjectHandles(PTPObjectHandles *oh);
void PTP_API_Init_ObjectInfo(PTPObjectInfo *oi);
void PTP_API_Init_PTPFileListInfo(PTPFileListInfo **fileinfo);
void PTP_API_Init_StorageIDs(PTPStorageIDs *storageids);
void PTP_API_Init_PTPStorageInfo(PTPStorageInfo *storageInfo);
void PTP_Init_Transaction(PTPDevContext *dc);
void PTP_API_Clear_DeviceInfo(PTPDeviceInfo *di);
void PTP_API_Clear_ObjectHandles(PTPObjectHandles *oh);
void PTP_API_Clear_ObjectInfo(PTPObjectInfo *oi);
void PTP_API_Clear_PTPFileListInfoInfo(PTPFileListInfo **fileinfo);
void PTP_API_Clear_StorageIDs(PTPStorageIDs *storageids);
void PTP_API_Clear_PTPStorageInfo(PTPStorageInfo *storageInfo);
void PTP_Terminate_Transaction(PTPDevContext *dc);
Bool PTP_API_Check_Devices(void);  

Bool PTP_API_Check_Devices_MPTP (uint16 *ptp_dev_cnt);  //sheetal added

//////////////////////////////////////////////////////////////////////////////////
// newly updated by sang u shim 
// date : 02Nov 2006
//////////////////////////////////////////////////////////////////////////////////
Bool t_FileExist(const char* fileName);
Bool t_FindString(const char *pStr, const char *pFindStr, int bContinue);
Bool PTP_API_CHKDEV(void);
Bool PTP_API_CHKDEV_MPTP(PTPDeviceHandle  ptpHandle);
uint16 PTP_API_OPENSESSION(void);
uint16 PTP_API_OPENSESSION_MPTP(PTPDeviceHandle  ptpHandle);//vishal
uint16 PTP_API_CLOSESESSION(void);
uint16 PTP_API_CLOSESESSION_MPTP(PTPDeviceHandle  ptpHandle); //vishal
uint16 PTP_API_OPENDEVICE(Bool getdevinfo);
uint16 PTP_API_OPENDEVICE_MPTP(Bool isgetdeviceinfo, PTPDeviceHandle  ptpHandle);//vishal
uint16 PTP_API_CLOSEDEVICE(void);
uint16 PTP_API_CLOSEDEVICE_MPTP(PTPDeviceHandle  ptpHandle); //vishal
uint16 PTP_API_INIT_COMM(void);
uint16 PTP_API_INIT_COMM_MPTP(PTPDeviceHandle  ptpHandle); //vishal
uint16 PTP_API_FINISH_COMM(void);

uint16 PTP_API_FINISH_COMM_MPTP(PTPDeviceHandle  ptpHandle); //vishal

Bool PTP_API_Is_Handle_Vaild (uint32 ObjectHandle);
Bool PTP_API_Is_Handle_Vaild_MPTP (uint32 ObjectHandle, PTPDeviceHandle  ptpHandle);//sheetal added


Bool Is_PTP_device(PTPDeviceInfo *deviceinfo);
Bool Is_PTP_device_MPTP(PTPDeviceHandle  ptpHandle);//sheetal added
uint16 PTP_API_Get_FilesList (uint32 storageID, uint32 ParentObjectHandle, PTPFileListInfo **fileinfo, uint32 *nCount);
uint16 PTP_API_Get_FilesList_MPTP (uint32 storageID, uint32 ParentObjectHandle, PTPFileListInfo **fileinfo, uint32 *nCount, PTPDeviceHandle  ptpHandle); //vishal

uint16 PTP_API_Get_JpegFilesList(uint32 storageID, uint32 ParentObjectHandle, PTPFileListInfo **fileinfo, uint32 *nCount);
uint16 PTP_API_Get_JpegFilesList_MPTP (uint32 storageID, uint32 ParentObjectHandle, PTPFileListInfo **fileinfo, uint32 *nCount,PTPDeviceHandle  ptpHandle);

uint16 PTP_API_Get_MP3FilesList(uint32 storageID, uint32 ParentObjectHandle, PTPFileListInfo **fileinfo, uint32 *nCount);
uint16 PTP_API_Get_MP3FilesList_MPTP (uint32 storageID, uint32 ParentObjectHandle, PTPFileListInfo **fileinfo, uint32 *nCount, PTPDeviceHandle  ptpHandle);//sheetal added
uint16 PTP_API_Get_ParentHandle(uint32 ObjectHandle, uint32 *ParentHandle);
uint16 PTP_API_Get_ParentHandle_MPTP (uint32 ObjectHandle, uint32 *ParentHandle, PTPDeviceHandle  ptpHandle);//sheetal added
uint16 PTP_API_Get_DirName(uint32 ObjectHandle, char *ParentDir);
uint16 PTP_API_Get_DirName_MPTP (uint32 ObjectHandle, char *ParentDir, PTPDeviceHandle  ptpHandle);//sheetal added
uint16 PTP_API_Get_ObjectInfo(uint32 ObjectHandle, PTPObjectInfo *oi);
uint16 PTP_API_Get_ObjectInfo_MPTP (uint32 ObjectHandle, PTPObjectInfo *oi, PTPDeviceHandle  ptpHandle);//sheetal added
uint16 PTP_API_Get_NumOfObjects(uint32 storageID, uint32 ParentObjectHandle, uint32 ObjectFormat, uint32 *nNum);
uint16 PTP_API_Get_NumOfObjects_MPTP(uint32 storageID, uint32 ParentObjectHandle, uint32 ObjectFormat, uint32 *nNum, PTPDeviceHandle  ptpHandle);//sheetal added

uint16 PTP_API_Get_NumOfList(uint32 storageID, uint32 ParentObjectHandle, uint32 *nNum);
uint16 PTP_API_Get_NumOfList_MPTP(uint32 storageID, uint32 ParentObjectHandle, uint32 *nNum, PTPDeviceHandle  ptpHandle);//sheetal added

uint16 PTP_API_Get_Thumbnail(uint32 handle, char **Image, uint32 *ImageSize);
uint16 PTP_API_Get_Thumbnail_MPTP(uint32 handle, char **Image, uint32 *ImageSize,PTPDeviceHandle  ptpHandle);//sheetal added
uint16 PTP_API_Get_JpegImage(uint32 handle, char **Image, uint32 *ImageSize);
uint16 PTP_API_Get_JpegImage_MPTP(uint32 handle, char **Image, uint32 *ImageSize, PTPDeviceHandle  ptpHandle);//sheetal added
uint16 PTP_API_Get_JpegObjectHandles (uint32 storageID, PTPObjectHandles *oh1 );
uint16 PTP_API_Get_JpegObjectHandles_MPTP (uint32 storageID, PTPObjectHandles *oh1, PTPDeviceHandle  ptpHandle );//sheetal added

void PTP_API_PartialReadStart(void);
void PTP_API_PartialReadStop(void);	
uint16 PTP_API_PartialRead(uint32 handle, char** buf, uint32 length, uint32 StartOffset, uint32 *ReturnCurOffset);
uint16 PTP_API_PartialRead_MPTP(uint32 handle, char** buf, uint32 length, uint32 StartOffset, uint32 *ReturnCurOffset, PTPDeviceHandle  ptpHandle);//added by niraj

uint16 PTP_API_Get_MP3File(uint32 handle, char **Image, uint32 *ImageSize);
uint16 PTP_API_Get_MP3File_MPTP(uint32 handle, char **Image, uint32 *ImageSize, PTPDeviceHandle  ptpHandle);//sheetal added
uint32 PTP_API_Get_JpegImageInfo(uint32 handle, PTPObjectInfo *oi);
uint32 PTP_API_Get_JpegImageInfo_MPTP(uint32 handle, PTPObjectInfo *oi,PTPDeviceHandle  ptpHandle);//sheetal added
uint16 PTP_API_Get_DeviceInfo(PTPDeviceInfo *deviceinfo);
uint16 PTP_API_Get_DeviceInfo_MPTP(PTPDeviceInfo *deviceinfo, PTPDeviceHandle  ptpHandle);//sheetal added

uint16 PTP_API_Get_DevPath(char *devpath);
uint16 PTP_API_Get_DevPath_MPTP(char *devpath, PTPDeviceHandle  ptpHandle);

uint16 PTP_API_Get_StorageIDs(PTPStorageIDs *storageids);  
uint16 PTP_API_Get_StorageIDs_MPTP(PTPDeviceHandle  ptpHandle,PTPStorageIDs *storageids);  // sheetal added
uint16 PTP_API_Get_StorageInfos(uint32_t storageid, PTPStorageInfo* storageinfo); 
uint16 PTP_API_Get_StorageInfos_MPTP(PTPDeviceHandle  ptpHandle, uint32_t storageid, PTPStorageInfo* storageinfo);  // sheetal added
uint16 PTP_API_Send_Reset(void);
uint16 PTP_API_Send_Reset_MPTP(PTPDeviceHandle  ptpHandle);//sheetal added
uint16  PTP_API_CheckValidStorageID(unsigned int storageID);

//uint16 PTP_API_Init_DeviceList (ptp_monitoring cb);
uint16 PTP_API_Init_DeviceList ();
//static PTPDevContext *getDc(PTPDeviceHandle  ptpHandle );//to map from devpath to device context
void PTP_API_GetDeviceList(UsbDev* device[],uint32_t* count);//added by Niraj

#endif
#ifdef __cplusplus
}
#endif



#endif /* _PTPAPI_H_ */
