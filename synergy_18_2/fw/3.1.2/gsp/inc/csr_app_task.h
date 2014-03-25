#ifndef CSR_APP_TASK_H__
#define CSR_APP_TASK_H__
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

/* Queue definition for CSR_APP */
extern CsrSchedQid CSR_APP_IFACEQUEUE;

/* Task definition for CSR_APP */
void CsrAppInitialization(void **gash);
void CsrAppDeinitialization(void **gash);
void CsrAppHandler(void **gash);

/* APP */
#define CSR_APP_INIT CsrAppInitialization
#define CSR_APP_HANDLER CsrAppHandler
#ifdef ENABLE_SHUTDOWN
#define CSR_APP_DEINIT CsrAppDeinitialization
#else
#define CSR_APP_DEINIT NULL
#endif

#ifdef __cplusplus
}
#endif

#endif
