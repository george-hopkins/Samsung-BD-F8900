#ifndef CSR_ENV_PRIM_H__
#define CSR_ENV_PRIM_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_prim_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/* search_string="CsrEnvPrim" */
/* conversion_rule="UPPERCASE_START_AND_REMOVE_UNDERSCORES" */

typedef CsrPrim CsrEnvPrim;

#define CSR_CLEANUP_IND        ((CsrEnvPrim) 0x0000)

typedef struct
{
    CsrEnvPrim type;
    CsrUint16  phandle;
} CsrCleanupInd;

#ifdef __cplusplus
}
#endif

#endif
