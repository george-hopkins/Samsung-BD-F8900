/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_msgconv.h"
#include "csr_autogen.h"
#include "csr_unicode.h"
#include "csr_env_prim.h"

#ifndef EXCLUDE_CSR_ENV_MODULE

void CsrEnvPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_CLEANUP_IND
CsrSize CsrCleanupIndSizeof(void *msg)
{
    CsrCleanupInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_CLEANUP_IND */

#ifndef EXCLUDE_CSR_CLEANUP_IND
CsrUint8 *CsrCleanupIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrCleanupInd *primitive;

    primitive = (CsrCleanupInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_CLEANUP_IND */

#ifndef EXCLUDE_CSR_CLEANUP_IND
void *CsrCleanupIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrCleanupInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrCleanupInd *) CsrPmemZalloc(sizeof(CsrCleanupInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_CLEANUP_IND */

#endif /* EXCLUDE_CSR_ENV_MODULE */
