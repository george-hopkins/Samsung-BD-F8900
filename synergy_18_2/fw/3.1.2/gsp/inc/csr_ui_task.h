#ifndef CSR_UI_TASK_H__
#define CSR_UI_TASK_H__
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

/* Queue definition for CSR_UI */
extern CsrSchedQid CSR_UI_IFACEQUEUE;

/* Task definition for CSR_UI */
void CsrUiInitialise(void **gash);
void CsrUiDeinitialise(void **gash);
void CsrUiHandler(void **gash);

/* UI */
#define CSR_UI_INIT CsrUiInitialise
#define CSR_UI_HANDLER CsrUiHandler
#ifdef ENABLE_SHUTDOWN
#define CSR_UI_DEINIT CsrUiDeinitialise
#else
#define CSR_UI_DEINIT NULL
#endif

#ifdef __cplusplus
}
#endif

#endif /* CSR_UI_TASK_H__ */
