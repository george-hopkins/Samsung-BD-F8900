#ifndef CSR_BT_HCRP_HANDLER_H__
#define CSR_BT_HCRP_HANDLER_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_message_queue.h"

#ifdef __cplusplus
extern "C" {
#endif

#define HCRP_IDLE_STATE                     (0)
#define HCRP_INITIALISING_STATE             (1)
#define HCRP_READY_STATE                    (2)
#define HCRP_CONNECTING_STATE               (3)
#define HCRP_CONNECTED_STATE                (4)
#define HCRP_DEACTIVATING_STATE             (5)
#define HCRP_NUMBER_OF_STATES               (6)

#define HCRP_CNTL_PSM                       (0x2001)                                /* as defined in hcrp spec. */
#define HCRP_DATA_PSM                       (0x2003)                                /* as defined in hcrp spec. */

typedef struct
{
    CsrUint8                 state;
    CsrUint16                event;
    CsrSchedQid              ctrlHandle;
    CsrSchedQid              dataHandle;
    void*                    pRecvMsg;
    void*                    pSaveMsg;
    CsrMessageQueueType*     pSaveQueue;
    CsrUint8                 savePrimTypeInd;
    CsrUint8                 savePrimTypeRes;
    CsrUint32                hServiceRec;
    CsrBool                  sdsRegistered;
    CsrBool                  cntlConnected;
    CsrBool                  dataConnected;
    CsrBtConnId              cntlCid;
    CsrBtConnId              dataCid;
    CsrUint16                payLoadLength;
    CsrUint8*                payLoad;
    CsrUint32                totalCredit;
    CsrUtf8String            *ieee;
    dm_security_level_t      secIncoming; /* Incoming security level */
    CsrBtDeviceAddr          bdAddr;
    CsrBool                  restoreHcrpFlag;
} hcrpInstanceData_t;

typedef void (*hcrpStateHandler)(hcrpInstanceData_t* hcrpInstanceData);

#ifdef __cplusplus
}
#endif

#endif    /* CSR_BT_HCRP_HANDLER_H__ */
