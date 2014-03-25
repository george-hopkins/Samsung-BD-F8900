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

/*---------------------------------*/
/*  Scheduler logging */
/*---------------------------------*/
void CsrLogSchedInit(CsrUint8 thread_id)
{
    CsrLog *p;

    for (p = CsrLogCtx.logList;
         p != NULL;
         p = p->next)
    {
        if (p->lschedinit != NULL)
        {
            p->lschedinit(p->ltHdl, thread_id);
        }
    }
}

void CsrLogSchedDeinit(CsrUint8 thread_id)
{
    CsrLog *p;

    for (p = CsrLogCtx.logList;
         p != NULL;
         p = p->next)
    {
        if (p->lscheddeinit != NULL)
        {
            p->lscheddeinit(p->ltHdl, thread_id);
        }
    }
}

void CsrLogSchedStart(CsrUint8 thread_id)
{
    CsrLog *p;

    for (p = CsrLogCtx.logList;
         p != NULL;
         p = p->next)
    {
        if (p->lschedstart != NULL)
        {
            p->lschedstart(p->ltHdl, thread_id);
        }
    }
}

void CsrLogSchedStop(CsrUint8 thread_id)
{
    CsrLog *p;

    for (p = CsrLogCtx.logList;
         p != NULL;
         p = p->next)
    {
        if (p->lschedstop != NULL)
        {
            p->lschedstop(p->ltHdl, thread_id);
        }
    }
}

void CsrLogInitTask(CsrUint8 thread_id,
    CsrSchedQid tskid,
    const CsrCharString *tskName)
{
    CsrLog *p;

    for (p = CsrLogCtx.logList;
         p != NULL;
         p = p->next)
    {
        if (p->linittask != NULL)
        {
            p->linittask(p->ltHdl, thread_id, tskid, tskName);
        }
    }
}

void CsrLogDeinitTask(CsrSchedQid tskid)
{
    CsrLog *p;

    for (p = CsrLogCtx.logList;
         p != NULL;
         p = p->next)
    {
        if (p->ldeinittask != NULL)
        {
            p->ldeinittask(p->ltHdl, tskid);
        }
    }
}

#endif
