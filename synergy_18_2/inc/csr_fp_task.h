#ifndef CSR_FP_TASK_H__
#define CSR_FP_TASK_H__
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

/* Queue definition for CSR_FP */
extern CsrSchedQid CSR_FP_IFACEQUEUE;

/* FP */
void CsrFpInit(void **gash);
void CsrFpHandler(void **gash);
void CsrFpDeinit(void **gash);

#define CSR_FP_INIT         CsrFpInit
#define CSR_FP_HANDLER      CsrFpHandler
#ifdef ENABLE_SHUTDOWN
#define CSR_FP_DEINIT       CsrFpDeinit
#else
#define CSR_FP_DEINIT       NULL
#endif

#ifdef __cplusplus
}
#endif

#endif
