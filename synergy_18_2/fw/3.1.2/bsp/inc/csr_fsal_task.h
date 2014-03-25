#ifndef CSR_FSAL_TASK_H__
#define CSR_FSAL_TASK_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Queue definition for CSR_FSAL */
extern CsrUint16 CSR_FSAL_IFACEQUEUE;

/* Task definition for CSR_FSAL */
void CsrFsalInit(void **gash);
void CsrFsalDeinit(void **gash);
void CsrFsalHandler(void **gash);

/* CSR_FSAL */
#define CSR_FSAL_INIT CsrFsalInit
#define CSR_FSAL_HANDLER CsrFsalHandler
#ifdef ENABLE_SHUTDOWN
#define CSR_FSAL_DEINIT CsrFsalDeinit
#else
#define CSR_FSAL_DEINIT NULL
#endif


#ifdef __cplusplus
}
#endif

#endif
