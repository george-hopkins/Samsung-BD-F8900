/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_sched.h"
#include "csr_log_private.h"
#include "csr_log_formats.h"
#include "csr_log_gsp.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_msgconv.h"

#ifdef CSR_LOG_ENABLE

CsrBool CsrLogEnvironmentIsFiltered(CsrLogLevelEnvironment level)
{
    CsrBool ret = FALSE;

    if (!(CsrLogCtx.logLevelFilterEnvironment & level))
    {
        ret = TRUE;
    }

    return ret;
}

static CsrBool csrLogTransportChannelMap(CsrUint8 channel, CsrLogLevelEnvironment *level, CsrUint8 *logChannel)
{
    CsrBool mapped = TRUE;

    switch (channel)
    {
        case TRANSPORT_CHANNEL_ACL:
        {
            *level = CSR_LOG_LEVEL_ENVIRONMENT_BCI_ACL;
            *logChannel = SYNERGY_BTI_TRANS_TYPE_ACL;
            break;
        }

        case TRANSPORT_CHANNEL_HCI:
        {
            *level = CSR_LOG_LEVEL_ENVIRONMENT_BCI_HCI;
            *logChannel = SYNERGY_BTI_TRANS_TYPE_HCI;
            break;
        }

        case TRANSPORT_CHANNEL_SCO:
        {
            *level = CSR_LOG_LEVEL_ENVIRONMENT_BCI_SCO;
            *logChannel = SYNERGY_BTI_TRANS_TYPE_SCO;
            break;
        }

        case TRANSPORT_CHANNEL_BCCMD:
        {
            *level = CSR_LOG_LEVEL_ENVIRONMENT_BCI_VENDOR;
            *logChannel = SYNERGY_BTI_TRANS_TYPE_BCCMD;
            break;
        }

        case TRANSPORT_CHANNEL_VM:
        {
            *level = CSR_LOG_LEVEL_ENVIRONMENT_BCI_VENDOR;
            *logChannel = SYNERGY_BTI_TRANS_TYPE_VM;
            break;
        }

        case TRANSPORT_CHANNEL_HQ:
        {
            *level = CSR_LOG_LEVEL_ENVIRONMENT_BCI_VENDOR;
            *logChannel = SYNERGY_BTI_TRANS_TYPE_HQ;
            break;
        }

        default:
        {
            mapped = FALSE;
            break;
        }
    }

    return mapped;
}

void CsrLogBci(CsrUint8 channel,
    CsrBool received,
    CsrSize payload_length,
    const void *payload)
{
    CsrLog *p;
    CsrLogLevelEnvironment level = 0; /* Init not needed but compiler complains */
    CsrUint8 logChannel;

    if (!csrLogTransportChannelMap(channel, &level, &logChannel))
    {
        return;
    }

    if (CsrLogEnvironmentIsFiltered(level))
    {
        return;
    }

    for (p = CsrLogCtx.logList;
         p != NULL;
         p = p->next)
    {
        if (p->lbci != NULL)
        {
            p->lbci(p->ltHdl, logChannel, received, payload_length, payload);
        }
    }
}

void CsrLogTransport(
    CsrUint8 transport_type,
    CsrUint8 direction,
    CsrSize consumed_length,
    CsrSize payload_length,
    const void *payload)
{
    CsrLog *p;
    CsrUint8 log_transport_type;

    switch (transport_type)
    {
        case TRANSPORT_TYPE_BCSP:
            log_transport_type = SYNERGY_TRANSPORT_TYPE_BCSP;
            break;

        default:
            return;
    }

    for (p = CsrLogCtx.logList;
         p != NULL;
         p = p->next)
    {
        if (p->ltrans != NULL)
        {
            p->ltrans(p->ltHdl,
                log_transport_type,
                direction,
                (CsrUint32) consumed_length,
                (CsrUint32) payload_length,
                payload);
        }
    }
}

void CsrLogBgintRegister(CsrUint8 thread_id,
    CsrSchedBgint irq,
    const CsrCharString *callback,
    const void *ptr)
{
    CsrLog *p;

    for (p = CsrLogCtx.logList;
         p != NULL;
         p = p->next)
    {
        if (p->bgintreg != NULL)
        {
            p->bgintreg(p->ltHdl, thread_id,
                irq, callback, ptr);
        }
    }
}

void CsrLogBgintUnregister(CsrSchedBgint irq)
{
    CsrLog *p;

    for (p = CsrLogCtx.logList;
         p != NULL;
         p = p->next)
    {
        if (p->bgintunreg != NULL)
        {
            p->bgintunreg(p->ltHdl, irq);
        }
    }
}

void CsrLogBgintSet(CsrSchedBgint irq)
{
    CsrLog *p;

    for (p = CsrLogCtx.logList;
         p != NULL;
         p = p->next)
    {
        if (p->bgintset != NULL)
        {
            p->bgintset(p->ltHdl, irq);
        }
    }
}

void CsrLogBgintServiceStart(CsrSchedBgint irq)
{
    CsrLog *p;

    for (p = CsrLogCtx.logList;
         p != NULL;
         p = p->next)
    {
        if (p->bgintservicestart != NULL)
        {
            p->bgintservicestart(p->ltHdl, irq);
        }
    }
}

void CsrLogBgintServiceDone(CsrSchedBgint irq)
{
    CsrLog *p;

    for (p = CsrLogCtx.logList;
         p != NULL;
         p = p->next)
    {
        if (p->bgintservicedone != NULL)
        {
            p->bgintservicedone(p->ltHdl, irq);
        }
    }
}

#endif
