/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_usr_config.h"
#include "csr_types.h"
#include "csr_sched.h"
#include "csr_util.h"
#include "csr_message_queue.h"
#include "csr_fp_sef.h"
#include "csr_fp_prim.h"
#include "csr_usr_config.h"
#include "csr_bccmd_lib.h"
#include "csr_transport.h"
#include "csr_hci_lib.h"
#include "csr_fp_util.h"
#include "csr_fp_task.h"
#include "csr_fp_handler.h"
#include "csr_log_text_2.h"

#define containerIsValidFpHandle(_list, _fpHandle)                      \
    ((_fpHandle) > 0 && (_fpHandle) <= CSR_FP_NUM_MAX                \
     && NULL != (_list)[(_fpHandle)]                                    \
     && 0 != (_list)[(_fpHandle)]->aclHandle)

#define containerIsEmpty(_List, _fpHandle) (NULL == (_list)[(_fpHandle)]->msg)

#define containerIsPacketSendNotStarted(_list, _fpHandle) ((_list)[(_fpHandle)]->offset)

#define containerSetDestroyNotch(_list, _fpHandle, _value) (_list)[(_fpHandle)]->destroyNotch = (_value)

#define containerSetClearNotch(_list, _fpHandle, _value) (_list)[(_fpHandle)]->clearNotch = (_value)

#define containerUpdateCredits(_list, _fpHandle, _credit)               \
    {                                                                       \
        if (containerIsValidFpHandle(_list, _fpHandle))                     \
        {                                                                   \
            _list[_fpHandle]->creditTx += (CsrUint32) _credit;               \
        }                                                                   \
    }

#define CsrFpListInit(_list) {(_list)->next = (_list);(_list)->previous = (_list);}

/* Used to remap codes s.t. orig. succes (0x000C)
 * starts at 0x0000 (CSR_RESULT_SUCCESS) */
#define CSR_FP_RESULT_REMAP_BASE (0x000C)

static void clistAdd2Bottom(CsrFpClistType *_list, void *data)
{
    CsrFpClistType *n = CsrPmemAlloc(sizeof(CsrFpClistType));

    n->next = _list;
    n->previous = _list->previous;
    n->data = data;
    _list->previous->next = n;
    _list->previous = n;
}

static void *clistGetDataFromTop(CsrFpClistType *_list)
{
    return (_list->next == _list) ? NULL : _list->next->data;
}

static void clistRemoveFromTop(CsrFpClistType *_list)
{
    CsrFpClistType *r = _list->next;

    _list->next = r->next;
    r->next->previous = _list;
    CsrPmemFree(r);
}

static CsrResult resultMap(CsrUint16 value)
{
    if (value > CSR_FP_RESULT_REMAP_BASE)
    {
        return value;
    }

    return (value + 1) % (CSR_FP_RESULT_REMAP_BASE + 1);
}

void CsrFpClistClear(CsrFpClistType *list)
{
    CsrFpWriteReq *msg;
    /* destroy all entries in cSendList */
    while (NULL != (msg = clistGetDataFromTop(list)))
    {
        if (msg->data)
        {
            CsrMblkDestroy(msg->data);
        }
        CsrPmemFree(msg);
        clistRemoveFromTop(list);
    }
}

CsrUint8 CsrFpContainerCreate(CsrFpInstType *inst)
{
    CsrUint8 i, n = 1;

    for (i = inst->last; n <= CSR_FP_NUM_MAX; n++, i++)
    {
        if (i > CSR_FP_NUM_MAX)
        {
            i = 1;
        }
        if (NULL == inst->list[i])
        {
            inst->list[i] = CsrPmemZalloc(sizeof(CsrFpContainerType));
            inst->last = (CsrFpHandleType) (i + 1);
            break;
        }
    }
    if (n > CSR_FP_NUM_MAX)
    {
        i = 0;
    }
    return i;
}

void CsrFpContainerDestroy(CsrFpInstType *inst, CsrUint8 pipeId)
{
    if (inst->list[pipeId] != NULL)
    {
        CsrFpClistClear(&inst->list[pipeId]->cSendListBottom);
        CsrPmemFree(inst->list[pipeId]);
        inst->list[pipeId] = NULL;
    }
}

void CsrFpContainerDestroyAll(CsrFpInstType *inst)
{
    CsrUint8 pipeId = CSR_FP_NUM_MAX;
    do
    {
        CsrFpContainerDestroy(inst, pipeId);
    } while (pipeId-- > 0);
}

static void containerInit(CsrFpContainerType *container, CsrSchedQid appHandle)
{
    container->appHandle = appHandle;
    CsrFpListInit(&container->cSendListBottom);
}

static void bccmdFpEnableReqSend()
{
    CsrUint8 *msg = CsrPmemZalloc(BCCMDPDU_FASTPIPE_ENABLE_LENGTH);

    /* Payload */
    SET_XAP_UINT32(&msg[0], CSR_FP_LIMIT_HOST);

    CsrBccmdWriteReqSend(CSR_FP_IFACEQUEUE, BCCMDVARID_FASTPIPE_ENABLE, 0 /*seqNo*/,
        BCCMDPDU_FASTPIPE_ENABLE_LENGTH, msg);
}

static void bccmdFpCreateReqSend(CsrUint8 pipeId,
    CsrUint32 overheadHost,          /* Pipe overhead on the host */
    CsrUint32 capacityRxHost,        /* Capacity of receive buffer on the host */
    CsrUint32 requiredTxController,  /* Required capacity of tx buffer on controller */
    CsrUint32 desiredTxController,   /* Desired capacity of tx buffer on controller */
    CsrUint32 requiredRxController,  /* Required capacity of rx buffer on controller */
    CsrUint32 desiredRxController)   /* Desired capacity of rx buffer on controller */
{
    CsrUint8 *msg = CsrPmemZalloc(BCCMDPDU_FASTPIPE_CREATE_LENGTH);

    /* Payload */
    SET_XAP_UINT16(&msg[0], pipeId);
    SET_XAP_UINT32(&msg[2], overheadHost);
    SET_XAP_UINT32(&msg[6], capacityRxHost);
    SET_XAP_UINT32(&msg[10], requiredTxController);
    SET_XAP_UINT32(&msg[14], desiredTxController);
    SET_XAP_UINT32(&msg[18], requiredRxController);
    SET_XAP_UINT32(&msg[22], desiredRxController);

    CsrBccmdWriteReqSend(CSR_FP_IFACEQUEUE, BCCMDVARID_FASTPIPE_CREATE, 0 /*seqNo*/,
        BCCMDPDU_FASTPIPE_CREATE_LENGTH, msg);
}

static void bccmdFpDestroyReqSend(CsrFpHandleType pipeId)
{
    CsrUint8 *msg = CsrPmemZalloc(BCCMDPDU_FASTPIPE_DESTROY_LENGTH);

    /* Payload */
    SET_XAP_UINT16(&msg[0], pipeId);

    CsrBccmdWriteReqSend(CSR_FP_IFACEQUEUE, BCCMDVARID_FASTPIPE_DESTROY, 0 /*seqNo*/,
        BCCMDPDU_FASTPIPE_DESTROY_LENGTH, msg);
}

static void bccmdFpResizeReqSend()
{
    CsrUint8 *msg = CsrPmemZalloc(BCCMDPDU_FASTPIPE_RESIZE_LENGTH);

    /* Payload */
    SET_XAP_UINT32(&msg[0], CSR_FP_LIMIT_HOST);

    CsrBccmdWriteReqSend(CSR_FP_IFACEQUEUE, BCCMDVARID_FASTPIPE_RESIZE, 0 /*seqNo*/,
        BCCMDPDU_FASTPIPE_RESIZE_LENGTH, msg);
}

static CsrUint32 fpSendDataChunk(CsrFpContainerType *container)
{
    CsrUint32 sendNofWriteCfm = 0;

    while (container->creditTx > 0)
    {
        CsrFpWriteReq *msg = clistGetDataFromTop(&container->cSendListBottom);

        if (NULL != msg)
        {
            CsrUint32 len, mblkLength;
            CsrMblk *mblkToSend;

            mblkLength = CsrMblkGetLength(msg->data);
            len = mblkLength - container->offset;

            if (len > container->creditTx)
            {
                len = container->creditTx;
            }

            if (len > container->creditTxMax)
            {
                len = container->creditTxMax;
            }

            mblkToSend = CsrMblkDuplicateRegion(msg->data, (CsrMblkSize) container->offset, (CsrMblkSize) len);

            CsrHciAclDataReqSend(container->aclHandle,
                mblkToSend
                );

            /* adjust credits */
            if (len <= container->creditTx)
            {
                container->creditTx -= len;
            }
            if (container->offset + len >= mblkLength)
            {
                container->offset = 0;
                CsrMblkDestroy(msg->data);
                CsrPmemFree(msg);
                clistRemoveFromTop(&container->cSendListBottom);
                sendNofWriteCfm++;
            }
            else
            {
                container->offset += len;
            }
        }
        else
        {
            break;
        }
    }
    return sendNofWriteCfm;
}

static void fpSendCreditToken(CsrFpContainerType *container,
    CsrUint8 pipeId,
    CsrUint16 credit)
{
    CsrUint8 *data;
    CsrMblk *mblk = CsrMblkMallocCreate((void **) &data, CSR_FP_TOKEN_SIZE);

    data[0] = pipeId;
    INV_XAP_UINT16(&data[1], credit);

    CsrHciAclDataReqSend(container->aclHandle, mblk);
}

static void bccmdEnableCfmHandler(CsrFpInstType *inst, CsrUint16 *data)
{
    CsrResult result = resultMap(GET_XAP_UINT16(&data[2]));
    CSR_LOG_TEXT_CONDITIONAL_ERROR(
        result != CSR_RESULT_SUCCESS,
        (CsrFpLto, 0,
         "Enable failed: %u", result));
    CSR_UNUSED(result);

    bccmdFpResizeReqSend();
}

static void bccmdResizeCfmHandler(CsrFpInstType *inst, CsrUint16 *data)
{
    CsrResult result = resultMap(GET_XAP_UINT16(&data[4]));
    CSR_LOG_TEXT_CONDITIONAL_ERROR(
        result != CSR_RESULT_SUCCESS,
        (CsrFpLto, 0,
         "Resize failed: %u", result));
    CSR_UNUSED(result);

    bccmdFpCreateReqSend(0,
        CSR_FP_CTRL_OVERHEAD_HOST,
        CSR_FP_CTRL_CAPACITY_RX_HOST,
        CSR_FP_CTRL_REQUIRED_TX_CONTROLLER,
        CSR_FP_CTRL_DESIRED_TX_CONTROLLER,
        CSR_FP_CTRL_REQUIRED_RX_CONTROLLER,
        CSR_FP_CTRL_DESIRED_RX_CONTROLLER);
}

static void bccmdCreateCfmHandler(CsrFpInstType *inst, CsrUint16 *data)
{
    CsrResult result = resultMap(GET_XAP_UINT16(&data[20]));
    CsrUint8 pipeId = (CsrUint8) GET_XAP_UINT16(&data[0]);
    CsrUint32 overheadController = 0;               /* Pipe overhead on the controller */
    CsrUint32 capacityTxController = 0;             /* Capacity of transmit buffer on controller */
    CsrUint32 capacityRxController = 0;             /* Capacity of receive buffer on controller */
    CsrSchedQid appHandle = CSR_SCHED_QID_INVALID;

    if ((pipeId <= CSR_FP_NUM_MAX) && (NULL != inst->list[pipeId]))
    {
        appHandle = inst->list[pipeId]->appHandle;

        if (result == CSR_RESULT_SUCCESS)
        {
            if (pipeId == 0)
            {
                inst->state = CSR_FP_ACTIVE_ST;
                CsrMessageQueueRestore(&inst->queue, CSR_FP_IFACEQUEUE);
            }
            inst->list[pipeId]->creditRx = GET_XAP_UINT32(&data[3]);
            overheadController = GET_XAP_UINT32(&data[13]);
            capacityTxController = GET_XAP_UINT32(&data[15]);
            capacityRxController = GET_XAP_UINT32(&data[17]);
            inst->list[pipeId]->creditTx = capacityRxController;
            if (inst->list[pipeId]->creditTx < inst->list[pipeId]->creditTxMax)
            {
                inst->list[pipeId]->creditTxMax = inst->list[pipeId]->creditTx;
            }
            inst->list[pipeId]->aclHandle = GET_XAP_UINT16(&data[19]);

            CsrHciRegisterAclHandlerReqSend(CSR_FP_IFACEQUEUE,
                inst->list[pipeId]->aclHandle);
        }
        else if (pipeId > 0)
        {
            CsrFpContainerDestroy(inst, pipeId);
        }
        else
        {
            CSR_LOG_TEXT_ERROR((CsrFpLto, 0,
                                "Create credit pipe failed: %u", result));
        }
    }
    else
    {
        CSR_LOG_TEXT_ERROR((CsrFpLto, 0,
                            "Invalid pipe (%u) in BCCMD 0x%04X", pipeId, BCCMDVARID_FASTPIPE_CREATE));
    }

    if (appHandle != CSR_SCHED_QID_INVALID)
    {
        fpCreateCfmSend(appHandle,
            (CsrFpHandleType) pipeId,
            overheadController,
            capacityRxController,
            capacityTxController,
            result);
    }
}

static void bccmdDestroyCfmHandler(CsrFpInstType *inst, CsrUint16 *data)
{
    CsrResult result = resultMap(GET_XAP_UINT16(&data[1]));
    CsrUint8 pipeId = (CsrUint8) GET_XAP_UINT16(&data[0]);
    CsrSchedQid appHandle = CSR_SCHED_QID_INVALID;

    if ((pipeId <= CSR_FP_NUM_MAX) && (pipeId > 0) && (NULL != inst->list[pipeId]))
    {
        appHandle = inst->list[pipeId]->appHandle;

        CSR_LOG_TEXT_CONDITIONAL_WARNING(
            result != CSR_RESULT_SUCCESS,
            (CsrFpLto, 0, "Destroy pipe (%u) failed: %u", pipeId, result));

        CsrHciUnregisterAclHandlerReqSend(CSR_FP_IFACEQUEUE,
            inst->list[pipeId]->aclHandle);

        CsrFpContainerDestroy(inst, pipeId);
    }
    else
    {
        CSR_LOG_TEXT_ERROR((CsrFpLto, 0,
                            "Invalid pipe (%u) in BCCMD 0x%04X", pipeId, BCCMDVARID_FASTPIPE_DESTROY));
    }

    if (appHandle != CSR_SCHED_QID_INVALID)
    {
        fpDestroyCfmSend(appHandle, (CsrFpHandleType) pipeId, result);
    }
}

/************************************************************/
/* CsrFpCreateReqHandler                                    */
/************************************************************/
void CsrFpCreateReqHandler(CsrFpInstType *inst)
{
    CsrFpCreateReq *msg = inst->msg;
    CsrUint8 pipeId;

    if (0 != (pipeId = CsrFpContainerCreate(inst)))
    {
        containerInit(inst->list[pipeId], msg->appHandle);
        inst->list[pipeId]->creditTxMax = msg->desiredRxController;

        bccmdFpCreateReqSend(pipeId,
            msg->overheadHost,
            msg->capacityRxHost,
            msg->requiredTxController,
            msg->desiredTxController,
            msg->requiredRxController,
            msg->desiredRxController);
    }
    else
    {
        fpCreateCfmSend(msg->appHandle, CSR_FP_HANDLE_NONE, 0, 0, 0,
            CSR_FP_RESULT_TOO_MANY_PIPES_ACTIVE);
    }
}

/************************************************************/
/* CsrFpWriteReqHandler                                     */
/************************************************************/
void CsrFpWriteReqHandler(CsrFpInstType *inst)
{
    CsrFpWriteReq *msg = inst->msg;
    CsrUint8 pipeId = msg->fpHandle;

    if (containerIsValidFpHandle(inst->list, pipeId))
    {
        void *data = clistGetDataFromTop(&inst->list[pipeId]->cSendListBottom);

        /* Add to queue */
        clistAdd2Bottom(&inst->list[pipeId]->cSendListBottom, inst->msg);
        inst->list[pipeId]->packetsOnQueue++;
        inst->msg = NULL;

        /* Transmit right away if queue was empty */
        if (data == NULL)
        {
            if (fpSendDataChunk(inst->list[pipeId]))
            {
                inst->list[pipeId]->packetsOnQueue--;
            }
        }

        /* Send confirm immediately if queue is not full */
        if (inst->list[pipeId]->packetsOnQueue < CSR_FP_PACEKTS_MAX)
        {
            fpWriteCfmSend(inst->list[pipeId]->appHandle, pipeId, CSR_RESULT_SUCCESS);
        }
    }
    else
    {
        CSR_LOG_TEXT_ERROR((CsrFpLto, 0,
                            "Invalid pipe (%u) in write", pipeId));
    }
}

/************************************************************/
/* CsrFpClearReqHandler                                     */
/************************************************************/
void CsrFpClearReqHandler(CsrFpInstType *inst)
{
    CsrFpClearReq *msg = inst->msg;
    CsrUint8 pipeId = msg->fpHandle;

    if (containerIsValidFpHandle(inst->list, pipeId))
    {
        containerSetClearNotch(inst->list, pipeId, TRUE);
        if ((NULL == clistGetDataFromTop(&inst->list[pipeId]->cSendListBottom))
            || (0 == containerIsPacketSendNotStarted(inst->list, pipeId)))
        {
            inst->list[pipeId]->clearNotch = FALSE;
            CsrFpClistClear(&inst->list[pipeId]->cSendListBottom);
            inst->list[pipeId]->packetsOnQueue = 0;
            fpClearCfmSend(inst->list[pipeId]->appHandle, pipeId, CSR_RESULT_SUCCESS);
        }
    }
    else
    {
        CSR_LOG_TEXT_ERROR((CsrFpLto, 0,
                            "Invalid pipe (%u) in clear", pipeId));
    }
}

/************************************************************/
/* CsrFpDestroyReqHandler                                   */
/************************************************************/
void CsrFpDestroyReqHandler(CsrFpInstType *inst)
{
    CsrFpDestroyReq *msg = inst->msg;
    CsrUint8 pipeId = msg->fpHandle;

    if (containerIsValidFpHandle(inst->list, pipeId))
    {
        containerSetDestroyNotch(inst->list, pipeId, TRUE);
        if ((NULL == clistGetDataFromTop(&inst->list[pipeId]->cSendListBottom))
            || (0 == containerIsPacketSendNotStarted(inst->list, pipeId)))
        {
            bccmdFpDestroyReqSend(pipeId);
        }
    }
    else
    {
        CSR_LOG_TEXT_ERROR((CsrFpLto, 0,
                            "Invalid pipe (%u) in destroy", pipeId));
    }
}

static void readIndSend(CsrFpInstType *inst, CsrUint8 pipeId, CsrMblk *data)
{
    CsrFpReadInd *msg = CsrPmemAlloc(sizeof(CsrFpReadInd));

    msg->type = CSR_FP_READ_IND;
    msg->fpHandle = pipeId;
    msg->data = data;

    CsrSchedMessagePut(inst->list[pipeId]->appHandle, CSR_FP_PRIM, msg);
}

/************************************************************/
/* CsrFpHciAclDataIndHandler                                */
/************************************************************/
void CsrFpHciAclDataIndHandler(CsrFpInstType *inst)
{
    CsrHciAclDataInd *msg = inst->msg;
    CsrUint16 handle, dataLen;
    CsrUint8 i;

    handle = msg->handlePlusFlags & 0x0FFF; /* mask away flags */
    dataLen = CsrMblkGetLength(msg->data);

    for (i = 0; i <= CSR_FP_NUM_MAX; i++)
    {
        if ((NULL != inst->list[i]) && (inst->list[i]->aclHandle == handle))
        {
            CsrFpHandleType fpHandle;
            /* update credits for pipe */
            inst->list[i]->creditRx -= (dataLen <= inst->list[i]->creditRx) ? dataLen : 0;

            if (0 == i) /* flow control pipe */
            {
                CsrUint8 *data;
                CsrUint16 credit;
                CsrUint32 sendWriteCfm;

                data = CsrMblkMap(msg->data, 0, dataLen);

                fpHandle = (CsrFpHandleType) data[0];
                fpHandle &= 0xf;
                credit = (CsrUint16) ((data[1] << 8) + data[2]);

                CsrMblkUnmap(msg->data, data);
                CsrMblkDestroy(msg->data);

                containerUpdateCredits(inst->list, fpHandle, credit);

                /* send more data on the pipe we have just got credit returned on */
                sendWriteCfm = fpSendDataChunk(inst->list[fpHandle]);
                if (inst->list[fpHandle]->destroyNotch && (0 == inst->list[fpHandle]->offset))
                {
                    bccmdFpDestroyReqSend(fpHandle);
                }
                else if (inst->list[fpHandle]->clearNotch && (0 == inst->list[fpHandle]->offset))
                {
                    inst->list[fpHandle]->clearNotch = FALSE;
                    CsrFpClistClear(&inst->list[fpHandle]->cSendListBottom);
                    inst->list[fpHandle]->packetsOnQueue = 0;
                    fpClearCfmSend(inst->list[fpHandle]->appHandle, fpHandle, CSR_RESULT_SUCCESS);
                }
                else if (sendWriteCfm)
                {
                    /* kick start a new packets request/cfm again */
                    if (inst->list[fpHandle]->packetsOnQueue >= CSR_FP_PACEKTS_MAX)
                    {
                        fpWriteCfmSend(inst->list[fpHandle]->appHandle, fpHandle,
                            CSR_RESULT_SUCCESS);
                    }

                    if (inst->list[fpHandle]->packetsOnQueue > 0)
                    {
                        inst->list[fpHandle]->packetsOnQueue--;
                    }
                }
            }
            else /* data pipe */
            {
                readIndSend(inst, i, msg->data);
            }

            if (CSR_FP_TOKEN_SIZE <= inst->list[0]->creditTx)
            {
                fpSendCreditToken(inst->list[0], i, dataLen);
            }
            inst->list[i]->creditRx += dataLen;
            break;
        }
    }
}

/************************************************************/
/* CsrFpBccmdCfmHandler                                     */
/************************************************************/
void CsrFpBccmdCfmHandler(CsrFpInstType *inst)
{
    CsrBccmdCfm *msg = inst->msg;

    CSR_LOG_TEXT_CONDITIONAL_CRITICAL(
        msg->payload == NULL,
        (CsrFpLto, 0,
         "NULL payload in BCCMD 0x%04X", msg->varId));

    CSR_LOG_TEXT_CONDITIONAL_ERROR(
        (msg->status != CSR_RESULT_SUCCESS) && (msg->status != CSR_BCCMD_RESULT_ERROR),
        (CsrFpLto, 0,
         "Unexpected status in BCCMD 0x%04X: 0x%04X", msg->varId, msg->status));

    switch (msg->varId)
    {
        case BCCMDVARID_FASTPIPE_ENABLE:
            bccmdEnableCfmHandler(inst, (CsrUint16 *) msg->payload);
            break;
        case BCCMDVARID_FASTPIPE_CREATE:
            bccmdCreateCfmHandler(inst, (CsrUint16 *) msg->payload);
            break;
        case BCCMDVARID_FASTPIPE_DESTROY:
            bccmdDestroyCfmHandler(inst, (CsrUint16 *) msg->payload);
            break;
        case BCCMDVARID_FASTPIPE_RESIZE:
            bccmdResizeCfmHandler(inst, (CsrUint16 *) msg->payload);
            break;
        default:
            CSR_LOG_TEXT_WARNING((CsrFpLto, 0,
                                  "Received invalid BCCMD 0x%04X", msg->varId));
            break;
    }
}

void CsrFpActivate(CsrFpInstType *inst)
{
    if (inst->list[0] == NULL)
    {
        inst->list[0] = CsrPmemZalloc(sizeof(CsrFpContainerType));
        containerInit(inst->list[0], CSR_SCHED_QID_INVALID);
        bccmdFpEnableReqSend();
    }
}
