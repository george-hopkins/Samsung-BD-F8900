/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_ui_prim.h"
#include "csr_ui_exception.h"
#include "csr_ui_icon.h"
#include "csr_ui_lower.h"

/* Include generated artwork data */
#include "csr_ui_icon_db.h"

void CsrUiIconPut(CsrUiInstanceData *instanceData, CsrUint16 icon, CsrInt16 x, CsrInt16 y)
{
    CsrUint16 index;
    CsrInt16 xc, yc;

    if ((icon == 0) || (icon == 1))
    {
        return;
    }

    icon -= 2; /* Offset into icon_db */
    if (icon >= CSR_UI_ICON_COUNT)
    {
        CSR_UIEXCEPTION("Invalid icon index");
    }

    /* Render the icon in-place */
    index = 0;
    for (yc = CSR_UI_ICON_WIDTH; yc > 0; yc--)
    {
        for (xc = 0; xc < CSR_UI_ICON_HEIGHT; xc++)
        {
            if (IconImageData[icon][index] != 0xFF00FF)
            {
                instanceData->graphicsRenderer.setPixel((CsrInt16) (x + xc), (CsrInt16) (y + yc - 1), IconImageData[icon][index]);
            }
            index++;
        }
    }
}
