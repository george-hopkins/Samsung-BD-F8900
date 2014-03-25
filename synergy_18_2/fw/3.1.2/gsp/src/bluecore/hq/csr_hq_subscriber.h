#ifndef CSR_HQ_SUBSCRIBER_H__
#define CSR_HQ_SUBSCRIBER_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_hq_handler.h"
#include "csr_sched.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct CsrHqSubscriptionTag
{
    struct CsrHqSubscriptionTag *next;
    struct CsrHqSubscriptionTag *prev;
    CsrSchedQid                  appHandle;
    CsrHqVarIdType               varId;
    CsrBool                      response;
} CsrHqSubscription;

void CsrHqInsertSubscriber(CsrHqInstanceData *hqInst, CsrSchedQid appH, CsrHqVarIdType varId, CsrBool response);
void CsrHqRemoveSubscriber(CsrHqInstanceData *hqInst, CsrSchedQid appH);
CsrResult CsrHqRemoveSubscriberSpecific(CsrHqInstanceData *hqInst, CsrHqDeregisterReq *prim);

#ifdef __cplusplus
}
#endif

#endif
