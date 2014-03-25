/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_usr_config.h"
#include "csr_types.h"
#include "csr_bccmd_handler.h"
#include "csr_bccmd_sef.h"
#include "csr_bccmd_util.h"
#include "csr_tm_bluecore_lib.h"
#include "csr_tm_bluecore_task.h"
#include "csr_hci_lib.h"
#include "csr_transport.h"
#ifdef CSR_CHIP_MANAGER_ENABLE
#include "csr_bccmd_cm.h"
#endif

#include "csr_log_text_2.h"

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DEFINE(CsrBccmdLto);

typedef void (*bccmdStateHandler)(CsrBccmdInstanceData *bccmdInst);
static const bccmdStateHandler bccmdHandlers[TOTAL_NUM_OF_CSR_BCCMD_DOWNSTREAM_PRIMS] =
{
    CsrBccmdReqHandler,             /* CSR_BCCMD_REQ                */
    CsrBccmdFlushReqHandler,        /* CSR_BCCMD_FLUSH_REQ          */
    CsrBccmdBootstrapReqHandler,    /* CSR_BCCMD_BOOTSTRAP_REQ      */
    CsrBccmdResetResHandler,        /* CSR_BCCMD_RESET_RES          */
    CsrBccmdGetBuildIdHandler,      /* CSR_BCCMD_GET_BUILD_ID_REQ   */
#ifdef CSR_CHIP_MANAGER_QUERY_ENABLE
    CsrBccmdQueryReqHandler,        /* CSR_BCCMD_QUERY_REQ          */
#else
    NULL,
#endif
#ifdef CSR_CHIP_MANAGER_ENABLE
    CsrBccmdPanicArgsHandler,       /* CSR_BCCMD_PANIC_ARGS_REQ     */
#else
    NULL,
#endif
};

void CsrBccmdInit(void **gash)
{
    CsrBccmdInstanceData *bccmdInst;

    *gash = (void *) CsrPmemAlloc(sizeof(CsrBccmdInstanceData));
    bccmdInst = (CsrBccmdInstanceData *) *gash;
    bccmdInst->phandle = CSR_SCHED_QID_INVALID;
    bccmdInst->inProgress = FALSE;
    bccmdInst->state = CSR_BCCMD_STATE_ACTIVATING_TRANSPORT;
    bccmdInst->bootVar = NULL;
    bccmdInst->messageQueue = NULL;
    bccmdInst->recvMsgP = NULL;
#ifdef CSR_BLUECORE_ONOFF
    bccmdInst->timerId = CSR_SCHED_TID_INVALID;
#endif
#ifdef CSR_CHIP_MANAGER_ENABLE
    CsrBccmdCmInit(bccmdInst);
    bccmdInst->panicStatus = CSR_RESULT_SUCCESS;
    bccmdInst->panicArg = 0;
#endif

    CsrHciRegisterVendorSpecificEventHandlerReqSend(CSR_BCCMD_IFACEQUEUE, TRANSPORT_CHANNEL_BCCMD);

#ifdef CSR_BLUECORE_ONOFF
    CsrTmBlueCoreRegisterReqSend(CSR_BCCMD_IFACEQUEUE);
#else
    CsrTmBlueCoreActivateTransportReqSend(CSR_BCCMD_IFACEQUEUE);
#endif

#ifdef CSR_CHIP_MANAGER_ENABLE
    CsrTmBlueCoreCmReplayRegisterReqSend(CSR_BCCMD_IFACEQUEUE);
#endif

    CSR_LOG_TEXT_REGISTER(&CsrBccmdLto, "BCCMD", 0, NULL);
}

void CsrBccmdHandler(void **gash)
{
    CsrUint16 eventClass;
    CsrPrim *primType;
    CsrBccmdInstanceData *bccmdInst = (CsrBccmdInstanceData *) (*gash);

    CsrSchedMessageGet(&eventClass, &(bccmdInst->recvMsgP));
    primType = (CsrPrim *) bccmdInst->recvMsgP;

    switch (eventClass)
    {
        case CSR_BCCMD_PRIM:
        {
            if ((*primType < TOTAL_NUM_OF_CSR_BCCMD_DOWNSTREAM_PRIMS) && (bccmdHandlers[*primType] != NULL))
            {
                bccmdHandlers[*primType](bccmdInst);
            }
            else
            {
                CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrBccmdLto, 0, eventClass, *primType);
            }
            break;
        }
        case CSR_HCI_PRIM:
        {
            if (*primType == CSR_HCI_VENDOR_SPECIFIC_EVENT_IND)
            {
                if (CsrBccmdHciVendorSpecificEventIndHandler(bccmdInst))
                {
                    CsrBccmdRestoreMessageHandler(bccmdInst);
                }
            }
            else if (*primType == CSR_HCI_REGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_CFM)
            {
            }
            else
            {
                CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrBccmdLto, 0, eventClass, *primType);
            }
            break;
        }
        case CSR_TM_BLUECORE_PRIM:
        {
#ifdef CSR_BLUECORE_ONOFF
            if (*primType == CSR_TM_BLUECORE_REGISTER_CFM)
            {
                /* No need to do anything */
            }
            else if (*primType == CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_IND)
            {
                bccmdInst->state = CSR_BCCMD_STATE_TRANSPORT_ACTIVATED;
                CsrBccmdRestoreMessageHandler(bccmdInst);
                CsrTmBluecoreActivateTransportResSend(CSR_BCCMD_IFACEQUEUE);
            }
            else if (*primType == CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_IND)
            {
                CsrTmBluecoreDeactivateTransportInd *indication = bccmdInst->recvMsgP;
                if (indication->active)
                {
                    /* Transport has already been stopped (by TM), make sure
                       that no requests are handled. */
                    bccmdInst->state = CSR_BCCMD_STATE_DEACTIVATING_TRANSPORT;

                    /* Remove any state pertaining to a pending BCCMD (any
                       response is discarded on reception) */
                    CsrSchedTimerCancel(bccmdInst->timerId, NULL, NULL);
                    bccmdInst->timerId = CSR_SCHED_TID_INVALID;
                    bccmdInst->inProgress = FALSE;

                    CsrTmBluecoreDeactivateTransportResSend(CSR_BCCMD_IFACEQUEUE);
                }
                else
                {
                    /* Discard any saved messages, so they do not linger and
                       get sent when activated */
                    CsrBccmdDiscardSavedMessages(bccmdInst);
                    bccmdInst->state = CSR_BCCMD_STATE_ACTIVATING_TRANSPORT;
                }
            }
#else
            if (*primType == CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_CFM)
            {
                CsrTmBlueCoreActivateTransportCfmHandler(bccmdInst);
            }
#endif
#ifdef CSR_CHIP_MANAGER_ENABLE
            else if (*primType == CSR_TM_BLUECORE_CM_REPLAY_REGISTER_CFM)
            {
                CsrTmBlueCoreCmReplayRegisterCfmHandler(bccmdInst);
            }
            else if (*primType == CSR_TM_BLUECORE_CM_REPLAY_START_IND)
            {
                CsrTmBlueCoreCmReplayStartIndHandler(bccmdInst);
            }
            else if (*primType == CSR_TM_BLUECORE_CM_STATUS_RESET_IND)
            {
                CsrTmBlueCoreCmStatusResetIndHandler(bccmdInst);
            }
#endif
            else
            {
                CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrBccmdLto, 0, eventClass, *primType);
            }
            break;
        }
        case CSR_SCHED_PRIM:
        {
            break;
        }
        default:
        {
            CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrBccmdLto, 0, eventClass, *primType);
            break;
        }
    }
    CsrPmemFree(bccmdInst->recvMsgP);
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
void CsrBccmdDeinit(void **gash)
{
    CsrUint16 eventClass;
    CsrBool lastMsg = FALSE;
    CsrBccmdInstanceData *bccmdInst = (CsrBccmdInstanceData *) (*gash);

    while (!lastMsg)
    {
        if (!CsrMessageQueuePop(&bccmdInst->messageQueue, &eventClass, &(bccmdInst->recvMsgP)))
        {
            lastMsg = (CsrBool) (!CsrSchedMessageGet(&eventClass, &(bccmdInst->recvMsgP)));
        }
        if (!lastMsg)
        {
            CsrPrim *primType = (CsrPrim *) bccmdInst->recvMsgP;

            switch (eventClass)
            {
                case CSR_BCCMD_PRIM:
                {
                    if (*primType == CSR_BCCMD_REQ)
                    {
                        CsrBccmdReq *prim = (CsrBccmdReq *) bccmdInst->recvMsgP;
                        CsrPmemFree(prim->payload);
                    }
                    else if (*primType == CSR_BCCMD_BOOTSTRAP_REQ)
                    {
                        CsrBccmdBootstrapReq *prim = (CsrBccmdBootstrapReq *) bccmdInst->recvMsgP;
                        CsrPmemFree(prim->payload);
                    }
                    break;
                }
                case CSR_HCI_PRIM:
                {
                    CsrHciFreeUpstreamMessageContents(CSR_HCI_PRIM, bccmdInst->recvMsgP);
                    break;
                }
                case CSR_TM_BLUECORE_PRIM:
                {
                    /* Free function not available (no contents in any received messages) */
                    break;
                }
                default:
                    break;
            }
            CsrPmemFree(bccmdInst->recvMsgP);
        }
    }
    CsrBccmdRemoveBootType(&(bccmdInst->bootVar));
#ifdef CSR_CHIP_MANAGER_ENABLE
    CsrBccmdCmDeinit(bccmdInst);
#endif
    CsrPmemFree(bccmdInst);
}

#endif /* ENABLE_SHUTDOWN */
