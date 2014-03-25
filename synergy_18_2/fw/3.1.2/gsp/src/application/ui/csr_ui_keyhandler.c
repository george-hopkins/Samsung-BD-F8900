/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/


#include "csr_types.h"
#include "csr_ui_ctrl.h"
#include "csr_ui_exception.h"
#include "csr_ui_private_prim.h"
#include "csr_ui_displaystack.h"
#include "csr_ui_uiemanager.h"
#include "csr_ui_keycode.h"
#include "csr_ui_renderer.h"
#include "csr_ui_renderer_text.h"
#include "csr_ui_util.h"

#define MULTIKEY_DELAY (2 * (1000000))
#define NUMBER_OF_INPUTKEYS 12

#define CSR_UI_KEYMAP_NUMERIC_SUBKEYS 2
static const CsrUint16 CsrUiKeyMapNumeric[NUMBER_OF_INPUTKEYS][CSR_UI_KEYMAP_NUMERIC_SUBKEYS] =
{
    {'0', 0},
    {'1', 0},
    {'2', 0},
    {'3', 0},
    {'4', 0},
    {'5', 0},
    {'6', 0},
    {'7', 0},
    {'8', 0},
    {'9', 0},
    {0, 0},
    {0, 0}
};

#define CSR_UI_KEYMAP_CONTROLNUMERIC_SUBKEYS 4
static const CsrUint16 CsrUiKeyMapControlnumeric[NUMBER_OF_INPUTKEYS][CSR_UI_KEYMAP_CONTROLNUMERIC_SUBKEYS] =
{
    {'0', '+', '-', 0},
    {'1', 0, 0, 0},
    {'2', 0, 0, 0},
    {'3', 0, 0, 0},
    {'4', 0, 0, 0},
    {'5', 0, 0, 0},
    {'6', 0, 0, 0},
    {'7', 0, 0, 0},
    {'8', 0, 0, 0},
    {'9', 0, 0, 0},
    {'*', '.', ',', 0},
    {'#', ' ', '\n', 0}
};

#define CSR_UI_KEYMAP_ALPHANUMERIC_SUBKEYS 7
static const CsrUint16 CsrUiKeyMapAlphanumeric[NUMBER_OF_INPUTKEYS][CSR_UI_KEYMAP_ALPHANUMERIC_SUBKEYS] =
{
    {'+', '0', 0, 0, 0, 0, 0},
    {'.', ',', '-', '?', '!', '1', 0},
    {'a', 'b', 'c', '2', 0, 0, 0},
    {'d', 'e', 'f', '3', 0, 0, 0},
    {'g', 'h', 'i', '4', 0, 0, 0},
    {'j', 'k', 'l', '5', 0, 0, 0},
    {'m', 'n', 'o', '6', 0, 0, 0},
    {'p', 'q', 'r', 's', '7', 0, 0},
    {'t', 'u', 'v', '8', 0, 0, 0},
    {'w', 'x', 'y', 'z', '9', 0, 0},
    {0, 0, 0, 0, 0, 0, 0}, /* Case Switch (a/A) */
    {' ', '\n', '#', '*', 0, 0, 0}
};

static CsrBool isInputKey(CsrUint16 key)
{
    if ((key >= CSR_UI_KEY_N0) && (key <= CSR_UI_KEY_HASH))
    {
        return TRUE;
    }
    return FALSE;
}

static CsrBool isAsciiKey(CsrUint16 key)
{
    if ((key >= CSR_UI_KEY_ASCII(0x20)) && (key <= CSR_UI_KEY_ASCII(0x7E)))
    {
        return TRUE;
    }
    return FALSE;
}

/* Translate a CSR_UI_KEY and sub key index to a UCS2 character using the specified key map */
static CsrUint16 CsrUiKeyTranslateKey(CsrUint8 keyMap, CsrUint16 key, CsrUint8 index)
{
    if (isInputKey(key))
    {
        switch (keyMap)
        {
            case CSR_UI_KEYMAP_NUMERIC:
                if (index < CSR_UI_KEYMAP_NUMERIC_SUBKEYS)
                {
                    return CsrUiKeyMapNumeric[key & 0x7FFF][index];
                }
                else
                {
                    CSR_UIEXCEPTION("Invalid sub key in key map CSR_UI_KEYMAP_NUMERIC specified");
                }
                break;
            case CSR_UI_KEYMAP_CONTROLNUMERIC:
                if (index < CSR_UI_KEYMAP_CONTROLNUMERIC_SUBKEYS)
                {
                    return CsrUiKeyMapControlnumeric[key & 0x7FFF][index];
                }
                else
                {
                    CSR_UIEXCEPTION("Invalid sub key in key map CSR_UI_KEYMAP_CONTROLNUMERIC specified");
                }
                break;
            case CSR_UI_KEYMAP_ALPHANUMERIC:
                if (index < CSR_UI_KEYMAP_ALPHANUMERIC_SUBKEYS)
                {
                    return CsrUiKeyMapAlphanumeric[key & 0x7FFF][index];
                }
                else
                {
                    CSR_UIEXCEPTION("Invalid sub key in key map CSR_UI_KEYMAP_ALPHANUMERIC specified");
                }
                break;
            default:
                CSR_UIEXCEPTION("Invalid key map specified");
                break;
        }
    }
    else
    {
        CSR_UIEXCEPTION("Invalid key");
    }

    return 0;
}

/* Receives key events from hardware and queues a message for the controller */
void CsrUiKeyEvent(CsrUint16 key)
{
    CsrUiInternalKeydownInd *message;
    message = CsrPmemAlloc(sizeof(CsrUiInternalKeydownInd));
    message->type = CSR_UI_INTERNAL_KEYDOWN_IND;
    message->key = key;
    CsrSchedMessagePut(CSR_UI_IFACEQUEUE, CSR_UI_PRIM, message);
}

/* Executes an action (must be non-zero) - returns TRUE if the display should be redrawn and FALSE otherwise */
static CsrBool CsrUiKeyExecuteAction(CsrUiInstanceData *instanceData, CsrUieHandle action, CsrUint16 key, CsrUieHandle displayElement)
{
    void *uie;
    DSitem *dsitem;

    uie = CsrUiUieHandle2Pointer(instanceData, action);

    /* Check if action is an Event */
    if (*((CsrUieType *) uie) == CSR_UI_UIETYPE_EVENT)
    {
        Event *event;
        CsrUiEventInd *message;

        event = (Event *) uie;
        message = CsrPmemAlloc(sizeof(CsrUiEventInd));

        message->type = CSR_UI_EVENT_IND;
        message->key = key;
        message->event = event->handle;
        message->inputMode = event->inputMode;
        message->displayElement = displayElement;

        ((DSitem *) instanceData->DisplayStack)->inputMode = event->inputMode;

        CsrSchedMessagePut(event->listener, CSR_UI_PRIM, message);
        return FALSE;
    }

    /* action is not an Event, display the UIE */
    dsitem = (DSitem *) instanceData->DisplayStack;
    CsrUiDisplayUieShow(instanceData, action, dsitem->priority, dsitem->inputMode, dsitem->listener);
    return TRUE;
}

/* Convert an alphabetical character to upper case if it is lower case */
static CsrUint16 CsrUiKeyCapitalise(CsrUint16 character)
{
    if ((character >= 'a') && (character <= 'z'))
    {
        return (CsrUint16) (character - ('a' - 'A'));
    }
    return character;
}

/* Add character to string - return TRUE if character was added */
static CsrBool CsrUiKeyAddToString(CsrUint16 *string, CsrUint16 stringLength, CsrUint16 character, CsrBool capitalise)
{
    CsrUint16 *pchar;
    CsrUint16 C = 0;

    if (!string)
    {
        /* String does not exist */
        return FALSE;
    }

    for (pchar = string; (*pchar != 0) && (C <= stringLength); pchar++)
    {
        C++;
    }

    if (C >= stringLength)
    {
        /* No more space in string */
        return FALSE;
    }

    *pchar = capitalise ? CsrUiKeyCapitalise(character) : character;
    return TRUE;
}

/* Delete last character from string - return TRUE if character was deleted */
static CsrBool CsrUiKeyDeleteFromString(CsrUint16 *string)
{
    if (!string)
    {
        /* String does not exist */
        return FALSE;
    }

    if (*string == 0)
    {
        /* String is empty */
        return FALSE;
    }

    for ( ; *(string + 1) != 0; string++)
    {
    }

    *string = 0;
    return TRUE;
}

void CsrUiKeyMultiKeyReset(CsrUiInstanceData *instanceData)
{
    if (instanceData->MKkey != 0)
    {
        CsrSchedTimerCancel(instanceData->MKtid, NULL, NULL);
    }
    instanceData->MKkey = 0;
    instanceData->MKcaps = FALSE;
    instanceData->MKindex = 0;
}

/* Triggered when the Multi Key Timer expires */
void CsrUiKeyMultiKeyTimerEvent(CsrUint16 pmi, void *pmv)
{
    CsrUiInstanceData *instanceData;
    instanceData = (CsrUiInstanceData *) pmv;
    instanceData->MKkey = 0;
    if (instanceData->graphicsRenderer.active)
    {
        CsrUiRenderRedraw(instanceData);
    }
    if (instanceData->textRenderer.active)
    {
        CsrUiRenderTextRedraw(instanceData);
    }
}

/* Multi Key Handler */
static CsrBool CsrUiKeyMultiKeyHandler(CsrUiInstanceData *instanceData, CsrUint16 key, CsrUint16 *string, CsrUint16 stringLength, CsrUint8 keyMap)
{
    /* Check for ASCII */
    if (isAsciiKey(key))
    {
        /* Revert to Idle State if in MK State */
        if (instanceData->MKkey != 0)
        {
            CsrSchedTimerCancel(instanceData->MKtid, NULL, NULL);
            instanceData->MKkey = 0;
        }

        /* Raw insert the character (stripping the encoding) */
        return CsrUiKeyAddToString(string, stringLength, (CsrUint16) (key & 0x7F), FALSE);
    }

    /* Check Caps Lock */
    if ((keyMap == CSR_UI_KEYMAP_ALPHANUMERIC) && (key == CSR_UI_KEY_STAR))
    {
        /* Revert to Idle State if in MK State */
        if (instanceData->MKkey != 0)
        {
            CsrSchedTimerCancel(instanceData->MKtid, NULL, NULL);
            instanceData->MKkey = 0;
        }

        /* Toggle the Caps Lock flag */
        instanceData->MKcaps = !instanceData->MKcaps;
        return TRUE;
    }

    if (instanceData->MKkey == 0) /* Idle State */
    {
        if (key == CSR_UI_KEY_DEL)
        {
            return CsrUiKeyDeleteFromString(string);
        }
        if ((key == CSR_UI_KEY_LEFT) || (key == CSR_UI_KEY_RIGHT))
        {
            /* Move cursor */
            return TRUE;
        }
        if (isInputKey(key))
        {
            if (CsrUiKeyTranslateKey(keyMap, key, 1) != 0)
            {
                instanceData->MKtid = CsrSchedTimerSet(MULTIKEY_DELAY, CsrUiKeyMultiKeyTimerEvent, 0, instanceData);
                instanceData->MKkey = key;
                instanceData->MKindex = 0;
            }
            return CsrUiKeyAddToString(string, stringLength, CsrUiKeyTranslateKey(keyMap, key, 0), instanceData->MKcaps);
        }
    }
    else /* MK State */
    {
        CsrSchedTimerCancel(instanceData->MKtid, NULL, NULL);

        if (key == CSR_UI_KEY_DEL)
        {
            instanceData->MKkey = 0;
            return CsrUiKeyDeleteFromString(string);
        }
        if ((key == CSR_UI_KEY_LEFT) || (key == CSR_UI_KEY_RIGHT))
        {
            /* Move cursor */
            instanceData->MKkey = 0;
            return TRUE;
        }
        if (isInputKey(key))
        {
            if (instanceData->MKkey == key)
            {
                instanceData->MKtid = CsrSchedTimerSet(MULTIKEY_DELAY, CsrUiKeyMultiKeyTimerEvent, 0, instanceData);
                CsrUiKeyDeleteFromString(string);
                instanceData->MKindex++;
                if (CsrUiKeyTranslateKey(keyMap, key, instanceData->MKindex) == 0)
                {
                    instanceData->MKindex = 0;
                }
                instanceData->MKkey = key;
            }
            else
            {
                instanceData->MKindex = 0;
                if (CsrUiKeyTranslateKey(keyMap, key, 1) != 0)
                {
                    instanceData->MKtid = CsrSchedTimerSet(MULTIKEY_DELAY, CsrUiKeyMultiKeyTimerEvent, 0, instanceData);
                    instanceData->MKkey = key;
                }
                else
                {
                    instanceData->MKkey = 0;
                }
            }
            return CsrUiKeyAddToString(string, stringLength, CsrUiKeyTranslateKey(keyMap, key, instanceData->MKindex), instanceData->MKcaps);
        }
    }

    return FALSE;
}

/*  Handles key presses - called from the controller
    Returns TRUE if the display should be redrawn and FALSE otherwise */
CsrBool CsrUiKeyHandler(CsrUiInstanceData *instanceData)              /* CSR_UI_INTERNAL_KEYDOWN_IND */
{
    CsrUiInternalKeydownInd *request = (CsrUiInternalKeydownInd *) instanceData->msg;
    DSitem *dsitem;
    void *uie;
    CsrUint16 key;

    key = request->key;
    dsitem = (DSitem *) instanceData->DisplayStack;

    /* Check if no UIE is displayed or input mode is BLOCK */
    if ((dsitem == NULL) || (dsitem->inputMode == CSR_UI_INPUTMODE_BLOCK))
    {
        /* Discard key event */
        return FALSE;
    }

    /* Check for AUTO behaviour and catch key events */
    if ((dsitem->inputMode == CSR_UI_INPUTMODE_AUTO) || (dsitem->inputMode == CSR_UI_INPUTMODE_AUTOPASS))
    {
        uie = CsrUiUieHandle2Pointer(instanceData, dsitem->handle);

        /* input mode is AUTO or AUTOPASS - processing depends on UIE type on top of Display Stack */
        switch (*((CsrUieType *) uie))
        {
            case CSR_UI_UIETYPE_MENU:
            {
                Menu *menu;
                menu = (Menu *) uie;

                /* Check if BACK was pressed */
                if ((dsitem->priority == CSR_UI_LOWESTPRIORITY) && (key == CSR_UI_KEY_BACK))
                {
                    /* If this is not the last item on the display stack, pop it */
                    if (dsitem->next != NULL)
                    {
                        /* pop and redraw */
                        CsrUiDisplayUieHide(instanceData, dsitem->handle);
                        return TRUE;
                    }
                    else
                    {
                        /* don't pop it and don't redraw */
                        return FALSE;
                    }
                }

                if (menu->currentItem)
                {
                    /* Catch SK1 SK2 BACK DEL */
                    if ((key == CSR_UI_KEY_SK1) && (menu->currentItem->actionSK1 != CSR_UI_DEFAULTACTION))
                    {
                        return CsrUiKeyExecuteAction(instanceData, menu->currentItem->actionSK1, menu->currentItem->key, dsitem->handle);
                    }
                    if ((key == CSR_UI_KEY_SK2) && (menu->currentItem->actionSK2 != CSR_UI_DEFAULTACTION))
                    {
                        return CsrUiKeyExecuteAction(instanceData, menu->currentItem->actionSK2, menu->currentItem->key, dsitem->handle);
                    }
                    if ((key == CSR_UI_KEY_BACK) && (menu->currentItem->actionBACK != CSR_UI_DEFAULTACTION))
                    {
                        return CsrUiKeyExecuteAction(instanceData, menu->currentItem->actionBACK, menu->currentItem->key, dsitem->handle);
                    }
                    if ((key == CSR_UI_KEY_DEL) && (menu->currentItem->actionDEL != CSR_UI_DEFAULTACTION))
                    {
                        return CsrUiKeyExecuteAction(instanceData, menu->currentItem->actionDEL, menu->currentItem->key, dsitem->handle);
                    }
                }
                if (key == CSR_UI_KEY_UP)
                {
                    if (menu->currentItem && menu->currentItem->prev)
                    {
                        menu->currentItem = menu->currentItem->prev;

                        if (instanceData->graphicsRenderer.active)
                        {
                            CsrUiRenderMenuScrollUp(menu);
                        }
                        if (instanceData->textRenderer.active)
                        {
                            CsrUiRenderTextMenuScrollUp(menu);
                        }
                        return TRUE;
                    }
                    return FALSE;
                }
                if (key == CSR_UI_KEY_DOWN)
                {
                    if (menu->currentItem && menu->currentItem->next)
                    {
                        menu->currentItem = menu->currentItem->next;

                        if (instanceData->graphicsRenderer.active)
                        {
                            CsrUiRenderMenuScrollDown(menu);
                        }
                        if (instanceData->textRenderer.active)
                        {
                            CsrUiRenderTextMenuScrollDown(menu);
                        }
                        return TRUE;
                    }
                    return FALSE;
                }
                break;
            }

            case CSR_UI_UIETYPE_DIALOG:
            {
                Dialog *dialog;
                dialog = (Dialog *) uie;

                /* Catch SK1 SK2 BACK DEL */
                if ((key == CSR_UI_KEY_SK1) && (dialog->actionSK1 != CSR_UI_DEFAULTACTION))
                {
                    return CsrUiKeyExecuteAction(instanceData, dialog->actionSK1, key, dsitem->handle);
                }
                if ((key == CSR_UI_KEY_SK2) && (dialog->actionSK2 != CSR_UI_DEFAULTACTION))
                {
                    return CsrUiKeyExecuteAction(instanceData, dialog->actionSK2, key, dsitem->handle);
                }
                if ((key == CSR_UI_KEY_BACK) && (dialog->actionBACK != CSR_UI_DEFAULTACTION))
                {
                    return CsrUiKeyExecuteAction(instanceData, dialog->actionBACK, key, dsitem->handle);
                }
                if ((key == CSR_UI_KEY_DEL) && (dialog->actionDEL != CSR_UI_DEFAULTACTION))
                {
                    return CsrUiKeyExecuteAction(instanceData, dialog->actionDEL, key, dsitem->handle);
                }
                if (key == CSR_UI_KEY_UP)
                {
                    CsrBool retval = FALSE;
                    if (instanceData->graphicsRenderer.active)
                    {
                        retval = CsrUiRenderDialogScrollUp(dialog);
                    }
                    if (instanceData->textRenderer.active)
                    {
                        retval = CsrUiRenderTextDialogScrollUp(dialog) || retval;
                    }
                    return retval;
                }
                if (key == CSR_UI_KEY_DOWN)
                {
                    CsrBool retval = FALSE;
                    if (instanceData->graphicsRenderer.active)
                    {
                        retval = CsrUiRenderDialogScrollDown(dialog);
                    }
                    if (instanceData->textRenderer.active)
                    {
                        retval = CsrUiRenderTextDialogScrollDown(dialog) || retval;
                    }
                    return retval;
                }
                break;
            }

            case CSR_UI_UIETYPE_INPUTDIALOG:
            {
                InputDialog *inputDialog;
                inputDialog = (InputDialog *) uie;

                /* Catch input keys, ASCII keys and DEL LEFT RIGHT */
                if ((isInputKey(key)) || isAsciiKey(key) || (key == CSR_UI_KEY_DEL) || (key == CSR_UI_KEY_LEFT) || (key == CSR_UI_KEY_RIGHT))
                {
                    /* Pass to multi key handler */
                    return CsrUiKeyMultiKeyHandler(instanceData, key, inputDialog->text, inputDialog->textLength, inputDialog->keyMap);
                }

                /* Catch SK1 SK2 BACK */
                if ((key == CSR_UI_KEY_SK1) && (inputDialog->actionSK1 != CSR_UI_DEFAULTACTION))
                {
                    return CsrUiKeyExecuteAction(instanceData, inputDialog->actionSK1, key, dsitem->handle);
                }
                if ((key == CSR_UI_KEY_SK2) && (inputDialog->actionSK2 != CSR_UI_DEFAULTACTION))
                {
                    return CsrUiKeyExecuteAction(instanceData, inputDialog->actionSK2, key, dsitem->handle);
                }
                if ((key == CSR_UI_KEY_BACK) && (inputDialog->actionBACK != CSR_UI_DEFAULTACTION))
                {
                    return CsrUiKeyExecuteAction(instanceData, inputDialog->actionBACK, key, dsitem->handle);
                }
                break;
            }

            case CSR_UI_UIETYPE_IDLESCREEN:
            {
                IdleScreen *idleScreen;
                idleScreen = (IdleScreen *) uie;

                /* Catch input keys, ASCII keys and DEL LEFT RIGHT */
                if ((isInputKey(key)) || isAsciiKey(key) || (key == CSR_UI_KEY_DEL) || (key == CSR_UI_KEY_LEFT) || (key == CSR_UI_KEY_RIGHT))
                {
                    /* Pass to multi key handler */
                    return CsrUiKeyMultiKeyHandler(instanceData, key, idleScreen->text, idleScreen->textLength, CSR_UI_KEYMAP_CONTROLNUMERIC);
                }

                /* Catch SK1 SK2 BACK */
                if ((key == CSR_UI_KEY_SK1) && (idleScreen->actionSK1 != CSR_UI_DEFAULTACTION))
                {
                    return CsrUiKeyExecuteAction(instanceData, idleScreen->actionSK1, key, dsitem->handle);
                }
                if ((key == CSR_UI_KEY_SK2) && (idleScreen->actionSK2 != CSR_UI_DEFAULTACTION))
                {
                    return CsrUiKeyExecuteAction(instanceData, idleScreen->actionSK2, key, dsitem->handle);
                }
                if ((key == CSR_UI_KEY_BACK) && (idleScreen->actionBACK != CSR_UI_DEFAULTACTION))
                {
                    return CsrUiKeyExecuteAction(instanceData, idleScreen->actionBACK, key, dsitem->handle);
                }
                break;
            }

            default:
            {
                CSR_UIEXCEPTION("Received key event while unknown UIE type is displayed");
                break;
            }
        }
    }

    /* If this point is reached no AUTO action was performed */
    /* Send key to listener if mode is PASS or AUTOPASS*/
    if ((dsitem->inputMode == CSR_UI_INPUTMODE_PASS) || (dsitem->inputMode == CSR_UI_INPUTMODE_AUTOPASS))
    {
        CsrUiKeydownInd *message;
        message = CsrPmemAlloc(sizeof(CsrUiKeydownInd));
        message->type = CSR_UI_KEYDOWN_IND;
        message->key = key;
        message->displayElement = dsitem->handle;

        CsrSchedMessagePut(dsitem->listener, CSR_UI_PRIM, message);
    }

    /* AUTO, AUTOPASS or PASS - no internal action performed, no redrawing needed */
    return FALSE;
}
