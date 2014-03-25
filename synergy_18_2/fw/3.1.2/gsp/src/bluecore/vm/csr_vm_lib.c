/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/
#include "csr_vm_lib.h"
#include "csr_pmem.h"

CsrVmRegisterReq *CsrVmRegisterReq_struct(CsrSchedQid pHandle)
{
    CsrVmRegisterReq *prim = (CsrVmRegisterReq *) CsrPmemAlloc(sizeof(CsrVmRegisterReq));
    prim->type = CSR_VM_REGISTER_REQ;
    prim->phandle = pHandle;
    return prim;
}

CsrVmDataReq *CsrVmDataReq_struct(CsrUint8 *payload, CsrUint16 payloadLength)
{
    CsrVmDataReq *prim = (CsrVmDataReq *) CsrPmemAlloc(sizeof(CsrVmDataReq));
    prim->type = CSR_VM_DATA_REQ;
    prim->payload = payload;
    prim->payloadLength = payloadLength;
    return prim;
}
