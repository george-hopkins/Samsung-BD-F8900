#ifndef CSR_TLS_SEF_TLS_H__
#define CSR_TLS_SEF_TLS_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_tls_handler.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrTlsTlsHandler(CsrTlsInstanceData *);
csrTlsSession *sessionCreateHandler(CsrTlsInstanceData *instanceData, CsrSchedQid qid);
void sessionDestroyHandler(CsrTlsInstanceData *instanceData, csrTlsSession *session);

csrTlsConnection *connectionCreateReqHandler(csrTlsSession *session);

void connectionDestroyReqHandler(csrTlsConnection *conn);


#ifdef __cplusplus
}
#endif

#endif
