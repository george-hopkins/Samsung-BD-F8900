/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_usr_config.h"
#include "csr_types.h"

#include "csr_am_handler.h"
#include "csr_am_sef.h"
#include "csr_am_util.h"
#include "csr_am_task.h"

#include "csr_bccmd_prim.h"
#include "csr_bccmd_lib.h"
#include "csr_bccmd_task.h"

#include "csr_log_text_2.h"

#if defined(CSR_CHIP_MANAGER_ENABLE) || defined(CSR_BLUECORE_ONOFF)
#include "csr_tm_bluecore_lib.h"
#include "csr_tm_bluecore_prim.h"
#endif

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DEFINE(CsrAmLto);

/* AM jump table */
static const AmHandlerType amHandlers[][CSR_AM_PRIM_DOWNSTREAM_COUNT] =
{
    /* AM_INIT_STATE */
    {
        CsrAmSaveMessage,                                        /* CSR_AM_INIT_REQ */
        CsrAmSaveMessage,                                        /* CSR_AM_AUDIO_PATH_CONNECT_REQ */
        CsrAmSaveMessage,                                        /* CSR_AM_AUDIO_PATH_ADD_SINK_REQ */
        CsrAmSaveMessage,                                        /* CSR_AM_AUDIO_PATH_REMOVE_SINK_REQ */
        CsrAmSaveMessage,                                        /* CSR_AM_AUDIO_PATH_CONFIG_REQ */
        CsrAmSaveMessage,                                        /* CSR_AM_AUDIO_PATH_RELEASE_REQ */
    },
    /* AM_IDLE_STATE */
    {
        CsrAmInitReqHandler,                                     /* CSR_AM_INIT_REQ */
        CsrAmAudioPathConnectReqHandler,                         /* CSR_AM_AUDIO_PATH_CONNECT_REQ */
        CsrAmAudioPathAddSinkReqHandler,                         /* CSR_AM_AUDIO_PATH_ADD_SINK_REQ */
        CsrAmAudioPathRemoveSinkReqHandler,                      /* CSR_AM_AUDIO_PATH_REMOVE_SINK_REQ */
        CsrAmAudioPathConfigReqHandler,                          /* CSR_AM_AUDIO_PATH_CONFIG_REQ */
        CsrAmAudioPathReleaseReqHandler,                         /* CSR_AM_AUDIO_PATH_RELEASE_REQ */
    },
    /* AM_BUSY_STATE */
    {
        CsrAmInitReqBusyHandler,                                 /* CSR_AM_INIT_REQ */
        CsrAmSaveMessage,                                        /* CSR_AM_AUDIO_PATH_CONNECT_REQ */
        CsrAmSaveMessage,                                        /* CSR_AM_AUDIO_PATH_ADD_SINK_REQ */
        CsrAmSaveMessage,                                        /* CSR_AM_AUDIO_PATH_REMOVE_SINK_REQ */
        CsrAmSaveMessage,                                        /* CSR_AM_AUDIO_PATH_CONFIG_REQ */
        CsrAmSaveMessage,                                        /* CSR_AM_AUDIO_PATH_RELEASE_REQ */
    },
#ifdef CSR_BLUECORE_ONOFF
    /* AM_DEACTIVATING_STATE */
    {
        CsrAmDiscardMessage,                                     /* CSR_AM_INIT_REQ */
        CsrAmDiscardMessage,                                     /* CSR_AM_AUDIO_PATH_CONNECT_REQ */
        CsrAmDiscardMessage,                                     /* CSR_AM_AUDIO_PATH_ADD_SINK_REQ */
        CsrAmDiscardMessage,                                     /* CSR_AM_AUDIO_PATH_REMOVE_SINK_REQ */
        CsrAmDiscardMessage,                                     /* CSR_AM_AUDIO_PATH_CONFIG_REQ */
        CsrAmDiscardMessage,                                     /* CSR_AM_AUDIO_PATH_RELEASE_REQ */
    },
#endif
};

#ifdef CSR_CHIP_MANAGER_ENABLE
static const AmHandlerType csrAmTmHandlers[CSR_TM_PRIM_UPSTREAM_COUNT] =
{
    NULL,                           /* CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_CFM    */
    NULL,                           /* CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_CFM  */
    NULL,                           /* CSR_TM_BLUECORE_CM_STATUS_SUBSCRIBE_CFM   */
    NULL,                           /* CSR_TM_BLUECORE_CM_STATUS_UNSUBSCRIBE_CFM */
    CsrAmResetIndHandler,           /* CSR_TM_BLUECORE_CM_STATUS_RESET_IND       */
    NULL,                           /* CSR_TM_BLUECORE_CM_STATUS_PANIC_IND       */
    CsrAmRestartIndHandler,         /* CSR_TM_BLUECORE_CM_STATUS_RESTART_IND     */
    NULL,                           /* CSR_TM_BLUECORE_CM_STATUS_REPLAY_ERROR_IND*/
    NULL,                           /* CSR_TM_BLUECORE_CM_STATUS_PANIC_ARGS_IND  */
    NULL,                           /* CSR_TM_BLUECORE_CM_PING_INTERVAL_SET_CFM  */
    NULL,                           /* CSR_TM_BLUECORE_CM_REPLAY_REGISTER_CFM    */
    NULL,                           /* CSR_TM_BLUECORE_CM_REPLAY_START_IND       */
};
#endif

static void initInst(AmInstanceDataType *inst)
{
    inst->currentAppHandle = CSR_SCHED_QID_INVALID;
    inst->rxMsgP = NULL;
    inst->rxDownstreamMsgP = NULL;
    inst->chipVer = AM_VALUE_UNKNOWN;
    inst->buildId = AM_VALUE_UNKNOWN;
    inst->requestType = AM_PRIM_ID_NONE;
    inst->saveQueue = NULL;
    inst->currentStream = NULL;
    CsrCmnListInit(&(inst->streamList), 0, NULL, NULL);
    inst->amCallback = NULL;
    inst->connId = STREAM_ID_NONE;
    inst->sinksToAdd = NULL;
    inst->streamTransSupported = TRUE;
    CsrCmnListInit(&(inst->newSinkList), 0, NULL, NULL);
}

/* Init function */
void CsrAmInit(void **gash)
{
    AmInstanceDataType *inst;

    inst = *gash = CsrPmemZalloc(sizeof(AmInstanceDataType));
    initInst(inst);

#ifdef CSR_CHIP_MANAGER_ENABLE
    CsrTmBlueCoreCmStatusSubscribeReqSend(CSR_AM_IFACEQUEUE);
#endif
#ifdef CSR_BLUECORE_ONOFF
    CsrTmBlueCoreRegisterReqSend(CSR_AM_IFACEQUEUE);
#else
    CsrAmIssueAmGetChipVer();
    inst->amCallback = CsrAmGetChipVersionCfm;
#endif

    CSR_LOG_TEXT_REGISTER(&CsrAmLto, "AM", 0, NULL);
}

/* CSR AM main handler function */
void CsrAmHandler(void **gash)
{
    AmInstanceDataType *inst;
    CsrUint16 event;
    CsrPrim *primType;
    void *msg;

    inst = (AmInstanceDataType *) (*gash);
    msg = NULL;
    event = 0;

    /* Pop from save queue if ready for it */
    if (inst->restoreFlag)
    {
        if (!CsrMessageQueuePop(&inst->saveQueue, &event, &msg))
        {
            inst->restoreFlag = FALSE;
            msg = NULL;
        }
    }

    /* Either not ready or no messages queued: get from scheduler */
    if (msg == NULL)
    {
        CsrSchedMessageGet(&event, &msg);
    }
    inst->msgClass = event;
    inst->rxMsgP = msg;
    primType = msg;

    switch (event)
    {
        case CSR_AM_PRIM:
        {
            if (inst->rxMsgP == NULL)
            {
                break;
            }

            /* find the message type */
            if ((*primType < CSR_AM_PRIM_DOWNSTREAM_COUNT) &&
                (amHandlers[inst->myState][*primType] != NULL))
            {
                if (inst->myState == AM_IDLE_STATE)
                {
                    inst->rxDownstreamMsgP = inst->rxMsgP;
                    inst->rxMsgP = NULL;
                }
                amHandlers[inst->myState][*primType](inst);
            }
            else
            {
                CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrAmLto, 0, event, *primType);
            }
            break;
        }
        case CSR_BCCMD_PRIM:
        {
            CsrBccmdCfm *prim = (CsrBccmdCfm *) msg;
            if (prim->type == CSR_BCCMD_CFM)
            {
                if (inst->amCallback != NULL)
                {
                    inst->amCallback(inst);
                }
                CsrBccmdFreeUpstreamMessageContents(CSR_BCCMD_PRIM, msg);
            }
            else
            {
                CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrAmLto, 0, event, prim->type);
            }
            break;
        }
#ifdef CSR_CHIP_MANAGER_ENABLE
        case CSR_TM_BLUECORE_PRIM:
        {
            CsrTmBlueCorePrim *tmPrim = (CsrTmBlueCorePrim *) msg;
            if (csrAmTmHandlers[(CsrUint16) (*tmPrim - CSR_TM_PRIM_UPSTREAM_LOWEST)] != NULL)
            {
                csrAmTmHandlers[(CsrUint16) (*tmPrim - CSR_TM_PRIM_UPSTREAM_LOWEST)](inst);
            }
            break;
        }
#endif
#ifdef CSR_BLUECORE_ONOFF
        case CSR_TM_BLUECORE_PRIM:
        {
            if (*primType == CSR_TM_BLUECORE_REGISTER_CFM)
            {
                /* No need to do anything */
            }
            else if (*primType == CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_IND)
            {
                CsrAmIssueAmGetChipVer();
                inst->amCallback = CsrAmGetChipVersionCfm;
                CsrTmBluecoreActivateTransportResSend(CSR_AM_IFACEQUEUE);
            }
            else if (*primType == CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_IND)
            {
                CsrTmBluecoreDeactivateTransportInd *indication = inst->rxMsgP;
                if (indication->active)
                {
                    /* Block BCCMD confirms */
                    inst->amCallback = NULL;

                    /* Deallocate all state */
                    CsrAmFreeOperationMessageAndQueuedMessages(inst);
                    CsrAmFreeStreamListAndNewSinkList(inst);

                    /* Reset instance data */
                    CsrMemSet(inst, 0, sizeof(AmInstanceDataType));
                    initInst(inst);

                    /* Transport has already been stopped (by TM), make sure
                       that no requests are handled. */
                    inst->myState = AM_DEACTIVATING_STATE;

                    CsrTmBluecoreDeactivateTransportResSend(CSR_AM_IFACEQUEUE);
                }
                else
                {
                    inst->myState = AM_INIT_STATE;
                }
            }
            else
            {
                CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrAmLto, 0, event, *primType);
            }
            break;
        }
#endif
        default:
        {
            CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrAmLto, 0, event, 0);
            break;
        }
    }

    CsrPmemFree(inst->rxMsgP);
    inst->rxMsgP = NULL;
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
void CsrAmDeinit(void **gash)
{
    AmInstanceDataType *inst = *gash;
    CsrUint16 event;
    void *message;

    CsrAmFreeOperationMessageAndQueuedMessages(inst);

    while (CsrSchedMessageGet(&event, &message))
    {
        switch (event)
        {
            case CSR_AM_PRIM:
                if (message != NULL)
                {
                    CsrAmFreeDownstreamMessageContents(CSR_AM_PRIM, message);
                }
                break;
            case CSR_BCCMD_PRIM:
                CsrBccmdFreeUpstreamMessageContents(CSR_BCCMD_PRIM, message);
                break;
#ifdef CSR_CHIP_MANAGER_ENABLE
            case CSR_TM_BLUECORE_PRIM:
                /* Free function not available (no contents in any received messages) */
                break;
#endif
            default:
                message = NULL;
                break;
        }
        CsrPmemFree(message);
    }

    CsrAmFreeStreamListAndNewSinkList(inst);

    CsrPmemFree(inst);
}

#endif

void CsrAmChangeState(CsrCharString *file, CsrUint32 lineno, CsrUint32 *state, CsrCharString *stateName, CsrUint32 newState, CsrCharString *newStateName)
{
#ifdef CSR_LOG_ENABLE
    CsrLogStateTransition((CSR_LOG_STATE_TRANSITION_MASK_FSM_NAME |
                           CSR_LOG_STATE_TRANSITION_MASK_NEXT_STATE |
                           CSR_LOG_STATE_TRANSITION_MASK_NEXT_STATE_STR |
                           CSR_LOG_STATE_TRANSITION_MASK_PREV_STATE),
        0,
        stateName,
        *state,
        NULL,
        0,
        NULL,
        newState,
        newStateName,
        lineno,
        file);
#endif

    *state = newState;
}
