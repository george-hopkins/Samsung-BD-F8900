#ifndef CSR_DHCP_SERVER_TASK_H__
#define CSR_DHCP_SERVER_TASK_H__
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

/* Queue definition for CSR_DHCP_SERVER */
extern CsrSchedQid CSR_DHCP_SERVER_IFACEQUEUE;

/* Task definition for CSR_DHCP_SERVER */
void CsrDhcpServerInit(void **gash);
void CsrDhcpServerDeinit(void **gash);
void CsrDhcpServerHandler(void **gash);

/* CSR_DHCP_SERVER */
#define CSR_DHCP_SERVER_INIT CsrDhcpServerInit
#define CSR_DHCP_SERVER_HANDLER CsrDhcpServerHandler
#ifdef ENABLE_SHUTDOWN
#define CSR_DHCP_SERVER_DEINIT CsrDhcpServerDeinit
#else
#define CSR_DHCP_SERVER_DEINIT NULL
#endif

#ifdef __cplusplus
}
#endif

#endif
