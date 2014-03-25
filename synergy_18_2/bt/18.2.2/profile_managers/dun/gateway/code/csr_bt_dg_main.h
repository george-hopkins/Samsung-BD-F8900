#ifndef CSR_BT_DG_MAIN_H__
#define CSR_BT_DG_MAIN_H__

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_DG_MODULE
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_types.h"
#include "csr_bt_profiles.h"
#include "rfcomm_prim.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "csr_sched.h"
#include "csr_message_queue.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DG_CLASS_OF_DEVICE           (0x420200)                       /* The COD for the DG - definition in BT assigned numbers */
#define ACTIVE_POWER_MODE            (CONNECTION_MODE)                /* DG always want to be in active mode once connected */
#define DG_PAGE_SCAN_INTERVAL        (0x800)                          /* interval 0x12 - 0x1000, default 0x800 */
#define DG_PAGE_SCAN_WINDOW          (0x100)                          /* window   0x12 - 0x1000, default 0x12 */
#define DG_PORT_SPEED                (PORT_SPEED_UNUSED)              /* the port speed used during port neg */
#define DG_SERVER_ROLE               (CSR_BT_DCE)

/* Dg states */
#define Init_s                        0x00
#define Idle_s                        0x01
#define Activated_s                   0x02
#define Connected_s                   0x03
#define DgNumberOfStates              0x05

typedef CsrUint8                       DgStates_t;

typedef struct
{
    CsrSchedQid                            ctrlHandle;                     /* control application handle */
    CsrSchedQid                            dataHandle;                     /* data application handle */
    DgStates_t                        state;                          /* my state */
    BD_ADDR_T                         bdAddr;                         /* BT address of connected peer */
    CsrUint8                          serverChannel;                  /* Local server channel for server role */
    CsrBtConnId                       btConnId;
    CsrBool                           restoreDgFlag;
    CsrBool                           deactivateFlag;
    CsrUint32                         sdsRecHandle;                   /* record handle for sds */
    CsrBool                           sdsRecordObtain;                /* true if a SDS record is obtained */
    CsrMessageQueueType              *saveQueue;                     /* local message queue for init-state */
    void                              *recvMsgP;                      /* pointer to the received message. NULL if it has been passed on */
    dm_security_level_t               secIncoming;                    /* Incoming security level */
    CsrBool                           registerDataPathPending;        /* need to send CSR_BT_DG_REGISTER_DATA_PATH_HANDLE_CFM when a ctrlHandle is registered */
    CsrUint8                          modemStatus;
    CsrUint8                          breakSignal;
} DgInstanceData_t;

typedef void (*DgStateHandlerType)(DgInstanceData_t* dgInstanceData);

void CsrBtDgFreeDownstreamMessageContents(CsrUint16 eventClass, void * message);
#ifdef __cplusplus
}
#endif

#endif

#endif
