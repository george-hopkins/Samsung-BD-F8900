/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_app_prim.h"
#include "csr_app_util.h"
#include "csr_app_ui_sef.h"
#include "csr_pmem.h"
#include "csr_app_task.h"
#include "csr_app_ui_strings.h"
#include "csr_app_prim.h"

void CsrAppRestoreSavedMessages(CsrAppInstData *inst)
{
    inst->initialized = TRUE;
    if (inst->saveQueue) /* A downstream messages is received doing initilization
         of the Bluetooth Manager. Make sure that the messages
         is restored                                                        */
    {
        inst->restoreFlag = TRUE;
        CsrSchedMessagePut(CSR_APP_IFACEQUEUE, CSR_APP_PRIM, NULL);
    }
    else /* No downstream messages is received doing initilization
         of the app. Ignore the message                       */
    {
    }
}

CsrSchedQid CsrAppGetProfileEntryFromKey(CsrAppInstData *inst, CsrUint32 key)
{
    CsrAppProfileUie *entry = inst->profileHandleList;

    while (entry != NULL)
    {
        if (entry->key == key)
        {
            return entry->phandle;
        }
        entry = entry->next;
    }

    return CSR_SCHED_QID_INVALID;
}

void CsrAppInsertDisplayMenuHandle(CsrAppInstData *inst, CsrUieHandle displayMenuHandle)
{
    CsrAppProfileUie *entry = inst->profileHandleList;
    CsrUtf8String *temp = NULL;

    while (entry != NULL)
    {
        if (entry->displayMenuHandle == 0)
        {
            if (temp == NULL)
            {
                CsrUiMenuSetReqSend(displayMenuHandle, CONVERT_TEXT_STRING_2_UTF16(entry->menuName), TEXT_SELECT_UTF16, NULL);
                CsrUiMenuAdditemReqSend(inst->hMainMenu, CSR_UI_FIRST,
                    inst->mainMenuEntries, entry->menuIcon,
                    CONVERT_TEXT_STRING_2_UTF16(entry->menuName), NULL,
                    displayMenuHandle, CSR_UI_DEFAULTACTION, inst->hBackEvent, CSR_UI_DEFAULTACTION);

                temp = entry->menuName;
            }

            if (CsrStrCmp((CsrCharString *) temp, (CsrCharString *) entry->menuName) == 0)
            {
                entry->displayMenuHandle = displayMenuHandle;
                CsrUiMenuAdditemReqSend(displayMenuHandle, (CsrUint16) (entry->top ? CSR_UI_FIRST : CSR_UI_LAST),
                    entry->key, entry->subMenuIcon,
                    CONVERT_TEXT_STRING_2_UTF16(entry->subMenuName), NULL,
                    inst->hTakeControlEvent, CSR_UI_DEFAULTACTION,
                    inst->hBackEvent, CSR_UI_DEFAULTACTION);
            }
        }
        entry = entry->next;
    }
}

CsrAppProfileUie *CsrAppInsertPopupDialogHandle(CsrAppInstData *inst, CsrUieHandle popupDialogHandle)
{
    CsrAppProfileUie *entry = inst->profileHandleList;

    while (entry != NULL)
    {
        if (entry->popupDialogHandle == 0)
        {
            entry->popupDialogHandle = popupDialogHandle;
            return entry;
        }
        entry = entry->next;
    }

    return NULL;
}

CsrAppProfileUie *CsrAppGetProfileUieFromMenuName(CsrAppInstData *inst, CsrUtf8String *menuName, CsrUtf8String *subMenuName)
{
    CsrAppProfileUie *entry = inst->profileHandleList;

    while (entry != NULL)
    {
        if ((CsrStrCmp((CsrCharString *) entry->menuName, (CsrCharString *) menuName) == 0) &&
            (CsrStrCmp((CsrCharString *) entry->subMenuName, (CsrCharString *) subMenuName) == 0))
        {
            return entry;
        }
        entry = entry->next;
    }
    return NULL;
}

static CsrBool csrAppGetDisplayMenuHandleFromMenuName(CsrAppInstData *inst, CsrUtf8String *menuName, CsrUieHandle *displayMenuHandle)
{
    CsrAppProfileUie *entry = inst->profileHandleList;
    CsrBool startUieHandleCreationCycle = TRUE;

    *displayMenuHandle = 0;

    while (entry != NULL)
    {
        if (CsrStrCmp((CsrCharString *) entry->menuName, (CsrCharString *) menuName) == 0)
        {
            *displayMenuHandle = entry->displayMenuHandle;

            /* UieCreationCycle already exectued or in progress so don't start over */
            startUieHandleCreationCycle = FALSE;
        }
        entry = entry->next;
    }

    return startUieHandleCreationCycle;
}

static CsrBool csrAppInsertProfileEntryInList(CsrAppInstData *inst, CsrSchedQid phandle, CsrUint16 key, CsrUtf8String *menuName, CsrUtf8String *subMenuName, CsrUint16 menuIcon, CsrUint16 subMenuIcon, CsrBool top)
{
    CsrAppProfileUie *entry = CsrPmemAlloc(sizeof(CsrAppProfileUie));
    CsrBool startUieHandleCreationCycle = FALSE;

    entry->key = key;
    entry->phandle = phandle;
    entry->menuName = (CsrUtf8String *) CsrStrDup((CsrCharString *) menuName);
    entry->subMenuName = (CsrUtf8String *) CsrStrDup((CsrCharString *) subMenuName);
    entry->menuIcon = menuIcon;
    entry->subMenuIcon = subMenuIcon;
    entry->top = top;
    startUieHandleCreationCycle = csrAppGetDisplayMenuHandleFromMenuName(inst, menuName, &entry->displayMenuHandle);
    entry->popupDialogHandle = 0;
    entry->next = inst->profileHandleList;

    inst->profileHandleList = entry;

    return startUieHandleCreationCycle;
}

void CsrAppRegisterProfile(CsrAppInstData *inst, CsrSchedQid phandle, CsrUtf8String *menuName, CsrUtf8String *subMenuName, CsrUint16 menuIcon, CsrUint16 subMenuIcon, CsrBool top)
{
    inst->mainMenuEntries++;

    if (!csrAppInsertProfileEntryInList(inst, phandle, inst->mainMenuEntries, menuName, subMenuName, menuIcon, subMenuIcon, top))
    {
        if (inst->profileHandleList->displayMenuHandle != 0)
        {
            CsrUiMenuAdditemReqSend(inst->profileHandleList->displayMenuHandle,
                (CsrUint16) (top ? CSR_UI_FIRST : CSR_UI_LAST),
                inst->mainMenuEntries, subMenuIcon,
                CONVERT_TEXT_STRING_2_UTF16(subMenuName), NULL,
                inst->hTakeControlEvent, CSR_UI_DEFAULTACTION,
                inst->hBackEvent, CSR_UI_DEFAULTACTION);
        }
        else /* Do nothing the submenu will be printed once the new submenu display handle has been created */
        {
        }
    }
    else
    {
        /* Create the sub menu */
        CsrUiUieCreateReqSend(CSR_APP_IFACEQUEUE, CSR_UI_UIETYPE_MENU);
    }

    /* Create the popup dialog */
    CsrUiUieCreateReqSend(CSR_APP_IFACEQUEUE, CSR_UI_UIETYPE_DIALOG);
}
