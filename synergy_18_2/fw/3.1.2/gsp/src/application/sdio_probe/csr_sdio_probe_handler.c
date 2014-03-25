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
#include "csr_log_text_2.h"

/* CSR_UI Interface */
#include "csr_ui_task.h"
#include "csr_ui_prim.h"
#include "csr_ui_lib.h"

/* CSR_APP Interface */
#include "csr_app_task.h"
#include "csr_app_prim.h"
#include "csr_app_lib.h"

/* SDIO Probe Headers */
#include "csr_sdio_probe_task.h"
#include "csr_sdio_probe_handler.h"
#include "csr_sdio_probe_sef.h"
#include "csr_sdio_probe_internal_prim.h"

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DEFINE(CsrSdioProbeLto);

/* Jump table for internal handlers */
typedef void (*msgHandler)(CsrSdioProbeInstanceData *instanceData);
static const msgHandler sdioProbeHandlers[CSR_SDIO_PROBE_INTERNAL_UPSTREAM_COUNT] =
{
    CsrSdioProbeInternalInsertedIndHandler,             /* CSR_SDIO_PROBE_INTERNAL_INSERTED_IND */
    CsrSdioProbeInternalRemovedIndHandler,              /* CSR_SDIO_PROBE_INTERNAL_REMOVED_IND */
};

/* Jump table for CSR_APP upstream messages */
static const msgHandler appHandlers[CSR_APP_PRIM_UPSTREAM_COUNT] =
{
    CsrSdioProbeAppRegisterCfmHandler,                  /* CSR_APP_REGISTER_CFM */
    CsrSdioProbeAppTakeControlIndHandler,               /* CSR_APP_TAKE_CONTROL_IND */
};

/* Jump table for CSR_UI upstream messages */
static const msgHandler uiHandlers[CSR_UI_PRIM_UPSTREAM_COUNT] =
{
    CsrSdioProbeUiUieCreateCfmHandler,                  /* CSR_UI_UIE_CREATE_CFM */
    NULL,                                               /* CSR_UI_MENU_GET_CFM */
    NULL,                                               /* CSR_UI_MENU_GETITEM_CFM */
    NULL,                                               /* CSR_UI_MENU_GETCURSOR_CFM */
    NULL,                                               /* CSR_UI_EVENT_GET_CFM */
    NULL,                                               /* CSR_UI_DIALOG_GET_CFM */
    NULL,                                               /* CSR_UI_INPUTDIALOG_GET_CFM */
    NULL,                                               /* CSR_UI_IDLESCREEN_GET_CFM */
    NULL,                                               /* CSR_UI_DISPLAY_GETHANDLE_CFM */
    NULL,                                               /* CSR_UI_KEYDOWN_IND */
    CsrSdioProbeUiEventIndHandler,                      /* CSR_UI_EVENT_IND */
};

void CsrSdioProbeInit(void **gash)
{
    *gash = CsrPmemZalloc(sizeof(CsrSdioProbeInstanceData));

    /* Start UI creation cycle */
    CsrUiUieCreateReqSend(CSR_SDIO_PROBE_IFACEQUEUE, CSR_UI_UIETYPE_EVENT);

    CSR_LOG_TEXT_REGISTER(&CsrSdioProbeLto, "SDIO_PROBE", 0, NULL);
}

void CsrSdioProbeHandler(void **gash)
{
    CsrSdioProbeInstanceData *instanceData;
    CsrUint16 event;
    void *message;

    instanceData = (CsrSdioProbeInstanceData *) *gash;

    if (CsrSchedMessageGet(&event, &message))
    {
        instanceData->message = message;

        switch (event)
        {
            case CSR_SDIO_PROBE_PRIM:
            {
                CsrPrim *type = (CsrPrim *) message;
                CsrPrim prim = (*type) - CSR_SDIO_PROBE_PRIM_UPSTREAM_LOWEST;

                if ((prim < CSR_SDIO_PROBE_INTERNAL_UPSTREAM_COUNT) && (sdioProbeHandlers[prim] != NULL))
                {
                    sdioProbeHandlers[prim](instanceData);
                }
                else
                {
                    CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrSdioProbeLto, 0, event, *type);
                }
                break;
            }
            case CSR_UI_PRIM:
            {
                CsrUint16 *type = (CsrUint16 *) message;
                CsrUint16 prim = (*type) - CSR_UI_PRIM_UPSTREAM_LOWEST;

                if ((prim < CSR_UI_PRIM_UPSTREAM_COUNT) && (uiHandlers[prim] != NULL))
                {
                    uiHandlers[prim](instanceData);
                }
                else
                {
                    CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrSdioProbeLto, 0, event, *type);
                }
                break;
            }
            case CSR_APP_PRIM:
            {
                CsrUint16 *type = (CsrUint16 *) message;
                CsrUint16 prim = (*type) - CSR_APP_PRIM_UPSTREAM_LOWEST;

                if ((prim < CSR_APP_PRIM_UPSTREAM_COUNT) && (appHandlers[prim] != NULL))
                {
                    appHandlers[prim](instanceData);
                }
                else
                {
                    CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrSdioProbeLto, 0, event, *type);
                }
                break;
            }
            default:
            {
                CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrSdioProbeLto, 0, event, 0);
                break;
            }
        }

        /* Free the message itself */
        CsrPmemFree(instanceData->message);
        instanceData->message = NULL;
    }
}

#ifdef ENABLE_SHUTDOWN
void CsrSdioProbeDeinit(void **gash)
{
    CsrSdioProbeInstanceData *instanceData;
    void *message;
    CsrUint16 event;

    instanceData = (CsrSdioProbeInstanceData *) *gash;

    /* Clean own queue */
    while (CsrSchedMessageGet(&event, &message))
    {
        switch (event)
        {
            case CSR_APP_PRIM:
                /* No allocated content in upstream messages */
                break;
            case CSR_UI_PRIM:
                CsrUiFreeUpstreamMessageContents(event, message);
                break;
            default:
                break;
        }
        CsrPmemFree(message);
    }

    /* Free instance data */
    CsrPmemFree(instanceData);
}

#endif
