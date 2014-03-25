#ifndef CSR_TLS_THREAD_H__
#define CSR_TLS_THREAD_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_sched.h"

#include "csr_tls_handler.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CSR_TLS_TYPE_TCP_CONNECT      0x0000
#define CSR_TLS_TYPE_TCP_SSL_CONNECT  0x0001
#define CSR_TLS_TYPE_TCP_DATA         0x0002

void CsrTlsThreadInit(CsrTlsThreadInstanceData *instanceData, CsrTlsInstanceData *taskInstanceData);
void CsrTlsThreadDeinit(CsrTlsThreadInstanceData *instanceData);
void CsrTlsSessionCreateHandler(CsrTlsInstanceData *taskInstanceData);
void CsrTlsSessionDestroyHandler(CsrTlsInstanceData *taskInstanceData);
void CsrTlsConnectionCreateReqHandler(CsrTlsInstanceData *taskInstanceData);
void CsrTlsConnectionBindReqHandler(CsrTlsInstanceData *taskInstanceData);
void CsrTlsConnectionDestroyReqHandler(CsrTlsInstanceData *taskInstanceData);
void CsrTlsConnectionConnectReqHandler(CsrTlsInstanceData *taskInstanceData);
void CsrTlsConnectionDataReqHandler(CsrTlsInstanceData *taskInstanceData);
void CsrTlsConnectionDataResHandler(CsrTlsInstanceData *taskInstanceData);
void CsrTlsConnectionDisconnectReqHandler(CsrTlsInstanceData *taskInstanceData);
void CsrTlsConnectionDisconnectResHandler(CsrTlsInstanceData *taskInstanceData);
void CsrTlsConnectionCertResHandler(CsrTlsInstanceData *taskInstanceData);

#ifdef __cplusplus
}
#endif

#endif
