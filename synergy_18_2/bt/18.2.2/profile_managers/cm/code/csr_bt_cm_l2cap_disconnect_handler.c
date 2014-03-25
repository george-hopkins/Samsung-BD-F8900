/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE

#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_cm_l2cap.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_util.h"
#include "csr_bt_sc_private_lib.h"

static void csrBtCmL2CaDeactivateQosSetupRequestHandler(cmInstanceData_t *cmData,
                                                        CsrBtDeviceAddr deviceAddr)
{
    aclTable   * aclConnectionElement = NULL;
    returnAclConnectionElement(cmData, deviceAddr, &aclConnectionElement);

    if (aclConnectionElement)
    {
        cmL2caConnElement * theElement = CM_FIND_L2CA_ELEMENT(CsrBtCmL2caFindL2caPriorityDataChannel, &(deviceAddr));

        if (!theElement)
        {
            /* All high data priority data channels are gone */
            dm_hci_qos_setup_req(&(deviceAddr),
                                 0,
                                 HCI_QOS_BEST_EFFORT,
                                 CSR_BT_CM_DEFAULT_QOS_TOKEN_RATE,
                                 CSR_BT_CM_DEFAULT_QOS_PEAK_BANDWIDTH,
                                 CSR_BT_CM_DEFAULT_QOS_LATENCY,
                                 CSR_BT_CM_DEFAULT_QOS_DELAY_VARIATION,
                                 NULL);
        }
    }
}

static void csrBtCmL2caDisconnectHandler(cmInstanceData_t   *cmData,
                                         CsrBool            localTerminated,
                                         l2ca_cid_t         cid,
                                         CsrBtReasonCode    reason)
{
    CsrBtConnId btConnId            = CM_CREATE_L2CA_CONN_ID(cid);
    cmL2caConnElement * theElement  = CM_FIND_L2CA_ELEMENT(CsrBtCmL2caFindL2caConnElementFromBtConnId, &(btConnId));

    if (theElement)
    {    
        cmL2caConnInstType *l2CaConnection = theElement->cmL2caConnInst;
        CsrBtDeviceAddr    remoteAddr      = l2CaConnection->deviceAddr;

        switch(l2CaConnection->state)
        {
            case l2capConnected_s :
            {/* The peer device has requested that the L2CAP is release or the CM
                has release it direct. Accept this request, clear the l2cap 
                connection table and notify the application, Note always 
                consider as a remote disconnect */
                CSR_BT_CM_STATE_CHANGE(l2CaConnection->state, idle_s);
                CsrBtCmModeChangeReqMsgSend(cmData, l2CaConnection->deviceAddr);
                CsrBtCmL2capDisconnectIndMsgSend(cmData, 
                                                 theElement, 
                                                 reason, 
                                                 CSR_BT_SUPPLIER_L2CAP_DISCONNECT,
                                                 FALSE);

                CsrBtCmWriteAutoFlushTimeout(cmData, &remoteAddr);
                break;
            }
            case l2capRelease_s :
            {
                CsrBtCmL2caFinalReleaseHandler(cmData, 
                                               theElement, 
                                               localTerminated,
                                               reason, 
                                               CSR_BT_SUPPLIER_L2CAP_DISCONNECT, 
                                               localTerminated);
                break;
            }
            case l2capConnect_s :
            { /* The  L2CAP is release while trying to establish it. Make sure that
                 the local device accept SNIFF and PARK before notifying the
                 application and cleaning up the l2cap connection table */
                CsrUint8 numOfConnection = CsrBtCmReturnNumOfConnectionsToPeerDevice(cmData, 
                                                                                     l2CaConnection->deviceAddr);

                if (!cmData->l2caVar.cancelConnect)
                {
                    cmData->smVar.resultCode        = reason;
                    cmData->smVar.resultSupplier    = CSR_BT_SUPPLIER_L2CAP_DISCONNECT;
                }
                else
                {
                    cmData->smVar.resultCode        = CSR_BT_RESULT_CODE_CM_CANCELLED;
                    cmData->smVar.resultSupplier    = CSR_BT_SUPPLIER_CM;
                }

                if(numOfConnection == 0)
                { /* There is no other connection attach to the device address.
                     Inform the application, restore the local service manager
                     queue, and clear up the l2cap connection table */
                    CsrBtCmL2caConnectCfmMsgHandler(cmData, 
                                                    theElement, 
                                                    cmData->smVar.resultCode, 
                                                    cmData->smVar.resultSupplier);
                }
                else
                { /* Currently there is at least one more connection attach to the
                     device address. Ensure that SNIFF and park is supported */
                    CsrBtCmDmWriteLpSettingsReqMsgSend(l2CaConnection->deviceAddr, L2CAP_PLAYER);
                }
                break;
            }
            case l2capConnectAccept_s :
            { /* The L2CAP connection is release while accepting a connection. 
                 Keep the link discoverable and restore the local service manager 
                 and DM queue */
                CsrBtCmL2capAcceptFailClearing(cmData, l2CaConnection);
                if (localTerminated)
                {
                    CsrBtCmWriteAutoFlushTimeout(cmData, &remoteAddr);
                }
                CsrBtCmServiceManagerLocalQueueHandler(cmData);
                break;
            }
            case l2capSaveDisconnect_s :
            { /* A disconnect request has been restored from the service manager
                 queue. Accept the release request clear the l2cap connection
                 table and notify the application. Note always consider as a 
                 remote disconnect */
                CsrBtCmL2capDisconnectIndMsgSend(cmData, theElement, 
                                                 reason, 
                                                 CSR_BT_SUPPLIER_L2CAP_DISCONNECT,
                                                 FALSE);
                
                CsrBtCmWriteAutoFlushTimeout(cmData, &remoteAddr);
                CsrBtCmServiceManagerLocalQueueHandler(cmData);
                break;
            }
            case l2capConnectAcceptFinal_s :
            { /* The L2CAP connection is release while removing the COD or
                 making the device unconnectable, save this message 
                 until this process is finish. Note when this process is
                 finish and this message is restored, the state will always be
                 l2capConnected_s                                                   */
                CSR_BT_CM_STATE_CHANGE(l2CaConnection->state, l2capSaveDisconnect_s);
                CsrMessageQueuePush(&cmData->smVar.SaveQueue, L2CAP_PRIM, cmData->recvMsgP);
                cmData->recvMsgP    = NULL;
                break;
            }
            case l2capReleaseInit_s :
            { /* Crossover between local and remote disconnect.                     */
                cmData->smVar.resultCode        = reason;
                cmData->smVar.resultSupplier    = CSR_BT_SUPPLIER_L2CAP_DISCONNECT;
                CSR_BT_CM_STATE_CHANGE(l2CaConnection->state, l2capReleaseInitXover_s);
                break;
            }
            case l2capReleaseFinal_s :
            { /* Just Ignore it.CM has already receive a a L2CA_DISCONNECT_CFM/IND */
                break;
            }
            default:
            {
                break;
            }
        }
    }
    else
    { /* No owner just ignore */
        ;
    }
}

void CsrBtCmL2caFinalReleaseHandler(cmInstanceData_t    *cmData, 
                                    cmL2caConnElement   *theElement, 
                                    CsrBool              flush,
                                    CsrBtReasonCode     reasonCode,
                                    CsrBtSupplier reasonSupplier,
                                    CsrBool              localTerminated)
{
    cmL2caConnInstType *l2CaConnection  = theElement->cmL2caConnInst;
    CsrUint8             numOfConnection = CsrBtCmReturnNumOfConnectionsToPeerDevice(cmData, l2CaConnection->deviceAddr);

    if(numOfConnection == 0)
    { /* Currently there is no other connections attach to the
         given Bluetooth address. Send disconnect ind to the
         application, clean up the l2cap connection table, and
         restore the local service manager queue */
        CsrBtDeviceAddr remoteAddr = l2CaConnection->deviceAddr;

        CsrBtCmL2capDisconnectIndMsgSend(cmData, theElement, reasonCode, reasonSupplier, localTerminated);

        if (flush)
        {
            CsrBtCmWriteAutoFlushTimeout(cmData, &remoteAddr);
        }
        CsrBtCmServiceManagerLocalQueueHandler(cmData);
    }
    else
    { /* Currently there is at least one more connection attach
         to the given device address. Ensure that the SNIFF and
         PARK mode is supported */
        CSR_BT_CM_STATE_CHANGE(l2CaConnection->state, l2capReleaseFinal_s);
        cmData->smVar.resultCode        = reasonCode;
        cmData->smVar.resultSupplier    = reasonSupplier;
        CsrBtCmDmWriteLpSettingsReqMsgSend(l2CaConnection->deviceAddr, L2CAP_PLAYER);
    }
}

void CsrBtCmL2capDisconnectIndMsgSend(cmInstanceData_t      *cmData,
                                      cmL2caConnElement     *theElement,
                                      CsrBtReasonCode       reasonCode,
                                      CsrBtSupplier   reasonSupplier,
                                      CsrBool                localTerminated)
{
    
    cmL2caConnInstType *l2CaConnection = theElement->cmL2caConnInst;
    CsrBtCmL2caDisconnectInd *prim  = (CsrBtCmL2caDisconnectInd *) CsrPmemAlloc(sizeof(CsrBtCmL2caDisconnectInd));

    prim->type              = CSR_BT_CM_L2CA_DISCONNECT_IND;
    prim->btConnId          = l2CaConnection->btConnId;
    prim->reasonCode        = reasonCode;
    prim->reasonSupplier    = reasonSupplier;
    prim->localTerminated   = localTerminated;
    prim->context           = l2CaConnection->context;
    CsrBtCmPutMessage(l2CaConnection->appHandle, prim);

    if (reasonCode == CSR_BT_RESULT_CODE_CM_UNKNOWN_CONNECTION_IDENTIFIER &&
        reasonSupplier == CSR_BT_SUPPLIER_CM)
    {
        ;
    }
    else
    {
        CsrUint8        numOfOutgoing;
        CsrUint8        numOfIncoming;
        CsrBtDeviceAddr deviceAddr;

        numberOfSecurityRegister(cmData, l2CaConnection->psm, l2CaConnection->deviceAddr, &numOfOutgoing, &numOfIncoming);

        if(l2CaConnection->remotePsm != NO_REMOTE_PSM)
        { /* The local device has initiate the connection, deregistered outgoing */
            if (numOfOutgoing == 1)
            { /* Unregister The Outgoing security setting */
                dm_sm_unregister_outgoing_req(CSR_BT_CM_IFACEQUEUE,
                                              0, /* context */
                                              &l2CaConnection->deviceAddr,
                                              SEC_PROTOCOL_L2CAP,
                                              l2CaConnection->remotePsm,
                                              NULL);
            }
            else
            { /* There is more that need this security setting */
                ;
            }
        }
        else
        { /* The remote device has initiate the connection, deregistered incoming */
            if (numOfIncoming == 1)
            { /* Unregister The Incoming security setting */
                CsrBtScDeregisterReqSend(SEC_PROTOCOL_L2CAP, l2CaConnection->psm);
            }
            else
            { /* There is more that need this security setting */
                ;
            }
        }
        if (l2CaConnection->dataPriority != CSR_BT_CM_PRIORITY_NORMAL)
        {
            deviceAddr = l2CaConnection->deviceAddr;
        }
        else
        {
            bd_addr_zero(&deviceAddr);
        }
        CsrBtCmL2capClearL2capTableIndex(cmData, &(theElement->cmL2caConnInst));

        if (!CsrBtBdAddrEqZero(&deviceAddr))
        {
            csrBtCmL2CaDeactivateQosSetupRequestHandler(cmData, deviceAddr);
        }
    }
}

void CsrBtCmL2caDisconnectReqHandler(cmInstanceData_t *cmData)
{ /* Request to disconnect L2CAP channel */
    CsrBtCmL2caDisconnectReq    * cmPrim = (CsrBtCmL2caDisconnectReq *) cmData->recvMsgP;
    cmL2caConnElement * theElement = CM_FIND_L2CA_ELEMENT(CsrBtCmL2caFindL2caConnElementFromBtConnId, &(cmPrim->btConnId));

    if (theElement)
    {
        cmL2caConnInstType *l2CaConnection = theElement->cmL2caConnInst;
        cmData->l2caVar.connElement        = theElement;

        if(l2CaConnection->state == l2capConnected_s)
        { /* The profile has a know l2cap connection. Before the CM can release
             the l2cap connection, it must insure that the host don't change
             the link policy. */
            CSR_BT_CM_STATE_CHANGE(l2CaConnection->state, l2capReleaseInit_s);
            CsrBtCmDmModeChangeReqMsgSend(L2CAP_PLAYER, theElement->elementId, CSR_BT_ACTIVE_MODE, l2CaConnection->deviceAddr);
        }
        else
        { /* The profile try to disconnect a connection that is not up running.
             Build and send CSR_BT_CM_L2CA_DISCONNECT_IND with ERROR, and restore the
             local service manager queue */
            theElement->cmL2caConnInst->context = cmPrim->context;
            CsrBtCmL2capDisconnectIndMsgSend(cmData, theElement,
                                             (CsrBtReasonCode) CSR_BT_RESULT_CODE_CM_UNKNOWN_CONNECTION_IDENTIFIER,
                                             CSR_BT_SUPPLIER_CM, TRUE);

            CsrBtCmServiceManagerLocalQueueHandler(cmData);
        }
    }
    else
    { /* Just resolve the local service manager queue */
        CsrBtCmServiceManagerLocalQueueHandler(cmData);
    }
}

void CsrBtCmL2caDisconnectCfmHandler(cmInstanceData_t *cmData)
{ /* Confirmation of request to disconnect L2CAP channel, and restore the local
     L2CAP queue */
    L2CA_DISCONNECT_CFM_T   * prim = (L2CA_DISCONNECT_CFM_T *) cmData->recvMsgP;
    csrBtCmL2caDisconnectHandler(cmData, TRUE, prim->cid, (CsrBtReasonCode) prim->result);
}

void CsrBtCmL2caDisconnectIndHandler(cmInstanceData_t *cmData)
{ /* Indication of request to disconnect L2CAP channel. Send a Respond back
     to accept this demand, and inform the application */
    L2CA_DISCONNECT_IND_T * prim = (L2CA_DISCONNECT_IND_T *) cmData->recvMsgP;

    csrBtCmL2caDisconnectHandler(cmData, FALSE, prim->cid, (CsrBtReasonCode) prim->reason);

    if (cmData->recvMsgP)
    { /* The CM has not save the L2CA_DISCONNECT_IND message. Accept the release
        request initiated from a peer device right away                            */
        L2CA_DisconnectRsp(prim->identifier, prim->cid);
    }
    else
    { /* The CM has save L2CA_DISCONNECT_IND. Wait to send the response message     */
        ;
    }
}

#endif /* #ifndef EXCLUDE_CSR_BT_L2CA_MODULE */
