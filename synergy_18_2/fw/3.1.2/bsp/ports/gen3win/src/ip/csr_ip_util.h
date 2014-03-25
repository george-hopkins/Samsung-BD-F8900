#ifndef CSR_IP_UTIL_H__
#define CSR_IP_UTIL_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_prim_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    void       *message;
    CsrPrim     prim;
    CsrSchedQid queue;
} csrIpMessageContainer;

void CsrIpMessageForward(CsrSchedQid, CsrPrim, void *);

#ifdef __cplusplus
}
#endif

#endif
