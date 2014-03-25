#ifndef CSR_SDIO_PROBE_INTERNAL_PRIM_H__
#define CSR_SDIO_PROBE_INTERNAL_PRIM_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_sdio.h"
#include "csr_prim_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Define this locally as there are no public primitives */
#define CSR_SDIO_PROBE_PRIM_UPSTREAM_LOWEST (0x0000 + CSR_PRIM_UPSTREAM)
#define CSR_SDIO_PROBE_PRIM_UPSTREAM_COUNT 0

#define CSR_SDIO_PROBE_INTERNAL_INSERTED_IND ((CsrPrim) ((0x0000 + CSR_SDIO_PROBE_PRIM_UPSTREAM_COUNT) | CSR_PRIM_UPSTREAM))
#define CSR_SDIO_PROBE_INTERNAL_REMOVED_IND  ((CsrPrim) ((0x0001 + CSR_SDIO_PROBE_PRIM_UPSTREAM_COUNT) | CSR_PRIM_UPSTREAM))

#define CSR_SDIO_PROBE_INTERNAL_UPSTREAM_COUNT 2

typedef struct
{
    CsrPrim          type;
    CsrSdioFunction *function;
} CsrSdioProbeInternalInsertedInd;

typedef struct
{
    CsrPrim          type;
    CsrSdioFunction *function;
} CsrSdioProbeInternalRemovedInd;

#ifdef __cplusplus
}
#endif

#endif
