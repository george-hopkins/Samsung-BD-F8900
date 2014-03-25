#ifndef CSR_BCCMD_TASK_H__
#define CSR_BCCMD_TASK_H__
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

/* Queue definition for CSR_BCCMD */
extern CsrSchedQid CSR_BCCMD_IFACEQUEUE;

/* BCCMD */
void CsrBccmdInit(void **gash);
void CsrBccmdHandler(void **gash);
void CsrBccmdDeinit(void **gash);

#define CSR_BCCMD_INIT      CsrBccmdInit
#define CSR_BCCMD_HANDLER   CsrBccmdHandler
#ifdef ENABLE_SHUTDOWN
#define CSR_BCCMD_DEINIT    CsrBccmdDeinit
#else
#define CSR_BCCMD_DEINIT    NULL
#endif

#ifdef __cplusplus
}
#endif

#endif
