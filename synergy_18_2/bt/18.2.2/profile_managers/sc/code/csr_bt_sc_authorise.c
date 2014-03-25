/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_sched.h"
#include "csr_pmem.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "csr_bt_sc_main.h"
#include "dm_prim.h"
#include "csr_bt_sd_prim.h"
#include "csr_bt_cm_dm_sc_lib.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_sc_prim.h"
#include "csr_bt_sd_lib.h"
#include "csr_bt_sc_lib_upstream.h"
#include "csr_bt_sc_utils.h"
#include "csr_bt_sc_ui.h"
#include "csr_bt_util.h"

/*----------------------------------------------------------------------------*
*  NAME
*      scCancelQueuedAuthoriseReq
*
*  DESCRIPTION
*       Cancel an authorisation request from the queue.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
CsrBool CsrBtScCancelQueuedAuthoriseReq(ScInstanceData_t *scData)
{
    CsrUint16             msgType;
    CsrBtScPrim            *msg;
    CsrBool               msgCancelled  = FALSE;
    CsrMessageQueueType *tempQueue = NULL;

    while(CsrMessageQueuePop(&scData->saveQueue, &msgType, (void *) &msg))
    {
        if (!msgCancelled && msgType == DM_PRIM && *msg == DM_SM_AUTHORISE_IND)
        {
            DM_SM_AUTHORISE_IND_T * prim = (DM_SM_AUTHORISE_IND_T *) msg;

            if (CsrBtBdAddrEq(&prim->cs.connection.addrt.addr, &scData->deviceAddr))
            {
                msgCancelled = TRUE;
                CsrBtCmScDmAuthoriseNegRes(prim->cs.connection.addrt.addr,
                                           prim->cs.incoming,
                                           prim->cs.connection.service.channel,
                                           prim->cs.connection.service.protocol_id,
                                           prim->cs.connection.addrt.type);
            }
            else
            {
                CsrMessageQueuePush(&tempQueue, msgType, msg);
            }
        }
        else
        {
            CsrMessageQueuePush(&tempQueue, msgType, msg);
        }
    }
    scData->saveQueue = tempQueue;
    return (msgCancelled);
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScDmSmAuthoriseIndIdleS
*
*  DESCRIPTION
*       A remote device has requested access to one of our services which
*       require authorisation. Ask the app to grant it if it hasn't already
*       done so.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScDmSmAuthoriseIndIdleS(ScInstanceData_t *scData)
{
    DM_SM_AUTHORISE_IND_T    *dmPrim;
    CsrBtScDbDeviceRecord record;

    dmPrim = (DM_SM_AUTHORISE_IND_T *) scData->recvMsgP;

    if (CsrBtScDbRead(&dmPrim->cs.connection.addrt.addr, &record) &&
        record.authorised == TRUE)
    {
        CsrBtCmScDmAuthoriseRes(dmPrim->cs.connection.addrt.addr,
                                dmPrim->cs.incoming,
                                DM_SM_AUTHORISE_ACCEPT_ONCE,
                                dmPrim->cs.connection.service.channel,
                                dmPrim->cs.connection.service.protocol_id,
                                dmPrim->cs.connection.addrt.type);
    }
    else
    {
        CsrBtBdAddrCopy(&scData->deviceAddr, &dmPrim->cs.connection.addrt.addr);
        scData->addressType = dmPrim->cs.connection.addrt.type;
        scData->channel           = dmPrim->cs.connection.service.channel;
        scData->protocolId        = dmPrim->cs.connection.service.protocol_id;
        scData->incoming          = dmPrim->cs.incoming;
        scData->restoreMsgFlag    = FALSE;
        SC_CHANGE_STATE(&scData->state, SC_ST_AUTHORISATION);
        SC_CHANGE_STATE(&scData->bondData.scState, SC_ACTIVE);
        SC_CHANGE_MSGTYPE(&scData->pendingMsgType, MSG_TYPE_AUTHORISE);

        CsrBtScStartUi(scData);
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScSetTrustLevelReqAnyS
*
*  DESCRIPTION
*       The application request to change the  trust level of a device, 
*       e.g. trusted or untrusted. Trusted devices are automatically granted 
*       access by the Security Manager and untrusted devices result in a 
*       CSR_BT_SC_ AUTHORISE_IND.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
#ifdef CSR_BT_INSTALL_SC_TRUST_LEVEL
void CsrBtScSetTrustLevelReqAnyS(ScInstanceData_t *scData)
{
    CsrBtScDbDeviceRecord deviceRecord;
    CsrBtScSetTrustLevelReq *scPrim;

    scPrim = (CsrBtScSetTrustLevelReq *) scData->recvMsgP;
    CsrMemSet(&deviceRecord, 0, sizeof(CsrBtScDbDeviceRecord));

    if (CsrBtScDbRead(&scPrim->deviceAddr, &deviceRecord))
    {
        if (deviceRecord.authorised == scPrim->authorised)
        {
            ; /* No need to change it */
        }
        else
        {
            deviceRecord.authorised = scPrim->authorised;
            CsrBtScDbWrite(&deviceRecord.deviceAddr, &deviceRecord);
            CsrBtScUpdateTrustLevelIndSend(scPrim->deviceAddr,
                                           scPrim->authorised,
                                           scPrim->addressType);
        }
    }
    else
    {
        /* Ignore the device. It is not on the trusted device list */
        CSR_LOG_TEXT_WARNING((LOG_TEXT_TASK_ID, LOG_TEXT_SO_SCDB, "IGNORE %04X:%02X:%06X", deviceRecord.deviceAddr.nap, deviceRecord.deviceAddr.uap, deviceRecord.deviceAddr.lap));
    }

}
#endif
/*----------------------------------------------------------------------------*
*  NAME
*      ScAuthoriseResBondingS
*
*  DESCRIPTION
*       A remote device has requested access to one of our services which
*       require authorisation. Ask the app to grant it.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScAuthoriseResAuthoriseS(ScInstanceData_t *scData)
{
    CsrBtScAuthoriseRes *scPrim;

    scPrim = (CsrBtScAuthoriseRes *)scData->recvMsgP;

    if (CsrBtBdAddrEq(&scData->deviceAddr, &scPrim->deviceAddr) &&
        scData->bondData.appState == APP_RESPOND)
    {
        CsrBtCmScDmAuthoriseResSimpleInclInstReset(scData, (CsrUint16) (scPrim->authorised
                                             ? DM_SM_AUTHORISE_ACCEPT_ONCE
                                             : DM_SM_AUTHORISE_REJECT_ONCE));
    }
    else
    {
        CsrBtScStateEventException(CSR_BT_SC_PRIM, scPrim->type, scData->state);
    }
}

