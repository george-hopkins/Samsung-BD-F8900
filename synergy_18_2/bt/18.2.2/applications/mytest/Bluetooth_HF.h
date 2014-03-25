/**********************************************************************
* File Name   : Bluetooth_Hf.h
* Description : This file contains functions and variables for
*                handling BT profiles
*	author 			:	Kim Jin Cheol	(SW Platform., HYUNDAI MOBIS)
*	Updates			:
***********************************************************************/
#ifndef _BLUETOOTH_HF_H_
#define _BLUETOOTH_HF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "csr_bt_hf.h"
#include "csr_bt_hf_prim.h"

/*  profile manager include files */
#include "csr_bt_profiles.h"


#define SCO_PACKET_TYPE             (0)
#define SCO_TX_BANDWIDTH            (0)
#define SCO_RX_BANDWIDTH            (0)
#define SCO_MAX_LATENCY             (CSR_BT_SCO_DEFAULT_1P1_MAX_LATENCY)
#define SCO_VOICE_SETTINGS          (0)
#define SCO_RETRANSMISSION_EFFORT   (HCI_ESCO_NO_RETX)

#define PCM_SLOT                             (CSR_BT_PCM_DONT_CARE)
#define PCM_SLOT_REALLOCATE                  (TRUE)

typedef struct
{
	CsrUint16		qid;
	void 			*recvMsgP;
	CsrBtHfConnectionId 		connectionId;

	
} HfAppInst;

extern void init_hf(void **gash);
extern void deinit_hf(void **gash);
extern void handleHfPrim(HfAppInst  *inst);

#ifdef __cplusplus
}
#endif

#endif
