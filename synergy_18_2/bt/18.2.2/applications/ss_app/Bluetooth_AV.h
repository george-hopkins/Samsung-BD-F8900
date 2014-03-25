/**********************************************************************
* File Name   : Bluetooth_AV.h
* Description : This file contains functions and variables for
*                handling BT profiles
*	Updates			:
***********************************************************************/
#ifndef _BLUETOOTH_AV_H_
#define _BLUETOOTH_AV_H_

#ifdef __cplusplus
extern "C" {
#endif

//#include "csr_bt_av2.h"

/*  profile manager include files */
#include "csr_bt_profiles.h"
#include "csr_bt_av2_filter.h"


/* General settings */
#define MAX_CONNECTIONS                  (1)
#define MAX_NUM_FILTERS                  (10)
#define MAX_NUM_REMOTE_SEIDS             (4)
#ifdef EXCLUDE_CSR_BT_SS_CODEC
#define MAX_NUM_LOCAL_SEIDS              (1)
#else
#define MAX_NUM_LOCAL_SEIDS              (2)
#endif
#define MEDIA_PAYLOAD_TYPE               (96) /* Any dynamic payload type in the range 96-127 */
#define NUM_AV_CAPABILITIES             9




#define  SBC_CODEC_TYPE                   (2) 
#define  INDEX_LOCAL_SBC_SEID             (0)
#ifndef EXCLUDE_CSR_BT_SS_CODEC
#define  SS_CODEC_TYPE                    (3) 
#define  INDEX_LOCAL_SS_SEID              (1)
#define  SS_VENDOR_ID      				0x75
#endif


typedef struct 
{
	CsrUint16		qid;

//	void 			*recvMsgP;
    CsrBool               avIsActivated;

    /* AV configuration */
    CsrUint8              avConfig;

    /* AV connections */
	CsrUint8			  connectionId;
	CsrUint8      		  state;
    CsrUint8              localSeid[MAX_NUM_LOCAL_SEIDS];
    CsrBool               localSeidInUse[MAX_NUM_LOCAL_SEIDS];
    CsrUint8              avTLabel;
	CsrUint8              streamHandle;
	CsrUint8              localSeidIndex;
	CsrBtConnId 		  btConnId;


	/*remote device*/
    CsrUint8              remoteSeid[MAX_NUM_REMOTE_SEIDS];
    CsrUint8              remoteSeidIndex;
#ifndef EXCLUDE_CSR_BT_SS_CODEC		
    CsrUint8              remoteSeidSBCIndex;
	CsrUint8              remoteSeidMax;
    CsrUint8              remoteSBCConf[10];
    CsrUint8              remoteSBCLen;
#endif
    /* Filter instances and timers */
    av2filter_t          *filters[MAX_NUM_FILTERS];           /* Actual filter chain being used */
    void                 *filter_instance[MAX_NUM_FILTERS];   /* Instance data pointers */
    CsrUint8              filter_count;                       /* Number of filters */
#ifndef EXCLUDE_CSR_BT_SS_CODEC		
    CsrUint8           	  installedCodec;                      /*installed codec*/
#endif	
	
    CsrSchedTid           filter_timer;                       /* Timer for auto-call of filter */
    CsrTime               filter_interval;                    /* Timer interval (constant) */
    CsrTime               filter_delay;                       /* Current delay for timer (dynamic) */
    CsrTime               filter_lasttime;                    /* Last time timer was fired */
    CsrUint32             time_stamp;                         /* Time-stamp for next outgoing packet */
    CsrUint8              mediaPayloadType;                   /* Media-type for next outgoing packet */
//    CsrUint8              ticker;                             /* Eye-candy ticker */

    /* Global filter/PCM configuration */
    CsrBool               reconfiguring;
    CsrUint8              pcm_channels;
    CsrUint16             pcm_freq;
    CsrUint8              pcm_bits;
    CsrUint16             pcm_buffer_size;
    CsrBool               pcm_locked;
    CsrUint16             maxFrameSize;
    
	CsrBtDeviceAddr       remoteAddr;


} Av2AppInst;

//typedef  struct Av2AppInst_t Av2AppInst;

extern void init_av2(void **gash);
extern void deinit_av2(void **gash);
extern void handleAv2Prim(Av2AppInst  *instData, CsrPrim    *thePrim);

#ifdef __cplusplus
}
#endif

#endif
