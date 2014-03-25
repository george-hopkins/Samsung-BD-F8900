#ifndef CSR_UI_PRIVATE_PRIM_H__
#define CSR_UI_PRIVATE_PRIM_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_ui_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Private Downstream primitives    */
#define CSR_UI_INTERNAL_KEYDOWN_IND                   ((CsrUiPrim) 0x0000 + CSR_UI_PRIM_DOWNSTREAM_COUNT)

#define NUM_OF_CSR_UI_PRIVATE_DOWNSTREAM_PRIMS (0x01)
#define TOTAL_NUM_OF_CSR_UI_DOWNSTREAM_PRIMS (CSR_UI_PRIM_DOWNSTREAM_COUNT + NUM_OF_CSR_UI_PRIVATE_DOWNSTREAM_PRIMS)

typedef struct
{
    CsrUiPrim type;
    CsrUint16 key;
} CsrUiInternalKeydownInd;

#ifdef __cplusplus
}
#endif

#endif /* CSR_UI_PRIVATE_PRIM_H__ */
