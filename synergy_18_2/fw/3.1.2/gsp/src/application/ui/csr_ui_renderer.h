#ifndef CSR_UI_RENDERER_H__
#define CSR_UI_RENDERER_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_ui_uiemanager.h"
#include "csr_ui_ctrl.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrUiRenderRedraw(CsrUiInstanceData *instanceData);

void CsrUiRenderCalculateScreenCoordinates(CsrUiInstanceData *instanceData);

void CsrUiRenderMenuScrollUp(Menu *menu);
void CsrUiRenderMenuScrollDown(Menu *menu);
void CsrUiRenderMenuScrollUpdate(Menu *menu);
void CsrUiRenderMenuScrollDelete(Menu *menu, CsrUint16 index);

CsrBool CsrUiRenderDialogScrollUp(Dialog *dialog);
CsrBool CsrUiRenderDialogScrollDown(Dialog *dialog);

#ifdef __cplusplus
}
#endif

#endif
