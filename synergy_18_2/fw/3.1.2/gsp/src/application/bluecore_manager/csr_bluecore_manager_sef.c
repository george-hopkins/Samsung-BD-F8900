/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_sched.h"
#include "csr_util.h"
#include "csr_log_text.h"

/* Transport Manager (TM) Interface */
#include "csr_tm_bluecore_task.h"
#include "csr_tm_bluecore_prim.h"
#include "csr_tm_bluecore_lib.h"

/* CSR_UI Interface */
#include "csr_ui_task.h"
#include "csr_ui_prim.h"
#include "csr_ui_lib.h"

/* CSR_APP Interface */
#include "csr_app_task.h"
#include "csr_app_prim.h"
#include "csr_app_lib.h"

/* Bluecore Manager Headers */
#include "csr_bluecore_manager_task.h"
#include "csr_bluecore_manager_handler.h"
#include "csr_bluecore_manager_sef.h"

#define MENU_PRIORITY 0
#define POPUP_PRIORITY 1

#define MENU_ITEM_ACTIVATE_ACTIVE_ICON CSR_UI_ICON_GEAR
#define MENU_ITEM_ACTIVATE_ACTIVATING_ICON CSR_UI_ICON_HOURGLASS
#define MENU_ITEM_ACTIVATE_DEACTIVATING_ICON CSR_UI_ICON_HOURGLASS
#define MENU_ITEM_ACTIVATE_INACTIVE_ICON CSR_UI_ICON_GEAR

#define MENU_ITEM_ACTIVATE_INACTIVE_TEXT "Activate"
#define MENU_ITEM_ACTIVATE_ACTIVATING_TEXT "Activate"
#define MENU_ITEM_ACTIVATE_DEACTIVATING_TEXT "Deactivate"
#define MENU_ITEM_ACTIVATE_ACTIVE_TEXT "Deactivate"

#define MENU_ITEM_ACTIVATE_ACTIVE_SUBTEXT "Active"
#define MENU_ITEM_ACTIVATE_ACTIVATING_SUBTEXT "Activating"
#define MENU_ITEM_ACTIVATE_DEACTIVATING_SUBTEXT "Deactivating"
#define MENU_ITEM_ACTIVATE_INACTIVE_SUBTEXT "Inactive"

#define MENU_ITEM_ACTIVATE_KEY 0

#define POPUP_MAX_MESSAGE_LENGTH 256

static void eventNotification(CsrBluecoreManagerInstanceData *instanceData, CsrBool backlog, CsrBool popup, const CsrCharString *fmt, ...)
{
    CsrCharString message[POPUP_MAX_MESSAGE_LENGTH];
    va_list args;

    va_start(args, fmt);
    CsrVsnprintf(message, POPUP_MAX_MESSAGE_LENGTH, fmt, args);
    va_end(args);

    if (backlog)
    {
        CsrAppBacklogReqSend2((CsrUtf8String *) CsrStrDup(CSR_BLUECORE_MANAGER_MENU_CATEGORY),
            (CsrUtf8String *) CsrStrDup(CSR_BLUECORE_MANAGER_MENU_ITEM_NAME), FALSE, TRUE, message);
    }
    if (popup)
    {
        CsrUiDialogSetReqSend(instanceData->hPopup,
            CsrUtf82Utf16String((CsrUtf8String *) CSR_BLUECORE_MANAGER_MENU_CATEGORY " " CSR_BLUECORE_MANAGER_MENU_ITEM_NAME),
            CsrUtf82Utf16String((CsrUtf8String *) message),
            CsrUtf82Utf16String((CsrUtf8String *) "OK"),
            NULL,
            instanceData->hBackEvent,
            CSR_UI_DEFAULTACTION,
            instanceData->hBackEvent,
            CSR_UI_DEFAULTACTION);
        CsrUiUieShowReqSend(instanceData->hPopup, CSR_BLUECORE_MANAGER_IFACEQUEUE, CSR_UI_INPUTMODE_AUTO, POPUP_PRIORITY);
    }
}

void CsrBluecoreManagerTmBluecoreActivateTransportCfmHandler(CsrBluecoreManagerInstanceData *instanceData)
{
    CsrTmBluecoreActivateTransportCfm *confirm = instanceData->message;

    if (confirm->result == CSR_RESULT_SUCCESS)
    {
        instanceData->state = CSR_BLUECORE_MANAGER_STATE_ACTIVE;
        CsrUiStatusBluetoothSetReqSend(TRUE);
        CsrUiMenuSetitemReqSend(instanceData->hMenu,
            MENU_ITEM_ACTIVATE_KEY, MENU_ITEM_ACTIVATE_ACTIVE_ICON,
            CsrUtf82Utf16String((CsrUtf8String *) MENU_ITEM_ACTIVATE_ACTIVE_TEXT),
            CsrUtf82Utf16String((CsrUtf8String *) MENU_ITEM_ACTIVATE_ACTIVE_SUBTEXT),
            instanceData->hSelectEvent, CSR_UI_DEFAULTACTION,
            instanceData->hBackEvent, CSR_UI_DEFAULTACTION);
        eventNotification(instanceData, TRUE, FALSE, "Successfully activated BlueCore transport");
    }
    else
    {
        instanceData->state = CSR_BLUECORE_MANAGER_STATE_INACTIVE;
        CsrUiMenuSetitemReqSend(instanceData->hMenu,
            MENU_ITEM_ACTIVATE_KEY, MENU_ITEM_ACTIVATE_INACTIVE_ICON,
            CsrUtf82Utf16String((CsrUtf8String *) MENU_ITEM_ACTIVATE_INACTIVE_TEXT),
            CsrUtf82Utf16String((CsrUtf8String *) MENU_ITEM_ACTIVATE_INACTIVE_SUBTEXT),
            instanceData->hSelectEvent, CSR_UI_DEFAULTACTION,
            instanceData->hBackEvent, CSR_UI_DEFAULTACTION);
        eventNotification(instanceData, TRUE, TRUE, "Failed to activate BlueCore transport");
    }
}

void CsrBluecoreManagerTmBluecoreDeactivateTransportCfmHandler(CsrBluecoreManagerInstanceData *instanceData)
{
    instanceData->state = CSR_BLUECORE_MANAGER_STATE_INACTIVE;
    CsrUiStatusBluetoothSetReqSend(FALSE);
    CsrUiMenuSetitemReqSend(instanceData->hMenu,
        MENU_ITEM_ACTIVATE_KEY, MENU_ITEM_ACTIVATE_INACTIVE_ICON,
        CsrUtf82Utf16String((CsrUtf8String *) MENU_ITEM_ACTIVATE_INACTIVE_TEXT),
        CsrUtf82Utf16String((CsrUtf8String *) MENU_ITEM_ACTIVATE_INACTIVE_SUBTEXT),
        instanceData->hSelectEvent, CSR_UI_DEFAULTACTION,
        instanceData->hBackEvent, CSR_UI_DEFAULTACTION);
    eventNotification(instanceData, TRUE, FALSE, "Deactivated BlueCore transport");
}

void CsrBluecoreManagerTmBluecoreDeactivateTransportIndHandler(CsrBluecoreManagerInstanceData *instanceData)
{
    if (instanceData->state == CSR_BLUECORE_MANAGER_STATE_ACTIVE)
    {
        instanceData->state = CSR_BLUECORE_MANAGER_STATE_DEACTIVATING;
        CsrTmBlueCoreDeactivateTransportReqSend(CSR_BLUECORE_MANAGER_IFACEQUEUE);
        CsrUiMenuSetitemReqSend(instanceData->hMenu,
            MENU_ITEM_ACTIVATE_KEY, MENU_ITEM_ACTIVATE_DEACTIVATING_ICON,
            CsrUtf82Utf16String((CsrUtf8String *) MENU_ITEM_ACTIVATE_DEACTIVATING_TEXT),
            CsrUtf82Utf16String((CsrUtf8String *) MENU_ITEM_ACTIVATE_DEACTIVATING_SUBTEXT),
            instanceData->hSelectEvent, CSR_UI_DEFAULTACTION,
            instanceData->hBackEvent, CSR_UI_DEFAULTACTION);
        eventNotification(instanceData, TRUE, TRUE, "BlueCore communication malfunction. Deactivating transport.");
    }
}

void CsrBluecoreManagerAppRegisterCfmHandler(CsrBluecoreManagerInstanceData *instanceData)
{
    CsrAppRegisterCfm *confirm = instanceData->message;
    instanceData->controlId = confirm->controlId;
}

void CsrBluecoreManagerAppTakeControlIndHandler(CsrBluecoreManagerInstanceData *instanceData)
{
    CsrAppTakeControlInd *indication = (CsrAppTakeControlInd *) instanceData->message;

    if (indication->controlId == instanceData->controlId)
    {
        CsrUiUieShowReqSend(instanceData->hMenu, CSR_BLUECORE_MANAGER_IFACEQUEUE, CSR_UI_INPUTMODE_AUTO, MENU_PRIORITY);
    }
}

void CsrBluecoreManagerUiUieCreateCfmHandler(CsrBluecoreManagerInstanceData *instanceData)
{
    CsrUiUieCreateCfm *confirm = instanceData->message;
    CsrBool uiCreationCycleDone = FALSE;

    switch (confirm->elementType)
    {
        case CSR_UI_UIETYPE_EVENT:
        {
            CsrBool lastEventCreated = FALSE;
            if (!instanceData->hSelectEvent)
            {
                instanceData->hSelectEvent = confirm->handle;
                CsrUiEventSetReqSend(confirm->handle, CSR_UI_INPUTMODE_AUTO, CSR_BLUECORE_MANAGER_IFACEQUEUE);
            }
            else if (!instanceData->hBackEvent)
            {
                instanceData->hBackEvent = confirm->handle;
                CsrUiEventSetReqSend(confirm->handle, CSR_UI_INPUTMODE_BLOCK, CSR_BLUECORE_MANAGER_IFACEQUEUE);
                lastEventCreated = TRUE;
            }

            if (!lastEventCreated)
            {
                CsrUiUieCreateReqSend(CSR_BLUECORE_MANAGER_IFACEQUEUE, CSR_UI_UIETYPE_EVENT);
            }
            else
            {
                CsrUiUieCreateReqSend(CSR_BLUECORE_MANAGER_IFACEQUEUE, CSR_UI_UIETYPE_MENU);
            }
            break;
        }
        case CSR_UI_UIETYPE_MENU:
        {
            CsrBool lastMenuCreated = FALSE;
            if (!instanceData->hMenu)
            {
                instanceData->hMenu = confirm->handle;

                CsrUiMenuSetReqSend(confirm->handle,
                    CsrUtf82Utf16String((const CsrUtf8String *) (CSR_BLUECORE_MANAGER_MENU_CATEGORY " " CSR_BLUECORE_MANAGER_MENU_ITEM_NAME)),
                    CsrUtf82Utf16String((const CsrUtf8String *) ("Select")), NULL);
                CsrUiMenuAdditemReqSend(confirm->handle, CSR_UI_FIRST,
                    MENU_ITEM_ACTIVATE_KEY, MENU_ITEM_ACTIVATE_INACTIVE_ICON,
                    CsrUtf82Utf16String((CsrUtf8String *) MENU_ITEM_ACTIVATE_INACTIVE_TEXT),
                    CsrUtf82Utf16String((CsrUtf8String *) MENU_ITEM_ACTIVATE_INACTIVE_SUBTEXT),
                    instanceData->hSelectEvent, CSR_UI_DEFAULTACTION,
                    instanceData->hBackEvent, CSR_UI_DEFAULTACTION);

                lastMenuCreated = TRUE;
            }

            if (!lastMenuCreated)
            {
                CsrUiUieCreateReqSend(CSR_BLUECORE_MANAGER_IFACEQUEUE, CSR_UI_UIETYPE_MENU);
            }
            else
            {
                CsrUiUieCreateReqSend(CSR_BLUECORE_MANAGER_IFACEQUEUE, CSR_UI_UIETYPE_DIALOG);
            }
            break;
        }
        case CSR_UI_UIETYPE_DIALOG:
        {
            instanceData->hPopup = confirm->handle;
            uiCreationCycleDone = TRUE;
            break;
        }
        default:
            break;
    }

    if (uiCreationCycleDone)
    {
        /* Add menu entry to main menu */
        CsrAppRegisterReqSend2(CSR_BLUECORE_MANAGER_IFACEQUEUE,
            (CsrUtf8String *) CsrStrDup(CSR_BLUECORE_MANAGER_MENU_CATEGORY),
            (CsrUtf8String *) CsrStrDup(CSR_BLUECORE_MANAGER_MENU_ITEM_NAME),
            CSR_UI_ICON_BLUETOOTH,
            CSR_UI_ICON_NONE,
            TRUE);
    }
}

static void activateDeactivate(CsrBluecoreManagerInstanceData *instanceData)
{
    switch (instanceData->state)
    {
        case CSR_BLUECORE_MANAGER_STATE_INACTIVE:
        {
            instanceData->state = CSR_BLUECORE_MANAGER_STATE_ACTIVATING;
            CsrTmBlueCoreActivateTransportReqSend(CSR_BLUECORE_MANAGER_IFACEQUEUE);
            CsrUiMenuSetitemReqSend(instanceData->hMenu,
                MENU_ITEM_ACTIVATE_KEY, MENU_ITEM_ACTIVATE_ACTIVATING_ICON,
                CsrUtf82Utf16String((CsrUtf8String *) MENU_ITEM_ACTIVATE_ACTIVATING_TEXT),
                CsrUtf82Utf16String((CsrUtf8String *) MENU_ITEM_ACTIVATE_ACTIVATING_SUBTEXT),
                instanceData->hSelectEvent, CSR_UI_DEFAULTACTION,
                instanceData->hBackEvent, CSR_UI_DEFAULTACTION);
            break;
        }
        case CSR_BLUECORE_MANAGER_STATE_ACTIVE:
        {
            instanceData->state = CSR_BLUECORE_MANAGER_STATE_DEACTIVATING;
            CsrTmBlueCoreDeactivateTransportReqSend(CSR_BLUECORE_MANAGER_IFACEQUEUE);
            CsrUiMenuSetitemReqSend(instanceData->hMenu,
                MENU_ITEM_ACTIVATE_KEY, MENU_ITEM_ACTIVATE_DEACTIVATING_ICON,
                CsrUtf82Utf16String((CsrUtf8String *) MENU_ITEM_ACTIVATE_DEACTIVATING_TEXT),
                CsrUtf82Utf16String((CsrUtf8String *) MENU_ITEM_ACTIVATE_DEACTIVATING_SUBTEXT),
                instanceData->hSelectEvent, CSR_UI_DEFAULTACTION,
                instanceData->hBackEvent, CSR_UI_DEFAULTACTION);
            break;
        }
        case CSR_BLUECORE_MANAGER_STATE_ACTIVATING:
        case CSR_BLUECORE_MANAGER_STATE_DEACTIVATING:
        default:
            break;
    }
}

void CsrBluecoreManagerUiEventIndHandler(CsrBluecoreManagerInstanceData *instanceData)
{
    CsrUiEventInd *indication = instanceData->message;

    if ((indication->displayElement == instanceData->hMenu) &&
        (indication->event == instanceData->hSelectEvent))
    {
        switch (indication->key)
        {
            case MENU_ITEM_ACTIVATE_KEY:
                activateDeactivate(instanceData);
                break;
            default:
                break;
        }
    }
    else if (indication->event == instanceData->hBackEvent)
    {
        CsrUiUieHideReqSend(indication->displayElement);
    }
}
