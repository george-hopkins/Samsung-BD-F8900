/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_pmem.h"
#include "csr_sched.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_prim.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_cm_dm_sc_lib.h"
#include "csr_bt_cm_dm_sc_ssp_lib.h"
#include "csr_bt_sc_prim.h"
#include "csr_bt_sc_main.h"
#include "csr_bt_util.h"
#include "csr_bt_sc_lib_upstream.h"
#include "csr_bt_sc_utils.h"
#ifdef CSR_BT_LE_ENABLE
#include "csr_bt_sc_le.h"
#endif

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScActivateReqAnyS
*
*  DESCRIPTION
*      Set the default apphandle which will receive all unsolicited
*      pairing requests.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScActivateReqAnyS(ScInstanceData_t *scData)
{
    CsrBtScActivateReq   *prim;

    prim = (CsrBtScActivateReq *) scData->recvMsgP;
    scData->mainAppHandle = prim->phandle;
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScCmGetSecurityConfIndNullS
*
*  DESCRIPTION
*      Determine whether the controller is legacy or Lisbon.  This
*      completes step 1 of the SC initialisation. Next step is to set the 
*      SM_SM security levels and config etc.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScCmGetSecurityConfIndNullS(ScInstanceData_t *scData)
{
    CsrBtCmGetSecurityConfInd *cmPrim = (CsrBtCmGetSecurityConfInd *) scData->recvMsgP;
    CsrUint16 *er;
    CsrUint16 *ir;
    CsrUint16 div;
    CsrUint32 sign;
    CsrUint16 options;

    if (scData->securityMode == SEC_MODE4_SSP && cmPrim->lmpVersion < CSR_BT_BLUETOOTH_VERSION_2P1)
    {
        /* Fallback to sec. mode 2 if we don't support SM4 */
        scData->securityMode = SEC_MODE2_SERVICE;
    }

    options = DM_SM_INIT_ALL_BREDR;
    er = NULL;
    ir = NULL;
    div = 0;
    sign = 0;

#ifdef CSR_BT_LE_ENABLE
    /* Read local info from scdb */
    {
        CsrBtScDbDeviceRecord db;
        CsrBtDeviceAddr addr;

        CsrMemSet(&db, 0, sizeof(CsrBtScDbDeviceRecord));
        CsrMemSet(&addr, 0xFF, sizeof(CsrBtDeviceAddr));

        if (CsrBtScDbRead(&addr,&db))
        {
            /* ER is stored in central LTK */
            er = CsrPmemAlloc(sizeof(CsrUint16)*8);
            CsrMemCpy(er,
                      db.leKeyEncCentral.ltk,
                      sizeof(CsrUint16)*8);

            /* IR is stored in IRK */
            ir = CsrPmemAlloc(sizeof(CsrUint16)*8);
            CsrMemCpy(ir,
                      db.leKeyId.irk,
                      sizeof(CsrUint16)*8);

            div = db.leKeyDiv;
            sign = db.leKeySignCounter;

            options |= DM_SM_INIT_ALL_LE;
        }
    }
#endif

    /* Set default security level, mode and other bits and bobs */
    CsrBtCmGetSecurityConfResSend(options,
                                  scData->securityMode,
                                  0, /* default security level, 0 because of SDP */
                                  scData->sspData.config,
                                  scData->sspData.writeAuthEnable,
                                  scData->encryptionMode,
                                  er, ir, div, sign);
}

/*----------------------------------------------------------------------------*
*  NAME
*      ScCmReadLocalBdAddrCfmHciNullS
*
*  DESCRIPTION
*      We got the local BD address. Last step is to set the CM event mask
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScCmReadLocalBdAddrCfmNullS(ScInstanceData_t *scData)
{
    CsrBtCmReadLocalBdAddrCfm *cmPrim;
    CsrUint32 mask;

    cmPrim = (CsrBtCmReadLocalBdAddrCfm *) scData->recvMsgP;
    scData->localAddr = cmPrim->deviceAddr;

    mask = CSR_BT_CM_EVENT_MASK_SUBSCRIBE_ACL_CONNECTION;
#ifdef CSR_BT_LE_ENABLE
    mask |= CSR_BT_CM_EVENT_MASK_SUBSCRIBE_LOW_ENERGY;
#endif

    CsrBtCmSetEventMaskReqSend(CSR_BT_SC_IFACEQUEUE,
                               mask,
                               CSR_BT_CM_EVENT_MASK_COND_ALL);
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScDmSmInitCfmNullS. Next step is to read the local BD address.
*
*  DESCRIPTION
*
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScDmSmInitCfmNullS(ScInstanceData_t *scData)
{
    CsrBtScStateEventExceptionOn(((DM_SM_INIT_CFM_T*)scData->recvMsgP)->status != 0,
                                 DM_PRIM,
                                 scData->pendingMsgType,
                                 scData->state);

    /* Low Energy requires a few persistent local keys. Write these
     * now */
#ifdef CSR_BT_LE_ENABLE
    {
        DM_SM_INIT_CFM_T *dmPrim;

        dmPrim = (DM_SM_INIT_CFM_T*)scData->recvMsgP;

        CsrBtScLeUpdateLocalDbKeys(dmPrim);
    }
#endif

    /* Always require unauthenticated link key on RFCOMM PSM */
    CsrBtCmScDmRegisterReq(SEC_PROTOCOL_L2CAP, RFCOMM_PSM, TRUE, SECL4_IN_SSP|SECL4_OUT_SSP, 0);
    CsrBtCmReadLocalBdAddrReqSend(CSR_BT_SC_IFACEQUEUE);
}

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScCmSetEventMaskCfmNullS
*
*  DESCRIPTION
*
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScCmSetEventMaskCfmNullS(ScInstanceData_t *scData)
{
    /* We do not care about the CFM */
    CsrBtScResetInstData(scData);
}

#ifdef CSR_BT_INSTALL_SC_SET_IO_CAPABILITY
/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScSetIoCapabilityReqHandler
*
*  DESCRIPTION
*
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScSetIoCapabilityReqHandler(ScInstanceData_t *scData)
{
    CsrBtScSetIoCapabilityReq  *prim;

    prim = (CsrBtScSetIoCapabilityReq *) scData->recvMsgP;

    if (prim->ioCapability <= 
#ifdef CSR_BT_LE_ENABLE
        HCI_IO_CAP_KEYBOARD_DISPLAY
#else
        HCI_IO_CAP_NO_INPUT_NO_OUTPUT
#endif
        )
    {
        scData->bondData.localIoCaps = prim->ioCapability;
    }
}
#endif /* CSR_BT_INSTALL_SC_SET_IO_CAPABILITY */

#ifdef CSR_BT_INSTALL_SC_SET_AUTH_REQUIREMENTS
/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScSetAuthRequirementsReqHandler
*
*  DESCRIPTION
*
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScSetAuthRequirementsReqHandler(ScInstanceData_t *scData)
{
    CsrBtScSetAuthRequirementsReq  *prim;

    prim = (CsrBtScSetAuthRequirementsReq *) scData->recvMsgP;

    if (prim->authRequirements <= HCI_MITM_PROTECTION_MAX)
    {
        scData->bondData.localAuth = prim->authRequirements;
    }
#ifdef CSR_BT_LE_ENABLE
    if(prim->leAuthRequirements != CSR_BT_SC_LE_AUTH_IGNORE)
    {
        scData->bondData.leDefaultLocalAuth = prim->leAuthRequirements;
        scData->bondData.leLocalAuth = prim->leAuthRequirements;
    }
#endif
}
#endif /* CSR_BT_INSTALL_SC_SET_AUTH_REQUIREMENTS */

#ifdef CSR_BT_INSTALL_SC_SET_EVENT_MASK
/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScSetEventMaskAnyS
*
*  DESCRIPTION
*      Set the default eventmask.
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
void CsrBtScSetEventMaskAnyS(ScInstanceData_t *scData)
{
    CsrBtScSetEventMaskReq   *prim;

    prim = (CsrBtScSetEventMaskReq *) scData->recvMsgP;
    scData->eventMask = prim->eventMask;
}
#endif /* CSR_BT_INSTALL_SC_SET_EVENT_MASK */

/*----------------------------------------------------------------------------*
*  NAME
*      CsrBtScDebugModeReqIdleS
*
*  DESCRIPTION
*      Enter SSP debug mode
*
*  RETURNS/MODIFIES
*
*----------------------------------------------------------------------------*/
#ifdef CSR_BT_INSTALL_SC_DEBUG_MODE
void CsrBtScDebugModeReqIdleS(ScInstanceData_t *scData)
{
    CsrBtScDebugModeReq *scPrim;

    scPrim = (CsrBtScDebugModeReq *) scData->recvMsgP;
    scData->appHandle      = scPrim->phandle;
    scData->restoreMsgFlag = FALSE;

    if(scPrim->enable)
    {
        scData->sspData.config |= DM_SM_SEC_MODE_CONFIG_DEBUG_MODE;
    }
    else
    {
        scData->sspData.config -= (CsrUint16)(scData->sspData.config & DM_SM_SEC_MODE_CONFIG_DEBUG_MODE);
    }

    SC_CHANGE_STATE(&scData->state, SC_ST_PENDING);
    SC_CHANGE_STATE(&scData->dmSmInitState, SM_INIT_ST_DEBUG);
    CsrBtCmScDmSecModeConfigReq(scData->sspData.writeAuthEnable,
                                scData->sspData.config);
}
#endif
static void csrBtScCancelAcceptBondIndSend(CsrSchedQid phandle, CsrBtResultCode resultCode)
{
    CsrBtScCancelAcceptBondInd *prim = (CsrBtScCancelAcceptBondInd *)
                                        CsrPmemAlloc(sizeof(CsrBtScCancelAcceptBondInd));

    prim->type           = CSR_BT_SC_CANCEL_ACCEPT_BOND_IND;
    prim->resultCode     = resultCode;
    prim->resultSupplier = CSR_BT_SUPPLIER_SC;
    CsrBtScMessagePut(phandle,prim);
}

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
void CsrBtScCancelAcceptBondIndHandler(ScInstanceData_t *scData)
{
    csrBtScCancelAcceptBondIndSend(scData->bondPhandle, scData->cancelBondResult);
    /* Remember to reset the "cancelBondResult" field */
    scData->cancelBondResult = CSR_BT_RESULT_CODE_SC_SUCCESS;
}

void csrBtAcceptBondPeriodTimeout(CsrUint16 mi, void *mv)
{
    ScInstanceData_t *scData = (ScInstanceData_t *)mv;

    scData->bondTime   = 0;
    scData->cancelBondResult = CSR_BT_RESULT_CODE_SC_BONDING_ACCEPT_TIMEOUT;
    CsrBtCml2caCancelConnectAcceptReqSend(CSR_BT_SC_IFACEQUEUE,scData->localPsm);
}
#endif 

void CsrBtScAcceptBondReqHandler(ScInstanceData_t *scData)
{
    CsrBtScAcceptBondReq *prim = (CsrBtScAcceptBondReq *)scData->recvMsgP;

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
    scData->bondPhandle = prim->phandle;

    if (scData->acceptingBond)
    {/* Already discoverable */
        if (scData->bondTime != 0)
        {/* Stop timer and res-tart it if needed */
            CsrSchedTimerCancel(scData->bondTime, NULL, NULL);
            scData->bondTime = 0;
        }
    }
    else
    {/* Not discoverable yet: start by registering the fake service record if not already done */
        if (scData->localPsm != 0)
        {
            scData->acceptingBond = TRUE;

            CsrBtCml2caConnectAcceptReqSend(CSR_BT_SC_IFACEQUEUE,
                                       scData->localPsm,
                                       0,
                                       scData->securityMode,
                                       40,
                                       0);
        }
        else
        {/* First register.... (and remember to store the timer value) */
            CsrBtCml2caRegisterReqSend(CSR_BT_SC_IFACEQUEUE,CSR_BT_ASSIGN_DYNAMIC_PSM);
            scData->bondTime = prim->time;
        }
    }

    if (scData->acceptingBond)
    {
        if (prim->time != 0)
        {/* (re-)start timer*/
            scData->bondTime = CsrSchedTimerSet(prim->time, csrBtAcceptBondPeriodTimeout, (CsrUint16)prim->phandle, (void *)scData);
        }
        else
        {/* 'Forever' */
            scData->bondTime = 0;
        }
    }
    else
    {/* Make sure to mark that we are processing the request already so new requests from the
        app do not start a dynamic PCM registration all over again */
        scData->acceptingBond = TRUE;
    }
#else
        csrBtScCancelAcceptBondIndSend(prim->phandle, CSR_BT_RESULT_CODE_SC_INTERNAL_ERROR);
#endif /* #ifndef EXCLUDE_CSR_BT_L2CA_MODULE */
}


void CsrBtScCancelAcceptBondReqHandler(ScInstanceData_t *scData)
{
#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
    if (scData->bondTime != 0)
    {
        CsrSchedTimerCancel(scData->bondTime, NULL, NULL);
        scData->bondTime = 0;
    }
    if (scData->acceptingBond)
    {
        CsrBtCml2caCancelConnectAcceptReqSend(CSR_BT_SC_IFACEQUEUE,scData->localPsm);
    }
    else
    {
        CsrBtScCancelAcceptBondIndHandler(scData);
    }
#else
    CsrBtScCancelAcceptBondReq *prim = (CsrBtScCancelAcceptBondReq *)scData->recvMsgP;
    csrBtScCancelAcceptBondIndSend(prim->phandle, CSR_BT_RESULT_CODE_SC_SUCCESS);
#endif /* #ifndef EXCLUDE_CSR_BT_L2CA_MODULE */
}
