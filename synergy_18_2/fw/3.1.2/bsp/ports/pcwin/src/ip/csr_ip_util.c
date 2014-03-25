/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_sched.h"
#include "csr_result.h"
#include "csr_prim_defs.h"

#include "csr_ip_util.h"
#include "csr_ip_task.h"


void CsrIpMessageForward(CsrSchedQid queue, CsrPrim prim, void *message)
{
    csrIpMessageContainer *container;

    container = CsrPmemAlloc(sizeof(*container));
    container->queue = queue;
    container->prim = prim;
    container->message = message;

    CsrSchedMessagePut(CSR_IP_IFACEQUEUE, CSR_IP_INTERNAL_PRIM, container);
}
