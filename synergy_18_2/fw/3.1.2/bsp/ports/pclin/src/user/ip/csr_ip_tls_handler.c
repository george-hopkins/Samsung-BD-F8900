/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_sched.h"
#include "csr_prim_defs.h"

#include "csr_tls_prim.h"
#include "csr_tls_lib.h"

#include "csr_ip_util.h"

#include "csr_ip_tls_thread.h"
#include "csr_ip_tls_handler.h"
#include "csr_ip_tls_util.h"
#include "csr_ip_tls_sef.h"

#include <openssl/crypto.h>

CsrIpTlsInstanceData *globalCsrIpTlsInstanceData = NULL;

void CsrIpTlsInit(CsrIpTlsInstanceData *instanceData,
    void *lowerArg, CsrUint8 *buffer, CsrSize bufSize)
{
    /*added to be able to find psk key in callback*/
    globalCsrIpTlsInstanceData = instanceData;

    instanceData->ipInstanceData = lowerArg;

    SSL_library_init();
    SSL_load_error_strings();

    instanceData->maxSocketHandle = -1;
    instanceData->rxBufferSize = bufSize;
    instanceData->rxBuffer = buffer;
}

#ifdef ENABLE_SHUTDOWN
void CsrIpTlsDeinit(CsrIpTlsInstanceData *instanceData)
{
#if 0
    void *message;
    CsrUint16 event;
#endif

    CsrUint32 i;

    for (i = 0; i < MAX_SOCKETS; i++)
    {
        if (instanceData->socketInstance[i] != NULL)
        {
            CsrTlsSocketInst *sockInst;

            sockInst = instanceData->socketInstance[i];

            csrTlsSocketInstFree(instanceData, sockInst);
        }
    }

    while (instanceData->tlsSessionList)
    {
        csrIpTlsSessionDestroy(instanceData, instanceData->tlsSessionList);
    }

    CRYPTO_cleanup_all_ex_data();

    globalCsrIpTlsInstanceData = NULL;

#if 0
    /* Clean queue */
    while (CsrSchedMessageGet(&event, &message))
    {
        switch (event)
        {
            case CSR_TLS_PRIM:
            {
                CsrTlsFreeDownstreamMessageContents(event, message);
                break;
            }
            case CSR_TLS_INTERNAL_PRIM:
            {
                csrIpMessageContainer *container;

                container = message;

                /* We only encapsulate upstream TLS primitives. */
                CsrTlsFreeUpstreamMessageContents(container->prim,
                    container->message);
                break;
            }
            default:
                break;
        }
        CsrPmemFree(message);
    }
#endif
}

#endif
