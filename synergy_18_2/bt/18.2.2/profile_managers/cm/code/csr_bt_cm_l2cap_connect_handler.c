/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE

#include "csr_bt_cm_l2cap.h"
#include "csr_bt_cm_l2cap_conftab.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_util.h"
#include "csr_bt_sc_private_lib.h"
#include "bkeyval.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_events_handler.h"

/* Called by the 'cache parameter update' or 'lp settings update'
 * handlers immediately before the L2CAP auto-connect-req gets
 * sent. The DM queue is already held at this point.  We return TRUE
 * if we detached, otherwise FALSE. */
CsrBool CsrBtCmL2caCheckLegacyDetach(cmInstanceData_t *cmData,
                                     cmL2caConnInstType *l2caConn)
{
    CsrUint8 numConns = 0;
    aclTable *aclElement = NULL;

    numConns = CsrBtCmReturnNumOfConnectionsToPeerDevice(cmData, l2caConn->deviceAddr);
    returnAclConnectionElement(cmData, l2caConn->deviceAddr, &aclElement);

    if((aclElement != NULL) &&
       (numConns == 0) &&
       ((l2caConn->secLevel & SECL_OUT_AUTHENTICATION) ||
        (l2caConn->secLevel & SECL_OUT_ENCRYPTION) ||
        (l2caConn->secLevel & SECL_OUT_AUTHORISATION)) &&
        !CsrBtCmCheckSavedIncomingConnectMessages(cmData, l2caConn->deviceAddr))
    {
        /* Send detach - DM queue already locked, so send to DM
         * directly */
        TYPED_BD_ADDR_T ad;
        CSR_BT_CM_STATE_CHANGE(l2caConn->state, l2capLegacyDetach_s);
        cmData->dmVar.appHandle = CSR_BT_CM_IFACEQUEUE;
        cmData->dmVar.dmMsgInProgress = CSR_BT_CM_ACL_DETACH_REQ;
        CsrBtBdAddrCopy(&cmData->dmVar.detachAddr, &l2caConn->deviceAddr);
        aclElement->detachRequested = TRUE;

        ad.addr = l2caConn->deviceAddr;
        ad.type = CSR_BT_ADDR_PUBLIC;
        dm_acl_close_req(&ad,
                         DM_ACL_FLAG_FORCE,
                         HCI_ERROR_OETC_USER, NULL);
        return TRUE;
    }
    else
    {
        /* Continue normal operation -- allow connection to proceed */
        return FALSE;
    }
}

static void csrBtCmL2caActivateQosSetupRequestHandler(cmL2caConnInstType *l2CaConnection)
{
    /* It is a high data priority data channel */
    if (l2CaConnection->dataPriority == CSR_BT_CM_PRIORITY_HIGH ||
        l2CaConnection->dataPriority == CSR_BT_CM_PRIORITY_HIGHEST)
    {
        dm_hci_qos_setup_req(&(l2CaConnection->deviceAddr), 
                             0, 
                             CSR_BT_AV_QOS_SERVICE_TYPE, 
                             CSR_BT_AV_QOS_TOKEN_RATE, 
                             CSR_BT_AV_QOS_PEAK_BANDWIDTH,
                             CSR_BT_AV_QOS_LATENCY,
                             CSR_BT_AV_QOS_DELAY_VARIATION, 
                             NULL);
    }
}

static cmL2caConnElement * returnCancelL2caConnectIndex(cmInstanceData_t *cmData,
                                                        psm_t localPsm, CsrSchedQid phandle,
                                                        CsrBtDeviceAddr deviceAddr)
{
    if (cmData->smVar.smInProgress &&
        (cmData->smVar.smMsgTypeInProgress == CSR_BT_CM_L2CA_CONNECT_REQ))
    {
        cmL2caConnElement *currentElem;

        for (currentElem = CM_L2CA_GET_FIRST(cmData->l2caVar.connList);
             currentElem;
             currentElem = currentElem->next)
        {
            /* Search through the l2ca list */
            if (currentElem->cmL2caConnInst)
            {
                if(currentElem->cmL2caConnInst->psm == localPsm)
                {
                    if (currentElem->cmL2caConnInst->appHandle == phandle)
                    {
                        if (bd_addr_eq(&(currentElem->cmL2caConnInst->deviceAddr), &(deviceAddr)))
                        {
                            if (currentElem->cmL2caConnInst->state == l2capConnect_s ||
                                currentElem->cmL2caConnInst->state == l2capConnectInit_s)
                            {
                                return (currentElem);
                            }
                        }
                    }
                }
            }
        }
    }
    return NULL;
}

static void csrBtCmL2caConnectCfmMsgSend(CsrSchedQid phandle,
                                         CsrBtConnId btConnId,
                                         psm_t psm,
                                         l2ca_mtu_t mtu,
                                         l2ca_mtu_t localMtu,
                                         CsrBtDeviceAddr deviceAddr,
                                         CsrBtResultCode resultCode,
                                         CsrBtSupplier resultSupplier,
                                         CsrUint16     context)
{
    CsrBtCmL2caConnectCfm * cmPrim = (CsrBtCmL2caConnectCfm *) CsrPmemAlloc(sizeof(CsrBtCmL2caConnectCfm));
    cmPrim->type                   = CSR_BT_CM_L2CA_CONNECT_CFM;
    cmPrim->btConnId               = btConnId;
    cmPrim->localPsm               = psm;
    cmPrim->mtu                    = mtu;
    cmPrim->localMtu               = localMtu;
    cmPrim->deviceAddr             = deviceAddr;
    cmPrim->resultCode             = resultCode;
    cmPrim->resultSupplier         = resultSupplier;
    cmPrim->context                = context; 
    CsrBtCmPutMessage(phandle, cmPrim);
}

void CsrBtCmL2caConnectCfmMsgHandler(cmInstanceData_t *cmData, cmL2caConnElement * theElement,
                                     CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    /* Send a CSR_BT_CM_L2CA_CONNECT_CFM signal to the application, and
     * restore the local service manager queue */
    cmL2caConnInstType *l2CaConnection = theElement->cmL2caConnInst;

    CsrBtCmDmSmClearRebondData(cmData);
    CsrBtCmDmUpdateAndClearCachedParamReqSend(l2CaConnection->deviceAddr);

    csrBtCmL2caConnectCfmMsgSend(l2CaConnection->appHandle,
                                 l2CaConnection->btConnId,
                                 l2CaConnection->psm,
                                 l2CaConnection->outgoingMtu,
                                 l2CaConnection->incomingMtu,
                                 l2CaConnection->deviceAddr,
                                 resultCode, resultSupplier,
                                 l2CaConnection->context);

    if (resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        csrBtCmL2caActivateQosSetupRequestHandler(l2CaConnection);
        cmDmAclKickRoleSwitch(cmData);
    }
    else
    {
        CsrUint8 numOutgoing;
        CsrUint8 numIncoming;

        numberOfSecurityRegister(cmData, l2CaConnection->psm,
                                 l2CaConnection->deviceAddr,
                                 &numOutgoing, &numIncoming);
        if (numOutgoing == 1)
        {
            /* Unregister The Outgoing security setting */
            dm_sm_unregister_outgoing_req(CSR_BT_CM_IFACEQUEUE,
                                          0, /* context */
                                          &l2CaConnection->deviceAddr,
                                          SEC_PROTOCOL_L2CAP,
                                          l2CaConnection->remotePsm, NULL);
        }
        else
        {
            /* There is more that need this security setting */
            ;
        }
        CsrBtCmL2capClearL2capTableIndex(cmData, &(theElement->cmL2caConnInst));
    }
    CsrBtCmServiceManagerLocalQueueHandler(cmData);
}

static void csrBtCmL2caConnectAcceptCfmMsgSend(CsrSchedQid appHandle, CsrBtConnId  btConnId,
                                               psm_t localPsm, psm_t remotePsm, l2ca_mtu_t mtu,
                                               l2ca_mtu_t localMtu, CsrBtDeviceAddr deviceAddr,
                                               CsrBtResultCode resultCode, CsrBtSupplier resultSupplier,
                                               CsrUint16 context)
{
    CsrBtCmL2caConnectAcceptCfm * cmPrim;

    cmPrim    = (CsrBtCmL2caConnectAcceptCfm *)CsrPmemAlloc(sizeof(CsrBtCmL2caConnectAcceptCfm));
    cmPrim->type            = CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM;
    cmPrim->btConnId        = btConnId;
    cmPrim->deviceAddr      = deviceAddr;
    cmPrim->localPsm        = localPsm;
    cmPrim->remotePsm       = remotePsm;
    cmPrim->mtu             = mtu;
    cmPrim->localMtu    = localMtu;
    cmPrim->resultCode      = resultCode;
    cmPrim->resultSupplier  = resultSupplier;
    cmPrim->context         = context;
    CsrBtCmPutMessage(appHandle, cmPrim);
}

void CsrBtCmL2caConnectAcceptCfmHandler(cmInstanceData_t *cmData, cmL2caConnElement * theElement,
                                        CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    /* Send a CM_L2CA_ACCEPT_CONNECT_CFM signal to the application,
     * and restore the local service manager queue */
    cmL2caConnInstType *theLink = theElement->cmL2caConnInst;

    csrBtCmL2caConnectAcceptCfmMsgSend(theLink->appHandle,
                                       theLink->btConnId,
                                       theLink->psm,
                                       theLink->remotePsm,
                                       theLink->outgoingMtu,
                                       theLink->incomingMtu,
                                       theLink->deviceAddr,
                                       resultCode,
                                       resultSupplier,
                                       theLink->context);

    if (resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        aclTable *aclElement;

        returnAclConnectionElement(cmData, theLink->deviceAddr, &aclElement);
        if( aclElement != NULL )
        {
            theLink->sniffSettings = aclElement->sniffSettings;
            theLink->parkSettings  = aclElement->parkSettings;
        }
        
        csrBtCmL2caActivateQosSetupRequestHandler(theLink);
        cmDmAclKickRoleSwitch(cmData);

        /* Start processing */
        CsrBtCmL2caDataStart(cmData, theLink);

        CsrBtCmDmLocalQueueHandler();
    }
    else
    {
        CsrUint8 numOutgoing;
        CsrUint8 numIncoming;

        numberOfSecurityRegister(cmData, theLink->psm, theLink->deviceAddr, &numOutgoing, &numIncoming);
        if (numIncoming == 1)
        {
            /* Unregister The Incoming security setting */
            CsrBtScDeregisterReqSend(SEC_PROTOCOL_L2CAP, theLink->psm);
        }
        else
        {
            /* There is more that need this security setting */
            ;
        }

        CsrBtCmL2capClearL2capTableIndex(cmData, &(theElement->cmL2caConnInst));
    }
    CsrBtCmServiceManagerLocalQueueHandler(cmData);
}

void CsrBtCmL2caCancelConnectAcceptCfmMsgSend(cmInstanceData_t *cmData, CsrSchedQid appHandle, psm_t psm,
                                              CsrBtResultCode resultCode, CsrBtSupplier resultSupplier,
                                              CsrUint16 context)
{
    /* Send a CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM signal to the
     * application, and restore the local service manager queue */
    CsrBtCmL2caCancelConnectAcceptCfm    * cmPrim;

    cmPrim    = (CsrBtCmL2caCancelConnectAcceptCfm *)CsrPmemAlloc(sizeof(CsrBtCmL2caCancelConnectAcceptCfm));

    cmPrim->type            = CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM;
    cmPrim->resultCode      = resultCode;
    cmPrim->resultSupplier  = resultSupplier;
    cmPrim->localPsm        = psm;
    cmPrim->context         = context;
    CsrBtCmPutMessage(appHandle, cmPrim);
}

void CsrBtCml2caAutoConnectSetup(cmInstanceData_t *cmData, cmL2caConnInstType *l2CaConnection)
{
    l2CaConnection->actualMode = CSR_BT_ACTIVE_MODE;

    CSR_BT_CM_STATE_CHANGE(cmData->l2caVar.connectState, CM_L2CA_CONNECT);
    CSR_BT_CM_STATE_CHANGE(l2CaConnection->state, l2capConnect_s);

    L2CA_AutoConnectReq(L2CA_CID_INVALID, /* L2CA_CID_INVALID =create new*/
                        l2CaConnection->psm,
                        &l2CaConnection->deviceAddr,
                        l2CaConnection->remotePsm,
                        CM_L2CA_CONNECT_INPROGRESS_CTX,
                        CSR_BT_AMP_CONTROLLER_BREDR,
                        CSR_BT_AMP_CONTROLLER_BREDR,
                        l2CaConnection->conftabIter.size,
                        CsrMemDup(l2CaConnection->conftabIter.block,
                                  l2CaConnection->conftabIter.size*sizeof(CsrUint16)));

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

static void csrBtCmL2caOptimalMtuMps(cmInstanceData_t *cmData,
                                     cmL2caConnInstType *l2capConn,
                                     CsrUint16 *mtu,
                                     CsrUint16 *mps,
                                     CsrUint16 *outMtu)
{
    CsrUint16 mode = 0;
    CsrUint16 bredr = 0;
#ifdef CSR_AMP_ENABLE
    CsrBool align = FALSE;
#endif
    aclTable *aclConnectionElement = NULL;

    /* Rules for determining incoming MTU:
     * RFCOMM build          -> use RFCOMM optimal value
     * basic mode            -> use BR/EDR optimal value
     * flow mode with AMP    -> use AMP max, maximise MTU
     * flow mode without AMP -> use BR/EDR optimal, maximise MTU
     */

    if (cmData->rfcBuild)
    {
        *mtu = CSR_BT_RFC_BUILD_L2CAP_MAX_FRAME_SIZE;
        *mps = CSR_BT_RFC_BUILD_L2CAP_MAX_FRAME_SIZE;
    }
    else
    {
        /* Figure out whether we're running BR or EDR */
        if (HCI_FEATURE_IS_SUPPORTED(LMP_FEATURES_5SLOT_MR_BIT, cmData->dmVar.lmpSuppFeatures))
        {/* The local device supports EDR */
            /* At this point the remote BD Address is not 0. This function is called either at outgoing connection request, or
               when the ACL link is already established at incoming connection, so just try to find the ACL instance */
            returnAclConnectionElement(cmData, l2capConn->deviceAddr, &aclConnectionElement);

            if (aclConnectionElement && 
               !HCI_FEATURE_IS_SUPPORTED(LMP_FEATURES_5SLOT_MR_BIT, aclConnectionElement->remoteFeatures))
            {/* There is an ACL link and the remote device does not support EDR; use BR */
                bredr = CSR_BT_HCI_BUILD_L2CAP_NON_EDR_MAX_FRAME_SIZE;
            }
            else
            {/* The remote supports EDR too */
                bredr = CSR_BT_HCI_BUILD_L2CAP_EDR_MAX_FRAME_SIZE;
            }
        }
        else
        {/* The local device does not support EDR */
            bredr = CSR_BT_HCI_BUILD_L2CAP_NON_EDR_MAX_FRAME_SIZE;
        }

        if(!BKV_Scan16Single(&l2capConn->conftabIter,
                             L2CA_AUTOPT_FLOW_MODE, &mode) || (mode == 0))
        {
            /* Basic mode */
            *mtu = bredr;
            *mps = bredr;
        }
        else
        {
#ifdef CSR_AMP_ENABLE
            /* Flow mode with AMP. L2CAPs flow control mode will
             * automatically adjust the MPS to whatever controller we
             * are using, so optimising for 802.11/UWB should not
             * matter -- except that we need to set this to the
             * highest possible packet size we will ever encounter */
            align = TRUE;
            *mtu = CSR_BT_MAXIMUM_AUTOMATIC_MTU;
            *mps = CSR_BT_MAXIMUM_AMP_PACKET_SIZE;
#else
            /* Flow mode over BR/EDR */
            *mtu = CSR_BT_MAXIMUM_AUTOMATIC_MTU;
            *mps = bredr;
#endif
        }

        /* Cap sizes */
        *mtu = CSRMIN(*mtu, CSR_BT_MAXIMUM_AUTOMATIC_MTU);
        *mps = CSRMIN(*mps, CSR_BT_MAXIMUM_AMP_PACKET_SIZE);
        *mps = CSRMIN(*mps, CSR_BT_MAXIMUM_AUTOMATIC_MTU);

        /* MPS should never exceed MTU */
        if(*mps > *mtu)
        {
            *mps = *mtu;
        }
#ifdef CSR_AMP_ENABLE
        else if(align)
        {
            /* Align MTU to a multiple of MPS */
            *mtu = *mtu - (*mtu % *mps);
        }
#endif
    }

    if(BKV_Scan16Single(&l2capConn->conftabIter,
                        L2CA_AUTOPT_MTU_OUT,
                        &mode))
    {
        /* Select hard requirement although minimum the spec value */
        *outMtu = CSRMAX(L2CA_MTU_MINIMUM, mode);
    }
    else
    {
        /* No hard requirements from profile, so we may only request
         * the specification default */
        *outMtu = L2CA_MTU_DEFAULT;
    }

}

/* Fix outgoing and non-critical flow control settings */
static void csrBtCmL2caFixConftabFlowControl(cmInstanceData_t *cmData,
                                             cmL2caConnInstType *l2capConn,
                                             CsrUint16 *rxWindowSize)
{
    CsrUint16 val1;
    CsrUint16 val2;

    /* Only set up flow params if using flow control mode */
    if(!BKV_Scan16Single(&l2capConn->conftabIter,
                         L2CA_AUTOPT_FLOW_MODE, &val1) || (val1 != 0))
    {       
        /* Fix outgoing window size */
        if(!BKV_Scan16Range(&l2capConn->conftabIter,
                            L2CA_AUTOPT_FLOW_WINDOW_OUT,
                            &val1, &val2))
        {
            CsrBtCmL2caConftabTxwinAllowAnyPeer(l2capConn->conftabIter.block,
                                                &l2capConn->conftabIter.size);
        }

        /* Read incoming window size */
        if(!BKV_Scan16Range(&l2capConn->conftabIter,
                            L2CA_AUTOPT_FLOW_WINDOW_IN,
                            &val1, &val2))
        {
            *rxWindowSize = MAX_L2CAP_TX_WINDOW;
        }
        else
        {
            *rxWindowSize = CSRMAX(val1, val2);
        }
    }
    else
    {
        *rxWindowSize = MAX_L2CAP_DATA_QUEUE_LENGTH;
    }
}

/* Read conftab data priority and set CM L2CA instance member */
static void csrBtCmL2caFixConftabDataPriority(cmInstanceData_t *cmData,
                                              cmL2caConnInstType *l2capConn)
{
    CsrUint16 val = 0;

    if(BKV_Scan16Single(&l2capConn->conftabIter,
                        L2CA_AUTOPT_CHANNEL_PRIORITY, &val))
    {
        /* CSR_BT_CM_PRIORITY_LOW(=2) is internal to the CM, so this
         * check is only here to ensure the range. Other values are
         * defined in cm_prim */
        l2capConn->dataPriority = (CsrUint16)((val >= CSR_BT_CM_PRIORITY_LOW)
                                              ? CSR_BT_CM_PRIORITY_NORMAL
                                              : val);
    }
    else
    {
        l2capConn->dataPriority = CSR_BT_CM_PRIORITY_NORMAL;
    }
}

/* Set or read conftab flush timeout values as appropriate */
static void csrBtCmL2caFixConftabFlush(cmInstanceData_t *cmData,
                                       cmL2caConnInstType *l2capConn)
{
    CsrUint32 val = 0;
    CsrUint32 val2 = 0;

    /* Has incoming flush NOT been set? */
    if(!BKV_Scan32Range(&l2capConn->conftabIter,
                        L2CA_AUTOPT_FLUSH_IN, &val, &val2))
    {
        CsrBtCmL2caConftabFlushToAllowAnyPeer(l2capConn->conftabIter.block,
                                              &l2capConn->conftabIter.size);
    }
}

/* Set or read conftab MTU/MPS values as appropriate */
static void csrBtCmL2caFixConftabMtu(cmInstanceData_t *cmData,
                                      cmL2caConnInstType *l2capConn)
{
    CsrUint16 val = 0;
    CsrUint16 val2 = 0;

    /* Has incoming MTU been set? */
    if(BKV_Scan16Single(&l2capConn->conftabIter,
                        L2CA_AUTOPT_MTU_IN, &val))
    {
        l2capConn->incomingMtu = val;
    }
    else
    {
        CsrBtCmL2caConftabMtu(l2capConn->conftabIter.block,
                              &l2capConn->conftabIter.size,
                              TRUE, /* incoming */
                              l2capConn->incomingMtu);
    }

    /* Has outgoing MTU been set? */
    if(BKV_Scan16Single(&l2capConn->conftabIter,
                        L2CA_AUTOPT_MTU_OUT, &val))
    {
        l2capConn->outgoingMtu = val;
    }
    else
    {
        CsrBtCmL2caConftabMtu(l2capConn->conftabIter.block,
                              &l2capConn->conftabIter.size,
                              FALSE, /* outgoing */
                              l2capConn->outgoingMtu);
    }


    if(BKV_Scan16Single(&l2capConn->conftabIter,
                        L2CA_AUTOPT_FLOW_MODE, &val))
    {
        /* Has incoming MPS been set? */
        if(BKV_Scan16Range(&l2capConn->conftabIter,
                           L2CA_AUTOPT_FLOW_MAX_PDU_IN, &val, &val2))
        {
            l2capConn->incomingMps = CSRMAX(val, val2);
        }
        else
        {
            CsrBtCmL2caConftabMps(l2capConn->conftabIter.block,
                                  &l2capConn->conftabIter.size,
                                  TRUE, /* incoming */
                                  l2capConn->incomingMps, l2capConn->incomingMps);
        }

        /* Has outgoing MPS been set? */
        if(!BKV_Scan16Range(&l2capConn->conftabIter,
                            L2CA_AUTOPT_FLOW_MAX_PDU_OUT, &val, &val2))
        {
            CsrBtCmL2caConftabMps(l2capConn->conftabIter.block,
                                  &l2capConn->conftabIter.size,
                                  FALSE, /* outgoing */
                                  1, l2capConn->outgoingMtu); /* min, max */
        }        
    }
}

/* Add the cached remote L2CAP extended features to the table, if not
 * there already */
static void csrBtCmL2caFixConftabExtendedFeatures(cmInstanceData_t *cmData,
                                                  cmL2caConnInstType *l2capConn,
                                                  CsrBool fakeAll)
{
    CsrUint32 val = 0;
    aclTable *aclConn;
    CsrUint32 feats;

    /* We have a cached value */
    if(fakeAll ||
       ((returnAclConnectionElement(cmData, l2capConn->deviceAddr, &aclConn) != CM_ERROR) &&
        (aclConn->l2capExtendedFeatures != CM_INVALID_L2CAP_EXT_FEAT) &&
        (aclConn->l2capExtendedFeatures != 0)))
    {
        feats = (fakeAll ? 0xFFFFFFFF : aclConn->l2capExtendedFeatures);

        if(!BKV_Scan32Single(&l2capConn->conftabIter,
                             L2CA_AUTOPT_EXT_FEATS, &val))
        {
            /* Add new entry */
            CsrBtCmL2caConftabExtendedFeatures(l2capConn->conftabIter.block,
                                               &l2capConn->conftabIter.size,
                                               feats);
        }
        else
        {
            /* Replace old entry, as we're sure that our cached value
             * is newer */
            if(BKV_JumpToKey(&l2capConn->conftabIter,
                             L2CA_AUTOPT_EXT_FEATS, FALSE))
            {
                BKVD_32_EXACT(l2capConn->conftabIter.block, l2capConn->conftabIter.iterator,
                              L2CA_AUTOPT_EXT_FEATS, feats);
            }
        }
    }
}

/* The application request to create a new l2cap connection. */
void CsrBtCml2caConnectReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmL2caConnectReq *cmPrim;
    CsrUint8 numOutgoing;
    CsrUint8 numIncoming;
    CsrUint8 numConnections;
    cmL2caConnElement *theElement;
    cmL2caConnInstType *l2caConnection;
    aclTable *aclElement;

    cmPrim = (CsrBtCmL2caConnectReq *) cmData->recvMsgP;

    /* Sanity check: Validate the conftab. Bail out and restore the queue */
    if(!BKV_Validate(cmPrim->conftab, cmPrim->conftabCount))
    {
        /* Invalid data - tell client and abort now */
        csrBtCmL2caConnectCfmMsgSend(cmPrim->phandle, L2CA_CID_INVALID, cmPrim->localPsm,
                                     0, 0,
                                     cmPrim->addr,
                                     CSR_BT_RESULT_CODE_CM_UNSUPPORTED_FEATURE_OR_PARAMETER_VALUE,
                                     CSR_BT_SUPPLIER_CM,
                                     cmPrim->context);

        CsrPmemFree(cmPrim->conftab);
        cmPrim->conftab = NULL;
        cmPrim->conftabCount = 0;

        CsrBtCmServiceManagerLocalQueueHandler(cmData);
        return;
    }

    /* Sanity check: Connection table full. Bail out and restore the queue */
    if (!CsrBtCmL2caSetElementCounter(cmData))
    {
        csrBtCmL2caConnectCfmMsgSend(cmPrim->phandle, L2CA_CID_INVALID, cmPrim->localPsm, 0, 0, cmPrim->addr,
                                     CSR_BT_RESULT_CODE_CM_REJECTED_DUE_TO_LIMITED_RESOURCES,
                                     CSR_BT_SUPPLIER_CM,
                                     cmPrim->context);

        CsrPmemFree(cmPrim->conftab);
        cmPrim->conftab = NULL;
        cmPrim->conftabCount = 0;

        CsrBtCmServiceManagerLocalQueueHandler(cmData);
        return;
    }

    numConnections = CsrBtCmReturnNumOfConnectionsToPeerDevice(cmData, cmPrim->addr);
    theElement = (cmL2caConnElement *) CsrCmnListElementAddLast(&(cmData->l2caVar.connList), sizeof(cmL2caConnElement));
    l2caConnection = theElement->cmL2caConnInst;

    theElement->elementId                 = cmData->l2caVar.elementCounter;
    cmData->l2caVar.connElement           = theElement;
    cmData->l2caVar.cancelConnect         = FALSE;

    CSR_BT_CM_STATE_CHANGE(cmData->l2caVar.connectState, CM_L2CA_IDLE);
    l2caConnection->context         = cmPrim->context;
    l2caConnection->deviceAddr      = cmPrim->addr;
    l2caConnection->appHandle       = cmPrim->phandle;
    l2caConnection->psm             = cmPrim->localPsm;
    l2caConnection->remotePsm       = cmPrim->remotePsm;
    l2caConnection->btConnId        = BTCONN_ID_EMPTY;
    l2caConnection->requestedMode   = CSR_BT_ACTIVE_MODE;
    l2caConnection->sniffSettings   = cmData->dmVar.defaultSniffSettings;
    l2caConnection->parkSettings    = cmData->dmVar.defaultParkSettings;
    l2caConnection->logicalChannelTypeMask = CSR_BT_ACTIVE_CONTROL_CHANNEL;
    l2caConnection->lowPowerPriority = DEFAULT_PRIORITY;
#if CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1
    l2caConnection->ssrAccepted     = FALSE;
#endif
    l2caConnection->secLevel        = cmPrim->secLevel;
    l2caConnection->incomingMtu     = 0;
    l2caConnection->incomingMps     = 0;
    l2caConnection->conftabIter.block    = cmPrim->conftab;
    l2caConnection->conftabIter.size     = cmPrim->conftabCount;
    l2caConnection->conftabIter.iterator = 0;
    cmPrim->conftab = NULL;
    cmPrim->conftabCount = 0;
#ifdef CSR_AMP_ENABLE
    l2caConnection->controller      = CSR_BT_AMP_CONTROLLER_BREDR;
#endif /* CSR_AMP_ENABLE */

    /* Calculate default, optimal MTU and MPS values */
    csrBtCmL2caOptimalMtuMps(cmData, l2caConnection,
                             &l2caConnection->incomingMtu,
                             &l2caConnection->incomingMps,
                             &l2caConnection->outgoingMtu);

    /* Make room for more conftab entries and adjust missing entries */
    CsrBtCmL2caConftabEnlarge(&l2caConnection->conftabIter.block, &l2caConnection->conftabIter.size);
    csrBtCmL2caFixConftabFlowControl(cmData, l2caConnection, &l2caConnection->rxQueueMax);
    csrBtCmL2caFixConftabMtu(cmData, l2caConnection);
    csrBtCmL2caFixConftabFlush(cmData, l2caConnection);
    csrBtCmL2caFixConftabExtendedFeatures(cmData, l2caConnection, FALSE);
    csrBtCmL2caFixConftabDataPriority(cmData, l2caConnection);
    CsrBtCmL2caConftabCull(&l2caConnection->conftabIter.block, &l2caConnection->conftabIter.size);

    /* Deal with security */
    numberOfSecurityRegister(cmData, cmPrim->localPsm, cmPrim->addr,
                             &numOutgoing, &numIncoming);

    CsrBtCmDmSmClearRebondData(cmData);

    if (numOutgoing == 1)
    {
        /* Register Outgoing Security on this. The PSM has been
         * changed to localPsm as a workaround. The core stack
         * expects the remote PSM value in this signal, but when
         * it is used, it is compared to the local PSM */
        dm_sm_register_outgoing_req(CSR_BT_CM_IFACEQUEUE,
                                    0, /* context */
                                    &cmPrim->addr,
                                    SEC_PROTOCOL_L2CAP,
                                    cmPrim->localPsm,
                                    cmPrim->secLevel, 0, NULL);
    }
    else
    {
        /* The security on this PSM has already been set */
        ;
    }

    returnAclConnectionElement(cmData, cmPrim->addr, &aclElement);

    if( aclElement != NULL )
    {
        l2caConnection->sniffSettings   = aclElement->sniffSettings;
        l2caConnection->parkSettings    = aclElement->parkSettings;
    }

    if(numConnections == 0)
    {
        /* Currently there is no connection attach to the given
         * Bluetooth address. Establish a new l2cap connection */
        CsrUint16 clockOffset = 0;
        page_scan_mode_t pageScanMode = 0;
        page_scan_rep_mode_t pageScanRepMode = 0;

        if(isCacheParamsKnown(cmData, cmPrim->addr, &clockOffset, &pageScanMode, &pageScanRepMode))
        {
            CSR_BT_CM_STATE_CHANGE(l2caConnection->state, l2capConnectInit_s);

            /* start writing cache parameters via DM queue */
            CsrBtCmDmWriteCacheParamsReqSend(L2CAP_PLAYER, cmPrim->addr, clockOffset,
                                             pageScanMode, pageScanRepMode);
        }
        else if((aclElement != NULL) &&
                ((cmPrim->secLevel & SECL_OUT_AUTHENTICATION) ||
                 (cmPrim->secLevel & SECL_OUT_ENCRYPTION) ||
                 (cmPrim->secLevel & SECL_OUT_AUTHORISATION)) &&
                 !CsrBtCmCheckSavedIncomingConnectMessages(cmData, cmPrim->addr))
        {
            /* ACL is up but there are no active connections to peer
             * - and we want outgoing security. To avoid the ACL
             * dropping while performing the local parts of legacy
             * pairing, ensure that we drop the ACL now to avoid
             * security to fail (due to ACL inactivity kill
             * engage). */
            CSR_BT_CM_STATE_CHANGE(l2caConnection->state, l2capLegacyDetach_s);
            CsrBtCmAclDetachReqSend(CSR_BT_CM_IFACEQUEUE,
                                    cmPrim->addr,
                                    CSR_BT_CM_ACL_DETACH_ALWAYS);
        }
        else
        {
            /* Everything seems OK - initiate the L2CAP connection
             * sequence */
            CsrBtCml2caAutoConnectSetup(cmData, l2caConnection);
        }
    }
    else
    {
        /* There is at least one connection to this peer. Return
         * the actual mode the link is in */
        CSR_BT_CM_STATE_CHANGE(l2caConnection->state, l2capConnectInit_s);
        CsrBtCmDmModeChangeReqMsgSend(L2CAP_PLAYER, theElement->elementId,
                                      CSR_BT_ACTIVE_MODE, l2caConnection->deviceAddr);
    }
}

void CsrBtCml2caCancelConnectReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmCancelL2caConnectReq *prim = (CsrBtCmCancelL2caConnectReq *) cmData->recvMsgP;
    cmL2caConnElement *theElement     = returnCancelL2caConnectIndex(cmData, prim->localPsm, prim->phandle, prim->deviceAddr);

    if (theElement == NULL)
    {
        /* The CSR_BT_CM_L2CA_CONNECT_REQ msg is not placed in the l2cap
         * connection table */
        CsrUint8    dummy;
        CsrUint16   context;
        if(cancelServiceManagerMsg(cmData, CSR_BT_CM_L2CA_CONNECT_REQ, prim->phandle, prim->deviceAddr, CSR_BT_NO_SERVER, prim->localPsm, &dummy, &context))
        {
            /* The CSR_BT_CM_L2CA_CONNECT_REQ msg is found and removed from
             * the local SM queue */
            csrBtCmL2caConnectCfmMsgSend(prim->phandle, L2CA_CID_INVALID, prim->localPsm, 0, 0, prim->deviceAddr,
                                         CSR_BT_RESULT_CODE_CM_CANCELLED, CSR_BT_SUPPLIER_CM, context);
        }
        else
        {
            /* Nothing to cancel just ignore */
            ;
        }
    }
    else
    {
        cmData->l2caVar.cancelConnect   = TRUE;
        CsrBtCmScRejectedForSecurityReasonMsgSend(cmData,
                                                  prim->deviceAddr, TRUE);

        /* This device is a LMP version 1.2 or higher */
        if (cmData->l2caVar.connectState == CM_L2CA_SSP_REPAIR)
        {
            CsrBtCmSmCancelSppRepairInd(cmData);
            CsrBtCmL2CaConnectCfmErrorHandler(cmData, theElement,
                    CSR_BT_RESULT_CODE_CM_CANCELLED, CSR_BT_SUPPLIER_CM);
        }
        else
        {
            if (CsrBtCmDmCancelPageOrDetach(cmData, prim->deviceAddr))
            { /* The ACL is being detach change state to ensure that
                 it not cancel twice                                            */
                CSR_BT_CM_STATE_CHANGE(cmData->l2caVar.connectState, CM_L2CA_CANCELING);
            }
            else 
            {
                cmL2caConnInstType *l2caConnection = theElement->cmL2caConnInst;
                
                if (cmData->l2caVar.connectState == CM_L2CA_CONNECT_PENDING)
                { /* Request l2cap to cancel the outgoing connection            */
                    L2CA_DisconnectReq(CM_GET_UINT16ID_FROM_BTCONN_ID(l2caConnection->btConnId));
                    CSR_BT_CM_STATE_CHANGE(cmData->l2caVar.connectState, CM_L2CA_CANCELING);
                }
                else
                { /* Must Wait until CM receives the right Confirm Msg          */
                    ;
                }
            }
        }
    }
}

void CsrBtCml2caConnectAcceptReqHandler(cmInstanceData_t *cmData)
{
    /* The application request to accept a new l2cap connection,
     * create from a peer device */
    CsrBtDeviceAddr    deviceAddr;
    CsrBtCmL2caConnectAcceptReq  *cmPrim = (CsrBtCmL2caConnectAcceptReq *) cmData->recvMsgP;
    bd_addr_zero(&deviceAddr);

    if (CsrBtCmL2caSetElementCounter(cmData))
    {
        /* If it is possible to accept a new l2cap connection */
        CsrUint8    numOutgoing;
        CsrUint8    numIncoming;
        cmL2caConnElement  *theElement;
        cmL2caConnInstType *l2caConnection;

        theElement = (cmL2caConnElement *) CsrCmnListElementAddLast(&(cmData->l2caVar.connList), sizeof(cmL2caConnElement));
        l2caConnection = theElement->cmL2caConnInst;
        theElement->elementId = cmData->l2caVar.elementCounter;
        l2caConnection->context         = cmPrim->context;    
        l2caConnection->appHandle       = cmPrim->phandle;
        l2caConnection->psm             = cmPrim->localPsm;
        l2caConnection->remotePsm       = NO_REMOTE_PSM;
        l2caConnection->btConnId        = BTCONN_ID_RESERVED;
        l2caConnection->classOfDevice   = cmPrim->classOfDevice;
        l2caConnection->requestedMode   = CSR_BT_ACTIVE_MODE;
        l2caConnection->actualMode      = CSR_BT_ACTIVE_MODE;
        l2caConnection->sniffSettings   = cmData->dmVar.defaultSniffSettings;
        l2caConnection->parkSettings    = cmData->dmVar.defaultParkSettings;
        l2caConnection->lowPowerPriority= DEFAULT_PRIORITY;

#if CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1
        l2caConnection->ssrAccepted     = FALSE;
#endif

#ifdef CSR_AMP_ENABLE
        l2caConnection->controller      = CSR_BT_AMP_CONTROLLER_BREDR;
#endif /* CSR_AMP_ENABLE */

        l2caConnection->logicalChannelTypeMask = CSR_BT_ACTIVE_CONTROL_CHANNEL;

        l2caConnection->conftabIter.block    = cmPrim->conftab;
        l2caConnection->conftabIter.size     = cmPrim->conftabCount;
        l2caConnection->conftabIter.iterator = 0;
        cmPrim->conftab = NULL;
        cmPrim->conftabCount = 0;

        if (cmPrim->primaryAcceptor)
        {
            /* This function starts the connectable routine */
            cmData->l2caVar.connElement = theElement;
            CSR_BT_CM_STATE_CHANGE(l2caConnection->state, l2capConnectAble_s);
            CsrBtCmWriteClassOfDevice(L2CAP_PLAYER);
        }
        else
        {
            /* Do not make the device connectable (Do not write a new
             * class of device value and do not set the device
             * connectable) because the profile allready have a
             * primary l2cap connection */
            CSR_BT_CM_STATE_CHANGE(l2caConnection->state, idle_s);
        }
        numberOfSecurityRegister(cmData, cmPrim->localPsm, deviceAddr, &numOutgoing, &numIncoming);

        if (numIncoming == 1)
        {
            /* Register Incoming Security on this PSM, Don't apply for
             * outgoing connections*/
            CsrBtScRegisterReqSend(cmPrim->profileUuid, cmPrim->localPsm, FALSE, SEC_PROTOCOL_L2CAP, cmPrim->secLevel);
        }
        else
        {
            /* The security on this psm has already been set */
            ;
        }
    }
    else
    {
        /* It is not possible to accept a L2CAP connection. Inform the
         * application and restore the local L2cap queue */
        csrBtCmL2caConnectAcceptCfmMsgSend(cmPrim->phandle, L2CA_CID_INVALID,
                                           cmPrim->localPsm, NO_REMOTE_PSM,
                                           0, 0,
                                           deviceAddr,
                                           CSR_BT_RESULT_CODE_CM_REJECTED_DUE_TO_LIMITED_RESOURCES,
                                           CSR_BT_SUPPLIER_CM,
                                           cmPrim->context);

        CsrPmemFree(cmPrim->conftab);
        cmPrim->conftab = NULL;
        cmPrim->conftabCount = 0;

        if (cmPrim->primaryAcceptor)
        {
            /* Need to called this function because if primaryAcceptor
             * is TRUE then it uses the SM queue */
            CsrBtCmServiceManagerLocalQueueHandler(cmData);
        }
    }
}

void CsrBtCml2caCancelAcceptConnectReqHandler(cmInstanceData_t *cmData)
{
    /* The application request to cancel the previous connect accept
     * request signal */
    CsrBtCmL2caCancelConnectAcceptReq *cmPrim = (CsrBtCmL2caCancelConnectAcceptReq *) cmData->recvMsgP;
    cmL2caConnElement * theElement  = CM_FIND_L2CA_ELEMENT(CsrBtCmL2caFindL2caConnElementFromCancelledBtConnIdPsm,
                                                           &(cmPrim->localPsm));

    if (theElement)
    {
        /* Remove COD and cancel the service */
        cmL2caConnInstType *l2caConnection  = theElement->cmL2caConnInst;
        cmData->l2caVar.connElement         = theElement;
        l2caConnection->btConnId            = BTCONN_ID_EMPTY;
        CSR_BT_CM_STATE_CHANGE((l2caConnection->state), l2capCancelConnectAble_s);
        CsrBtCmWriteClassOfDevice(L2CAP_PLAYER);
    }
    else
    {
        /* The service can't be cancelled. Restore the local service manager queue.
         * A confirm has already been sent to application */
        CsrBtCmServiceManagerLocalQueueHandler(cmData);
    }
}

static CsrBool CsrBtCmL2CaReconnect(cmInstanceData_t *cmData, l2ca_conn_result_t response, CsrBtDeviceAddr deviceAddr)
{
    if (response == L2CA_CONNECT_REJ_SECURITY ||
        response == L2CA_CONNECT_KEY_MISSING)
    {
        if (!cmData->l2caVar.cancelConnect && CsrBtCmDmSmRebondNeeded(cmData))
        {
            return TRUE;
        }
        else
        {
            CsrBtCmScRejectedForSecurityReasonMsgSend(cmData,
                                                      deviceAddr, FALSE);
            return FALSE;
        }
    }
    return FALSE;
}

void CsrBtCmL2CaConnectCfmErrorHandler(cmInstanceData_t     *cmData,
                                       cmL2caConnElement    *theElement,
                                       CsrBtResultCode      resultCode,
                                       CsrBtSupplier  resultSupplier)
{
    cmL2caConnInstType *l2caConnection = theElement->cmL2caConnInst;
    CsrUint8 numConnections = CsrBtCmReturnNumOfConnectionsToPeerDevice(cmData, l2caConnection->deviceAddr);

    CSR_BT_CM_STATE_CHANGE(cmData->l2caVar.connectState, CM_L2CA_IDLE);

    if(numConnections == 0)
    {
        /* There is no other connection attach to the
         * device address.  Inform the application,
         * restore the local service manager queue, and
         * clear up the l2cap connection table */
        CsrBtCmL2caConnectCfmMsgHandler(cmData, theElement, resultCode, resultSupplier);
    }
    else
    {
        /* Currently there is at least one more connection
         * attach to the device address. Ensure that SNIFF
         * and park is supported */
        cmData->smVar.resultCode        = resultCode;
        cmData->smVar.resultSupplier    = resultSupplier;
        CsrBtCmDmWriteLpSettingsReqMsgSend(l2caConnection->deviceAddr, L2CAP_PLAYER);
    }

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

static void csrBtCmL2CaAutoConnectCfmErrorHandler(cmInstanceData_t     *cmData,
                                                  cmL2caConnElement    *theElement,
                                                  CsrBtResultCode      resultCode,
                                                  CsrBtSupplier  resultSupplier)
{
    cmL2caConnInstType *l2caConnection = theElement->cmL2caConnInst;

    switch (l2caConnection->state)
    {
        case l2capConnectAccept_s:
            CsrBtCmL2capAcceptFailClearing(cmData, l2caConnection);
            CsrBtCmServiceManagerLocalQueueHandler(cmData);
            break;

        case l2capConnect_s:
            CsrBtCmL2CaConnectCfmErrorHandler(cmData, theElement, resultCode, resultSupplier);
            break;

        default:
            CsrBtCmStateEventException(0, 0, cmData->globalState);
            break;
    }

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

static void csrBtCmL2caRemoveAutoConnectIndFromSaveQueue(cmInstanceData_t *cmData,
                                                         l2ca_cid_t       cid)
{
    CsrUint16               eventClass;
    void                    *msg;
    CsrMessageQueueType     *tempQueue  = NULL;

    while(CsrMessageQueuePop(&cmData->smVar.SaveQueue, &eventClass, &msg))
    {
        if( (eventClass == L2CAP_PRIM)
            && (((L2CA_UPRIM_T *) msg)->type == L2CA_AUTO_CONNECT_IND) )
        {
            L2CA_AUTO_CONNECT_IND_T * saved_prim = (L2CA_AUTO_CONNECT_IND_T *) msg;

            if (cid == saved_prim->cid)
            {
                CsrPmemFree(msg);
            }
            else
            {
                CsrMessageQueuePush(&tempQueue, eventClass, msg);
            }
        }
        else
        {
            CsrMessageQueuePush(&tempQueue, eventClass, msg);
        }
    }
    cmData->smVar.SaveQueue = tempQueue;
}

static void csrBtCmL2caAutoConnectCmpHandler(cmInstanceData_t *cmData)
{ 
    L2CA_AUTO_CONNECT_CFM_T *prim = (L2CA_AUTO_CONNECT_CFM_T*)cmData->recvMsgP;

    if (cmData->l2caVar.connElement && cmData->l2caVar.connElement->cmL2caConnInst)
    { /* The attempt to setup a new incoming or outgoing l2cap connection
         is finish */
        cmL2caConnInstType *l2caConnection = cmData->l2caVar.connElement->cmL2caConnInst;

        switch(prim->result)
        {
            case L2CA_CONNECT_SUCCESS:
            { /* A L2CAP connection is establish with success */
                CSR_BT_CM_STATE_CHANGE(cmData->l2caVar.connectState, CM_L2CA_IDLE);
                l2caConnection->btConnId = CM_CREATE_L2CA_CONN_ID(prim->cid);

                CsrBtCmDmSmClearRebondData(cmData);

                if (!cmData->l2caVar.cancelConnect)
                {
                    aclTable *aclConn;

                    CsrBtCmL2capStoreConfigOptions(&prim->config, l2caConnection);
                    CsrBtCmL2capConfigHandler(cmData, l2caConnection);

                    /* Cache the remove supported features */
                    if(returnAclConnectionElement(cmData, l2caConnection->deviceAddr, &aclConn) != CM_ERROR)
                    {
                        aclConn->l2capExtendedFeatures = prim->ext_feats;
                    }
                }
                else
                {
                    cmData->smVar.resultCode        = CSR_BT_RESULT_CODE_CM_CANCELLED;
                    cmData->smVar.resultSupplier    = CSR_BT_SUPPLIER_CM;
                    L2CA_DisconnectReq(prim->cid);
                }

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_INQUIRY_PAGE_STATE
                /* Raise event that connection operation is ended */
                CsrBtCmPropgateEvent(cmData,
                                     CsrBtCmPropagateInquiryPageEvents,
                                     CSR_BT_CM_EVENT_MASK_SUBSCRIBE_INQUIRY_PAGE_STATE,
                                     HCI_SUCCESS,
                                     NULL,
                                     NULL);
#endif

                break;
            }
            case L2CA_CONNECT_RETRYING :
            { /* Retry L2CAP connection (handled internally) */
                break;
            }
            case L2CA_CONNECT_INITIATING:
            case L2CA_CONNECT_PENDING :
            { /* The L2CAP connection is pending, wait for another CFM signal */
                l2caConnection->btConnId = CM_CREATE_L2CA_CONN_ID(prim->cid);

                if (cmData->l2caVar.cancelConnect)
                { /* The application has requested to cancel the outgoing 
                     l2cap connection */
                    if (cmData->l2caVar.connectState == CM_L2CA_CANCELING)
                    { /* The connection is being cancel ignore this message */
                        ;
                    }
                    else
                    { /* Request l2cap to cancel the outgoing connection
                         E.g. CsrBtCmCancelL2caConnectReq were received before 
                         CM receives this message */
                        CSR_BT_CM_STATE_CHANGE(cmData->l2caVar.connectState, CM_L2CA_CANCELING);
                        L2CA_DisconnectReq(prim->cid);
                    }
                }
                else
                {
                    CSR_BT_CM_STATE_CHANGE(cmData->l2caVar.connectState, CM_L2CA_CONNECT_PENDING);
                }
                break;
            }
            default:
            { /* Establishment failure */
                if (CsrBtCmL2CaReconnect(cmData, prim->result, l2caConnection->deviceAddr))
                {
                    /* The connect attempt fail due to security
                     * reason, e.g the local device had a SSP link key
                     * and the remote device did not */
                    CSR_BT_CM_STATE_CHANGE(cmData->l2caVar.connectState, CM_L2CA_SSP_REPAIR);
                    CsrBtCmSmSppRepairIndSend(cmData, l2caConnection->deviceAddr);
                }
                else
                {
                    if (!cmData->l2caVar.cancelConnect)
                    {
                        csrBtCmL2CaAutoConnectCfmErrorHandler(cmData,
                                                              cmData->l2caVar.connElement,
                                                              (CsrBtResultCode) prim->result,
                                                              CSR_BT_SUPPLIER_L2CAP_CONNECT);
                    }
                    else
                    {
                        csrBtCmL2CaAutoConnectCfmErrorHandler(cmData,
                                                              cmData->l2caVar.connElement,
                                                              CSR_BT_RESULT_CODE_CM_CANCELLED,
                                                              CSR_BT_SUPPLIER_CM);
                    }
                }
                break;
            }
        }
    }
    else
    { /* An incoming L2CAP connection must have failed, 
         remove it from save queue */
        csrBtCmL2caRemoveAutoConnectIndFromSaveQueue(cmData, prim->cid);
    }
}

static CsrBool CsrBtCmL2caReConfigIndHandler(cmInstanceData_t *cmData)
{
    L2CA_AUTO_CONNECT_IND_T *prim = (L2CA_AUTO_CONNECT_IND_T*)cmData->recvMsgP;
    CsrBtConnId btConnId          = CM_CREATE_L2CA_CONN_ID(prim->cid);
    cmL2caConnElement *theElement = CM_FIND_L2CA_ELEMENT(CsrBtCmL2caFindL2caConnElementFromBtConnId, &(btConnId));

    if (theElement)
    { /*The peer request to reconfigure an established l2cap connection */
#ifdef CSR_AMP_ENABLE
        cmL2caConnInstType *l2caConnection  = theElement->cmL2caConnInst;

        if((CsrBtCmL2caFcsReConfigEnable(l2caConnection->conftabIter)) &&
            (l2caConnection->state == l2capConnected_s                 ||
             l2caConnection->state == l2capConnect_s                   ||
             l2caConnection->state == l2capConnectAcceptFinal_s))
        {
            /* Tweak FCS based on AMP controller */
            CsrUint16 conftabLength;
            CsrUint16 *conftab = CsrBtCmL2caBuildFcsConftab(l2caConnection->controller, &conftabLength);
            L2CA_AutoConnectRsp(prim->identifier,
                                prim->cid,
                                L2CA_CONNECT_SUCCESS,
                                CM_L2CA_RECONFIG_INPROGRESS_CTX, 
                                conftabLength,
                                conftab);
            /* Check if this connection is autorised */            
            if (CsrBtBdAddrEq(&cmData->scVar.deviceAddr,&prim->bd_addr) && (cmData->scVar.psm == prim->psm_local))
            {
                l2caConnection->authorised = TRUE;
                CsrBtCmScCleanupVar(cmData);
            }
        }
        else
#endif
        { /* Refuse reconfig */
            L2CA_AutoConnectRsp(prim->identifier,
                                prim->cid,
                                (l2ca_conn_result_t)L2CA_CONFIG_UNACCEPTABLE,
                                CM_L2CA_INCOMING_CONNECT_REJECTED_CTX,
                                0, /*conftab_length*/
                                NULL); /*conftab*/
        }
        return (TRUE);
    }
    return (FALSE);
}

static void csrBtCmL2caAutoConnectIndRejectHandler(cmInstanceData_t  *cmData,
                                                   l2ca_cid_t        cid,
                                                   l2ca_identifier_t identifier)
{ /* Reject the incoming l2cap connection */
    L2CA_AutoConnectRsp(identifier,
                        cid,
                        L2CA_CONNECT_REJ_RESOURCES,
                        CM_L2CA_INCOMING_CONNECT_REJECTED_CTX,
                        0, /*conftab_length*/
                        NULL); /*conftab*/

    /* We are not active after rejecting this request */
    cmData->l2caVar.connElement = NULL;
    
    if (cmData->smVar.popFromSaveQueue)
    { /* The L2CA_AUTO_CONNECT_IND message has been restore
         from the SM queue. Restore it and lock it again
         in order to make sure that no message can use
         the SM before the CSR_BT_CM_SM_HOUSE_CLEANING
         is received.                                   */
        cmData->smVar.smInProgress = TRUE;
        CsrBtCmServiceManagerLocalQueueHandler(cmData);
    }
}

void CsrBtCmL2caAutoConnectCfmHandler(cmInstanceData_t *cmData)
{
    L2CA_AUTO_CONNECT_CFM_T *prim = (L2CA_AUTO_CONNECT_CFM_T*)cmData->recvMsgP;

    switch (prim->con_ctx)
    {
        case CM_L2CA_CONNECT_INPROGRESS_CTX:
        { /* Confirmation to earlier L2CA_AUTO_CONNECT_REQ or
             L2CA_AUTO_CONNECT_RSP where a new connection is
             being setup */
            csrBtCmL2caAutoConnectCmpHandler(cmData);
            break;
        }
        case CM_L2CA_RECONFIG_INPROGRESS_CTX:
        { /* Confirmation to earlier L2CA_AUTO_CONNECT_REQ or
             L2CA_AUTO_CONNECT_RSP where a connected l2cap 
             connection has been reconfigure. Note always 
             completed or failure at this point. Can not do
             anything about it either way */
            break;
        }
        default:
        { /* An incoming L2CAP connection must have failed, 
             remove it from save queue */
            csrBtCmL2caRemoveAutoConnectIndFromSaveQueue(cmData, prim->cid);
            break;
        }
    }
    
    /* Safe freeing of unconsumed config members */
    L2CA_FreeConfigPtrs(&prim->config);
}

void CsrBtCmL2caAutoConnectIndHandler(cmInstanceData_t *cmData)
{    
    if (CsrBtCmL2caReConfigIndHandler(cmData))
    { /* The peer request to reconfigure an established l2cap connection */
        ;
    }
    else
    { /* The peer request to setup a new l2cap connection */
        if(cmData->smVar.smInProgress)
        { /* Another service is in progress, place this event on queue */
            CsrMessageQueuePush(&cmData->smVar.SaveQueue, L2CAP_PRIM, cmData->recvMsgP);
            cmData->recvMsgP = NULL;
        }
        else
        { /* Ready to handle the incoming connection attempt */
            L2CA_AUTO_CONNECT_IND_T *prim = (L2CA_AUTO_CONNECT_IND_T*)cmData->recvMsgP;

            if(prim->local_control == CSR_BT_AMP_CONTROLLER_BREDR)
            {
                cmL2caConnElement *theElement = CM_FIND_L2CA_ELEMENT(CsrBtCmL2caFindL2caConnElementFromReserveBtConnIdPsm,
                                                                     &(prim->psm_local));

                if(theElement)
                { /* Accept the new incoming connection */
                    cmL2caConnInstType *l2caConnection  = theElement->cmL2caConnInst;
                    cmData->l2caVar.connElement         = theElement;
                    cmData->l2caVar.cancelConnect       = FALSE;
                    cmData->smVar.smInProgress          = TRUE;
                    l2caConnection->btConnId            = CM_CREATE_L2CA_CONN_ID(prim->cid);
                    l2caConnection->deviceAddr          = prim->bd_addr;

                    CSR_BT_CM_STATE_CHANGE(l2caConnection->state, l2capConnectAccept_s);

                    /* Calculate default, optimal MTU and MPS values */
                    csrBtCmL2caOptimalMtuMps(cmData, l2caConnection,
                                             &l2caConnection->incomingMtu,
                                             &l2caConnection->incomingMps,
                                             &l2caConnection->outgoingMtu);

                    /* Make room for more conftab entries and adjust missing entries */
                    CsrBtCmL2caConftabEnlarge(&l2caConnection->conftabIter.block, &l2caConnection->conftabIter.size);
                    csrBtCmL2caFixConftabFlowControl(cmData, l2caConnection, &l2caConnection->rxQueueMax);
                    csrBtCmL2caFixConftabMtu(cmData, l2caConnection);
                    csrBtCmL2caFixConftabFlush(cmData, l2caConnection);
                    csrBtCmL2caFixConftabExtendedFeatures(cmData, l2caConnection, FALSE);
                    csrBtCmL2caFixConftabDataPriority(cmData, l2caConnection);
                    CsrBtCmL2caConftabCull(&l2caConnection->conftabIter.block, &l2caConnection->conftabIter.size);
                    /* Make sure that the remote L2CAP extended features gets set correctly * /
                    CsrBtCmL2caConftabEnlarge(&l2caConnection->conftabIter.block, &l2caConnection->conftabIter.size);
                    csrBtCmL2caFixConftabExtendedFeatures(cmData, l2caConnection, FALSE);
                    CsrBtCmL2caConftabCull(&l2caConnection->conftabIter.block, &l2caConnection->conftabIter.size);
                    */
                    /* Conftab already adjusted (at connectacceptreq time), so
                     * simply send our answer to L2CAP */
                    L2CA_AutoConnectRsp(prim->identifier,
                                        prim->cid,
                                        L2CA_CONNECT_SUCCESS,
                                        CM_L2CA_CONNECT_INPROGRESS_CTX, 
                                        l2caConnection->conftabIter.size,
                                        CsrMemDup(l2caConnection->conftabIter.block,
                                                  l2caConnection->conftabIter.size*sizeof(CsrUint16)));
                    /* Check if this connection is autorised */
                    if (CsrBtBdAddrEq(&cmData->scVar.deviceAddr,&prim->bd_addr) && (cmData->scVar.psm == prim->psm_local))
                    {
                        l2caConnection->authorised = TRUE;
                        CsrBtCmScCleanupVar(cmData);
                    }
                }
                else
                { /* No applications are ready to accept a new L2CAP connection, 
                     it is therefor refused */
                    csrBtCmL2caAutoConnectIndRejectHandler(cmData, prim->cid, prim->identifier);
                }

            }
            else
            { /* Remote device asks for an AMP connection. We don't support
                 create channel at the moment, so refuse it */
                csrBtCmL2caAutoConnectIndRejectHandler(cmData, prim->cid, prim->identifier);
            }
        }
    }
}

void CsrBtCmL2caConnectCancelCleanup(cmInstanceData_t *cmData, cmL2caConnElement * theElement, CsrBool restoreDm)
{
    cmL2caConnInstType *l2caConnection  = theElement->cmL2caConnInst;
    CsrUint8 numConnections         = CsrBtCmReturnNumOfConnectionsToPeerDevice(cmData, l2caConnection->deviceAddr);

    if(numConnections == 0)
    {
        /* There is no other connection attach to the device address.
         * Inform the application, restore the local service manager
         * queue, and clear up the l2cap connection table */
        CsrBtCmL2caConnectCfmMsgHandler(cmData, theElement,
            CSR_BT_RESULT_CODE_CM_CANCELLED, CSR_BT_SUPPLIER_CM);
        if (restoreDm)
        {
            CsrBtCmDmLocalQueueHandler();
        }
    }
    else
    {
        /* Currently there is at least one more connection attach to
         * the device address. Ensure that SNIFF and park is
         * supported */
        cmData->smVar.resultCode        = CSR_BT_RESULT_CODE_CM_CANCELLED;
        cmData->smVar.resultSupplier    = CSR_BT_SUPPLIER_CM;
        CsrBtCmDmWriteLpSettingsAllMsgSend(cmData, l2caConnection->deviceAddr, KEEP_CURRENT_PLAYER);
        CSR_BT_CM_STATE_CHANGE(l2caConnection->state, l2capConnect_s);
    }
}

#endif /* #ifndef EXCLUDE_CSR_BT_L2CA_MODULE */

