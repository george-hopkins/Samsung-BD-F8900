/**********************************************************************
* File Name   : Bluetooth_CM.h
* Description : This file define functions and variables for Security Controller.
* Updates			:
***********************************************************************/
#ifndef _BLUETOOTH_SC_H_
#define _BLUETOOTH_SC_H_

#include "csr_bt_profiles.h"


/*****************************************************************************
*
* Macro Definition
*
******************************************************************************/
#define MAX_NUM_OF_SCDB 10
#define PASSKEY_MAX_LEN  10

typedef struct {
	CsrUint16           qid;
//	void 			*recvMsgP;
	
//	CsrBool         pairingFromPeer;

//	CsrBtDeviceAddr	bondingDevAddr; //For Passkey Indication from remote.
	
//	CsrUint8		passkeyLength;
//	CsrUint8		passkey[CSR_BT_PASSKEY_MAX_LEN];

//	CsrUint8 		selectedIndex;
//	CsrUint8		scDbListNum;
//	scDbDeviceRecord_t scDbList[MAX_NUM_OF_SCDB];
	
}ScAppInst;



extern void init_sc(void **gash);
extern void deinit_sc(void **gash);
extern void handleScPrim(ScAppInst  *inst,CsrPrim    *primType);


#endif /*_BLUETOOTH_SC_H_*/
