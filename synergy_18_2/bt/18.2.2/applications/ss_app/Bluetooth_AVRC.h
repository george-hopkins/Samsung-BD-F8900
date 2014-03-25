/**********************************************************************
* File Name   : Bluetooth_Hf.h
* Description : This file contains functions and variables for
*                handling BT profiles
*	Updates			:
***********************************************************************/
#ifndef _BLUETOOTH_AVRC_H_
#define _BLUETOOTH_AVRC_H_

#ifdef __cplusplus
extern "C" {
#endif


/*  profile manager include files */
#include "csr_bt_profiles.h"
#include "csr_bt_avrcp_prim.h"
#include "csr_bt_ss_prim.h"




typedef struct
{
	CsrUint16		qid;
//	void 			*recvMsgP;
	CsrUint16            remoteAvrcpVersion;
	CsrUint16            lastUidCounter;
	CsrUint8			 connectionId;
	CsrBtDeviceAddr      remoteAddr;
	CsrUint8             state;
	CsrBool 			 proceedFragmentation;
    CsrBtAvrcpUid        lastFolderUidReceived;
	CsrBtAvrcpUid        lastUidReceived;
} AvrcAppInst;

extern void init_avrc(void **gash);
extern void deinit_avrc(void **gash);
extern void handleAvrcPrim(AvrcAppInst  *instData,CsrPrim    *thePrim);

#ifdef __cplusplus
}
#endif

#endif
