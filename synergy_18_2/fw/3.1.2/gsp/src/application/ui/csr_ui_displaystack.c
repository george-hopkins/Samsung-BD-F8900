/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_sched.h"
#include "csr_ui_displaystack.h"
#include "csr_ui_ctrl.h"
#include "csr_ui_exception.h"
#include "csr_ui_uiemanager.h"
#include "csr_ui_keyhandler.h"

/* Show a UIE, returns TRUE if the display should be redrawn */
CsrBool CsrUiDisplayUieShow(CsrUiInstanceData *instanceData, CsrUieHandle handle, CsrUint16 priority, CsrUint16 inputMode, CsrSchedQid listener)
{
    DSitem *newItem;
    DSitem *pthis;
    CsrBool redraw;

    /* Check if this element is already on the Display Stack */
    redraw = FALSE;
    for (pthis = instanceData->DisplayStack; pthis != NULL; pthis = pthis->next)
    {
        if (pthis->handle == handle)
        {
            /* Hide it, and save the redraw info (TRUE if top element was hidden) */
            redraw = CsrUiDisplayUieHide(instanceData, handle);
            break;
        }
    }

    /* Allocate memory for new DSitem */
    newItem = CsrPmemAlloc(sizeof(DSitem));

    /* Fill out fields in new DSitem */
    newItem->handle = handle;
    newItem->priority = priority;
    newItem->inputMode = inputMode;
    newItem->listener = listener;

    /* Reset cursor position of menu to top position */
    {
        void *uie;

        uie = CsrUiUieHandle2Pointer(instanceData, handle);
        if (*((CsrUieType *) uie) == CSR_UI_UIETYPE_MENU)
        {
            Menu *menu;
            menu = (Menu *) uie;
            menu->scrollGUI = menu->scrollTUI = 0;
            menu->currentItem = menu->items;
        }
    }

    /* Check if DSitem can be pushed onto the stack */
    if ((((DSitem *) instanceData->DisplayStack) == NULL) || (((DSitem *) instanceData->DisplayStack)->priority <= priority))
    {
        newItem->next = instanceData->DisplayStack;
        instanceData->DisplayStack = newItem;
        CsrUiKeyMultiKeyReset(instanceData);
        return TRUE;
    }

    /* Dive into the Display Stack until priorities allow insertion */
    for (pthis = instanceData->DisplayStack; pthis->next != NULL && (priority < pthis->next->priority); pthis = pthis->next)
    {
    }
    newItem->next = pthis->next;
    pthis->next = newItem;
    return redraw;
}

/* Hide a UIE, returns TRUE if the display should be redrawn */
CsrBool CsrUiDisplayUieHide(CsrUiInstanceData *instanceData, CsrUieHandle handle)
{
    DSitem *pthis;
    DSitem *pnext;

    if (instanceData->DisplayStack == NULL)
    {
        return FALSE;
    }

    /* Check if the currently shown item should be hidden */
    if (((DSitem *) instanceData->DisplayStack)->handle == handle)
    {
        pnext = ((DSitem *) instanceData->DisplayStack)->next;
        CsrPmemFree(instanceData->DisplayStack);
        instanceData->DisplayStack = pnext;
        CsrUiKeyMultiKeyReset(instanceData);
        return TRUE;
    }

    /* Search through the Display Stack until a referencing object is found */
    for (pthis = instanceData->DisplayStack; (pthis->next != NULL) && (pthis->next->handle != handle); pthis = pthis->next)
    {
    }

    /* Remove the element after pthis from the linked list */
    if (pthis->next != NULL)
    {
        pnext = pthis->next->next;
        CsrPmemFree(pthis->next);
        pthis->next = pnext;
    }
    return FALSE;
}

void CsrUiDisplaySetinputmode(CsrUiInstanceData *instanceData, CsrUieHandle handle, CsrUint16 inputMode)
{
    DSitem *pthis;

    /* Search the Display Stack for the specified handle */
    for (pthis = instanceData->DisplayStack; (pthis != NULL) && (pthis->handle != handle); pthis = pthis->next)
    {
    }

    if (pthis != NULL)
    {
        pthis->inputMode = inputMode;
    }
    else
    {
        CSR_UIEXCEPTION("The specified UIE is not referenced by the Display Stack and the input mode cannot be modified");
    }
}

/* Return the CsrUieHandle of the currently displayed UIE */
CsrUieHandle CsrUiDisplayGethandle(CsrUiInstanceData *instanceData)
{
    if (instanceData->DisplayStack)
    {
        return ((DSitem *) instanceData->DisplayStack)->handle;
    }
    else
    {
        return 0;
    }
}

/* Completely wipe out all DSitems on the Display Stack */
void CsrUiDisplayDestroyAll(CsrUiInstanceData *instanceData)
{
    DSitem *pthis;
    DSitem *pnext;

    for (pthis = instanceData->DisplayStack; pthis != NULL; pthis = pnext)
    {
        pnext = pthis->next;
        CsrPmemFree(pthis);
    }
    instanceData->DisplayStack = NULL;
}
