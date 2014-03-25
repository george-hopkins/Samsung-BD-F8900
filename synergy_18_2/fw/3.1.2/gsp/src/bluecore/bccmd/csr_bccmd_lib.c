/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/
#include "csr_bccmd_lib.h"
#include "csr_pmem.h"
#include "csr_bccmd_util.h"
#include "csr_tm_bluecore_bootstrap.h"

void CsrBccmdWriteReqSend(CsrSchedQid phandle,
    CsrUint16 varId,
    CsrUint16 seqNo,
    CsrUint16 payloadLength,
    CsrUint8 *payload)
{
    CsrUint16 bccmdLength;
    CsrUint8 *bccmd = CsrBccmdBccmdCommandBuild(CSR_BCCMD_SET_REQ, varId, seqNo, payloadLength, payload, &bccmdLength);
    CsrBccmdReqSend(phandle, bccmdLength, bccmd);
}

void CsrBccmdReadReqSend(CsrSchedQid phandle,
    CsrUint16 varId,
    CsrUint16 seqNo,
    CsrUint16 payloadLength,
    CsrUint8 *payload)
{
    CsrUint16 bccmdLength;
    CsrUint8 *bccmd = CsrBccmdBccmdCommandBuild(CSR_BCCMD_GET_REQ, varId, seqNo, payloadLength, payload, &bccmdLength);
    CsrBccmdReqSend(phandle, bccmdLength, bccmd);
}

void CsrBccmdReadPsValueReqSend(CsrSchedQid phandle,
    CsrUint16 seqNo,
    CsrUint16 key,
    CsrBccmdStoresType stores,
    CsrUint16 psValuelength)
{
    CsrUint16 bccmdLength;
    CsrUint8 *bccmd = CsrBccmdPsKeyCommandBuild(CSR_BCCMD_GET_REQ, seqNo, key, stores, psValuelength, NULL, &bccmdLength);
    CsrBccmdReqSend(phandle, bccmdLength, bccmd);
}

void CsrBccmdWritePsValueReqSend(CsrSchedQid phandle,
    CsrUint16 seqNo,
    CsrUint16 key,
    CsrBccmdStoresType stores,
    CsrUint16 psValuelength,
    CsrUint8 *psValue)
{
    CsrUint16 bccmdLength;
    CsrUint8 *bccmd = CsrBccmdPsKeyCommandBuild(CSR_BCCMD_SET_REQ, seqNo, key, stores, psValuelength, psValue, &bccmdLength);
    CsrBccmdReqSend(phandle, bccmdLength, bccmd);
}

CsrBccmdReq *CsrBccmdReq_struct(CsrSchedQid phandle, CsrUint16 payloadLength, CsrUint8 *payload)
{
    CsrBccmdReq *prim = (CsrBccmdReq *) CsrPmemAlloc(sizeof(CsrBccmdReq));
    prim->type = CSR_BCCMD_REQ;
    prim->phandle = phandle;
    prim->payloadLength = payloadLength;
    prim->payload = payload;
    return prim;
}

CsrBccmdFlushReq *CsrBccmdFlushReq_struct(CsrSchedQid phandle)
{
    CsrBccmdFlushReq *prim;
    prim = (CsrBccmdFlushReq *) CsrPmemAlloc(sizeof(CsrBccmdFlushReq));
    prim->type = CSR_BCCMD_FLUSH_REQ;
    prim->phandle = phandle;
    return prim;
}

CsrUint8 *CsrTmBlueCoreBuildBccmdSetMsg(CsrUint16 varId, CsrUint16 payloadLength, const CsrUint16 *payload)
{
    CsrUint16 bccmdLength;
    CsrUint16 bccmdPayloadLength = 0;
    CsrUint8 *bccmdPayload = NULL;

    if (payload)
    {
        CsrUint16 i;
        CsrUint8 *tmpPtr;

        bccmdPayloadLength = (payloadLength * 2);
        bccmdPayload = (CsrUint8 *) CsrPmemAlloc(bccmdPayloadLength);
        tmpPtr = bccmdPayload;

        for (i = 0; i < payloadLength; i++)
        {
            CSR_ADD_UINT16_TO_XAP(tmpPtr, payload[i]);
        }
    }
    return CsrBccmdBccmdCommandBuild(CSR_BCCMD_SET_REQ, varId, 0, bccmdPayloadLength, bccmdPayload, &bccmdLength);
}

CsrUint8 *CsrTmBlueCoreBuildBccmdPsSetMsg(CsrUint16 key, CsrUint16 stores, CsrUint16 psValuelength, const CsrUint16 *psValue)
{
    CsrUint16 bccmdLength;
    CsrUint16 psPayloadLength = 0;
    CsrUint8 *psPayload = NULL;

    if (psValue)
    {
        CsrUint16 i;
        CsrUint8 *tmpPtr;

        psPayloadLength = (psValuelength * 2);
        psPayload = (CsrUint8 *) CsrPmemAlloc(psPayloadLength);
        tmpPtr = psPayload;

        for (i = 0; i < psValuelength; i++)
        {
            CSR_ADD_UINT16_TO_XAP(tmpPtr, psValue[i]);
        }
    }
    return CsrBccmdPsKeyCommandBuild(CSR_BCCMD_SET_REQ, 0, key, stores, psPayloadLength, psPayload, &bccmdLength);
}
