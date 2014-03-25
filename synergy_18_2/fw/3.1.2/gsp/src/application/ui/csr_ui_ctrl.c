/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_sched.h"
#include "csr_ui_lib.h"

#include "csr_ui_sef.h"
#include "csr_ui_ctrl.h"
#include "csr_ui_private_prim.h"
#include "csr_ui_keyhandler.h"
#include "csr_ui_exception.h"
#include "csr_ui_uiemanager.h"
#include "csr_ui_displaystack.h"
#include "csr_ui_renderer_text.h"
#include "csr_ui_renderer.h"
#include "csr_ui_emulator_touchdecoder.h"

/* ---------------------------- CSR_UI Jump Table ------------------------------*/
typedef CsrBool (*CsrUiStateHandlerType)(CsrUiInstanceData *instanceData);
static const CsrUiStateHandlerType
    stateHandlers[TOTAL_NUM_OF_CSR_UI_DOWNSTREAM_PRIMS] =
{
    CsrUiUieCreateReqHandler,               /* CSR_UI_UIE_CREATE_REQ */
    CsrUiMenuSetReqHandler,                 /* CSR_UI_MENU_SET_REQ */
    CsrUiMenuGetReqHandler,                 /* CSR_UI_MENU_GET_REQ */
    CsrUiMenuAdditemReqHandler,             /* CSR_UI_MENU_ADDITEM_REQ */
    CsrUiMenuRemoveitemReqHandler,          /* CSR_UI_MENU_REMOVEITEM_REQ */
    CsrUiMenuRemoveallitemsReqHandler,      /* CSR_UI_MENU_REMOVEALLITEMS_REQ */
    CsrUiMenuSetitemReqHandler,             /* CSR_UI_MENU_SETITEM_REQ */
    CsrUiMenuGetitemReqHandler,             /* CSR_UI_MENU_GETITEM_REQ */
    CsrUiMenuSetcursorReqHandler,           /* CSR_UI_MENU_SETCURSOR_REQ */
    CsrUiMenuGetcursorReqHandler,           /* CSR_UI_MENU_GETCURSOR_REQ */
    CsrUiEventSetReqHandler,                /* CSR_UI_EVENT_SET_REQ */
    CsrUiEventGetReqHandler,                /* CSR_UI_EVENT_GET_REQ */
    CsrUiDialogSetReqHandler,               /* CSR_UI_DIALOG_SET_REQ */
    CsrUiDialogGetReqHandler,               /* CSR_UI_DIALOG_GET_REQ */
    CsrUiInputdialogSetReqHandler,          /* CSR_UI_INPUTDIALOG_SET_REQ */
    CsrUiInputdialogGetReqHandler,          /* CSR_UI_INPUTDIALOG_GET_REQ */
    CsrUiIdlescreenSetReqHandler,           /* CSR_UI_IDLESCREEN_SET_REQ */
    CsrUiIdlescreenGetReqHandler,           /* CSR_UI_IDLESCREEN_GET_REQ */
    CsrUiUieShowReqHandler,                 /* CSR_UI_UIE_SHOW_REQ */
    CsrUiUieHideReqHandler,                 /* CSR_UI_UIE_HIDE_REQ */
    CsrUiDisplaySetinputmodeReqHandler,     /* CSR_UI_DISPLAY_SETINPUTMODE_REQ */
    CsrUiDisplayGethandleReqHandler,        /* CSR_UI_DISPLAY_GETHANDLE_REQ */
    CsrUiStatusBatterySetReqHandler,        /* CSR_UI_STATUS_BATTERY_SET_REQ */
    CsrUiStatusRadiometerSetReqHandler,     /* CSR_UI_STATUS_RADIOMETER_SET_REQ */
    CsrUiStatusHeadsetSetReqHandler,        /* CSR_UI_STATUS_HEADSET_SET_REQ */
    CsrUiStatusBluetoothSetReqHandler,      /* CSR_UI_STATUS_BLUETOOTH_SET_REQ */
    CsrUiStatusWifiSetReqHandler,           /* CSR_UI_STATUS_WIFI_SET_REQ */
    CsrUiUieDestroyReqHandler,              /* CSR_UI_UIE_DESTROY_REQ */
    CsrUiKeyHandler                         /* CSR_UI_INTERNAL_KEYDOWN_IND */
};
/* ---------------------------- CSR_UI Jump Table End ----------------------*/

void CsrUiHandler(void **gash)
{
    CsrUiInstanceData *instanceData;
    CsrUint16 event;
    void *message;

    instanceData = (CsrUiInstanceData *) *gash;

    if (CsrSchedMessageGet(&event, &message))
    {
        instanceData->msg = message;

        switch (event)
        {
            case CSR_UI_PRIM:
            {
                CsrUint16 *type;

                type = (CsrUint16 *) message;

                if ((*type < TOTAL_NUM_OF_CSR_UI_DOWNSTREAM_PRIMS) && (stateHandlers[*type]))
                {
                    if (stateHandlers[*type](instanceData))
                    {
                        /* Redraw screen if it has changed */
                        if (instanceData->graphicsRenderer.active)
                        {
                            CsrUiRenderRedraw(instanceData);
                        }
                        if (instanceData->textRenderer.active)
                        {
                            CsrUiRenderTextRedraw(instanceData);
                        }
                    }
                }
                else
                {
                    CSR_UIEXCEPTION("Unknown message type received");
                }
                break;
            }
            default:
            {
                CSR_UIEXCEPTION("Message with unknown/unsupported event class received");
                break;
            }
        }

        CsrPmemFree(instanceData->msg);
    }
    else
    {
        CSR_UIEXCEPTION("Failed to retrieve message from queue");
    }
}

void CsrUiInitialise(void **gash)
{
    CsrUiInstanceData *instanceData;

    instanceData = (CsrUiInstanceData *) CsrPmemZalloc(sizeof(CsrUiInstanceData));
    *gash = (void *) instanceData;

    /* Initialise instance data members */
    instanceData->batteryPercentage = 100;
    instanceData->rssiPercentage = 100;


    /* Retrieve the renderer configurations */
    CsrUiGraphicsRendererConfigurationGet(&(instanceData->graphicsRenderer));
    CsrUiTextRendererConfigurationGet(&(instanceData->textRenderer));

    CsrUiRenderCalculateScreenCoordinates(instanceData);

    if (instanceData->graphicsRenderer.active)
    {
        CsrUiRenderRedraw(instanceData);
    }
    if (instanceData->textRenderer.active)
    {
        CsrUiRenderTextRedraw(instanceData);
    }
}

#ifdef ENABLE_SHUTDOWN

void CsrUiDeinitialise(void **gash)
{
    CsrUint16 event;
    void *message;
    CsrUiInstanceData *instanceData;

    instanceData = (CsrUiInstanceData *) *gash;

    /* Deinitialise the Touch decoder */
    CsrUiTouchDecoderDeinit();

    /* Clean out the display-stack */
    CsrUiDisplayDestroyAll(instanceData);

    /* Destroy every UIE on the heap (and subelements) */
    /* This also deallocates the Heap itself */
    CsrUiUieDestroyAll(instanceData);

    /* Clean out our own queue */
    while (CsrSchedMessageGet(&event, &message))
    {
        if (event == CSR_UI_PRIM)
        {
            CsrUiFreeDownstreamMessageContents(event, message);
        }
        else
        {
            CsrPmemFree(message);
        }
    }

    /* Deallocate instance data */
    CsrPmemFree(instanceData);
}

#endif    /* ENABLE_SHUTDOWN */
