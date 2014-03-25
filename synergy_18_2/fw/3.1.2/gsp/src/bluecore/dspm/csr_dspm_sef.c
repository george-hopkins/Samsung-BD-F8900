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
#include "csr_macro.h"
#include "csr_log_text_2.h"

/* DSPM Headers */
#include "csr_dspm_task.h"
#include "csr_dspm_prim.h"
#include "csr_dspm_lib.h"
#include "csr_dspm_handler.h"
#include "csr_dspm_sef.h"
#include "csr_dspm_util.h"
#include "csr_dspm_lower.h"

/* BCCMD Headers */
#include "csr_bccmd_task.h"
#include "csr_bccmd_prim.h"
#include "csr_bccmd_lib.h"

/* HQ Headers */
#include "csr_hq_task.h"
#include "csr_hq_prim.h"
#include "csr_hq_lib.h"

#ifdef CSR_DSPM_SUPPORT_CAPABILITY_DOWNLOAD
/* FP Headers */
#include "csr_fp_task.h"
#include "csr_fp_prim.h"
#include "csr_fp_lib.h"
#endif

#ifdef CSR_BLUECORE_ONOFF
/* TM Headers */
#include "csr_tm_bluecore_task.h"
#include "csr_tm_bluecore_prim.h"
#include "csr_tm_bluecore_lib.h"
#endif


/*** Definitions ***/

#define BCCMD_VARID_BUILD_ID                        0x2819
#define BCCMD_VARID_STREAM_GET_SOURCE               0x505A
#define BCCMD_VARID_STREAM_CLOSE_SOURCE             0x486B
#define BCCMD_VARID_STREAM_GET_SINK                 0x505B
#define BCCMD_VARID_STREAM_CLOSE_SINK               0x486C
#define BCCMD_VARID_STREAM_CONFIGURE                0x505C
#define BCCMD_VARID_ENABLE_SCO_STREAMS              0x4876
#define BCCMD_VARID_STREAM_SYNC_SID                 0x5062
#define BCCMD_VARID_CREATE_OPERATOR_C               0x5075
#define BCCMD_VARID_CREATE_OPERATOR_P               0x5076
#define BCCMD_VARID_DESTROY_OPERATORS               0x5073
#define BCCMD_VARID_OPERATOR_MESSAGE                0x5074
#define BCCMD_VARID_START_OPERATORS                 0x5070
#define BCCMD_VARID_STOP_OPERATORS                  0x5071
#define BCCMD_VARID_RESET_OPERATORS                 0x5072
#define BCCMD_VARID_STREAM_CONNECT                  0x505E
#define BCCMD_VARID_STREAM_TRANSFORM_DISCONNECT     0x486D
#define BCCMD_VARID_DOWNLOAD_CAPABILITY             0x5077
#define BCCMD_VARID_REMOVE_DOWNLOADED_CAPABILITY    0x487D

#define BCCMD_HEADER_LENGTH 10

#define HQ_VARID_MESSAGE_FROM_OPERATOR              0x101A
#define HQ_VARID_CAPABILITY_DOWNLOAD_COMPLETE       0x0803
#define HQ_VARID_CAPABILITY_DOWNLOAD_FAILED         0x101B
#define HQ_VARID_DSPMANAGER_CONFIG_REQUEST          0x101C

#define HQ_HEADER_LENGTH 10

#define FP_OVERHEAD_HOST            0
#define FP_CAPACITY_RX_HOST         0
#define FP_REQUIRED_TX_CONTROLLER   0
#define FP_DESIRED_TX_CONTROLLER    0
#define FP_REQUIRED_RX_CONTROLLER   64
#define FP_DESIRED_RX_CONTROLLER    2048


/*** DSPM Lower ***/

static CsrDspmConfigAcquireFunction configAcquire = NULL;
void CsrDspmConfigAcquireFunctionRegister(CsrDspmConfigAcquireFunction configAcquireFunction)
{
    configAcquire = configAcquireFunction;
}

static CsrDspmOperatorPatchAcquireFunction operatorPatchAcquire = NULL;
void CsrDspmOperatorPatchAcquireFunctionRegister(CsrDspmOperatorPatchAcquireFunction operatorPatchAcquireFunction)
{
    operatorPatchAcquire = operatorPatchAcquireFunction;
}

/*** DSPM ***/

/* Send a BCCMD with a variable number of arguments */
static void bccmd(CsrUint16 varId, CsrUint16 seqNo, CsrSize args, ...)
{
    CsrSize payloadLength = args * sizeof(CsrUint16);
    CsrUint8 *payload = CsrPmemAlloc(payloadLength);
    CsrUint8 *tmpPayload = payload;
    va_list list;

    CSR_LOG_TEXT_ASSERT(CsrDspmLto, 0, args <= ((65535 - BCCMD_HEADER_LENGTH) / 2))

    va_start(list, args);
    while (args--)
    {
        CsrUint16 arg = (CsrUint16) va_arg(list, CsrSize);
        CSR_ADD_UINT16_TO_XAP(tmpPayload, arg);
    }
    va_end(list);

    CsrBccmdWriteReqSend(CSR_DSPM_IFACEQUEUE, varId, seqNo, (CsrUint16) payloadLength, payload);
}

/* Send a BCCMD based on a list of values. listCount is added to the command
   before the list contents. */
static void bccmdList(CsrUint16 varId, CsrUint16 seqNo, CsrUint16 listCount, CsrUint16 list[])
{
    CsrUint16 payloadLength = (listCount + 1) * sizeof(CsrUint16);
    CsrUint8 *payload = CsrPmemAlloc(payloadLength);
    CsrUint8 *tmpPayload = payload;
    CsrUint8 i;

    CSR_LOG_TEXT_ASSERT(CsrDspmLto, 0, (listCount + 1) <= ((65535 - BCCMD_HEADER_LENGTH) / 2))

    CSR_ADD_UINT16_TO_XAP(tmpPayload, listCount);
    for (i = 0; i < listCount; ++i)
    {
        CSR_ADD_UINT16_TO_XAP(tmpPayload, list[i]);
    }

    CsrBccmdWriteReqSend(CSR_DSPM_IFACEQUEUE, varId, seqNo, payloadLength, payload);
}

static CsrBool isInvalidQid(CsrDspmInstanceData *instanceData, CsrSchedQid qid)
{
    CsrUint16 value = 0;
    return CsrDspmInfoTupleLookup(&instanceData->users, qid, &value) != CSR_RESULT_SUCCESS;
}

CsrBool CsrDspmRequestHandler(CsrDspmInstanceData *instanceData)
{
    CsrPrim *type = instanceData->currentRequest;
    CsrBool done = FALSE;

    /* Switch on the type of DSPM request to handle */
    switch (*type)
    {
        case CSR_DSPM_ACTIVATE_REQ:
        {
            CsrDspmActivateReq *request = instanceData->currentRequest;
            CsrBool first = instanceData->users == NULL;

            CsrDspmInfoTupleInsert(&instanceData->users, request->qid, 0);

            if (first)
            {
                CsrUint16 payloadLength = 4;
                CsrUint8 *payload = CsrPmemZalloc(payloadLength);
                CsrBccmdReadReqSend(CSR_DSPM_IFACEQUEUE, BCCMD_VARID_BUILD_ID,
                    instanceData->bccmdSequenceNumber++, payloadLength, payload);
            }
            else
            {
                CsrDspmActivateCfmSend(request->qid);
                done = TRUE;
            }

            break;
        }
        case CSR_DSPM_DEACTIVATE_REQ:
        {
            CsrDspmDeactivateReq *request = instanceData->currentRequest;

            if (instanceData->users != NULL)
            {
                if (CsrDspmInfoTupleRemove(&instanceData->users, request->qid) == CSR_RESULT_SUCCESS)
                {
                    CsrDspmDeactivateCfmSend(request->qid);
                }
            }
            done = TRUE;
            break;
        }
        case CSR_DSPM_STREAM_CP_OPEN_REQ:
        {
            CsrDspmStreamCpOpenReq *request = instanceData->currentRequest;
            CsrDspmCpInfo *cpInfo;

            done = isInvalidQid(instanceData, request->qid);
            if (!done)
            {
                CSR_LOG_TEXT_ASSERT(CsrDspmLto, 0, request->cpInfoCount > instanceData->currentRequestIndex);
                cpInfo = &request->cpInfo[instanceData->currentRequestIndex];

                if (cpInfo->cpid == CSR_DSPM_CPID_SOURCE)
                {
                    bccmd(BCCMD_VARID_STREAM_GET_SOURCE,
                        instanceData->bccmdSequenceNumber++,
                        3, cpInfo->cpType, cpInfo->instance, cpInfo->channel);
                }
                else if (cpInfo->cpid == CSR_DSPM_CPID_SINK)
                {
                    bccmd(BCCMD_VARID_STREAM_GET_SINK,
                        instanceData->bccmdSequenceNumber++,
                        3, cpInfo->cpType, cpInfo->instance, cpInfo->channel);
                }
                else
                {
                    CSR_LOG_TEXT_ERROR((CsrDspmLto, 0, "Invalid cpid in request 0x%04X\n", *type));
                }
            }
            break;
        }
        case CSR_DSPM_STREAM_CP_CLOSE_REQ:
        {
            CsrDspmStreamCpCloseReq *request = instanceData->currentRequest;
            CsrDspmCpid cpid;

            done = isInvalidQid(instanceData, request->qid);
            if (!done)
            {
                CSR_LOG_TEXT_ASSERT(CsrDspmLto, 0, request->cpidCount > instanceData->currentRequestIndex);
                cpid = request->cpid[instanceData->currentRequestIndex];

                if (CSR_DSPM_CPID_IS_SOURCE(cpid))
                {
                    bccmd(BCCMD_VARID_STREAM_CLOSE_SOURCE,
                        instanceData->bccmdSequenceNumber++,
                        1, cpid);
                }
                else if (CSR_DSPM_CPID_IS_SINK(cpid))
                {
                    bccmd(BCCMD_VARID_STREAM_CLOSE_SINK,
                        instanceData->bccmdSequenceNumber++,
                        1, cpid);
                }
                else
                {
                    CSR_LOG_TEXT_ERROR((CsrDspmLto, 0, "Invalid cpid in request 0x%04X\n", *type));
                }
            }
            break;
        }
        case CSR_DSPM_STREAM_SCO_ENABLE_REQ:
        {
            CsrDspmStreamScoEnableReq *request = instanceData->currentRequest;

            done = isInvalidQid(instanceData, request->qid);
            if (!done)
            {
                bccmd(BCCMD_VARID_ENABLE_SCO_STREAMS,
                    instanceData->bccmdSequenceNumber++,
                    1, request->enable ? 1 : 0);
            }
            break;
        }
        case CSR_DSPM_STREAM_CONFIGURE_REQ:
        {
            CsrDspmStreamConfigureReq *request = instanceData->currentRequest;
            CsrDspmConfigInfo *configInfo;

            done = isInvalidQid(instanceData, request->qid);
            if (!done)
            {
                CSR_LOG_TEXT_ASSERT(CsrDspmLto, 0, request->configInfoCount > instanceData->currentRequestIndex);
                configInfo = &request->configInfo[instanceData->currentRequestIndex];

                bccmd(BCCMD_VARID_STREAM_CONFIGURE,
                    instanceData->bccmdSequenceNumber++,
                    4, configInfo->cpid, configInfo->feature,
                    (CsrUint16) ((configInfo->value >> 16) & 0xFFFF),
                    (CsrUint16) ((configInfo->value) & 0xFFFF));
            }
            break;
        }
        case CSR_DSPM_STREAM_SYNC_REQ:
        {
            CsrDspmStreamSyncReq *request = instanceData->currentRequest;
            CsrDspmSyncInfo *syncInfo;

            done = isInvalidQid(instanceData, request->qid);
            if (!done)
            {
                CSR_LOG_TEXT_ASSERT(CsrDspmLto, 0, request->syncInfoCount > instanceData->currentRequestIndex);
                syncInfo = &request->syncInfo[instanceData->currentRequestIndex];

                bccmd(BCCMD_VARID_STREAM_SYNC_SID,
                    instanceData->bccmdSequenceNumber++,
                    2, syncInfo->cpid1, syncInfo->cpid2);
            }
            break;
        }
        case CSR_DSPM_OPERATOR_CREATE_REQ:
        {
            CsrDspmOperatorCreateReq *request = instanceData->currentRequest;
            CsrDspmOperatorInfo *operatorInfo;
            CsrUint16 itemCount = 0, skipCount = 0;

            done = isInvalidQid(instanceData, request->qid);
            if (!done)
            {
                CSR_LOG_TEXT_ASSERT(CsrDspmLto, 0, request->operatorInfoCount > instanceData->currentRequestIndex);
                operatorInfo = &request->operatorInfo[instanceData->currentRequestIndex];

                if (operatorPatchAcquire != NULL)
                {
                    operatorPatchAcquire(instanceData->buildId, operatorInfo->capability, 0, &itemCount, &skipCount, NULL);
                }
                instanceData->itemIndex = skipCount + 1;
                bccmd(BCCMD_VARID_CREATE_OPERATOR_C,
                    instanceData->bccmdSequenceNumber++,
                    3, operatorInfo->capability, itemCount, skipCount);
            }
            break;
        }
        case CSR_DSPM_OPERATOR_DESTROY_REQ:
        {
            CsrDspmOperatorDestroyReq *request = instanceData->currentRequest;

            done = isInvalidQid(instanceData, request->qid);
            if (!done)
            {
                CSR_LOG_TEXT_ASSERT(CsrDspmLto, 0, request->opidCount > 0);
                CSR_LOG_TEXT_ASSERT(CsrDspmLto, 0, request->opid != NULL);
                bccmdList(BCCMD_VARID_DESTROY_OPERATORS,
                    instanceData->bccmdSequenceNumber++,
                    request->opidCount, request->opid);
            }
            break;
        }
        case CSR_DSPM_OPERATOR_MESSAGE_REQ:
        {
            CsrDspmOperatorMessageReq *request = instanceData->currentRequest;
            CsrDspmOperatorMessageInfo *operatorMessageInfo;
            CsrUint16 payloadLength, i;
            CsrUint8 *payload, *tmpPayload;

            done = isInvalidQid(instanceData, request->qid);
            if (!done)
            {
                CSR_LOG_TEXT_ASSERT(CsrDspmLto, 0, request->operatorMessageInfoCount > instanceData->currentRequestIndex);
                operatorMessageInfo = &request->operatorMessageInfo[instanceData->currentRequestIndex];

                payloadLength = (operatorMessageInfo->messageLength + 1) * sizeof(CsrUint16);
                tmpPayload = payload = CsrPmemAlloc(payloadLength);

                CSR_ADD_UINT16_TO_XAP(tmpPayload, operatorMessageInfo->opid);
                for (i = 0; i < operatorMessageInfo->messageLength; ++i)
                {
                    CSR_ADD_UINT16_TO_XAP(tmpPayload, operatorMessageInfo->message[i]);
                }

                CsrBccmdWriteReqSend(CSR_DSPM_IFACEQUEUE, BCCMD_VARID_OPERATOR_MESSAGE,
                    instanceData->bccmdSequenceNumber++, payloadLength, payload);
            }
            break;
        }
        case CSR_DSPM_OPERATOR_START_REQ:
        {
            CsrDspmOperatorStartReq *request = instanceData->currentRequest;

            done = isInvalidQid(instanceData, request->qid);
            if (!done)
            {
                CSR_LOG_TEXT_ASSERT(CsrDspmLto, 0, request->opidCount > 0);
                CSR_LOG_TEXT_ASSERT(CsrDspmLto, 0, request->opid != NULL);
                bccmdList(BCCMD_VARID_START_OPERATORS,
                    instanceData->bccmdSequenceNumber++,
                    request->opidCount, request->opid);
            }
            break;
        }
        case CSR_DSPM_OPERATOR_STOP_REQ:
        {
            CsrDspmOperatorStopReq *request = instanceData->currentRequest;

            done = isInvalidQid(instanceData, request->qid);
            if (!done)
            {
                CSR_LOG_TEXT_ASSERT(CsrDspmLto, 0, request->opidCount > 0);
                CSR_LOG_TEXT_ASSERT(CsrDspmLto, 0, request->opid != NULL);
                bccmdList(BCCMD_VARID_STOP_OPERATORS,
                    instanceData->bccmdSequenceNumber++,
                    request->opidCount, request->opid);
            }
            break;
        }
        case CSR_DSPM_OPERATOR_RESET_REQ:
        {
            CsrDspmOperatorResetReq *request = instanceData->currentRequest;

            done = isInvalidQid(instanceData, request->qid);
            if (!done)
            {
                CSR_LOG_TEXT_ASSERT(CsrDspmLto, 0, request->opidCount > 0);
                CSR_LOG_TEXT_ASSERT(CsrDspmLto, 0, request->opid != NULL);
                bccmdList(BCCMD_VARID_RESET_OPERATORS,
                    instanceData->bccmdSequenceNumber++,
                    request->opidCount, request->opid);
            }
            break;
        }
        case CSR_DSPM_CONNECTION_CREATE_REQ:
        {
            CsrDspmConnectionCreateReq *request = instanceData->currentRequest;
            CsrDspmConnectionInfo *connectionInfo;

            done = isInvalidQid(instanceData, request->qid);
            if (!done)
            {
                CSR_LOG_TEXT_ASSERT(CsrDspmLto, 0, request->connectionInfoCount > instanceData->currentRequestIndex);
                connectionInfo = &request->connectionInfo[instanceData->currentRequestIndex];

                bccmd(BCCMD_VARID_STREAM_CONNECT,
                    instanceData->bccmdSequenceNumber++,
                    2, connectionInfo->cpidSource, connectionInfo->cpidSink);
            }
            break;
        }
        case CSR_DSPM_CONNECTION_DESTROY_REQ:
        {
            CsrDspmConnectionDestroyReq *request = instanceData->currentRequest;
            CsrDspmCid cid;

            done = isInvalidQid(instanceData, request->qid);
            if (!done)
            {
                CSR_LOG_TEXT_ASSERT(CsrDspmLto, 0, request->cidCount > instanceData->currentRequestIndex);
                cid = request->cid[instanceData->currentRequestIndex];

                bccmd(BCCMD_VARID_STREAM_TRANSFORM_DISCONNECT,
                    instanceData->bccmdSequenceNumber++,
                    1, cid);
            }
            break;
        }
#ifdef CSR_DSPM_SUPPORT_CAPABILITY_DOWNLOAD
        case CSR_DSPM_CAPABILITY_DOWNLOAD_REQ:
        {
            CsrDspmCapabilityDownloadReq *request = instanceData->currentRequest;

            done = isInvalidQid(instanceData, request->qid);
            if (!done)
            {
                CsrFpCreateReqSend(CSR_DSPM_IFACEQUEUE,
                    FP_OVERHEAD_HOST,
                    FP_CAPACITY_RX_HOST,
                    FP_REQUIRED_TX_CONTROLLER,
                    FP_DESIRED_TX_CONTROLLER,
                    FP_REQUIRED_RX_CONTROLLER,
                    FP_DESIRED_RX_CONTROLLER);
            }
            break;
        }
        case CSR_DSPM_CAPABILITY_REMOVE_REQ:
        {
            CsrDspmCapabilityRemoveReq *request = instanceData->currentRequest;

            done = isInvalidQid(instanceData, request->qid);
            if (!done)
            {
                bccmd(BCCMD_VARID_REMOVE_DOWNLOADED_CAPABILITY,
                    instanceData->bccmdSequenceNumber++,
                    1, request->capability);
            }
            break;
        }
#endif
        default:
        {
            CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrDspmLto, 0, CSR_DSPM_PRIM, *type);
            break;
        }
    }

    return done;
}

/*** BCCMD ***/

void CsrDspmBccmdCfmHandler(CsrDspmInstanceData *instanceData, CsrBccmdCfm *confirm)
{
    if (instanceData->state == CSR_DSPM_STATE_ACTIVE)
    {
        CsrUint8 *payload = confirm->payload;
        CsrBool done = TRUE;
        CsrPrim *type;

        CSR_LOG_TEXT_ASSERT(CsrDspmLto, 0, instanceData->currentRequest != NULL);
        CSR_LOG_TEXT_ASSERT(CsrDspmLto, 0, confirm->payload != NULL);

        /* Switch on the type of DSPM request currently being handled */
        type = instanceData->currentRequest;
        switch (*type)
        {
            case CSR_DSPM_ACTIVATE_REQ:
            {
                CsrDspmActivateReq *request = instanceData->currentRequest;

                CSR_LOG_TEXT_CONDITIONAL_ERROR(confirm->varId != BCCMD_VARID_BUILD_ID,
                    (CsrDspmLto, 0, "Unexpected BCCMD 0x%04X, handling request 0x%04X", confirm->varId, *type));
                CSR_LOG_TEXT_CONDITIONAL_ERROR(confirm->status != CSR_RESULT_SUCCESS,
                    (CsrDspmLto, 0, "Read build ID status 0x%04X, handling request 0x%04X", confirm->status, *type));
                CSR_CONVERT_16_FROM_XAP(instanceData->buildId, payload);

                CsrDspmActivateCfmSend(request->qid);
                break;
            }
            case CSR_DSPM_DEACTIVATE_REQ:
            {
                CSR_LOG_TEXT_ERROR((CsrDspmLto, 0,
                                    "Unexpected BCCMD 0x%04X, handling request 0x%04X", confirm->varId, *type));
                break;
            }
            case CSR_DSPM_STREAM_CP_OPEN_REQ:
            {
                CsrDspmStreamCpOpenReq *request = instanceData->currentRequest;
                CsrDspmCpInfo *cpInfo;
                CsrResult result = CSR_RESULT_SUCCESS;

                CSR_LOG_TEXT_CONDITIONAL_ERROR(
                    confirm->varId != BCCMD_VARID_STREAM_GET_SOURCE &&
                    confirm->varId != BCCMD_VARID_STREAM_GET_SINK,
                    (CsrDspmLto, 0, "Unexpected BCCMD 0x%04X, handling request 0x%04X", confirm->varId, *type));

                CSR_LOG_TEXT_ASSERT(CsrDspmLto, 0, request->cpInfoCount > instanceData->currentRequestIndex);
                cpInfo = &request->cpInfo[instanceData->currentRequestIndex];

                if (confirm->status == CSR_RESULT_SUCCESS)
                {
                    CSR_CONVERT_16_FROM_XAP(cpInfo->cpid, payload);
                    instanceData->currentRequestIndex += 1;
                    if (request->cpInfoCount > instanceData->currentRequestIndex)
                    {
                        done = CsrDspmRequestHandler(instanceData);
                    }
                }
                else
                {
                    if (confirm->status == CSR_BCCMD_RESULT_NO_SUCH_VARID)
                    {
                        result = CSR_DSPM_RESULT_UNSUPPORTED;
                    }
                    else
                    {
                        result = CSR_DSPM_RESULT_UNAVAILABLE;
                    }
                }
                if (done)
                {
                    CsrDspmStreamCpOpenCfmSend(request->qid, result, instanceData->currentRequestIndex,
                        request->cpInfoCount, request->cpInfo);
                    request->cpInfo = NULL;
                }
                break;
            }
            case CSR_DSPM_STREAM_CP_CLOSE_REQ:
            {
                CsrDspmStreamCpCloseReq *request = instanceData->currentRequest;

                CSR_LOG_TEXT_CONDITIONAL_ERROR(
                    confirm->varId != BCCMD_VARID_STREAM_CLOSE_SOURCE &&
                    confirm->varId != BCCMD_VARID_STREAM_CLOSE_SINK,
                    (CsrDspmLto, 0, "Unexpected BCCMD 0x%04X, handling request 0x%04X", confirm->varId, *type));

                instanceData->currentRequestIndex += 1;
                if (request->cpidCount > instanceData->currentRequestIndex)
                {
                    done = CsrDspmRequestHandler(instanceData);
                }
                else
                {
                    CsrDspmStreamCpCloseCfmSend(request->qid, request->cpidCount, request->cpid);
                    request->cpid = NULL;
                }
                break;
            }
            case CSR_DSPM_STREAM_SCO_ENABLE_REQ:
            {
                CsrDspmStreamScoEnableReq *request = instanceData->currentRequest;
                CsrResult result = CSR_RESULT_SUCCESS;

                CSR_LOG_TEXT_CONDITIONAL_ERROR(confirm->varId != BCCMD_VARID_ENABLE_SCO_STREAMS,
                    (CsrDspmLto, 0, "Unexpected BCCMD 0x%04X, handling request 0x%04X", confirm->varId, *type));

                if (confirm->status != CSR_RESULT_SUCCESS)
                {
                    if (confirm->status == CSR_BCCMD_RESULT_NO_SUCH_VARID)
                    {
                        result = CSR_DSPM_RESULT_UNSUPPORTED;
                    }
                    else
                    {
                        result = CSR_DSPM_RESULT_UNAVAILABLE;
                    }
                }

                CsrDspmStreamScoEnableCfmSend(request->qid, result);
                break;
            }
            case CSR_DSPM_STREAM_CONFIGURE_REQ:
            {
                CsrDspmStreamConfigureReq *request = instanceData->currentRequest;
                CsrResult result = CSR_RESULT_SUCCESS;

                CSR_LOG_TEXT_CONDITIONAL_ERROR(confirm->varId != BCCMD_VARID_STREAM_CONFIGURE,
                    (CsrDspmLto, 0, "Unexpected BCCMD 0x%04X, handling request 0x%04X", confirm->varId, *type));

                if (confirm->status == CSR_RESULT_SUCCESS)
                {
                    instanceData->currentRequestIndex += 1;
                    if (request->configInfoCount > instanceData->currentRequestIndex)
                    {
                        done = CsrDspmRequestHandler(instanceData);
                    }
                }
                else
                {
                    if (confirm->status == CSR_BCCMD_RESULT_NO_SUCH_VARID)
                    {
                        result = CSR_DSPM_RESULT_UNSUPPORTED;
                    }
                    else
                    {
                        result = CSR_DSPM_RESULT_UNAVAILABLE;
                    }
                }
                if (done)
                {
                    CsrDspmStreamConfigureCfmSend(request->qid, result, instanceData->currentRequestIndex,
                        request->configInfoCount, request->configInfo);
                    request->configInfo = NULL;
                }
                break;
            }
            case CSR_DSPM_STREAM_SYNC_REQ:
            {
                CsrDspmStreamSyncReq *request = instanceData->currentRequest;
                CsrResult result = CSR_RESULT_SUCCESS;

                CSR_LOG_TEXT_CONDITIONAL_ERROR(confirm->varId != BCCMD_VARID_STREAM_SYNC_SID,
                    (CsrDspmLto, 0, "Unexpected BCCMD 0x%04X, handling request 0x%04X", confirm->varId, *type));

                if (confirm->status == CSR_RESULT_SUCCESS)
                {
                    instanceData->currentRequestIndex += 1;
                    if (request->syncInfoCount > instanceData->currentRequestIndex)
                    {
                        done = CsrDspmRequestHandler(instanceData);
                    }
                }
                else
                {
                    if (confirm->status == CSR_BCCMD_RESULT_NO_SUCH_VARID)
                    {
                        result = CSR_DSPM_RESULT_UNSUPPORTED;
                    }
                    else
                    {
                        result = CSR_DSPM_RESULT_UNAVAILABLE;
                    }
                }
                if (done)
                {
                    CsrDspmStreamSyncCfmSend(request->qid, result, instanceData->currentRequestIndex,
                        request->syncInfoCount, request->syncInfo);
                    request->syncInfo = NULL;
                }
                break;
            }
            case CSR_DSPM_OPERATOR_CREATE_REQ:
            {
                CsrDspmOperatorCreateReq *request = instanceData->currentRequest;
                CsrDspmOperatorInfo *operatorInfo;
                CsrResult result = CSR_RESULT_SUCCESS;

                CSR_LOG_TEXT_CONDITIONAL_ERROR(
                    confirm->varId != BCCMD_VARID_CREATE_OPERATOR_C && confirm->varId != BCCMD_VARID_CREATE_OPERATOR_P,
                    (CsrDspmLto, 0, "Unexpected BCCMD 0x%04X, handling request 0x%04X", confirm->varId, *type));

                CSR_LOG_TEXT_ASSERT(CsrDspmLto, 0, request->operatorInfoCount > instanceData->currentRequestIndex);
                operatorInfo = &request->operatorInfo[instanceData->currentRequestIndex];

                if (confirm->status == CSR_RESULT_SUCCESS)
                {
                    CSR_CONVERT_16_FROM_XAP(operatorInfo->opid, payload);
                    if (operatorInfo->opid != CSR_DSPM_OPID_INVALID)
                    {
                        CsrDspmInfoTupleInsert(&instanceData->opids, operatorInfo->opid, request->qid);
                        instanceData->currentRequestIndex += 1;
                        if (request->operatorInfoCount > instanceData->currentRequestIndex)
                        {
                            done = CsrDspmRequestHandler(instanceData);
                        }
                    }
                    else
                    {
                        CsrUint16 itemLength = 0;
                        CsrUint8 *item;
                        if (confirm->varId == BCCMD_VARID_CREATE_OPERATOR_C)
                        {
                            CsrUint16 skipFlag;
                            payload += 2 * sizeof(CsrUint16); /* Skip item/skip count */
                            CSR_CONVERT_16_FROM_XAP(skipFlag, payload);
                            if (skipFlag == 1)
                            {
                                instanceData->itemIndex = 1;
                            }
                        }
                        CSR_LOG_TEXT_ASSERT(CsrDspmLto, 0, operatorPatchAcquire != NULL);
                        item = operatorPatchAcquire(instanceData->buildId, operatorInfo->capability,
                            instanceData->itemIndex++, NULL, NULL, &itemLength);

                        CSR_LOG_TEXT_CONDITIONAL_ERROR(itemLength > (65535 - BCCMD_HEADER_LENGTH),
                            (CsrDspmLto, 0, "Operator patch item exceeds maximum BCCMD size"));

                        CsrBccmdWriteReqSend(CSR_DSPM_IFACEQUEUE, BCCMD_VARID_CREATE_OPERATOR_P,
                            instanceData->bccmdSequenceNumber++, itemLength, item);
                        done = FALSE;
                    }
                }
                else if (confirm->status == CSR_BCCMD_RESULT_NO_SUCH_VARID)
                {
                    result = CSR_DSPM_RESULT_UNSUPPORTED;
                }
                else
                {
                    CsrUint16 reason = 0;
                    if (confirm->status == CSR_BCCMD_RESULT_ERROR)
                    {
                        CSR_CONVERT_16_FROM_XAP(reason, payload);
                    }
                    CSR_LOG_TEXT_INFO((CsrDspmLto, 0,
                                       "BCCMD 0x%04X status 0x%04X reason 0x%04X", confirm->varId, confirm->status, reason));
                    CSR_UNUSED(reason);
                    result = CSR_DSPM_RESULT_UNAVAILABLE;
                }
                if (done)
                {
                    CsrDspmOperatorCreateCfmSend(request->qid, result, instanceData->currentRequestIndex,
                        request->operatorInfoCount, request->operatorInfo);
                    request->operatorInfo = NULL;
                }
                break;
            }
            case CSR_DSPM_OPERATOR_DESTROY_REQ:
            {
                CsrDspmOperatorDestroyReq *request = instanceData->currentRequest;
                CsrResult result = CSR_RESULT_SUCCESS;
                CsrUint16 successes = 0;

                CSR_LOG_TEXT_CONDITIONAL_ERROR(confirm->varId != BCCMD_VARID_DESTROY_OPERATORS,
                    (CsrDspmLto, 0, "Unexpected BCCMD 0x%04X, handling request 0x%04X", confirm->varId, *type));

                if (confirm->status == CSR_BCCMD_RESULT_NO_SUCH_VARID)
                {
                    result = CSR_DSPM_RESULT_UNSUPPORTED;
                }
                else
                {
                    CSR_CONVERT_16_FROM_XAP(successes, payload);
                    if (confirm->status == CSR_RESULT_SUCCESS)
                    {
                        CsrUint8 i;
                        for (i = 0; i < successes; ++i)
                        {
                            (void) CsrDspmInfoTupleRemove(&instanceData->opids, request->opid[i]);
                        }
                    }
                    else
                    {
                        CsrUint16 reason = 0;
                        if (confirm->status == CSR_BCCMD_RESULT_ERROR)
                        {
                            CSR_CONVERT_16_FROM_XAP(reason, payload);
                        }
                        CSR_LOG_TEXT_INFO((CsrDspmLto, 0,
                                           "BCCMD 0x%04X status 0x%04X reason 0x%04X", confirm->varId, confirm->status, reason));
                        CSR_UNUSED(reason);
                        result = CSR_DSPM_RESULT_UNAVAILABLE;
                    }
                }
                CsrDspmOperatorDestroyCfmSend(request->qid, result, (CsrUint8) successes, request->opidCount, request->opid);
                request->opid = NULL;
                break;
            }
            case CSR_DSPM_OPERATOR_MESSAGE_REQ:
            {
                CsrDspmOperatorMessageReq *request = instanceData->currentRequest;
                CsrDspmOperatorMessageInfo *operatorMessageInfo;
                CsrResult result = CSR_RESULT_SUCCESS;

                CSR_LOG_TEXT_CONDITIONAL_ERROR(confirm->varId != BCCMD_VARID_OPERATOR_MESSAGE,
                    (CsrDspmLto, 0, "Unexpected BCCMD 0x%04X, handling request 0x%04X", confirm->varId, *type));

                CSR_LOG_TEXT_ASSERT(CsrDspmLto, 0, request->operatorMessageInfoCount > instanceData->currentRequestIndex);
                operatorMessageInfo = &request->operatorMessageInfo[instanceData->currentRequestIndex];

                if (confirm->status == CSR_RESULT_SUCCESS)
                {
                    CsrUint16 i;
                    operatorMessageInfo->messageLength = (confirm->payloadLength - sizeof(CsrUint16)) / 2;
                    CsrPmemFree(operatorMessageInfo->message);
                    operatorMessageInfo->message = CsrPmemAlloc(confirm->payloadLength - sizeof(CsrUint16));
                    payload += sizeof(CsrUint16); /* Skip opid */
                    for (i = 0; i < operatorMessageInfo->messageLength; ++i)
                    {
                        CSR_CONVERT_16_FROM_XAP(operatorMessageInfo->message[i], payload);
                    }
                    instanceData->currentRequestIndex += 1;
                    if (request->operatorMessageInfoCount > instanceData->currentRequestIndex)
                    {
                        done = CsrDspmRequestHandler(instanceData);
                    }
                }
                else if (confirm->status == CSR_BCCMD_RESULT_NO_SUCH_VARID)
                {
                    result = CSR_DSPM_RESULT_UNSUPPORTED;
                }
                else
                {
                    CsrUint16 reason = 0;
                    if (confirm->status == CSR_BCCMD_RESULT_ERROR)
                    {
                        CSR_CONVERT_16_FROM_XAP(reason, payload);
                    }
                    CSR_LOG_TEXT_INFO((CsrDspmLto, 0,
                                       "BCCMD 0x%04X status 0x%04X reason 0x%04X", confirm->varId, confirm->status, reason));
                    CSR_UNUSED(reason);
                    result = CSR_DSPM_RESULT_UNAVAILABLE;
                }
                if (done)
                {
                    CsrDspmOperatorMessageCfmSend(request->qid, result, instanceData->currentRequestIndex,
                        request->operatorMessageInfoCount, request->operatorMessageInfo);
                    request->operatorMessageInfoCount = 0;
                    request->operatorMessageInfo = NULL;
                }
                break;
            }
            case CSR_DSPM_OPERATOR_START_REQ:
            {
                CsrDspmOperatorStartReq *request = instanceData->currentRequest;
                CsrResult result = CSR_RESULT_SUCCESS;
                CsrUint16 successes = 0;

                CSR_LOG_TEXT_CONDITIONAL_ERROR(confirm->varId != BCCMD_VARID_START_OPERATORS,
                    (CsrDspmLto, 0, "Unexpected BCCMD 0x%04X, handling request 0x%04X", confirm->varId, *type));

                if (confirm->status == CSR_BCCMD_RESULT_NO_SUCH_VARID)
                {
                    result = CSR_DSPM_RESULT_UNSUPPORTED;
                }
                else
                {
                    CSR_CONVERT_16_FROM_XAP(successes, payload);
                    if (confirm->status != CSR_RESULT_SUCCESS)
                    {
                        CsrUint16 reason = 0;
                        if (confirm->status == CSR_BCCMD_RESULT_ERROR)
                        {
                            CSR_CONVERT_16_FROM_XAP(reason, payload);
                        }
                        CSR_LOG_TEXT_INFO((CsrDspmLto, 0,
                                           "BCCMD 0x%04X status 0x%04X reason 0x%04X", confirm->varId, confirm->status, reason));
                        CSR_UNUSED(reason);
                        result = CSR_DSPM_RESULT_UNAVAILABLE;
                    }
                }
                CsrDspmOperatorStartCfmSend(request->qid, result, (CsrUint8) successes, request->opidCount, request->opid);
                request->opid = NULL;
                break;
            }
            case CSR_DSPM_OPERATOR_STOP_REQ:
            {
                CsrDspmOperatorStopReq *request = instanceData->currentRequest;
                CsrResult result = CSR_RESULT_SUCCESS;
                CsrUint16 successes = 0;

                CSR_LOG_TEXT_CONDITIONAL_ERROR(confirm->varId != BCCMD_VARID_STOP_OPERATORS,
                    (CsrDspmLto, 0, "Unexpected BCCMD 0x%04X, handling request 0x%04X", confirm->varId, *type));

                if (confirm->status == CSR_BCCMD_RESULT_NO_SUCH_VARID)
                {
                    result = CSR_DSPM_RESULT_UNSUPPORTED;
                }
                else
                {
                    CSR_CONVERT_16_FROM_XAP(successes, payload);
                    if (confirm->status != CSR_RESULT_SUCCESS)
                    {
                        CsrUint16 reason = 0;
                        if (confirm->status == CSR_BCCMD_RESULT_ERROR)
                        {
                            CSR_CONVERT_16_FROM_XAP(reason, payload);
                        }
                        CSR_LOG_TEXT_INFO((CsrDspmLto, 0,
                                           "BCCMD 0x%04X status 0x%04X reason 0x%04X", confirm->varId, confirm->status, reason));
                        CSR_UNUSED(reason);
                        result = CSR_DSPM_RESULT_UNAVAILABLE;
                    }
                }
                CsrDspmOperatorStopCfmSend(request->qid, result, (CsrUint8) successes, request->opidCount, request->opid);
                request->opid = NULL;
                break;
            }
            case CSR_DSPM_OPERATOR_RESET_REQ:
            {
                CsrDspmOperatorResetReq *request = instanceData->currentRequest;
                CsrResult result = CSR_RESULT_SUCCESS;
                CsrUint16 successes = 0;

                CSR_LOG_TEXT_CONDITIONAL_ERROR(confirm->varId != BCCMD_VARID_RESET_OPERATORS,
                    (CsrDspmLto, 0, "Unexpected BCCMD 0x%04X, handling request 0x%04X", confirm->varId, *type));

                if (confirm->status == CSR_BCCMD_RESULT_NO_SUCH_VARID)
                {
                    result = CSR_DSPM_RESULT_UNSUPPORTED;
                }
                else
                {
                    CSR_CONVERT_16_FROM_XAP(successes, payload);
                    if (confirm->status != CSR_RESULT_SUCCESS)
                    {
                        CsrUint16 reason = 0;
                        if (confirm->status == CSR_BCCMD_RESULT_ERROR)
                        {
                            CSR_CONVERT_16_FROM_XAP(reason, payload);
                        }
                        CSR_LOG_TEXT_INFO((CsrDspmLto, 0,
                                           "BCCMD 0x%04X status 0x%04X reason 0x%04X", confirm->varId, confirm->status, reason));
                        CSR_UNUSED(reason);
                        result = CSR_DSPM_RESULT_UNAVAILABLE;
                    }
                }
                CsrDspmOperatorResetCfmSend(request->qid, result, (CsrUint8) successes, request->opidCount, request->opid);
                request->opid = NULL;
                break;
            }
            case CSR_DSPM_CONNECTION_CREATE_REQ:
            {
                CsrDspmConnectionCreateReq *request = instanceData->currentRequest;
                CsrDspmConnectionInfo *connectionInfo;
                CsrResult result = CSR_RESULT_SUCCESS;

                CSR_LOG_TEXT_CONDITIONAL_ERROR(confirm->varId != BCCMD_VARID_STREAM_CONNECT,
                    (CsrDspmLto, 0, "Unexpected BCCMD 0x%04X, handling request 0x%04X", confirm->varId, *type));

                CSR_LOG_TEXT_ASSERT(CsrDspmLto, 0, request->connectionInfoCount > instanceData->currentRequestIndex);
                connectionInfo = &request->connectionInfo[instanceData->currentRequestIndex];

                if (confirm->status == CSR_RESULT_SUCCESS)
                {
                    CSR_CONVERT_16_FROM_XAP(connectionInfo->cid, payload);
                    instanceData->currentRequestIndex += 1;
                    if (request->connectionInfoCount > instanceData->currentRequestIndex)
                    {
                        done = CsrDspmRequestHandler(instanceData);
                    }
                }
                else
                {
                    if (confirm->status == CSR_BCCMD_RESULT_NO_SUCH_VARID)
                    {
                        result = CSR_DSPM_RESULT_UNSUPPORTED;
                    }
                    else
                    {
                        result = CSR_DSPM_RESULT_UNAVAILABLE;
                    }
                }
                if (done)
                {
                    CsrDspmConnectionCreateCfmSend(request->qid, result, instanceData->currentRequestIndex,
                        request->connectionInfoCount, request->connectionInfo);
                    request->connectionInfo = NULL;
                }
                break;
            }
            case CSR_DSPM_CONNECTION_DESTROY_REQ:
            {
                CsrDspmConnectionDestroyReq *request = instanceData->currentRequest;

                CSR_LOG_TEXT_CONDITIONAL_ERROR(confirm->varId != BCCMD_VARID_STREAM_TRANSFORM_DISCONNECT,
                    (CsrDspmLto, 0, "Unexpected BCCMD 0x%04X, handling request 0x%04X", confirm->varId, *type));

                instanceData->currentRequestIndex += 1;
                if (request->cidCount > instanceData->currentRequestIndex)
                {
                    done = CsrDspmRequestHandler(instanceData);
                }
                else
                {
                    CsrDspmConnectionDestroyCfmSend(request->qid, request->cidCount, request->cid);
                    request->cid = NULL;
                }
                break;
            }
#ifdef CSR_DSPM_SUPPORT_CAPABILITY_DOWNLOAD
            case CSR_DSPM_CAPABILITY_DOWNLOAD_REQ:
            {
                CsrDspmCapabilityDownloadReq *request = instanceData->currentRequest;

                CSR_LOG_TEXT_CONDITIONAL_ERROR(confirm->varId != BCCMD_VARID_DOWNLOAD_CAPABILITY,
                    (CsrDspmLto, 0, "Unexpected BCCMD 0x%04X, handling request 0x%04X", confirm->varId, *type));

                done = FALSE;

                if (confirm->status == CSR_RESULT_SUCCESS)
                {
                    CsrUint16 versionMajor, versionMinor;

                    payload += 2 * sizeof(CsrUint16); /* skip capability and fastpipe handle*/
                    CSR_CONVERT_16_FROM_XAP(versionMajor, payload);
                    CSR_CONVERT_16_FROM_XAP(versionMinor, payload);

                    if ((versionMajor == request->versionMajor) && (versionMinor >= request->versionMinor))
                    {
                        instanceData->itemIndex = 0;
                        instanceData->result = CSR_RESULT_SUCCESS;
                        CsrFpWriteReqSend(instanceData->fpHandle, request->data);
                        request->data = NULL;
                        break;
                    }
                    CSR_LOG_TEXT_INFO((CsrDspmLto, 0,
                                       "Capability version incompatibility (%u.%u)", versionMajor, versionMinor));
                    instanceData->result = CSR_DSPM_RESULT_UNSUPPORTED;
                }
                else if (confirm->status == CSR_BCCMD_RESULT_NO_SUCH_VARID)
                {
                    instanceData->result = CSR_DSPM_RESULT_UNSUPPORTED;
                }
                else
                {
                    CsrUint16 reason = 0;
                    if (confirm->status == CSR_BCCMD_RESULT_ERROR)
                    {
                        CSR_CONVERT_16_FROM_XAP(reason, payload);
                    }
                    CSR_LOG_TEXT_INFO((CsrDspmLto, 0,
                                       "BCCMD 0x%04X status 0x%04X reason 0x%04X", confirm->varId, confirm->status, reason));
                    CSR_UNUSED(reason);
                    instanceData->result = CSR_DSPM_RESULT_UNAVAILABLE;
                }
                CsrFpDestroyReqSend(instanceData->fpHandle);
                break;
            }
            case CSR_DSPM_CAPABILITY_REMOVE_REQ:
            {
                CsrDspmCapabilityRemoveReq *request = instanceData->currentRequest;
                CsrResult result = CSR_RESULT_SUCCESS;

                CSR_LOG_TEXT_CONDITIONAL_ERROR(confirm->varId != BCCMD_VARID_REMOVE_DOWNLOADED_CAPABILITY,
                    (CsrDspmLto, 0, "Unexpected BCCMD 0x%04X, handling request 0x%04X", confirm->varId, *type));

                if (confirm->status != CSR_RESULT_SUCCESS)
                {
                    if (confirm->status == CSR_BCCMD_RESULT_NO_SUCH_VARID)
                    {
                        result = CSR_DSPM_RESULT_UNSUPPORTED;
                    }
                    else
                    {
                        CsrUint16 reason = 0;
                        if (confirm->status == CSR_BCCMD_RESULT_ERROR)
                        {
                            CSR_CONVERT_16_FROM_XAP(reason, payload);
                        }
                        CSR_LOG_TEXT_INFO((CsrDspmLto, 0,
                                           "BCCMD 0x%04X status 0x%04X reason 0x%04X", confirm->varId, confirm->status, reason));
                        CSR_UNUSED(reason);
                        result = CSR_DSPM_RESULT_UNAVAILABLE;
                    }
                }

                CsrDspmCapabilityRemoveCfmSend(request->qid, result, request->capability);
                break;
            }
#endif
            default:
            {
                CSR_LOG_TEXT_ERROR((CsrDspmLto, 0,
                                    "Unexpected BCCMD 0x%04X, handling request 0x%04X", confirm->varId, *type));
                break;
            }
        }

        if (done)
        {
            CsrDspmRequestQueuePopHandle(instanceData);
        }
    }

    /* Free the payload of the BCCMD */
    CsrPmemFree(confirm->payload);
}

/*** HQ ***/

void CsrDspmHqMsgIndHandler(CsrDspmInstanceData *instanceData, CsrHqMsgInd *indication)
{
    if (instanceData->state == CSR_DSPM_STATE_ACTIVE)
    {
        CsrUint8 *payload = indication->payload;

        CSR_LOG_TEXT_ASSERT(CsrDspmLto, 0, indication->payload != NULL);

        switch (indication->varId)
        {
            case HQ_VARID_MESSAGE_FROM_OPERATOR:
            {
                CsrUint16 messageLength = (indication->payloadLength - sizeof(CsrUint16)) / 2;
                CsrUint16 *message;
                CsrSchedQid qid = 0;
                CsrDspmOpid opid;
                CsrUint16 i;
                CSR_CONVERT_16_FROM_XAP(opid, payload);
                if (CsrDspmInfoTupleLookup(&instanceData->opids, opid, &qid) == CSR_RESULT_SUCCESS)
                {
                    message = CsrPmemAlloc(indication->payloadLength - sizeof(CsrUint16));
                    for (i = 0; i < messageLength; ++i)
                    {
                        CSR_CONVERT_16_FROM_XAP(message[i], payload);
                    }
                    CsrDspmOperatorMessageIndSend(qid, opid, messageLength, message);
                }
                else
                {
                    CSR_LOG_TEXT_WARNING((CsrDspmLto, 0, "Unknown opid in HQ 0x%04X, ignored", indication->varId));
                }
                break;
            }
#ifdef CSR_DSPM_SUPPORT_CAPABILITY_DOWNLOAD
            case HQ_VARID_CAPABILITY_DOWNLOAD_FAILED:
            {
                CsrUint16 reason;
                payload += sizeof(CsrUint16); /* skip capability */
                CSR_CONVERT_16_FROM_XAP(reason, payload);
                CSR_LOG_TEXT_INFO((CsrDspmLto, 0, "HQ 0x%04X reason 0x%04X", indication->varId, reason));
                CSR_UNUSED(reason);
                instanceData->result = CSR_DSPM_RESULT_UNAVAILABLE;
            }
            /* Fallthrough */
            case HQ_VARID_CAPABILITY_DOWNLOAD_COMPLETE:
            {
                if ((instanceData->currentRequest == NULL) ||
                    (*((CsrPrim *) instanceData->currentRequest) != CSR_DSPM_CAPABILITY_DOWNLOAD_REQ))
                {
                    CSR_LOG_TEXT_WARNING((CsrDspmLto, 0, "Unexpected HQ 0x%04X ignored", indication->varId));
                }
                else if (instanceData->itemIndex++ > 0)
                {
                    CsrFpDestroyReqSend(instanceData->fpHandle);
                }
                break;
            }
#endif
            case HQ_VARID_DSPMANAGER_CONFIG_REQUEST:
            {
                CsrUint8 *configBlock = NULL;
                CsrUint16 configBlockIndex;
                CsrUint16 configBlockCount = 0, configBlockLength = 0;
                CsrUint16 status = 0;

                CSR_CONVERT_16_FROM_XAP(configBlockIndex, payload);
                if (configAcquire != NULL)
                {
                    configBlock = configAcquire(instanceData->buildId, configBlockIndex,
                        &configBlockCount, &configBlockLength);
                }
                if (configBlock == NULL)
                {
                    configBlockLength = sizeof(CsrUint16);
                    payload = configBlock = CsrPmemAlloc(configBlockLength);
                    CSR_ADD_UINT16_TO_XAP(payload, configBlockCount);
                    status = 4; /* BAD_REQ */
                }

                CSR_LOG_TEXT_CONDITIONAL_ERROR(configBlockLength > (65535 - HQ_HEADER_LENGTH),
                    (CsrDspmLto, 0, "Config data block exceeds maximum HQ size"));

                CsrHqMsgResSend(0x0001 /* GETRESP */, indication->seqNo, indication->varId, status,
                    configBlockLength, configBlock);
                break;
            }
            default:
                CSR_LOG_TEXT_WARNING((CsrDspmLto, 0, "Unknown HQ 0x%04X ignored", indication->varId));
                break;
        }
    }

    /* Free the payload of the HQ message */
    CsrPmemFree(indication->payload);
}

void CsrDspmHqRegister(void)
{
    CsrHqRegisterReqSend2(CSR_DSPM_IFACEQUEUE, HQ_VARID_MESSAGE_FROM_OPERATOR, FALSE);
    CsrHqRegisterReqSend2(CSR_DSPM_IFACEQUEUE, HQ_VARID_DSPMANAGER_CONFIG_REQUEST, TRUE);
#ifdef CSR_DSPM_SUPPORT_CAPABILITY_DOWNLOAD
    CsrHqRegisterReqSend2(CSR_DSPM_IFACEQUEUE, HQ_VARID_CAPABILITY_DOWNLOAD_COMPLETE, FALSE);
    CsrHqRegisterReqSend2(CSR_DSPM_IFACEQUEUE, HQ_VARID_CAPABILITY_DOWNLOAD_FAILED, FALSE);
#endif
}

#ifdef CSR_DSPM_SUPPORT_CAPABILITY_DOWNLOAD

/*** FP ***/

void CsrDspmFpCreateCfmHandler(CsrDspmInstanceData *instanceData, CsrFpCreateCfm *confirm)
{
    if (instanceData->state == CSR_DSPM_STATE_ACTIVE)
    {
        CsrDspmCapabilityDownloadReq *request;

        if ((instanceData->currentRequest == NULL) ||
            (*((CsrPrim *) instanceData->currentRequest) != CSR_DSPM_CAPABILITY_DOWNLOAD_REQ))
        {
            CSR_LOG_TEXT_CRITICAL((CsrDspmLto, 0, "Unexpected FP 0x%04X", CSR_FP_CREATE_CFM));
        }

        request = instanceData->currentRequest;

        if (confirm->result == CSR_RESULT_SUCCESS)
        {
            instanceData->fpHandle = confirm->fpHandle;
            bccmd(BCCMD_VARID_DOWNLOAD_CAPABILITY,
                instanceData->bccmdSequenceNumber++,
                2, request->capability, confirm->fpHandle);
        }
        else
        {
            CSR_LOG_TEXT_INFO((CsrDspmLto, 0,
                               "Failed FP 0x%04X, handling 0x%04X", CSR_FP_CREATE_CFM, CSR_DSPM_CAPABILITY_DOWNLOAD_REQ));
            CsrDspmCapabilityDownloadCfmSend(request->qid, CSR_DSPM_RESULT_UNAVAILABLE, request->capability);
            CsrDspmRequestQueuePopHandle(instanceData);
        }
    }
}

void CsrDspmFpWriteCfmHandler(CsrDspmInstanceData *instanceData, CsrFpWriteCfm *confirm)
{
    if (instanceData->state == CSR_DSPM_STATE_ACTIVE)
    {
        if ((instanceData->currentRequest == NULL) ||
            (*((CsrPrim *) instanceData->currentRequest) != CSR_DSPM_CAPABILITY_DOWNLOAD_REQ))
        {
            CSR_LOG_TEXT_ERROR((CsrDspmLto, 0, "Unexpected FP 0x%04X", CSR_FP_CREATE_CFM));
        }

        if (confirm->result == CSR_RESULT_SUCCESS)
        {
            if (instanceData->itemIndex++ == 0)
            {
                return; /* wait for HQ message */
            }
        }
        else
        {
            CSR_LOG_TEXT_INFO((CsrDspmLto, 0,
                               "Failed FP 0x%04X, handling 0x%04X", CSR_FP_WRITE_CFM, CSR_DSPM_CAPABILITY_DOWNLOAD_REQ));
            instanceData->result = CSR_DSPM_RESULT_UNAVAILABLE;
        }
        CsrFpDestroyReqSend(confirm->fpHandle);
    }
}

void CsrDspmFpDestroyCfmHandler(CsrDspmInstanceData *instanceData, CsrFpDestroyCfm *confirm)
{
    if (instanceData->state == CSR_DSPM_STATE_ACTIVE)
    {
        CsrDspmCapabilityDownloadReq *request;

        if ((instanceData->currentRequest == NULL) ||
            (*((CsrPrim *) instanceData->currentRequest) != CSR_DSPM_CAPABILITY_DOWNLOAD_REQ))
        {
            CSR_LOG_TEXT_CRITICAL((CsrDspmLto, 0, "Unexpected FP 0x%04X", CSR_FP_CREATE_CFM));
        }

        request = instanceData->currentRequest;

        CsrDspmCapabilityDownloadCfmSend(request->qid, instanceData->result, request->capability);
        CsrDspmRequestQueuePopHandle(instanceData);
    }
}

#endif /* CSR_DSPM_SUPPORT_CAPABILITY_DOWNLOAD */

#ifdef CSR_BLUECORE_ONOFF

/* TM */

void CsrDspmTmBluecoreActivateTransportIndHandler(CsrDspmInstanceData *instanceData, CsrTmBluecoreActivateTransportInd *indication)
{
    instanceData->state = CSR_DSPM_STATE_ACTIVE;

    /* Start handling anything queued */
    CsrDspmRequestQueueHandle(instanceData);
}

void CsrDspmTmBluecoreDeactivateTransportIndHandler(CsrDspmInstanceData *instanceData, CsrTmBluecoreDeactivateTransportInd *indication)
{
    if (indication->active)
    {
        instanceData->state = CSR_DSPM_STATE_DEACTIVATING;

        /* Deactivate all users */
        while (instanceData->users != NULL)
        {
            CsrDspmInfoTuple *tuple = instanceData->users;
            instanceData->users = tuple->next;
            CsrDspmDeactivateIndSend((CsrSchedQid) tuple->key);
            CsrPmemFree(tuple);
        }

        /* Remove all entries from opids list */
        CsrDspmInfoTupleRemoveAll(&instanceData->opids);

        CsrTmBluecoreDeactivateTransportResSend(CSR_DSPM_IFACEQUEUE);
    }
    else
    {
        instanceData->state = CSR_DSPM_STATE_IDLE;
    }
}

#endif /* CSR_BLUECORE_ONOFF */
