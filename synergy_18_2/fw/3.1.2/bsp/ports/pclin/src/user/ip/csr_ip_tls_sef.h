#ifndef CSR_IP_TLS_SEF_H__
#define CSR_IP_TLS_SEF_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_ip_tls_handler.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrIpTlsHandler(CsrIpTlsInstanceData *, void *);
void csrIpTlsSocketsHandle(CsrIpTlsInstanceData *,
    int *, fd_set *, fd_set *);
void csrIpTlsSocketsReap(CsrIpTlsInstanceData *);

#ifdef __cplusplus
}
#endif

#endif
