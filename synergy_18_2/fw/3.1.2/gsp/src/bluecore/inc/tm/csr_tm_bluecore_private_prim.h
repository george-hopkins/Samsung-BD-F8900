#ifndef CSR_TM_BLUECORE_PRIVATE_PRIM_H__
#define CSR_TM_BLUECORE_PRIVATE_PRIM_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/
#include "csr_tm_bluecore_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

/* search_string="CsrTmBlueCorePrim" */
/* conversion_rule="UPPERCASE_START_AND_REMOVE_UNDERSCORES" */

#define CSR_TM_BLUECORE_PRIVATE_PRIM (CSR_TM_BLUECORE_PRIM)

#define CSR_TM_BLUECORE_RESET_IND ((CsrTmBlueCorePrim) ((0x0000 + CSR_TM_PRIM_UPSTREAM_COUNT) | CSR_PRIM_UPSTREAM))
#define CSR_TM_BLUECORE_PANIC_IND ((CsrTmBlueCorePrim) ((0x0001 + CSR_TM_PRIM_UPSTREAM_COUNT) | CSR_PRIM_UPSTREAM))

typedef struct
{
    CsrTmBlueCorePrim type;
} CsrTmBluecoreResetInd;

typedef struct
{
    CsrTmBlueCorePrim type;
} CsrTmBluecorePanicInd;

#ifdef __cplusplus
}
#endif

#endif /* CSR_TM_BLUECORE_PRIVATE_PRIM_H__ */
