#ifndef CSR_ETHERNET_MANAGER_TASK_H__
#define CSR_ETHERNET_MANAGER_TASK_H__
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

/* Queue definition for CSR_ETHERNET_MANAGER */
extern CsrSchedQid CSR_ETHERNET_MANAGER_IFACEQUEUE;

/* Task definition for CSR_EXAMPLE_NETMGR */
void CsrEthernetManagerInit(void **gash);
void CsrEthernetManagerDeinit(void **gash);
void CsrEthernetManagerHandler(void **gash);

#define CSR_ETHERNET_MANAGER_INIT      CsrEthernetManagerInit
#define CSR_ETHERNET_MANAGER_HANDLER   CsrEthernetManagerHandler
#ifdef ENABLE_SHUTDOWN
#define CSR_ETHERNET_MANAGER_DEINIT    CsrEthernetManagerDeinit
#else
#define CSR_ETHERNET_MANAGER_DEINIT    NULL
#endif

#ifdef __cplusplus
}
#endif

#endif
