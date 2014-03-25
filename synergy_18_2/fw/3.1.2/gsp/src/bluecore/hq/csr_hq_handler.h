#ifndef CSR_HQ_HANDLER_H__
#define CSR_HQ_HANDLER_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_usr_config.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_hq_prim.h"
#include "csr_list.h"
#include "csr_sched.h"
#include "csr_log_text_2.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DECLARE(CsrHqLto);

typedef struct
{
    CsrCmnList_t subscribers;
#ifdef CSR_BLUECORE_ONOFF
    CsrBool deactivating;
#endif
} CsrHqInstanceData;

/* Prototypes from csr_hq_free_down.c */
void CsrHqFreeDownstreamMessageContents(CsrUint16 eventClass, void *message);

#ifdef __cplusplus
}
#endif

#endif /* CSR_HQ_HANDLER_H__ */
