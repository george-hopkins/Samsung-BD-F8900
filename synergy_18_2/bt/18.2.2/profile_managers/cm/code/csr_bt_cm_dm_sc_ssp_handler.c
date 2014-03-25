/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_cm_main.h"
#include "dmlib.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_rfc.h"
#include "csr_bt_sc_prim.h"
#include "csr_bt_cm_l2cap.h"
#include "csr_bt_cm_bnep.h"
#include "csr_bt_cm_dm_sc_ssp_handler.h"
#include "csr_bt_cm_events_handler.h"

/* Relay security request to SM/DM_SM */
#ifdef CSR_BT_LE_ENABLE
void CsrBtCmSmLeSecurityReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmSmLeSecurityReq *cmPrim;
    cmPrim = (CsrBtCmSmLeSecurityReq*)cmData->recvMsgP;
    dm_sm_security_req(CSR_BT_CM_IFACEQUEUE,
                       &cmPrim->addr,
                       cmPrim->l2caConFlags,
                       cmPrim->context,
                       cmPrim->securityRequirements,
                       NULL);
}
#endif

void CsrBtCmSmBondingReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmSmBondingReq    *cmPrim;
    CsrUint16 clockOffset = 0;
    page_scan_mode_t pageScanMode= 0;
    page_scan_rep_mode_t pageScanRepMode = 0;
    TYPED_BD_ADDR_T ad;

    cmPrim                  = (CsrBtCmSmBondingReq *) cmData->recvMsgP;
    cmData->dmVar.appHandle = cmData->scHandle;
    if(isCacheParamsKnown(cmData, cmPrim->deviceAddr, &clockOffset, &pageScanMode, &pageScanRepMode))
    {
        CSR_BT_CM_STATE_CHANGE(cmData->dmVar.state, scConnectInit_s);

        CsrBtCmDmWriteCacheParamsDirect(cmData,
                                   cmPrim->deviceAddr,
                                   clockOffset,
                                   pageScanMode,
                                   pageScanRepMode,
                                   SC_PLAYER,
                                   CSR_BT_CM_SM_BONDING_REQ);
    }
    else
    {
        CSR_BT_CM_STATE_CHANGE(cmData->dmVar.state, scConnect_s);
        
        ad.addr = cmPrim->deviceAddr;
        ad.type = CSR_BT_ADDR_PUBLIC;
        dm_sm_bonding_req(&ad, NULL);
        CsrBtCmDmLocalQueueHandler();
    }
}

void CsrBtCmDmBondingCfm(cmInstanceData_t *cmData,
                         BD_ADDR_T *p_bd_addr, CsrUint8 status)
{
    DM_SM_BONDING_CFM_T *p_prim;
    p_prim = CsrPmemAlloc(sizeof(DM_SM_BONDING_CFM_T));
    p_prim->type = DM_SM_BONDING_CFM;
    p_prim->phandle = CSR_BT_CM_IFACEQUEUE;
    p_prim->status = status;
    p_prim->flags = 0;

    bd_addr_copy(&p_prim->addrt.addr, p_bd_addr);
    p_prim->addrt.type = CSR_BT_ADDR_PUBLIC;

    CsrSchedMessagePut(cmData->scHandle, DM_PRIM, p_prim);
}

void CsrBtCmDmSmBondingScStateHandler(cmInstanceData_t * cmData)
{
    DM_SM_BONDING_CFM_T * prim;

    prim = (DM_SM_BONDING_CFM_T *) cmData->recvMsgP;

    if (prim != NULL &&
        bd_addr_eq(&cmData->dmVar.cacheTargetDev, &prim->addrt.addr) &&
        cmData->dmVar.state == scConnect_s)
    {
        CSR_BT_CM_STATE_CHANGE(cmData->dmVar.state, scNull_s);
        cmData->dmVar.cancelConnect = FALSE;
    }
}

void CsrBtCmSmBondingCancelReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmSmBondingCancelReq    *prim;

    prim = (CsrBtCmSmBondingCancelReq *) cmData->recvMsgP;
    if (bd_addr_eq(&cmData->dmVar.cacheTargetDev, &prim->deviceAddr) &&
        cmData->dmVar.state != scNull_s)
    {
        TYPED_BD_ADDR_T ad;
        ad.addr = prim->deviceAddr;
        ad.type = prim->addressType;
        dm_sm_bonding_cancel_req(&ad,
                                 prim->force,
                                 NULL);
        cmData->dmVar.cancelConnect = TRUE;
    }
    else
    {
        /* ACL already up or unknown */
        ;
    }
}


#ifdef CSR_BT_INSTALL_CM_SC_MODE_CONFIG
void CsrBtCmSmSecModeConfigReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmSmSecModeConfigReq    *prim;

    prim = (CsrBtCmSmSecModeConfigReq *) cmData->recvMsgP;

    /* Only used in testing */
    dm_sm_init_req(DM_SM_INIT_WRITE_AUTH_ENABLE | DM_SM_INIT_CONFIG, /* options */
                   0, /* mode (not specified) */
                   0, /* security_level_default (not specified) */
                   prim->config,
                   prim->writeAuthEnable,
                   0, /* mode3_enc (not specified) */
                   NULL, /* sm_key_state */
                   0, /* sm_div_state*/                  
                   NULL);
}
#endif

#if CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1

void CsrBtCmSmIoCapabilityRequestResHandler(cmInstanceData_t *cmData)
{
    CsrBtCmSmIoCapabilityRequestRes *prim;
    TYPED_BD_ADDR_T ad;

    prim = (CsrBtCmSmIoCapabilityRequestRes *) cmData->recvMsgP;
    ad.addr = prim->deviceAddr;
    ad.type = prim->addressType;
    dm_sm_io_capability_request_rsp(&ad,
                                    prim->ioCapability,
                                    prim->authenticationRequirements,
                                    prim->oobDataPresent,
                                    prim->oobHashC,
                                    prim->oobRandR,
                                    prim->keyDistribution,
                                    NULL);
}

void CsrBtCmSmIoCapabilityRequestNegResHandler(cmInstanceData_t *cmData)
{
    CsrBtCmSmIoCapabilityRequestNegRes    *prim;
    TYPED_BD_ADDR_T ad;

    prim = (CsrBtCmSmIoCapabilityRequestNegRes *) cmData->recvMsgP;
    ad.addr = prim->deviceAddr;
    ad.type = prim->addressType;
    dm_sm_io_capability_request_neg_rsp(&ad,
                                        prim->reason,
                                        NULL);
}
#ifdef CSR_BT_INSTALL_CM_OOB
void CsrBtCmSmReadLocalOobDataReqHandler(cmInstanceData_t *cmData)
{
    dm_sm_read_local_oob_data_req(NULL);
}
#endif
void CsrBtCmSmSendKeypressNotificationReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmSmSendKeypressNotificationReq    *prim;

    prim = (CsrBtCmSmSendKeypressNotificationReq *) cmData->recvMsgP;
    dm_sm_send_keypress_notification_req(&prim->deviceAddr, prim->notificationType, NULL);
}

void CsrBtCmSmRepairResHandler(cmInstanceData_t *cmData)
{
    CsrBtCmSmRepairRes    * prim;

    prim = (CsrBtCmSmRepairRes *) cmData->recvMsgP;

    if (prim->repairId == cmData->dmVar.rebond.keyMissingId && cmData->smVar.smInProgress)
    {
        switch (cmData->smVar.smMsgTypeInProgress)
        {
#ifndef EXCLUDE_CSR_BT_RFC_MODULE
            case CSR_BT_CM_CONNECT_REQ:
            case CSR_BT_CM_CONNECT_EXT_REQ:
            {
                if (cmData->rfcVar.connElement && cmData->rfcVar.connElement->cmRfcConnInst)
                {
                    cmRfcConnInstType *theLogicalLink = cmData->rfcVar.connElement->cmRfcConnInst;
                    if (cmData->rfcVar.connectState == CM_RFC_SSP_REPAIR)
                    {
                        if (prim->accept)
                        { /* Repairing has been performed                   */
                            aclTable *aclConnectionElement;
                            CsrUint8 featIndex = LMP_FEATURES_SIMPLE_PAIRING_BIT/8;
                            CsrUint8 featOffsetBit = LMP_FEATURES_SIMPLE_PAIRING_BIT%8;

                            CSR_BT_CM_STATE_CHANGE(cmData->rfcVar.connectState, CM_RFC_IDLE);
                            returnAclConnectionElement(cmData, theLogicalLink->deviceAddr, &aclConnectionElement);

                            if ((aclConnectionElement && !CSR_BIT_IS_SET(aclConnectionElement->remoteFeatures[featIndex], featOffsetBit)) ||
                                !aclConnectionElement)
                            {
                                CSR_BT_CM_STATE_CHANGE(theLogicalLink->state, connect_s);
                                CsrBtCmRfcStartInitiateConnection(cmData, theLogicalLink);
                            }
                            else /* if (aclConnectionElement) */
                            {
                                CSR_BT_CM_STATE_CHANGE(theLogicalLink->state, access_s);
                                CsrBtCmDmSmAccessReqMsgSend();
                            }
                        }
                        else
                        { /* Repairing has been rejected or fail            */
                            cmData->smVar.resultCode     = CSR_BT_RESULT_CODE_CM_REBOND_REJECTED_BY_APPLICATION;
                            cmData->smVar.resultSupplier = CSR_BT_SUPPLIER_CM;
                            CSR_BT_CM_STATE_CHANGE(cmData->rfcVar.connectState, CM_RFC_CANCELING);
                            CsrBtCmRfcCommonErrorHandler(cmData, theLogicalLink);
                        }
                    }
                    else
                    {
                        CsrBtCmStateEventException(CSR_BT_CM_PRIM, cmData->smVar.smMsgTypeInProgress, cmData->rfcVar.connectState);
                    }
                }
                else
                {
                    CsrBtCmGeneralException(CSR_BT_CM_PRIM, cmData->smVar.smMsgTypeInProgress, "No cmRfcConnElement in CsrBtCmSmRepairResHandler");
                }
                break;
            }
#endif

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
            case CSR_BT_CM_L2CA_CONNECT_REQ:
            {
                if (cmData->l2caVar.connectState == CM_L2CA_SSP_REPAIR)
                {
                    if (cmData->l2caVar.connElement && cmData->l2caVar.connElement->cmL2caConnInst)
                    {
                        cmL2caConnInstType *l2CaConnection = cmData->l2caVar.connElement->cmL2caConnInst;

                        if (prim->accept)
                        { /* Repairing has been performed                   */
                            CSR_BT_CM_STATE_CHANGE(cmData->l2caVar.connectState, CM_L2CA_CONNECT);
                            L2CA_AutoConnectReq(L2CA_CID_INVALID, /* L2CA_CID_INVALID =create new */
                                                l2CaConnection->psm,
                                                &l2CaConnection->deviceAddr,
                                                l2CaConnection->remotePsm,
                                                CM_L2CA_CONNECT_INPROGRESS_CTX,
                                                CSR_BT_AMP_CONTROLLER_BREDR,
                                                CSR_BT_AMP_CONTROLLER_BREDR,
                                                l2CaConnection->conftabIter.size,
                                                CsrMemDup(l2CaConnection->conftabIter.block, l2CaConnection->conftabIter.size*sizeof(CsrUint16)));
                        }
                        else
                        { /* Repairing has been rejected or fail            */
                            CsrBtCmL2CaConnectCfmErrorHandler(cmData,
                                                              cmData->l2caVar.connElement,
                                                              CSR_BT_RESULT_CODE_CM_REBOND_REJECTED_BY_APPLICATION,
                                                              CSR_BT_SUPPLIER_CM);
                        }
                    }
                    else
                    {
                        CsrBtCmGeneralException(CSR_BT_CM_PRIM, cmData->smVar.smMsgTypeInProgress, "No cmL2caConnElement in CsrBtCmSmRepairResHandler");
                    }
                }
                else
                {
                    CsrBtCmStateEventException(CSR_BT_CM_PRIM, CSR_BT_CM_L2CA_CONNECT_REQ, cmData->l2caVar.connectState);
                }
                break;
            }
#endif

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
            case CSR_BT_CM_BNEP_CONNECT_REQ:
            {
                if (cmData->bnepVar.connectState  == CM_BNEP_SSP_REPAIR)
                {
                    bnepTable *bnepConnection = cmData->bnepVar.indexPtr;
                    ETHER_ADDR remAddr = CsrBtBdAddrToEtherAddr(&(bnepConnection->deviceAddr));

                    if (prim->accept)
                    { /* Repairing has been performed                   */
                        CSR_BT_CM_STATE_CHANGE(cmData->bnepVar.connectState, CM_BNEP_CONNECT);
                        CsrBtBnepConnectReqSend(cmData->bnepVar.connectReqFlags, remAddr);

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_INQUIRY_PAGE_STATE
                        /* Raise event that connection operation is started */
                        CsrBtCmPropgateEvent(cmData,
                                            CsrBtCmPropagateInquiryPageEvents,
                                            CSR_BT_CM_EVENT_MASK_SUBSCRIBE_INQUIRY_PAGE_STATE,
                                            HCI_SUCCESS,
                                            NULL,
                                            NULL);
#endif
                    }
                    else
                    { /* Repairing has been rejected or fail            */
                        CsrBtCmBnepConnectErrorIndHandler(cmData, bnepConnection->deviceAddr,
                                              bnepConnection, ID_EMPTY, 0, 0, remAddr,
                                              CSR_BT_RESULT_CODE_CM_REBOND_REJECTED_BY_APPLICATION,
                                              CSR_BT_SUPPLIER_CM);
                    }
                }
                else
                {
                    CsrBtCmStateEventException(CSR_BT_CM_PRIM, CSR_BT_CM_BNEP_CONNECT_REQ, cmData->bnepVar.connectState);
                }
                break;
            }
#endif
            default:
            {
                CsrBtCmStateEventException(CSR_BT_CM_PRIM, cmData->smVar.smMsgTypeInProgress, cmData->globalState);
                break;
            }
        }
    }
    else
    { /* The Id don't match, just ignore this request   */
        CsrBtCmStateEventException(CSR_BT_CM_PRIM, cmData->smVar.smMsgTypeInProgress, cmData->globalState);
    }
}

void CsrBtCmSmUserConfirmationRequestResHandler(cmInstanceData_t *cmData)
{
    CsrBtCmSmUserConfirmationRequestRes *prim;
    TYPED_BD_ADDR_T ad;

    prim = (CsrBtCmSmUserConfirmationRequestRes *) cmData->recvMsgP;
    ad.addr = prim->deviceAddr;
    ad.type = prim->addressType;
    dm_sm_user_confirmation_request_rsp(&ad, NULL);
}

void CsrBtCmSmUserConfirmationRequestNegResHandler(cmInstanceData_t *cmData)
{
    CsrBtCmSmUserConfirmationRequestNegRes *prim;
    TYPED_BD_ADDR_T ad;

    prim = (CsrBtCmSmUserConfirmationRequestNegRes *) cmData->recvMsgP;
    ad.addr = prim->deviceAddr;
    ad.type = prim->addressType;
    dm_sm_user_confirmation_request_neg_rsp(&ad, NULL);
}

void CsrBtCmSmUserPasskeyRequestResHandler(cmInstanceData_t *cmData)
{
    CsrBtCmSmUserPasskeyRequestRes *prim;
    TYPED_BD_ADDR_T ad;

    prim = (CsrBtCmSmUserPasskeyRequestRes *) cmData->recvMsgP;
    ad.addr = prim->deviceAddr;
    ad.type = prim->addressType;
    dm_sm_user_passkey_request_rsp(&ad, prim->numericValue, NULL);
}

void CsrBtCmSmUserPasskeyRequestNegResHandler(cmInstanceData_t *cmData)
{
    CsrBtCmSmUserPasskeyRequestNegRes *prim;
    TYPED_BD_ADDR_T ad;

    prim = (CsrBtCmSmUserPasskeyRequestNegRes *) cmData->recvMsgP;
    ad.addr = prim->deviceAddr;
    ad.type = prim->addressType;
    dm_sm_user_passkey_request_neg_rsp(&ad, NULL);
}

#endif

