/**********************************************************************
* File Name   : Bluetooth_CM.h
* Description : This file define functions and variables for Connection Manager.
* Updates			:
***********************************************************************/

#ifndef _BLUETOOTH_CM_H_
#define _BLUETOOTH_CM_H_

#include "csr_types.h"


typedef struct {
	CsrUint16		qid;
//	void 			*recvMsgP;
    CsrBtDeviceAddr             localDeviceAddr;
	CsrBtDeviceName             localName;
	CsrBool 					isInit;
} CmAppInst;

extern void init_cm(void **gash);
extern void deinit_cm(void **gash);
extern void handleCmPrim(CmAppInst  *inst,CsrPrim    *primType);

#endif /*_BLUETOOTH_CM_H_*/
