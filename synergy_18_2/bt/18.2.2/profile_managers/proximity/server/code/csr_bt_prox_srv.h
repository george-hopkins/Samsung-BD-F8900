#ifndef CSR_BT_PROX_SRV_H__
#define CSR_BT_PROX_SRV_H__

#include "csr_synergy.h"
/****************************************************************************

   Copyright Cambridge Silicon Radio Limited and its licensors 2011.
   
   All rights reserved. 

REVISION:      $Revision: #3 $
****************************************************************************/


#include "csr_types.h"
#include "csr_message_queue.h"
#include "csr_file.h"
#include "csr_list.h"

#ifdef __cplusplus
extern "C" {
#endif


    typedef enum
{
    CSR_BT_PROX_SRV_ST_LISTEN, /* accept on both */
    CSR_BT_PROX_SRV_ST_LE,     /* connected on LE */
    CSR_BT_PROX_SRV_ST_STOP    /* disconnect all */
} CsrBtProxsState;

typedef struct CsrBtProxSrvClientConfigElementTag 
{
    struct CsrBtProxSrvClientConfigElementTag    *next;              /* must be first */
    struct CsrBtProxSrvClientConfigElementTag    *prev;              /* must be second */
    CsrBtDeviceAddr                               deviceAddr;        /* registered device addr */
    CsrUint8                                     *value;             /* client config value*/
    CsrBool                                       connectionFailed;  /* true if connect on update has been tried without success */
    CsrBool                                       paired;            /* device paired or not */

    } CsrBtProxSrvClientConfigElement;



/* CSR_BT_PROXS instance data */
typedef struct
{
    CsrSchedQid                     appHandle;               /* handle for current app                          */
    CsrMessageQueueType            *saveQueue;
    CsrBool                         restoreFlag;
    CsrBool                         initialized;
    CsrBtProxsState                 state;

    CsrSchedQid                     phandle;                 /* proximity servers own phandle                   */
    CsrBtGattId                     gattId;                  /* gattId that this demo app identifies with       */

    void                           *recvMsgP;
    /*currently connected device */
    CsrBtTypedAddr                  connectedDeviceAddr;
    /* Connection IDs */
    CsrBtConnId                     leBtConnId;
    CsrBtConnId                     connectedBtConnId;
    /*database allocation */
    CsrUint16                       dbStartHandle;
    CsrUint16                       dbEndHandle;
    /*current values in database*/
    CsrUint16                       llValue;
    CsrUint16                       iAlertValue;
    CsrInt8                         txValue;
    CsrUint16                       battValue;
    CsrUint16                       battMask;
    CsrUint8                        serviceRequired;
    /*client configuration list */
    CsrCmnList_t                    clientConfigList;
    /*timer used to poll CM for and transmit txPower to client */
    CsrSchedTid                     txPower_timer;
    /* current eventmask as set in activate */
    CsrBtProxSrvEventMask           eventMask;
} CsrBtProxSrvInstData;

/* helper macros for the client configuration linked list */
#define CSR_BT_PROX_SRV_ADD_CLIENT_CONFIG(_List) \
    (CsrBtProxSrvClientConfigElement *)CsrCmnListElementAddLast(&(_List), \
                                                                 sizeof(CsrBtProxSrvClientConfigElement))
#define CSR_BT_PROX_SRV_FIND_CLIENT_CONFIG_BY_ADDR(_List,_addr) \
    ((CsrBtProxSrvClientConfigElement *)CsrCmnListSearch(&(_List), \
                                                        CsrBtProxSrvFindClientConfigByAddr, \
                                                        (void *)(_addr)))

void CsrBtProxSrvHandleGattPrim(CsrBtProxSrvInstData *proxSrvData);
void CsrBtProxSrvHandleProxSrvPrim(CsrBtProxSrvInstData *proxSrvData);
void CsrBtProxSrvHandleCmPrim(CsrBtProxSrvInstData *proxSrvData);
void CsrBtProxSrvHandleSdPrim(CsrBtProxSrvInstData *proxSrvData);


#ifdef __cplusplus
}
#endif


#endif /* CSR_BT_PROX_SRV__H__ */

