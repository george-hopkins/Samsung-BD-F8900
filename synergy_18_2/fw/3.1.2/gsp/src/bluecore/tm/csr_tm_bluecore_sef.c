/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_usr_config.h"
#include "csr_types.h"
#include "csr_util.h"
#include "csr_list.h"
#include "csr_transport.h"
#include "csr_log_gsp.h"
#include "csr_message_queue.h"
#include "csr_log_text_2.h"
#include "csr_tm_bluecore_sef.h"
#include "csr_tm_bluecore_handler.h"
#include "csr_tm_bluecore_prim.h"
#include "csr_tm_bluecore_bootstrap.h"
#include "csr_tm_bluecore_transport.h"
#include "csr_tm_bluecore_private_prim.h"
#include "csr_tm_bluecore_private_lib.h"
#include "csr_bccmd_private_lib.h"
#include "csr_bccmd_lib.h"
#include "csr_hci_private_prim.h"

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DEFINE(CsrTmBluecoreLto);

#define CSR_TM_BLUECORE_BCCMD_MIN_SIZE        ((CsrUint16) 0x0012)

extern void CsrHciSendEventInd(CsrSchedQid phandle, CsrUint8 *payload, CsrUint16 length);

#ifdef CSR_BLUECORE_ONOFF
static void saveMessage(CsrTmBlueCoreInstanceData *tmBcInst)
{
    CsrMessageQueuePush(&tmBcInst->saveQueue, CSR_TM_BLUECORE_PRIM, tmBcInst->recvMsgP);
    tmBcInst->recvMsgP = NULL;
}

static void restoreSavedMessages(CsrTmBlueCoreInstanceData *tmBcInst)
{
    CsrMessageQueueRestore(&tmBcInst->saveQueue, CSR_TM_BLUECORE_IFACEQUEUE);
}

#endif

#if defined(CSR_CHIP_MANAGER_ENABLE) || defined(CSR_BLUECORE_ONOFF)
static CsrBool csrTmBlueCoreHandleListCompareI(CsrCmnListElm_t *elem, void *data)
{
    CsrTmBlueCoreHandleList *element = (CsrTmBlueCoreHandleList *) elem;
    CsrSchedQid phandle = *(CsrSchedQid *) data;
    return (element->phandle == phandle) ? TRUE : FALSE;
}

#endif

#ifdef CSR_CHIP_MANAGER_ENABLE
static void csrTmBlueCoreCmStatusSubscribeCfmSend(CsrSchedQid phandle)
{
    CsrTmBluecoreCmStatusSubscribeCfm *prim;
    prim = (CsrTmBluecoreCmStatusSubscribeCfm *) CsrPmemAlloc(sizeof(CsrTmBluecoreCmStatusSubscribeCfm));
    prim->type = CSR_TM_BLUECORE_CM_STATUS_SUBSCRIBE_CFM;
    prim->result = CSR_RESULT_SUCCESS;
    CsrSchedMessagePut(phandle, CSR_TM_BLUECORE_PRIM, prim);
}

static void csrTmBlueCoreCmStatusSubscribeReqHandler(CsrTmBlueCoreInstanceData *tmBcInst)
{
    CsrTmBluecoreCmStatusSubscribeReq *prim = (CsrTmBluecoreCmStatusSubscribeReq *) tmBcInst->recvMsgP;
    CsrTmBlueCoreHandleList *elem = CSR_TM_BLUECORE_LIST_ADD(&tmBcInst->cmStatusSubscribers);
    elem->phandle = prim->phandle;
    csrTmBlueCoreCmStatusSubscribeCfmSend(prim->phandle);
}

static void csrTmBlueCoreCmStatusUnsubscribeCfmSend(CsrSchedQid phandle)
{
    CsrTmBluecoreCmStatusUnsubscribeCfm *prim;
    prim = (CsrTmBluecoreCmStatusUnsubscribeCfm *) CsrPmemAlloc(sizeof(CsrTmBluecoreCmStatusUnsubscribeCfm));
    prim->type = CSR_TM_BLUECORE_CM_STATUS_UNSUBSCRIBE_CFM;
    prim->result = CSR_RESULT_SUCCESS;
    CsrSchedMessagePut(phandle, CSR_TM_BLUECORE_PRIM, prim);
}

static void csrTmBlueCoreCmStatusUnsubscribeReqHandler(CsrTmBlueCoreInstanceData *tmBcInst)
{
    CsrTmBluecoreCmStatusUnsubscribeReq *prim = (CsrTmBluecoreCmStatusUnsubscribeReq *) tmBcInst->recvMsgP;
    CSR_TM_BLUECORE_LIST_ITERATE_AND_REMOVE(&tmBcInst->cmStatusSubscribers,
        csrTmBlueCoreHandleListCompareI,
        &prim->phandle);
    csrTmBlueCoreCmStatusUnsubscribeCfmSend(prim->phandle);
}

static void csrTmBlueCoreCmStatusResetIndSend(CsrSchedQid phandle)
{
    CsrTmBluecoreCmStatusResetInd *prim;
    prim = (CsrTmBluecoreCmStatusResetInd *) CsrPmemAlloc(sizeof(CsrTmBluecoreCmStatusResetInd));
    prim->type = CSR_TM_BLUECORE_CM_STATUS_RESET_IND;
    CsrSchedMessagePut(phandle, CSR_TM_BLUECORE_PRIM, prim);
}

static void csrTmBlueCoreCmStatusResetIndSendI(CsrCmnListElm_t *elem, void *data)
{
    CsrTmBlueCoreHandleList *element = (CsrTmBlueCoreHandleList *) elem;
    csrTmBlueCoreCmStatusResetIndSend(element->phandle);
    CSR_UNUSED(data);
}

static void csrTmBlueCoreCmStatusPanicIndSend(CsrSchedQid phandle)
{
    CsrTmBluecoreCmStatusPanicInd *prim;
    prim = (CsrTmBluecoreCmStatusPanicInd *) CsrPmemAlloc(sizeof(CsrTmBluecoreCmStatusPanicInd));
    prim->type = CSR_TM_BLUECORE_CM_STATUS_PANIC_IND;
    CsrSchedMessagePut(phandle, CSR_TM_BLUECORE_PRIM, prim);
}

static void csrTmBlueCoreCmStatusPanicIndSendI(CsrCmnListElm_t *elem, void *data)
{
    CsrTmBlueCoreHandleList *element = (CsrTmBlueCoreHandleList *) elem;
    csrTmBlueCoreCmStatusPanicIndSend(element->phandle);
    CSR_UNUSED(data);
}

static void csrTmBlueCoreCmStatusRestartIndSend(CsrSchedQid phandle)
{
    CsrTmBluecoreCmStatusRestartInd *prim;
    prim = (CsrTmBluecoreCmStatusRestartInd *) CsrPmemAlloc(sizeof(CsrTmBluecoreCmStatusRestartInd));
    prim->type = CSR_TM_BLUECORE_CM_STATUS_RESTART_IND;
    CsrSchedMessagePut(phandle, CSR_TM_BLUECORE_PRIM, prim);
}

static void csrTmBlueCoreCmStatusRestartIndSendI(CsrCmnListElm_t *elem, void *data)
{
    CsrTmBlueCoreHandleList *element = (CsrTmBlueCoreHandleList *) elem;
    csrTmBlueCoreCmStatusRestartIndSend(element->phandle);
    CSR_UNUSED(data);
}

static void csrTmBlueCoreCmStatusReplayErrorIndSend(CsrSchedQid phandle, CsrUint16 hciOpcode)
{
    CsrTmBluecoreCmStatusReplayErrorInd *prim;
    prim = (CsrTmBluecoreCmStatusReplayErrorInd *) CsrPmemAlloc(sizeof(CsrTmBluecoreCmStatusReplayErrorInd));
    prim->type = CSR_TM_BLUECORE_CM_STATUS_REPLAY_ERROR_IND;
    prim->hciOpcode = hciOpcode;
    CsrSchedMessagePut(phandle, CSR_TM_BLUECORE_PRIM, prim);
}

static void csrTmBlueCoreCmStatusReplayErrorIndSendI(CsrCmnListElm_t *elem, void *data)
{
    CsrTmBlueCoreHandleList *element = (CsrTmBlueCoreHandleList *) elem;
    csrTmBlueCoreCmStatusReplayErrorIndSend(element->phandle, *(CsrUint16 *) data);
}

static void csrTmBlueCoreCmStatusPanicArgsIndSend(CsrSchedQid phandle, CsrBccmdPanicArgsCfm *panicArgsCfm)
{
    CsrTmBluecoreCmStatusPanicArgsInd *prim;
    prim = (CsrTmBluecoreCmStatusPanicArgsInd *) CsrPmemAlloc(sizeof(CsrTmBluecoreCmStatusPanicArgsInd));
    prim->type = CSR_TM_BLUECORE_CM_STATUS_PANIC_ARGS_IND;
    prim->panicStatus = panicArgsCfm->panicStatus;
    prim->panicArg = panicArgsCfm->panicArg;
    prim->faultStatus = panicArgsCfm->faultStatus;
    prim->faultArg = panicArgsCfm->faultArg;
    CsrSchedMessagePut(phandle, CSR_TM_BLUECORE_PRIM, prim);
}

static void csrTmBlueCoreCmStatusPanicArgsIndSendI(CsrCmnListElm_t *elem, void *data)
{
    CsrTmBlueCoreHandleList *element = (CsrTmBlueCoreHandleList *) elem;
    csrTmBlueCoreCmStatusPanicArgsIndSend(element->phandle, (CsrBccmdPanicArgsCfm *) data);
}

static void csrTmBlueCoreCmPingIntervalSetCfmSend(CsrSchedQid phandle)
{
    CsrTmBluecoreCmPingIntervalSetCfm *prim;
    prim = (CsrTmBluecoreCmPingIntervalSetCfm *) CsrPmemAlloc(sizeof(CsrTmBluecoreCmPingIntervalSetCfm));
    prim->type = CSR_TM_BLUECORE_CM_PING_INTERVAL_SET_CFM;
    prim->result = CSR_RESULT_SUCCESS;
    CsrSchedMessagePut(phandle, CSR_TM_BLUECORE_PRIM, prim);
}

static void csrTmBlueCorePingMsgSend(void) /* Ping bc using bccmd get panic_arg */
{
    CsrBccmdReadReqSend(CSR_TM_BLUECORE_IFACEQUEUE, CSR_BCCMD_PANIC_ARG_VARID, 0, 0, NULL);
}

static void csrTmBlueCorePingTimeoutHandler(CsrUint8 dummy, void *instData)
{
    CsrTmBlueCoreInstanceData *tmBcInst = (CsrTmBlueCoreInstanceData *) instData;
    tmBcInst->pingTimerId = CSR_SCHED_TID_INVALID;
    csrTmBlueCorePingMsgSend();
}

static void csrTmBlueCoreCmPingIntervalSetReqHandler(CsrTmBlueCoreInstanceData *tmBcInst)
{
    CsrTmBluecoreCmPingIntervalSetReq *prim = (CsrTmBluecoreCmPingIntervalSetReq *) tmBcInst->recvMsgP;
    tmBcInst->pingInterval = prim->interval;
    if (tmBcInst->pingTimerId != CSR_SCHED_TID_INVALID)
    {
        CsrSchedTimerCancel(tmBcInst->pingTimerId, NULL, NULL);
        if (tmBcInst->pingInterval != 0)
        {
            tmBcInst->pingTimerId = CsrSchedTimerSet(tmBcInst->pingInterval,
                (void (*)(CsrUint16, void *))csrTmBlueCorePingTimeoutHandler,
                (CsrUint16) 0,
                (void *) tmBcInst);
        }
    }
    csrTmBlueCoreCmPingIntervalSetCfmSend(prim->phandle);
}

static void csrTmBlueCoreCmReplayRegisterCfmSend(CsrSchedQid phandle)
{
    CsrTmBluecoreCmReplayRegisterCfm *prim;
    prim = (CsrTmBluecoreCmReplayRegisterCfm *) CsrPmemAlloc(sizeof(CsrTmBluecoreCmReplayRegisterCfm));
    prim->type = CSR_TM_BLUECORE_CM_REPLAY_REGISTER_CFM;
    prim->result = CSR_RESULT_SUCCESS;
    CsrSchedMessagePut(phandle, CSR_TM_BLUECORE_PRIM, prim);
}

static void csrTmBlueCoreCmReplayRegisterReqHandler(CsrTmBlueCoreInstanceData *tmBcInst)
{
    CsrTmBluecoreCmReplayRegisterReq *prim = (CsrTmBluecoreCmReplayRegisterReq *) tmBcInst->recvMsgP;
    CsrTmBlueCoreHandleList *elem = CSR_TM_BLUECORE_LIST_ADD(&tmBcInst->cmReplayersRegistered);
    elem->phandle = prim->phandle;
    csrTmBlueCoreCmReplayRegisterCfmSend(prim->phandle);
}

static void csrTmBlueCoreCmReplayStartIndSend(CsrSchedQid phandle)
{
    CsrTmBluecoreCmReplayStartInd *prim;
    prim = (CsrTmBluecoreCmReplayStartInd *) CsrPmemAlloc(sizeof(CsrTmBluecoreCmReplayStartInd));
    prim->type = CSR_TM_BLUECORE_CM_REPLAY_START_IND;
    CsrSchedMessagePut(phandle, CSR_TM_BLUECORE_PRIM, prim);
}

static void csrTmBlueCoreCmReplayersStartedAdd(CsrSchedQid phandle, CsrTmBlueCoreInstanceData *tmBcInst)
{
    CsrTmBlueCoreHandleList *element = CSR_TM_BLUECORE_LIST_ADD(&tmBcInst->cmReplayersStarted);
    element->phandle = phandle;
}

static void csrTmBlueCoreCmReplayStartIndSendI(CsrCmnListElm_t *elem, void *data)
{
    CsrTmBlueCoreHandleList *element = (CsrTmBlueCoreHandleList *) elem;
    csrTmBlueCoreCmReplayStartIndSend(element->phandle);
    csrTmBlueCoreCmReplayersStartedAdd(element->phandle, data);
    CSR_UNUSED(data);
}

static void csrTmBlueCoreCmReplayStartResHandler(CsrTmBlueCoreInstanceData *tmBcInst)
{
    CsrTmBluecoreCmReplayStartRes *prim = (CsrTmBluecoreCmReplayStartRes *) tmBcInst->recvMsgP;

    CSR_TM_BLUECORE_LIST_ITERATE_AND_REMOVE(&tmBcInst->cmReplayersStarted,
        csrTmBlueCoreHandleListCompareI,
        &prim->phandle);
    if (CSR_TM_BLUECORE_LIST_IS_EMPTY(&tmBcInst->cmReplayersStarted))
    {
        CSR_TM_BLUECORE_LIST_ITERATE(&tmBcInst->cmStatusSubscribers,
            csrTmBlueCoreCmStatusRestartIndSendI,
            NULL);
    }
}

static void csrTmBlueCoreCmReplayErrorReqHandler(CsrTmBlueCoreInstanceData *tmBcInst)
{
    CsrTmBluecoreCmReplayErrorReq *prim = (CsrTmBluecoreCmReplayErrorReq *) tmBcInst->recvMsgP;

    CSR_TM_BLUECORE_LIST_ITERATE(&tmBcInst->cmStatusSubscribers,
        csrTmBlueCoreCmStatusReplayErrorIndSendI,
        &prim->hciOpcode);
}

static void csrTmBlueCorePanicIndHandler(CsrTmBlueCoreInstanceData *tmBcInst)
{
    CSR_TM_BLUECORE_LIST_ITERATE(&tmBcInst->cmStatusSubscribers,
        csrTmBlueCoreCmStatusPanicIndSendI,
        NULL);
}

#endif /* CSR_CHIP_MANAGER_ENABLE */

#if defined(CSR_CHIP_MANAGER_ENABLE) || defined(CSR_BLUECORE_ONOFF) && defined(CSR_BLUECORE_PING_INTERVAL)
static void csrTmBlueCoreBccmdCfmHandler(CsrTmBlueCoreInstanceData *tmBcInst)
{
    CsrBccmdCfm *prim = tmBcInst->recvMsgP;
#ifdef CSR_CHIP_MANAGER_ENABLE
    if (prim->varId == CSR_BCCMD_PANIC_ARG_VARID)
    {
        if (tmBcInst->pingInterval != 0)
        {
            tmBcInst->pingTimerId = CsrSchedTimerSet(tmBcInst->pingInterval,
                (void (*)(CsrUint16, void *))csrTmBlueCorePingTimeoutHandler,
                (CsrUint16) 0,
                (void *) tmBcInst);
        }
    }
#endif /* CSR_CHIP_MANAGER_ENABLE */
    CsrPmemFree(prim->payload);
}

#endif

static void sendSavedNop(CsrTmBlueCoreInstanceData *tmBcInst)
{
    if (tmBcInst->savedNop != NULL)
    {
        if (tmBcInst->nopHandler != CSR_SCHED_QID_INVALID)
        {
            CsrHciSendEventInd(tmBcInst->nopHandler, tmBcInst->savedNop, tmBcInst->savedNopLength);
        }
        else
        {
            CsrPmemFree(tmBcInst->savedNop);
        }
        tmBcInst->nopHandler = CSR_SCHED_QID_INVALID;
        tmBcInst->savedNop = NULL;
        tmBcInst->savedNopLength = 0;
    }
}

#ifdef CSR_BLUECORE_ONOFF
static void discardSavedNop(CsrTmBlueCoreInstanceData *tmBcInst)
{
    CsrPmemFree(tmBcInst->savedNop);
    tmBcInst->savedNop = NULL;
}

#endif

static void csrTmBlueCoreActivateTransportCfmSend(CsrSchedQid phandle, CsrResult result)
{
    CsrTmBluecoreActivateTransportCfm *prim;
    prim = CsrPmemAlloc(sizeof(CsrTmBluecoreActivateTransportCfm));
    prim->type = CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_CFM;
    prim->result = result;
    CsrSchedMessagePut(phandle, CSR_TM_BLUECORE_PRIM, prim);
}

#ifdef CSR_BLUECORE_ONOFF
static void csrTmBlueCoreDeactivateTransportCfmSend(CsrSchedQid phandle, CsrResult result)
{
    CsrTmBluecoreDeactivateTransportCfm *prim;
    prim = CsrPmemAlloc(sizeof(CsrTmBluecoreDeactivateTransportCfm));
    prim->type = CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_CFM;
    prim->result = result;
    CsrSchedMessagePut(phandle, CSR_TM_BLUECORE_PRIM, prim);
}

static void csrTmBlueCoreActivateTransportIndSendI(CsrCmnListElm_t *elem, void *data)
{
    CsrTmBlueCoreHandleList *element = (CsrTmBlueCoreHandleList *) elem;
    CsrTmBlueCoreInstanceData *tmBcInst = data;
    CsrTmBluecoreActivateTransportInd *indication;

    indication = CsrPmemAlloc(sizeof(CsrTmBluecoreActivateTransportInd));
    indication->type = CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_IND;

    tmBcInst->pendingResponses += 1;
    CsrSchedMessagePut(element->phandle, CSR_TM_BLUECORE_PRIM, indication);
}

static void csrTmBlueCoreDeactivateTransportIndSend(CsrSchedQid phandle, CsrBool active)
{
    CsrTmBluecoreDeactivateTransportInd *indication;

    indication = CsrPmemAlloc(sizeof(CsrTmBluecoreDeactivateTransportInd));
    indication->type = CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_IND;
    indication->active = active;

    CsrSchedMessagePut(phandle, CSR_TM_BLUECORE_PRIM, indication);
}

static void csrTmBlueCoreDeactivateTransportIndSendI(CsrCmnListElm_t *elem, void *data)
{
    CsrTmBlueCoreHandleList *element = (CsrTmBlueCoreHandleList *) elem;
    CsrTmBlueCoreInstanceData *tmBcInst = data;

    tmBcInst->pendingResponses += 1;
    csrTmBlueCoreDeactivateTransportIndSend(element->phandle, (CsrBool) (tmBcInst->state != CSR_TM_BLUECORE_STATE_IDLE));
}

static void csrTmBlueCoreRegisterCfmSend(CsrSchedQid phandle, CsrBool active)
{
    CsrTmBluecoreRegisterCfm *prim;
    prim = CsrPmemAlloc(sizeof(CsrTmBluecoreRegisterCfm));
    prim->type = CSR_TM_BLUECORE_REGISTER_CFM;
    prim->active = active;
    CsrSchedMessagePut(phandle, CSR_TM_BLUECORE_PRIM, prim);
}

static void csrTmBlueCoreUnregisterCfmSend(CsrSchedQid phandle)
{
    CsrTmBluecoreUnregisterCfm *prim;
    prim = CsrPmemAlloc(sizeof(CsrTmBluecoreUnregisterCfm));
    prim->type = CSR_TM_BLUECORE_UNREGISTER_CFM;
    CsrSchedMessagePut(phandle, CSR_TM_BLUECORE_PRIM, prim);
}

static void csrTmBlueCoreRegisterReqHandler(CsrTmBlueCoreInstanceData *tmBcInst)
{
    CsrTmBluecoreRegisterReq *request = tmBcInst->recvMsgP;
    switch (tmBcInst->state)
    {
        case CSR_TM_BLUECORE_STATE_IDLE:
        case CSR_TM_BLUECORE_STATE_ACTIVATED:
        {
            CsrTmBlueCoreHandleList *elem = CSR_TM_BLUECORE_LIST_ADD(&tmBcInst->transportDelegates);
            elem->phandle = request->phandle;
            csrTmBlueCoreRegisterCfmSend(request->phandle, (CsrBool) (tmBcInst->state == CSR_TM_BLUECORE_STATE_ACTIVATED));
            break;
        }
        default:
            saveMessage(tmBcInst);
            break;
    }
}

static void csrTmBlueCoreUnregisterReqHandler(CsrTmBlueCoreInstanceData *tmBcInst)
{
    CsrTmBluecoreUnregisterReq *request = tmBcInst->recvMsgP;
    switch (tmBcInst->state)
    {
        case CSR_TM_BLUECORE_STATE_IDLE:
        case CSR_TM_BLUECORE_STATE_ACTIVATED:
        {
            CSR_TM_BLUECORE_LIST_ITERATE_AND_REMOVE(&tmBcInst->transportDelegates,
                csrTmBlueCoreHandleListCompareI,
                &request->phandle);
            csrTmBlueCoreUnregisterCfmSend(request->phandle);
            break;
        }
        default:
            saveMessage(tmBcInst);
            break;
    }
}

#ifdef CSR_BLUECORE_PING_INTERVAL
static void pingTimerHandler(CsrUint16 mi, void *mv)
{
    CsrTmBlueCoreInstanceData *tmBcInst = mv;
    tmBcInst->pingTimerId = CsrSchedTimerSet(CSR_BLUECORE_PING_INTERVAL, pingTimerHandler, 0, tmBcInst);
    CsrBccmdReadReqSend(CSR_TM_BLUECORE_IFACEQUEUE, CSR_BCCMD_PANIC_ARG_VARID, 0, 0, NULL);
}

#endif

static void csrTmBlueCoreActivateTransportResHandler(CsrTmBlueCoreInstanceData *tmBcInst)
{
    CSR_LOG_TEXT_CONDITIONAL_ERROR(tmBcInst->pendingResponses == 0,
        (CsrTmBluecoreLto, 0, "Unexpected CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_RES"))

    tmBcInst->pendingResponses -= 1;

    if (tmBcInst->pendingResponses == 0)
    {
        tmBcInst->state = CSR_TM_BLUECORE_STATE_ACTIVATED;
#ifdef CSR_BLUECORE_PING_INTERVAL
        tmBcInst->pingTimerId = CsrSchedTimerSet(CSR_BLUECORE_PING_INTERVAL, pingTimerHandler, 0, tmBcInst);
#endif
        sendSavedNop(tmBcInst);
        csrTmBlueCoreActivateTransportCfmSend(tmBcInst->transportActivator, CSR_RESULT_SUCCESS);
        restoreSavedMessages(tmBcInst);
    }
}

static void csrTmBlueCoreDeactivateTransportResHandler(CsrTmBlueCoreInstanceData *tmBcInst)
{
    CSR_LOG_TEXT_CONDITIONAL_ERROR(tmBcInst->pendingResponses == 0,
        (CsrTmBluecoreLto, 0, "Unexpected CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_RES"))

    tmBcInst->pendingResponses -= 1;

    if (tmBcInst->pendingResponses == 0)
    {
        /* Send out the second wave of deactivate indications (delegates do
           not respond to these) */
        tmBcInst->state = CSR_TM_BLUECORE_STATE_IDLE;
        CSR_TM_BLUECORE_LIST_ITERATE(&tmBcInst->transportDelegates,
            csrTmBlueCoreDeactivateTransportIndSendI,
            tmBcInst);
        tmBcInst->pendingResponses = 0;
#ifdef CSR_BLUECORE_PING_INTERVAL
        CsrSchedTimerCancel(tmBcInst->pingTimerId, NULL, NULL);
        tmBcInst->pingTimerId = CSR_SCHED_TID_INVALID;
#endif
        csrTmBlueCoreDeactivateTransportCfmSend(tmBcInst->transportActivator, CSR_RESULT_SUCCESS);
        restoreSavedMessages(tmBcInst);
    }
}

#endif /* CSR_BLUECORE_ONOFF */

#ifndef CSR_BLUECORE_ONOFF
static void csrTmBlueCoreActivateTransportCfmSendI(CsrCmnListElm_t *elem, void *data)
{
    CsrTmBlueCoreHandleList *element = (CsrTmBlueCoreHandleList *) elem;
    csrTmBlueCoreActivateTransportCfmSend(element->phandle, *(CsrResult *) data);
}

#endif /* !CSR_BLUECORE_ONOFF */

static CsrUint16 csrTmBlueCoreGetPayloadLength(CsrUint8 *payload)
{
    CsrUint16 length;

    CsrUint8 *tmpPtr = payload;

    tmpPtr += 2;
    CSR_CONVERT_16_FROM_XAP(length, tmpPtr);
    return (CsrUint16) (length * 2);
}

static CsrUint16 csrTmBlueCoreExtractBootstrapPayloadLength(CsrUint8 **array, CsrUint16 entries)
{
    CsrUint16 i;
    CsrUint16 len, length = 0;

    for (i = 0; i < entries; i++)
    {
        len = csrTmBlueCoreGetPayloadLength(array[i]);
        if (len >= CSR_TM_BLUECORE_BCCMD_MIN_SIZE)
        {
            length = (CsrUint16) (length + len);
        }
        else
        {
            return 0;
        }
    }
    return length;
}

static CsrUint8 *csrTmBlueCoreBuildBootstrapPayload(CsrUint8 **array, CsrUint16 entries, CsrUint16 *payloadLength)
{
    CsrUint16 length = csrTmBlueCoreExtractBootstrapPayloadLength(array, entries);

    if (length > 0)
    {
        CsrUint16 i, len;
        CsrUint16 index = 0;
        CsrUint8 *bccmd = (CsrUint8 *) CsrPmemAlloc(length);

        for (i = 0; i < entries; i++)
        {
            len = csrTmBlueCoreGetPayloadLength(array[i]);
            CsrMemCpy(&(bccmd[index]), array[i], len);
            index = (CsrUint16) (index + len);
            CsrPmemFree(array[i]);
            array[i] = NULL;
        }
        *payloadLength = length;
        return bccmd;
    }
    else
    {
        *payloadLength = 0;
        return NULL;
    }
}

#ifdef CSR_BLUECORE_ONOFF
static void resetTimeoutHandler(CsrUint16 mi, void *mv)
{
    CsrTmBlueCoreInstanceData *tmBcInst = (CsrTmBlueCoreInstanceData *) mv;
    tmBcInst->timerId = CSR_SCHED_TID_INVALID;

    CSR_LOG_TEXT_ERROR((CsrTmBluecoreLto, 0, "BlueCore reset timeout"));

    switch (tmBcInst->state)
    {
        case CSR_TM_BLUECORE_STATE_ACTIVATING:
        {
            tmBcInst->state = CSR_TM_BLUECORE_STATE_IDLE;
            CsrTmBlueCoreTransportStop(tmBcInst->blueCoreTransportHandle);
            discardSavedNop(tmBcInst);
            csrTmBlueCoreActivateTransportCfmSend(tmBcInst->transportActivator, CSR_RESULT_FAILURE);
            restoreSavedMessages(tmBcInst);
            break;
        }
        case CSR_TM_BLUECORE_STATE_RESTARTING:
        {
            /* Indicate the failure to BCCMD to terminate the bootstrap process */
            tmBcInst->state = CSR_TM_BLUECORE_STATE_ACTIVATING;
            CsrBccmdResetResSend(CSR_RESULT_FAILURE);
            break;
        }
        default:
            break;
    }
}

#endif

static void csrTmBlueCoreResetDelayHandler(CsrUint16 mi, void *mv)
{
    CsrTmBlueCoreInstanceData *tmBcInst = (CsrTmBlueCoreInstanceData *) mv;
    tmBcInst->timerId = CSR_SCHED_TID_INVALID;

    CSR_LOG_TEXT_INFO((CsrTmBluecoreLto, 0, "Restarting transport (%u)", tmBcInst->numberOfForcedReset));

    if (CsrTmBlueCoreTransportDriverRestart(tmBcInst->blueCoreTransportHandle,
            tmBcInst->numberOfForcedReset))
    {
#ifdef CSR_BLUECORE_ONOFF
        tmBcInst->timerId = CsrSchedTimerSet(CSR_BLUECORE_RESET_TIMEOUT, resetTimeoutHandler, 0, tmBcInst);
#endif
    }
    else
    {
        CSR_LOG_TEXT_ERROR((CsrTmBluecoreLto, 0,
                            "CsrTmBlueCoreTransportDriverRestart failed"));
#ifdef CSR_BLUECORE_ONOFF
        tmBcInst->state = CSR_TM_BLUECORE_STATE_IDLE;
        CsrTmBlueCoreTransportStop(tmBcInst->blueCoreTransportHandle);
        discardSavedNop(tmBcInst);
        csrTmBlueCoreActivateTransportCfmSend(tmBcInst->transportActivator, CSR_RESULT_FAILURE);
        restoreSavedMessages(tmBcInst);
#endif
    }
}

static void csrTmBlueCoreBccmdResetIndHandler(CsrTmBlueCoreInstanceData *tmBcInst)
{
    CsrTmBlueCoreTransportClose(tmBcInst->blueCoreTransportHandle);
    tmBcInst->state = CSR_TM_BLUECORE_STATE_RESTARTING;
    tmBcInst->numberOfForcedReset += 1;
    tmBcInst->timerId = CsrSchedTimerSet(CSR_BLUECORE_RESET_TIMER, csrTmBlueCoreResetDelayHandler, 0, tmBcInst);
    CSR_LOG_TEXT_INFO((CsrTmBluecoreLto, 0,
                       "Reset pending, restarting transport in %uus", CSR_BLUECORE_RESET_TIMER));
}

static void csrTmBlueCoreActivate(CsrTmBlueCoreInstanceData *tmBcInst)
{
#ifdef CSR_BLUECORE_ONOFF
    CSR_TM_BLUECORE_LIST_ITERATE(&tmBcInst->transportDelegates,
        csrTmBlueCoreActivateTransportIndSendI,
        tmBcInst);
#else /* !CSR_BLUECORE_ONOFF */
    CsrResult result = CSR_RESULT_SUCCESS;

    sendSavedNop(tmBcInst);
#ifdef CSR_CHIP_MANAGER_ENABLE
    if (!tmBcInst->activateTransportCfmIsSend)
    {
        CSR_TM_BLUECORE_LIST_ITERATE(&tmBcInst->transportActivators,
            csrTmBlueCoreActivateTransportCfmSendI,
            &result);
        tmBcInst->activateTransportCfmIsSend = TRUE;
    }
    else
    {
        CSR_TM_BLUECORE_LIST_ITERATE(&tmBcInst->cmReplayersRegistered,
            csrTmBlueCoreCmReplayStartIndSendI,
            tmBcInst);
    }
    if (!tmBcInst->pingStarted && (tmBcInst->pingInterval != 0))
    {
        csrTmBlueCorePingMsgSend();
        tmBcInst->pingStarted = TRUE;
    }
#else /* !CSR_CHIP_MANAGER_ENABLE */
    CSR_TM_BLUECORE_LIST_ITERATE(&tmBcInst->transportActivators,
        csrTmBlueCoreActivateTransportCfmSendI,
        &result);
#endif /* !CSR_CHIP_MANAGER_ENABLE */
    tmBcInst->state = CSR_TM_BLUECORE_STATE_ACTIVATED;
#endif /* !CSR_BLUECORE_ONOFF */
}

static void csrTmBlueCoreBccmdBootstrapCfmHandler(CsrTmBlueCoreInstanceData *tmBcInst)
{
    CsrBccmdBootstrapCfm *prim = (CsrBccmdBootstrapCfm *) tmBcInst->recvMsgP;

    if (prim->status == CSR_RESULT_SUCCESS)
    {
        CSR_LOG_TEXT_INFO((CsrTmBluecoreLto, 0, "Bootstrap complete"));
        csrTmBlueCoreActivate(tmBcInst);
    }
    else
    {
        CSR_LOG_TEXT_ERROR((CsrTmBluecoreLto, 0, "Bootstrap failed (0x%04X)", prim->status));
#ifdef CSR_BLUECORE_ONOFF
        tmBcInst->state = CSR_TM_BLUECORE_STATE_IDLE;
        CsrTmBlueCoreTransportStop(tmBcInst->blueCoreTransportHandle);
        discardSavedNop(tmBcInst);
        csrTmBlueCoreActivateTransportCfmSend(tmBcInst->transportActivator, CSR_RESULT_FAILURE);
        restoreSavedMessages(tmBcInst);
#endif
    }
}

static void csrTmBlueCoreBccmdGetBuildIdCfm(CsrTmBlueCoreInstanceData *tmBcInst)
{
    CsrBccmdGetBuildIdCfm *prim = (CsrBccmdGetBuildIdCfm *) tmBcInst->recvMsgP;

    if (prim->status == CSR_RESULT_SUCCESS)
    {
        CsrUint16 entries, payloadLength;

        CsrUint8 **array = CsrTmBlueCoreGetBootstrap(prim->buildId, &entries);

        if (array != NULL)
        {
            CsrUint8 *payload = csrTmBlueCoreBuildBootstrapPayload(array, entries, &payloadLength);

            if (payload != NULL)
            {
                CSR_LOG_TEXT_INFO((CsrTmBluecoreLto, 0,
                                   "Starting Bootstrap (Build ID: %u)", prim->buildId));
                CsrBccmdBootstrapReqSend(CSR_TM_BLUECORE_IFACEQUEUE, payloadLength, payload);
            }
            else
            {
                CSR_LOG_TEXT_ERROR((CsrTmBluecoreLto, 0,
                                    "Invalid bootstrap array"));
#ifdef CSR_BLUECORE_ONOFF
                tmBcInst->state = CSR_TM_BLUECORE_STATE_IDLE;
                CsrTmBlueCoreTransportStop(tmBcInst->blueCoreTransportHandle);
                discardSavedNop(tmBcInst);
                csrTmBlueCoreActivateTransportCfmSend(tmBcInst->transportActivator, CSR_RESULT_FAILURE);
                restoreSavedMessages(tmBcInst);
#endif
            }
            CsrPmemFree(array);
        }
        else /* The Bootstrap procedure is not required */
        {
            csrTmBlueCoreActivate(tmBcInst);
        }
    }
    else
    {
        CSR_LOG_TEXT_ERROR((CsrTmBluecoreLto, 0,
                            "Failed to retrieve build ID from device (0x%04X)", prim->status));
#ifdef CSR_BLUECORE_ONOFF
        tmBcInst->state = CSR_TM_BLUECORE_STATE_IDLE;
        CsrTmBlueCoreTransportStop(tmBcInst->blueCoreTransportHandle);
        discardSavedNop(tmBcInst);
        csrTmBlueCoreActivateTransportCfmSend(tmBcInst->transportActivator, CSR_RESULT_FAILURE);
        restoreSavedMessages(tmBcInst);
#endif
    }
}

#ifdef CSR_CHIP_MANAGER_ENABLE
static void csrTmBlueCoreBccmdPanicArgsCfm(CsrTmBlueCoreInstanceData *tmBcInst)
{
    CsrBccmdPanicArgsCfm *prim = (CsrBccmdPanicArgsCfm *) tmBcInst->recvMsgP;

    /* Notify status subscribers */
    CSR_TM_BLUECORE_LIST_ITERATE(&tmBcInst->cmStatusSubscribers,
        csrTmBlueCoreCmStatusPanicArgsIndSendI,
        prim);

    /* Restart bootstrap */
    tmBcInst->state = CSR_TM_BLUECORE_STATE_ACTIVATING;
    CsrBccmdGetBuildIdReqSend(CSR_TM_BLUECORE_IFACEQUEUE);
}

#endif

static void csrTmBlueCoreActivateTransportReqHandler(CsrTmBlueCoreInstanceData *tmBcInst)
{
    CsrTmBluecoreActivateTransportReq *prim = tmBcInst->recvMsgP;

#ifdef CSR_BLUECORE_ONOFF
    tmBcInst->transportActivator = prim->phandle;
#else
    CsrTmBlueCoreHandleList *elem = CSR_TM_BLUECORE_LIST_ADD(&tmBcInst->transportActivators);
    elem->phandle = prim->phandle;
#endif

    switch (tmBcInst->state)
    {
        case CSR_TM_BLUECORE_STATE_IDLE:
        {
            /* Start activating the transport for the BlueCore chip */
            if (CsrTmBlueCoreTransportStart(tmBcInst->blueCoreTransportHandle))
            {
#ifndef CSR_ENABLE_WARM_RESET // Sean added for non-warm-reset version
				CsrBccmdGetBuildIdReqSend(CSR_TM_BLUECORE_IFACEQUEUE);
				tmBcInst->state = CSR_TM_BLUECORE_STATE_ACTIVATED;
#else // original
				tmBcInst->state = CSR_TM_BLUECORE_STATE_ACTIVATING;
#endif
#ifdef CSR_BLUECORE_ONOFF
                tmBcInst->numberOfForcedReset = 0;
                tmBcInst->timerId = CsrSchedTimerSet(CSR_BLUECORE_RESET_TIMEOUT, resetTimeoutHandler, 0, tmBcInst);
#endif
            }
            else
            {
                CSR_LOG_TEXT_ERROR((CsrTmBluecoreLto, 0,
                                    "CsrTmBlueCoreTransportStart failed"));
#ifdef CSR_BLUECORE_ONOFF
                csrTmBlueCoreActivateTransportCfmSend(prim->phandle, CSR_RESULT_FAILURE);
#endif
            }
            break;
        }
        case CSR_TM_BLUECORE_STATE_ACTIVATED:
        {
            /* The transport is already activated */
            csrTmBlueCoreActivateTransportCfmSend(prim->phandle, CSR_RESULT_SUCCESS);
            break;
        }
        default:
        {
#ifdef CSR_BLUECORE_ONOFF
            CSR_LOG_TEXT_ERROR((CsrTmBluecoreLto, 0,
                                "Illegal repeated CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_REQ ignored"));
#else
            /* Transport is starting up, confirm will be sent when activated */
#endif
            break;
        }
    }
}

#ifdef CSR_BLUECORE_ONOFF
static void csrTmBlueCoreDeactivateTransportReqHandler(CsrTmBlueCoreInstanceData *tmBcInst)
{
    CsrTmBluecoreActivateTransportReq *prim = tmBcInst->recvMsgP;

    switch (tmBcInst->state)
    {
        case CSR_TM_BLUECORE_STATE_IDLE:
        {
            /* The transport is already deactivated */
            csrTmBlueCoreDeactivateTransportCfmSend(prim->phandle, CSR_RESULT_SUCCESS);
            break;
        }
        case CSR_TM_BLUECORE_STATE_ACTIVATED:
        {
            /* Stop transport and send first deactivate indication wave to all delegates,
               and wait for their responses before sending the second wave. */
            tmBcInst->state = CSR_TM_BLUECORE_STATE_DEACTIVATING;
            CsrTmBlueCoreTransportStop(tmBcInst->blueCoreTransportHandle);
            CSR_TM_BLUECORE_LIST_ITERATE(&tmBcInst->transportDelegates,
                csrTmBlueCoreDeactivateTransportIndSendI,
                tmBcInst);
            break;
        }
        default:
        {
            CSR_LOG_TEXT_ERROR((CsrTmBluecoreLto, 0,
                                "Illegal repeated CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_REQ ignored"));
            break;
        }
    }
}

#endif /* CSR_BLUECORE_ONOFF */

static void saveNop(CsrTmBlueCoreInstanceData *tmBcInst, CsrHciNopInd *prim)
{
    CsrPmemFree(tmBcInst->savedNop);
    tmBcInst->nopHandler = prim->phandle;
    tmBcInst->savedNop = prim->payload;
    prim->payload = NULL;
    tmBcInst->savedNopLength = prim->payloadLength;
}

static void csrTmBlueCoreHciNopIndHandler(CsrTmBlueCoreInstanceData *tmBcInst)
{
    CsrHciNopInd *prim = (CsrHciNopInd *) tmBcInst->recvMsgP;

#ifdef CSR_CHIP_MANAGER_QUERY_ENABLE
    CsrBccmdQueryReqSend();
#endif

    CSR_LOG_TEXT_INFO((CsrTmBluecoreLto, 0, "Received NOP (state: %u)", tmBcInst->state));

    switch (tmBcInst->state)
    {
        case CSR_TM_BLUECORE_STATE_ACTIVATING:
        {
#ifdef CSR_CHIP_MANAGER_ENABLE
            /* Get panic and fault args and then restart bootstrap */
            CsrBccmdPanicArgsReqSend(CSR_TM_BLUECORE_IFACEQUEUE);
#else
#ifdef CSR_BLUECORE_ONOFF
            CsrSchedTimerCancel(tmBcInst->timerId, NULL, NULL);
            tmBcInst->timerId = CSR_SCHED_TID_INVALID;
            saveNop(tmBcInst, prim);
#endif
            /* Get the build ID from the chip */
            CsrBccmdGetBuildIdReqSend(CSR_TM_BLUECORE_IFACEQUEUE);
#endif
            break;
        }
        case CSR_TM_BLUECORE_STATE_RESTARTING:
        {
            tmBcInst->state = CSR_TM_BLUECORE_STATE_ACTIVATING;
            CsrSchedTimerCancel(tmBcInst->timerId, NULL, NULL);
            tmBcInst->timerId = CSR_SCHED_TID_INVALID;
            saveNop(tmBcInst, prim);
            CsrBccmdResetResSend(CSR_RESULT_SUCCESS);
            break;
        }
        case CSR_TM_BLUECORE_STATE_ACTIVATED:
        {
            if (prim->phandle != CSR_SCHED_QID_INVALID)
            {
                CsrHciSendEventInd(prim->phandle, prim->payload, prim->payloadLength);
                prim->payload = NULL;
            }
            break;
        }
#ifdef CSR_CHIP_MANAGER_ENABLE
        case CSR_TM_BLUECORE_STATE_RESETTING:
        {
            /* Get panic and fault args and then restart bootstrap */
            CsrBccmdPanicArgsReqSend(CSR_TM_BLUECORE_IFACEQUEUE);
            break;
        }
#endif
#ifdef CSR_BLUECORE_ONOFF
        case CSR_TM_BLUECORE_STATE_IDLE:
        case CSR_TM_BLUECORE_STATE_DEACTIVATING:
            /* Discard */
            break;
#endif
        default:
        {
            CSR_LOG_TEXT_WARNING((CsrTmBluecoreLto, 0,
                                  "Unexpected CSR_HCI_NOP_IND in state 0x%02X", tmBcInst->state));
            break;
        }
    }

    CsrPmemFree(prim->payload);
}

static void csrTmBlueCoreResetIndHandler(CsrTmBlueCoreInstanceData *tmBcInst)
{
#ifdef CSR_BLUECORE_ONOFF
    switch (tmBcInst->state)
    {
        default:
        case CSR_TM_BLUECORE_STATE_IDLE:
        case CSR_TM_BLUECORE_STATE_DEACTIVATING:
            /* Discard */
            break;
        case CSR_TM_BLUECORE_STATE_ACTIVATING:
            saveMessage(tmBcInst);
            break;
        case CSR_TM_BLUECORE_STATE_ACTIVATED:
            csrTmBlueCoreDeactivateTransportIndSend(tmBcInst->transportActivator, TRUE);
            break;
        case CSR_TM_BLUECORE_STATE_RESTARTING:
            CsrTmBlueCoreTransportRestart(tmBcInst->blueCoreTransportHandle);
            break;
    }
#else /* !CSR_BLUECORE_ONOFF */
    if (tmBcInst->state != CSR_TM_BLUECORE_STATE_RESTARTING)
    {
        /* This is an unexpected reset */
#ifdef CSR_CHIP_MANAGER_ENABLE
        /* Notify of unexpected reset */
        CSR_TM_BLUECORE_LIST_ITERATE(&tmBcInst->cmStatusSubscribers,
            csrTmBlueCoreCmStatusResetIndSendI,
            NULL);
        CSR_TM_BLUECORE_LIST_ITERATE(&tmBcInst->cmReplayersRegistered,
            csrTmBlueCoreCmStatusResetIndSendI,
            NULL);
        tmBcInst->state = CSR_TM_BLUECORE_STATE_RESETTING;

        /* Restart transport driver - Note: tmBcInst->numberOfForcedReset = 0 will imply initial baudrate for uart */
        tmBcInst->numberOfForcedReset = 0;
        CsrTmBlueCoreTransportDriverRestart(tmBcInst->blueCoreTransportHandle, tmBcInst->numberOfForcedReset);
#else
        CSR_LOG_TEXT_ERROR((CsrTmBluecoreLto, 0, "BlueCore transport lost"));
#endif
    }
    CsrTmBlueCoreTransportRestart(tmBcInst->blueCoreTransportHandle);
#endif /* !CSR_BLUECORE_ONOFF */
}

void CsrTmBlueCoreTransportInit(void **gash, void *blueCoreTransportDescriptor)
{
    CsrTmBlueCoreInstanceData *tmBcInst;

    tmBcInst = *gash = CsrPmemAlloc(sizeof(CsrTmBlueCoreInstanceData));

#ifdef CSR_BLUECORE_ONOFF
    tmBcInst->transportActivator = CSR_SCHED_QID_INVALID;
    CsrCmnListInit(&(tmBcInst->transportDelegates), 0, NULL, NULL);
    tmBcInst->pendingResponses = 0;
    tmBcInst->saveQueue = NULL;
#ifdef CSR_BLUECORE_PING_INTERVAL
    tmBcInst->pingTimerId = CSR_SCHED_TID_INVALID;
#endif
#else /* !CSR_BLUECORE_ONOFF */
    CsrCmnListInit(&(tmBcInst->transportActivators), 0, NULL, NULL);
#endif /* !CSR_BLUECORE_ONOFF */

    tmBcInst->blueCoreTransportHandle = CsrHciTransportCreate(blueCoreTransportDescriptor);
    tmBcInst->numberOfForcedReset = 0;
    tmBcInst->state = CSR_TM_BLUECORE_STATE_IDLE;
    tmBcInst->timerId = CSR_SCHED_TID_INVALID;
    tmBcInst->nopHandler = CSR_SCHED_QID_INVALID;
    tmBcInst->savedNop = NULL;
    tmBcInst->savedNopLength = 0;

#ifdef CSR_CHIP_MANAGER_ENABLE
    CsrCmnListInit(&(tmBcInst->cmStatusSubscribers), 0, NULL, NULL);
    CsrCmnListInit(&(tmBcInst->cmReplayersRegistered), 0, NULL, NULL);
    CsrCmnListInit(&(tmBcInst->cmReplayersStarted), 0, NULL, NULL);
    tmBcInst->activateTransportCfmIsSend = FALSE;
    tmBcInst->pingTimerId = CSR_SCHED_TID_INVALID;
    tmBcInst->pingInterval = CSR_BLUECORE_DEFAULT_PING_INTERVAL;
    tmBcInst->pingStarted = FALSE;
#endif /* CSR_CHIP_MANAGER_ENABLE */

    CSR_LOG_TEXT_REGISTER(&CsrTmBluecoreLto, "TM_BLUECORE", 0, NULL);
}

void CsrTmBlueCoreTmBlueCoreMsgHandler(CsrTmBlueCoreInstanceData *tmBcInst)
{
    CsrPrim *primType = (CsrPrim *) tmBcInst->recvMsgP;

    switch (*primType)
    {
        case CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_REQ:
            csrTmBlueCoreActivateTransportReqHandler(tmBcInst);
            break;
        case CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_REQ:
#ifdef CSR_BLUECORE_ONOFF
            csrTmBlueCoreDeactivateTransportReqHandler(tmBcInst);
#endif
            break;
        case CSR_TM_BLUECORE_RESET_IND:
            csrTmBlueCoreResetIndHandler(tmBcInst);
            break;
#ifdef CSR_CHIP_MANAGER_ENABLE
        case CSR_TM_BLUECORE_CM_STATUS_SUBSCRIBE_REQ:
            csrTmBlueCoreCmStatusSubscribeReqHandler(tmBcInst);
            break;
        case CSR_TM_BLUECORE_CM_STATUS_UNSUBSCRIBE_REQ:
            csrTmBlueCoreCmStatusUnsubscribeReqHandler(tmBcInst);
            break;
        case CSR_TM_BLUECORE_CM_PING_INTERVAL_SET_REQ:
            csrTmBlueCoreCmPingIntervalSetReqHandler(tmBcInst);
            break;
        case CSR_TM_BLUECORE_CM_REPLAY_REGISTER_REQ:
            csrTmBlueCoreCmReplayRegisterReqHandler(tmBcInst);
            break;
        case CSR_TM_BLUECORE_CM_REPLAY_START_RES:
            csrTmBlueCoreCmReplayStartResHandler(tmBcInst);
            break;
        case CSR_TM_BLUECORE_CM_REPLAY_ERROR_REQ:
            csrTmBlueCoreCmReplayErrorReqHandler(tmBcInst);
            break;
        case CSR_TM_BLUECORE_PANIC_IND:
            csrTmBlueCorePanicIndHandler(tmBcInst);
            break;
#endif
#ifdef CSR_BLUECORE_ONOFF
        case CSR_TM_BLUECORE_REGISTER_REQ:
            csrTmBlueCoreRegisterReqHandler(tmBcInst);
            break;
        case CSR_TM_BLUECORE_UNREGISTER_REQ:
            csrTmBlueCoreUnregisterReqHandler(tmBcInst);
            break;
        case CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_RES:
            csrTmBlueCoreActivateTransportResHandler(tmBcInst);
            break;
        case CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_RES:
            csrTmBlueCoreDeactivateTransportResHandler(tmBcInst);
            break;
#endif
        default:
            CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrTmBluecoreLto, 0, CSR_TM_BLUECORE_PRIM, *primType);
            break;
    }
}

void CsrTmBlueCoreBccmdMsgHandler(CsrTmBlueCoreInstanceData *tmBcInst)
{
    CsrPrim *primType = (CsrPrim *) tmBcInst->recvMsgP;

    switch (*primType)
    {
        case CSR_BCCMD_RESET_IND:
            csrTmBlueCoreBccmdResetIndHandler(tmBcInst);
            break;
        case CSR_BCCMD_BOOTSTRAP_CFM:
            csrTmBlueCoreBccmdBootstrapCfmHandler(tmBcInst);
            break;
        case CSR_BCCMD_GET_BUILD_ID_CFM:
            csrTmBlueCoreBccmdGetBuildIdCfm(tmBcInst);
            break;
#if defined(CSR_CHIP_MANAGER_ENABLE) || defined(CSR_BLUECORE_ONOFF) && defined(CSR_BLUECORE_PING_INTERVAL)
        case CSR_BCCMD_CFM:
            csrTmBlueCoreBccmdCfmHandler(tmBcInst);
            break;
#endif
#ifdef CSR_CHIP_MANAGER_ENABLE
        case CSR_BCCMD_PANIC_ARGS_CFM:
            csrTmBlueCoreBccmdPanicArgsCfm(tmBcInst);
            break;
#endif
        default:
            CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrTmBluecoreLto, 0, CSR_BCCMD_PRIM, *primType);
            break;
    }
}

void CsrTmBlueCoreHciMsgHandler(CsrTmBlueCoreInstanceData *tmBcInst)
{
    CsrPrim *primType = (CsrPrim *) tmBcInst->recvMsgP;

    switch (*primType)
    {
        case CSR_HCI_NOP_IND:
            csrTmBlueCoreHciNopIndHandler(tmBcInst);
            break;
        default:
            CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrTmBluecoreLto, 0, CSR_HCI_PRIM, *primType);
            break;
    }
}
