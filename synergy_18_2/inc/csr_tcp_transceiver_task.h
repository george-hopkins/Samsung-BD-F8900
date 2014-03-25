#ifndef CSR_TCP_TRANSCEIVER_TASK_H__
#define CSR_TCP_TRANSCEIVER_TASK_H__
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

extern CsrSchedQid CSR_TCP_TRANSCEIVER_IFACEQUEUE;
void CsrTcpTransceiverInit(void **gash);
void CsrTcpTransceiverDeinit(void **gash);
void CsrTcpTransceiverHandler(void **gash);

#define CSR_TCP_TRANSCEIVER_INIT      CsrTcpTransceiverInit
#define CSR_TCP_TRANSCEIVER_HANDLER   CsrTcpTransceiverHandler
#ifdef ENABLE_SHUTDOWN
#define CSR_TCP_TRANSCEIVER_DEINIT    CsrTcpTransceiverDeinit
#else
#define CSR_TCP_TRANSCEIVER_DEINIT    NULL
#endif

#ifdef __cplusplus
}
#endif

#endif
