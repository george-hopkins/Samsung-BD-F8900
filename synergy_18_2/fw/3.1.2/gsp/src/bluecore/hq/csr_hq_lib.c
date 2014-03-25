/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_hq_lib.h"
#include "csr_pmem.h"
/* --------------------------------------------------------------------
   Name
       CSR_HQ_REGISTER_REQ

   Description
       Register an event in the CSR HQ task

 * -------------------------------------------------------------------- */
CsrHqRegisterReq *CsrHqRegisterReq_struct(CsrSchedQid pHandle, CsrHqVarIdType varId, CsrBool response)
{
    CsrHqRegisterReq *prim;

    prim = (CsrHqRegisterReq *) CsrPmemAlloc(sizeof(CsrHqRegisterReq));
    prim->type = CSR_HQ_REGISTER_REQ;
    prim->phandle = pHandle;
    prim->varId = varId;
    prim->response = response;

    return prim;
}

/* --------------------------------------------------------------------
   Name
       CSR_HQ_DEREGISTER_REQ

   Description
       Deregister an event in the CSR HQ task

 * -------------------------------------------------------------------- */
CsrHqDeregisterReq *CsrHqDeregisterReq_struct(CsrSchedQid pHandle, CsrHqVarIdType varId)
{
    CsrHqDeregisterReq *prim;

    prim = (CsrHqDeregisterReq *) CsrPmemAlloc(sizeof(CsrHqDeregisterReq));
    prim->type = CSR_HQ_DEREGISTER_REQ;
    prim->phandle = pHandle;
    prim->varId = varId;

    return prim;
}

/* --------------------------------------------------------------------
   Name
       CSR_HQ_MSG_RES

   Description
        Send a response to a CSR_HQ_MSG_IND. This message shall always be sent
        if and only if the response member was set to TRUE in the
        CSR_HQ_REGISTER_REQ.

 * -------------------------------------------------------------------- */
CsrHqMsgRes *CsrHqMsgRes_struct(CsrUint16 cmdType, CsrUint16 seqNo, CsrHqVarIdType varId, CsrUint16 status, CsrUint16 payloadLength, CsrUint8 *payload)
{
    CsrHqMsgRes *prim;

    prim = (CsrHqMsgRes *) CsrPmemAlloc(sizeof(CsrHqMsgRes));
    prim->type = CSR_HQ_MSG_RES;
    prim->cmdType = cmdType;
    prim->seqNo = seqNo;
    prim->varId = varId;
    prim->status = status;
    prim->payloadLength = payloadLength;
    prim->payload = payload;

    return prim;
}
