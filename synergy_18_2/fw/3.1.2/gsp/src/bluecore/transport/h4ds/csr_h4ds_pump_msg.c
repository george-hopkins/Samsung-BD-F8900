/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/
#include "csr_h4.h"
#include "csr_h4_tx.h"
#include "csr_h4_int_handle.h"
#include "csr_h4ds.h"
#include "csr_h4ds_inst.h"

#ifdef __cplusplus
extern "C" {
#endif

extern H4InstanceData CsrH4Inst;
extern H4DSInstanceData CsrH4DSInst;

void CsrH4DSMsgPump(void *unused)
{
    if (CsrH4DSInst.pumpRequest > 0)
    {
        CsrH4DSInst.pumpRequest--;
        CsrH4DSPump();
    }

    if (CsrH4Inst.pumpRequest > 0)
    {
        CsrH4Inst.pumpRequest--;
        CsrH4TxMsgsPump();
    }

    if ((CsrH4Inst.pumpRequest > 0) || (CsrH4DSInst.pumpRequest > 0))
    {
        CsrH4PumpTxMsgsReq();
    }
}

void CsrH4DSPumpTxMsgsReq(void)
{
    if (CsrH4DSInst.txBufNotReadyTid == 0)
    {
        CsrH4DSInst.pumpRequest++;
        CsrH4PumpTxMsgsReq();
    }
}

void CsrH4PumpTxReq(void)
{
    CsrH4Inst.pumpRequest++;
    CsrH4PumpTxMsgsReq();
}

#ifdef __cplusplus
}
#endif
