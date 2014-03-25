/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_ui_lower.h"

#include "csr_ui_ctrl.h"
#include "csr_ui_uiemanager.h"
#include "csr_ui_util.h"
#include "csr_ui_displaystack.h"
#include "csr_ui_renderer_text.h"
#include "csr_ui_keycode.h"


/* Heading */
#define HEADING_HEIGHT                  2

/* Menu Item */
#define MENUITEM_LEFT_INDENT            3
#define MENUITEM_LINES                  (CSR_UI_TEXT_CANVAS_LINES - HEADING_HEIGHT)
#define MENUITEM_CHARACTERS             (CSR_UI_TEXT_CANVAS_CHARACTERS - MENUITEM_LEFT_INDENT)

/* Dialog */
#define DIALOG_TEXT_INDENT              1
#define DIALOG_LINES                    (CSR_UI_TEXT_CANVAS_LINES - HEADING_HEIGHT - 2 *DIALOG_TEXT_INDENT)
#define DIALOG_CHARACTERS               (CSR_UI_TEXT_CANVAS_CHARACTERS - 2 *DIALOG_TEXT_INDENT)

/* Input Dialog */
#define INPUTDIALOG_TEXT_INDENT         1
#define INPUTDIALOG_MESSAGE_LINES       2
#define INPUTDIALOG_MESSAGE_CHARACTERS  (CSR_UI_TEXT_CANVAS_CHARACTERS - 2 *INPUTDIALOG_TEXT_INDENT)
#define INPUTDIALOG_LINES               (CSR_UI_TEXT_CANVAS_LINES - HEADING_HEIGHT - 2 *INPUTDIALOG_TEXT_INDENT)
#define INPUTDIALOG_CHARACTERS          (CSR_UI_TEXT_CANVAS_CHARACTERS - 2 *INPUTDIALOG_TEXT_INDENT)

/* Idle Screen */
#define IDLESCREEN_TEXT_INDENT          1
#define IDLESCREEN_TOPSKIP              16
#define IDLESCREEN_HEADING_CHARACTERS   CSR_UI_TEXT_CANVAS_CHARACTERS
#define IDLESCREEN_TEXT_CHARACTERS      CSR_UI_TEXT_CANVAS_CHARACTERS
#define IDLESCREEN_TEXT_LINES           1

/* Paint a menu item at the specified vertical position, return the new vertical position */
static CsrUint16 *renderMenuItem(CsrUint16 *canvas, MenuItem *menuitem, CsrBool cursor)
{
    CsrInt32 cap = MENUITEM_CHARACTERS;
    CsrUint32 labelLength;
    CsrUint16 selected[] = {'-', '>', ' ', '\0'};
    CsrUint16 unselected[] = {' ', ' ', ' ', '\0'};
    CsrUint16 newline[] = {'\n', '\0'};

    if (cursor)
    {
        canvas = CsrUiUtilStringCat(canvas, selected);
    }
    else
    {
        canvas = CsrUiUtilStringCat(canvas, unselected);
    }

    labelLength = CsrUiUtilStringLength(menuitem->label);

    if (labelLength > 0)
    {
        canvas = CsrUiUtilStringCatCap(canvas, menuitem->label, cap);
        cap -= labelLength;
    }
    if (menuitem->sublabel != NULL)
    {
        CsrUint32 sublabelLength = CsrUiUtilStringLength(menuitem->sublabel);
        CsrUint16 spaceparenleft[] = {' ', '(', '\0'};
        CsrUint16 parenright[] = {')', '\0'};

        if ((sublabelLength > 0) && (cap > 3))
        {
            canvas = CsrUiUtilStringCat(canvas, spaceparenleft);
            canvas = CsrUiUtilStringCatCap(canvas, menuitem->sublabel, cap - 3);
            canvas = CsrUiUtilStringCat(canvas, parenright);
        }
    }
    return CsrUiUtilStringCat(canvas, newline);
}

/* Get the number of on-screen items in the menu */
static CsrUint16 onscreenMenuitemsGet(Menu *menu)
{
    CsrUint16 skip, c;
    MenuItem *ptr;

    skip = menu->scrollTUI;
    c = 0;

    for (ptr = menu->items; ptr != NULL; ptr = ptr->next)
    {
        if (skip)
        {
            skip--;
            continue;
        }

        if (++c == MENUITEM_LINES)
        {
            return MENUITEM_LINES;
        }
    }

    return c;
}

static void renderStatus(CsrUiInstanceData *instanceData, CsrUint16 *status)
{
    CsrUint16 bspace[] = {'B', ' ', '\0'};
    CsrUint16 wspace[] = {'W', ' ', '\0'};
    CsrUint16 h1space[] = {'H', '1', ' ', '\0'};
    CsrUint16 h2space[] = {'H', '2', ' ', '\0'};

    /* Bluetooth On/Off indicator */
    if (instanceData->bluetoothActive)
    {
        status = CsrUiUtilStringCat(status, bspace);
    }

    /* WiFi On/Off indicator */
    if (instanceData->wifiActive)
    {
        status = CsrUiUtilStringCat(status, wspace);
    }

    if (instanceData->headset1 >= CSR_UI_STATUS_HEADSET_PRESENT)
    {
        status = CsrUiUtilStringCat(status, h1space);
    }

    if (instanceData->headset2 >= CSR_UI_STATUS_HEADSET_PRESENT)
    {
        CsrUiUtilStringCat(status, h2space);
    }
}

static CsrUint16 *renderHeading(CsrUint16 *canvas, CsrUint16 *heading)
{
    CsrUint32 headingLength;
    CsrUint32 skip = 0;
    CsrUint16 newlinenewline[] = {'\n', '\n', '\0'};

    headingLength = CsrUiUtilStringLength(heading);

    if (headingLength > 0)
    {
        if (headingLength < CSR_UI_TEXT_CANVAS_CHARACTERS)
        {
            skip = (CSR_UI_TEXT_CANVAS_CHARACTERS - headingLength) / 2;
        }

        while (skip--)
        {
            CsrUint16 space[] = {' ', '\0'};
            canvas = CsrUiUtilStringCat(canvas, space);
        }

        canvas = CsrUiUtilStringCatCap(canvas, heading, CSR_UI_TEXT_CANVAS_CHARACTERS);

        if (headingLength > CSR_UI_TEXT_CANVAS_CHARACTERS)
        {
            headingLength = CSR_UI_TEXT_CANVAS_CHARACTERS;
        }
    }

    return CsrUiUtilStringCat(canvas, newlinenewline);
}

void CsrUiRenderTextRedraw(CsrUiInstanceData *instanceData)
{
    DSitem *dsitem = (DSitem *) instanceData->DisplayStack;
    CsrUint16 *status = CsrPmemAlloc((CSR_UI_TEXT_STATUS_CHARACTERS + 1) * 2);
    CsrUint16 *canvas = CsrPmemAlloc(CSR_UI_TEXT_CANVAS_CHARACTERS * (CSR_UI_TEXT_CANVAS_CHARACTERS + 1) * 2);
    CsrUint16 *canvasPtr;
    CsrUint16 *softkey1 = NULL;
    CsrUint16 *softkey2 = NULL;

    canvasPtr = canvas;
    *status = 0;
    *canvas = 0;

    renderStatus(instanceData, status);

    if (dsitem != NULL)
    {
        void *uie;
        uie = CsrUiUieHandle2Pointer(instanceData, dsitem->handle);
        switch (*((CsrUieType *) uie))
        {
            case CSR_UI_UIETYPE_MENU:
            {
                Menu *menu = (Menu *) uie;
                MenuItem *menuitem;
                CsrUint16 skip, count, displayedItems;

                /* Heading */
                canvasPtr = renderHeading(canvasPtr, menu->heading);

                displayedItems = onscreenMenuitemsGet(menu);

                /* Menu items */
                skip = menu->scrollTUI;
                count = 0;
                for (menuitem = menu->items; menuitem != NULL; menuitem = menuitem->next)
                {
                    if (skip) /* Scroll down */
                    {
                        skip--;
                        continue;
                    }

                    if (++count > displayedItems) /* Stop when screen is full */
                    {
                        break;
                    }

                    canvasPtr = renderMenuItem(canvasPtr, menuitem, (CsrBool) (menuitem == menu->currentItem));
                }

                /* Softkeys */
                softkey1 = menu->textSK1;
                softkey2 = menu->textSK2;
                break;
            }
            case CSR_UI_UIETYPE_DIALOG:
            {
                Dialog *dialog;

                dialog = (Dialog *) uie;

                /* Heading */
                canvasPtr = renderHeading(canvasPtr, dialog->heading);

                /* Apply hard word wrap to message */
                if (dialog->message != NULL)
                {
                    CsrUint16 *messagewrapped;
                    CsrUint16 *stringstart;
                    CsrUint16 *pstr;
                    CsrUint16 c;

                    messagewrapped = CsrUiUtilWordWrap(DIALOG_CHARACTERS, 0, dialog->message);

                    /* Skip scroll number of lines */
                    stringstart = messagewrapped;
                    for (c = dialog->scrollTUI; (*stringstart != '\0') && (c > 0); stringstart++)
                    {
                        if (*stringstart == '\n')
                        {
                            c--;
                        }
                    }

                    /* Cap number of lines */
                    c = 0;
                    for (pstr = stringstart; (*pstr != 0) && (c < DIALOG_LINES); pstr++)
                    {
                        if (*pstr == '\n')
                        {
                            if (++c == DIALOG_LINES)
                            {
                                *pstr = '\0';
                                break;
                            }
                        }
                    }

                    /* Starting position for first line of message */
                    CsrUiUtilStringCat(canvasPtr, stringstart);

                    CsrPmemFree(messagewrapped);
                }

                /* Softkeys */
                softkey1 = dialog->textSK1;
                softkey2 = dialog->textSK2;
                break;
            }
            case CSR_UI_UIETYPE_INPUTDIALOG:
            {
                InputDialog *inputDialog;

                inputDialog = (InputDialog *) uie;

                /* Heading */
                renderHeading(canvas, inputDialog->heading);

                if (inputDialog->message != NULL)
                {
                    CsrUint16 *messagewrapped;
                    CsrUint16 newlinenewline[] = {'\n', '\n', '\0'};

                    messagewrapped = CsrUiUtilWordWrap(INPUTDIALOG_MESSAGE_CHARACTERS, INPUTDIALOG_MESSAGE_LINES, inputDialog->message);

                    canvasPtr = CsrUiUtilStringCat(canvasPtr, messagewrapped);
                    canvasPtr = CsrUiUtilStringCat(canvasPtr, newlinenewline);

                    CsrPmemFree(messagewrapped);
                }

                /* Apply hard word wrap to text */
                if (inputDialog->text != NULL)
                {
                    CsrUint16 *textwrapped;
                    CsrUint16 newline[] = {'\n', '\0'};

                    textwrapped = CsrUiUtilWordWrap(INPUTDIALOG_CHARACTERS, INPUTDIALOG_LINES, inputDialog->text);

                    canvasPtr = CsrUiUtilStringCat(canvasPtr, textwrapped);
                    CsrUiUtilStringCat(canvasPtr, newline);

                    CsrPmemFree(textwrapped);
                }

                /* Softkeys */
                softkey1 = inputDialog->textSK1;
                softkey2 = inputDialog->textSK2;
                break;
            }
            case CSR_UI_UIETYPE_IDLESCREEN:
            {
                IdleScreen *idleScreen;
                CsrUint16 newline[] = {'\n', '\0'};

                idleScreen = (IdleScreen *) uie;

                /* Heading */
                renderHeading(canvas, idleScreen->heading);

                /* Apply hard word wrap to text */
                if (idleScreen->text != NULL)
                {
                    CsrUint16 *textwrapped;

                    textwrapped = CsrUiUtilWordWrap(IDLESCREEN_TEXT_CHARACTERS, IDLESCREEN_TEXT_LINES, idleScreen->text);

                    canvasPtr = CsrUiUtilStringCat(canvasPtr, textwrapped);
                    CsrUiUtilStringCat(canvasPtr, newline);

                    CsrPmemFree(textwrapped);
                }

                /* Softkeys */
                softkey1 = idleScreen->textSK1;
                if (CsrUiUtilStringLength(idleScreen->text) > 0)
                {
                    softkey2 = idleScreen->textSK2;
                }
                break;
            }

            default:
            {
                break;
            }
        }
    }

    instanceData->textRenderer.setText(status, canvas, softkey1, softkey2);

    CsrPmemFree(status);
    CsrPmemFree(canvas);
}

void CsrUiRenderTextMenuScrollUp(Menu *menu)
{
    CsrUint16 cursor;

    /* Check if cursor dropped out of the screen */
    cursor = CsrUiUieMenuGetCursorIndex(menu);
    if ((menu->scrollTUI > 0) && (cursor == menu->scrollTUI - 1))
    {
        menu->scrollTUI--;
    }
    if ((menu->scrollTUI > 0) && (cursor == menu->scrollTUI))
    {
        menu->scrollTUI--;
    }
}

void CsrUiRenderTextMenuScrollDown(Menu *menu)
{
    CsrUint16 displayItems = onscreenMenuitemsGet(menu);

    /* Check if cursor dropped out of the screen */
    if (CsrUiUieMenuGetCursorIndex(menu) == menu->scrollTUI + displayItems)
    {
        /* Scroll down to keep cursor inside the screen */
        menu->scrollTUI++;
        if (onscreenMenuitemsGet(menu) < displayItems)
        {
            /* Scroll one more line, because visible items count decreased */
            menu->scrollTUI++;
        }
    }
    if (menu->currentItem->next != NULL)
    {
        /* Check if cursor is at the last on-screen entry */
        displayItems = onscreenMenuitemsGet(menu);
        if (CsrUiUieMenuGetCursorIndex(menu) == menu->scrollTUI + displayItems - 1)
        {
            /* Scroll one entry down to reveal additional entries */
            menu->scrollTUI++;
            if (onscreenMenuitemsGet(menu) < displayItems)
            {
                /* Scroll one more line, because visible items count decreased */
                menu->scrollTUI++;
            }
        }
    }
}

void CsrUiRenderTextMenuScrollUpdate(Menu *menu)
{
    CsrUint16 position = CsrUiUieMenuGetCursorIndex(menu);
    CsrUint16 items = CsrUiUieMenuGetNumberOfItems(menu);
    CsrUint16 onScreenItems;

    if ((menu->scrollTUI > 0) && (position < menu->scrollTUI))
    {
        /* Scroll up if cursor is above screen */
        menu->scrollTUI = position > 0 ? position - 1 : 0;
    }
    else
    {
        /* Scroll down until cursor is on-screen */
        while ((menu->scrollTUI + (onScreenItems = onscreenMenuitemsGet(menu)) < items) && (position >= menu->scrollTUI + onScreenItems))
        {
            menu->scrollTUI++;
        }
    }
}

void CsrUiRenderTextMenuScrollDelete(Menu *menu, CsrUint16 index)
{
    if (menu->scrollTUI > index)
    {
        menu->scrollTUI--;
    }
}

CsrBool CsrUiRenderTextDialogScrollUp(Dialog *dialog)
{
    if (dialog->scrollTUI > 0)
    {
        dialog->scrollTUI--;
        return TRUE;
    }
    return FALSE;
}

CsrBool CsrUiRenderTextDialogScrollDown(Dialog *dialog)
{
    CsrUint16 *messagewrapped;

    if (dialog->message != NULL)
    {
        messagewrapped = CsrUiUtilWordWrap(DIALOG_CHARACTERS, 0, dialog->message);

        if (dialog->scrollTUI < CsrUiUtilStringLines(messagewrapped) - DIALOG_LINES)
        {
            dialog->scrollTUI++;
        }

        CsrPmemFree(messagewrapped);

        return TRUE;
    }
    return FALSE;
}
