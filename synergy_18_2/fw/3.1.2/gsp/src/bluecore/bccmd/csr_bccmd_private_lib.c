/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_bccmd_private_prim.h"
#include "csr_pmem.h"

CsrBccmdBootstrapReq *CsrBccmdBootstrapReq_struct(CsrSchedQid phandle, CsrUint16 payloadLength, CsrUint8 *payload)
{
    CsrBccmdBootstrapReq *prim = (CsrBccmdBootstrapReq *) CsrPmemAlloc(sizeof(CsrBccmdBootstrapReq));
    prim->type = CSR_BCCMD_BOOTSTRAP_REQ;
    prim->phandle = phandle;
    prim->payloadLength = payloadLength;
    prim->payload = payload;
    return prim;
}

CsrBccmdResetRes *CsrBccmdResetRes_struct(CsrResult status)
{
    CsrBccmdResetRes *prim = (CsrBccmdResetRes *) CsrPmemAlloc(sizeof(CsrBccmdResetRes));
    prim->type = CSR_BCCMD_RESET_RES;
    prim->status = status;
    return prim;
}

CsrBccmdGetBuildIdReq *CsrBccmdGetBuildIdReq_struct(CsrSchedQid phandle)
{
    CsrBccmdGetBuildIdReq *prim = (CsrBccmdGetBuildIdReq *) CsrPmemAlloc(sizeof(CsrBccmdGetBuildIdReq));
    prim->type = CSR_BCCMD_GET_BUILD_ID_REQ;
    prim->phandle = phandle;
    return prim;
}

CsrBccmdQueryReq *CsrBccmdQueryReq_struct(void)
{
    CsrBccmdQueryReq *prim = (CsrBccmdQueryReq *) CsrPmemAlloc(sizeof(CsrBccmdQueryReq));
    prim->type = CSR_BCCMD_QUERY_REQ;
    return prim;
}

CsrBccmdPanicArgsReq *CsrBccmdPanicArgsReq_struct(CsrSchedQid phandle)
{
    CsrBccmdPanicArgsReq *prim = (CsrBccmdPanicArgsReq *) CsrPmemAlloc(sizeof(CsrBccmdPanicArgsReq));
    prim->type = CSR_BCCMD_PANIC_ARGS_REQ;
    prim->phandle = phandle;
    return prim;
}
