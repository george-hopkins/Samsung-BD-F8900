/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
#ifdef CSR_AMP_ENABLE

#include "csr_bt_cm_l2cap.h"
#include "csr_bt_cm_util.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_cm_common_amp.h"
#include "csr_bt_cm_l2cap_conftab.h"

#define CSR_BT_CM_FCS_CONFTAB_SIZE (3)

/* Public helpers */
CsrUint16 * CsrBtCmL2caBuildFcsConftab(CsrBtAmpController controller,
                                       CsrUint16          *conftabLength)
{
    CsrUint16 *conftab  = CsrPmemAlloc(CSR_BT_CM_FCS_CONFTAB_SIZE * sizeof(CsrUint16));
    conftab[0]          = BKV_SEPARATOR;
    *conftabLength      = 1;

    if (controller == CSR_BT_AMP_CONTROLLER_BREDR)
    {
        CsrBtCmL2caConftabFcs(conftab, conftabLength, TRUE);
    }
    else
    {
        CsrBtCmL2caConftabFcs(conftab, conftabLength, FALSE);
    }
    return (conftab);
}

CsrBool CsrBtCmL2caFcsReConfigEnable(BKV_ITERATOR_T conftabIter)
{
#ifdef CSR_BT_CONFIG_L2CAP_FCS
    CsrUint16 val;
    
    if(BKV_Scan16Single(&conftabIter, CSR_BT_CM_AUTOPT_AUTO_FCS_OFF_AMP, &val) &&
       (val == CSR_BT_CM_AUTOPT_TRUE))
    { /* According to the GOEP2.0 specification the value of the FCS option should be configured 
         to the default value of 16-bit FCS. When running GOEP over AMP Controllers the FCS option 
         should be configured to the value of No FCS. The FCS option can be reconfigured between 
         these two values. An implementation may reconfigure the FCS option for the L2CAP channel 
         for GOEP after the channel is moved between BR/EDR and AMP controllers.
         Other L2CAP Configuration options are excluded from GOEP. */
        return (TRUE);
    }
#else
    CSR_UNUSED(conftabIter);
#endif

    return (FALSE);
}

/* Static helpers */
static void csrBtCmL2caReConfigFcsReqHandler(cmInstanceData_t *cmData, cmL2caConnInstType *l2caConn)
{
    if (CsrBtCmL2caFcsReConfigEnable(l2caConn->conftabIter))
    { /* According to the GOEP2.0 specification the value of the FCS option should be configured 
         to the default value of 16-bit FCS. When running GOEP over AMP Controllers the FCS option 
         should be configured to the value of No FCS. The FCS option can be reconfigured between 
         these two values. An implementation may reconfigure the FCS option for the L2CAP channel 
         for GOEP after the channel is moved between BR/EDR and AMP controllers.
         Other L2CAP Configuration options are excluded from GOEP. */
        CsrUint16 conftabLength;

        CsrUint16 *conftab = CsrBtCmL2caBuildFcsConftab(l2caConn->controller, &conftabLength);
        
        /* Start L2CAP reconfig */
        L2CA_AutoConnectReq(CM_GET_UINT16ID_FROM_BTCONN_ID(l2caConn->btConnId),
                            l2caConn->psm,
                            &l2caConn->deviceAddr,
                            l2caConn->remotePsm,
                            CM_L2CA_RECONFIG_INPROGRESS_CTX, 
                            CSR_BT_AMP_CONTROLLER_BREDR,
                            CSR_BT_AMP_CONTROLLER_BREDR,
                            conftabLength,
                            conftab);     
    }
    else
    {
        ;
    }
}

/* Downstream handler */
void CsrBtCmL2caMoveChannelReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmMoveChannelReq *prim;
    cmL2caConnElement *connElm;

    prim = (CsrBtCmMoveChannelReq*)cmData->recvMsgP;
    connElm = CM_FIND_L2CA_ELEMENT(CsrBtCmL2caFindL2caConnElementFromBtConnId, &(prim->btConnId));

    if(connElm)
    {
        cmL2caConnInstType *conn = connElm->cmL2caConnInst;

        if(conn->state == l2capConnected_s)
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
            { /* Righty then, send move request to L2CAP */
                L2CA_MoveChannelReq(CM_GET_UINT16ID_FROM_BTCONN_ID(conn->btConnId),
                                    prim->remoteControl,
                                    prim->localControl);
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

void CsrBtCmL2caMoveChannelResHandler(cmInstanceData_t *cmData)
{
    CsrBtCmMoveChannelRes *prim = (CsrBtCmMoveChannelRes*)cmData->recvMsgP;
    cmL2caConnElement *connElm  = CM_FIND_L2CA_ELEMENT(CsrBtCmL2caFindL2caConnElementFromBtConnId, &(prim->btConnId));

    if(connElm)
    {
        cmL2caConnInstType *conn = connElm->cmL2caConnInst;

        if(conn->moveIdentifier != 0)
        {
            /* Righty then, send move response to L2CAP */
            L2CA_MoveChannelRsp(conn->moveIdentifier,
                                CM_GET_UINT16ID_FROM_BTCONN_ID(conn->btConnId),
                                (l2ca_move_result_t)(prim->accept ? L2CA_MOVE_SUCCESS : L2CA_MOVE_REFUSED));
            conn->moveIdentifier = 0;
        }
        else
        {
            if (conn->ampProcessState == CSR_BT_CM_AMP_PROCESS_LINK_LOST_IND_PENDING)
            { /* An AMP Link lost has occured must check if the application 
                 has accepted to return to BR/EDR or not */
                if (prim->accept)
                { /* Return to BR/EDR */
                    conn->ampProcessState = CSR_BT_CM_AMP_PROCESS_LINK_LOST_REQ_PENDING;
                    L2CA_MoveChannelReq(CM_GET_UINT16ID_FROM_BTCONN_ID(conn->btConnId), 
                                        CSR_BT_AMP_CONTROLLER_BREDR, 
                                        CSR_BT_AMP_CONTROLLER_BREDR);
                }
                else
                { /* Release the l2cap connection direct, as it from the application 
                     point of view must look like the remote device did it */
                    L2CA_DisconnectReq(CM_GET_UINT16ID_FROM_BTCONN_ID(conn->btConnId));
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
void CsrBtCmL2caMoveChannelIndHandler(cmInstanceData_t *cmData)
{
    L2CA_MOVE_CHANNEL_IND_T *prim = (L2CA_MOVE_CHANNEL_IND_T*)cmData->recvMsgP;

    if(cmData->ampmHandle == CSR_SCHED_QID_INVALID)
    {
        /* No AMPM registered. Refuse and bail out */
        L2CA_MoveChannelRsp(prim->identifier, prim->cid, L2CA_MOVE_REFUSED);
    }
    else
    {
        CsrBtConnId  btConnId         = CM_CREATE_L2CA_CONN_ID(prim->cid);
        cmL2caConnElement *connElm    = CM_FIND_L2CA_ELEMENT(CsrBtCmL2caFindL2caConnElementFromBtConnId, &(btConnId));
        
        if (connElm)
        {
            cmL2caConnInstType *conn = connElm->cmL2caConnInst;
        
            switch (conn->ampProcessState)
            {
                case CSR_BT_CM_AMP_PROCESS_LINK_LOST_IND_PENDING:
                { /* This message is receive while waiting for a 
                     CSR_BT_CM_MOVE_CHANNEL_RES as part of the L2CA_AMP_LINK_LOSS_IND
                     procedure */
                    if (prim->local_control == CSR_BT_AMP_CONTROLLER_BREDR)
                    { /* Set conn->ampProcessState to CSR_BT_CM_AMP_PROCESS_IDLE
                         and use the CSR_BT_CM_MOVE_CHANNEL_RES message that 
                         as ACK */
                        conn->moveIdentifier  = prim->identifier;
                        conn->ampProcessState = CSR_BT_CM_AMP_PROCESS_IDLE; 
                    }
                    else
                    { /* Reject this request */
                        L2CA_MoveChannelRsp(prim->identifier, prim->cid, L2CA_MOVE_REFUSED);
                    }
                    break;
                }
                case CSR_BT_CM_AMP_PROCESS_LINK_LOST_REQ_PENDING:
                { /* This message is crossing with a L2CA_MOVE_CHANNEL_REQ. 
                     The L2CA_MOVE_CHANNEL_REQ were sent in order to move the AMP 
                     link to BD/EDR after a L2CA_AMP_LINK_LOSS_IND has occured */
                    if (prim->local_control == CSR_BT_AMP_CONTROLLER_BREDR)
                    { /* Auto accept this message. */
                        L2CA_MoveChannelRsp(prim->identifier, prim->cid, L2CA_MOVE_SUCCESS);
                    }
                    else
                    { /* Reject this request */
                        L2CA_MoveChannelRsp(prim->identifier, prim->cid, L2CA_MOVE_REFUSED);
                    }
                    break;
                }
                default:
                { 
                    conn->moveIdentifier = prim->identifier;
                    CsrBtCmAmpMoveChannelIndSend(cmData,
                                                 conn->btConnId,
                                                 prim->local_control,
                                                 conn->deviceAddr);
                    break;
                }
            }
        }
        else
        { /* No owner, refuse and bail out */
            L2CA_MoveChannelRsp(prim->identifier, prim->cid, L2CA_MOVE_REFUSED);
        }
    }
}

void CsrBtCmL2caMoveChannelCfmHandler(cmInstanceData_t *cmData)
{
    if(cmData->ampmHandle == CSR_SCHED_QID_INVALID)
    {
        /* No AMPM registered */
        ;
    }
    else
    {
        L2CA_MOVE_CHANNEL_CFM_T *prim = (L2CA_MOVE_CHANNEL_CFM_T*)cmData->recvMsgP;
        CsrBtConnId btConnId          = CM_CREATE_L2CA_CONN_ID(prim->cid);
        cmL2caConnElement *connElm    = CM_FIND_L2CA_ELEMENT(CsrBtCmL2caFindL2caConnElementFromBtConnId, &(btConnId));

        if(connElm)
        {
            cmL2caConnInstType *conn = connElm->cmL2caConnInst;
            conn->moveIdentifier     = 0;
            
            if (prim->result == L2CA_MOVE_SUCCESS)
            { /* Change sniff mode depending on whether AMP is used or not */
                if (conn->ampProcessState == CSR_BT_CM_AMP_PROCESS_LINK_LOST_REQ_PENDING)
                {
                    CsrBtCmAmpMoveChannelCmpIndSend(cmData,
                                                    conn->btConnId,
                                                    prim->local_control,
                                                    conn->deviceAddr,
                                                    CSR_BT_RESULT_CODE_CM_SUCCESS,
                                                    CSR_BT_SUPPLIER_CM);     
                }   
                else
                {
                    CsrBtCmAmpMoveChannelCfmSend(cmData,
                                                 conn->btConnId,
                                                 prim->local_control,
                                                 conn->deviceAddr,
                                                 CSR_BT_RESULT_CODE_CM_SUCCESS,
                                                 CSR_BT_SUPPLIER_CM);
                }
                conn->controller      = prim->local_control;
                conn->ampProcessState = CSR_BT_CM_AMP_PROCESS_IDLE;
                CsrBtCmAmpForceModeChange(cmData, &conn->deviceAddr);
                csrBtCmL2caReConfigFcsReqHandler(cmData, conn);
            }
            else
            {
                if (conn->ampProcessState == CSR_BT_CM_AMP_PROCESS_LINK_LOST_REQ_PENDING)
                {
                    if ( (prim->result == L2CA_MOVE_LOCAL_BUSY) || (prim->result == L2CA_MOVE_INITIATING))
                    { /* L2CA_MOVE_CHANNEL_REQ and a L2CA_MOVE_CHANNEL_IND is crossing.
                         Ignore this message and wait for the L2CA_MOVE_CHANNEL_CMP_IND
                         instead */
                        ;
                    }
                    else
                    { /* Release the l2cap connection direct, as it from the application 
                         point of view must look like the remote device did it */
                        L2CA_DisconnectReq(prim->cid);
                    }
                }
                else
                {
                    conn->ampProcessState = CSR_BT_CM_AMP_PROCESS_IDLE;
                    CsrBtCmAmpMoveChannelCfmSend(cmData,
                                                 conn->btConnId,
                                                 prim->local_control,
                                                 conn->deviceAddr,
                                                 (CsrBtResultCode)prim->result,
                                                 CSR_BT_SUPPLIER_L2CAP_MOVE);
                }
            }
        }
        else
        { /* No owner, just ignore */
            ;
        }
    }
}

void CsrBtCmL2caMoveChannelCmpIndHandler(cmInstanceData_t *cmData)
{
    if(cmData->ampmHandle == CSR_SCHED_QID_INVALID)
    { /* No AMPM registered */
        ;
    }
    else
    {
        L2CA_MOVE_CHANNEL_CMP_IND_T *prim = (L2CA_MOVE_CHANNEL_CMP_IND_T*)cmData->recvMsgP;
        CsrBtConnId btConnId              = CM_CREATE_L2CA_CONN_ID(prim->cid);
        cmL2caConnElement *connElm        = CM_FIND_L2CA_ELEMENT(CsrBtCmL2caFindL2caConnElementFromBtConnId, &(btConnId));
        
        if(connElm)
        {
            cmL2caConnInstType *conn = connElm->cmL2caConnInst;
            conn->moveIdentifier     = 0;

            if (prim->result == L2CA_MOVE_SUCCESS)
            {
                /* Change sniff mode depending on whether AMP is used or not */
                conn->controller        = prim->local_control;
                conn->ampProcessState   = CSR_BT_CM_AMP_PROCESS_IDLE;
                CsrBtCmAmpForceModeChange(cmData, &conn->deviceAddr);
                CsrBtCmAmpMoveChannelCmpIndSend(cmData,
                                                conn->btConnId,
                                                prim->local_control,
                                                conn->deviceAddr,
                                                CSR_BT_RESULT_CODE_CM_SUCCESS,
                                                CSR_BT_SUPPLIER_CM);

                csrBtCmL2caReConfigFcsReqHandler(cmData, conn);
                 
            }
            else
            {
                switch (conn->ampProcessState)
                {
                    case CSR_BT_CM_AMP_PROCESS_LINK_LOST_IND_PENDING:
                    { /* The local device has rejected the peer device attempt
                         to move the controller to a different controller than 
                         CSR_BT_AMP_CONTROLLER_BREDR while running the 
                         L2CA_AMP_LINK_LOSS_IND procedure. Just ignore the message */
                        break;
                    }
                    case CSR_BT_CM_AMP_PROCESS_LINK_LOST_REQ_PENDING:
                    { /* The local device has rejected the peer device attempt
                         to move the controller to a different controller than 
                         CSR_BT_AMP_CONTROLLER_BREDR while running the 
                         L2CA_AMP_LINK_LOSS_IND procedure and while trying
                         L2CA_MoveChannelReq were pending. Try to move it one
                         more time */
                         L2CA_MoveChannelReq(prim->cid, 
                                             CSR_BT_AMP_CONTROLLER_BREDR, 
                                             CSR_BT_AMP_CONTROLLER_BREDR);
                        break;
                    }
                    default:
                    { /* Past the result to the application */
                        conn->ampProcessState = CSR_BT_CM_AMP_PROCESS_IDLE;
                        CsrBtCmAmpMoveChannelCmpIndSend(cmData,
                                                        conn->btConnId,
                                                        prim->local_control,
                                                        conn->deviceAddr,
                                                        (CsrBtResultCode)prim->result,
                                                        CSR_BT_SUPPLIER_L2CAP_MOVE);
                        break;
                    }
                }
            }
        }
        else
        { /* No owner just ignore */
            ;
        }
    }
}

void CsrBtCmL2caAmpLinkLossIndHandler(cmInstanceData_t *cmData)
{ /* AMP Link lost. Note l2cap will not sent this message while 
     moving is pending */
    if(cmData->ampmHandle == CSR_SCHED_QID_INVALID)
    { /* No AMPM registered */
        ;
    }
    else
    {
        L2CA_AMP_LINK_LOSS_IND_T *prim = (L2CA_AMP_LINK_LOSS_IND_T*)cmData->recvMsgP;
        CsrBtConnId btConnId           = CM_CREATE_L2CA_CONN_ID(prim->cid);
        cmL2caConnElement *connElm     = CM_FIND_L2CA_ELEMENT(CsrBtCmL2caFindL2caConnElementFromBtConnId, &(btConnId));
        
        if(connElm)
        { /* Ask the app if it will go back to BR/EDR or not */
            cmL2caConnInstType *conn = connElm->cmL2caConnInst;
            conn->moveIdentifier     = 0;
            conn->ampProcessState    = CSR_BT_CM_AMP_PROCESS_LINK_LOST_IND_PENDING;
            CsrBtCmAmpMoveChannelIndSend(cmData,
                                         conn->btConnId,
                                         CSR_BT_AMP_CONTROLLER_BREDR,
                                         conn->deviceAddr);
        }
        else
        { /* No owner just ignore */
            ;
        }
    }
}

#endif /* #ifdef CSR_AMP_ENABLE */
#endif /* #ifndef EXCLUDE_CSR_BT_L2CA_MODULE */
