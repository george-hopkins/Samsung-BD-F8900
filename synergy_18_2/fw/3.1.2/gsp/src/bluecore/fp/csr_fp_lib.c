/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_fp_lib.h"

CsrFpCreateReq *CsrFpCreateReq_struct(CsrSchedQid appHandle,
    CsrUint32 overheadHost,                                          /* Pipe overhead on the host */
    CsrUint32 capacityRxHost,                                        /* Capacity of receive buffer on the host */
    CsrUint32 requiredTxController,                                  /* Required capacity of tx buffer on controller */
    CsrUint32 desiredTxController,                                   /* Desired capacity of tx buffer on controller */
    CsrUint32 requiredRxController,                                  /* Required capacity of rx buffer on controller */
    CsrUint32 desiredRxController)                                  /* Desired capacity of rx buffer on controller */
{
    CsrFpCreateReq *msg = CsrPmemAlloc(sizeof(CsrFpCreateReq));

    msg->type = CSR_FP_CREATE_REQ;
    msg->appHandle = appHandle;
    msg->overheadHost = overheadHost;
    msg->capacityRxHost = capacityRxHost;
    msg->requiredTxController = requiredTxController;
    msg->desiredTxController = desiredTxController;
    msg->requiredRxController = requiredRxController;
    msg->desiredRxController = desiredRxController;
    return msg;
}

CsrFpWriteReq *CsrFpWriteReq_struct(CsrFpHandleType fpHandle,
    CsrMblk *data)
{
    CsrFpWriteReq *msg = CsrPmemAlloc(sizeof(CsrFpWriteReq));

    msg->type = CSR_FP_WRITE_REQ;
    msg->fpHandle = fpHandle;
    msg->data = data;
    return msg;
}

CsrFpClearReq *CsrFpClearReq_struct(CsrFpHandleType fpHandle)
{
    CsrFpClearReq *msg = CsrPmemAlloc(sizeof(CsrFpClearReq));

    msg->type = CSR_FP_CLEAR_REQ;
    msg->fpHandle = fpHandle;
    return msg;
}

CsrFpDestroyReq *CsrFpDestroyReq_struct(CsrFpHandleType fpHandle)
{
    CsrFpDestroyReq *msg = CsrPmemAlloc(sizeof(CsrFpDestroyReq));

    msg->type = CSR_FP_DESTROY_REQ;
    msg->fpHandle = fpHandle;
    return msg;
}
