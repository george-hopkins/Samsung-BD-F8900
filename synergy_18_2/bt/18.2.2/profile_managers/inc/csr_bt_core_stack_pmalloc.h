#ifndef CSR_BT_CORE_STACK_PMALLOC_H__
#define CSR_BT_CORE_STACK_PMALLOC_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_bt_bluestack_types.h"
#include "csr_pmem.h"
#include "csr_panic.h"
#include "csr_bt_util.h"
#include "csr_util.h"

#ifdef CSR_LOG_ENABLE
#include "csr_log.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef pmalloc
#define pmalloc(x) CsrPmemAlloc((x))
#endif

#ifndef zpmalloc
#define zpmalloc(x) CsrPmemZalloc((x))
#endif

#ifndef xpmalloc
#define xpmalloc pmalloc
#endif

#ifndef xzpmalloc
#define xzpmalloc zpmalloc
#endif

#ifndef pcopy
#define pcopy CsrMemDup
#endif

#ifndef xpcopy
#define xpcopy pcopy
#endif

#ifndef xpnew
#define xpnew(t)    ((t *)( pmalloc( sizeof(t) )))
#endif

#ifndef xzpnew
#define xzpnew(t)    ((t *)( zpmalloc( sizeof(t) )))
#endif

#ifndef pfree
#define pfree(x) CsrPmemFree((x))
#endif

extern void pdestroy_array(void **ptr, CsrUint16 num);

#ifdef __cplusplus
}
#endif

#endif
