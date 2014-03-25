/** 
* @file	PTPDeviceAccess.h
* @brief	PTP Device Access File through USB Interface
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
*/

#ifndef _PTPDEVICEACCESS_H_ 
#define _PTPDEVICEACCESS_H_


#include <stdlib.h>
#include "libusb.h"
#include "PtpTypes.h"

/* Flag for PTP Function using Lib USB 1.0 */

#define USB_DEV_DETECTED 1

#define ADD_NODE(pListHead, pNode) \
{\
    if (!pListHead)\
    { \
    pNode->next = NULL;\
    }\
    else\
    { \
    pListHead->prev = pNode; \
    pNode->next = pListHead; \
    }\
    pListHead = pNode; \
    pNode->prev = NULL; \
}

#define DELETE_NODE(pListHead, pNode) \
{ \
    if (!(pNode->prev))\
    {\
    pListHead = pNode->next;\
    }\
    else\
    { \
    pNode->prev->next = pNode->next; \
    }\
    if (pNode->next)\
    { \
    pNode->next->prev = pNode->prev; \
    }\
    pNode->next = pNode->prev = NULL; \
 } 

/**
* @fn 		void PTPDevUsbInit(void)
* @brief	Intializes the PTP Device through USB Interface
* @param	None	  
* @return  	None
*/
void PTPDevUsbInit(void);

/**
* @fn 		void PTPDevUsbExit(void)
* @brief	Intializes the PTP Device through USB Interface
* @param	None	  
* @return  	None
*/
void PTPDevUsbExit(void);

/**
* @fn 		int  PTPDevUsbOpen(void * pvDev , PUSB_DEVICE pUsbDev)
* @brief		Opens the PTP Device through USB Interface.
* @param	(in)pvDev, Pointer to libusb_device Structure
*@param		(out)pUsbDev, Pointer to USB-Dev Structure
* @return       Device Handle
*/

int  PTPDevUsbOpen(void * pvDev , PUSB_DEVICE pUsbDev);



/**
* @fn 		int PTPDevUsbClose(PUSB_DEVICE pUsbDev)
* @brief	Closes the PTP Device through USB Interface.
* @param	(in)pUsbDev,Pointer to USB-Dev Handle
* @return 	Error code
* @see		None
*/

int PTPDevUsbClose(PUSB_DEVICE pUsbDev);

/**
* @fn 		int PTPDevUsbSetCfg(PUSB_DEVICE pUsbDevHandle, int iConfig)
* @brief	Gets PTP Device through USB Interface.
* @param	(in)pdev,Pointer to USB-Dev Handle
* @return 	Pointer to USB Dev structure
*		 	pdev->device
*/

int PTPDevUsbSetCfg(PUSB_DEVICE pUsbDevHandle, int iConfig);

/**
* @fn 		int PTPDevUsbClaimInterface(PUSB_DEVICE pUsbDevHandle, int iIntFace)
* @brief	Claims PTP Device USB-Interface .
* @param	(in)pUsbDevHandle,Pointer to USB-Dev Handle
*			(in)iIntFace, interface number
* @return 	Error code
*/

int PTPDevUsbClaimInterface(PUSB_DEVICE pUsbDevHandle, int iIntFace);

/**
* @fn 		int PTPDevUsbRelInterface(PUSB_DEVICE pUsbDevHandle, int iIntFace)
* @brief	Releases Claimed PTP Device USB-Interface .
* @param	(in) pUsbDevHandle,Pointer to USB-Dev Handle
*			(in)iIntFace,interface number
* @return 	Error code
*/

int PTPDevUsbRelInterface(PUSB_DEVICE pUsbDevHandle, int iIntFace);

/**
* @fn 		int PTPDevUsbSetAltInterface(PUSB_DEVICE pUsbDevHandle, int iAltIntFace)
* @brief	Releases Claimed PTP Device USB-Interface .
* @param	(in) pUsbDevHandle,Pointer to USB-Dev Handle
*			(in)iAltIntFace,interface number
* @return 	Error code
*/

int PTPDevUsbSetAltInterface(PUSB_DEVICE pUsbDevHandle, int iAltIntFace);

/**
* @fn 	    int PTPDevUsbResetep(PUSB_DEVICE pUsbDevHandle, unsigned int uiEp)
* @brief	Resets Endpoint PTP Device through USB Interface .
* @param	(in)pUsbDevHandle,Pointer to USB-Dev Handle
*			(in)uiEp, EndPoint Number
* @return 	Error code
*/


int PTPDevUsbResetep(PUSB_DEVICE pUsbDevHandle, unsigned int uiEp);

/**
* @fn 	     int PTPDevUsbClrHalt(PUSB_DEVICE pUsbDevHandle, unsigned int uiEp)
* @brief	 Resets Endpoint PTP Device through USB Interface .
* @param     (in)pUsbDevHandle,Pointer to USB-Dev Handle
*		     (in)uiEp, EndPoint Number
* @return 	 Error code
*/

int PTPDevUsbClrHalt(PUSB_DEVICE pUsbDevHandle, unsigned int uiEp);

/**
* @fn 	    int PTPDevUsbReset(PUSB_DEVICE pUsbDevHandle)
* @brief	Resets PTP -USB Device.
* @param	(in)pUsbDevHandle,Pointer to USB-Dev Handle
* @return 	Error code
*/

int PTPDevUsbReset(PUSB_DEVICE pUsbDevHandle);

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

int PTPDevUsbBulkRd(PUSB_DEVICE pUsbDevHandle, int iEp, char *pBytes,int iSize, int iTimeout);

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

int PTPDevUsbBulkWr(PUSB_DEVICE pUsbDevHandle, int iEp, char *pBytes,int iSize, int iTimeout);

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

int PTPDevUsbIntrRd(PUSB_DEVICE pUsbDevHandle, int iEp, char *pBytes,int iSize, int iTimeout);

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

int PTPDevUsbSendCtrlMsg(PUSB_DEVICE pUsbDevHandle, int ibmRequestType,
                         int ibReq, int iwValue, int iwIndex, char *pBytes, int iSize, int iTimeout);
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

int PTPDevUsbGetStr(PUSB_DEVICE pUsbDevHandle, int iDescIndex, int iLangid,	char *pBuf, size_t BufLen);

/**
* @fn 		int PTPDevUsbGetStrSimple(PUSB_DEVICE pUsbDevHandle, int iDescIndex,char *pBuf, size_t BufLen)
* @brief	Gets String Descriptor from PTP-USB Device.
* @param	(in)pUsbDevHandle,Pointer to USB-Dev Handle
*			(in)iDescIndex,Index
*			(out)pBuf,Buffer to Read
*			(in)BufLen, Buffer Length 
* @return 	Error code
*/

int PTPDevUsbGetStrSimple(PUSB_DEVICE pUsbDevHandle, int iDescIndex,char *pBuf, size_t BufLen);

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

int PTPDevUsbGetDesc(PUSB_DEVICE pUsbDevHandle, unsigned char uiType,unsigned char uiDescIndex, void *pBuf, int iSize);

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

int PTPDevUsbGetDescEp(PUSB_DEVICE pUsbDevHandle, int iEp,	unsigned char uiType, unsigned char uiDescIndex, void *pBuf, int iSize);


/**
* @fn	    int PTPDevUsbGetActiveDriver(PUSB_DEVICE pUsbDevHandle, int iIntFace,	char *pName, unsigned int uiNameLen)
* @brief 	Gets Active Driver PTP-USB Interface.
* @param    (in)pUsbDevHandle -Pointer to USB-Dev Handle
*		    (in)iIntFace -Interface Number
*		    (in)pName -Driver Name
*		    (in)uiNameLen- Name Length
* @return   Error code
*/

int PTPDevUsbGetActiveDriver(PUSB_DEVICE pUsbDevHandle, int iIntFace,	char *pName, unsigned int uiNameLen);

/**
* @fn		int PTPDevUsbDetachKernDrv(PUSB_DEVICE pUsbDevHandle, int iIntFace)
* @brief	Detache Active Kernel Driver through PTP-USB Interface.
* @param	(in) pUsbDevHandle -Pointer to USB-Dev Handle
*		    (in)iIntFace -Interface Number
* @return	Error code
*/

int PTPDevUsbDetachKernDrv(PUSB_DEVICE pUsbDevHandle, int iIntFace);


/**
* @fn	  	int PTPDevUsbAttchKernDriv(PUSB_DEVICE pUsbDevHandle, int iIntFace)
* @brief	Attach Kernel Driver through PTP-USB Interface.
* @param  	(in) pUsbDevHandle,Pointer to USB-Dev Handle
*		    (in) iIntFace,Interface Number
* @return	Error code
*/


int PTPDevUsbAttchKernDriv(PUSB_DEVICE pUsbDevHandle, int iIntFace);


/**  
* @fn    	int PTPDevUsbRegisterCbFn(PTP_CALLBACK callback)
* @brief	Registers callback function if any change is there in count of connected devices.
* @param	(in)callback,Fn. Pntr to Registered  Fn.
* @return   Success
*/

int PTPDevUsbRegisterCbFn(PTP_CALLBACK callback);

/**
* @fn	   	PTPDevFindPTPDevices(void)
* @brief	Finds PTP Devices through USB Interface.
* @param   	void
* @return	Error code
*/
int PTPDevFindPTPDevices(void);

/**
* @fn		int TempPTPDevFindPTPDevices(uint8_t uiBusNum, uint8_t uiDevNum);
* @brief		Finds PTP Devices through USB Interface.
* @param   	uint8_t uiBusNum : Bus Number
* @param   	uint8_t uiDevNum : Device Number
* @return   	Error code
*/
int TempPTPDevFindPTPDevices(uint8_t uiBusNum, uint8_t uiDevNum);
/**
* @fn		int PTPDevUsbGetDevices(void)
* @brief	Returns PTP Devices List
* @param	None
* @return	PUSB_DEVICE * : pointer to list of devices
*/

PUSB_DEVICE * PTPDevUsbGetDevices();


#endif /* _PTPDEVICEACCESS_H_ */

