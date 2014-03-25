/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_RFC_MODULE
#ifdef CSR_AMP_ENABLE

#include "csr_bt_cm_main.h"
#include "csr_bt_cm_rfc.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_cm_common_amp.h"

typedef CsrBool (*CsrBtCmRfcAmpCommonFuncType)(cmInstanceData_t    *cmData,
                                               cmRfcConnInstType   *conn,
                                               l2ca_identifier_t   identifier,
                                               l2ca_controller_t   local_control,
                                               l2ca_move_result_t  status,
                                               CsrBool             *checkFlag);

static CsrBool csrBtCmRfcAmpLinkLossHandler(cmInstanceData_t    *cmData,
                                            cmRfcConnInstType   *conn,
                                            l2ca_identifier_t   identifier,
                                            l2ca_controller_t   local_control,
                                            l2ca_move_result_t  status,
                                            CsrBool             *checkFlag)
{
    conn->moveIdentifier     = 0;
    conn->ampProcessState    = CSR_BT_CM_AMP_PROCESS_LINK_LOST_IND_PENDING;
    CsrBtCmAmpMoveChannelIndSend(cmData,
                                 conn->btConnId,
                                 CSR_BT_AMP_CONTROLLER_BREDR,
                                 conn->deviceAddr);
    CSR_UNUSED(identifier);
    CSR_UNUSED(local_control);
    CSR_UNUSED(status);
    CSR_UNUSED(checkFlag);
    return (TRUE);
}

static CsrBool CsrBtCmRfcAmpChangeToLinkLostReqPendingState(cmInstanceData_t    *cmData,
                                                            cmRfcConnInstType   *conn,
                                                            l2ca_identifier_t   identifier,
                                                            l2ca_controller_t   local_control,
                                                            l2ca_move_result_t  status,
                                                            CsrBool             *checkFlag)
{
    if (conn->ampProcessState == CSR_BT_CM_AMP_PROCESS_LINK_LOST_IND_PENDING)
    {
        conn->ampProcessState = CSR_BT_CM_AMP_PROCESS_LINK_LOST_REQ_PENDING;

    }
    CSR_UNUSED(identifier);
    CSR_UNUSED(local_control);
    CSR_UNUSED(status);
    CSR_UNUSED(checkFlag);
    return (TRUE);
}

static CsrBool CsrBtCmRfcAmpRfcReleaseAll(cmInstanceData_t    *cmData,
                                          cmRfcConnInstType   *conn,
                                          l2ca_identifier_t   identifier,
                                          l2ca_controller_t   local_control,
                                          l2ca_move_result_t  status,
                                          CsrBool             *checkFlag)
{
    if (conn->ampProcessState == CSR_BT_CM_AMP_PROCESS_LINK_LOST_IND_PENDING)
    {
        /* Release the rfcomm connection direct, as it from 
           the application  point of view must look like the remote 
           device did it */
        RfcDisconnectReqSend((CsrUint16)conn->btConnId);
    }
    
    CSR_UNUSED(identifier);
    CSR_UNUSED(local_control);
    CSR_UNUSED(status);
    CSR_UNUSED(checkFlag);
    return (TRUE);
}



static CsrBool CsrBtCmRfcAmpMoveIndHandler(cmInstanceData_t    *cmData,
                                           cmRfcConnInstType   *conn,
                                           l2ca_identifier_t   identifier,
                                           l2ca_controller_t   local_control,
                                           l2ca_move_result_t  status,
                                           CsrBool             *checkFlag)
{
    if (*checkFlag)
    { /* No need to go though this code again because 
         either has rfc_l2ca_move_channel_response has allready 
         been called */
        switch (conn->ampProcessState)
        {
            case CSR_BT_CM_AMP_PROCESS_LINK_LOST_IND_PENDING:
            { /* This message is receive while waiting for a 
                 CSR_BT_CM_MOVE_CHANNEL_RES as part of the 
                 RFC_L2CA_AMP_LINK_LOSS_IND procedure */
                if (local_control == CSR_BT_AMP_CONTROLLER_BREDR)
                { /* Set conn->ampProcessState to CSR_BT_CM_AMP_PROCESS_IDLE
                     and use the CSR_BT_CM_MOVE_CHANNEL_RES message that 
                     as ACK */
                    conn->moveIdentifier  = identifier;
                    conn->ampProcessState = CSR_BT_CM_AMP_PROCESS_IDLE; 
                }
                else
                { /* This request must be rejected */
                    return (FALSE);
                }
                break;
            }
            case CSR_BT_CM_AMP_PROCESS_LINK_LOST_REQ_PENDING:
            { /* This message is crossing with a L2CA_MOVE_CHANNEL_REQ. 
                 The L2CA_MOVE_CHANNEL_REQ were sent in order to move the AMP 
                 link to BD/EDR after a RFC_L2CA_AMP_LINK_LOSS_IND has occured */
                if (local_control == CSR_BT_AMP_CONTROLLER_BREDR)
                { /* Auto accept this message. */
                    *checkFlag = FALSE;
                    rfc_l2ca_move_channel_response(&conn->deviceAddr, 
                                                   identifier,
                                                   (RFC_RESPONSE_T)L2CA_MOVE_SUCCESS);
                
                }
                else
                { /* This request must be rejected */
                    return (FALSE);
                }
            }
            default:
            { 
                conn->moveIdentifier = identifier;
                CsrBtCmAmpMoveChannelIndSend(cmData,
                                             conn->btConnId,
                                             local_control,
                                             conn->deviceAddr);
                break;
            }
        }
    }
    return (TRUE);
}

static CsrBool csrBtCmRfcAmpMoveCmpIndHandler(cmInstanceData_t    *cmData,
                                              cmRfcConnInstType   *conn,
                                              l2ca_identifier_t   identifier,
                                              l2ca_controller_t   local_control,
                                              l2ca_move_result_t  status,
                                              CsrBool             *checkFlag)
{
    conn->moveIdentifier = 0;

    if (*checkFlag)
    { /* No need to go though this code again because 
         either has rfc_l2ca_move_channel_request has allready 
         been called or this message shall just be ignore.
         Note the for loop must still be running as all 
         conn->moveIdentifier must be set to 0 */
        if(status == L2CA_MOVE_SUCCESS)
        {
            /* Change sniff mode depending on whether AMP is used or not */
            conn->controller      = local_control;
            conn->ampProcessState = CSR_BT_CM_AMP_PROCESS_IDLE;
            CsrBtCmAmpForceModeChange(cmData, &conn->deviceAddr);
            CsrBtCmAmpMoveChannelCmpIndSend(cmData,
                                            conn->btConnId,
                                            conn->controller,
                                            conn->deviceAddr,
                                            CSR_BT_RESULT_CODE_CM_SUCCESS,
                                            CSR_BT_SUPPLIER_CM);
        }
        else
        {
            switch (conn->ampProcessState)
            {
                case CSR_BT_CM_AMP_PROCESS_LINK_LOST_IND_PENDING:
                { /* The local device has rejected the peer device attempt
                     to move the controller to a different controller than 
                     CSR_BT_AMP_CONTROLLER_BREDR while running the 
                     RFC_L2CA_AMP_LINK_LOSS_IND procedure. 
                     Just ignore the message */
                    *checkFlag = FALSE;
                    break;
                }
                case CSR_BT_CM_AMP_PROCESS_LINK_LOST_REQ_PENDING:
                { /* The local device has rejected the peer device attempt
                     to move the controller to a different controller than 
                     CSR_BT_AMP_CONTROLLER_BREDR while running the 
                     RFC_L2CA_AMP_LINK_LOSS_IND procedure and while trying
                     rfc_l2ca_move_channel_request were pending. 
                     Try to move it one more time */
                    *checkFlag = FALSE;
                    rfc_l2ca_move_channel_request(&conn->deviceAddr, 
                                                  CSR_BT_AMP_CONTROLLER_BREDR, 
                                                  CSR_BT_AMP_CONTROLLER_BREDR);
                    break;
                }
                default:
                { /* Past the result to the application */
                    conn->ampProcessState = CSR_BT_CM_AMP_PROCESS_IDLE;
                    CsrBtCmAmpMoveChannelCmpIndSend(cmData,
                                                    conn->btConnId,
                                                    local_control,
                                                    conn->deviceAddr,
                                                    (CsrBtResultCode)status,
                                                    CSR_BT_SUPPLIER_L2CAP_MOVE);
                    break;
                }
            }
        }
    }
    CSR_UNUSED(identifier);
    return (TRUE);
}

static CsrBool csrBtCmRfcAmpMoveCfmHandler(cmInstanceData_t    *cmData,
                                           cmRfcConnInstType   *conn,
                                           l2ca_identifier_t   identifier,
                                           l2ca_controller_t   local_control,
                                           l2ca_move_result_t  status,
                                           CsrBool             *checkFlag)
{
    conn->moveIdentifier = 0;
                    
    if (*checkFlag)
    { /* No need to go though this code again because 
         this message shall just be ignore. Note the 
         for loop must still be running as all 
         conn->moveIdentifier must be set to 0 */
        if(status == L2CA_MOVE_SUCCESS)
        {
            if (conn->ampProcessState == CSR_BT_CM_AMP_PROCESS_LINK_LOST_REQ_PENDING)
            {
                CsrBtCmAmpMoveChannelCmpIndSend(cmData,
                                                conn->btConnId,
                                                local_control,
                                                conn->deviceAddr,
                                                CSR_BT_RESULT_CODE_CM_SUCCESS,
                                                CSR_BT_SUPPLIER_CM);
            }
            else
            {
                CsrBtCmAmpMoveChannelCfmSend(cmData,
                                             conn->btConnId,
                                             local_control,
                                             conn->deviceAddr,
                                             CSR_BT_RESULT_CODE_CM_SUCCESS,
                                             CSR_BT_SUPPLIER_CM);
            }
            /* Change sniff mode depending on whether AMP is used or not */
            conn->controller        = local_control;
            conn->ampProcessState   = CSR_BT_CM_AMP_PROCESS_IDLE;
            CsrBtCmAmpForceModeChange(cmData, &conn->deviceAddr);
        }
        else
        {
            if (conn->ampProcessState == CSR_BT_CM_AMP_PROCESS_LINK_LOST_REQ_PENDING)
            {
                if (status == L2CA_MOVE_LOCAL_BUSY)
                { /* RFC_L2CA_MOVE_CHANNEL_REQ and a RFC_L2CA_MOVE_CHANNEL_IND 
                     is crossing. Ignore this message and wait for the 
                     RFC_L2CA_MOVE_CHANNEL_CMP_IND instead */
                    *checkFlag = FALSE;
                }
                else
                { /* Release the rfcomm connection direct, as it from 
                     the application  point of view must look like the remote 
                     device did it */
                    RfcDisconnectReqSend((CsrUint16)conn->btConnId);
                }
            }
            else
            {
                conn->ampProcessState = CSR_BT_CM_AMP_PROCESS_IDLE;
                CsrBtCmAmpMoveChannelCfmSend(cmData,
                                             conn->btConnId,
                                             local_control,
                                             conn->deviceAddr,
                                             (CsrBtResultCode)status,
                                             CSR_BT_SUPPLIER_L2CAP_MOVE);
            }
        }
    }
    CSR_UNUSED(identifier);
    return (TRUE);
}


static CsrBool CsrBtCmRfcAmpHandler(cmInstanceData_t            *cmData,
                                    CsrBtDeviceAddr             deviceAddr,
                                    l2ca_identifier_t           identifier,
                                    l2ca_controller_t           local_control,
                                    RFC_RESPONSE_T              status,
                                    CsrBtCmRfcAmpCommonFuncType callbackFunc)
{
    CsrBool          checkFlag  = TRUE;
    cmRfcConnElement *rfcElm    = NULL;  

    if(cmData->ampmHandle == CSR_SCHED_QID_INVALID)
    { /* No AMPM registered */
        return (FALSE);
    }

    for(rfcElm = CM_RFC_GET_FIRST(cmData->rfcVar.connList); rfcElm; rfcElm = rfcElm->next)
    {
        if (rfcElm->cmRfcConnInst)
        {
            cmRfcConnInstType *conn = rfcElm->cmRfcConnInst;

            if(CsrBtBdAddrEq(&conn->deviceAddr, &deviceAddr))
            { 
                if (!callbackFunc(cmData, conn, identifier, local_control, (l2ca_move_result_t)status, &checkFlag))
                { /* The message is rejected */ 
                    return (FALSE);
                }
            }
        }
    }
    return (TRUE);
}


/* Downstream handler */
void CsrBtCmRfcAmpMoveChannelReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmMoveChannelReq *cmPrim = (CsrBtCmMoveChannelReq*)cmData->recvMsgP;
    cmRfcConnElement *rfcElm = CM_FIND_RFC_ELEMENT(CsrBtCmRfcFindRfcConnElementFromBtConnId, &(cmPrim->btConnId));

    if(rfcElm)
    {
        cmRfcConnInstType *conn = rfcElm->cmRfcConnInst;
        if(conn->state == connected_s)
        {
            if (conn->ampProcessState != CSR_BT_CM_AMP_PROCESS_IDLE)
            { /* An AMP Link lost has occured. The link cannot move right now. 
                 Return an error */
                CsrBtCmAmpMoveChannelCfmSend(cmData,
                                         conn->btConnId,
                                         CSR_BT_AMP_CONTROLLER_BREDR,
                                         conn->deviceAddr,
                                         L2CA_MOVE_LOCAL_BUSY,
                                         CSR_BT_SUPPLIER_L2CAP_MOVE);    
            }
            else
            { /* Righty then, send the move request to RFCOMM */
                rfc_l2ca_move_channel_request(&conn->deviceAddr,
                                              cmPrim->remoteControl,
                                              cmPrim->localControl);
            }
        }
        else
        {
            /* Tried to move a non-existant channel */
            CsrBtCmAmpMoveChannelCfmSend(cmData,
                                         conn->btConnId,
                                         CSR_BT_AMP_CONTROLLER_BREDR,
                                         conn->deviceAddr,
                                         CSR_BT_RESULT_CODE_CM_UNKNOWN_CONNECTION_IDENTIFIER,
                                         CSR_BT_SUPPLIER_CM);
        }
    }
}

void CsrBtCmRfcAmpMoveChannelResHandler(cmInstanceData_t *cmData)
{
    CsrBtCmMoveChannelRes *cmPrim;
    cmRfcConnElement *rfcElm;

    cmPrim = (CsrBtCmMoveChannelRes*)cmData->recvMsgP;
    rfcElm = CM_FIND_RFC_ELEMENT(CsrBtCmRfcFindRfcConnElementFromBtConnId,
                               &(cmPrim->btConnId));

    if(rfcElm)
    {
        cmRfcConnInstType *conn = rfcElm->cmRfcConnInst;
        if(conn->moveIdentifier != 0)
        {
            /* Righty then, send the move request to RFCOMM. Note the
             * result code type cast: We need this as RFCOMM will pass
             * the error code straight through to L2CAP. */
            rfc_l2ca_move_channel_response(&conn->deviceAddr,
                                           conn->moveIdentifier,
                                           (RFC_RESPONSE_T)(cmPrim->accept
                                                            ? L2CA_MOVE_SUCCESS
                                                            : L2CA_MOVE_REFUSED));
            conn->moveIdentifier = 0;
        }
        else
        {
            if (conn->ampProcessState == CSR_BT_CM_AMP_PROCESS_LINK_LOST_IND_PENDING)
            { /* An AMP Link lost has occured must check if the application 
                 has accepted to return to BR/EDR or not */
                if (cmPrim->accept)
                { /* Return to BR/EDR */
                    CsrBtCmRfcAmpHandler(cmData,
                                         conn->deviceAddr,
                                         0,
                                         0,
                                         L2CA_MOVE_SUCCESS,
                                         CsrBtCmRfcAmpChangeToLinkLostReqPendingState); 
                    
                    rfc_l2ca_move_channel_request(&conn->deviceAddr,
                                                  CSR_BT_AMP_CONTROLLER_BREDR,
                                                  CSR_BT_AMP_CONTROLLER_BREDR);
                }
                else
                { /* Release the rfcomm connections */
                    CsrBtCmRfcAmpHandler(cmData,
                                         conn->deviceAddr,
                                         0,
                                         0,
                                         L2CA_MOVE_SUCCESS,
                                         CsrBtCmRfcAmpRfcReleaseAll); 
                }
            }
            else
            { /* Tried to move a non-existant channel */
                CsrBtCmAmpMoveChannelCmpIndSend(cmData,
                                                conn->btConnId,
                                                CSR_BT_AMP_CONTROLLER_BREDR,
                                                conn->deviceAddr,
                                                CSR_BT_RESULT_CODE_CM_UNKNOWN_CONNECTION_IDENTIFIER,
                                                CSR_BT_SUPPLIER_CM);
            }
        }
    }
}

/* Upstream handler */
void CsrBtCmRfcAmpMoveChannelIndHandler(cmInstanceData_t *cmData)
{
    RFC_L2CA_MOVE_CHANNEL_IND_T *prim = (RFC_L2CA_MOVE_CHANNEL_IND_T*)cmData->recvMsgP;
    CsrBool accepted = CsrBtCmRfcAmpHandler(cmData,
                                            prim->bd_addr,
                                            prim->identifier,
                                            prim->local_control,
                                            L2CA_MOVE_SUCCESS,
                                            CsrBtCmRfcAmpMoveIndHandler); 

    if (!accepted)
    {
        /* No AMPM registered, or connection didn't match. Refuse and
         * bail out */
        rfc_l2ca_move_channel_response(&prim->bd_addr, 
                                       prim->identifier,
                                       (RFC_RESPONSE_T)L2CA_MOVE_REFUSED);
    }
}

void CsrBtCmRfcAmpMoveChannelCfmHandler(cmInstanceData_t *cmData)
{
    RFC_L2CA_MOVE_CHANNEL_CFM_T *prim = (RFC_L2CA_MOVE_CHANNEL_CFM_T*)cmData->recvMsgP;
    CsrBtCmRfcAmpHandler(cmData,
                         prim->bd_addr,
                         0,
                         prim->local_control,
                         prim->status,
                         csrBtCmRfcAmpMoveCfmHandler); 
}

void CsrBtCmRfcAmpMoveChannelCmpIndHandler(cmInstanceData_t *cmData)
{
    RFC_L2CA_MOVE_CHANNEL_CMP_IND_T *prim = (RFC_L2CA_MOVE_CHANNEL_CMP_IND_T *)cmData->recvMsgP;
    CsrBtCmRfcAmpHandler(cmData,
                         prim->bd_addr,
                         0,
                         prim->local_control,
                         prim->status,
                         csrBtCmRfcAmpMoveCmpIndHandler); 
}

void CsrBtCmRfcAmpLinkLossIndHandler(cmInstanceData_t *cmData)
{
    RFC_L2CA_AMP_LINK_LOSS_IND_T *prim = (RFC_L2CA_AMP_LINK_LOSS_IND_T*) cmData->recvMsgP;
    CsrBtCmRfcAmpHandler(cmData,
                         prim->bd_addr,
                         0,
                         0,
                         prim->reason,
                         csrBtCmRfcAmpLinkLossHandler);    
}

#endif /* CSR_AMP_ENABLE */
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */

