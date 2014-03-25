#ifndef CSR_BLUECORE_MANAGER_TASK_H__
#define CSR_BLUECORE_MANAGER_TASK_H__
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

extern CsrSchedQid CSR_BLUECORE_MANAGER_IFACEQUEUE;

void CsrBluecoreManagerInit(void **gash);
void CsrBluecoreManagerDeinit(void **gash);
void CsrBluecoreManagerHandler(void **gash);

#define CSR_BLUECORE_MANAGER_INIT      CsrBluecoreManagerInit
#define CSR_BLUECORE_MANAGER_HANDLER   CsrBluecoreManagerHandler
#ifdef ENABLE_SHUTDOWN
#define CSR_BLUECORE_MANAGER_DEINIT    CsrBluecoreManagerDeinit
#else
#define CSR_BLUECORE_MANAGER_DEINIT    NULL
#endif

#ifdef __cplusplus
}
#endif

#endif
