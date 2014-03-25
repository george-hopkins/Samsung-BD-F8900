#ifndef CSR_ETHERNET_TASK_H__
#define CSR_ETHERNET_TASK_H__
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

/* Queue definition for ETHERNET */
extern CsrSchedQid CSR_ETHERNET_IFACEQUEUE;

/* Task definition for ETHERNET */
void CsrEthernetTaskInitialise(void **gash);
void CsrEthernetTaskDeinitialise(void **gash);
void CsrEthernetTaskHandler(void **gash);

#define CSR_ETHERNET_INIT CsrEthernetTaskInitialise
#define CSR_ETHERNET_HANDLER CsrEthernetTaskHandler
#ifdef ENABLE_SHUTDOWN
#define CSR_ETHERNET_DEINIT CsrEthernetTaskDeinitialise
#else
#define CSR_ETHERNET_DEINIT NULL
#endif

#ifdef __cplusplus
}
#endif

#endif
