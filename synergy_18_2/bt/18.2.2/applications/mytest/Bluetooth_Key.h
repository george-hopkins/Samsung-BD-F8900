/**********************************************************************
* File Name   : Bluetooth_key.h
* Description : This file contains functions and variables for
*                handling BT profiles
*	author 			:	Kim Jin Cheol	(SW Platform., HYUNDAI MOBIS)
*	Updates			:
***********************************************************************/
#ifndef _BLUETOOTH_KEY_H_
#define _BLUETOOTH_KEY_H_

#ifdef __cplusplus
extern "C" {
#endif


/*  profile manager include files */
#include "csr_bt_profiles.h"


#define IDLE_STATUS             0x0100
#define GAP_STATUS              0x0200
#define AV_SNK_STATUS           0x0300
#define HF_STATUS               0x0400
#define SPP_STATUS              0x0500
#define PBAP_STATUS             0x0600
/*
typedef enum APP_STATUS_t
{
    IDLE_IDLE       = IDLE_STATUS,
   
    END_OF_IDLE_STATUS,

    AV_SNK_IDLE			= AV_SNK_STATUS,
    AV_SNK_CONNECTED,
    AV_SNK_OPENED,
    AV_SNK_STARTED,
    END_OF_AV_SRC_STATUS,

    HF_IDLE				= HF_STATUS,
    HF_CONNECTED,
    END_OF_HF_STATUS,

    SPP_IDLE				= SPP_STATUS,
    SPP_CONNECTED,
    SPP_SENDING,
    SPP_RECEIVING,
    END_OF_SPP_STATUS,

    PBAP_IDLE				= PBAP_STATUS,
    PBAP_CONNECTED,
    END_OF_PBAP_STATUS
} APP_STATUS;


#define NUM_OF_IDLE   (END_OF_IDLE_STATUS-IDLE_IDLE)
#define NUM_OF_AV_SRC   (END_OF_AV_SRC_STATUS-AV_SNK_STATUS)
#define NUM_OF_HF   (END_OF_HF_STATUS-HF_STATUS)
#define NUM_OF_SPP   (END_OF_SPP_STATUS-SPP_STATUS)
#define NUM_OF_IDLE   (END_OF_PBAP_STATUS-PBAP_STATUS)
*/

typedef struct
{
    CsrUint16           qid;
    void                *recvMsgP;
    CsrUint16           Keystate;
} KeyAppInst;

extern void init_key(void **gash);
extern void deinit_key(void **gash);
extern void handlekeyPrim(void  *inst ,CsrBtKeyPress_t *key);

#ifdef __cplusplus
}
#endif

#endif
