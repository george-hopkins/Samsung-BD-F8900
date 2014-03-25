#ifndef CSR_UI_RENDERER_TEXT_H__
#define CSR_UI_RENDERER_TEXT_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif


void CsrUiRenderTextRedraw(CsrUiInstanceData *instanceData);

void CsrUiRenderTextCalculateScreenCoordinates(CsrUiInstanceData *instanceData);

void CsrUiRenderTextMenuScrollUp(Menu *menu);
void CsrUiRenderTextMenuScrollDown(Menu *menu);
void CsrUiRenderTextMenuScrollUpdate(Menu *menu);
void CsrUiRenderTextMenuScrollDelete(Menu *menu, CsrUint16 index);

CsrBool CsrUiRenderTextDialogScrollUp(Dialog *dialog);
CsrBool CsrUiRenderTextDialogScrollDown(Dialog *dialog);

#ifdef __cplusplus
}
#endif

#endif
