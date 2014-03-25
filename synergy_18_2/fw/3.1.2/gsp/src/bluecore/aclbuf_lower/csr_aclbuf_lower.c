/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_aclbuf.h"
#include "csr_aclbuf_lower.h"

static CsrAclBufAllocCb csrAclBufAlloc = NULL;
static CsrAclBufDeliverCb csrAclBufDeliver = NULL;

CsrUint8 CsrAclBufAlloc(CsrUint16 chanAndFlags,
    CsrUint16 size,
    void *hdr,
    CsrUint16 hdrSize,
    void **ctx,
    CsrUint8 **data)
{
    CsrUint8 res = CSR_ACLBUF_NONE;
    if (csrAclBufAlloc)
    {
        res = csrAclBufAlloc(chanAndFlags, size, hdr, hdrSize, ctx, data);
    }
    return res;
}

CsrAclBufCb CsrAclBufDeliver(CsrUint16 chanAndFlags,
    void *ctx,
    CsrUint16 size)
{
    return csrAclBufDeliver(chanAndFlags, ctx, size);
}

void CsrAclBufLowerInit(CsrAclBufAllocCb alloc,
    CsrAclBufDeliverCb deliver)
{
    csrAclBufAlloc = alloc;
    csrAclBufDeliver = deliver;
}
