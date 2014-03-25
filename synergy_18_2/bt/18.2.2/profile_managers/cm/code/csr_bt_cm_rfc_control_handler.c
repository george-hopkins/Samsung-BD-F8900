/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_RFC_MODULE

#include "csr_bt_cm_main.h"
#include "csr_bt_cm_rfc.h"

void CsrBtCmControlIndMsgSend(cmRfcConnInstType * theLogicalLink, CsrUint8 modem_signal, CsrUint8  break_signal)
{ /* Send a CSR_BT_CM_CONTROL_IND signal to the application */

    CsrBtCmControlInd * cmPrim;

    cmPrim                    = (CsrBtCmControlInd *)CsrPmemAlloc(sizeof(CsrBtCmControlInd));
    cmPrim->type            = CSR_BT_CM_CONTROL_IND;
    cmPrim->btConnId           = theLogicalLink->btConnId;
    cmPrim->modemstatus        = modem_signal;
    cmPrim->break_signal    = break_signal;
    cmPrim->context         = theLogicalLink->context;
    CsrBtCmPutMessage(theLogicalLink->appHandle, cmPrim);
}

void CsrBtCmRfcModemStatusIndHandler(cmInstanceData_t *cmData)
{ /* This event indicates to the port entity that the remote port emulation
     entity wishes to use the control parameters set in control_pars. */
    RFC_MODEM_STATUS_IND_T *rfcPrim     = (RFC_MODEM_STATUS_IND_T *) cmData->recvMsgP;
    CsrBtConnId   value = CM_CREATE_RFC_CONN_ID(rfcPrim->conn_id);
    cmRfcConnElement * theElement;
    theElement = CM_FIND_RFC_ELEMENT(CsrBtCmRfcFindRfcConnElementFromBtConnId, &(value));
    if(theElement)
    { /* Note that the application can received this event before it get a
         CSR_BT_CM_CONNECT_CFM with success */
        cmRfcConnInstType *theLogicalLink   = theElement->cmRfcConnInst;

        if( theLogicalLink->state == connect_s            ||
            theLogicalLink->state == connectAccept_s    ||
            theLogicalLink->state == connectAcceptFinal_s    )
        {
            if(theLogicalLink->controlSignalQueue == NULL)
            {
                theLogicalLink->controlSignalQueue = rfcPrim;
                cmData->recvMsgP = NULL;
            }
            else
            {
                CsrPmemFree(theLogicalLink->controlSignalQueue);
                theLogicalLink->controlSignalQueue = rfcPrim;
                cmData->recvMsgP = NULL;
            }
        }
        else
        {
            CsrBtCmControlIndMsgSend(theLogicalLink, rfcPrim->modem_signal, rfcPrim->break_signal);
        }
    }
    else
    { /* No owner, just ignore                                              */
        ;
    }
}

void CsrBtCmRfcModemStatusCfmHandler(cmInstanceData_t *cmData)
{
    RFC_MODEM_STATUS_CFM_T *rfcPrim = (RFC_MODEM_STATUS_CFM_T *) cmData->recvMsgP;
    CsrBtConnId value               = CM_CREATE_RFC_CONN_ID(rfcPrim->conn_id);
    cmRfcConnElement *theElement    = CM_FIND_RFC_ELEMENT(CsrBtCmRfcFindRfcConnElementFromBtConnId, &(value));

    if (theElement)
    {
        cmRfcConnInstType *theLogicalLink   = theElement->cmRfcConnInst;
        
        if(theLogicalLink->state == connected_s)
        {
            if (theLogicalLink->modemStatusState == CSR_BT_CM_MODEM_STATUS_PENDING)
            { /* All modem status messages are sent, goto CSR_BT_CM_MODEM_STATUS_IDLE */
                theLogicalLink->modemStatusState = CSR_BT_CM_MODEM_STATUS_IDLE;
            }
            else
            { /* modemStatusState must be CSR_BT_CM_MODEM_STATUS_QUEUED. E.g the next
                 modem status message must be sent. Goto CSR_BT_CM_MODEM_STATUS_PENDING */ 
                theLogicalLink->modemStatusState = CSR_BT_CM_MODEM_STATUS_PENDING;
                RfcModemStatusReqSend(rfcPrim->conn_id, 
                                      theLogicalLink->modemStatus, 
                                      theLogicalLink->signalBreak);
            }
        }
        else
        { /* Not connected, just ignore */
            ;
        }
    }
    else
    { /* No owner, just ignore */
        ;
    }
}

void CsrBtCmRfcControlReqMsgSend(cmInstanceData_t *cmData)
{ /* This event is used by application to convey control information to the
     remote port (including flow control on a btConnId). The control
     information is set in control_pars. */
    CsrBtCmControlReq  *cmPrim   = (CsrBtCmControlReq *) cmData->recvMsgP;
    cmRfcConnElement *theElement = CM_FIND_RFC_ELEMENT(CsrBtCmRfcFindRfcConnElementFromBtConnId, &(cmPrim->btConnId));
    
    if(theElement)
    { 
        cmRfcConnInstType *theLogicalLink   = theElement->cmRfcConnInst;
        
        if(theLogicalLink->state == connected_s)
        {
            if (theLogicalLink->modemStatusState == CSR_BT_CM_MODEM_STATUS_IDLE)
            { /* No modem signal is being sent, pass it direct to rfcomm 
                 and goto CSR_BT_CM_MODEM_STATUS_PENDING state          */
                theLogicalLink->modemStatusState = CSR_BT_CM_MODEM_STATUS_PENDING;
                RfcModemStatusReqSend(CM_GET_UINT16ID_FROM_BTCONN_ID(cmPrim->btConnId), 
                                                                     cmPrim->modemstatus, 
                                                                     cmPrim->break_signal);
            }
            else
            { /* A modem message is being sent, must wait for a 
                 RFC_MODEM_STATUS_CFM, Goto CSR_BT_CM_MODEM_STATUS_QUEUED state 
                 and save incoming values */                
                theLogicalLink->modemStatusState = CSR_BT_CM_MODEM_STATUS_QUEUED;      
                theLogicalLink->modemStatus      = cmPrim->modemstatus;
                theLogicalLink->signalBreak      = cmPrim->break_signal;
            }
        }
        else
        { /* Not connected, just ignore */
            ;
        }
    }
    else
    { /* No owner, just ignore */
        ;
    }
}

#endif /*#ifndef EXCLUDE_CSR_BT_RFC_MODULE */
