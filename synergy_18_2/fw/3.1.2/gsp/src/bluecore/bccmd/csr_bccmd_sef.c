/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.


*****************************************************************************/

#include "csr_usr_config.h"
#include "csr_types.h"
#include "csr_macro.h"
#include "csr_util.h"
#include "csr_transport.h"
#include "csr_log_text_2.h"
#include "csr_bccmd_handler.h"
#include "csr_bccmd_sef.h"
#include "csr_bccmd_util.h"
#include "csr_bccmd_cm.h"
#include "csr_bccmd_lib.h"
#include "csr_hci_lib.h"
#include "csr_tm_bluecore_prim.h"
#include "csr_tm_bluecore_lib.h"
#include "csr_tm_bluecore_private_lib.h"

#ifdef CSR_BLUECORE_ONOFF
static void commandTimeoutHandler(CsrUint16 mi, void *mv)
{
    CsrBccmdInstanceData *bccmdInst = mv;
    bccmdInst->timerId = CSR_SCHED_TID_INVALID;
    CSR_LOG_TEXT_ERROR((CsrBccmdLto, 0, "Command timeout"));
    CsrTmBluecoreResetIndSend();
}
#endif

static void csrSendBccmdData(CsrBccmdInstanceData *bccmdInst, CsrUint8 *payload, CsrUint16 payloadLength, CsrBool highPriority)
{
    CsrMblk *data;
#ifdef CSR_CHIP_MANAGER_ENABLE
    if (!CsrBccmdCmReqHandler(bccmdInst, payload, payloadLength, highPriority))
    {
        CsrPmemFree(payload);
        return;
    }
#else
    CSR_UNUSED(highPriority);
#endif
    data = CsrMblkDataCreate(payload, payloadLength, TRUE);
    CsrHciVendorSpecificCommandReqSend(TRANSPORT_CHANNEL_BCCMD, data);
}

static void csrBccmdHandler(CsrBccmdInstanceData *bccmdInst, void *message)
{
    CsrBccmdReq *prim = message;

    bccmdInst->inProgress = TRUE;
    bccmdInst->phandle = prim->phandle;
#ifdef CSR_BLUECORE_ONOFF
    bccmdInst->timerId = CsrSchedTimerSet(CSR_BCCMD_CMD_TIMEOUT, commandTimeoutHandler, 0, bccmdInst);
#endif
    csrSendBccmdData(bccmdInst, prim->payload, prim->payloadLength, FALSE);
}

static CsrUint16 csrBccmdGetVarid(CsrUint8 *payload)
{
    CsrUint16 varId;
    CsrUint8 *tmpPtr = payload;

    tmpPtr += 6;
    CSR_CONVERT_16_FROM_XAP(varId, tmpPtr);
    return varId;
}

static CsrBool csrBccmdResetCommandSend(CsrUint8 *payload)
{
    CsrUint16 varId = csrBccmdGetVarid(payload);

    if ((varId == CSR_BCCMD_COLD_RESET_VARID) ||
        (varId == CSR_BCCMD_WARM_RESET_VARID))
    {
        return TRUE;
    }
    return FALSE;
}

static CsrUint16 csrBccmdGetPayloadLength(CsrUint8 *payload)
{
    CsrUint16 length;

    CsrUint8 *tmpPtr = payload;

    tmpPtr += 2;
    CSR_CONVERT_16_FROM_XAP(length, tmpPtr);
    return (CsrUint16) (length * 2);
}

static CsrResult csrBccmdGetStatus(CsrUint8 *payload)
{
    CsrResult status;

    CsrUint8 *tmpPtr = payload;

    tmpPtr += 8;
    CSR_CONVERT_16_FROM_XAP(status, tmpPtr);
    return status;
}

static CsrUint8 *csrGetBccmd(CsrBccmdBoot *bootVar, CsrUint16 *length)
{
    *length = 0;

    if ((bootVar->currentIndex + CSR_BCCMD_MIN_SIZE) <= bootVar->payloadLength)
    {
        CsrUint16 bccmdLength = csrBccmdGetPayloadLength(&(bootVar->payload[bootVar->currentIndex]));

        if (bccmdLength >= CSR_BCCMD_MIN_SIZE)
        {
            CsrUint8 *bccmd = (CsrUint8 *) CsrPmemAlloc(bccmdLength);
            CsrMemCpy(bccmd, &(bootVar->payload[bootVar->currentIndex]), bccmdLength);
            *length = bccmdLength;
            return bccmd;
        }
    }
    return NULL;
}

CsrUint16 CsrBccmdGetBuildId(CsrUint8 *payload)
{
    CsrUint16 buildId = 0;
    if (payload != NULL)
    {
        payload += 10;
        CSR_CONVERT_16_FROM_XAP(buildId, payload);
    }
    return buildId;
}

#ifdef CSR_CHIP_MANAGER_ENABLE
static CsrUint16 csrBccmdGetArgUint16(CsrUint8 *payload)
{
    CsrUint16 arg;
    CsrUint8 *tmpPtr = payload;
    tmpPtr += 10;
    CSR_CONVERT_16_FROM_XAP(arg, tmpPtr);
    return arg;
}

#endif

static void csrBccmdGetBuildIdCfmHandler(CsrSchedQid phandle, CsrUint8 *payload, CsrResult status)
{
    CsrBccmdGetBuildIdCfm *confirm;

    confirm = CsrPmemAlloc(sizeof(CsrBccmdGetBuildIdCfm));
    confirm->type = CSR_BCCMD_GET_BUILD_ID_CFM;
    confirm->status = status;
    confirm->buildId = CsrBccmdGetBuildId(payload);

    CsrSchedMessagePut(phandle, CSR_BCCMD_PRIM, confirm);
}

#ifdef CSR_CHIP_MANAGER_ENABLE
static void csrBccmdGetPanicArgCfmHandler(CsrBccmdInstanceData *bccmdInst, CsrUint8 *payload, CsrUint16 payloadLength, CsrResult status)
{
    CsrUint16 bccmdLength;
    CsrUint8 *bccmd;

    bccmdInst->panicStatus = status;
    bccmdInst->panicArg = csrBccmdGetArgUint16(payload);

    bccmd = CsrBccmdBccmdCommandBuild(CSR_BCCMD_GET_REQ, CSR_BCCMD_FAULT_ARG_VARID, 0, 0, NULL, &bccmdLength);
    csrSendBccmdData(bccmdInst, bccmd, bccmdLength, TRUE);
}

static void csrBccmdGetFaultArgCfmHandler(CsrBccmdInstanceData *bccmdInst, CsrUint8 *payload, CsrUint16 payloadLength, CsrResult status)
{
    CsrBccmdPanicArgsCfm *prim = (CsrBccmdPanicArgsCfm *) CsrPmemAlloc(sizeof(CsrBccmdPanicArgsCfm));

    prim->type = CSR_BCCMD_PANIC_ARGS_CFM;
    prim->panicStatus = bccmdInst->panicStatus;
    prim->panicArg = bccmdInst->panicArg;
    prim->faultStatus = status;
    prim->faultArg = csrBccmdGetArgUint16(payload);
    CsrSchedMessagePut(bccmdInst->phandle, CSR_BCCMD_PRIM, prim);

    bccmdInst->panicStatus = CSR_RESULT_SUCCESS;
    bccmdInst->panicArg = 0;
}

#endif

static void csrBccmdFlushCfmHandler(CsrSchedQid phandle, CsrResult status)
{
    CsrBccmdFlushCfm *prim = (CsrBccmdFlushCfm *) CsrPmemAlloc(sizeof(CsrBccmdFlushCfm));

    prim->type = CSR_BCCMD_FLUSH_CFM;
    prim->result = status;
    CsrSchedMessagePut(phandle, CSR_BCCMD_PRIM, prim);
}

static void csrBccmdBootstrapCfmHandler(CsrBccmdInstanceData *bccmdInst, CsrResult status)
{
    CsrBccmdBootstrapCfm *prim = CsrPmemAlloc(sizeof(CsrBccmdBootstrapCfm));
    prim->type = CSR_BCCMD_BOOTSTRAP_CFM;
    prim->status = status;
    CsrSchedMessagePut(bccmdInst->phandle, CSR_BCCMD_PRIM, prim);
    CsrBccmdRemoveBootType(&(bccmdInst->bootVar));
}

static void csrBccmdResetIndSend(CsrSchedQid phandle)
{
    CsrBccmdResetInd *prim = (CsrBccmdResetInd *) CsrPmemAlloc(sizeof(CsrBccmdResetInd));
    prim->type = CSR_BCCMD_RESET_IND;
    CsrSchedMessagePut(phandle, CSR_BCCMD_PRIM, prim);
}

#ifdef CSR_BLUECORE_ONOFF
static void bootstrapTimeoutHandler(CsrUint16 mi, void *mv)
{
    CsrBccmdInstanceData *bccmdInst = mv;
    bccmdInst->timerId = CSR_SCHED_TID_INVALID;
    CSR_LOG_TEXT_ERROR((CsrBccmdLto, 0, "Bootstrap timeout"));
    csrBccmdBootstrapCfmHandler(bccmdInst, CSR_RESULT_FAILURE);
}
#endif

static void csrBccmdBootstrapHandler(CsrBccmdInstanceData *bccmdInst, CsrResult status)
{
    CsrBccmdBoot *bootVar = bccmdInst->bootVar;

    if (bootVar && (status == CSR_RESULT_SUCCESS) && (bootVar->currentIndex < bootVar->payloadLength))
    {
        CsrUint16 length;
        CsrUint8 *bccmd = csrGetBccmd(bootVar, &length);

        if (bccmd)
        {
            if (csrBccmdResetCommandSend(bccmd))
            {
                bootVar->resetInitiated = TRUE;
                csrBccmdResetIndSend(bccmdInst->phandle);
            }
#ifdef CSR_BLUECORE_ONOFF
            else
            {
                bccmdInst->timerId = CsrSchedTimerSet(CSR_BCCMD_CMD_TIMEOUT, bootstrapTimeoutHandler, 0, bccmdInst);
            }
#endif
            csrSendBccmdData(bccmdInst, bccmd, length, TRUE);
            bootVar->currentIndex += length;
#ifdef CSR_CHIP_MANAGER_ENABLE
            if (bootVar->resetInitiated)
            {
                CsrBccmdCmResetDetectedRequest(bccmdInst);
            }
#endif
        }
        else
        {
            csrBccmdBootstrapCfmHandler(bccmdInst, CSR_BCCMD_RESULT_ERROR);
        }
    }
    else
    {
        csrBccmdBootstrapCfmHandler(bccmdInst, status);
    }
}

static CsrBccmdBoot *csrBccmdGetBootType(void)
{
    CsrBccmdBoot *bootType = (CsrBccmdBoot *) CsrPmemAlloc(sizeof(CsrBccmdBoot));
    bootType->currentIndex = 0;
    bootType->resetInitiated = FALSE;
    return bootType;
}

void CsrBccmdCfmSend(CsrSchedQid phandle, CsrUint8 *payload, CsrUint16 payloadLength)
{
    CsrUint16 length;
    CsrUint8 *tmpPtr = payload;
    CsrBccmdCfm *prim = (CsrBccmdCfm *) CsrPmemAlloc(sizeof(CsrBccmdCfm));
    prim->type = CSR_BCCMD_CFM;

    CSR_CONVERT_16_FROM_XAP(prim->cmdType, tmpPtr);
    CSR_CONVERT_16_FROM_XAP(length, tmpPtr);
    CSR_CONVERT_16_FROM_XAP(prim->seqNo, tmpPtr);
    CSR_CONVERT_16_FROM_XAP(prim->varId, tmpPtr);
    CSR_CONVERT_16_FROM_XAP(prim->status, tmpPtr);
    prim->payloadLength = (CsrUint16) ((length * 2) - CSR_BCCMD_STD_HEADER_SIZE);
    prim->payload = (CsrUint8 *) CsrPmemAlloc(prim->payloadLength);
    CsrMemCpy(prim->payload, &(payload[CSR_BCCMD_STD_HEADER_SIZE]), prim->payloadLength);
    CsrSchedMessagePut(phandle, CSR_BCCMD_PRIM, prim);
}

void CsrBccmdRemoveBootType(CsrBccmdBoot **bootVar)
{
    CsrBccmdBoot *bootType = *bootVar;

    if (bootType)
    {
        CsrPmemFree(bootType->payload);
        CsrPmemFree(bootType);
        *bootVar = NULL;
    }
}

void CsrBccmdReqHandler(CsrBccmdInstanceData *bccmdInst)
{
    if ((bccmdInst->state == CSR_BCCMD_STATE_TRANSPORT_ACTIVATED) && !bccmdInst->inProgress)
    {
        csrBccmdHandler(bccmdInst, bccmdInst->recvMsgP);
    }
    else
    {
        CsrMessageQueuePush(&bccmdInst->messageQueue, CSR_BCCMD_PRIM, bccmdInst->recvMsgP);
        bccmdInst->recvMsgP = NULL;
    }
}

void CsrBccmdFlushReqHandler(CsrBccmdInstanceData *bccmdInst)
{
    CsrUint16 eventClass;
    void *msg;
    CsrBccmdFlushReq *flushPrim = (CsrBccmdFlushReq *) bccmdInst->recvMsgP;
    CsrMessageQueueType *tempQueue = NULL;

    while (CsrMessageQueuePop(&(bccmdInst->messageQueue), &eventClass, &msg))
    {
        CsrBccmdReq *prim = (CsrBccmdReq *) msg;

        if (prim->phandle == flushPrim->phandle) /* Flush the message */
        {
            CsrPmemFree(prim->payload);
            CsrPmemFree(msg);
        }
        else /* Keep message */
        {
            CsrMessageQueuePush(&tempQueue, eventClass, msg);
        }
    }
    bccmdInst->messageQueue = tempQueue;
    csrBccmdFlushCfmHandler(flushPrim->phandle, CSR_RESULT_SUCCESS);
}

void CsrBccmdBootstrapReqHandler(CsrBccmdInstanceData *bccmdInst)
{
    CsrBccmdBootstrapReq *prim = (CsrBccmdBootstrapReq *) bccmdInst->recvMsgP;

    CSR_LOG_TEXT_CONDITIONAL_ERROR(bccmdInst->state != CSR_BCCMD_STATE_ACTIVATING_TRANSPORT,
        (CsrBccmdLto, 0, "Unexpected CSR_BCCMD_BOOTSTRAP_REQ"));

    bccmdInst->phandle = prim->phandle;
    bccmdInst->bootVar = csrBccmdGetBootType();
    bccmdInst->bootVar->payloadLength = prim->payloadLength;
    bccmdInst->bootVar->payload = prim->payload;
    csrBccmdBootstrapHandler(bccmdInst, CSR_RESULT_SUCCESS);
}

void CsrBccmdResetResHandler(CsrBccmdInstanceData *bccmdInst)
{
    CsrBccmdResetRes *response = bccmdInst->recvMsgP;

    CSR_LOG_TEXT_CONDITIONAL_ERROR(bccmdInst->state != CSR_BCCMD_STATE_ACTIVATING_TRANSPORT,
        (CsrBccmdLto, 0, "Unexpected CSR_BCCMD_RESET_RES"));

    if (bccmdInst->bootVar && (response->status == CSR_RESULT_SUCCESS))
    {
        bccmdInst->bootVar->resetInitiated = FALSE;
    }
    csrBccmdBootstrapHandler(bccmdInst, response->status);
}

#ifdef CSR_BLUECORE_ONOFF
static void getBuildIdTimeoutHandler(CsrUint16 mi, void *mv)
{
    CsrBccmdInstanceData *bccmdInst = mv;
    bccmdInst->timerId = CSR_SCHED_TID_INVALID;
    CSR_LOG_TEXT_ERROR((CsrBccmdLto, 0, "Get build ID timeout"));
    csrBccmdGetBuildIdCfmHandler(bccmdInst->phandle, NULL, CSR_RESULT_FAILURE);
}
#endif

void CsrBccmdGetBuildIdHandler(CsrBccmdInstanceData *bccmdInst)
{
    CsrBccmdGetBuildIdReq *prim = bccmdInst->recvMsgP;
    CsrUint16 bccmdLength;
    CsrUint8 *bccmd;

    CSR_LOG_TEXT_CONDITIONAL_ERROR(bccmdInst->state != CSR_BCCMD_STATE_ACTIVATING_TRANSPORT,
        (CsrBccmdLto, 0, "Unexpected CSR_GET_BUILD_ID_REQ"));

#ifdef CSR_BLUECORE_ONOFF
    bccmdInst->timerId = CsrSchedTimerSet(CSR_BCCMD_CMD_TIMEOUT, getBuildIdTimeoutHandler, 0, bccmdInst);
#endif
    bccmd = CsrBccmdBccmdCommandBuild(CSR_BCCMD_GET_REQ, CSR_BCCMD_BUILD_ID_VARID, 0, 0, NULL, &bccmdLength);
    bccmdInst->phandle = prim->phandle;
    csrSendBccmdData(bccmdInst, bccmd, bccmdLength, TRUE);
}

#ifdef CSR_CHIP_MANAGER_ENABLE
void CsrBccmdPanicArgsHandler(CsrBccmdInstanceData *bccmdInst)
{
    CsrBccmdPanicArgsReq *prim = bccmdInst->recvMsgP;
    CsrUint16 bccmdLength;
    CsrUint8 *bccmd;

    CSR_LOG_TEXT_CONDITIONAL_ERROR(bccmdInst->state != CSR_BCCMD_STATE_ACTIVATING_TRANSPORT,
        (CsrBccmdLto, 0, "Unexpected CSR_BCCMD_PANIC_ARGS_REQ"));

    bccmd = CsrBccmdBccmdCommandBuild(CSR_BCCMD_GET_REQ, CSR_BCCMD_PANIC_ARG_VARID, 0, 0, NULL, &bccmdLength);
    bccmdInst->phandle = prim->phandle;
    csrSendBccmdData(bccmdInst, bccmd, bccmdLength, TRUE);
}
#endif

CsrBool CsrBccmdHciVendorSpecificEventIndHandler(CsrBccmdInstanceData *bccmdInst)
{
    CsrHciVendorSpecificEventInd *prim = bccmdInst->recvMsgP;
    CsrUint16 dataLen;
    CsrUint8 *data;

    dataLen = CsrMblkGetLength(prim->data);
    data = CsrMblkMap(prim->data, 0, dataLen);

#ifdef CSR_CHIP_MANAGER_ENABLE
    if (!CsrBccmdCmResHandler(bccmdInst, data))
    {
        CsrMblkUnmap(prim->data, data);
        CsrMblkDestroy(prim->data);
        return FALSE;
    }
#endif
    if (bccmdInst->state == CSR_BCCMD_STATE_TRANSPORT_ACTIVATED)
    {
#ifdef CSR_BLUECORE_ONOFF
        CsrSchedTimerCancel(bccmdInst->timerId, NULL, NULL);
        bccmdInst->timerId = CSR_SCHED_TID_INVALID;
#endif
        CsrBccmdCfmSend(bccmdInst->phandle, data, dataLen);
    }
    else if (bccmdInst->state == CSR_BCCMD_STATE_ACTIVATING_TRANSPORT)
    {
        if (bccmdInst->bootVar && bccmdInst->bootVar->resetInitiated)
        {
            /* A warm or cold reset has been initiated, and a GETRESP command
            has reach the caller. Just ignore it. Note it is highly probable
            that this command not will reach the caller */
        }
        else
        {
            CsrUint16 varId = CSR_BCCMD_INVALID_VARID;
            CsrResult status = CSR_BCCMD_RESULT_ERROR;
            if (data && (dataLen >= CSR_BCCMD_MIN_SIZE))
            {
                status = csrBccmdGetStatus(data);
                varId = csrBccmdGetVarid(data);
            }

            if (varId == CSR_BCCMD_BUILD_ID_VARID)
            {
#ifdef CSR_BLUECORE_ONOFF
                CsrSchedTimerCancel(bccmdInst->timerId, NULL, NULL);
                bccmdInst->timerId = CSR_SCHED_TID_INVALID;
#endif
                csrBccmdGetBuildIdCfmHandler(bccmdInst->phandle, data, status);
            }
#ifdef CSR_CHIP_MANAGER_ENABLE
            else if (varId == CSR_BCCMD_PANIC_ARG_VARID)
            {
                csrBccmdGetPanicArgCfmHandler(bccmdInst, data, dataLen, status);
            }
            else if (varId == CSR_BCCMD_FAULT_ARG_VARID)
            {
                csrBccmdGetFaultArgCfmHandler(bccmdInst, data, dataLen, status);
            }
#endif
            else
            {
#ifdef CSR_BLUECORE_ONOFF
                CsrSchedTimerCancel(bccmdInst->timerId, NULL, NULL);
                bccmdInst->timerId = CSR_SCHED_TID_INVALID;
#endif
                csrBccmdBootstrapHandler(bccmdInst, status);
            }
        }
    }
    CsrMblkUnmap(prim->data, data);
    CsrMblkDestroy(prim->data);
    return TRUE;
}

void CsrBccmdRestoreMessageHandler(CsrBccmdInstanceData *bccmdInst)
{
    if (bccmdInst->state == CSR_BCCMD_STATE_TRANSPORT_ACTIVATED)
    {
        CsrUint16 event;
        void *message = NULL;

        if (CsrMessageQueuePop(&bccmdInst->messageQueue, &event, &message))
        {
            csrBccmdHandler(bccmdInst, message);
        }
        else
        {
            bccmdInst->inProgress = FALSE;
        }
        CsrPmemFree(message);
    }
}

#ifndef CSR_BLUECORE_ONOFF
void CsrTmBlueCoreActivateTransportCfmHandler(CsrBccmdInstanceData *bccmdInst)
{
    CsrTmBluecoreActivateTransportCfm *prim = (CsrTmBluecoreActivateTransportCfm *) bccmdInst->recvMsgP;

    if (prim->result == CSR_RESULT_SUCCESS)
    {
        bccmdInst->state = CSR_BCCMD_STATE_TRANSPORT_ACTIVATED;
#ifdef CSR_CHIP_MANAGER_ENABLE
        CsrBccmdCmResendStart(bccmdInst);
#endif
        CsrBccmdRestoreMessageHandler(bccmdInst);
    }
    else
    {
        CSR_LOG_TEXT_WARNING((CsrBccmdLto, 0,
                              "CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_CFM failed: %u", prim->result));
    }
}
#endif

#ifdef CSR_CHIP_MANAGER_ENABLE
void CsrTmBlueCoreCmReplayRegisterCfmHandler(CsrBccmdInstanceData *bccmdInst)
{
    /* do nothing */
    CSR_UNUSED(bccmdInst);
}

void CsrTmBlueCoreCmReplayStartIndHandler(CsrBccmdInstanceData *bccmdInst)
{
    bccmdInst->state = CSR_BCCMD_STATE_TRANSPORT_ACTIVATED;
    CsrBccmdCmRestartDetectedRequest(bccmdInst);
    CsrBccmdCmResendStart(bccmdInst);
    CsrBccmdRestoreMessageHandler(bccmdInst);
    CsrTmBlueCoreCmReplayStartResSend(CSR_BCCMD_IFACEQUEUE, CSR_RESULT_SUCCESS);
}

void CsrTmBlueCoreCmStatusResetIndHandler(CsrBccmdInstanceData *bccmdInst)
{
    bccmdInst->state = CSR_BCCMD_STATE_ACTIVATING_TRANSPORT;
    CsrBccmdCmResetDetectedRequest(bccmdInst);
    CsrBccmdRemoveBootType(&(bccmdInst->bootVar));
}

#endif
