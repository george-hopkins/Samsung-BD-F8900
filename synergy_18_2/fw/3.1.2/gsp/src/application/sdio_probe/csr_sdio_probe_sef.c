/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_types.h"
#include "csr_formatted_io.h"
#include "csr_unicode.h"
#include "csr_log_text_2.h"
#include "csr_sdio.h"

/* CSR UI Interface */
#include "csr_ui_task.h"
#include "csr_ui_prim.h"
#include "csr_ui_lib.h"

/* CSR APP Interface */
#include "csr_app_task.h"
#include "csr_app_prim.h"
#include "csr_app_lib.h"

/* SDIO Probe Headers */
#include "csr_sdio_probe_task.h"
#include "csr_sdio_probe_handler.h"
#include "csr_sdio_probe_sef.h"
#include "csr_sdio_probe_internal_lib.h"
#include "csr_sdio_probe_test.h"

#define MENU_PRIORITY 0
#define POPUP_PRIORITY 1

#define MENU_ITEM_REGISTER_ICON CSR_UI_ICON_NONE
#define MENU_ITEM_REGISTER_TEXT "Register/Unregister"
#define MENU_ITEM_REGISTER_UNREGISTERED_SUBTEXT "Not registered"
#define MENU_ITEM_REGISTER_REGISTERED_SUBTEXT "Registered"

#define MENU_ITEM_REGISTER_KEY (CSR_SDIO_PROBE_MAX_FUNCTIONS + 1)

#define TEST_MENU_ITEM_ASYNC_KEY 0
#define TEST_MENU_ITEM_SYNC_KEY  1
#define TEST_MENU_ITEM_INT_KEY   2

#define POPUP_MAX_MESSAGE_LENGTH 256

void CsrSdioProbeEventNotification(CsrSdioProbeInstanceData *instanceData, CsrBool backlog, CsrBool popup, const CsrCharString *fmt, ...)
{
    CsrCharString message[POPUP_MAX_MESSAGE_LENGTH];
    va_list args;

    va_start(args, fmt);
    CsrVsnprintf(message, POPUP_MAX_MESSAGE_LENGTH, fmt, args);
    va_end(args);

    if (backlog)
    {
        CsrAppBacklogReqSend((CsrUtf8String *) CsrStrDup(CSR_SDIO_PROBE_MENU_CATEGORY), (CsrUtf8String *) CsrStrDup(CSR_SDIO_PROBE_MENU_ITEM_NAME), FALSE, message);
    }
    if (popup)
    {
        CsrUiDialogSetReqSend(instanceData->hPopup,
            CsrUtf82Utf16String((CsrUtf8String *) CSR_SDIO_PROBE_MENU_CATEGORY " " CSR_SDIO_PROBE_MENU_ITEM_NAME),
            CsrUtf82Utf16String((CsrUtf8String *) message),
            CsrUtf82Utf16String((CsrUtf8String *) "OK"),
            NULL,
            instanceData->hBackEvent,
            CSR_UI_DEFAULTACTION,
            instanceData->hBackEvent,
            CSR_UI_DEFAULTACTION);
        CsrUiUieShowReqSend(instanceData->hPopup, CSR_SDIO_PROBE_IFACEQUEUE, CSR_UI_INPUTMODE_AUTO, POPUP_PRIORITY);
    }
}

static void insertedCallback(CsrSdioFunction *function)
{
    CsrSdioProbeInternalInsertedIndSend(function);
}

static void removedCallback(CsrSdioFunction *function)
{
    CsrSdioProbeInternalRemovedIndSend(function);
}

static CsrSdioFunctionId functionId[] =
{
    { /* CSR WiFi */
        0x032A,
        0x0007,
        1,
        CSR_SDIO_ANY_SDIO_INTERFACE,
    },
    { /* CSR Bluetooth */
        0x032A,
        CSR_SDIO_ANY_CARD_ID,
        CSR_SDIO_ANY_SDIO_FUNCTION,
        0x02,
    },
};

static CsrSdioFunctionDriver functionDriver =
{
    insertedCallback,
    removedCallback,
    CsrSdioProbeInterruptCallback,
    NULL,
    NULL,
    functionId,
    CSR_ARRAY_SIZE(functionId),
    NULL
};

static void parseDeviceInfo(CsrSdioProbeInstanceData *instanceData, CsrUint8 key, CsrSdioFunction *function, CsrUtf16String **deviceName, CsrUtf16String **deviceId)
{
    CsrSdioProbeTestData *testData = &instanceData->testData[key];
    CsrCharString devId[11];

    if ((function->sdioId.manfId == 0x032A) && (function->sdioId.cardId == 0x0007))
    {
        testData->chipType = CSR_SDIO_PROBE_CHIP_TYPE_WIFI;
        testData->wakeupInterruptClearAddress = 0xF1;
        testData->scratchRegister = 0x10;
        testData->dataLength = 2048;
        testData->dataVerify = TRUE;
        *deviceName = CsrUtf82Utf16String((const CsrUtf8String *) "WiFi");
    }
    else if ((function->sdioId.manfId == 0x032A) &&
             (function->sdioId.sdioInterface == 0x02) &&
             (function->sdioId.cardId == 0x0004))
    {
        testData->chipType = CSR_SDIO_PROBE_CHIP_TYPE_BC6;
        testData->wakeupInterruptClearAddress = 0;
        testData->scratchRegister = 0x10;
        testData->dataLength = 2048;
        testData->dataVerify = FALSE;
        *deviceName = CsrUtf82Utf16String((const CsrUtf8String *) "Bluetooth (BC6)");
    }
    else if ((function->sdioId.manfId == 0x032A) &&
             (function->sdioId.sdioInterface == 0x02) &&
             (function->sdioId.cardId == 0x0006))
    {
        testData->chipType = CSR_SDIO_PROBE_CHIP_TYPE_BC7;
        testData->wakeupInterruptClearAddress = 0;
        testData->scratchRegister = 0x10;
        testData->dataLength = 2048;
        testData->dataVerify = FALSE;
        *deviceName = CsrUtf82Utf16String((const CsrUtf8String *) "Bluetooth (BC7)");
    }
    else
    {
        *deviceName = CsrUtf82Utf16String((const CsrUtf8String *) "Unknown Device");
    }
    CsrSnprintf(devId, 11, "[%08X]", (CsrUint32) (CsrUintptr) function->device);
    *deviceId = CsrUtf82Utf16String((const CsrUtf8String *) devId);
}

void CsrSdioProbeInternalInsertedIndHandler(CsrSdioProbeInstanceData *instanceData)
{
    CsrSdioProbeInternalInsertedInd *indication = instanceData->message;
    CsrUint8 i;

    for (i = 0; i < CSR_SDIO_PROBE_MAX_FUNCTIONS; ++i)
    {
        if (instanceData->functions[i] == NULL)
        {
            CsrUtf16String *deviceName, *deviceId;
            parseDeviceInfo(instanceData, i, indication->function, &deviceName, &deviceId);
            instanceData->functions[i] = indication->function;
            CsrUiMenuAdditemReqSend(instanceData->hMenu, CSR_UI_LAST,
                i, CSR_UI_ICON_DEVICE,
                deviceName, deviceId,
                instanceData->hSelectEvent, CSR_UI_DEFAULTACTION,
                instanceData->hBackEvent, CSR_UI_DEFAULTACTION);
            break;
        }
    }

    instanceData->attachedCount += 1;
    CsrSdioInsertedAcknowledge(indication->function, CSR_RESULT_SUCCESS);
}

void CsrSdioProbeInternalRemovedIndHandler(CsrSdioProbeInstanceData *instanceData)
{
    CsrSdioProbeInternalRemovedInd *indication = instanceData->message;
    CsrUint8 i;

    for (i = 0; i < CSR_SDIO_PROBE_MAX_FUNCTIONS; ++i)
    {
        if (instanceData->functions[i] == indication->function)
        {
            instanceData->functions[i] = NULL;
            CsrUiMenuRemoveitemReqSend(instanceData->hMenu, i);
        }
    }

    instanceData->attachedCount -= 1;
    CsrSdioRemovedAcknowledge(indication->function);
}

void CsrSdioProbeAppRegisterCfmHandler(CsrSdioProbeInstanceData *instanceData)
{
    CsrAppRegisterCfm *confirm = instanceData->message;
    instanceData->controlId = confirm->controlId;
}

void CsrSdioProbeAppTakeControlIndHandler(CsrSdioProbeInstanceData *instanceData)
{
    CsrAppTakeControlInd *indication = (CsrAppTakeControlInd *) instanceData->message;

    if (indication->controlId == instanceData->controlId)
    {
        CsrUiUieShowReqSend(instanceData->hMenu, CSR_SDIO_PROBE_IFACEQUEUE, CSR_UI_INPUTMODE_AUTO, MENU_PRIORITY);
    }
}

void CsrSdioProbeUiUieCreateCfmHandler(CsrSdioProbeInstanceData *instanceData)
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
                CsrUiEventSetReqSend(confirm->handle, CSR_UI_INPUTMODE_AUTO, CSR_SDIO_PROBE_IFACEQUEUE);
            }
            else if (!instanceData->hBackEvent)
            {
                instanceData->hBackEvent = confirm->handle;
                CsrUiEventSetReqSend(confirm->handle, CSR_UI_INPUTMODE_BLOCK, CSR_SDIO_PROBE_IFACEQUEUE);
                lastEventCreated = TRUE;
            }

            if (!lastEventCreated)
            {
                CsrUiUieCreateReqSend(CSR_SDIO_PROBE_IFACEQUEUE, CSR_UI_UIETYPE_EVENT);
            }
            else
            {
                CsrUiUieCreateReqSend(CSR_SDIO_PROBE_IFACEQUEUE, CSR_UI_UIETYPE_MENU);
            }
            break;
        }
        case CSR_UI_UIETYPE_MENU:
        {
            CsrBool lastMenuCreated = FALSE;
            if (!instanceData->hTestMenu)
            {
                instanceData->hTestMenu = confirm->handle;
                CsrUiMenuSetReqSend(confirm->handle,
                    CsrUtf82Utf16String((const CsrUtf8String *) (CSR_SDIO_PROBE_MENU_CATEGORY " " CSR_SDIO_PROBE_MENU_ITEM_NAME " Tests")),
                    CsrUtf82Utf16String((const CsrUtf8String *) ("Select")), NULL);
                CsrUiMenuAdditemReqSend(confirm->handle, CSR_UI_LAST,
                    TEST_MENU_ITEM_ASYNC_KEY, CSR_UI_ICON_NONE,
                    NULL,
                    NULL,
                    CSR_UI_DEFAULTACTION, CSR_UI_DEFAULTACTION,
                    CSR_UI_DEFAULTACTION, CSR_UI_DEFAULTACTION);
                CsrUiMenuAdditemReqSend(confirm->handle, CSR_UI_LAST,
                    TEST_MENU_ITEM_SYNC_KEY, CSR_UI_ICON_NONE,
                    NULL,
                    NULL,
                    CSR_UI_DEFAULTACTION, CSR_UI_DEFAULTACTION,
                    CSR_UI_DEFAULTACTION, CSR_UI_DEFAULTACTION);
                CsrUiMenuAdditemReqSend(confirm->handle, CSR_UI_LAST,
                    TEST_MENU_ITEM_INT_KEY, CSR_UI_ICON_NONE,
                    NULL,
                    NULL,
                    CSR_UI_DEFAULTACTION, CSR_UI_DEFAULTACTION,
                    CSR_UI_DEFAULTACTION, CSR_UI_DEFAULTACTION);
            }
            else if (!instanceData->hMenu)
            {
                instanceData->hMenu = confirm->handle;

                CsrUiMenuSetReqSend(confirm->handle,
                    CsrUtf82Utf16String((const CsrUtf8String *) (CSR_SDIO_PROBE_MENU_CATEGORY " " CSR_SDIO_PROBE_MENU_ITEM_NAME)),
                    CsrUtf82Utf16String((const CsrUtf8String *) ("Select")), NULL);
                CsrUiMenuAdditemReqSend(confirm->handle, CSR_UI_FIRST,
                    MENU_ITEM_REGISTER_KEY, MENU_ITEM_REGISTER_ICON,
                    CsrUtf82Utf16String((CsrUtf8String *) MENU_ITEM_REGISTER_TEXT),
                    CsrUtf82Utf16String((CsrUtf8String *) MENU_ITEM_REGISTER_UNREGISTERED_SUBTEXT),
                    instanceData->hSelectEvent, CSR_UI_DEFAULTACTION,
                    instanceData->hBackEvent, CSR_UI_DEFAULTACTION);

                lastMenuCreated = TRUE;
            }

            if (!lastMenuCreated)
            {
                CsrUiUieCreateReqSend(CSR_SDIO_PROBE_IFACEQUEUE, CSR_UI_UIETYPE_MENU);
            }
            else
            {
                CsrUiUieCreateReqSend(CSR_SDIO_PROBE_IFACEQUEUE, CSR_UI_UIETYPE_DIALOG);
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
        CsrAppRegisterReqSend(CSR_SDIO_PROBE_IFACEQUEUE, (CsrUtf8String *) CsrStrDup(CSR_SDIO_PROBE_MENU_CATEGORY), (CsrUtf8String *) CsrStrDup(CSR_SDIO_PROBE_MENU_ITEM_NAME));
    }
}

static void startStop(CsrSdioProbeInstanceData *instanceData)
{
    if ((instanceData->state == CSR_SDIO_PROBE_STATE_OFF) && (instanceData->attachedCount == 0))
    {
        instanceData->state = CSR_SDIO_PROBE_STATE_ON;
        CsrUiMenuSetitemReqSend(instanceData->hMenu,
            MENU_ITEM_REGISTER_KEY, MENU_ITEM_REGISTER_ICON,
            CsrUtf82Utf16String((CsrUtf8String *) MENU_ITEM_REGISTER_TEXT),
            CsrUtf82Utf16String((CsrUtf8String *) MENU_ITEM_REGISTER_REGISTERED_SUBTEXT),
            instanceData->hSelectEvent, CSR_UI_DEFAULTACTION,
            instanceData->hBackEvent, CSR_UI_DEFAULTACTION);
        CsrSdioFunctionDriverRegister(&functionDriver);
    }
    else if (instanceData->state == CSR_SDIO_PROBE_STATE_ON)
    {
        if (instanceData->testsRunning != 0)
        {
            CsrSdioProbeEventNotification(instanceData, FALSE, TRUE, "Unable to unregister while tests are running.");
        }
        else
        {
            instanceData->state = CSR_SDIO_PROBE_STATE_OFF;
            CsrUiMenuSetitemReqSend(instanceData->hMenu,
                MENU_ITEM_REGISTER_KEY, MENU_ITEM_REGISTER_ICON,
                CsrUtf82Utf16String((CsrUtf8String *) MENU_ITEM_REGISTER_TEXT),
                CsrUtf82Utf16String((CsrUtf8String *) MENU_ITEM_REGISTER_UNREGISTERED_SUBTEXT),
                instanceData->hSelectEvent, CSR_UI_DEFAULTACTION,
                instanceData->hBackEvent, CSR_UI_DEFAULTACTION);
            CsrSdioFunctionDriverUnregister(&functionDriver);
        }
    }
}

void CsrSdioProbeTestMenuUpdate(CsrSdioProbeInstanceData *instanceData, CsrUint16 key)
{
    CsrSdioProbeTestData *testData = &instanceData->testData[key];
    CsrUtf16String *asyncSubstring = NULL;
    CsrUtf16String *syncSubstring = NULL;
    CsrUtf16String *intSubstring = NULL;
    CsrUint16 asyncIcon = CSR_UI_ICON_MARK_DELETE;
    CsrUint16 syncIcon = CSR_UI_ICON_MARK_DELETE;
    CsrUint16 intIcon = CSR_UI_ICON_MARK_DELETE;

    if (instanceData->testMenuKey != key)
    {
        return;
    }

    if (testData->testType == CSR_SDIO_PROBE_TEST_TYPE_ASYNC)
    {
        asyncSubstring = CsrUtf82Utf16String((CsrUtf8String *) "Running");
        asyncIcon = CSR_UI_ICON_MARK_ACCEPT;
    }
    else if (testData->testType == CSR_SDIO_PROBE_TEST_TYPE_SYNC)
    {
        syncSubstring = CsrUtf82Utf16String((CsrUtf8String *) "Running");
        syncIcon = CSR_UI_ICON_MARK_ACCEPT;
    }
    else if (testData->testType == CSR_SDIO_PROBE_TEST_TYPE_INT)
    {
        intSubstring = CsrUtf82Utf16String((CsrUtf8String *) "Running");
        intIcon = CSR_UI_ICON_MARK_ACCEPT;
    }
    if (asyncSubstring == NULL)
    {
        asyncSubstring = CsrUtf82Utf16String((CsrUtf8String *) "Inactive");
    }
    if (syncSubstring == NULL)
    {
        syncSubstring = CsrUtf82Utf16String((CsrUtf8String *) "Inactive");
    }
    if (intSubstring == NULL)
    {
        intSubstring = CsrUtf82Utf16String((CsrUtf8String *) "Inactive");
    }

    CsrUiMenuSetitemReqSend(instanceData->hTestMenu,
        TEST_MENU_ITEM_ASYNC_KEY, asyncIcon,
        CsrUtf82Utf16String((CsrUtf8String *) "Asynchronous"),
        asyncSubstring,
        instanceData->hSelectEvent, CSR_UI_DEFAULTACTION,
        instanceData->hBackEvent, CSR_UI_DEFAULTACTION);
    CsrUiMenuSetitemReqSend(instanceData->hTestMenu,
        TEST_MENU_ITEM_SYNC_KEY, syncIcon,
        CsrUtf82Utf16String((CsrUtf8String *) "Synchronous"),
        syncSubstring,
        instanceData->hSelectEvent, CSR_UI_DEFAULTACTION,
        instanceData->hBackEvent, CSR_UI_DEFAULTACTION);
    CsrUiMenuSetitemReqSend(instanceData->hTestMenu,
        TEST_MENU_ITEM_INT_KEY, intIcon,
        CsrUtf82Utf16String((CsrUtf8String *) "Interrupt"),
        intSubstring,
        instanceData->hSelectEvent, CSR_UI_DEFAULTACTION,
        instanceData->hBackEvent, CSR_UI_DEFAULTACTION);
}

void CsrSdioProbeUiEventIndHandler(CsrSdioProbeInstanceData *instanceData)
{
    CsrUiEventInd *indication = instanceData->message;

    if ((indication->displayElement == instanceData->hMenu) &&
        (indication->event == instanceData->hSelectEvent))
    {
        switch (indication->key)
        {
            case MENU_ITEM_REGISTER_KEY:
                startStop(instanceData);
                break;
            default:
                if (instanceData->testData[indication->key].chipType != CSR_SDIO_PROBE_CHIP_TYPE_UNKNOWN)
                {
                    instanceData->testMenuKey = indication->key;
                    CsrSdioProbeTestMenuUpdate(instanceData, indication->key);
                    CsrUiUieShowReqSend(instanceData->hTestMenu, CSR_SDIO_PROBE_IFACEQUEUE, CSR_UI_INPUTMODE_AUTO, CSR_UI_LOWESTPRIORITY);
                }
                break;
        }
    }
    else if ((indication->displayElement == instanceData->hTestMenu) &&
             (indication->event == instanceData->hSelectEvent))
    {
        switch (indication->key)
        {
            case TEST_MENU_ITEM_ASYNC_KEY:
                CsrSdioProbeStartStopTest(instanceData, instanceData->testMenuKey, CSR_SDIO_PROBE_TEST_TYPE_ASYNC);
                break;
            case TEST_MENU_ITEM_SYNC_KEY:
                CsrSdioProbeStartStopTest(instanceData, instanceData->testMenuKey, CSR_SDIO_PROBE_TEST_TYPE_SYNC);
                break;
            case TEST_MENU_ITEM_INT_KEY:
                CsrSdioProbeStartStopTest(instanceData, instanceData->testMenuKey, CSR_SDIO_PROBE_TEST_TYPE_INT);
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
