/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifdef CSR_AMP_ENABLE

#include "csr_bt_cm_l2cap.h"
#include "csr_bt_cm_rfc.h"
#include "csr_bt_cm_private_lib.h"

void CsrBtCmAmpMoveChannelCfmSend(cmInstanceData_t *inst,
                                  CsrBtConnId btConnId,
                                  CsrBtAmpController localControl,
                                  CsrBtDeviceAddr addr,
                                  CsrBtResultCode resultCode,
                                  CsrBtSupplier resultSupplier)
{
    CsrBtCmMoveChannelCfm *prim = (CsrBtCmMoveChannelCfm*)CsrPmemAlloc(sizeof(CsrBtCmMoveChannelCfm));

    prim->type = CSR_BT_CM_MOVE_CHANNEL_CFM;
    prim->btConnId = btConnId;
    prim->localControl = localControl;
    prim->deviceAddr = addr;
    prim->resultCode = resultCode;
    prim->resultSupplier = resultSupplier;

    CsrBtCmPutMessage(inst->ampmHandle, prim);
}

void CsrBtCmAmpMoveChannelCmpIndSend(cmInstanceData_t *inst,
                                     CsrBtConnId btConnId,
                                     CsrBtAmpController localControl,
                                     CsrBtDeviceAddr addr,
                                     CsrBtResultCode resultCode,
                                     CsrBtSupplier resultSupplier)
{
    CsrBtCmMoveChannelCmpInd *prim = (CsrBtCmMoveChannelCmpInd*)CsrPmemAlloc(sizeof(CsrBtCmMoveChannelCmpInd));

    prim->type = CSR_BT_CM_MOVE_CHANNEL_CMP_IND;
    prim->btConnId = btConnId;
    prim->localControl = localControl;
    prim->deviceAddr = addr;
    prim->resultCode = resultCode;
    prim->resultSupplier = resultSupplier;

    CsrBtCmPutMessage(inst->ampmHandle, prim);
}

void CsrBtCmAmpMoveChannelIndSend(cmInstanceData_t *inst,
                                  CsrBtConnId btConnId,
                                  CsrBtAmpController localControl,
                                  CsrBtDeviceAddr addr)
{
    CsrBtCmMoveChannelInd *prim = (CsrBtCmMoveChannelInd*) CsrPmemAlloc(sizeof(CsrBtCmMoveChannelInd));

    prim->type = CSR_BT_CM_MOVE_CHANNEL_IND;
    prim->btConnId = btConnId;
    prim->localControl = localControl;
    prim->deviceAddr = addr;

    CsrBtCmPutMessage(inst->ampmHandle, prim);
}

/* Downstream handler for move channel request */
void CsrBtCmAmpMoveChannelReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmMoveChannelReq *cmPrim = (CsrBtCmMoveChannelReq*)cmData->recvMsgP;

    if(cmData->ampmHandle == CSR_SCHED_QID_INVALID)
    {
        /* No AMPM registered - can not handle this request */
        return ;
    }

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
    if(CSR_BT_CONN_ID_IS_L2CA(cmPrim->btConnId))
    {
        CsrBtCmL2caMoveChannelReqHandler(cmData);
    }
#endif
#ifndef EXCLUDE_CSR_BT_RFC_MODULE
    if(CSR_BT_CONN_ID_IS_RFC(cmPrim->btConnId))
    {
        CsrBtCmRfcAmpMoveChannelReqHandler(cmData);
    }
#endif
}

/* Downstream handler for move channel response */
void CsrBtCmAmpMoveChannelResHandler(cmInstanceData_t *cmData)
{
    CsrBtCmMoveChannelRes *cmPrim = (CsrBtCmMoveChannelRes*)cmData->recvMsgP;

    if(cmData->ampmHandle == CSR_SCHED_QID_INVALID)
    {
        /* No AMPM registered - can not handle this request */
        return ;
    }

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
    if(CSR_BT_CONN_ID_IS_L2CA(cmPrim->btConnId))
    {
        CsrBtCmL2caMoveChannelResHandler(cmData);        
    }
#endif
#ifndef EXCLUDE_CSR_BT_RFC_MODULE
    if(CSR_BT_CONN_ID_IS_RFC(cmPrim->btConnId))
    {
        CsrBtCmRfcAmpMoveChannelResHandler(cmData);
    }
#endif
}

/* Force link in to sniff mode */
void CsrBtCmAmpForceModeChange(cmInstanceData_t *cmData,
                               CsrBtDeviceAddr *addr)
{
    /* The caller can be both RFC and L2CAP. Use RFCOMM's
     * btConnId if it's on AMP, otherwise use L2CAP's */
#ifndef EXCLUDE_CSR_BT_RFC_MODULE
    {
        cmRfcConnElement *rfcElm;
        for (rfcElm = CM_RFC_GET_FIRST(cmData->rfcVar.connList);
             rfcElm; 
             rfcElm = rfcElm->next)
        {
            if (rfcElm->cmRfcConnInst)
            {
                if(bd_addr_eq(addr, &(rfcElm->cmRfcConnInst->deviceAddr)))
                {
                    CsrBtCmRfcAmpForceModeChangeReqSend(rfcElm->cmRfcConnInst->btConnId);
                    return;
                }
            }
        }
    }
#endif
#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
    {
        cmL2caConnElement *l2capElm;
        for (l2capElm = CM_L2CA_GET_FIRST(cmData->l2caVar.connList);
             l2capElm;
             l2capElm = l2capElm->next)
        {
            /* Search through the l2ca list */
            if (l2capElm->cmL2caConnInst)
            {
                if(bd_addr_eq(addr, &(l2capElm->cmL2caConnInst->deviceAddr)))
                {
                    CsrBtCml2caAmpForceModeChangeReqSend(l2capElm->cmL2caConnInst->btConnId);
                    return;
                }
            }
        }
    }
#endif

    /* Noting matched, ignore */
}

#endif /* #ifdef CSR_AMP_ENABLE */
