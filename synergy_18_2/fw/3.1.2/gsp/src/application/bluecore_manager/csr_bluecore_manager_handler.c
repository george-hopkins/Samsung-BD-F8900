/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_usr_config.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_sched.h"
#include "csr_util.h"
#include "csr_log_text_2.h"

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

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DEFINE(CsrBluecoreManagerLto);

void CsrBluecoreManagerInit(void **gash)
{
    *gash = CsrPmemZalloc(sizeof(CsrBluecoreManagerInstanceData));

#ifndef CSR_BLUECORE_ONOFF
    CsrTmBlueCoreActivateTransportReqSend(CSR_BLUECORE_MANAGER_IFACEQUEUE);
#else
    /* Start UI creation cycle */
    CsrUiUieCreateReqSend(CSR_BLUECORE_MANAGER_IFACEQUEUE, CSR_UI_UIETYPE_EVENT);
#endif

    CSR_LOG_TEXT_REGISTER(&CsrBluecoreManagerLto, "BLUECORE_MANAGER", 0, NULL);
}

void CsrBluecoreManagerHandler(void **gash)
{
    CsrBluecoreManagerInstanceData *instanceData;
    CsrUint16 event;

    instanceData = (CsrBluecoreManagerInstanceData *) *gash;

    if (CsrSchedMessageGet(&event, &instanceData->message))
    {
        switch (event)
        {
            case CSR_TM_BLUECORE_PRIM:
            {
                CsrPrim *type = instanceData->message;

                if (*type == CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_CFM)
                {
#ifndef CSR_BLUECORE_ONOFF
                    CsrUiStatusBluetoothSetReqSend(TRUE);
#else
                    CsrBluecoreManagerTmBluecoreActivateTransportCfmHandler(instanceData);
#endif
                }
                else if (*type == CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_CFM)
                {
                    CsrBluecoreManagerTmBluecoreDeactivateTransportCfmHandler(instanceData);
                }
                else if (*type == CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_IND)
                {
                    CsrBluecoreManagerTmBluecoreDeactivateTransportIndHandler(instanceData);
                }
                break;
            }
            case CSR_APP_PRIM:
            {
                CsrPrim *type = instanceData->message;

                if (*type == CSR_APP_REGISTER_CFM)
                {
                    CsrBluecoreManagerAppRegisterCfmHandler(instanceData);
                }
                else if (*type == CSR_APP_TAKE_CONTROL_IND)
                {
                    CsrBluecoreManagerAppTakeControlIndHandler(instanceData);
                }
                else
                {
                    CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrBluecoreManagerLto, 0, event, *type);
                }
                break;
            }
            case CSR_UI_PRIM:
            {
                CsrPrim *type = instanceData->message;

                if (*type == CSR_UI_UIE_CREATE_CFM)
                {
                    CsrBluecoreManagerUiUieCreateCfmHandler(instanceData);
                }
                else if (*type == CSR_UI_EVENT_IND)
                {
                    CsrBluecoreManagerUiEventIndHandler(instanceData);
                }
                else
                {
                    CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrBluecoreManagerLto, 0, event, *type);
                }
                break;
            }
            default:
            {
                CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrBluecoreManagerLto, 0, event, 0);
                break;
            }
        }

        /* Free the message itself */
        CsrPmemFree(instanceData->message);
        instanceData->message = NULL;
    }
}

#ifdef ENABLE_SHUTDOWN
void CsrBluecoreManagerDeinit(void **gash)
{
    CsrBluecoreManagerInstanceData *instanceData;
    void *message;
    CsrUint16 event;

    instanceData = (CsrBluecoreManagerInstanceData *) *gash;

    /* Clean own queue */
    while (CsrSchedMessageGet(&event, &message))
    {
        switch (event)
        {
            case CSR_TM_BLUECORE_PRIM:
                /* No allocated content in upstream messages */
                break;
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
