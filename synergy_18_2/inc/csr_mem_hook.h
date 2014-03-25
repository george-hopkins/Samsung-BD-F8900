#ifndef CSR_MEM_HOOK_H__
#define CSR_MEM_HOOK_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_util.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************

    NAME
        CsrMemHookSet

    DESCRIPTION
        This function is used to install the memory hooks and set up the
        amount of header and tail space to reserve in allocations.

    PARAMETERS
        cbAlloc -- callback for allocation
        cbFree -- callback for deallocation
        hdrSz -- size of header installed by allocation hook
        tailSz -- size of tail installed by allocation hook

    RETURNS
        Pointer to allocated memory.

*****************************************************************************/

#ifdef CSR_MEMALLOC_PROFILING

typedef void (*CsrMemHookAlloc)(void *hdr, void *buf,
                                CsrSize count, CsrSize size,
                                const CsrCharString *file, CsrUint32 line);
typedef void (*CsrMemHookFree)(void *hdr, void *buf);

void CsrMemHookSet(CsrMemHookAlloc allocCb, CsrMemHookFree freeCb,
    CsrMemHookAlloc allocDmaCb, CsrMemHookFree freeDmaCb,
    CsrSize hdrSz, CsrSize tailSz);

#endif

#ifdef __cplusplus
}
#endif

#endif /* CSR_MEM_HOOK_H__ */
