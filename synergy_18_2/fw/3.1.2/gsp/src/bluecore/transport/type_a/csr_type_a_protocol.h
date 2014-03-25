#ifndef CSR_TYPE_A_PROTOCOL_H__
#define CSR_TYPE_A_PROTOCOL_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_sdio.h"
#include "csr_pmem.h"
#include "csr_sched.h"
#include "csr_panic.h"
#include "csr_util.h"
#include "csr_type_a_private.h"

#ifdef __cplusplus
extern "C" {
#endif

/* queue definitions */
/* data to transmit to chip */
extern CsrInt32 CsrTypeATxQueueMessageGet(SdioATxData **ppTxData);
extern void CsrTypeATxQueueMessageRemove(SdioATxData **ppTxData);
/* data received from chip */
extern void CsrTypeARxQueueMsgPut(void *);

/* events to process */
extern void CsrSdioBtACardEvent(CsrSdioFunction *f, CsrBool inserted);

/* Internal interface */
extern void CsrSdioATxQueueSignal(void *);

/* SDIO Type-A protocol handler interface - implemented in csr_type_a_protocol.c */
extern void CsrSdioAProtReinit(void **gash);
extern void CsrSdioAProtInit(void **gash);
extern void CsrSdioACardEvent(void *p,
    CsrSdioFunction *f,
    CsrBool inserted);
extern void CsrSdioAProtDeinit(void **gash);

#define TYPE_A_TIME_MS (1000)
#define TYPE_A_PANIC(a, l) CsrPanic(CSR_TECH_FW, a, l)


#ifdef __cplusplus
}
#endif

#endif
