#ifndef CSR_ENV_SERIALIZE_H__
#define CSR_ENV_SERIALIZE_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrEnvPfree(void *ptr);

#ifndef EXCLUDE_CSR_CLEANUP_IND
CsrSize CsrCleanupIndSizeof(void *msg);
CsrUint8 *CsrCleanupIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrCleanupIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrCleanupIndSerFree CsrEnvPfree
#endif /* EXCLUDE_CSR_CLEANUP_IND */

#ifdef __cplusplus
}
#endif

#endif
