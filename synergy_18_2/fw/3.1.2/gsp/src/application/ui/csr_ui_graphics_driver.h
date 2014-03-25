#ifndef CSR_UI_GRAPHICS_DRIVER_H__
#define CSR_UI_GRAPHICS_DRIVER_H__
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

#define FONT_SAXMONO_10x21              ((10 << 8) | (21 << 0))
#define FONT_SAXMONO_12x22              ((12 << 8) | (22 << 0))

void CsrUiGraphicsDriverLine(CsrUiInstanceData *instanceData, CsrInt16 x1, CsrInt16 y1, CsrInt16 x2, CsrInt16 y2, CsrUint32 color);
CsrInt16 CsrUiGraphicsDriverGetFontWidth(CsrInt16 fontNumber);
CsrInt16 CsrUiGraphicsDriverGetFontHeight(CsrInt16 fontNumber);
void CsrUiGraphicsDriverPutFontCharacter(CsrUiInstanceData *instanceData, CsrInt16 fontNumber, CsrInt16 x, CsrInt16 y, CsrUint16 character, CsrUint32 foregroundColor);
void CsrUiGraphicsDriverPrintFontString(CsrUiInstanceData *instanceData, CsrInt16 fontNumber, CsrInt16 x, CsrInt16 y, CsrUint16 *string, CsrUint32 foregroundColor);

#ifdef __cplusplus
}
#endif

#endif /* CSR_UI_GRAPHICS_DRIVER_H__ */
