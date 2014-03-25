/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_ui_lower.h"

#include "csr_ui_renderer.h"
#include "csr_ui_ctrl.h"
#include "csr_ui_graphics_driver.h"
#include "csr_ui_util.h"
#include "csr_ui_uiemanager.h"
#include "csr_ui_keycode.h"
#include "csr_ui_displaystack.h"
#include "csr_ui_icon.h"

/* Include generated artwork data */
#include "csr_ui_logo.h"
#include "csr_ui_status_icon.h"

/* Colours */
#define RGB(r, g, b)                    (((b) << 16) | ((g) << 8) | (r))
#define WHITE                           RGB(255, 255, 255)
#define BLACK                           RGB(0, 0, 0)
#define GRAY(x)                         RGB(x, x, x)
#define RED(x)                          RGB(x, 0, 0)
#define LIGHT_RED(x)                    RGB(255, x, x)
#define GREEN(x)                        RGB(0, x, 0)
#define LIGHT_GREEN(x)                  RGB(x, 255, x)
#define BLUE(x)                         RGB(0, 0, x)
#define LIGHT_BLUE(x)                   RGB(x, x, 255)
#define YELLOW(x)                       RGB(x, x, 0)
#define LIGHT_YELLOW()                  RGB(255, 255, x)

/* Screen Dimensions */
#define SCREEN_WIDTH                    256
#define SCREEN_HEIGHT                   320

/* Status Area */
#define STATUS_WIDTH                    SCREEN_WIDTH
#define STATUS_HEIGHT                   32
#define RADIOMETER_WIDTH                28
#define BATTERY_WIDTH                   28
#define STATUS_COLOR0                   LIGHT_BLUE(80)
#define STATUS_COLOR1                   LIGHT_BLUE(70)
#define STATUS_COLOR2                   LIGHT_BLUE(60)
#define STATUS_COLOR3                   LIGHT_BLUE(50)
#define STATUS_COLOR4                   LIGHT_BLUE(40)
#define STATUS_RADIOMETER_TOP_MARGIN    4
#define STATUS_BATTERY_TOP_MARGIN       5
#define STATUS_EDGE_RIGHT_MARGIN        3
#define STATUS_EDGE_LEFT_MARGIN         3
#define STATUS_ICON_SKIP                7
#define STATUS_ICON_TOP_MARGIN          3

/* Softkey Area */
#define SK_WIDTH                        SCREEN_WIDTH
#define SK_HEIGHT                       28
#define SK_COLOR0                       LIGHT_BLUE(80)
#define SK_COLOR1                       LIGHT_BLUE(100)
#define SK_COLOR2                       LIGHT_BLUE(120)
#define SK_COLOR3                       LIGHT_BLUE(140)
#define SK_FONT                         FONT_SAXMONO_12x22
#define SK_FONT_WIDTH                   12
#define SK_FONT_HEIGHT                  22
#define SK_FONT_COLOR                   WHITE
#define SK_SCROLL_WIDTH                 80
#define SK_CHARACTERS                   ((SK_WIDTH - SK_SCROLL_WIDTH) / (2 * SK_FONT_WIDTH))

/* Canvas Area */
#define CANVAS_HEIGHT                   (SCREEN_HEIGHT - STATUS_HEIGHT - SK_HEIGHT)
#define CANVAS_WIDTH                    SCREEN_WIDTH
#define CANVAS_BACKGROUND_COLOR         LIGHT_BLUE(245)

/* Heading */
#define HEADING_TOPSKIP                 4
#define HEADING_FONT                    FONT_SAXMONO_12x22
#define HEADING_FONT_WIDTH              12
#define HEADING_FONT_HEIGHT             22
#define HEADING_FONT_COLOR              BLACK
#define HEADING_RULER_COLOR             LIGHT_BLUE(150)
#define HEADING_RULER_HEIGHT            2
#define HEADING_CHARACTERS              (CANVAS_WIDTH / HEADING_FONT_WIDTH)
#define HEADING_BOTTOMSKIP              4

/* Menu Item */
#define MENUITEM_INDENT                 8
#define MENUITEM_FONT                   FONT_SAXMONO_12x22
#define MENUITEM_FONT_WIDTH             12
#define MENUITEM_FONT_HEIGHT            22
#define MENUITEM_FONT_COLOR             GRAY(64)
#define MENUITEM_FONT_SELCOLOR          WHITE
#define MENUITEM_FONT_SUBCOLOR          GRAY(140)
#define MENUITEM_FONT_SELSUBCOLOR       GRAY(220)
#define MENUITEM_CURSOR_COLOR           LIGHT_BLUE(150)
#define MENUITEM_ICON_WIDTH             16
#define MENUITEM_ICON_HEIGHT            16
#define MENUITEM_SPACING                0
#define MENUITEM_HEIGHT                 (CANVAS_HEIGHT - HEADING_TOPSKIP - HEADING_FONT_HEIGHT - HEADING_RULER_HEIGHT - HEADING_BOTTOMSKIP)
#define MENUITEM_LINES                  ((CANVAS_HEIGHT - HEADING_TOPSKIP - HEADING_FONT_HEIGHT - HEADING_RULER_HEIGHT - HEADING_BOTTOMSKIP) / (MENUITEM_FONT_HEIGHT + MENUITEM_SPACING))
#define MENUITEM_CHARACTERS             ((CANVAS_WIDTH - MENUITEM_INDENT) / MENUITEM_FONT_WIDTH)

/* Dialog */
#define DIALOG_BOX_INDENT               4
#define DIALOG_TEXT_INDENT              3
#define DIALOG_BACKGROUND_COLOR         CANVAS_BACKGROUND_COLOR
#define DIALOG_EDGE_COLOR               LIGHT_BLUE(150)
#define DIALOG_FONT                     FONT_SAXMONO_10x21
#define DIALOG_FONT_WIDTH               10
#define DIALOG_FONT_HEIGHT              21
#define DIALOG_FONT_COLOR               GRAY(64)
#define DIALOG_LINES                    ((CANVAS_HEIGHT - DIALOG_BOX_INDENT - DIALOG_TEXT_INDENT - HEADING_TOPSKIP - HEADING_FONT_HEIGHT - DIALOG_TEXT_INDENT) / DIALOG_FONT_HEIGHT)
#define DIALOG_CHARACTERS               ((CANVAS_WIDTH - DIALOG_BOX_INDENT - DIALOG_TEXT_INDENT - DIALOG_BOX_INDENT - DIALOG_TEXT_INDENT) / DIALOG_FONT_WIDTH)

/* Input Dialog */
#define INPUTDIALOG_FONT                FONT_SAXMONO_10x21
#define INPUTDIALOG_FONT_WIDTH          10
#define INPUTDIALOG_FONT_HEIGHT         21
#define INPUTDIALOG_FONT_COLOR          GRAY(64)
#define INPUTDIALOG_MESSAGE_INDENT      4
#define INPUTDIALOG_MESSAGE_LINES       10
#define INPUTDIALOG_ICON_WIDTH          16
#define INPUTDIALOG_ICON_HEIGHT         16
#define INPUTDIALOG_MESSAGE_CHARACTERS  ((CANVAS_WIDTH - INPUTDIALOG_MESSAGE_INDENT * 3 - INPUTDIALOG_ICON_WIDTH) / INPUTDIALOG_FONT_WIDTH)
#define INPUTDIALOG_BOX_INDENT          4
#define INPUTDIALOG_TEXT_INDENT         3
#define INPUTDIALOG_BACKGROUND_COLOR    CANVAS_BACKGROUND_COLOR
#define INPUTDIALOG_EDGE_COLOR          LIGHT_BLUE(150)

/* Idle Screen */
#define IDLESCREEN_TOPSKIP              16
#define IDLESCREEN_HEADING_FONT         FONT_SAXMONO_12x22
#define IDLESCREEN_HEADING_FONT_WIDTH   12
#define IDLESCREEN_HEADING_FONT_HEIGHT  22
#define IDLESCREEN_HEADING_FONT_COLOR   GRAY(64)
#define IDLESCREEN_HEADING_CHARACTERS   (CANVAS_WIDTH / IDLESCREEN_HEADING_FONT_WIDTH)
#define IDLESCREEN_LOGO_TOPSKIP         8
#define IDLESCREEN_LOGO_WIDTH           130
#define IDLESCREEN_LOGO_HEIGHT          130
#define IDLESCREEN_TEXT_TOPSKIP         12
#define IDLESCREEN_TEXT_FONT            FONT_SAXMONO_12x22
#define IDLESCREEN_TEXT_FONT_WIDTH      12
#define IDLESCREEN_TEXT_FONT_HEIGHT     22
#define IDLESCREEN_TEXT_FONT_COLOR      GRAY(64)
#define IDLESCREEN_TEXT_CHARACTERS      (CANVAS_WIDTH / IDLESCREEN_TEXT_FONT_WIDTH)
#define IDLESCREEN_TEXT_LINES           1

static void renderSKTextSet(CsrUiInstanceData *instanceData, CsrUint16 key, CsrUint16 *string)
{
    CsrUint16 i;
    CsrUint16 str[SK_CHARACTERS + 1];
    ScreenCoordinates *coor;

    coor = &(instanceData->coor);

    for (i = 0; (string != NULL) && (string[i] != 0) && (i < SK_CHARACTERS); i++)
    {
        str[i] = string[i];
    }
    str[i] = 0;

    if (key == CSR_UI_KEY_SK1)
    {
        CsrUiGraphicsDriverPrintFontString(instanceData, SK_FONT, (CsrInt16) (coor->skLeft + (((SCREEN_WIDTH - SK_SCROLL_WIDTH) / 2) - SK_FONT_WIDTH * CsrUiUtilStringLength(str)) / 2 - 1),
            (CsrInt16) (coor->skTop + (SK_HEIGHT - SK_FONT_HEIGHT) / 2 + 1), str, SK_FONT_COLOR);
    }
    else if (key == CSR_UI_KEY_SK2)
    {
        CsrUiGraphicsDriverPrintFontString(instanceData, SK_FONT, (CsrInt16) (coor->skRight - ((SCREEN_WIDTH - SK_SCROLL_WIDTH) / 4) - SK_FONT_WIDTH * CsrUiUtilStringLength(str) / 2 + 1),
            (CsrInt16) (coor->skTop + (SK_HEIGHT - SK_FONT_HEIGHT) / 2 + 1), str, SK_FONT_COLOR);
    }
}

static void renderStatusBluetooth(CsrUiInstanceData *instanceData, CsrInt16 x, CsrInt16 y)
{
    CsrInt32 xc, yc, index;

    for (index = 0, yc = CSR_UI_STATUS_ICON_BLUETOOTH_HEIGHT; yc != 0; yc--)
    {
        for (xc = 0; xc < CSR_UI_STATUS_ICON_BLUETOOTH_WIDTH; xc++, index += 3)
        {
            instanceData->graphicsRenderer.setPixel((CsrInt16) (x + xc), (CsrInt16) (y + yc - 1), RGB(statusIconBluetooth[index + 2], statusIconBluetooth[index + 1], statusIconBluetooth[index]));
        }
    }
}

static void renderStatusWifi(CsrUiInstanceData *instanceData, CsrInt16 x, CsrInt16 y)
{
    CsrInt32 xc, yc, index;

    for (index = 0, yc = CSR_UI_STATUS_ICON_WIFI_HEIGHT; yc != 0; yc--)
    {
        for (xc = 0; xc < CSR_UI_STATUS_ICON_WIFI_WIDTH; xc++, index += 3)
        {
            instanceData->graphicsRenderer.setPixel((CsrInt16) (x + xc), (CsrInt16) (y + yc - 1), RGB(statusIconWifi[index + 2], statusIconWifi[index + 1], statusIconWifi[index]));
        }
    }
}

static void renderStatusRadiometerSet(CsrUiInstanceData *instanceData, CsrInt16 x, CsrInt16 y, CsrUint8 percentage)
{
    CsrUint8 i;
    CsrUint32 color;

    for (i = 0; i < 4; i++)
    {
        if (percentage > i * 25)
        {
            color = GREEN(168);
        }
        else
        {
            color = GRAY(192);
        }
        instanceData->graphicsRenderer.drawBox((CsrInt16) (x + 1 + i * 7), (CsrInt16) (y + 13 - i * 4), (CsrInt16) (x + 6 + i * 7), (CsrInt16) (y + 20), color, color, TRUE);

        /* White glow */
        instanceData->graphicsRenderer.drawBox((CsrInt16) (x + 0 + i * 7), (CsrInt16) (y + 12 - i * 4), (CsrInt16) (x + 7 + i * 7), (CsrInt16) (y + 21), WHITE, 0, FALSE);
    }
}

static CsrInt16 renderStatusHeadset(CsrUiInstanceData *instanceData, CsrInt16 x, CsrInt16 y, CsrInt8 headset)
{
    CsrInt16 skip = 0;

    /* Render the indicator */
    if (headset >= CSR_UI_STATUS_HEADSET_PRESENT_INDICATOR(0))
    {
        CsrInt32 colour;
        CsrInt32 i;
        CsrInt32 boxCount = (headset - 2 + 10) / 20;

        if (boxCount > 2)
        {
            colour = GREEN(255);
        }
        else if (boxCount > 1)
        {
            colour = YELLOW(255);
        }
        else
        {
            colour = RED(255);
        }

        for (i = 0; i < 5; i++)
        {
            if ((5 - boxCount) <= i)
            {
                instanceData->graphicsRenderer.drawBox(x, (CsrInt16) (y + i * 5), (CsrInt16) (x + 2), (CsrInt16) (y + i * 5 + 3), colour, colour, TRUE);
            }
            else
            {
                instanceData->graphicsRenderer.drawBox(x, (CsrInt16) (y + i * 5), (CsrInt16) (x + 2), (CsrInt16) (y + i * 5 + 3), GRAY(192), GRAY(192), TRUE);
            }
        }

        skip += 5;
    }

    /* Render the headset icon */
    if (headset >= CSR_UI_STATUS_HEADSET_PRESENT)
    {
        CsrInt32 xc, yc, index;

        for (index = 0, yc = CSR_UI_STATUS_ICON_HEADSET_HEIGHT; yc != 0; yc--)
        {
            for (xc = 0; xc < CSR_UI_STATUS_ICON_HEADSET_WIDTH; xc++, index += 3)
            {
                instanceData->graphicsRenderer.setPixel((CsrInt16) (x + skip + xc), (CsrInt16) (y + yc - 1), RGB(statusIconHeadset[index + 2], statusIconHeadset[index + 1], statusIconHeadset[index]));
            }
        }

        skip += CSR_UI_STATUS_ICON_HEADSET_WIDTH + STATUS_ICON_SKIP;
    }

    return skip;
}

static void renderStatusBatterySet(CsrUiInstanceData *instanceData, CsrInt16 x, CsrInt16 y, CsrUint8 percentage)
{
    CsrUint32 chargeColour[6];
    CsrInt16 left;

    if (percentage > 100)
    {
        percentage = 100;
    }

    /* White glow */
    instanceData->graphicsRenderer.drawBox((CsrInt16) (x + 2), (CsrInt16) (y), (CsrInt16) (x + 27), (CsrInt16) (y + 19), WHITE, 0, FALSE);
    instanceData->graphicsRenderer.drawBox((CsrInt16) (x), (CsrInt16) (y + 5), (CsrInt16) (x + 1), (CsrInt16) (y + 5 + 9), WHITE, 0, FALSE);

    /* Battery */
    instanceData->graphicsRenderer.drawBox((CsrInt16) (x + 3), (CsrInt16) (y + 1), (CsrInt16) (x + 26), (CsrInt16) (y + 18), GRAY(64), WHITE, TRUE);
    instanceData->graphicsRenderer.drawBox((CsrInt16) (x + 1), (CsrInt16) (y + 6), (CsrInt16) (x + 2), (CsrInt16) (y + 6 + 7), GRAY(64), 0, FALSE);

    /* Charge */
    if (percentage == 0)
    {
        return;
    }
    else if (percentage > 50)
    {
        chargeColour[0] = GREEN(127);
        chargeColour[1] = GREEN(153);
        chargeColour[2] = GREEN(178);
        chargeColour[3] = GREEN(204);
        chargeColour[4] = GREEN(229);
        chargeColour[5] = GREEN(255);
    }
    else if (percentage > 20)
    {
        chargeColour[0] = YELLOW(128);
        chargeColour[1] = YELLOW(168);
        chargeColour[2] = YELLOW(208);
        chargeColour[3] = YELLOW(204);
        chargeColour[4] = YELLOW(229);
        chargeColour[5] = YELLOW(255);
    }
    else
    {
        chargeColour[0] = RED(128);
        chargeColour[1] = RED(168);
        chargeColour[2] = RED(208);
        chargeColour[3] = RED(204);
        chargeColour[4] = RED(229);
        chargeColour[5] = RED(255);
    }

    left = x + 6 + (10 - percentage / 10);

    instanceData->graphicsRenderer.drawBox(left, (CsrInt16) (y + 2), (CsrInt16) (x + 25), (CsrInt16) (y + 3), chargeColour[1], 0, FALSE);
    instanceData->graphicsRenderer.drawBox(left, (CsrInt16) (y + 4), (CsrInt16) (x + 25), (CsrInt16) (y + 5), chargeColour[2], 0, FALSE);
    instanceData->graphicsRenderer.drawBox(left, (CsrInt16) (y + 6), (CsrInt16) (x + 25), (CsrInt16) (y + 6), chargeColour[3], 0, FALSE);
    instanceData->graphicsRenderer.drawBox(left, (CsrInt16) (y + 7), (CsrInt16) (x + 25), (CsrInt16) (y + 7), chargeColour[4], 0, FALSE);
    instanceData->graphicsRenderer.drawBox(left, (CsrInt16) (y + 8), (CsrInt16) (x + 25), (CsrInt16) (y + 9), chargeColour[5], 0, FALSE);
    instanceData->graphicsRenderer.drawBox(left, (CsrInt16) (y + 10), (CsrInt16) (x + 25), (CsrInt16) (y + 10), chargeColour[4], 0, FALSE);
    instanceData->graphicsRenderer.drawBox(left, (CsrInt16) (y + 11), (CsrInt16) (x + 25), (CsrInt16) (y + 11), chargeColour[3], 0, FALSE);
    instanceData->graphicsRenderer.drawBox(left, (CsrInt16) (y + 12), (CsrInt16) (x + 25), (CsrInt16) (y + 13), chargeColour[2], 0, FALSE);
    instanceData->graphicsRenderer.drawBox(left, (CsrInt16) (y + 14), (CsrInt16) (x + 25), (CsrInt16) (y + 15), chargeColour[1], 0, FALSE);
    instanceData->graphicsRenderer.drawBox(left, (CsrInt16) (y + 16), (CsrInt16) (x + 25), (CsrInt16) (y + 17), chargeColour[0], 0, FALSE);
}

static void renderScrollableSet(CsrUiInstanceData *instanceData)
{
    ScreenCoordinates *coor;

    coor = &(instanceData->coor);

    /* Arrow up */

    instanceData->graphicsRenderer.drawBox((CsrInt16) (coor->skLeft + CANVAS_WIDTH / 2 - 1), (CsrInt16) (coor->skTop + 6),
        (CsrInt16) (coor->skLeft + CANVAS_WIDTH / 2 + 0), (CsrInt16) (coor->skTop + 6), WHITE, 0, FALSE);
    instanceData->graphicsRenderer.drawBox((CsrInt16) (coor->skLeft + CANVAS_WIDTH / 2 - 2), (CsrInt16) (coor->skTop + 7),
        (CsrInt16) (coor->skLeft + CANVAS_WIDTH / 2 + 1), (CsrInt16) (coor->skTop + 7), WHITE, 0, FALSE);
    instanceData->graphicsRenderer.drawBox((CsrInt16) (coor->skLeft + CANVAS_WIDTH / 2 - 3), (CsrInt16) (coor->skTop + 8),
        (CsrInt16) (coor->skLeft + CANVAS_WIDTH / 2 + 2), (CsrInt16) (coor->skTop + 8), WHITE, 0, FALSE);
    instanceData->graphicsRenderer.drawBox((CsrInt16) (coor->skLeft + CANVAS_WIDTH / 2 - 4), (CsrInt16) (coor->skTop + 9),
        (CsrInt16) (coor->skLeft + CANVAS_WIDTH / 2 + 3), (CsrInt16) (coor->skTop + 9), WHITE, 0, FALSE);
    instanceData->graphicsRenderer.drawBox((CsrInt16) (coor->skLeft + CANVAS_WIDTH / 2 - 5), (CsrInt16) (coor->skTop + 10),
        (CsrInt16) (coor->skLeft + CANVAS_WIDTH / 2 + 4), (CsrInt16) (coor->skTop + 10), WHITE, 0, FALSE);
    instanceData->graphicsRenderer.drawBox((CsrInt16) (coor->skLeft + CANVAS_WIDTH / 2 - 6), (CsrInt16) (coor->skTop + 11),
        (CsrInt16) (coor->skLeft + CANVAS_WIDTH / 2 + 5), (CsrInt16) (coor->skTop + 11), WHITE, 0, FALSE);
    instanceData->graphicsRenderer.drawBox((CsrInt16) (coor->skLeft + CANVAS_WIDTH / 2 - 7), (CsrInt16) (coor->skTop + 12),
        (CsrInt16) (coor->skLeft + CANVAS_WIDTH / 2 + 6), (CsrInt16) (coor->skTop + 12), WHITE, 0, FALSE);

    /* Arrow down */

    instanceData->graphicsRenderer.drawBox((CsrInt16) (coor->skLeft + CANVAS_WIDTH / 2 - 1), (CsrInt16) (coor->skTop + 23),
        (CsrInt16) (coor->skLeft + CANVAS_WIDTH / 2 + 0), (CsrInt16) (coor->skTop + 23), WHITE, 0, FALSE);
    instanceData->graphicsRenderer.drawBox((CsrInt16) (coor->skLeft + CANVAS_WIDTH / 2 - 2), (CsrInt16) (coor->skTop + 22),
        (CsrInt16) (coor->skLeft + CANVAS_WIDTH / 2 + 1), (CsrInt16) (coor->skTop + 22), WHITE, 0, FALSE);
    instanceData->graphicsRenderer.drawBox((CsrInt16) (coor->skLeft + CANVAS_WIDTH / 2 - 3), (CsrInt16) (coor->skTop + 21),
        (CsrInt16) (coor->skLeft + CANVAS_WIDTH / 2 + 2), (CsrInt16) (coor->skTop + 21), WHITE, 0, FALSE);
    instanceData->graphicsRenderer.drawBox((CsrInt16) (coor->skLeft + CANVAS_WIDTH / 2 - 4), (CsrInt16) (coor->skTop + 20),
        (CsrInt16) (coor->skLeft + CANVAS_WIDTH / 2 + 3), (CsrInt16) (coor->skTop + 20), WHITE, 0, FALSE);
    instanceData->graphicsRenderer.drawBox((CsrInt16) (coor->skLeft + CANVAS_WIDTH / 2 - 5), (CsrInt16) (coor->skTop + 19),
        (CsrInt16) (coor->skLeft + CANVAS_WIDTH / 2 + 4), (CsrInt16) (coor->skTop + 19), WHITE, 0, FALSE);
    instanceData->graphicsRenderer.drawBox((CsrInt16) (coor->skLeft + CANVAS_WIDTH / 2 - 6), (CsrInt16) (coor->skTop + 18),
        (CsrInt16) (coor->skLeft + CANVAS_WIDTH / 2 + 5), (CsrInt16) (coor->skTop + 18), WHITE, 0, FALSE);
    instanceData->graphicsRenderer.drawBox((CsrInt16) (coor->skLeft + CANVAS_WIDTH / 2 - 7), (CsrInt16) (coor->skTop + 17),
        (CsrInt16) (coor->skLeft + CANVAS_WIDTH / 2 + 6), (CsrInt16) (coor->skTop + 17), WHITE, 0, FALSE);
}

static void statusSoftKeyAreaClear(CsrUiInstanceData *instanceData)
{
    /* Clear Status Area */
    instanceData->graphicsRenderer.drawBox(instanceData->coor.statusLeft, instanceData->coor.statusTop, (CsrInt16) (instanceData->coor.statusLeft + STATUS_WIDTH - 1), (CsrInt16) (instanceData->coor.statusTop + STATUS_HEIGHT - 1 - 4), STATUS_COLOR0, STATUS_COLOR0, TRUE);
    instanceData->graphicsRenderer.drawBox(instanceData->coor.statusLeft, (CsrInt16) (instanceData->coor.statusTop + STATUS_HEIGHT - 4), (CsrInt16) (instanceData->coor.statusLeft + STATUS_WIDTH - 1), (CsrInt16) (instanceData->coor.statusTop + STATUS_HEIGHT - 4), STATUS_COLOR1, 0, FALSE);
    instanceData->graphicsRenderer.drawBox(instanceData->coor.statusLeft, (CsrInt16) (instanceData->coor.statusTop + STATUS_HEIGHT - 3), (CsrInt16) (instanceData->coor.statusLeft + STATUS_WIDTH - 1), (CsrInt16) (instanceData->coor.statusTop + STATUS_HEIGHT - 3), STATUS_COLOR2, 0, FALSE);
    instanceData->graphicsRenderer.drawBox(instanceData->coor.statusLeft, (CsrInt16) (instanceData->coor.statusTop + STATUS_HEIGHT - 2), (CsrInt16) (instanceData->coor.statusLeft + STATUS_WIDTH - 1), (CsrInt16) (instanceData->coor.statusTop + STATUS_HEIGHT - 2), STATUS_COLOR3, 0, FALSE);
    instanceData->graphicsRenderer.drawBox(instanceData->coor.statusLeft, (CsrInt16) (instanceData->coor.statusTop + STATUS_HEIGHT - 1), (CsrInt16) (instanceData->coor.statusLeft + STATUS_WIDTH - 1), (CsrInt16) (instanceData->coor.statusTop + STATUS_HEIGHT - 1), STATUS_COLOR4, 0, FALSE);

    /* Clear Softkey Area */
    instanceData->graphicsRenderer.drawBox(instanceData->coor.skLeft, instanceData->coor.skTop, instanceData->coor.skRight, instanceData->coor.skTop, SK_COLOR3, 0, FALSE);
    instanceData->graphicsRenderer.drawBox(instanceData->coor.skLeft, (CsrInt16) (instanceData->coor.skTop + 1), instanceData->coor.skRight, (CsrInt16) (instanceData->coor.skTop + 1), SK_COLOR2, 0, FALSE);
    instanceData->graphicsRenderer.drawBox(instanceData->coor.skLeft, (CsrInt16) (instanceData->coor.skTop + 2), instanceData->coor.skRight, (CsrInt16) (instanceData->coor.skTop + 2), SK_COLOR1, 0, FALSE);
    instanceData->graphicsRenderer.drawBox(instanceData->coor.skLeft, (CsrInt16) (instanceData->coor.skTop + 3), instanceData->coor.skRight, instanceData->coor.skBottom, SK_COLOR0, SK_COLOR0, TRUE);
}

/* Determine whether a menu item is double height */
static CsrBool menuItemDoubleHeightGet(MenuItem *menuitem)
{
    if (menuitem->sublabel)
    {
        return TRUE;
    }
    return FALSE;
}

/* Render a string, truncate to Cap characters and return actual length */
static CsrUint32 printFontStringCap(CsrUiInstanceData *instanceData, CsrUint16 Cap, CsrInt16 fontNumber, CsrInt16 x, CsrInt16 y, CsrUint16 *string, CsrUint32 foregroundColor)
{
    CsrUint32 stringlen;
    CsrUint16 bc;

    stringlen = CsrUiUtilStringLength(string);
    if (stringlen > Cap)
    {
        /* Truncate the string */
        stringlen = Cap;
    }

    bc = string[stringlen];
    string[stringlen] = 0;

    CsrUiGraphicsDriverPrintFontString(instanceData, fontNumber, x, y, string, foregroundColor);

    string[stringlen] = bc;

    return stringlen;
}

/* Count number of characters before next line end */
static CsrUint16 getLineLength(CsrUint16 *string)
{
    CsrUint16 c;

    /* Count until new line or terminator */
    for (c = 0; (*string != 0) && (*string != 0x0A); string++)
    {
        c++;
    }

    return c;
}

/* Render a string, respecting newline characters.
   If width is greater than 0 the text will be centered in the area between x and x+width.
   If width is 0 the text will be left-aligned to x. */
static void printFontString(CsrUiInstanceData *instanceData, CsrInt16 fontNumber, CsrInt16 x, CsrInt16 y, CsrUint16 width, CsrUint16 *string, CsrUint16 cursor, CsrUint32 foregroundColor)
{
    CsrInt16 characterWidth;
    CsrInt16 characterHeight;
    CsrInt16 currentLine;
    CsrInt16 xc;

    currentLine = 0;

    characterWidth = CsrUiGraphicsDriverGetFontWidth(fontNumber);
    characterHeight = CsrUiGraphicsDriverGetFontHeight(fontNumber);

    if (width > 0)
    {
        xc = (CsrInt16) (x + (width - getLineLength(string) * characterWidth) / 2);
    }
    else
    {
        xc = x;
    }

    if (string && *string)
    {
        while (*string)
        {
            if (*string == 0x0A) /* New Line */
            {
                currentLine++;
                string++;
                if (width > 0)
                {
                    xc = (CsrInt16) (x + (width - getLineLength(string) * characterWidth) / 2);
                }
                else
                {
                    xc = x;
                }
                continue;
            }
            CsrUiGraphicsDriverPutFontCharacter(instanceData, fontNumber, xc, (CsrInt16) (y + currentLine * characterHeight), *string++, foregroundColor);
            xc += characterWidth;
        }
        if (cursor)
        {
            CsrInt16 xpos = (CsrInt16) (xc - (cursor - 1) * characterWidth);
            if ((cursor == 2) && ((*(string - 1) == ' ') || (*(string - 1) == '\n')))
            {
                /* Skip one character position if cursor is waiting for completion of a space or a new line */
                xpos += characterWidth;
            }
            instanceData->graphicsRenderer.drawBox(xpos, (CsrInt16) (y + currentLine * characterHeight), xpos, (CsrInt16) (y + (1 + currentLine) * characterHeight), foregroundColor, 0, FALSE);
        }
    }
}

/* Render a heading at a fixed position */
static void renderHeading(CsrUiInstanceData *instanceData, CsrUint16 *string, CsrBool underline)
{
    CsrUint32 stringlen;
    CsrUint16 bc;

    stringlen = CsrUiUtilStringLength(string);
    if (stringlen > HEADING_CHARACTERS)
    {
        /* Truncate the string */
        stringlen = HEADING_CHARACTERS;
    }

    bc = string[stringlen];
    string[stringlen] = 0;

    CsrUiGraphicsDriverPrintFontString(instanceData, HEADING_FONT, (CsrInt16) (instanceData->coor.canvasLeft + (CANVAS_WIDTH - stringlen * HEADING_FONT_WIDTH) / 2), (CsrInt16) (instanceData->coor.canvasTop + HEADING_TOPSKIP), string, HEADING_FONT_COLOR);
    if (underline)
    {
        instanceData->graphicsRenderer.drawBox((CsrInt16) (instanceData->coor.canvasLeft + (CANVAS_WIDTH - stringlen * HEADING_FONT_WIDTH) / 2 - 1), (CsrInt16) (instanceData->coor.canvasTop + HEADING_TOPSKIP + HEADING_FONT_HEIGHT + 1), (CsrInt16) (instanceData->coor.canvasLeft + (CANVAS_WIDTH - stringlen * HEADING_FONT_WIDTH) / 2 + stringlen * HEADING_FONT_WIDTH), (CsrInt16) (instanceData->coor.canvasTop + HEADING_TOPSKIP + HEADING_FONT_HEIGHT + HEADING_RULER_HEIGHT), HEADING_RULER_COLOR, HEADING_RULER_COLOR, TRUE);
    }

    string[stringlen] = bc;
}

/* Paint a menu item at the specified vertical position, return the new vertical position */
static CsrInt16 renderMenuItem(CsrUiInstanceData *instanceData, CsrInt16 vpos, MenuItem *menuitem, MenuItem *currentItem)
{
    CsrBool doubleHeight;
    CsrInt16 indent, width;
    CsrInt16 vposIcon;

    doubleHeight = menuItemDoubleHeightGet(menuitem);

    if (menuitem->icon)
    {
        indent = 0;
        width = MENUITEM_CHARACTERS;
        while (indent < MENUITEM_ICON_WIDTH)
        {
            indent += MENUITEM_FONT_WIDTH;
            width--;
        }

        vposIcon = vpos + (MENUITEM_FONT_HEIGHT - MENUITEM_ICON_HEIGHT) / 2;

        /* Draw Icon */
        CsrUiIconPut(instanceData, menuitem->icon, (CsrInt16) (instanceData->coor.canvasLeft + MENUITEM_INDENT), vposIcon);
    }
    else
    {
        indent = 0;
        width = MENUITEM_CHARACTERS;
    }

    if (menuitem->label && menuitem->sublabel)
    {
        if (menuitem == currentItem) /* Print item label with cursor */
        {
            instanceData->graphicsRenderer.drawBox((CsrInt16) (instanceData->coor.canvasLeft + MENUITEM_INDENT + indent), (CsrInt16) (vpos - 1), (CsrInt16) (instanceData->coor.canvasLeft + MENUITEM_INDENT + MENUITEM_CHARACTERS * MENUITEM_FONT_WIDTH - 1), (CsrInt16) (vpos + MENUITEM_FONT_HEIGHT * 2 - 1), MENUITEM_CURSOR_COLOR, MENUITEM_CURSOR_COLOR, TRUE);
            printFontStringCap(instanceData, width, MENUITEM_FONT, (CsrInt16) (instanceData->coor.canvasLeft + MENUITEM_INDENT + indent), (CsrInt16) (vpos), menuitem->label, MENUITEM_FONT_SELCOLOR);
            printFontStringCap(instanceData, width, MENUITEM_FONT, (CsrInt16) (instanceData->coor.canvasLeft + MENUITEM_INDENT + indent), (CsrInt16) (vpos + MENUITEM_FONT_HEIGHT), menuitem->sublabel, MENUITEM_FONT_SELSUBCOLOR);
        }
        else
        {
            printFontStringCap(instanceData, width, MENUITEM_FONT, (CsrInt16) (instanceData->coor.canvasLeft + MENUITEM_INDENT + indent), (CsrInt16) (vpos), menuitem->label, MENUITEM_FONT_COLOR);
            printFontStringCap(instanceData, width, MENUITEM_FONT, (CsrInt16) (instanceData->coor.canvasLeft + MENUITEM_INDENT + indent), (CsrInt16) (vpos + MENUITEM_FONT_HEIGHT), menuitem->sublabel, MENUITEM_FONT_SUBCOLOR);
        }
    }
    else if (menuitem->label && (menuitem->sublabel == NULL))
    {
        if (menuitem == currentItem) /* Print item label with cursor */
        {
            instanceData->graphicsRenderer.drawBox((CsrInt16) (instanceData->coor.canvasLeft + MENUITEM_INDENT + indent), (CsrInt16) (vpos - 1), (CsrInt16) (instanceData->coor.canvasLeft + MENUITEM_INDENT + MENUITEM_CHARACTERS * MENUITEM_FONT_WIDTH - 1), (CsrInt16) (vpos + MENUITEM_FONT_HEIGHT - 1), MENUITEM_CURSOR_COLOR, MENUITEM_CURSOR_COLOR, TRUE);
            printFontStringCap(instanceData, width, MENUITEM_FONT, (CsrInt16) (instanceData->coor.canvasLeft + MENUITEM_INDENT + indent), (CsrInt16) (vpos), menuitem->label, MENUITEM_FONT_SELCOLOR);
        }
        else
        {
            printFontStringCap(instanceData, width, MENUITEM_FONT, (CsrInt16) (instanceData->coor.canvasLeft + MENUITEM_INDENT + indent), (CsrInt16) (vpos), menuitem->label, MENUITEM_FONT_COLOR);
        }
    }

    if (doubleHeight)
    {
        return vpos + MENUITEM_FONT_HEIGHT * 2 + MENUITEM_SPACING;
    }
    else
    {
        return vpos + MENUITEM_FONT_HEIGHT + MENUITEM_SPACING;
    }
}

static void renderStatus(CsrUiInstanceData *instanceData)
{
    CsrInt16 currentPosition = instanceData->coor.statusLeft + STATUS_EDGE_LEFT_MARGIN + RADIOMETER_WIDTH + STATUS_ICON_SKIP;

    /* Render battery and radiometer into fixed positions */
    renderStatusRadiometerSet(instanceData, (CsrInt16) (instanceData->coor.statusLeft + STATUS_EDGE_LEFT_MARGIN), (CsrInt16) (instanceData->coor.statusTop + STATUS_RADIOMETER_TOP_MARGIN), instanceData->rssiPercentage);
    renderStatusBatterySet(instanceData, (CsrInt16) (instanceData->coor.statusLeft + STATUS_WIDTH - BATTERY_WIDTH - STATUS_EDGE_RIGHT_MARGIN - 1), (CsrInt16) (instanceData->coor.statusTop + STATUS_BATTERY_TOP_MARGIN), instanceData->batteryPercentage);

    /* Bluetooth On/Off indicator */
    if (instanceData->bluetoothActive)
    {
        renderStatusBluetooth(instanceData, currentPosition, (CsrInt16) (instanceData->coor.statusTop + STATUS_ICON_TOP_MARGIN));
        currentPosition += CSR_UI_STATUS_ICON_BLUETOOTH_WIDTH + STATUS_ICON_SKIP;
    }

    /* WiFi On/Off indicator */
    if (instanceData->wifiActive)
    {
        renderStatusWifi(instanceData, currentPosition, (CsrInt16) (instanceData->coor.statusTop + STATUS_ICON_TOP_MARGIN));
        currentPosition += CSR_UI_STATUS_ICON_WIFI_WIDTH + STATUS_ICON_SKIP;
    }

    /* Headsets */
    currentPosition += renderStatusHeadset(instanceData, currentPosition, (CsrInt16) (instanceData->coor.statusTop + STATUS_ICON_TOP_MARGIN), instanceData->headset1);
    currentPosition += renderStatusHeadset(instanceData, currentPosition, (CsrInt16) (instanceData->coor.statusTop + STATUS_ICON_TOP_MARGIN), instanceData->headset2);
}

/* Get the number of on-screen items in the menu */
static CsrUint16 onscreenMenuitemsGet(Menu *menu)
{
    CsrUint16 skip, c;
    CsrInt16 vspace;
    MenuItem *ptr;

    skip = menu->scrollGUI;
    c = 0;
    vspace = MENUITEM_HEIGHT;

    for (ptr = menu->items; ptr != NULL; ptr = ptr->next)
    {
        if (skip)
        {
            skip--;
            continue;
        }

        if (menuItemDoubleHeightGet(ptr))
        {
            vspace -= 2 * MENUITEM_FONT_HEIGHT + MENUITEM_SPACING;
        }
        else
        {
            vspace -= MENUITEM_FONT_HEIGHT + MENUITEM_SPACING;
        }

        if (vspace < 0)
        {
            break;
        }

        c++;
    }

    return c;
}

void CsrUiRenderRedraw(CsrUiInstanceData *instanceData)
{
    DSitem *dsitem = (DSitem *) instanceData->DisplayStack;

    /* Make sure that the physical display is not redrawn while the framebuffer is updated */
    instanceData->graphicsRenderer.lock();

    /* Clear the status and softkey area */
    statusSoftKeyAreaClear(instanceData);

    /* Render the status area */
    renderStatus(instanceData);

    /* Clear the canvas */
    instanceData->graphicsRenderer.drawBox(instanceData->coor.canvasLeft, instanceData->coor.canvasTop, (CsrInt16) (instanceData->coor.canvasLeft + CANVAS_WIDTH - 1), (CsrInt16) (instanceData->coor.canvasTop + CANVAS_HEIGHT - 1), CANVAS_BACKGROUND_COLOR, CANVAS_BACKGROUND_COLOR, TRUE);

    if (dsitem != NULL)
    {
        void *uie;
        uie = CsrUiUieHandle2Pointer(instanceData, dsitem->handle);
        switch (*((CsrUieType *) uie))
        {
            case CSR_UI_UIETYPE_MENU:
            {
                Menu *menu;
                MenuItem *menuitem;
                CsrUint16 vpos, skip, count, displayedItems;

                menu = (Menu *) uie;

                /* Heading */
                if (menu->heading)
                {
                    renderHeading(instanceData, menu->heading, TRUE);
                }

                vpos = instanceData->coor.menuitemStart;
                displayedItems = onscreenMenuitemsGet(menu);

                /* Menu items */
                skip = menu->scrollGUI;
                count = 0;
                for (menuitem = menu->items; menuitem != NULL; menuitem = menuitem->next)
                {
                    if (skip)     /* Scroll down */
                    {
                        skip--;
                        continue;
                    }

                    if (++count > displayedItems)     /* Stop when screen is full */
                    {
                        break;
                    }

                    vpos = renderMenuItem(instanceData, vpos, menuitem, menu->currentItem);
                }

                renderScrollableSet(instanceData);

                /* Softkeys */
                renderSKTextSet(instanceData, CSR_UI_KEY_SK1, menu->textSK1);
                renderSKTextSet(instanceData, CSR_UI_KEY_SK2, menu->textSK2);
                break;
            }

            case CSR_UI_UIETYPE_DIALOG:
            {
                Dialog *dialog;
                CsrUint16 vpos;

                dialog = (Dialog *) uie;

                /* Heading */
                if (dialog->heading)
                {
                    renderHeading(instanceData, dialog->heading, FALSE);
                }

                /* Box */
                instanceData->graphicsRenderer.drawBox(instanceData->coor.dialogBoxLeft, instanceData->coor.dialogBoxTop, instanceData->coor.dialogBoxRight, instanceData->coor.dialogBoxBottom, DIALOG_EDGE_COLOR, 0, FALSE);
                instanceData->graphicsRenderer.drawBox((CsrInt16) (instanceData->coor.dialogBoxLeft + 1), (CsrInt16) (instanceData->coor.dialogBoxTop + 1), (CsrInt16) (instanceData->coor.dialogBoxRight - 1), (CsrInt16) (instanceData->coor.dialogBoxBottom - 1), DIALOG_EDGE_COLOR, 0, FALSE);

                /* Apply hard word wrap to message */
                if (dialog->message)
                {
                    CsrUint16 *messagewrapped;
                    CsrUint16 *stringstart;
                    CsrUint16 *pstr;
                    CsrUint16 c;

                    messagewrapped = CsrUiUtilWordWrap(DIALOG_CHARACTERS, 0, dialog->message);

                    /* Skip scroll number of lines */
                    stringstart = messagewrapped;
                    for (c = dialog->scrollGUI; (*stringstart != 0) && (c > 0); stringstart++)
                    {
                        if (*stringstart == 0x0A)
                        {
                            c--;
                        }
                    }

                    /* Cap number of lines */
                    c = 0;
                    for (pstr = stringstart; (*pstr != 0) && (c < DIALOG_LINES); pstr++)
                    {
                        if (*pstr == 0x0A)
                        {
                            if (++c == DIALOG_LINES)
                            {
                                *pstr = 0;
                                break;
                            }
                        }
                    }

                    /* Starting position for first line of message */
                    vpos = (instanceData->coor.dialogTextTop + ((DIALOG_LINES - CsrUiUtilStringLines(stringstart)) * DIALOG_FONT_HEIGHT) / 2);
                    printFontString(instanceData, DIALOG_FONT, instanceData->coor.dialogTextLeft, vpos, (CsrInt16) (DIALOG_CHARACTERS * DIALOG_FONT_WIDTH), stringstart, 0, DIALOG_FONT_COLOR);

                    CsrPmemFree(messagewrapped);
                }

                renderScrollableSet(instanceData);

                /* Softkeys */
                renderSKTextSet(instanceData, CSR_UI_KEY_SK1, dialog->textSK1);
                renderSKTextSet(instanceData, CSR_UI_KEY_SK2, dialog->textSK2);
                break;
            }

            case CSR_UI_UIETYPE_INPUTDIALOG:
            {
                InputDialog *inputDialog;
                CsrUint16 vpos, displace;

                inputDialog = (InputDialog *) uie;

                displace = 0;
                if (inputDialog->icon && inputDialog->message)
                {
                    CsrUint16 *messagewrapped;
                    CsrUint16 lines;

                    /* Heading */
                    if (inputDialog->heading)
                    {
                        renderHeading(instanceData, inputDialog->heading, TRUE);
                    }

                    messagewrapped = CsrUiUtilWordWrap(INPUTDIALOG_MESSAGE_CHARACTERS, INPUTDIALOG_MESSAGE_LINES, inputDialog->message);

                    lines = CsrUiUtilStringLines(messagewrapped);

                    if (lines < 2)
                    {
                        lines = 2;
                    }

                    displace = INPUTDIALOG_MESSAGE_INDENT * 2 + INPUTDIALOG_FONT_HEIGHT * lines;

                    /* Render icon */
                    CsrUiIconPut(instanceData, inputDialog->icon, instanceData->coor.inputdialogIconLeft, instanceData->coor.inputdialogIconTop);

                    /* Render message */
                    printFontString(instanceData, INPUTDIALOG_FONT, instanceData->coor.inputdialogMessageLeft, instanceData->coor.inputdialogMessageTop, 0, messagewrapped, 0, INPUTDIALOG_FONT_COLOR);

                    CsrPmemFree(messagewrapped);
                }
                else if (inputDialog->heading)
                {
                    renderHeading(instanceData, inputDialog->heading, FALSE);
                }

                /* Box */
                instanceData->graphicsRenderer.drawBox(instanceData->coor.inputdialogBoxLeft, (CsrInt16) (instanceData->coor.inputdialogBoxTop + displace), instanceData->coor.inputdialogBoxRight, instanceData->coor.inputdialogBoxBottom, INPUTDIALOG_EDGE_COLOR, 0, FALSE);
                instanceData->graphicsRenderer.drawBox((CsrInt16) (instanceData->coor.inputdialogBoxLeft + 1), (CsrInt16) (instanceData->coor.inputdialogBoxTop + 1 + displace), (CsrInt16) (instanceData->coor.inputdialogBoxRight - 1), (CsrInt16) (instanceData->coor.inputdialogBoxBottom - 1), INPUTDIALOG_EDGE_COLOR, 0, FALSE);

                /* Apply hard word wrap to text */
                if (inputDialog->text)
                {
                    CsrUint16 displacelines;
                    CsrUint16 *textwrapped;

                    displacelines = 0;

                    while (displacelines * INPUTDIALOG_FONT_HEIGHT < displace)
                    {
                        displacelines++;
                    }

                    textwrapped = CsrUiUtilWordWrap(instanceData->coor.inputdialogCharacters, (CsrUint16) (instanceData->coor.inputdialogLines - displacelines), inputDialog->text);

                    /* Starting position for first line of message */
                    vpos = (instanceData->coor.inputdialogTextTop + ((displacelines + instanceData->coor.inputdialogLines - CsrUiUtilStringLines(textwrapped)) * INPUTDIALOG_FONT_HEIGHT) / 2);
                    printFontString(instanceData, INPUTDIALOG_FONT, instanceData->coor.inputdialogTextLeft, vpos, (CsrInt16) (instanceData->coor.inputdialogCharacters * INPUTDIALOG_FONT_WIDTH), textwrapped, (CsrUint16) (instanceData->MKkey ? 2 : 1), INPUTDIALOG_FONT_COLOR);

                    CsrPmemFree(textwrapped);
                }

                /* Softkeys */
                renderSKTextSet(instanceData, CSR_UI_KEY_SK1, inputDialog->textSK1);
                renderSKTextSet(instanceData, CSR_UI_KEY_SK2, inputDialog->textSK2);

                break;
            }

            case CSR_UI_UIETYPE_IDLESCREEN:
            {
                IdleScreen *idleScreen;

                idleScreen = (IdleScreen *) uie;

                /* Heading */
                if (idleScreen->heading)
                {
                    CsrUint32 stringlen;
                    CsrUint16 bc;

                    stringlen = CsrUiUtilStringLength(idleScreen->heading);
                    if (stringlen > IDLESCREEN_HEADING_CHARACTERS)
                    {
                        /* Truncate the string */
                        stringlen = IDLESCREEN_HEADING_CHARACTERS;
                    }

                    bc = idleScreen->heading[stringlen];
                    idleScreen->heading[stringlen] = 0;

                    CsrUiGraphicsDriverPrintFontString(instanceData, IDLESCREEN_HEADING_FONT, (CsrInt16) (instanceData->coor.canvasLeft + (CANVAS_WIDTH - stringlen * IDLESCREEN_HEADING_FONT_WIDTH) / 2), instanceData->coor.idlescreenHeadingTop, idleScreen->heading, IDLESCREEN_HEADING_FONT_COLOR);

                    idleScreen->heading[stringlen] = bc;
                }

                /* Render Logo */
                {
                    CsrInt32 index;
                    CsrInt32 xc, yc;

                    /* Render the icon in-place */
                    index = 0;
                    for (yc = CSR_UI_LOGO_HEIGHT; yc > 0; yc--)
                    {
                        for (xc = 0; xc < CSR_UI_LOGO_WIDTH; xc++)
                        {
                            if ((logoImageData[index] != 0xFF) || (logoImageData[index + 1] != 0xFF) || (logoImageData[index + 2] != 0xFF))
                            {
                                instanceData->graphicsRenderer.setPixel((CsrInt16) (instanceData->coor.idlescreenLogoLeft + xc), (CsrInt16) (instanceData->coor.idlescreenLogoTop + yc - 1), RGB(logoImageData[index + 2], logoImageData[index + 1], logoImageData[index]));
                            }
                            index += 3;
                        }
                    }
                }

                /* Apply hard word wrap to text */
                if (idleScreen->text)
                {
                    CsrUint16 *textwrapped;

                    textwrapped = CsrUiUtilWordWrap(IDLESCREEN_TEXT_CHARACTERS, IDLESCREEN_TEXT_LINES, idleScreen->text);
                    printFontString(instanceData, IDLESCREEN_TEXT_FONT, instanceData->coor.idlescreenTextLeft, instanceData->coor.idlescreenTextTop, IDLESCREEN_TEXT_CHARACTERS * IDLESCREEN_TEXT_FONT_WIDTH, textwrapped, 0, IDLESCREEN_TEXT_FONT_COLOR);

                    CsrPmemFree(textwrapped);
                }

                /* Softkeys */
                renderSKTextSet(instanceData, CSR_UI_KEY_SK1, idleScreen->textSK1);
                renderSKTextSet(instanceData, CSR_UI_KEY_SK2, CsrUiUtilStringLength(idleScreen->text) ? idleScreen->textSK2 : NULL);

                break;
            }

            default:
            {
                break;
            }
        }
    }

    instanceData->graphicsRenderer.unlock();
}

void CsrUiRenderCalculateScreenCoordinates(CsrUiInstanceData *instanceData)
{
    ScreenCoordinates *coor;

    coor = &(instanceData->coor);

    coor->statusLeft = instanceData->graphicsRenderer.left;
    coor->statusTop = instanceData->graphicsRenderer.top;

    coor->canvasLeft = instanceData->graphicsRenderer.left;
    coor->canvasTop = coor->statusTop + STATUS_HEIGHT;

    coor->skLeft = instanceData->graphicsRenderer.left;
    coor->skTop = coor->canvasTop + CANVAS_HEIGHT;
    coor->skRight = coor->skLeft + SCREEN_WIDTH - 1;
    coor->skBottom = coor->skTop + SK_HEIGHT - 1;
    coor->sk1Right = coor->skLeft + SCREEN_WIDTH / 2 - 1 - SK_SCROLL_WIDTH / 2;
    coor->sk2Left = coor->skRight - SCREEN_WIDTH / 2 + 1 + SK_SCROLL_WIDTH / 2;

    coor->menuitemStart = instanceData->graphicsRenderer.top + STATUS_HEIGHT + HEADING_TOPSKIP + HEADING_FONT_HEIGHT + HEADING_RULER_HEIGHT + HEADING_BOTTOMSKIP;

    coor->dialogBoxTop = coor->canvasTop + HEADING_TOPSKIP + HEADING_FONT_HEIGHT;
    coor->dialogBoxLeft = coor->canvasLeft + DIALOG_BOX_INDENT;
    coor->dialogBoxBottom = coor->canvasTop + CANVAS_HEIGHT - 1 - DIALOG_BOX_INDENT;
    coor->dialogBoxRight = coor->canvasLeft + CANVAS_WIDTH - 1 - DIALOG_BOX_INDENT;
    coor->dialogTextTop = coor->dialogBoxTop + DIALOG_TEXT_INDENT;
    coor->dialogTextLeft = coor->dialogBoxLeft + DIALOG_TEXT_INDENT;
    coor->dialogTextBottom = coor->dialogBoxBottom - DIALOG_TEXT_INDENT;
    coor->dialogTextRight = coor->dialogBoxRight - DIALOG_TEXT_INDENT;

    coor->inputdialogMessageTop = coor->canvasTop + HEADING_TOPSKIP + HEADING_FONT_HEIGHT + HEADING_RULER_HEIGHT + INPUTDIALOG_MESSAGE_INDENT;
    coor->inputdialogIconLeft = coor->canvasLeft + INPUTDIALOG_MESSAGE_INDENT;
    coor->inputdialogIconTop = coor->inputdialogMessageTop + (INPUTDIALOG_FONT_HEIGHT - INPUTDIALOG_ICON_HEIGHT) / 2;
    coor->inputdialogMessageLeft = coor->inputdialogIconLeft + INPUTDIALOG_ICON_WIDTH + INPUTDIALOG_MESSAGE_INDENT + 1;
    coor->inputdialogBoxTop = coor->canvasTop + HEADING_TOPSKIP + HEADING_FONT_HEIGHT;
    coor->inputdialogBoxLeft = coor->canvasLeft + INPUTDIALOG_BOX_INDENT;
    coor->inputdialogBoxBottom = coor->canvasTop + CANVAS_HEIGHT - 1 - INPUTDIALOG_BOX_INDENT;
    coor->inputdialogBoxRight = coor->canvasLeft + CANVAS_WIDTH - 1 - INPUTDIALOG_BOX_INDENT;
    coor->inputdialogTextTop = coor->inputdialogBoxTop + INPUTDIALOG_TEXT_INDENT;
    coor->inputdialogTextLeft = coor->inputdialogBoxLeft + INPUTDIALOG_TEXT_INDENT;
    coor->inputdialogTextBottom = coor->inputdialogBoxBottom - INPUTDIALOG_TEXT_INDENT;
    coor->inputdialogTextRight = coor->inputdialogBoxRight - INPUTDIALOG_TEXT_INDENT;
    coor->inputdialogLines = (coor->inputdialogTextBottom - coor->inputdialogTextTop + 1) / (INPUTDIALOG_FONT_HEIGHT);
    coor->inputdialogCharacters = (coor->inputdialogTextRight - coor->inputdialogTextLeft + 1) / INPUTDIALOG_FONT_WIDTH;

    coor->idlescreenHeadingTop = coor->canvasTop + IDLESCREEN_TOPSKIP;
    coor->idlescreenLogoTop = coor->canvasTop + IDLESCREEN_TOPSKIP + IDLESCREEN_HEADING_FONT_HEIGHT + IDLESCREEN_LOGO_TOPSKIP;
    coor->idlescreenLogoLeft = coor->canvasLeft + (CANVAS_WIDTH - IDLESCREEN_LOGO_WIDTH) / 2;
    coor->idlescreenTextTop = coor->idlescreenLogoTop + IDLESCREEN_LOGO_HEIGHT + IDLESCREEN_TEXT_TOPSKIP;
    coor->idlescreenTextLeft = coor->canvasLeft;
}

void CsrUiRenderMenuScrollUp(Menu *menu)
{
    CsrUint16 cursor;

    /* Check if cursor dropped out of the screen */
    cursor = CsrUiUieMenuGetCursorIndex(menu);
    if ((menu->scrollGUI > 0) && (cursor == menu->scrollGUI - 1))
    {
        menu->scrollGUI--;
    }
    if ((menu->scrollGUI > 0) && (cursor == menu->scrollGUI))
    {
        menu->scrollGUI--;
    }
}

void CsrUiRenderMenuScrollDown(Menu *menu)
{
    CsrUint16 displayItems = onscreenMenuitemsGet(menu);

    /* Check if cursor dropped out of the screen */
    if (CsrUiUieMenuGetCursorIndex(menu) == menu->scrollGUI + displayItems)
    {
        /* Scroll down to keep cursor inside the screen */
        menu->scrollGUI++;
        if (onscreenMenuitemsGet(menu) < displayItems)
        {
            /* Scroll one more line, because visible items count decreased */
            menu->scrollGUI++;
        }
    }
    if (menu->currentItem->next != NULL)
    {
        /* Check if cursor is at the last on-screen entry */
        displayItems = onscreenMenuitemsGet(menu);
        if (CsrUiUieMenuGetCursorIndex(menu) == menu->scrollGUI + displayItems - 1)
        {
            /* Scroll one entry down to reveal additional entries */
            menu->scrollGUI++;
            if (onscreenMenuitemsGet(menu) < displayItems)
            {
                /* Scroll one more line, because visible items count decreased */
                menu->scrollGUI++;
            }
        }
    }
}

void CsrUiRenderMenuScrollUpdate(Menu *menu)
{
    CsrUint16 position = CsrUiUieMenuGetCursorIndex(menu);
    CsrUint16 items = CsrUiUieMenuGetNumberOfItems(menu);
    CsrUint16 onScreenItems;

    if ((menu->scrollGUI > 0) && (position < menu->scrollGUI))
    {
        /* Scroll up if cursor is above screen */
        menu->scrollGUI = position > 0 ? position - 1 : 0;
    }
    else
    {
        /* Scroll down until cursor is on-screen */
        while ((menu->scrollGUI + (onScreenItems = onscreenMenuitemsGet(menu)) < items) && (position >= menu->scrollGUI + onScreenItems))
        {
            menu->scrollGUI++;
        }
    }
}

void CsrUiRenderMenuScrollDelete(Menu *menu, CsrUint16 index)
{
    if (menu->scrollGUI > index)
    {
        menu->scrollGUI--;
    }
}

CsrBool CsrUiRenderDialogScrollUp(Dialog *dialog)
{
    if (dialog->scrollGUI > 0)
    {
        dialog->scrollGUI--;
        return TRUE;
    }
    return FALSE;
}

CsrBool CsrUiRenderDialogScrollDown(Dialog *dialog)
{
    CsrUint16 *messagewrapped;

    if (dialog->message != NULL)
    {
        messagewrapped = CsrUiUtilWordWrap(DIALOG_CHARACTERS, 0, dialog->message);

        if (dialog->scrollGUI < CsrUiUtilStringLines(messagewrapped) - DIALOG_LINES)
        {
            dialog->scrollGUI++;
        }

        CsrPmemFree(messagewrapped);

        return TRUE;
    }
    return FALSE;
}
