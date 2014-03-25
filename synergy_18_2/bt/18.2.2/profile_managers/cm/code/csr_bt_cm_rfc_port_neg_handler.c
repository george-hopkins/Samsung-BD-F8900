/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_RFC_MODULE

#include "csr_bt_cm_main.h"
#include "csr_bt_cm_rfc.h"

static void csrBtCmRfcPortNegCfmSend(CsrSchedQid             phandle,
                                     CsrBtConnId          btConnId,
                                     RFC_PORTNEG_VALUES_T         portPar,
                                     CsrUint16          context,
                                     CsrBtResultCode    resultCode,
                                     CsrBtSupplier      resultSupplier)
{
    CsrBtCmPortnegCfm *cfmPrim = (CsrBtCmPortnegCfm *)CsrPmemAlloc(sizeof(CsrBtCmPortnegCfm));
    cfmPrim->type              = CSR_BT_CM_PORTNEG_CFM;
    cfmPrim->btConnId          = btConnId;
    cfmPrim->portPar           = portPar;
    cfmPrim->context           = context;
    cfmPrim->resultCode        = resultCode;
    cfmPrim->resultSupplier    = resultSupplier;
    CsrBtCmPutMessage(phandle, cfmPrim);
}

void CsrBtCmRfcPortNegCfmHandler(cmInstanceData_t *cmData)
{ /* This event is the confirmation of the locally-initiated port negotiation
     procedure. The contents of the port parameters will be the same as those
     in the original request, except where the corresponding bit in the
     parameter_mask is set to zero, in which case that parameter is set to
     an alternative value proposed by the remote device.

     Now request the establishment of a new server channel with the current
     set of dlc parameters on the multiplexer. */
    RFC_PORTNEG_CFM_T *rfcPrim      = (RFC_PORTNEG_CFM_T *) cmData->recvMsgP;
    CsrBtConnId            value    = CM_CREATE_RFC_CONN_ID(rfcPrim->conn_id);
    cmRfcConnElement * theElement   = CM_FIND_RFC_ELEMENT(CsrBtCmRfcFindRfcConnElementFromBtConnId, &(value));

    if (theElement)
    {
        cmRfcConnInstType *theLogicalLink = theElement->cmRfcConnInst;
        theLogicalLink->portPar           = rfcPrim->port_pars;

        if (theLogicalLink->state == connected_s)
        {/* This is the answer to a request from the application */
            csrBtCmRfcPortNegCfmSend(theLogicalLink->appHandle,
                                     value,
                                     rfcPrim->port_pars,
                                     theLogicalLink->context,
                                     CSR_BT_RESULT_CODE_CM_SUCCESS,
                                     CSR_BT_SUPPLIER_CM);
        }
        else
        { /* ignore, state invalid                                      */
            ;
        }
    }
   else
    { /* ignore; this must be a cross with disconnection                */
        ;
    }
}

void CsrBtCmRfcPortNegIndHandler(cmInstanceData_t *cmData)
{ /* This event is the response to the previous RFC_PORTNEG_REQ event. The
     responding port entity must set the 'port_pars.parameter_mask' bits to
     '1' to indicate which new parameters have been accepted, or to '0' to
     indicate any which are rejected. If request is true, the indication
     shows the values of the port at the remote port entity. */
    RFC_PORTNEG_IND_T * rfcPrim   = (RFC_PORTNEG_IND_T *) cmData->recvMsgP;
    CsrBtConnId value = CM_CREATE_RFC_CONN_ID(rfcPrim->conn_id);
    cmRfcConnElement * theElement    = CM_FIND_RFC_ELEMENT(CsrBtCmRfcFindRfcConnElementFromBtConnId, &(value));

    if(theElement)
    { /* The CM has identify the index in the connection table. Send the signal
         to the application */
        cmRfcConnInstType *theLogicalLink = theElement->cmRfcConnInst;
        CsrBtCmPortnegInd *cmPrim         = (CsrBtCmPortnegInd *)CsrPmemAlloc(sizeof(CsrBtCmPortnegInd));

        cmPrim->type          = CSR_BT_CM_PORTNEG_IND;
        cmPrim->portPar       = rfcPrim->port_pars;
        cmPrim->request       = (CsrBool)rfcPrim->request;
        cmPrim->btConnId      = value;
        cmPrim->context       = theLogicalLink->context;
        CsrBtCmPutMessage(theLogicalLink->appHandle, cmPrim);
    }
    else
    { /* Could not identify the index in the connection table. Just
         return the signal */
        RFC_PORTNEG_VALUES_T        portPar;

        portPar = rfcPrim->port_pars;
        RfcPortnegResSend(rfcPrim->conn_id, &portPar);
    }
}

void CsrBtCmRfcPortNegResHandler(cmInstanceData_t *cmData)
{ /* This event is the response received from the application to the previous
     RFC_PORTNEG_IND event. */
    CsrBtCmPortnegRes *cmPrim;

    cmPrim    = (CsrBtCmPortnegRes *) cmData->recvMsgP;
    RfcPortnegResSend(CM_GET_UINT16ID_FROM_BTCONN_ID(cmPrim->btConnId), &cmPrim->portPar);
}

void CsrBtCmRfcPortnegReqHandler(cmInstanceData_t *cmData)
{ /* This event is the request from the application; if the connection is found forward
     it to the RFC module. */
    CsrBtCmPortnegReq *cmPrim           = (CsrBtCmPortnegReq *) cmData->recvMsgP;
    cmRfcConnElement * theElement    = CM_FIND_RFC_ELEMENT(CsrBtCmRfcFindRfcConnElementFromBtConnId, &(cmPrim->btConnId));

    if(theElement)
    { /* The CM has identify the index in the connection table */
        cmRfcConnInstType *theLogicalLink = theElement->cmRfcConnInst;

        if (theLogicalLink->state == connected_s)
        {/* Connection found. Send request to the RFC module */
            RfcPortnegReqSend(CM_GET_UINT16ID_FROM_BTCONN_ID(cmPrim->btConnId), FALSE, &cmPrim->portPar);
        }
    }
}

#endif /* #ifndef EXCLUDE_CSR_BT_RFC_MODULE */
