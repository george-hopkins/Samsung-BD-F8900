#ifndef CSR_HQ_TASK_H__
#define CSR_HQ_TASK_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_sched.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Queue definition for CSR_HQ */
extern CsrSchedQid CSR_HQ_IFACEQUEUE;

/* HQ */
void CsrHqInit(void **gash);
void CsrHqHandler(void **gash);
void CsrHqDeinit(void **gash);

#define CSR_HQ_INIT      CsrHqInit
#define CSR_HQ_HANDLER   CsrHqHandler
#ifdef ENABLE_SHUTDOWN
#define CSR_HQ_DEINIT    CsrHqDeinit
#else
#define CSR_HQ_DEINIT    NULL
#endif

#ifdef __cplusplus
}
#endif

#endif
