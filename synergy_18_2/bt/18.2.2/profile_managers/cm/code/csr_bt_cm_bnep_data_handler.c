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
#include "csr_bt_cm_util.h"

/*************************************************************************************
  CsrBtCmBnepExtendedDataReqHandler
************************************************************************************/
void CsrBtCmBnepExtendedDataReqHandler(cmInstanceData_t *cmData)
{ /* This event request the CM to transmit the data given in the payload. */
    CsrBtCmBnepExtendedDataReq    *cmPrim;
    cmPrim = (CsrBtCmBnepExtendedDataReq *) cmData->recvMsgP;
    CsrBtBnepExtendedDataReqSend(cmPrim->etherType, cmPrim->id, cmPrim->length, cmPrim->payload,
                            cmPrim->dstAddr, cmPrim->srcAddr);

}

/*************************************************************************************
  CsrBtCmBnepExtendedMulticastDataReqHandler
************************************************************************************/
void CsrBtCmBnepExtendedMulticastDataReqHandler(cmInstanceData_t *cmData)
{ /* This event request the CM to transmit the data given in the payload. */
    CsrBtCmBnepExtendedMulticastDataReq    *cmPrim;
    cmPrim = (CsrBtCmBnepExtendedMulticastDataReq *) cmData->recvMsgP;
    CsrBtBnepExtendedMultiCastDataReqSend(cmPrim->etherType, cmPrim->idNot, cmPrim->length, cmPrim->payload,
                                     cmPrim->dstAddr, cmPrim->srcAddr);
}

/*************************************************************************************
  CsrBtCmBnepExtendedDataIndHandler
************************************************************************************/
void CsrBtCmBnepExtendedDataIndHandler(cmInstanceData_t *cmData)
{ /* This event indicates that data has arrived from the remote device
     and is stored in the location given by payload. */

    CsrUint16                        mblkLength;
    BNEP_EXTENDED_DATA_IND_T        *prim;
    CsrBtCmBnepExtendedDataInd        *cmPrim;

    prim = (BNEP_EXTENDED_DATA_IND_T *) cmData->recvMsgP;
    mblkLength = CsrMblkGetLength(prim->mblk);

    if (mblkLength > 0)
    {
        cmPrim = (CsrBtCmBnepExtendedDataInd *) CsrPmemAlloc(sizeof(CsrBtCmBnepExtendedDataInd));

        cmPrim->payload = CsrPmemAlloc(mblkLength);

        cmPrim->type        = CSR_BT_CM_BNEP_EXTENDED_DATA_IND;
        cmPrim->id            = prim->id;
        cmPrim->etherType    = prim->ether_type;
        cmPrim->dstAddr        = prim->dst_addr;
        cmPrim->srcAddr        = prim->src_addr;
        cmPrim->length        = mblkLength;
        CsrMblkReadHead(&prim->mblk, cmPrim->payload, mblkLength);
        CsrBtCmPutMessage(cmData->bnepVar.appHandle, cmPrim);
    }
    else
    {
        CsrMblkDestroy(prim->mblk);
    }
}
#endif

