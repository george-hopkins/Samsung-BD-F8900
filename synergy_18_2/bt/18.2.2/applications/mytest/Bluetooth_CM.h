/**********************************************************************
* File Name   : Bluetooth_CM.h
* Description : This file define functions and variables for Connection Manager.
* Author 	  :	Kim Jin Cheol	(Connectivity Team, HYUNDAI MOBIS)
* Updates			:
***********************************************************************/

#ifndef _BLUETOOTH_CM_H_
#define _BLUETOOTH_CM_H_

#include "csr_types.h"


typedef struct {
	CsrUint16		qid;
	void 			*recvMsgP;
    CsrBtDeviceAddr             bdLocal;
    CsrUint8                    selectedRemoteDeviceIdx;
} CmAppInst;

extern void init_cm(void **gash);
extern void deinit_cm(void **gash);
extern void handleCmPrim(CmAppInst  *inst);
extern void Bluetooth_SetClassOfDevice(unsigned int uiCOD);
extern void Bluetooth_ScanEnable(void);
extern void Bluetooth_ScanDisable(void);

#endif /*_BLUETOOTH_CM_H_*/
