#ifndef CSR_IPS_MANAGER_TASK_H__
#define CSR_IPS_MANAGER_TASK_H__
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

/* Queue definition for CSR_IPS_MANAGER */
extern CsrSchedQid CSR_IPS_MANAGER_IFACEQUEUE;

/* Task definition for CSR_IPS_MANAGER */
void CsrIpsManagerInit(void **gash);
void CsrIpsManagerDeinit(void **gash);
void CsrIpsManagerHandler(void **gash);

#define CSR_IPS_MANAGER_INIT      CsrIpsManagerInit
#define CSR_IPS_MANAGER_HANDLER   CsrIpsManagerHandler
#ifdef ENABLE_SHUTDOWN
#define CSR_IPS_MANAGER_DEINIT    CsrIpsManagerDeinit
#else
#define CSR_IPS_MANAGER_DEINIT    NULL
#endif

#ifdef __cplusplus
}
#endif

#endif
