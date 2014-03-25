/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_sched.h"
#include "csr_pmem.h"
#include "dm_prim.h"
#include "csr_bt_cm_dm_sc_lib.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_sc_prim.h"
#include "csr_bt_sc_main.h"
#include "csr_bt_sd_lib.h"
#include "csr_bt_util.h"
#include "csr_bt_sc_authenticate.h"
#include "csr_bt_sc_utils.h"
#include "csr_bt_sc_bond_unified.h"
#include "csr_bt_sc_bond_legacy.h"
#include "csr_bt_sc_bond_local.h"
#include "csr_bt_sc_bond_remote.h"
#include "csr_bt_sc_encrypt.h"
#include "csr_bt_sc_authenticate.h"
#include "csr_bt_cm_dm_sc_lib.h"
#include "csr_bt_cm_dm_sc_ssp_lib.h"
#include "csr_bt_sc_lib_upstream.h"
#include "csr_bt_sc_ui.h"
#include "csr_bt_sc_authorise.h"
#include "csr_bt_sc_oob.h"
#include "csr_bt_sd_private_prim.h"
#include "csr_bt_sd_private_lib.h"
#include "csr_bt_sc_register.h"

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScCmSmRepairIndHandlerIdleS
*
*  DESCRIPTION
*
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScCmSmRepairIndHandlerIdleS(ScInstanceData_t *scData)
{
    CsrBtCmSmRepairInd *cmPrim;

    cmPrim = (CsrBtCmSmRepairInd *) scData->recvMsgP;

    CsrBtBdAddrCopy(&scData->deviceAddr, &cmPrim->deviceAddr);

    scData->repairData.repairId  = cmPrim->repairId;
    scData->appHandle            = scData->mainAppHandle;
    scData->restoreMsgFlag       = FALSE;
    scData->bondData.initiator   = INITIATOR_REBOND;

    SC_CHANGE_MSGTYPE(&scData->pendingMsgType, MSG_TYPE_REBOND);
    SC_CHANGE_STATE(&scData->state, SC_ST_REBOND);
    SC_CHANGE_STATE(&scData->bondData.scState, SC_ACTIVE);

    if (cmPrim->linkKeyType != HCI_COMBINATION_KEY && CsrBtScEventIsEnabled(scData->eventMask, CSR_BT_SC_EVENT_MASK_REBOND))
    {
        /* Ask user whether to accept rebonding */
        CsrBtScStartUi(scData);
    }
    else
    { /* Auto accept */
        SC_CHANGE_STATE(&scData->bondData.dmState, DM_PENDING);
        SC_CHANGE_STATE(&scData->bondData.appState, APP_IDLE);
        SC_CHANGE_STATE(&scData->bondData.uiState, UI_IDLE);
        CsrBtCmScDmDeleteStoredLinkKeyReq(scData->deviceAddr, DELETE_BDADDR);
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScRebondResHandlerRebondS
*
*  DESCRIPTION
*
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScRebondResHandlerRebondS(ScInstanceData_t *scData)
{
    CsrBtScRebondRes *scPrim;

    scPrim = (CsrBtScRebondRes *) scData->recvMsgP;

    if (CsrBtBdAddrEq(&scPrim->deviceAddr, &scData->deviceAddr) &&
        scData->bondData.appState == APP_RESPOND)
    {
        scData->accept = scPrim->accept;

        if (scPrim->accept)
        {
            SC_CHANGE_STATE(&scData->bondData.dmState, DM_PENDING);
            SC_CHANGE_STATE(&scData->bondData.appState, APP_IDLE);
            SC_CHANGE_STATE(&scData->bondData.uiState, UI_IDLE);

            /* Stop watchdog timer */
            CsrBtScStopBondTimer(scData);
            /* Clear msg type */
            SC_CHANGE_MSGTYPE(&scData->pendingMsgType, MSG_TYPE_NONE);
            CsrBtCmScDmDeleteStoredLinkKeyReq(scPrim->deviceAddr, DELETE_BDADDR);
        }
        else
        {
            if (CSR_BT_SC_IS_LOW_ENERGY(scData) == FALSE)
            {
                CsrBtCmSmSendRepairRes(scData->deviceAddr, FALSE,
                                       scData->repairData.repairId,
                                       scData->addressType);
            }

            scData->authorised = FALSE;
            CsrBtScBondReqCancelHandlerAllowReset(scData, scData->appHandle); /* allow reset */
        }
    }
    else
    {
        CsrBtScStateEventException(CSR_BT_SC_PRIM, VOIDP_TO_MSG_TYPE(scData), scData->state);
    }
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScCmSmRepairIndHandler
*
*  DESCRIPTION
*      Main dispatcher for the CSR_BT_CM_SM_REPAIR_IND
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScCmSmRepairIndHandler(ScInstanceData_t *scData)
{
    CsrBtCmSmRepairInd *cmPrim;

    cmPrim = (CsrBtCmSmRepairInd *) scData->recvMsgP;

    switch(scData->state)
    {
        case SC_ST_IDLE:
            CsrBtScCmSmRepairIndHandlerIdleS(scData);
            break;

        case SC_ST_LOCAL_POST_LISBON_BONDING:
            /* fallthrough */
       case SC_ST_REMOTE_BONDING:
            /* fallthrough */
        case SC_ST_AUTHORISATION:
            /* fallthrough */
#ifdef CSR_BT_INSTALL_SC_AUTHENTICATE
        case SC_ST_AUTHENTICATE:
            /* fallthrough */
#endif
#ifdef CSR_BT_INSTALL_SC_ENCRYPTION
        case SC_ST_ENCRYPT:
            /* fallthrough */
#endif
        case SC_ST_PENDING:
            /* fallthrough */
        case SC_ST_REBOND:
            CsrBtCmSmSendRepairRes(cmPrim->deviceAddr,
                                   FALSE, cmPrim->repairId,
                                   cmPrim->addressType);
            break;

        default:
            CsrBtScStateEventException(CSR_BT_CM_PRIM, VOIDP_TO_MSG_TYPE(scData), scData->state);
            break;
    }
}

void CsrBtScDmHciDeleteStoredLinkKeyCompleteRebondS(ScInstanceData_t *scData)
{
    DM_HCI_DELETE_STORED_LINK_KEY_CFM_T *dmPrim;

    dmPrim = (DM_HCI_DELETE_STORED_LINK_KEY_CFM_T *)scData->recvMsgP;

    CsrBtScDbRemove(&scData->deviceAddr);

    if (dmPrim->status == HCI_SUCCESS)
    {
        CsrBtCmScDmRemoveDeviceReq(scData->deviceAddr,
                                   CSR_BT_SC_ADDR_TYPE(scData));
    }
    else
    {
        CsrBtCmSmSendRepairResNoAccept(scData);
        scData->authorised = FALSE;
        scData->hciStatus  = dmPrim->status;
        CsrBtScBondReqCancelHandlerAllowReset(scData, scData->appHandle); /* allow reset */
    }
}

void CsrBtScDmRemoveDeviceCfmRebondS(ScInstanceData_t *scData)
{
    CsrBtCmSmSendRepairRes(scData->deviceAddr, TRUE,
                           scData->repairData.repairId,
                           scData->addressType);
    CsrBtScResetInstData(scData);
}
