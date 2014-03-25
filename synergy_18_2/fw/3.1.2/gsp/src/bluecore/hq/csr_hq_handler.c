/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_usr_config.h"
#include "csr_hq_handler.h"
#include "csr_env_prim.h"
#include "csr_hq_sef.h"
#include "csr_hq_task.h"
#include "csr_hq_subscriber.h"
#include "csr_hci_lib.h"
#include "csr_util.h"
#include "csr_transport.h"
#include "csr_tm_bluecore_prim.h"
#include "csr_tm_bluecore_lib.h"

#include "csr_log_text_2.h"
#include "csr_log_gsp.h"

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DEFINE(CsrHqLto);

void CsrHqInit(void **gash)
{
    CsrHqInstanceData *hqInst;

    hqInst = *gash = CsrPmemAlloc(sizeof(CsrHqInstanceData));

    CsrCmnListInit(&hqInst->subscribers, 0, NULL, NULL);
#ifdef CSR_BLUECORE_ONOFF
    hqInst->deactivating = FALSE;
#endif

    CsrHciRegisterVendorSpecificEventHandlerReqSend(CSR_HQ_IFACEQUEUE, TRANSPORT_CHANNEL_HQ);
#ifdef CSR_BLUECORE_ONOFF
    CsrTmBlueCoreRegisterReqSend(CSR_HQ_IFACEQUEUE);
#endif

    CSR_LOG_TEXT_REGISTER(&CsrHqLto, "HQ", 0, NULL);
}

void CsrHqHandler(void **gash)
{
    CsrHqInstanceData *hqInst = *gash;
    CsrUint16 eventClass;
    void *message;
    CsrPrim *primType;

    CsrSchedMessageGet(&eventClass, &message);
    primType = (CsrPrim *) message;

    switch (eventClass)
    {
        case CSR_HQ_PRIM:
        {
            if (*primType == CSR_HQ_REGISTER_REQ)
            {
                CsrHqRegisterReqHandler(hqInst, message);
            }
            else if (*primType == CSR_HQ_DEREGISTER_REQ)
            {
                CsrHqDeregisterReqHandler(hqInst, message);
            }
            else if (*primType == CSR_HQ_MSG_RES)
            {
                CsrHqMsgResHandler(hqInst, message);
            }
            else
            {
                CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrHqLto, 0, eventClass, *primType);
            }
            break;
        }
        case CSR_HCI_PRIM:
        {
            if (*primType == CSR_HCI_VENDOR_SPECIFIC_EVENT_IND)
            {
                CsrHqHciVendorSpecificEventIndHandler(hqInst, message);
            }
            else if (*primType == CSR_HCI_REGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_CFM)
            {
                /* No actions */
            }
            else
            {
                CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrHqLto, 0, eventClass, *primType);
            }
            break;
        }
#ifdef CSR_BLUECORE_ONOFF
        case CSR_TM_BLUECORE_PRIM:
        {
            if (*primType == CSR_TM_BLUECORE_REGISTER_CFM)
            {
                /* No actions */
            }
            else if (*primType == CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_IND)
            {
                CsrTmBluecoreActivateTransportResSend(CSR_HQ_IFACEQUEUE);
            }
            else if (*primType == CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_IND)
            {
                CsrTmBluecoreDeactivateTransportInd *indication = message;
                if (indication->active)
                {
                    hqInst->deactivating = TRUE;
                    CsrTmBluecoreDeactivateTransportResSend(CSR_HQ_IFACEQUEUE);
                }
                else
                {
                    hqInst->deactivating = FALSE;
                }
            }
            break;
        }
#endif
        case CSR_SCHED_PRIM:
        {
            if (*primType == CSR_CLEANUP_IND)
            {
                CsrCleanupInd *indication = message;
                CsrHqRemoveSubscriber(hqInst, indication->phandle);
            }
            else
            {
                CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrHqLto, 0, eventClass, *primType);
            }
            break;
        }
        default:
        {
            CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrHqLto, 0, eventClass, *primType);
            break;
        }
    }

    CsrPmemFree(message);
}

#ifdef ENABLE_SHUTDOWN
/****************************************************************************
    This function is called by the scheduler to perform a graceful shutdown
    of a scheduler task.
    This function must:
    1)    empty the input message queue and free any allocated memory in the
        messages.
    2)    free any instance data that may be allocated.
****************************************************************************/
void CsrHqDeinit(void **gash)
{
    CsrHqInstanceData *hqInst = *gash;
    CsrUint16 eventClass;
    void *message;

    CsrCmnListDeinit(&hqInst->subscribers);

    while (CsrSchedMessageGet(&eventClass, &message))
    {
        switch (eventClass)
        {
            case CSR_HQ_PRIM:
                CsrHqFreeDownstreamMessageContents(eventClass, message);
                break;
            case CSR_HCI_PRIM:
                CsrHciFreeUpstreamMessageContents(eventClass, message);
                break;
            default:
                break;
        }
        CsrPmemFree(message);
    }

    CsrPmemFree(hqInst);
}

#endif /* ENABLE_SHUTDOWN */
