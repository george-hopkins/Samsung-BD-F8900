/**
* @File		PtpCallback.h
* @brief		PTP Callback Functions declaration
* @author	Sandeep Kumar(SISC)
* @Date	    	January 8, 2010

 * Copyright 2010 by Samsung Electronics, Inc.,
 * 
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */


 
#ifndef _PTPCALLBACK_H_
#define _PTPCALLBACK_H_

#include "ptp.h"
#include "PtpTypes.h"

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

PtpErr_t PtpCallbackUpdateDevInfo(Bool bConnectStat,UsbDev *ptpDevice,PTPDevContext *pPTPDevCtxt);


/**
* @fn	  	PtpErr_t PtpCallbackInvokeCb(PtpDevInstances_t tDevInst,PtpCbId_t tAppCbId, void *pCbParam)
* @brief	  	Invokes the installed i.e Registered Callback function
* @param 	(in)	 tDevInst,Device Instance Number
*		  	(in)     tAppCbId, Callback ID
*			(in)     pCbParam, Callback param
* @return  	Error code
*		 	PTPAPI_ERROR : in case of error.
*		  	PTP_RC_OK : in case of success.
*/
PtpErr_t PtpCallbackInvokeCb(PtpDevInstances_t tDevInst,PtpCbId_t tAppCbId, void *pCbParam);

/**
* @fn	       uint16 PtpCallbackGetDevInfo(PTPDeviceInfo *deviceinfo, PtpDevHndl  ptpHandle)
* @brief	       Get information of the given ptp device
* @param	PTPDeviceInfo *deviceinfo	: the store of a device information
* @param	PTPDevContext *dc		:Device context of the given device
* @param	PtpDevHndl  ptpHandle	:Ptp Device handle
* @return  	Error code
*		  	PTPAPI_ERROR : in case of error.
*		   	PTP_RC_OK : in case of success.
*/ 
uint16 PtpCallbackGetDevInfo(PTPDeviceInfo *pDeviceinfo, PtpDevHndl  ptpHandle);



/**
* @fn	       uint16 PtpCallbackGetStorageIDs(PtpDevHndl  ptpHandle,PTPStorageIDs *storageids)
* @brief	       Get list of storageIds from the given ptp device
* @param	PTPStorageIDs *storageids:		list of storage ids
* @param	PtpDevHndl ptpHandle:	Handle for the given ptp device
* @return  	Error code
*		  	PTPAPI_ERROR : in case of error.
*		   	PTP_RC_OK : in case of success.
*/ 
uint16 PtpCallbackGetStorageIDs(PtpDevHndl  ptpHandle,PTPStorageIDs *pStorageids);
/**
* @fn	       uint16 PtpCallbackGetStorageInfos(PtpDevHndl  ptpHandle, uint32_t storageid, PTPStorageInfo* storageinfo)
* @brief	       Get storage information of the given storage id from the given  device
* @param	PTPStorageInfo* storageinfo:	structure for storage infomation		
* @param	PtpDevHndl ptpHandle:	Handle for the given ptp device
* @return      	Error code
*		  	PTPAPI_ERROR : in case of error.
*		   	PTP_RC_OK : in case of success.
*/ 
uint16 PtpCallbackGetStorageInfos(PtpDevHndl  ptpHandle, uint32_t uiStorageid, PTPStorageInfo* pStorageinfo);
/**
* @fn           	void PtpPrivateInitTransaction(PTPDevContext *pDevCtxt)
* @brief	     	Initializing the Context data of the device
* @remarks	none
* @param	PTPDevContext :  
* @return	none
*/
void   PtpPrivateInitTransaction(PTPDevContext *dc);

/**
* @fn	      	uint16 PtpCallbackFinishComm(PtpDevHndl  ptpHandle)
* @brief	      	Finishing process of communication with the given ptp device
* @remarks	none
* @param	PtpDevHndl ptpHandle:    Handle for the given ptp device
* @return       Error code
*		       PTPAPI_ERROR : in case of error.
*		       PTP_RC_OK : in case of success.
*/
uint16 PtpCallbackFinishComm(PtpDevHndl  ptpHandle);
/**
 *@brief	 	This function opens the given device and updates the DC
 *@ remarks  	Endpoints are updated and ports are intialized
 *@param	PTPDevContext *dc:  Device context of the given device
 *@return	uint16
 *			PTPAPI_ERROR : in case of error.
 *			PTP_RC_OK : in case of success.
 */

uint16 PtpPrivateOpenDeviceContext(PTPDevContext *dc);
/**
 *@brief	 	This function closes device
 *@param	PTPDevContext *dc:  Device context of the given device
 *@return	uint16
 *			PTPAPI_ERROR : in case of error.
 *			PTP_RC_OK : in case of success.
 */

uint16 PtpPrivateCloseDeviceContext(PTPDevContext *dc);


/**
* @fn	      	uint16 PtpCallbackInitComm(PtpDevHndl  ptpHandle)
* @brief	      	Initialization of communication with the given ptp device
* @remarks	This function must be called onetime when the device is attached
* @param	PtpDevHndl ptpHandle:    Handle for the given ptp device
* @return  	Error code
*		  	PTPAPI_ERROR : in case of error.
*		   	PTP_RC_OK : in case of success.
*/
uint16 PtpCallbackInitComm(PtpDevHndl  ptpHandle);
/**
   * @fn	PtpDevLock
   * @brief 	Locks Device Mutex
   * @param (in)   DevInst
   * @return	None
   * @see		
   */

void PtpDevLock(PtpDevInstances_t DevInst);

 /**
  * @fn 	   PtpDevUnLock
  * @brief	   UnLocks Device Mutex
  * @param (in)   DevInst
  * @return    None
  * @see	   
  */

void PtpDevUnLock(PtpDevInstances_t DevInst);

#endif	/*_PTPCALLBACK_H_*/

