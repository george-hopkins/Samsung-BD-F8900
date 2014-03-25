#ifndef CSR_DSPM_TASK_H__
#define CSR_DSPM_TASK_H__
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

extern CsrSchedQid CSR_DSPM_IFACEQUEUE;

void CsrDspmInit(void **gash);
void CsrDspmHandler(void **gash);
void CsrDspmDeinit(void **gash);

#define CSR_DSPM_INIT      CsrDspmInit
#define CSR_DSPM_HANDLER   CsrDspmHandler
#ifdef ENABLE_SHUTDOWN
#define CSR_DSPM_DEINIT    CsrDspmDeinit
#else
#define CSR_DSPM_DEINIT    NULL
#endif

#ifdef __cplusplus
}
#endif

#endif
