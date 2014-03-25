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
#include "csr_hci_task.h"
#include "csr_hci_prim.h"
#include "csr_hci_sef.h"
#include "csr_hci_util.h"
#include "csr_tm_bluecore_lib.h"
#include "csr_hci_sco.h"
#include "csr_log_text_2.h"

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DEFINE(CsrHciLto);

#ifdef CSR_BLUECORE_ONOFF
/* - 1 below because we don't process CSR_HCI_INCOMING_DATA_REQ */
static const CsrHciEventType csrHciMsgHandlers[CSR_HCI_PRIM_DOWNSTREAM_COUNT - 1] =
{
    CsrHciRegisterEventHandlerReqHandler,                 /* CSR_HCI_REGISTER_EVENT_HANDLER_REQ */
    CsrHciRegisterAclHandlerReqHandler,                   /* CSR_HCI_REGISTER_ACL_HANDLER_REQ */
    CsrHciUnregisterAclHandlerReqHandler,                 /* CSR_HCI_UNREGISTER_ACL_HANDLER_REQ */
    CsrHciRegisterScoHandlerReqHandler,                   /* CSR_HCI_REGISTER_SCO_HANDLER_REQ */
    CsrHciUnregisterScoHandlerReqHandler,                 /* CSR_HCI_UNREGISTER_SCO_HANDLER_REQ */
    CsrHciRegisterVendorSpecificEventHandlerReqHandler,   /* CSR_HCI_REGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_REQ */
    CsrHciUnregisterVendorSpecificEventHandlerReqHandler, /* CSR_HCI_UNREGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_REQ */
    CsrHciCommandReqHandler,                              /* CSR_HCI_COMMAND_REQ */
    CsrHciAclDataReqHandler,                              /* CSR_HCI_ACL_DATA_REQ */
    CsrHciScoDataReqHandler,                              /* CSR_HCI_SCO_DATA_REQ */
    CsrHciVendorSpecificCommandReqHandler,                /* CSR_HCI_VENDOR_SPECIFIC_COMMAND_REQ */
};
#else
/* - 1 below because we don't process CSR_HCI_INCOMING_DATA_REQ */
static const CsrHciEventType csrHciMsgHandlers[][CSR_HCI_PRIM_DOWNSTREAM_COUNT - 1] =
{
    { /* Jump tabel to handle downstream CSR_HCI messages send from the upper layers                                  */
        CsrHciRegisterEventHandlerReqHandler,                 /* CSR_HCI_REGISTER_EVENT_HANDLER_REQ                   */
        CsrHciSaveMessage,                                    /* CSR_HCI_REGISTER_ACL_HANDLER_REQ                     */
        CsrHciSaveMessage,                                    /* CSR_HCI_UNREGISTER_ACL_HANDLER_REQ                   */
        CsrHciSaveMessage,                                    /* CSR_HCI_REGISTER_SCO_HANDLER_REQ                     */
        CsrHciSaveMessage,                                    /* CSR_HCI_UNREGISTER_SCO_HANDLER_REQ                   */
        CsrHciRegisterVendorSpecificEventHandlerReqHandler,   /* CSR_HCI_REGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_REQ   */
        CsrHciSaveMessage,                                    /* CSR_HCI_UNREGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_REQ */
        CsrHciSaveMessage,                                    /* CSR_HCI_COMMAND_REQ                                  */
        CsrHciSaveMessage,                                    /* CSR_HCI_ACL_DATA_REQ                                 */
        CsrHciSaveMessage,                                    /* CSR_HCI_SCO_DATA_REQ                                 */
        CsrHciVendorSpecificCommandReqHandler,                /* CSR_HCI_VENDOR_SPECIFIC_COMMAND_REQ                  */
    },
    { /* Jump tabel to handle downstream CSR_HCI messages send from the upper layers                                  */
        CsrHciRegisterEventHandlerReqHandler,                 /* CSR_HCI_REGISTER_EVENT_HANDLER_REQ                   */
        CsrHciRegisterAclHandlerReqHandler,                   /* CSR_HCI_REGISTER_ACL_HANDLER_REQ                     */
        CsrHciUnregisterAclHandlerReqHandler,                 /* CSR_HCI_UNREGISTER_ACL_HANDLER_REQ                   */
        CsrHciRegisterScoHandlerReqHandler,                   /* CSR_HCI_REGISTER_SCO_HANDLER_REQ                     */
        CsrHciUnregisterScoHandlerReqHandler,                 /* CSR_HCI_UNREGISTER_SCO_HANDLER_REQ                   */
        CsrHciRegisterVendorSpecificEventHandlerReqHandler,   /* CSR_HCI_REGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_REQ   */
        CsrHciUnregisterVendorSpecificEventHandlerReqHandler, /* CSR_HCI_UNREGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_REQ */
        CsrHciCommandReqHandler,                              /* CSR_HCI_COMMAND_REQ                                  */
        CsrHciAclDataReqHandler,                              /* CSR_HCI_ACL_DATA_REQ                                 */
        CsrHciScoDataReqHandler,                              /* CSR_HCI_SCO_DATA_REQ                                 */
        CsrHciVendorSpecificCommandReqHandler,                /* CSR_HCI_VENDOR_SPECIFIC_COMMAND_REQ                  */
    }
};
#endif

void CsrHciInit(void **gash)
{
    CsrHciInstData *inst = *gash = CsrPmemAlloc(sizeof(CsrHciInstData));

    inst->state = CSR_HCI_STATE_IDLE;
    inst->hciEventHandler = CSR_SCHED_QID_INVALID;
    inst->hciAclMainHandler = CSR_SCHED_QID_INVALID;
    inst->hciScoHandler = NULL;
    inst->hciAclHandler = NULL;
    inst->hciVendorEventHandler = NULL;
#ifndef CSR_BLUECORE_ONOFF
    inst->saveQueue = NULL;
    inst->restoreFlag = FALSE;
#endif
    inst->recvMsgP = NULL;

    CsrHciInitScoHandle();

    CsrHciInstanceRegister(inst);

#ifdef CSR_BLUECORE_ONOFF
    CsrTmBlueCoreRegisterReqSend(CSR_HCI_IFACEQUEUE);
#else
    CsrTmBlueCoreActivateTransportReqSend(CSR_HCI_IFACEQUEUE);
#endif

    CSR_LOG_TEXT_REGISTER(&CsrHciLto, "HCI", 0, NULL);
}

#ifdef ENABLE_SHUTDOWN
void CsrHciDeinit(void **gash)
{
    CsrHciInstData *inst = *gash;
    CsrUint16 event;

    CsrHciUnregisterAllHandles(&inst->hciScoHandler);
    CsrHciUnregisterAllHandles(&inst->hciAclHandler);
    CsrHciUnregisterAllHandles(&inst->hciVendorEventHandler);
#ifndef CSR_BLUECORE_ONOFF
    CsrHciDiscardSavedMessages(inst);
#endif

    /* Clean own queue */
    while (CsrSchedMessageGet(&event, &inst->recvMsgP))
    {
        switch (event)
        {
            case CSR_TM_BLUECORE_PRIM:
                /* No allocated content in upstream messages */
                break;
            case CSR_HCI_PRIM:
                CsrHciFreeDownstreamMessageContents(CSR_HCI_PRIM, inst->recvMsgP);
                break;
            default:
                CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrHciLto, 0, event, 0);
                break;
        }
        CsrPmemFree(inst->recvMsgP);
    }

    CsrPmemFree(inst);
}

#endif /* ENABLE_SHUTDOWN */

void CsrHciHandler(void **gash)
{
    CsrHciInstData *inst = *gash;
    CsrUint16 event;

#ifndef CSR_BLUECORE_ONOFF
    if (inst->restoreFlag)
    {
        if (!CsrMessageQueuePop(&inst->saveQueue, &event, &inst->recvMsgP))
        {
            inst->restoreFlag = FALSE;
            CsrSchedMessageGet(&event, &inst->recvMsgP);
        }
    }
    else
#endif
    {
        CsrSchedMessageGet(&event, &inst->recvMsgP);
    }

    switch (event)
    {
        case CSR_TM_BLUECORE_PRIM:
        {
            CsrTmBlueCorePrim *primType = (CsrTmBlueCorePrim *) inst->recvMsgP;

#ifdef CSR_BLUECORE_ONOFF
            if (*primType == CSR_TM_BLUECORE_REGISTER_CFM)
            {
                /* No need to do anything */
            }
            else if (*primType == CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_IND)
            {
                inst->state = CSR_HCI_STATE_ACTIVATED;
                CsrTmBluecoreActivateTransportResSend(CSR_HCI_IFACEQUEUE);
            }
            else if (*primType == CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_IND)
            {
                CsrTmBluecoreDeactivateTransportInd *indication = inst->recvMsgP;
                if (indication->active)
                {
                    /* Transport has already been stopped (by TM), make sure
                       that nothing goes out */
                    inst->state = CSR_HCI_STATE_DEACTIVATING;

                    /* Remove all registrations for a specific SCO or ACL handle */
                    CsrHciUnregisterAllHandles(&inst->hciScoHandler);
                    CsrHciUnregisterAllHandles(&inst->hciAclHandler);

                    CsrTmBluecoreDeactivateTransportResSend(CSR_HCI_IFACEQUEUE);
                }
                else
                {
                    inst->state = CSR_HCI_STATE_IDLE;
                }
            }
#else
            if (*primType == CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_CFM)
            {
                CsrTmBluecoreActivateTransportCfm *prim = inst->recvMsgP;

                CSR_LOG_TEXT_CONDITIONAL_ERROR(prim->result != CSR_RESULT_SUCCESS,
                    (CsrHciLto, 0, "Unsuccessfull result in CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_CFM"));
                CSR_UNUSED(prim);

                CsrHciRestoreSavedMessages(inst);
            }
#endif
            else
            {
                CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrHciLto, 0, event, *primType);
            }
            break;
        }
        case CSR_HCI_PRIM:
        {
#ifndef CSR_BLUECORE_ONOFF
            if (inst->recvMsgP != NULL)
#endif
            {
                CsrHciPrim *primType = (CsrHciPrim *) inst->recvMsgP;
#ifdef CSR_BLUECORE_ONOFF
                if ((*primType < CSR_HCI_PRIM_DOWNSTREAM_COUNT) && (csrHciMsgHandlers[*primType] != NULL))
                {
                    csrHciMsgHandlers[*primType](inst);
                }
#else
                if ((*primType < CSR_HCI_PRIM_DOWNSTREAM_COUNT) && (csrHciMsgHandlers[inst->state][*primType] != NULL))
                {
                    csrHciMsgHandlers[inst->state][*primType](inst);
                }
#endif
                else
                {
                    CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrHciLto, 0, event, *primType);
                }
            }
            break;
        }
        default:
        {
            CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrHciLto, 0, event, 0);
            break;
        }
    }

    CsrPmemFree(inst->recvMsgP);
    inst->recvMsgP = NULL;
}
