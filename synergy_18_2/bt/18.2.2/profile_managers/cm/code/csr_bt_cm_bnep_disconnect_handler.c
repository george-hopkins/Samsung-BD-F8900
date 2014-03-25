/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE

#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_cm_bnep.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_util.h"
#include "csr_bt_cm_events_handler.h"

void CsrBtCmBnepDisconnectIndMsgSend(CsrUint16 theId, CsrSchedQid appHandle,
                                     CsrBtReasonCode reasonCode,
                                     CsrBtSupplier reasonSupplier)
{
    CsrBtCmBnepDisconnectInd    *prim;

    prim                    = (CsrBtCmBnepDisconnectInd *) CsrPmemAlloc(sizeof(CsrBtCmBnepDisconnectInd));
    prim->type              = CSR_BT_CM_BNEP_DISCONNECT_IND;
    prim->id                = theId;
    prim->reasonCode        = reasonCode;
    prim->reasonSupplier    = reasonSupplier;
    CsrBtCmPutMessage(appHandle, prim);
}

/*************************************************************************************
  CsrBtCmBnepDisconnectReqHandler
************************************************************************************/
void CsrBtCmBnepDisconnectReqHandler(cmInstanceData_t *cmData)
{ /* Request to disconnect BNEP connection */
    CsrUint8                        theIndex;
    CsrBtCmBnepDisconnectReq    * cmPrim;

    cmPrim        = (CsrBtCmBnepDisconnectReq *) cmData->recvMsgP;
    theIndex    = returnBnepIdIndex(cmData, cmPrim->id);

    if(theIndex != CM_ERROR)
    {
        bnepTable * bnepConnection;

        cmData->bnepVar.indexPtr        = &(cmData->bnepVar.connectVar[theIndex]);
        bnepConnection                    = cmData->bnepVar.indexPtr;

        if(bnepConnection->state == bnepConnected_s)
        { /* The profile has a know bnep connection. Before the CM can release
             the bnep connection, it must insure that the host don't change
             the link policy. */
            CSR_BT_CM_STATE_CHANGE(bnepConnection->state, bnepDisconnectInit_s);
            CsrBtCmDmModeChangeReqMsgSend(BNEP_PLAYER, theIndex, CSR_BT_ACTIVE_MODE, bnepConnection->deviceAddr);
        }
        else
        { /* The profile try to disconnect a connection that is not up running.
             Build and send CSR_BT_CM_BNEP_DISCONNECT_IND with ERROR, and restore the
             local service manager queue */
            CsrBtCmBnepDisconnectIndMsgSend(cmPrim->id,
                                            cmData->bnepVar.appHandle,
                                            CSR_BT_RESULT_CODE_CM_UNKNOWN_CONNECTION_IDENTIFIER,
                                            CSR_BT_SUPPLIER_CM);

            CsrBtCmServiceManagerLocalQueueHandler(cmData);
        }
    }
    else
    { /* Just restore the local service manager queue */
        CsrBtCmServiceManagerLocalQueueHandler(cmData);
    }
}

/*************************************************************************************
  CsrBtCmBnepDisconnectIndHandler
************************************************************************************/
void CsrBtCmBnepDisconnectIndHandler(cmInstanceData_t *cmData)
{ /* Indication of request to disconnect BNEP connection. Send a Respond back
     to accept this demand, and inform the application */
    CsrUint8                        theIndex;
    BNEP_DISCONNECT_IND_T        * prim;

    prim        = (BNEP_DISCONNECT_IND_T *) cmData->recvMsgP;
    theIndex    = returnBnepIdIndex(cmData, prim->id);

    if(theIndex != CM_ERROR)
    { /* Inform the application and bnep connection table is cleared */
        bnepTable *bnepConnection;

        bnepConnection = &(cmData->bnepVar.connectVar[theIndex]);
        bnepConnection->logicalChannelTypeMask = CSR_BT_NO_ACTIVE_LOGICAL_CHANNEL;

        switch(cmData->bnepVar.connectVar[theIndex].state)
        {
            case bnepConnected_s :
            {/* The peer device has requested that the BNEP is release. Accept
                this request, clear the bnep connection table and notify the
                application */
                CsrUint8       numOfConnection;

                CSR_BT_CM_STATE_CHANGE(bnepConnection->state, bnepDisconnectInd_s);
                CsrBtCmBnepDisconnectIndMsgSend(prim->id,
                                                cmData->bnepVar.appHandle,
                                                (CsrBtReasonCode) prim->reason,
                                                CSR_BT_SUPPLIER_BNEP);

                numOfConnection        = CsrBtCmReturnNumOfConnectionsToPeerDevice(cmData, bnepConnection->deviceAddr);

                if(numOfConnection == 0)
                { /* Currently there is no other connections attach to the given Bluetooth
                     address. Send disconnect ind to the application, clean up the bnep
                     connection table. */
                    CsrBtCmBnepClearBnepTableIndex(bnepConnection);
                }
                else
                { /* Currently there is at least one more connection attach to the given device
                     address. Ensure that the SNIFF and PARK mode is supported */
                    cmData->bnepVar.moreLpSettingsInProgress = FALSE;
                    CsrBtCmDmWriteLpSettingsReqMsgSend(bnepConnection->deviceAddr, BNEP_PLAYER);

                }
                break;
            }
            case bnepConnect_s :
            case bnepConnectCfm_s :
            {

                if (!cmData->bnepVar.cancelConnect)
                {
                    /* The peer device has requested that the BNEP is release while
                     trying to establish it. Accept this request, and make sure that
                     the local device accept SNIFF and PARK before notifying the
                     application and cleaning up the bnep connection table         */
                    CSR_BT_CM_STATE_CHANGE(bnepConnection->state, bnepDisconnectReq_s);
                    CsrBtCmBnepDisconnectIndMsgSend(prim->id,
                                                    cmData->bnepVar.appHandle,
                                                    (CsrBtReasonCode) prim->reason,
                                                    CSR_BT_SUPPLIER_BNEP);
                }
                else
                {
                    CsrUint8 numOfConnection = CsrBtCmReturnNumOfConnectionsToPeerDevice(cmData, bnepConnection->deviceAddr);

                    CsrBtCmBnepConnectIndMsgSend(cmData, cmData->bnepVar.appHandle, ID_EMPTY,
                                                 CsrBtBdAddrToEtherAddr(&bnepConnection->deviceAddr),
                                                 0, 0, 0,
                                                 CSR_BT_RESULT_CODE_CM_CANCELLED,
                                                 CSR_BT_SUPPLIER_CM);

                    if(numOfConnection == 0)
                    { /* Currently there is no other connections attach to the given Bluetooth
                         address. Send disconnect ind to the application, clean up the bnep
                         connection table, and restore the local service manager queue                */
                        CsrBtCmBnepClearBnepTableIndex(bnepConnection);
                        CsrBtCmServiceManagerLocalQueueHandler(cmData);
                    }
                    else
                    { /* Currently there is at least one more connection attach to the given device
                         address. Ensure that the SNIFF and PARK mode is supported                    */
                        cmData->smVar.resultCode        = CSR_BT_RESULT_CODE_CM_CANCELLED;
                        cmData->smVar.resultSupplier    = CSR_BT_SUPPLIER_CM;
                        CsrBtCmDmWriteLpSettingsReqMsgSend(bnepConnection->deviceAddr, BNEP_PLAYER);
                    }
                }
                break;
            }
            case bnepConnectAcceptFinal_s :
            { /* The peer device has requested that the BNEP connection is
                 release. This indication has occure while linkpolicychange or
                 write lp settings */
                CSR_BT_CM_STATE_CHANGE(bnepConnection->state, bnepDisconnectReq_s);
                CsrBtCmBnepDisconnectIndMsgSend(prim->id,
                                                cmData->bnepVar.appHandle,
                                                (CsrBtReasonCode) prim->reason,
                                                CSR_BT_SUPPLIER_BNEP);
                break;
            }
            case bnepDisconnectReq_s :
            {
                CsrUint8 numOfConnection;
                CsrBtCmBnepDisconnectIndMsgSend(prim->id,
                                                cmData->bnepVar.appHandle,
                                                (CsrBtReasonCode) prim->reason,
                                                CSR_BT_SUPPLIER_BNEP);

                numOfConnection        = CsrBtCmReturnNumOfConnectionsToPeerDevice(cmData, bnepConnection->deviceAddr);

                if(numOfConnection == 0)
                { /* Currently there is no other connections attach to the given Bluetooth
                     address. Send disconnect ind to the application, clean up the bnep
                     connection table, and restore the local service manager queue */
                    CsrBtCmBnepClearBnepTableIndex(bnepConnection);
                    CsrBtCmServiceManagerLocalQueueHandler(cmData);
                    CsrBtCmDmLocalQueueHandler();
                }
                else
                { /* Currently there is at least one more connection attach to the given device
                     address. Ensure that the SNIFF and PARK mode is supported */
                    CsrBtCmDmWriteLpSettingsAllMsgSend(cmData, bnepConnection->deviceAddr, BNEP_PLAYER);
                }
                break;
            }

            case bnepDisconnectInit_s :
            { /* The peer device has disconnected while the local device also was disconnecting */
                cmData->bnepVar.disconnectInd = TRUE;
                CsrBtCmBnepDisconnectIndMsgSend(prim->id,
                                                cmData->bnepVar.appHandle,
                                                (CsrBtReasonCode) prim->reason,
                                                CSR_BT_SUPPLIER_BNEP);
                break;
            }
            default :
            {
                break;
            }
        }
        /* AMP-Coexistance */
        /* Update ACL table and trigger event if needed */
        if (updateLogicalChannelTypeMaskAndNumber(cmData,&bnepConnection->deviceAddr))
        {/* Changed: issue event */
#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_CHANNEL_TYPE
            aclTable *aclConnectionElement;

            returnAclConnectionElement(cmData,bnepConnection->deviceAddr,&aclConnectionElement);

            CsrBtCmPropgateEvent(cmData,
                                CsrBtCmPropgateLogicalChannelTypeEvents,
                                CSR_BT_CM_EVENT_MASK_SUBSCRIBE_CHANNEL_TYPE,
                                HCI_SUCCESS,
                                (void *)aclConnectionElement,
                                NULL);
#endif
        }
    }
    else
    {/* Could not find the ID in the bnep connection table. Accept the release
        request initiated from a peer device */
        CsrBtBnepDisconnectReqSend(0, prim->id);
    }
}

/*************************************************************************************
  CsrBtCmBnepDisconnectResHandler
************************************************************************************/
void CsrBtCmBnepDisconnectResHandler(cmInstanceData_t *cmData)
{ /*  Response from profile on a disconnect from the peer side */
    CsrBtCmBnepDisconnectRes    * cmPrim;

    cmPrim     = (CsrBtCmBnepDisconnectRes *) cmData->recvMsgP;
    CsrBtBnepDisconnectReqSend(cmPrim->flags, cmPrim->id);
}
#endif

