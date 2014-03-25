/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_RFC_MODULE

#include "csr_bt_cm_main.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_cm_l2cap.h"
#include "csr_bt_cm_rfc.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_sdc.h"
#include "csr_bt_cm_util.h"


static void csrBtCmRfcReleaseWhileConnected(cmInstanceData_t *cmData, cmRfcConnElement *theElement, RFC_RESPONSE_T reason)
{ /* The remote device has release the connection. Inform the
     application and check if another link policy is demand                         */
    cmRfcConnInstType   *theLogicalLink = theElement->cmRfcConnInst;
    CSR_BT_CM_STATE_CHANGE(theLogicalLink->state, idle_s);
    CsrBtCmModeChangeReqMsgSend(cmData, theLogicalLink->deviceAddr);
    CsrBtCmDmSyncClearPcmSlotFromTable(cmData, &theLogicalLink->eScoParms);
    CsrBtCmDisconnectIndMsgCleanupSend(theElement, (CsrBtReasonCode) reason,
                                       CSR_BT_SUPPLIER_RFCOMM, TRUE, FALSE);
}

static CsrBool csrBtCmRfcReleaseWhileRelease(cmInstanceData_t *cmData, cmRfcConnElement *theElement, RFC_RESPONSE_T reason)
{ /* The local device has release the RFC connection.                               */
    CsrBool status;
    cmRfcConnInstType   *theLogicalLink = theElement->cmRfcConnInst;
    CsrUint8    numOfConnection         = CsrBtCmReturnNumOfConnectionsToPeerDevice(cmData, theLogicalLink->deviceAddr);

    if (CsrBtCmRfcReleaseStatus((CsrBtReasonCode) reason, CSR_BT_SUPPLIER_RFCOMM))
    {
        status = TRUE;
    }
    else
    {
        status = FALSE;
    }

    if(numOfConnection == 0)
    { /* Currently there is no other connections attach to the given Bluetooth
         address. Send disconnect ind to the application, clean up the l2cap
        connection table, and restore the local service manager queue               */
        CsrBtCmDmSyncClearPcmSlotFromTable(cmData, &theLogicalLink->eScoParms);
        CsrBtCmDisconnectIndMsgCleanupSend(theElement,
                                           (CsrBtReasonCode) reason,
                                           CSR_BT_SUPPLIER_RFCOMM,
                                           status,
                                           TRUE);
        CsrBtCmServiceManagerLocalQueueHandler(cmData);
    }
    else
    { /* Currently there is at least one more connection attach to the given device
         address. Ensure that the SNIFF and PARK mode is supported                  */
        cmData->smVar.resultCode        = (CsrBtResultCode) reason;
        cmData->smVar.resultSupplier    = CSR_BT_SUPPLIER_RFCOMM;
        CSR_BT_CM_STATE_CHANGE(theLogicalLink->state, releaseFinal_s);
        CsrBtCmDmWriteLpSettingsReqMsgSend(theLogicalLink->deviceAddr, RFC_PLAYER);
    }
    return (status);
}


static void csrBtCmDisconnectIndMsgSend(CsrSchedQid                 appHandle,
                                        CsrBtConnId            btConnId,
                                        CsrBtReasonCode     reasonCode,
                                        CsrBtSupplier reasonSupplier,
                                        CsrBool              status,
                                        CsrBool              localTerminated,
                                        CsrUint16            context)
{/* Send a CSR_BT_CM_DISCONNECT_IND signal to the application                              */
    CsrBtCmDisconnectInd     * cmPrim;
    cmPrim                  = (CsrBtCmDisconnectInd *)CsrPmemAlloc(sizeof(CsrBtCmDisconnectInd));
    cmPrim->type            = CSR_BT_CM_DISCONNECT_IND;
    cmPrim->btConnId        = btConnId;
    cmPrim->reasonCode      = reasonCode;
    cmPrim->reasonSupplier  = reasonSupplier;
    cmPrim->status          = status;
    cmPrim->localTerminated = localTerminated;
    cmPrim->context         = context;
    CsrBtCmPutMessage(appHandle, cmPrim);
}

CsrBool CsrBtCmRfcReleaseStatus(CsrBtReasonCode     reasonCode,
                               CsrBtSupplier reasonSupplier)
{

    if (reasonSupplier == CSR_BT_SUPPLIER_RFCOMM)
    {
        RFC_RESPONSE_T reason = (RFC_RESPONSE_T) reasonCode;

        if (reason == RFC_RES_ACK_TIMEOUT)
        {
            return FALSE;
        }
    }
    return TRUE;
}

void CsrBtCmDisconnectIndMsgCleanupSend(cmRfcConnElement    *theElement,
                                        CsrBtReasonCode     reasonCode,
                                        CsrBtSupplier reasonSupplier,
                                        CsrBool              status,
                                        CsrBool              localTerminated)
{/* Send a CSR_BT_CM_DISCONNECT_IND signal to the application, and clean up the
    connection table                                                                */
    cmRfcConnInstType * theLogicalLink = theElement->cmRfcConnInst;

    csrBtCmDisconnectIndMsgSend(theLogicalLink->appHandle,
                                theLogicalLink->btConnId,
                                reasonCode, reasonSupplier,
                                status, localTerminated,
                                theLogicalLink->context);

    if (status)
    {
        if(theLogicalLink->remoteServerChan != CSR_BT_NO_SERVER)
        { /* The local device has initiate the connection, deregistered outgoing    */
            dm_sm_unregister_outgoing_req(CSR_BT_CM_IFACEQUEUE,
                                          0, /* context */
                                          &theLogicalLink->deviceAddr,
                                          SEC_PROTOCOL_RFCOMM,
                                          theLogicalLink->remoteServerChan,
                                          NULL);
        }
        else
        { /* The remote device has initiate the connection, deregistered incoming   */
            CsrBtScDeregisterReqSend(SEC_PROTOCOL_RFCOMM, theLogicalLink->serverChannel);
        }
        cleanUpConnectionTable(&(theElement->cmRfcConnInst));
    }
    else
    {
        if (theLogicalLink->state == release_s ||
            theLogicalLink->state == releaseFinal_s)
        {
            CSR_BT_CM_STATE_CHANGE(theLogicalLink->state, connected_s);
        }
    }
}

void CsrBtCmRfcReleaseReqHandler(cmInstanceData_t *cmData)
{ /* The application wishes to close down the data link connection                  */
    CsrBtCmDisconnectReq * cmPrim  = (CsrBtCmDisconnectReq *) cmData->recvMsgP;
    cmRfcConnElement * theElement    = CM_FIND_RFC_ELEMENT(CsrBtCmRfcFindRfcConnElementFromBtConnId, &(cmPrim->btConnId));

    if(theElement)
    { /* The CM has found a valid index in the connection table                     */
        cmRfcConnInstType *theLogicalLink   = theElement->cmRfcConnInst;

        if(theLogicalLink->state == connected_s)
        { /* The profile has a know data link connection. Before the CM can release
             it, the CM must insure that the host don't change the link policy.
             In order to do this it change the link policy is change to CSR_BT_ACTIVE_MODE
             and  change DM_HCI_WRITE_LINK_POLICY_SETTINGS_REQ to accept ONLY_ACTIVE             */
            CSR_BT_CM_STATE_CHANGE(theLogicalLink->state, releaseInit_s);
            cmData->rfcVar.connElement   = theElement;
            CsrBtCmDmModeChangeReqMsgSend(RFC_PLAYER, theElement->elementId, CSR_BT_ACTIVE_MODE, theLogicalLink->deviceAddr);
        }
        else
        { /* The profile try to disconnect a connection that is not up running.
             Build and send CSR_BT_CM_DISCONNECT_IND with ERROR                            */
            csrBtCmDisconnectIndMsgSend(theLogicalLink->appHandle,
                                        cmPrim->btConnId,
                                        (CsrBtReasonCode) CSR_BT_RESULT_CODE_CM_UNKNOWN_CONNECTION_IDENTIFIER,
                                        CSR_BT_SUPPLIER_CM,
                                        TRUE, TRUE, cmPrim->context);

            CsrBtCmServiceManagerLocalQueueHandler(cmData);
        }
    }
    else
    { /* The CM can not find the conection id and server in the connection table. Ignore
         the request                                                                */
        CsrBtCmServiceManagerLocalQueueHandler(cmData);
    }
}

static void csrBtCmRfcCommonReleaseConnectSHandler(cmInstanceData_t     *cmData,
                                                   cmRfcConnInstType    * theLogicalLink,
                                                   RFC_RESPONSE_T        reason)
{
    if (cmData->rfcVar.cancelConnect)
    { /* The rfcomm connection has been release because the application
         has issued a cancel connect                                                */
        cmData->smVar.resultCode     = CSR_BT_RESULT_CODE_CM_CANCELLED;
        cmData->smVar.resultSupplier = CSR_BT_SUPPLIER_CM;
    }
    else
    {
        cmData->smVar.resultCode     = (CsrBtResultCode) reason;
        cmData->smVar.resultSupplier = CSR_BT_SUPPLIER_RFCOMM;
    }
    CsrBtCmRfcCommonErrorHandler(cmData, theLogicalLink);
}

void CsrBtCmRfcReleaseIndHandler(cmInstanceData_t *cmData)
{ /* This event is an indication that the data link connection referenced
     by the connection id has closed down.                                          */
    RFC_DISCONNECT_IND_T   * rfcPrim = (RFC_DISCONNECT_IND_T *) cmData->recvMsgP;
    cmRfcConnElement * theElement;
    CsrBtConnId        value = CM_CREATE_RFC_CONN_ID(rfcPrim->conn_id);
    theElement = CM_FIND_RFC_ELEMENT(CsrBtCmRfcFindRfcConnElementFromBtConnId, &(value));

    if(theElement)
    { /* The CM has found a valid index in the connection table                     */
        cmRfcConnInstType *theLogicalLink   = theElement->cmRfcConnInst;

        switch(theLogicalLink->state)
        {
            case connected_s :
            { /* The remote device has release the connection. Inform the
                 application and check if another link policy is demand             */
                csrBtCmRfcReleaseWhileConnected(cmData, theElement, rfcPrim->reason);
                break;
            }
            case release_s :
            { /* The local device has release the RFC connection.                   */
                if (!csrBtCmRfcReleaseWhileRelease(cmData, theElement, rfcPrim->reason))
                {
                    ;
                }
                break;
            }
            case releaseFinal_s:
            { /* Either the RFC_RELEASE_REQ and RFC_RELEASE_IND is crossing, or
                 the first RFC_RELEASE_IND did report an error                      */
                if (!CsrBtCmRfcReleaseStatus((CsrBtReasonCode) cmData->smVar.resultCode, cmData->smVar.resultSupplier))
                { /* A RFC_RELEASE_IND is allready received with an error.          */ 
                    if (CsrBtCmRfcReleaseStatus((CsrBtReasonCode) rfcPrim->reason, CSR_BT_SUPPLIER_RFCOMM))
                    { /* This RFC_RELEASE_IND message is a success.                 */
                        cmData->smVar.resultCode        = (CsrBtResultCode) rfcPrim->reason;
                        cmData->smVar.resultSupplier    = CSR_BT_SUPPLIER_RFCOMM;
                    }
                    else
                    { /* This one also report an error                              */
                        ;
                    }
                }
                else
                { /* Just ignore it, an succesful RFC_RELEASE_IND is already received.
                     E.g a RFC_RELEASE_REQ and RFC_RELEASE_IND is crossing. Set 
                     releaseReason to RFC_RELEASE_FAIL to make sure that Rfc close 
                     is not call twice                                              */ 
                    ;
                }
                break;
            }
            case connect_s :
            { /* Build and send CSR_BT_CM_CONNECT_CFM to the profile.
                 The connection is release while connecting                         */
                csrBtCmRfcCommonReleaseConnectSHandler(cmData, theLogicalLink, rfcPrim->reason);
                break;
            }
            case connectAccept_s :
            { /* The connection is release while accepting a connection             */
                cmData->smVar.resultCode      = (CsrBtResultCode) rfcPrim->reason;
                cmData->smVar.resultSupplier  = CSR_BT_SUPPLIER_RFCOMM;
                theLogicalLink->btConnId      = CSR_BT_CONN_ID_INVALID;
                cmData->rfcVar.btConnId      = CSR_BT_CONN_ID_INVALID;
                CsrBtCmWriteClassOfDevice(RFC_PLAYER);
                break;
            }
            case connectAcceptFinal_s :
            { /* The connection is release while accepting a connection             */
                cmData->smVar.resultCode      = (CsrBtResultCode) rfcPrim->reason;
                cmData->smVar.resultSupplier  = CSR_BT_SUPPLIER_RFCOMM;
                theLogicalLink->btConnId      = CSR_BT_CONN_ID_INVALID;
                cmData->rfcVar.btConnId      = CSR_BT_CONN_ID_INVALID;
                break;
            }
            case releaseInit_s :
            { /* Build and send CSR_BT_CM_DISCONNECT_IND to the profile.                   */
                CsrBtCmDmSyncClearPcmSlotFromTable(cmData, &theLogicalLink->eScoParms);

                CsrBtCmDisconnectIndMsgCleanupSend(theElement,
                                                   (CsrBtReasonCode) rfcPrim->reason,
                                                           CSR_BT_SUPPLIER_RFCOMM,
                                                           TRUE, TRUE);
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
            }
}

#endif /* #ifndef EXCLUDE_CSR_BT_RFC_MODULE */
