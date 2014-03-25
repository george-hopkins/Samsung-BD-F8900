/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_sched.h"
#include "csr_log_private.h"
#include "csr_msgconv.h"

#ifdef CSR_LOG_ENABLE

extern CsrLogDispatchCtx CsrLogCtx;

void CsrLogRegisterTechnology(const CsrLogTechInformation *techInfo)
{
    CsrUint32 primitiveInfoIdx;
    CsrLog *p;

    CsrMsgConvInit();

    for (primitiveInfoIdx = 0; primitiveInfoIdx < techInfo->primitiveInfoCount; ++primitiveInfoIdx)
    {
        CsrMsgConvInsert(techInfo->primitiveInfo[primitiveInfoIdx].primitiveType,
            techInfo->primitiveInfo[primitiveInfoIdx].messageConv);
    }

    for (p = CsrLogCtx.logList;
         p != NULL;
         p = p->next)
    {
        if (p->lregtech != NULL)
        {
            p->lregtech(p->ltHdl, techInfo);
        }
    }
}

#endif
