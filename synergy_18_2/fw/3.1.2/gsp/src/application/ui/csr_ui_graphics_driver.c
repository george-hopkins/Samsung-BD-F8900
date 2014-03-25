/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_ui_lower.h"
#include "csr_ui_graphics_driver.h"
#include "csr_ui_ctrl.h"

extern const CsrUint16 CsrUiFontSaxMono10x21DataTable[];
extern const CsrUint16 CsrUiFontSaxMono12x22DataTable[];

void CsrUiGraphicsDriverLine(CsrUiInstanceData *instanceData, CsrInt16 x1, CsrInt16 y1, CsrInt16 x2, CsrInt16 y2, CsrUint32 color)
{
    CsrInt32 d, dx, dy;
    CsrInt32 Aincr, Bincr, xincr, yincr;
    CsrInt32 x, y;

    dx = x2 - x1;

    if (dx < 0)
    {
        dx = -dx;
    }

    dy = y2 - y1;

    if (dy < 0)
    {
        dy = -dy;
    }

    if (dx > dy)
    {
        if (x1 > x2)
        {
            CsrInt32 t;

            t = x1;
            x1 = x2;
            x2 = (CsrInt16) t;
            t = y1;
            y1 = y2;
            y2 = (CsrInt16) t;
        }

        if (y2 > y1)
        {
            yincr = 1;
        }
        else
        {
            yincr = -1;
        }

        dx = x2 - x1;

        dy = y2 - y1;  /* dy = abs(y2 - y1); */
        if (dy < 0)
        {
            dy = -dy;
        }

        d = 2 * dy - dx;

        Aincr = 2 * (dy - dx);
        Bincr = 2 * dy;

        x = x1;
        y = y1;

        instanceData->graphicsRenderer.setPixel((CsrInt16) x, (CsrInt16) y, color);

        for (x = x1 + 1; x <= x2; ++x)
        {
            if (d >= 0)
            {
                y += yincr;
                d += Aincr;
            }
            else
            {
                d += Bincr;
            }

            instanceData->graphicsRenderer.setPixel((CsrInt16) x, (CsrInt16) y, color);
        }
    }
    else
    {
        if (y1 > y2)
        {
            CsrInt32 t;

            t = x1;
            x1 = x2;
            x2 = (CsrInt16) t;
            t = y1;
            y1 = y2;
            y2 = (CsrInt16) t;
        }

        if (x2 > x1)
        {
            xincr = 1;
        }
        else
        {
            xincr = -1;
        }

        dy = y2 - y1;

        dx = x2 - x1;  /* dx = abs(x2 - x1); */
        if (dx < 0)
        {
            dx = -dx;
        }

        d = 2 * dx - dy;

        Aincr = 2 * (dx - dy);
        Bincr = 2 * dx;

        x = x1;
        y = y1;

        instanceData->graphicsRenderer.setPixel((CsrInt16) x, (CsrInt16) y, color);

        for (y = y1 + 1; y <= y2; ++y)
        {
            if (d >= 0)
            {
                x += xincr;
                d += Aincr;
            }
            else
            {
                d += Bincr;
            }

            instanceData->graphicsRenderer.setPixel((CsrInt16) x, (CsrInt16) y, color);
        }
    }
}

CsrInt16 CsrUiGraphicsDriverGetFontWidth(CsrInt16 fontNumber)
{
    return (fontNumber >> 8) & 0xFF;
}

CsrInt16 CsrUiGraphicsDriverGetFontHeight(CsrInt16 fontNumber)
{
    return fontNumber & 0xFF;
}

void CsrUiGraphicsDriverPutFontCharacter(CsrUiInstanceData *instanceData, CsrInt16 fontNumber, CsrInt16 x, CsrInt16 y, CsrUint16 character, CsrUint32 foregroundColor)
{
    const CsrUint16 *characterPointer16;
    CsrUint32 row, column, bitmask;

    if (character > 127)
    {
        /* Character sets only support 127 first characters */
        character = 9;
    }

    switch (fontNumber)
    {
        case FONT_SAXMONO_10x21:
        {
            characterPointer16 = &CsrUiFontSaxMono10x21DataTable[character * 21];
            for (row = 0; row < 21; row++)
            {
                bitmask = *characterPointer16++;
                for (column = 0; column < 10; column++, bitmask = (bitmask << 1))
                {
                    if (bitmask & 0x8000)
                    {
                        instanceData->graphicsRenderer.setPixel((CsrInt16) (x + column), (CsrInt16) (y + row), foregroundColor);
                    }
                }
            }
            break;
        }
        case FONT_SAXMONO_12x22:
        {
            characterPointer16 = &CsrUiFontSaxMono12x22DataTable[character * 22];
            for (row = 0; row < 22; row++)
            {
                bitmask = *characterPointer16++;
                for (column = 0; column < 12; column++, bitmask = (bitmask << 1))
                {
                    if (bitmask & 0x8000)
                    {
                        instanceData->graphicsRenderer.setPixel((CsrInt16) (x + column), (CsrInt16) (y + row), foregroundColor);
                    }
                }
            }
            break;
        }
        default:
            return;
    }
}

void CsrUiGraphicsDriverPrintFontString(CsrUiInstanceData *instanceData, CsrInt16 fontNumber, CsrInt16 x, CsrInt16 y, CsrUint16 *string, CsrUint32 foregroundColor)
{
    CsrInt16 characterWidth;

    characterWidth = CsrUiGraphicsDriverGetFontWidth(fontNumber);

    if (characterWidth)
    {
        while (*string)
        {
            CsrUiGraphicsDriverPutFontCharacter(instanceData, fontNumber, x, y, *string++, foregroundColor);
            x += characterWidth;
        }
    }
}
