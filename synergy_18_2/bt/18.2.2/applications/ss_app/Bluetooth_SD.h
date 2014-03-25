/**********************************************************************
* File Name   : Bluetooth_SD.h
* Description : This file define functions and variables for Service Discovery.
* Updates			:
***********************************************************************/

#ifndef _BLUETOOTH_SD_H_
#define _BLUETOOTH_SD_H_

#include "csr_types.h"


#define MAX_SEARCH_LIST_NUM	10


#define CSR_BT_GAP_SD_STATE_IDLE                                        0x00
#define CSR_BT_GAP_SD_STATE_SEARCHING                                   0x01
#define CSR_BT_GAP_SD_STATE_CANCEL_SEARCH                               0x02


typedef struct {
	CsrUint16		qid;

//	void 			*recvMsgP;
	CsrUint8 		state;
	CsrUint16                       devicesInSearchList;
	CsrUint16                       devicesInSearchMax;
} SdAppInst;


extern void init_sd(void **gash);
extern void deinit_sd(void **gash);
extern void handleSdPrim(SdAppInst  *inst,CsrPrim    *primType);


#endif /*_BLUETOOTH_SD_H_*/
