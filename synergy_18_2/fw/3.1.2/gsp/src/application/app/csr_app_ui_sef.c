/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_app_ui_sef.h"
#include "csr_app_task.h"
#include "csr_app_util.h"
#include "csr_ui_prim.h"
#include "csr_pmem.h"
#include "csr_app_lower_private.h"
#include "csr_app_ui_strings.h"
#include "csr_app_prim.h"
#include "csr_app_handler.h"
#include "csr_log_text_2.h"

/* CSR_UI upstream handler functions                                                */
void CsrAppCsrUiUieCreateCfmHandler(CsrAppInstData *instanceData)
{
    CsrUiUieCreateCfm *confirm = (CsrUiUieCreateCfm *) instanceData->recvMsgP;
    CsrBool uiCreationCycleDone = FALSE;

    if (instanceData->initialized)
    {
        switch (confirm->elementType)
        {
            case CSR_UI_UIETYPE_MENU:
            {
                CsrAppInsertDisplayMenuHandle(instanceData, confirm->handle);
                break;
            }
            case CSR_UI_UIETYPE_DIALOG:
            {
                CsrAppProfileUie *entry;

                /* Save the handle to the popup dialog */
                entry = CsrAppInsertPopupDialogHandle(instanceData, confirm->handle);

                if (entry != NULL)
                {
                    /* Send the CSR_APP_REGISTER_CFM message */
                    CsrAppRegisterCfm *registerConfirm = CsrPmemAlloc(sizeof(CsrAppRegisterCfm));
                    registerConfirm->type = CSR_APP_REGISTER_CFM;
                    registerConfirm->controlId = entry->key;
                    CsrSchedMessagePut(entry->phandle, CSR_APP_PRIM, registerConfirm);
                }
                break;
            }
            default:
                break;
        }
        return;
    }

    switch (confirm->elementType)
    {
        case CSR_UI_UIETYPE_EVENT:
        {
            CsrBool lastEventCreated = FALSE;

            if (!instanceData->hExitEvent)
            {
                instanceData->hExitEvent = confirm->handle;
                CsrUiEventSetReqSend(confirm->handle, CSR_UI_INPUTMODE_BLOCK, CSR_APP_IFACEQUEUE);
            }
            else if (!instanceData->hBackEvent)
            {
                instanceData->hBackEvent = confirm->handle;
                CsrUiEventSetReqSend(confirm->handle, CSR_UI_INPUTMODE_BLOCK, CSR_APP_IFACEQUEUE);
            }
            else if (!instanceData->hTakeControlEvent)
            {
                instanceData->hTakeControlEvent = confirm->handle;
                CsrUiEventSetReqSend(confirm->handle, CSR_UI_INPUTMODE_AUTO, CSR_APP_IFACEQUEUE);
            }
            else if (!instanceData->hSelectEvent)
            {
                instanceData->hSelectEvent = confirm->handle;
                CsrUiEventSetReqSend(confirm->handle, CSR_UI_INPUTMODE_AUTO, CSR_APP_IFACEQUEUE);
                lastEventCreated = TRUE;
            }

            if (!lastEventCreated)
            {
                CsrUiUieCreateReqSend(CSR_APP_IFACEQUEUE, CSR_UI_UIETYPE_EVENT);
            }
            else
            {
                CsrUiUieCreateReqSend(CSR_APP_IFACEQUEUE, CSR_UI_UIETYPE_DIALOG);
            }
            break;
        }
        case CSR_UI_UIETYPE_DIALOG:
        {
            CsrBool lastDialogCreated = FALSE;

            if (!instanceData->hBacklogDialog)
            {
                instanceData->hBacklogDialog = confirm->handle;
                lastDialogCreated = TRUE;
            }

            if (!lastDialogCreated)
            {
                CsrUiUieCreateReqSend(CSR_APP_IFACEQUEUE, CSR_UI_UIETYPE_DIALOG);
            }
            else
            {
                CsrUiUieCreateReqSend(CSR_APP_IFACEQUEUE, CSR_UI_UIETYPE_MENU);
            }
            break;
        }
        case CSR_UI_UIETYPE_MENU:
        {
            CsrBool lastMenuCreated = FALSE;

            if (!instanceData->hBacklogMenu)
            {
                instanceData->hBacklogMenu = confirm->handle;
                CsrUiMenuSetReqSend(confirm->handle, TEXT_BACKLOG_UTF16, TEXT_SELECT_UTF16, NULL);
            }
            else if (!instanceData->hMainMenu)
            {
                instanceData->hMainMenu = confirm->handle;
                CsrUiMenuSetReqSend(confirm->handle, TEXT_MAIN_MENU_UTF16, TEXT_SELECT_UTF16, NULL);
                CsrUiMenuAdditemReqSend(confirm->handle, CSR_UI_LAST, 0, CSR_UI_ICON_LETTER,
                    TEXT_BACKLOG_UTF16, NULL, instanceData->hBacklogMenu,
                    CSR_UI_DEFAULTACTION, CSR_UI_DEFAULTACTION, CSR_UI_DEFAULTACTION);
                CsrUiMenuAdditemReqSend(confirm->handle, CSR_UI_LAST, 1, CSR_UI_ICON_CROSS,
                    TEXT_EXIT_UTF16, NULL, instanceData->hExitEvent,
                    CSR_UI_DEFAULTACTION, CSR_UI_DEFAULTACTION, CSR_UI_DEFAULTACTION);
                lastMenuCreated = TRUE;
            }

            if (!lastMenuCreated)
            {
                CsrUiUieCreateReqSend(CSR_APP_IFACEQUEUE, CSR_UI_UIETYPE_MENU);
            }
            else
            {
                CsrUiUieCreateReqSend(CSR_APP_IFACEQUEUE, CSR_UI_UIETYPE_IDLESCREEN);
            }
            break;
        }
        case CSR_UI_UIETYPE_IDLESCREEN:
        {
            if (!instanceData->hIdleScreen)
            {
                instanceData->hIdleScreen = confirm->handle;
                CsrUiIdlescreenSetReqSend(confirm->handle, TEXT_CSR_DEMO_APPLICATION_UTF16, NULL, 0,
                    TEXT_MAIN_MENU_UTF16, NULL, instanceData->hMainMenu,
                    CSR_UI_DEFAULTACTION, CSR_UI_DEFAULTACTION, CSR_UI_DEFAULTACTION);
                uiCreationCycleDone = TRUE;
            }
            break;
        }
        default:
        {
            CSR_LOG_TEXT_WARNING((CsrAppLto, 0,
                                  "Unexpected CsrAppCsrUiUieCreateCfmHandler is received"));
            break;
        }
    }

    if (uiCreationCycleDone)
    {
        CsrUiUieShowReqSend(instanceData->hIdleScreen, CSR_APP_IFACEQUEUE, CSR_UI_INPUTMODE_AUTO, CSR_UI_LOWESTPRIORITY);
        CsrAppRestoreSavedMessages(instanceData);
    }
}

void CsrAppCsrUiMenuGetitemCfmHandler(CsrAppInstData *instanceData)
{
    CsrUiMenuGetitemCfm *confirm = (CsrUiMenuGetitemCfm *) instanceData->recvMsgP;
    if (confirm->handle == instanceData->hBacklogMenu)
    {
        CsrUiDialogSetReqSend(instanceData->hBacklogDialog, confirm->label, confirm->sublabel,
            TEXT_OK_UTF16, NULL,
            instanceData->hBackEvent,
            CSR_UI_DEFAULTACTION,
            instanceData->hBackEvent,
            CSR_UI_DEFAULTACTION);
        CsrUiUieShowReqSend(instanceData->hBacklogDialog, CSR_APP_IFACEQUEUE, CSR_UI_INPUTMODE_AUTO, 0);
    }
}

void CsrAppCsrUiEventIndHandler(CsrAppInstData *inst)
{
    CsrUiEventInd *indication = (CsrUiEventInd *) inst->recvMsgP;

    if (indication->event == inst->hExitEvent)
    {
        if (CsrAppLowerPrivateExitFunction != NULL)
        {
            CsrAppLowerPrivateExitFunction();
        }
    }
    else if (indication->event == inst->hBackEvent)
    {
        CsrUiUieHideReqSend(indication->displayElement);
    }
    else if (indication->event == inst->hTakeControlEvent)
    {
        CsrSchedQid phandle = CsrAppGetProfileEntryFromKey(inst, indication->key);

        if (phandle != CSR_SCHED_QID_INVALID)
        {
            CsrAppTakeControlInd *takeControlIndicaiton = CsrPmemAlloc(sizeof(CsrAppTakeControlInd));

            takeControlIndicaiton->type = CSR_APP_TAKE_CONTROL_IND;
            takeControlIndicaiton->controlId = indication->key;

            CsrSchedMessagePut(phandle, CSR_APP_PRIM, takeControlIndicaiton);
        }
    }
    else if (indication->event == inst->hSelectEvent)
    {
        if (indication->displayElement == inst->hBacklogMenu)
        {
            CsrUiMenuGetitemReqSend(CSR_APP_IFACEQUEUE, inst->hBacklogMenu, indication->key);
        }
    }
}
