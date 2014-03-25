/**********************************************************************
* File Name   : Bluetooth_HF.h
* Description : This file contains functions and variables for
*                handling BT profiles
*	author 			:	Kim Jin Cheol	(SW Platform., HYUNDAI MOBIS)
*	Updates			:
***********************************************************************/
#ifndef _BLUETOOTH_PBAC_H_
#define _BLUETOOTH_PBAC_H_

#include "csr_bt_profiles.h"


/*****************************************************************************
*
* Macro Definition
*
******************************************************************************/


#define PAC_MAX_OBEX_PACKET_SIZE        (0x1f00)


typedef struct {
	CsrUint16		qid;
	void 			*recvMsgP;
	

	
}PbacAppInst;



extern void init_pbac(void **gash);
extern void deinit_pbac(void **gash);
extern void handlePbacPrim(PbacAppInst  *inst);
extern void Pac_connect(CsrBtDeviceAddr   theDestination, CsrBool theAuthtorize, CsrUint8 *password);



#endif /*_BLUETOOTH_PBAC_H_*/
