#ifndef CSR_UI_KEYHANDLER_H__
#define CSR_UI_KEYHANDLER_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_ui_ctrl.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrUiKeyMultiKeyReset(CsrUiInstanceData *instanceData);
void CsrUiKeyEvent(CsrUint16 key);
CsrBool CsrUiKeyHandler(CsrUiInstanceData *instanceData);

#ifdef __cplusplus
}
#endif

#endif /* CSR_UI_KEYHANDLER_H__ */
