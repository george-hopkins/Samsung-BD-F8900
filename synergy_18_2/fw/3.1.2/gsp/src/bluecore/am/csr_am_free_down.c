/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_AM_MODULE
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_autogen.h"
#include "csr_am_lib.h"
#include "csr_am_prim.h"

void CsrAmFreeDownstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_AM_PRIM)
    {
        CsrAmPrim *prim = (CsrAmPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_CONFIG_REQ
            case CSR_AM_AUDIO_PATH_CONFIG_REQ:
            {
                CsrAmAudioPathConfigReq *p = message;
                CsrPmemFree(p->configData);
                p->configData = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_CONFIG_REQ */
#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_CONNECT_REQ
            case CSR_AM_AUDIO_PATH_CONNECT_REQ:
            {
                CsrAmAudioPathConnectReq *p = message;
                CsrPmemFree(p->source.configData);
                p->source.configData = NULL;
                CsrPmemFree(p->sourceR.configData);
                p->sourceR.configData = NULL;
                CsrPmemFree(p->sinks.newSinkId);
                p->sinks.newSinkId = NULL;
                CsrPmemFree(p->sinks.sinkConfig);
                p->sinks.sinkConfig = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_CONNECT_REQ */
#ifndef EXCLUDE_CSR_AM_AUDIO_PATH_ADD_SINK_REQ
            case CSR_AM_AUDIO_PATH_ADD_SINK_REQ:
            {
                CsrAmAudioPathAddSinkReq *p = message;
                CsrPmemFree(p->sinks.newSinkId);
                p->sinks.newSinkId = NULL;
                CsrPmemFree(p->sinks.sinkConfig);
                p->sinks.sinkConfig = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_AM_AUDIO_PATH_ADD_SINK_REQ */
            default:
            {
                break;
            }
        } /* End switch */
    } /* End if */
    else
    {
        /* Unknown primitive type, exception handling */
    }
}
#endif /* EXCLUDE_CSR_AM_MODULE */
