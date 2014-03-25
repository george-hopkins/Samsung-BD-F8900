/****************************************************************************

    Copyright Cambridge Silicon Radio Limited and its licensors 2011.
        
          All rights reserved.

REVISION:      $Revision: #6 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_cm_main.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_rfc.h"
#include "csr_bt_cm_bnep.h"
#include "csr_bt_cm_l2cap.h"
#include "csr_bt_sc_prim.h"
#include "dm_prim.h"
#include "csr_bt_cm_private_lib.h"


void CsrBtCmGetSecurityConfIndSend(cmInstanceData_t *cmData,
                                   CsrUint8 lmpVersion)
{
    CsrBtCmGetSecurityConfInd * prim;

    prim = (CsrBtCmGetSecurityConfInd*)CsrPmemAlloc(sizeof(CsrBtCmGetSecurityConfInd));
    prim->type = CSR_BT_CM_GET_SECURITY_CONF_IND;
    prim->lmpVersion = lmpVersion;
    CsrBtCmPutMessage(cmData->scHandle, prim);
}

void CsrBtCmDmSmAccessIndHandler(cmInstanceData_t * cmData)
{
    DM_SM_ACCESS_IND_T * prim;

    prim = (DM_SM_ACCESS_IND_T *) cmData->recvMsgP;

    if (!prim->conn_setup.incoming && prim->status != HCI_SUCCESS)
    {
        if (cmData->smVar.smInProgress)
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

                        if(bd_addr_eq(&prim->conn_setup.connection.addrt.addr,
                                      &(theLogicalLink->deviceAddr)))
                        {
                            cmData->dmVar.rebond.dmSmAccessIndStatus = prim->status;
                        }
                        else
                        {
                            /* Just ignore */
                            ;
                        }
                    }
                    else
                    {
                        /* just igore */
                        ;
                    }
                    break;
                }
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */
#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
                case CSR_BT_CM_L2CA_CONNECT_REQ:
                {
                    if (cmData->l2caVar.connElement && cmData->l2caVar.connElement->cmL2caConnInst)
                    {
                        cmL2caConnInstType *l2CaConnection = cmData->l2caVar.connElement->cmL2caConnInst;

                        if( bd_addr_eq(&prim->conn_setup.connection.addrt.addr,
                                       &(l2CaConnection->deviceAddr)))
                        {
                            cmData->dmVar.rebond.dmSmAccessIndStatus = prim->status;
                        }
                        else
                        { /* Just ignore */
                            ;
                        }
                    }
                    else
                    { /* just igore */
                        ;
                    }
                    break;
                }
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */
#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
                case CSR_BT_CM_BNEP_CONNECT_REQ:
                {
                    bnepTable *bnepConnection;

                    bnepConnection = cmData->bnepVar.indexPtr;

                    if( bd_addr_eq(&prim->conn_setup.connection.addrt.addr,
                                   &(bnepConnection->deviceAddr)))
                    {
                        cmData->dmVar.rebond.dmSmAccessIndStatus = prim->status;
                    }
                    else
                    { /* Just ignore */
                        ;
                    }
                    break;
                }
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */
                default:
                { /* Just ignore */
                    break;
                }
            }
        }
        else
        { /* Just ignore */
            ;
        }
    }
    else
    { /* It is an incoming connect, just ignore */
        ;
    }
}

void CsrBtCmDmSmClearRebondData(cmInstanceData_t *cmData)
{
    cmData->dmVar.rebond.linkKeyType         = DM_SM_LINK_KEY_NONE;
    cmData->dmVar.rebond.dmSmAccessIndStatus = HCI_SUCCESS;
    bd_addr_zero(&cmData->dmVar.rebond.keyMissingDeviceAddr);
}

CsrBool CsrBtCmDmSmRebondNeeded(cmInstanceData_t *cmData)
{
    return
      (CsrBtBdAddrEqZero(&cmData->dmVar.rebond.keyMissingDeviceAddr)           &&
      cmData->dmVar.rebond.dmSmAccessIndStatus != HCI_SUCCESS                  &&
      (cmData->dmVar.rebond.linkKeyType == HCI_UNAUTHENTICATED_COMBINATION_KEY ||
       cmData->dmVar.rebond.linkKeyType == HCI_AUTHENTICATED_COMBINATION_KEY   ||
       cmData->dmVar.rebond.linkKeyType == HCI_COMBINATION_KEY                 ||
       cmData->dmVar.rebond.linkKeyType == HCI_DEBUG_COMBINATION_KEY) );
}

void CsrBtCmDmAclOpenCfm(cmInstanceData_t *cmData,
                         BD_ADDR_T *p_bd_addr,
                         CsrBool success)
{
    DM_ACL_OPEN_CFM_T *p_prim;
    p_prim = CsrPmemAlloc(sizeof(DM_ACL_OPEN_CFM_T));
    p_prim->type = DM_ACL_OPEN_CFM;
    p_prim->phandle = CSR_BT_CM_IFACEQUEUE;
    p_prim->success = success;

    bd_addr_copy(&p_prim->addrt.addr, p_bd_addr);
    p_prim->addrt.type = CSR_BT_ADDR_PUBLIC;

    CsrSchedMessagePut(cmData->scHandle, DM_PRIM, p_prim);
}

void CsrBtCmSmDeleteStoreLinkKeyReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmSmDeleteStoreLinkKeyReq    *cmPrim;
    cmPrim = (CsrBtCmSmDeleteStoreLinkKeyReq *) cmData->recvMsgP;
    dm_hci_delete_stored_link_key(&(cmPrim->deviceAddr), cmPrim->flag, NULL);
}

void CsrBtCmSmRemoveDeviceReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmSmRemoveDeviceReq *cmPrim;
    TYPED_BD_ADDR_T ad;

    cmPrim = (CsrBtCmSmRemoveDeviceReq *) cmData->recvMsgP;
    ad.addr = cmPrim->deviceAddr;
    ad.type = cmPrim->addressType;
    dm_sm_remove_device_req(CSR_BT_CM_IFACEQUEUE,
                            &ad, NULL);
}

void CsrBtCmDmAclOpenCfmScStateHandler(cmInstanceData_t * cmData)
{
    DM_ACL_OPEN_CFM_T * prim;

    prim = (DM_ACL_OPEN_CFM_T *) cmData->recvMsgP;

    if (prim != NULL &&
        bd_addr_eq(&cmData->dmVar.cacheTargetDev, &prim->addrt.addr) &&
        cmData->dmVar.state == scConnect_s)
    {
        CSR_BT_CM_STATE_CHANGE(cmData->dmVar.state, scNull_s);
        cmData->dmVar.cancelConnect = FALSE;
    }
}

void CsrBtCmSmAclOpenReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmSmAclOpenReq    *cmPrim;

    CsrUint16 clockOffset                    = 0;
    page_scan_mode_t pageScanMode           = 0;
    page_scan_rep_mode_t pageScanRepMode    = 0;

    cmPrim                      = (CsrBtCmSmAclOpenReq *) cmData->recvMsgP;
    cmData->dmVar.appHandle = cmPrim->appHandle;

    if(isCacheParamsKnown(cmData, cmPrim->deviceAddr, &clockOffset, &pageScanMode, &pageScanRepMode))
    {
        CSR_BT_CM_STATE_CHANGE(cmData->dmVar.state, scConnectInit_s);

        CsrBtCmDmWriteCacheParamsDirect(cmData,
                                   cmPrim->deviceAddr,
                                   clockOffset,
                                   pageScanMode,
                                   pageScanRepMode,
                                   SC_PLAYER,
                                   CSR_BT_CM_SM_ACL_OPEN_REQ);
    }
    else
    {
        TYPED_BD_ADDR_T ad;
        ad.addr = cmPrim->deviceAddr;
        ad.type = CSR_BT_ADDR_PUBLIC;
        dm_acl_open_req(&ad, NULL);

        CsrBtCmDmLocalQueueHandler();
    }
}

void CsrBtCmSmCancelConnectReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmSmCancelConnectReq *cmPrim;
    aclTable * aclConnectionElement;

    cmPrim   = (CsrBtCmSmCancelConnectReq *) cmData->recvMsgP;

    returnAclConnectionElement(cmData, cmPrim->deviceAddr, &aclConnectionElement);

    if (aclConnectionElement == NULL &&
        cmData->dmVar.state != scNull_s &&
        bd_addr_eq(&cmData->dmVar.cacheTargetDev, &cmPrim->deviceAddr))
    {
        if (cmData->dmVar.state == scConnect_s)
        {
            dm_hci_create_connection_cancel(&(cmPrim->deviceAddr), NULL);
        }
        else
        {
            cmData->dmVar.cancelConnect = TRUE;
        }
    }
    else
    {
        /* ACL already up or unknown */
        ;
    }
}

void CsrBtCmSmAclCloseReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmSmAclCloseReq    *cmPrim;
    TYPED_BD_ADDR_T ad;

    cmPrim  = (CsrBtCmSmAclCloseReq *) cmData->recvMsgP;
    ad.addr = cmPrim->deviceAddr;
    ad.type = CSR_BT_ADDR_PUBLIC;
    dm_acl_close_req(&ad,
                     0, /* flags */
                     0, /* reason */
                     NULL);
    CsrBtCmDmLocalQueueHandler();
}

#ifdef CSR_BT_INSTALL_SC_AUTHENTICATE
void CsrBtCmSmAuthenticateReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmSmAuthenticateReq    *cmPrim;

    cmPrim                      = (CsrBtCmSmAuthenticateReq *) cmData->recvMsgP;
    dm_sm_authenticate_req(&cmPrim->deviceAddr, NULL);
    CsrBtCmDmLocalQueueHandler();
}
#endif
#ifdef CSR_BT_INSTALL_SC_ENCRYPTION
void CsrBtCmSmEncryptionReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmSmEncryptionReq    *cmPrim;
    cmPrim = (CsrBtCmSmEncryptionReq *) cmData->recvMsgP;

    cmData->dmVar.appHandle = cmPrim->appHandle;
    dm_sm_encrypt_req( &cmPrim->deviceAddr, cmPrim->encryptionMode, NULL);
}
#endif

#ifdef CSR_BT_INSTALL_SC_SECURITY_MODE
void CsrBtCmSmSetSecModeReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmSmSetSecModeReq    *cmPrim;

    /* Usually invoked by the SC after the application has set the
     * security level */
    cmPrim = (CsrBtCmSmSetSecModeReq *) cmData->recvMsgP;
    dm_sm_init_req(DM_SM_INIT_SECURITY_MODE | DM_SM_INIT_MODE3_ENC, /* options */
                   cmPrim->mode,
                   0, /* security_level_default (not specified) */
                   0, /* config (not specified) */
                   0, /* write_auth_enable (not specified) */
                   cmPrim->mode3Enc,
                   NULL, /* sm_key_state */
                   0, /* sm_div_state*/
                   NULL);
}
#endif

void CsrBtCmSmSetDefaultSecLevelReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmSmSetDefaultSecLevelReq    *cmPrim;

    cmPrim = (CsrBtCmSmSetDefaultSecLevelReq *) cmData->recvMsgP;

    dm_sm_init_req(DM_SM_INIT_SECURITY_LEVEL_DEFAULT, /* options */
                   0, /* mode  (not specified) */
                   cmPrim->seclDefault,
                   0, /* config (not specified) */
                   0, /* write_auth_enable (not specified) */
                   0, /* mode3_enc */
                   NULL, /* sm_key_state */
                   0, /* sm_div_state*/
                   NULL);

    CsrBtCmDmLocalQueueHandler();
}

void CsrBtCmSmUnRegisterReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmSmUnregisterReq    *cmPrim;

    cmPrim                      = (CsrBtCmSmUnregisterReq *) cmData->recvMsgP;
    dm_sm_unregister_req(CSR_BT_CM_IFACEQUEUE,
                         0, /* context */
                         cmPrim->protocolId, cmPrim->channel, NULL);
    CsrBtCmDmLocalQueueHandler();
}

void CsrBtCmSmLinkKeyRequestResHandler(cmInstanceData_t *cmData)
{
    CsrBtCmSmLinkKeyRequestRes    *cmPrim;
    cmPrim = (CsrBtCmSmLinkKeyRequestRes *) cmData->recvMsgP;
    cmData->dmVar.rebond.linkKeyType = cmPrim->keyType;
    dm_sm_link_key_request_rsp(&cmPrim->deviceAddr, cmPrim->keyType, cmPrim->key, NULL);
    CsrPmemFree(cmPrim->key); /* The key will be copied to a typed key struct in the lib function. */
    cmPrim->key = NULL;
}

void CsrBtCmSmPinRequestResHandler(cmInstanceData_t *cmData)
{
    CsrBtCmSmPinRequestRes    *cmPrim;
    cmPrim = (CsrBtCmSmPinRequestRes *) cmData->recvMsgP;
    dm_sm_pin_request_rsp(&cmPrim->deviceAddr, cmPrim->pinLength, cmPrim->pin, NULL);
}

void CsrBtCmSmRegisterReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmSmRegisterReq    *cmPrim;

    cmPrim                      = (CsrBtCmSmRegisterReq *) cmData->recvMsgP;
    dm_sm_register_req(CSR_BT_CM_IFACEQUEUE,
                       0, /* context */
                       cmPrim->protocolId,
                       cmPrim->channel,
                       cmPrim->outgoingOk,
                       cmPrim->securityLevel,
                       cmPrim->psm, NULL);
    CsrBtCmDmLocalQueueHandler();
}

void CsrBtCmScRejectedForSecurityReasonMsgSend(cmInstanceData_t *cmData,
                                               CsrBtDeviceAddr theAddr,
                                               CsrBool cancelInitiated)
{
    CsrBtCmConnectionRejSecurityInd * prim;
    prim = (CsrBtCmConnectionRejSecurityInd *) CsrPmemAlloc(sizeof(CsrBtCmConnectionRejSecurityInd));

    prim->type          = CSR_BT_CM_CONNECTION_REJ_SECURITY_IND;
    prim->cancelInitiated   = cancelInitiated;
    prim->deviceAddr        = theAddr;
    CsrBtCmPutMessage(cmData->scHandle, prim);
}

#ifdef CSR_BT_INSTALL_SC_ENCRYPTION
static void sendCmReadEncryptionStatusCfm(CsrSchedQid                 appHandle,
                                          CsrBool              encrypted,
                                          CsrBtResultCode     resultCode,
                                          CsrBtSupplier resultSupplier)
{
    CsrBtCmReadEncryptionStatusCfm *cmPrim;

    cmPrim                  = (CsrBtCmReadEncryptionStatusCfm *)CsrPmemAlloc(sizeof(CsrBtCmReadEncryptionStatusCfm));
    cmPrim->type            = CSR_BT_CM_READ_ENCRYPTION_STATUS_CFM;
    cmPrim->encrypted       = encrypted;
    cmPrim->resultCode      = resultCode;
    cmPrim->resultSupplier  = resultSupplier;
    CsrBtCmPutMessage(appHandle, cmPrim);
}

void CsrBtCmReadEncryptionStatusReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmReadEncryptionStatusReq *cmPrim;
    aclTable                       * aclConnectionElement;

    cmPrim = (CsrBtCmReadEncryptionStatusReq *) cmData->recvMsgP;

    returnAclConnectionElement(cmData, cmPrim->deviceAddr, &aclConnectionElement);

    if (aclConnectionElement)
    {
        sendCmReadEncryptionStatusCfm(cmPrim->appHandle, aclConnectionElement->encrypted,
                                        CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM);
    }
    else
    {
        sendCmReadEncryptionStatusCfm(cmPrim->appHandle, FALSE,
                                CSR_BT_RESULT_CODE_CM_UNKNOWN_CONNECTION_IDENTIFIER, CSR_BT_SUPPLIER_CM);
    }
    CsrBtCmDmLocalQueueHandler();
}
#endif

/*************************************************************************************
 CsrBtCmAutoAcceptForThisPsmAllowed:
************************************************************************************/
#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
static void cmScMarkPeerAsAuthorised(cmInstanceData_t *cmData, CsrBtDeviceAddr *deviceAddr, CsrUint32 psm)
{
    cmData->scVar.deviceAddr = *deviceAddr;
    cmData->scVar.psm = psm;
}

void CsrBtCmScCleanupVar(cmInstanceData_t *cmData)
{
    CsrBtBdAddrZero(&cmData->scVar.deviceAddr);
    cmData->scVar.psm = L2CA_PSM_INVALID;
}

static CsrBool CsrBtCmAutoAcceptForThisPsmAllowed(cmInstanceData_t *cmData, CsrBtDeviceAddr *deviceAddr, CsrUint32 psm)
{
    cmL2caConnElement *currentElem;
    CsrBool allow = FALSE;

    for ( currentElem = CM_L2CA_GET_FIRST(cmData->l2caVar.connList);
         currentElem;
         currentElem = currentElem->next )
    {
        cmL2caConnInstType *cmL2caConn = currentElem->cmL2caConnInst;
        if (cmL2caConn != NULL)
         {
             if((cmL2caConn->btConnId != BTCONN_ID_EMPTY) &&
                (cmL2caConn->psm == psm) &&
                CsrBtBdAddrEq(&cmL2caConn->deviceAddr, deviceAddr))
             {
                if(cmL2caConn->authorised)
                {
                    allow = TRUE;
                    break;
                }
            }
        }
    }

    return allow;
}

/*************************************************************************************
 CsrBtCmAllowAutoAcceptForThisPsm:
************************************************************************************/
static void CsrBtCmAllowAutoAcceptForThisPsm(cmInstanceData_t *cmData, CsrBtDeviceAddr *deviceAddr, CsrUint32 psm)
{
    cmL2caConnElement *currentElem;

    for (currentElem = CM_L2CA_GET_FIRST(cmData->l2caVar.connList);
         currentElem;
         currentElem = currentElem->next)
     {
         cmL2caConnInstType *cmL2caConn = currentElem->cmL2caConnInst;
         if (cmL2caConn != NULL)
         {
             if((cmL2caConn->btConnId != BTCONN_ID_EMPTY) &&
                (cmL2caConn->psm == psm) &&
                CsrBtBdAddrEq(&cmL2caConn->deviceAddr, deviceAddr))
             {
                if(!cmL2caConn->authorised)
                {
                    cmL2caConn->authorised = TRUE;
                    break;
                }
             }
         }
     }
}
#endif

void CsrBtCmSmAuthoriseResHandler(cmInstanceData_t *cmData)
{
    CsrBtCmSmAuthoriseRes *cmPrim;

    cmPrim = (CsrBtCmSmAuthoriseRes *) cmData->recvMsgP;

    dm_sm_authorise_rsp(&cmPrim->deviceAddr,
                        cmPrim->protocolId,
                        cmPrim->channel,
                        cmPrim->incoming,
                        cmPrim->authorisation,
                        NULL);

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
    if (((cmPrim->authorisation == DM_SM_AUTHORISE_ACCEPT_ONCE) ||
         (cmPrim->authorisation == DM_SM_AUTHORISE_ACCEPT_TWICE) ||
         (cmPrim->authorisation == DM_SM_AUTHORISE_ACCEPT_LIFE_OF_ACL)) && (cmPrim->protocolId == CSR_BT_SC_PROTOCOL_L2CAP))
    {
        cmScMarkPeerAsAuthorised(cmData, &cmPrim->deviceAddr, cmPrim->channel);
    }
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

#ifndef EXCLUDE_CSR_BT_RFC_MODULE
    if (((cmPrim->authorisation == DM_SM_AUTHORISE_REJECT_ONCE) ||
         (cmPrim->authorisation == DM_SM_AUTHORISE_REJECT_TWICE) ||
         (cmPrim->authorisation == DM_SM_AUTHORISE_REJECT_LIFE_OF_ACL)) && (cmPrim->protocolId == CSR_BT_SC_PROTOCOL_RFCOMM))
    {
        cmRfcConnElement *rfcElement = CsrBtCmRfcFindRfcConnElementFromDeviceAddrState1OrState2(cmData, &cmPrim->deviceAddr, 
                                                                    connectAccept_s, connectAcceptFinal_s);
        if (rfcElement)
        {
           cmRfcConnInstType *theLogicalLink   = rfcElement->cmRfcConnInst;
           CsrBtCmDisconnectReqSend(theLogicalLink->btConnId);
        }
    }
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */
}


static CsrBool CsrBtCmDmAuthoriseIndRejectHandler(cmInstanceData_t *cmData)
{
    DM_SM_AUTHORISE_IND_T *prim;
    CsrUint8 theIndex = CM_ERROR;

    prim = (DM_SM_AUTHORISE_IND_T *) cmData->recvMsgP;

    if (prim->cs.connection.service.protocol_id == CSR_BT_SC_PROTOCOL_L2CAP)
    {
#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
        if ((psm_t)prim->cs.connection.service.channel == CSR_BT_PAN_BNEP_PSM)
        {
            theIndex = returnReserveBnepIdIndex(cmData);
        }
        else
#endif
        {
#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
            cmL2caConnElement *theElement = CM_FIND_L2CA_ELEMENT(CsrBtCmL2caFindL2caConnElementFromReserveBtConnIdPsm, &prim->cs.connection.service.channel);

            if (theElement)
            {
                theIndex = theElement->elementId;
            }
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */
        }
    }
    else
    {
#ifndef EXCLUDE_CSR_BT_RFC_MODULE
        cmRfcConnElement *theElement = CsrBtCmRfcFindRfcConnElementFromServerState(cmData, (CsrUint8)prim->cs.connection.service.channel, connectAccept_s);
        if (theElement)
        {
            theIndex = theElement->elementId;
        }
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */
    }

    if (prim && prim->cs.incoming && theIndex == CM_ERROR)
    {/* We don't have any servers for this incoming connection. Simply reject it. */
        dm_sm_authorise_rsp(&prim->cs.connection.addrt.addr, prim->cs.connection.service.protocol_id, prim->cs.connection.service.channel,
                            prim->cs.incoming, DM_SM_AUTHORISE_REJECT_ONCE, NULL);

        CsrPmemFree(cmData->recvMsgP);
        cmData->recvMsgP = NULL;

        return TRUE;
    }

    return FALSE;
}

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
static CsrBool CsrBtCmDmAuthoriseIndAutoAcceptHandler(cmInstanceData_t *cmData)
{
    DM_SM_AUTHORISE_IND_T *prim;

    prim = (DM_SM_AUTHORISE_IND_T *) cmData->recvMsgP;

    if (prim && prim->cs.incoming &&
        prim->cs.connection.service.protocol_id == CSR_BT_SC_PROTOCOL_L2CAP &&
        CsrBtCmAutoAcceptForThisPsmAllowed(cmData, &prim->cs.connection.addrt.addr, prim->cs.connection.service.channel))
    {/* Auto accept allowed, so it accept it. */
        dm_sm_authorise_rsp(&prim->cs.connection.addrt.addr, prim->cs.connection.service.protocol_id, prim->cs.connection.service.channel,
                            prim->cs.incoming, DM_SM_AUTHORISE_ACCEPT_ONCE, NULL);

        CsrBtCmAllowAutoAcceptForThisPsm(cmData, &prim->cs.connection.addrt.addr, prim->cs.connection.service.channel);

        CsrPmemFree(cmData->recvMsgP);
        cmData->recvMsgP = NULL;

        return TRUE;
    }

    return FALSE;
}
#else
    #define CsrBtCmDmAuthoriseIndAutoAcceptHandler(cmData) FALSE
#endif

void CsrBtCmDmAuthoriseIndHandler(cmInstanceData_t *cmData)
{
     if (!CsrBtCmDmAuthoriseIndRejectHandler(cmData) && !CsrBtCmDmAuthoriseIndAutoAcceptHandler(cmData))
     {
         /* Allow SC to process this message */
         CsrSchedMessagePut(CSR_BT_SC_IFACEQUEUE, DM_PRIM, (cmData->recvMsgP));
         cmData->recvMsgP = NULL;
     }
}

void CsrBtCmGetSecurityConfResHandler(cmInstanceData_t *cmData)
{
    CsrBtCmGetSecurityConfRes * prim = (CsrBtCmGetSecurityConfRes *) cmData->recvMsgP;
    DM_SM_KEY_STATE_T ks;

    CsrMemSet(&ks, 0, sizeof(DM_SM_KEY_STATE_T));
#ifdef CSR_BT_LE_ENABLE
    if(prim->leEr && prim->leErCount == 8)
    {
        CsrMemCpy(ks.er, prim->leEr, sizeof(CsrUint16)*8);
        CsrPmemFree(prim->leEr);
        prim->leEr = NULL;
    }
    if(prim->leIr && prim->leIrCount == 8)
    {
        CsrMemCpy(ks.ir, prim->leIr, sizeof(CsrUint16)*8);
        CsrPmemFree(prim->leIr);
        prim->leIr = NULL;
    }
#endif

    /* The SC has fetched the security setup */
    dm_sm_init_req_le(prim->options,
                      prim->securityMode,
                      prim->securityLevelDefault,
                      prim->config,
                      prim->writeAuthEnable,
                      prim->mode3enc,
                      &ks, /* sm_key_state */
                      prim->leSmDivState,
                      prim->leSmSignCounter,
                      NULL);
}

/* CM start-up handler for initial DM_SM_INIT_CFM */
void CsrBtCmDmSmInitCfmHandler(cmInstanceData_t *cmData)
{
    
#if CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1
    if (cmData->dmVar.lmpVersion >= CSR_BT_BLUETOOTH_VERSION_2P1)
    {
        /* LMP version is 2.1 or newer - read local name for EIR */
        dm_hci_read_local_name(NULL);
    }
    else
#endif
    {
        dm_hci_write_inquiryscan_activity(cmData->dmVar.inquiryscanInterval,
                                          cmData->dmVar.inquiryscanWindow,
                                          NULL);
    }
}

#ifdef CSR_BT_LE_ENABLE
void CsrBtCmSmKeyRequestResHandler(cmInstanceData_t *cmData)
{
    CsrBtCmSmKeyRequestRes *prim;
    DM_SM_UKEY_T key;
    TYPED_BD_ADDR_T ad;
    prim = (CsrBtCmSmKeyRequestRes*)cmData->recvMsgP;

    CsrMemSet(&key, 0, sizeof(DM_SM_UKEY_T));
    if(prim->keyType != DM_SM_LINK_KEY_NONE
       && prim->keySize != 0
       && prim->key != NULL)
    {
        if ((prim->keyType == DM_SM_KEY_DIV)
            || (prim->keyType == DM_SM_KEY_DIV_CSRK))
        {
            /* Transfer uint16 */
            key.div = *(CsrUint16*)prim->key;
        }
        else
        {
            /* Transfer pointer to struct (consumes key) */
            key.none = prim->key;
            prim->key = NULL;
        }
    }

    /* Ensure key is freed */
    CsrPmemFree(prim->key);
    prim->key = NULL;

    ad.addr = prim->address;
    ad.type = prim->addressType;

    /* Key truly present */
    if(prim->keySize != 0)
    {
        dm_sm_key_request_rsp(&ad,
                              prim->securityRequirements,
                              (DM_SM_KEY_TYPE_T)prim->keyType,
                              key,
                              NULL);
    }
    else
    {
        dm_sm_key_request_neg_rsp(&ad,
                                  (DM_SM_KEY_TYPE_T)prim->keyType,
                                  NULL);
    }
}
#endif
