/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_ui_prim.h"
#include "csr_ui_ctrl.h"
#include "csr_ui_uiemanager.h"
#include "csr_ui_exception.h"
#include "csr_ui_renderer.h"
#include "csr_ui_renderer_text.h"

#define HEAP_INITIAL_SIZE       64
#define HEAP_GROW_SIZE          64
#define HEAP_SIZE_MAX        50000

/* Get the number of items in a menu */
CsrUint16 CsrUiUieMenuGetNumberOfItems(Menu *menu)
{
    MenuItem *pthis;
    CsrUint16 items;

    items = 0;
    for (pthis = menu->items; (pthis != NULL); pthis = pthis->next)
    {
        items++;
    }
    return items;
}

/* Get the cursor index of a menu */
CsrUint16 CsrUiUieMenuGetCursorIndex(Menu *menu)
{
    MenuItem *pthis;
    CsrUint16 cursor;

    cursor = 0;
    for (pthis = menu->items; (pthis != NULL) && (menu->currentItem != pthis); pthis = pthis->next)
    {
        cursor++;
    }
    return cursor;
}

/* Get the cursor position in a menu in terms of the key */
CsrUint16 CsrUiUieMenuGetCursorPosition(CsrUiInstanceData *instanceData, CsrUieHandle menu)
{
    void *uie;
    Menu *pmenu;

    uie = CsrUiUieHandle2Pointer(instanceData, menu);
    if (*((CsrUieType *) uie) != CSR_UI_UIETYPE_MENU)
    {
        CSR_UIEXCEPTION("Attempted to get cursor position from an invalid UIE type (not a Menu)");
    }
    pmenu = (Menu *) uie;

    /* Check for no cursor */
    if (pmenu->currentItem == NULL)
    {
        CSR_UIEXCEPTION("Attempted to get cursor position from an empty Menu");
    }

    return pmenu->currentItem->key;
}

/* Set the cursor position in a menu */
void CsrUiUieMenuSetCursorPosition(CsrUiInstanceData *instanceData, CsrUieHandle menu, CsrUint16 key)
{
    void *uie;
    Menu *pmenu;
    MenuItem *pthis;

    uie = CsrUiUieHandle2Pointer(instanceData, menu);
    if (*((CsrUieType *) uie) != CSR_UI_UIETYPE_MENU)
    {
        CSR_UIEXCEPTION("Attempted to set cursor position in an invalid UIE type (not a Menu)");
    }
    pmenu = (Menu *) uie;

    /* Check for empty Menu before proceeding */
    if (pmenu->items == NULL)
    {
        CSR_UIEXCEPTION("Attempted to set cursor position in an empty Menu");
    }

    /* loop from start of chain until key or end is reached */
    for (pthis = pmenu->items; (pthis->next != NULL) && (pthis->key != key); pthis = pthis->next)
    {
    }

    if (pthis->key == key)
    {
        /* Set cursor position */
        pmenu->currentItem = pthis;

        if (instanceData->graphicsRenderer.active)
        {
            CsrUiRenderMenuScrollUpdate(pmenu);
        }
        if (instanceData->textRenderer.active)
        {
            CsrUiRenderTextMenuScrollUpdate(pmenu);
        }
    }
    else
    {
        /* The key was not found */
        CSR_UIEXCEPTION("The specified key was not found in the Menu");
    }
}

/* Map CsrUieHandle to memory pointer */
void *CsrUiUieHandle2Pointer(CsrUiInstanceData *instanceData, CsrUieHandle handle)
{
    if ((handle == 0) || (handle > instanceData->HeapSize) || (instanceData->Heap[handle - 1] == NULL))
    {
        CSR_UIEXCEPTION("Referenced invalid CsrUieHandle");
    }
    return instanceData->Heap[handle - 1];
}

/* Create new UIE */
CsrUieHandle CsrUiUieCreate(CsrUiInstanceData *instanceData, CsrUieType type)
{
    CsrUint16 i;
    CsrUint16 freeHandle = 0;

    /* Create table if it does not exist */
    if (instanceData->Heap == NULL)
    {
        instanceData->Heap = CsrPmemZalloc(HEAP_INITIAL_SIZE * sizeof(void *));
        instanceData->HeapSize = HEAP_INITIAL_SIZE;
    }

    /* Search for free entry */
    for (i = 0; i < instanceData->HeapSize; i++)
    {
        if (instanceData->Heap[i] == NULL)
        {
            /* Found a free entry */
            freeHandle = i + 1;
            break;
        }
    }

    /* Check if table is full, and reallocate */
    if (freeHandle == 0)
    {
        if (instanceData->HeapSize + HEAP_GROW_SIZE <= HEAP_SIZE_MAX)
        {
            void **newTab;
            CsrUint32 c;
            newTab = CsrPmemZalloc((instanceData->HeapSize + HEAP_GROW_SIZE) * sizeof(void *));

            c = instanceData->HeapSize * sizeof(void *);
            while (c--)
            {
                ((CsrUint8 *) newTab)[c] = ((CsrUint8 *) instanceData->Heap)[c];
            }
            CsrPmemFree(instanceData->Heap);
            instanceData->Heap = newTab;
            freeHandle = instanceData->HeapSize + 1;
            instanceData->HeapSize += HEAP_GROW_SIZE;
        }
        else
        {
            CSR_UIEXCEPTION("Maximum number of CsrUieHandles reached");
        }
    }

    /* Create UIE on the real heap */
    switch (type)
    {
        case CSR_UI_UIETYPE_MENU:
        {
            Menu *uie = CsrPmemZalloc(sizeof(Menu));

            uie->type = CSR_UI_UIETYPE_MENU;
            uie->handle = freeHandle;
            instanceData->Heap[freeHandle - 1] = uie;
            break;
        }

        case CSR_UI_UIETYPE_EVENT:
        {
            Event *uie = CsrPmemZalloc(sizeof(Event));

            uie->type = CSR_UI_UIETYPE_EVENT;
            uie->handle = freeHandle;
            instanceData->Heap[freeHandle - 1] = uie;
            break;
        }

        case CSR_UI_UIETYPE_DIALOG:
        {
            Dialog *uie = CsrPmemZalloc(sizeof(Dialog));

            uie->type = CSR_UI_UIETYPE_DIALOG;
            uie->handle = freeHandle;
            instanceData->Heap[freeHandle - 1] = uie;
            break;
        }

        case CSR_UI_UIETYPE_INPUTDIALOG:
        {
            InputDialog *uie = CsrPmemZalloc(sizeof(InputDialog));

            uie->type = CSR_UI_UIETYPE_INPUTDIALOG;
            uie->handle = freeHandle;
            instanceData->Heap[freeHandle - 1] = uie;
            break;
        }

        case CSR_UI_UIETYPE_IDLESCREEN:
        {
            IdleScreen *uie = CsrPmemZalloc(sizeof(IdleScreen));

            uie->type = CSR_UI_UIETYPE_IDLESCREEN;
            uie->handle = freeHandle;
            instanceData->Heap[freeHandle - 1] = uie;
            break;
        }

        default:
        {
            CSR_UIEXCEPTION("Attempted to create undefined UIE type");
            break;
        }
    }

    return freeHandle;
}

/* Destroy existing UIE */
void CsrUiUieDestroy(CsrUiInstanceData *instanceData, CsrUieHandle handle)
{
    void *uie;
    uie = CsrUiUieHandle2Pointer(instanceData, handle);

    /* Catch UIE types that carry memory objects and deallocate those too */
    switch (*((CsrUieType *) uie))
    {
        case CSR_UI_UIETYPE_MENU:
        {
            Menu *menu;
            MenuItem *item;
            MenuItem *pthis;

            menu = (Menu *) uie;

            CsrPmemFree(menu->heading);
            CsrPmemFree(menu->textSK1);
            CsrPmemFree(menu->textSK2);

            for (item = ((Menu *) uie)->items; item != NULL; item = pthis)
            {
                pthis = item->next;
                CsrPmemFree(item->label);
                CsrPmemFree(item->sublabel);
                CsrPmemFree(item);
            }
            break;
        }

        case CSR_UI_UIETYPE_DIALOG:
        {
            Dialog *dialog;
            dialog = (Dialog *) uie;
            CsrPmemFree(dialog->heading);
            CsrPmemFree(dialog->message);
            CsrPmemFree(dialog->textSK1);
            CsrPmemFree(dialog->textSK2);
            break;
        }

        case CSR_UI_UIETYPE_INPUTDIALOG:
        {
            InputDialog *inputDialog;
            inputDialog = (InputDialog *) uie;
            CsrPmemFree(inputDialog->heading);
            CsrPmemFree(inputDialog->message);
            CsrPmemFree(inputDialog->text);
            CsrPmemFree(inputDialog->textSK1);
            CsrPmemFree(inputDialog->textSK2);
            break;
        }

        case CSR_UI_UIETYPE_IDLESCREEN:
        {
            IdleScreen *idleScreen;
            idleScreen = (IdleScreen *) uie;
            CsrPmemFree(idleScreen->heading);
            CsrPmemFree(idleScreen->text);
            CsrPmemFree(idleScreen->textSK1);
            CsrPmemFree(idleScreen->textSK2);
            break;
        }

        default:
        {
            break;
        }
    }

    instanceData->Heap[handle - 1] = NULL;
    CsrPmemFree(uie);
}

/* Destroy all existing UIEs and deallocate the UIE Heap itself */
void CsrUiUieDestroyAll(CsrUiInstanceData *instanceData)
{
    CsrUint16 i;
    for (i = 0; i < instanceData->HeapSize; i++)
    {
        if (instanceData->Heap[i] != NULL)
        {
            CsrUiUieDestroy(instanceData, (CsrUieHandle) (i + 1));
        }
    }
    CsrPmemFree(instanceData->Heap);
    instanceData->Heap = NULL;
    instanceData->HeapSize = 0;
}

/* Add a menu item to a Menu */
void CsrUiUieMenuAdditem(CsrUiInstanceData *instanceData, CsrUieHandle menu, CsrUint16 key, CsrUint16 icon, CsrUint16 *label, CsrUint16 *sublabel, CsrUieHandle actionSK1, CsrUieHandle actionSK2, CsrUieHandle actionBACK, CsrUieHandle actionDEL, CsrUint16 position)
{
    void *uie;
    Menu *pmenu;
    MenuItem *pitem;
    MenuItem *pthis;
    CsrUint16 i;

    uie = CsrUiUieHandle2Pointer(instanceData, menu);
    if (*((CsrUieType *) uie) != CSR_UI_UIETYPE_MENU)
    {
        CSR_UIEXCEPTION("Attempted to add a menu item to an invalid UIE type (not a Menu)");
    }
    pmenu = (Menu *) uie;

    if (key > 0x7FFF) /* Keyboard codes start at 0x8000 */
    {
        CSR_UIEXCEPTION("Key value must be lower than 0x8000");
    }

    pitem = CsrPmemZalloc(sizeof(MenuItem));
    pitem->key = key;
    pitem->icon = icon;
    pitem->label = label;
    pitem->sublabel = sublabel;
    pitem->actionSK1 = actionSK1;
    pitem->actionSK2 = actionSK2;
    pitem->actionBACK = actionBACK;
    pitem->actionDEL = actionDEL;
    pitem->parent = menu;

    /* No items in list */
    if (pmenu->items == NULL)
    {
        /* Set cursor position */
        pmenu->currentItem = pitem;
        pitem->next = NULL;
        pitem->prev = NULL;
        pmenu->items = pitem;
        return;
    }

    /* Insert at start of list */
    if (position == CSR_UI_FIRST)
    {
        pitem->next = pmenu->items;
        pitem->next->prev = pitem;
        pitem->prev = NULL;
        pmenu->items = pitem;
    }
    else
    {
        /* loop from element position 2 until end of chain or desired position reached */
        pthis = pmenu->items;
        for (i = 2; (pthis->next != NULL) && (i != position); i++)
        {
            pthis = pthis->next;
        }

        if (pthis->next != NULL)
        {
            /* Insert */
            pthis->next->prev = pitem;
            pitem->prev = pthis;
            pitem->next = pthis->next;
            pthis->next = pitem;
        }
        else
        {
            /* Add to chain */
            pthis->next = pitem;
            pitem->prev = pthis;
        }
    }

    if (instanceData->graphicsRenderer.active)
    {
        CsrUiRenderMenuScrollDown(pmenu);
    }
    if (instanceData->textRenderer.active)
    {
        CsrUiRenderTextMenuScrollDown(pmenu);
    }
}

/* Remove a menu item from a Menu */
void CsrUiUieMenuRemoveitem(CsrUiInstanceData *instanceData, CsrUieHandle menu, CsrUint16 key)
{
    void *uie;
    Menu *pmenu;
    MenuItem *pthis;
    MenuItem *pdelete;
    CsrUint16 c;

    uie = CsrUiUieHandle2Pointer(instanceData, menu);
    if (*((CsrUieType *) uie) != CSR_UI_UIETYPE_MENU)
    {
        CSR_UIEXCEPTION("Attempted to remove a menu item from an invalid UIE type (not a Menu)");
    }
    pmenu = (Menu *) uie;

    /* Check for empty Menu before proceeding */
    if (pmenu->items == NULL)
    {
        CSR_UIEXCEPTION("Attempted to remove a menu item from an empty Menu");
    }

    /* Check first element */
    if (pmenu->items->key == key)
    {
        if (pmenu->currentItem == pmenu->items)
        {
            /* Cursor points at the first element that is to be deleted */
            pmenu->currentItem = pmenu->items->next;
        }
        if (pmenu->items->next != NULL)
        {
            pmenu->items->next->prev = NULL;
        }
        pdelete = pmenu->items;
        pmenu->items = pmenu->items->next;

        /* Scroll up if item is outside visible area */
        if (instanceData->graphicsRenderer.active)
        {
            CsrUiRenderMenuScrollDelete(pmenu, 0);
        }
        if (instanceData->textRenderer.active)
        {
            CsrUiRenderTextMenuScrollDelete(pmenu, 0);
        }
    }
    else
    {
        /* loop from start of chain until key or end is reached */
        c = 0;
        for (pthis = pmenu->items; (pthis->next != NULL) && (pthis->key != key); pthis = pthis->next)
        {
            c++;
        }
        if (pthis->key == key)
        {
            if (pmenu->currentItem == pthis)
            {
                /* Deleting the current item */
                if (pthis->next != NULL)
                {
                    /* Move cursor to next item */
                    pmenu->currentItem = pthis->next;
                }
                else
                {
                    /* Move cursor to previous item */
                    pmenu->currentItem = pthis->prev;
                }
            }
            /* pthis contains the element to delete */
            pthis->prev->next = pthis->next;
            if (pthis->next != NULL)
            {
                pthis->next->prev = pthis->prev;
            }
            pdelete = pthis;

            /* Scroll up if item is outside visible area */
            if (instanceData->graphicsRenderer.active)
            {
                CsrUiRenderMenuScrollDelete(pmenu, c);
            }
            if (instanceData->textRenderer.active)
            {
                CsrUiRenderTextMenuScrollDelete(pmenu, c);
            }
        }
        else
        {
            /* The key was not found */
            pdelete = NULL;
        }
    }

    if (pdelete != NULL)
    {
        CsrPmemFree(pdelete->sublabel);
        CsrPmemFree(pdelete->label);
        CsrPmemFree(pdelete);
    }
    else
    {
        CSR_UIEXCEPTION("The specified key was not found in the Menu");
    }

    /* Update scroll if cursor has moved to top of window */
    if (instanceData->graphicsRenderer.active)
    {
        CsrUiRenderMenuScrollUp(pmenu);
    }
    if (instanceData->textRenderer.active)
    {
        CsrUiRenderTextMenuScrollUp(pmenu);
    }
}

/* Remove all menu items from a menu */
void CsrUiUieMenuRemoveallitems(CsrUiInstanceData *instanceData, CsrUieHandle menu)
{
    void *uie;
    Menu *pmenu;
    MenuItem *pthis;
    MenuItem *pnext;

    uie = CsrUiUieHandle2Pointer(instanceData, menu);
    if (*((CsrUieType *) uie) != CSR_UI_UIETYPE_MENU)
    {
        CSR_UIEXCEPTION("Attempted to remove all menu items from an invalid UIE type (not a Menu)");
    }
    pmenu = (Menu *) uie;

    for (pthis = pmenu->items; pthis != NULL; pthis = pnext)
    {
        CsrPmemFree(pthis->sublabel);
        CsrPmemFree(pthis->label);
        pnext = pthis->next;
        CsrPmemFree(pthis);
    }
    pmenu->items = NULL;
    pmenu->currentItem = NULL;
    pmenu->scrollGUI = pmenu->scrollTUI = 0;
}

/* Set a menu item in a Menu */
void CsrUiUieMenuSetitem(CsrUiInstanceData *instanceData, CsrUieHandle menu, CsrUint16 key, CsrUint16 icon, CsrUint16 *label, CsrUint16 *sublabel, CsrUieHandle actionSK1, CsrUieHandle actionSK2, CsrUieHandle actionBACK, CsrUieHandle actionDEL)
{
    void *uie;
    Menu *pmenu;
    MenuItem *pthis;

    uie = CsrUiUieHandle2Pointer(instanceData, menu);
    if (*((CsrUieType *) uie) != CSR_UI_UIETYPE_MENU)
    {
        CSR_UIEXCEPTION("Attempted to set a menu item in an invalid UIE type (not a Menu)");
    }
    pmenu = (Menu *) uie;

    /* Check for empty Menu before proceeding */
    if (pmenu->items == NULL)
    {
        CSR_UIEXCEPTION("Attempted to set a menu item in an empty Menu");
    }

    /* loop from start of chain until key or end is reached */
    for (pthis = pmenu->items; (pthis->next != NULL) && (pthis->key != key); pthis = pthis->next)
    {
    }

    if (pthis->key == key)
    {
        /* The key was found */
        CsrPmemFree(pthis->label);
        pthis->label = label;
        CsrPmemFree(pthis->sublabel);
        pthis->sublabel = sublabel;
        pthis->icon = icon;
        pthis->actionSK1 = actionSK1;
        pthis->actionSK2 = actionSK2;
        pthis->actionBACK = actionBACK;
        pthis->actionDEL = actionDEL;
    }
    else
    {
        /* The key was not found */
        CSR_UIEXCEPTION("The specified key was not found in the Menu");
    }
}

/* Get a menu item from a Menu */
MenuItem *CsrUiUieMenuGetitem(CsrUiInstanceData *instanceData, CsrUieHandle menu, CsrUint16 key)
{
    void *uie;
    Menu *pmenu;
    MenuItem *pthis;

    uie = CsrUiUieHandle2Pointer(instanceData, menu);
    if (*((CsrUieType *) uie) != CSR_UI_UIETYPE_MENU)
    {
        CSR_UIEXCEPTION("Attempted to get a menu item from an invalid UIE type (not a Menu)");
    }
    pmenu = (Menu *) uie;

    /* Check for empty Menu before proceeding */
    if (pmenu->items == NULL)
    {
        CSR_UIEXCEPTION("Attempted to get a menu item from an empty Menu");
    }

    /* loop from start of chain until key or end is reached */
    for (pthis = pmenu->items; (pthis->next != NULL) && (pthis->key != key); pthis = pthis->next)
    {
    }

    if (pthis->key != key)
    {
        /* The key was not found */
        CSR_UIEXCEPTION("The specified key was not found in the Menu");
    }

    return pthis;
}
