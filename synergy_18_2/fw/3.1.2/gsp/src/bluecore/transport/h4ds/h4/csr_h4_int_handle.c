/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_sched.h"
#include "csr_queue_lib.h"

#include "csr_h4ds_inst.h"

extern H4DSInstanceData CsrH4DSInst;

void CsrH4PumpTxMsgsReq(void)
{
    CsrSchedBgintSet(CsrH4DSInst.bgint_pump);
}
