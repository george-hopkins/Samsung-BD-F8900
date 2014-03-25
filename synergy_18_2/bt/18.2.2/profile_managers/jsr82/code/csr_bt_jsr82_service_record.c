/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_pmem.h"
#include "csr_types.h"
#include "csr_bt_jsr82_prim.h"
#include "csr_bt_cm_prim.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_jsr82_service_record.h"
#include "csr_bt_jsr82_main.h"
#include "csr_bt_jsr82_maintenance_handler.h"
#include "csr_bt_jsr82_discovery.h"
#include "csr_bt_util.h"
#include "csr_bt_sdc_support.h"
#include "csr_bt_profiles.h"

#define CM_SIGNAL_NOT_IMPLEMENTED_YET 0
#define SERVICE_DB_HANDLE 0x00000000
#define SERVICE_DB_STATE_ID 0x0201

#define HANDLE_NOT_ASSIGNED 0
#define HANDLE_NOT_ALLOCATED 1

static CsrUint32 byteArrayToInt(CsrUint8 *data, CsrUint8 length)
{
    CsrUint8 index, offset;
    CsrUint32 size = 0;

    for (index = 0, offset=(CsrUint8)(8*(length - 1)); index < length; index++, offset -= 8)
    {
        size |= (data[index] << offset);
    }
    return size;
}

static CsrBool getAttrPos(CsrUint16 attributeListLength, CsrUint8 *attributeList, CsrUint16 theAttribute, CsrUint32 *theSize, CsrUint32 *theIndex)
{
    CsrUint32 index  = 0;
    CsrUint8  sizeOffset = 0;

    while( index < attributeListLength)
    {
        if ((attributeList[index] & CSR_BT_DATA_ELEMENT_TYPE_DESCRIPTOR_MASK) == CSR_BT_DATA_ELEMENT_UNSIGNED_INTEGER_TYPE)
        {
            if ((index + 4) < attributeListLength)
            {
                CsrUint32 size;
                CsrUint16 attributeValue = 0x0000;
                CsrUint8  sizeDescriptor;
                CsrUint8  sizeLength;

                attributeValue    = (CsrUint16)((attributeList[index + 1] << 8) | (attributeList[index + 2]));
                sizeDescriptor  = (CsrUint8)(attributeList[index + 3] & CSR_BT_DATA_ELEMENT_SIZE_DESCRIPTOR_MASK);

                if (((sizeDescriptor > 0) || (sizeDescriptor == 0)) && (sizeDescriptor <= 4))
                {
                    size        = CsrBtReturnSizeDescriptorValue(attributeList[index + 3]) + 4 + sizeOffset;
                }
                else if ((sizeDescriptor >= 5) && (sizeDescriptor <= 7) && ((index + 5) < attributeListLength))
                {
                    sizeLength  = CsrBtReturnSizeDescriptorValue(attributeList[index + 3]);

                    size        = byteArrayToInt(attributeList + index + 4, sizeLength) + sizeLength + 4 + sizeOffset;
                }
                else
                {
                    return FALSE;
                }

                if (attributeValue == theAttribute)
                {
                    *theSize    = size;
                    *theIndex    = index - sizeOffset;
                    return TRUE;
                }
                sizeOffset = 0;
                index = index + size;
            }
            else
            {
                return FALSE;
            }
        }
        else if ((attributeList[index] & CSR_BT_DATA_ELEMENT_TYPE_DESCRIPTOR_MASK) == CSR_BT_DATA_ELEMENT_SEQUENCE_TYPE)
        {
            CsrUint32        size;

            size            = CsrBtReturnSizeDescriptorValue(attributeList[index]);
            sizeOffset      = (CsrUint8)(size + 1);
            index           += sizeOffset;
        }
        else
        {
            return FALSE;
        }
    }
    return FALSE;
}

static CsrBool extractServiceRecordHandle(CsrUint8 **destRecord, CsrUint16 *destRecordSize, CsrUint32 *serviceRecHandle, CsrUint8 *srcRecord, CsrUint16 srcRecordLength)
{
    CsrUint32 index;
    CsrUint32 size;

    if ((CsrBtSdcGetUint32Direct(srcRecordLength, srcRecord, CSR_BT_SERVICE_RECORD_HANDLE_ATTRIBUTE_IDENTIFIER, serviceRecHandle)) &&
        (getAttrPos(srcRecordLength, srcRecord, CSR_BT_SERVICE_RECORD_HANDLE_ATTRIBUTE_IDENTIFIER, &size, &index)))
    {
        CsrUint32 srcIndex = 0;
        CsrUint32 destIndex = 0;

        if ((((CsrInt32)(srcRecordLength - size)) > 0xFFFF) || (((CsrInt32)(srcRecordLength - size)) < 0))
        {
            return FALSE;
        }
        else
        {
            *destRecordSize = (CsrUint16)(srcRecordLength - size);
            *destRecord = CsrPmemAlloc(*destRecordSize * sizeof(CsrUint8));

            for (srcIndex = 0; (srcIndex < srcRecordLength) && (srcIndex < index); srcIndex++)
            {
                (*destRecord)[destIndex++] = srcRecord[srcIndex];
            }
            for (srcIndex = index + size; srcIndex < srcRecordLength; srcIndex++)
            {
                (*destRecord)[destIndex++] = srcRecord[srcIndex];
            }
            return TRUE;
        }
    }
    else
    {
        return FALSE;
    }
}

static void addNewPoolElement(serviceHandlePoolStruct **pool)
{
    serviceHandlePoolStruct *poolPtr, *newElement;
    CsrUint32                nextHandle = 0x00010000;
    CsrUint16                i = 0;

    poolPtr = *pool;

    while (poolPtr && poolPtr->next)
    {
        nextHandle = poolPtr->mapList[SERVICE_REC_HANDLE_POOL_SIZE - 1].upstreamHandle + 1;
        poolPtr = poolPtr->next;
    }
    newElement = CsrPmemZalloc(sizeof(serviceHandlePoolStruct));

    for (i=0; i < SERVICE_REC_HANDLE_POOL_SIZE; i++)
    {
        newElement->mapList[i].upstreamHandle = nextHandle++;
    }
    if (poolPtr)
    {
        poolPtr->next = newElement;
    }
    else
    {
        *pool = newElement;
    }
}

static CsrUint32 findUnassignedHandle(serviceHandlePoolStruct *pool)
{
    CsrUint32    handle = 0;
    CsrUint16    i = 0;
    serviceHandlePoolStruct *poolPtr;

    poolPtr = pool;

    while (poolPtr)
    {
        for (i=0; i < SERVICE_REC_HANDLE_POOL_SIZE; i++)
        {
            if (!(poolPtr->mapList[i].assigned))
            {
                handle = poolPtr->mapList[i].upstreamHandle;
                poolPtr->mapList[i].assigned = TRUE;
                return handle;
            }
        }
        poolPtr = poolPtr->next;
    }
    return handle;
}

static void allocateHandle(CsrUint32 upstreamHandle, CsrUint32 downstreamHandle, serviceHandlePoolStruct *pool)
{
    serviceHandlePoolStruct *poolPtr;
    CsrUint16 i;

    poolPtr = pool;
    while (poolPtr)
    {
        for (i=0; i < SERVICE_REC_HANDLE_POOL_SIZE; i++)
        {
            if (poolPtr->mapList[i].upstreamHandle == upstreamHandle)
            {
                poolPtr->mapList[i].downstreamHandle = downstreamHandle;
                poolPtr->mapList[i].allocated = TRUE;
                return;
            }
        }
        poolPtr = poolPtr->next;
    }
}

static CsrBool mapListEmpty(serviceHandleMapping *mapList, CsrUint16 mapListLength)
{
    CsrBool result = TRUE;
    CsrUint16 index;

    for (index = 0; index < mapListLength; index++)
    {
        if (mapList[index].allocated || mapList[index].assigned)
        {
            result = FALSE;
            return result;
        }
    }
    return result;
}

static void removeHandle(CsrUint32 upstreamHandle, serviceHandlePoolStruct **pool)
{
    serviceHandlePoolStruct *poolPtr;
    serviceHandlePoolStruct *prevPoolPtr = NULL;
    CsrUint16 i;

    poolPtr = *pool;
    while (poolPtr)
    {
        for (i=0; i < SERVICE_REC_HANDLE_POOL_SIZE; i++)
        {
            if (poolPtr->mapList[i].upstreamHandle == upstreamHandle)
            {
                poolPtr->mapList[i].downstreamHandle = 0;
                poolPtr->mapList[i].allocated = FALSE;
                poolPtr->mapList[i].assigned = FALSE;

                if (mapListEmpty(poolPtr->mapList, SERVICE_REC_HANDLE_POOL_SIZE))
                {
                    if (prevPoolPtr)
                    {
                        prevPoolPtr->next = poolPtr->next;
                        CsrPmemFree(poolPtr);
                    }
                    else
                    {
                        *pool = poolPtr->next;
                        CsrPmemFree(poolPtr);
                    }
                }
                return;
            }
        }
        prevPoolPtr = poolPtr;
        poolPtr = poolPtr->next;
    }
}

static CsrUint32 upstreamToDownstreamHandle(CsrUint32 upstreamHandle, serviceHandlePoolStruct *pool)
{
    serviceHandlePoolStruct *poolPtr;
    CsrUint16 i;

    poolPtr = pool;
    while (poolPtr)
    {
        for (i=0; i < SERVICE_REC_HANDLE_POOL_SIZE; i++)
        {
            if (poolPtr->mapList[i].upstreamHandle == upstreamHandle)
            {
                if (!poolPtr->mapList[i].assigned)
                {
                    return HANDLE_NOT_ASSIGNED;
                }
                else
                {
                    if(!poolPtr->mapList[i].allocated)
                    {
                        return HANDLE_NOT_ALLOCATED;
                    }
                    else
                    {
                        return poolPtr->mapList[i].downstreamHandle;
                    }
                }
            }
        }
        poolPtr = poolPtr->next;
    }
    return HANDLE_NOT_ASSIGNED;
}

static CsrUint32 downstreamToUpstreamHandle(CsrUint32 downstreamHandle, serviceHandlePoolStruct *pool)
{
    serviceHandlePoolStruct *poolPtr;
    CsrUint16 i;

    poolPtr = pool;
    while (poolPtr)
    {
        for (i=0; i < SERVICE_REC_HANDLE_POOL_SIZE; i++)
        {
            if (poolPtr->mapList[i].downstreamHandle == downstreamHandle)
            {
                return poolPtr->mapList[i].upstreamHandle;
            }
        }
        poolPtr = poolPtr->next;
    }
    return 0;
}

void CsrBtJsr82SrCreateRecordHandle(Jsr82MainInstanceData *instData)
{
    CsrBtJsr82SrCreateRecordReq  *prim;
    CsrBtJsr82SrCreateRecordCfm  *msg;
    serviceHandlePoolStruct **pool;
    CsrUint32                upstreamHandle = 0;

    pool = &instData->serviceRecHandlePool;
    upstreamHandle = findUnassignedHandle(*pool);
    while (!upstreamHandle)
    {
        addNewPoolElement(pool);
        upstreamHandle = findUnassignedHandle(*pool);
    }

    prim = (CsrBtJsr82SrCreateRecordReq *)instData->recvMsg;
    msg = CsrPmemAlloc(sizeof(CsrBtJsr82SrCreateRecordCfm));
    msg->type = CSR_BT_JSR82_SR_CREATE_RECORD_CFM;
    msg->serviceRecHandle = upstreamHandle;
    msg->reqID = prim->reqID;
    CsrBtJsr82MessagePut(prim->appHandle, msg);
}

static void csrBtJsr82SendCmSdsRegisterReq(CsrUint16 dummy, void *argv)
{
    CsrUint8 *newRecord;
    registerServiceType *req;

    CSR_UNUSED(dummy);

    req = (registerServiceType *)argv;
    newRecord = (CsrUint8 *)CsrPmemAlloc(req->serviceRecordSize*sizeof(CsrUint8));
    CsrMemCpy(newRecord, req->serviceRecord, req->serviceRecordSize*sizeof(CsrUint8));
    CsrBtCmSdsRegisterReqSend(CSR_BT_JSR82_IFACEQUEUE, newRecord, req->serviceRecordSize);
    CsrBtJsr82SetBusy(req->instData);
}

void CsrBtJsr82SrRegisterReq(Jsr82MainInstanceData *instData)
{
    CsrBtJsr82SrRegisterRecordReq    *prim;
    registerServiceType         *req;
    CsrUint8                     *tempRecord;
    CsrUint16                    tempRecordLength;
    CsrUint8                     sizeLength;
    CsrUint32                    size;

    prim = (CsrBtJsr82SrRegisterRecordReq *)instData->recvMsg;

    sizeLength  = CsrBtReturnSizeDescriptorValue(prim->serviceRecord[0]);
    size        = byteArrayToInt(prim->serviceRecord + 1, sizeLength) + sizeLength + 1;
    if (((prim->serviceRecord[0] & CSR_BT_DATA_ELEMENT_TYPE_DESCRIPTOR_MASK) == CSR_BT_DATA_ELEMENT_SEQUENCE_TYPE) && (size == prim->serviceRecordLength))
    {
        tempRecord = prim->serviceRecord + sizeLength + 1;
        tempRecordLength = (CsrUint16)(prim->serviceRecordLength - (sizeLength + 1));
    }
    else
    {
        tempRecord = prim->serviceRecord;
        tempRecordLength = prim->serviceRecordLength;
    }

    req = CsrPmemAlloc(sizeof(registerServiceType));
    if (extractServiceRecordHandle(&req->serviceRecord, &req->serviceRecordSize, &req->serviceRecHandle, tempRecord, tempRecordLength))
    {
        req->appHandle = prim->appHandle;
        req->reqID = prim->reqID;
        req->retries = 0;
        req->instData = instData;
        req->prevState = instData->state;
        instData->argv = (void *)req;
        csrBtJsr82SendCmSdsRegisterReq(0, (void *)req);
    }
    else
    {
        CsrBtJsr82SrRegisterRecordCfm *msg;
        CsrPmemFree(req);
        msg = CsrPmemAlloc(sizeof(CsrBtJsr82SrRegisterRecordCfm));
        msg->type = CSR_BT_JSR82_SR_REGISTER_RECORD_CFM;
        msg->serviceRecHandle = 0;
        msg->reqID = prim->reqID;
        CsrBtJsr82MessagePut(prim->appHandle, msg);
    }
    CsrPmemFree(prim->serviceRecord);
}

void CsrBtJsr82SendSrRegisterRecordCfm(Jsr82MainInstanceData *instData)
{
    CsrBtCmSdsRegisterCfm *prim;
    registerServiceType    *registeringService;

    registeringService = (registerServiceType *)instData->argv;

    prim = (CsrBtCmSdsRegisterCfm *)instData->recvMsg;

    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        CsrBtJsr82SrRegisterRecordCfm *msg = CsrPmemAlloc(sizeof(CsrBtJsr82SrRegisterRecordCfm));
        msg->type = CSR_BT_JSR82_SR_REGISTER_RECORD_CFM;
        allocateHandle(registeringService->serviceRecHandle, prim->serviceRecHandle, instData->serviceRecHandlePool);
        msg->serviceRecHandle = registeringService->serviceRecHandle;
        msg->reqID = registeringService->reqID;
        CsrBtJsr82MessagePut(registeringService->appHandle, msg);
        CsrBtJsr82SetState(instData,registeringService->prevState);
        CsrPmemFree(registeringService->serviceRecord);
        CsrPmemFree(instData->argv);
        instData->argv=NULL;
    }
    else
    {
        if(registeringService->retries < JSR82_SR_RETRIES)
        {
            registeringService->retries++;
            CsrSchedTimerSet(JSR82_SR_DELAY, csrBtJsr82SendCmSdsRegisterReq, 0, (void *)registeringService);
            CsrBtJsr82SetState(instData,registeringService->prevState);
        }
        else
        {
            CsrBtJsr82SrRegisterRecordCfm *msg;
            msg = CsrPmemAlloc(sizeof(CsrBtJsr82SrRegisterRecordCfm));
            msg->type = CSR_BT_JSR82_SR_REGISTER_RECORD_CFM;
            msg->serviceRecHandle = 0;
            msg->reqID = registeringService->reqID;
            CsrBtJsr82MessagePut(registeringService->appHandle, msg);
            CsrBtJsr82SetState(instData,registeringService->prevState);
            CsrPmemFree(registeringService->serviceRecord);
            CsrPmemFree(instData->argv);
            instData->argv=NULL;
        }
    }
}


static void csrBtJsr82SendCmSdsUnRegisterReq(CsrUint16 dummy, void *argv)
{
    unregisterServiceType *req;

    CSR_UNUSED(dummy);

    req = (unregisterServiceType *)argv;
    CsrBtCmSdsUnRegisterReqSend(CSR_BT_JSR82_IFACEQUEUE, req->serviceRecHandle);
    CsrBtJsr82SetBusy(req->instData);
}

void CsrBtJsr82SrUnRegisterReq(Jsr82MainInstanceData *instData)
{

    CsrUint32 downstreamHandle;
    CsrBtJsr82SrRemoveRecordReq *msg;
    unregisterServiceType     *req;

    msg = (CsrBtJsr82SrRemoveRecordReq *)instData->recvMsg;
    downstreamHandle = upstreamToDownstreamHandle(msg->serviceRecHandle, instData->serviceRecHandlePool);

    if(downstreamHandle == HANDLE_NOT_ALLOCATED) /* i.e. a create has been run, but not a register */
    {
        CsrBtJsr82SrRemoveRecordCfm *cfm;
        removeHandle(msg->serviceRecHandle, &instData->serviceRecHandlePool);
        cfm = CsrPmemAlloc(sizeof(CsrBtJsr82SrRemoveRecordCfm));
        cfm->type = CSR_BT_JSR82_SR_REMOVE_RECORD_CFM;
        cfm->serviceRecHandle = msg->serviceRecHandle; /* answer with handle from request to indicate removal */
        cfm->reqID = msg->reqID;
        CsrBtJsr82MessagePut(msg->appHandle, cfm);
    }
    else
    {
        if(downstreamHandle == HANDLE_NOT_ASSIGNED)             /* no create has been run or handle doesn't exist in pool */
        {
            CsrBtJsr82SrRemoveRecordCfm *cfm;
            removeHandle(msg->serviceRecHandle, &instData->serviceRecHandlePool);
            cfm = CsrPmemAlloc(sizeof(CsrBtJsr82SrRemoveRecordCfm));
            cfm->type = CSR_BT_JSR82_SR_REMOVE_RECORD_CFM;
            cfm->serviceRecHandle = 0;  /* answer 0 to indicate error */
            cfm->reqID = msg->reqID;
            CsrBtJsr82MessagePut(msg->appHandle, cfm);
        }
        else /* start unregistering */
        {
            req = CsrPmemAlloc(sizeof(unregisterServiceType));
            req->serviceRecHandle = downstreamHandle;
            req->appHandle = msg->appHandle;
            req->reqID = msg->reqID;
            req->instData = instData;
            req->prevState = instData->state;
            req->retries = 0;
            instData->argv = (void *)req;
            csrBtJsr82SendCmSdsUnRegisterReq(0, (void *)req);
        }
    }
}

void CsrBtJsr82SendSrRemoveRecordCfm(Jsr82MainInstanceData *instData)
{
    CsrBtCmSdsUnregisterCfm *prim;
    unregisterServiceType   *unregisteringService;

    unregisteringService = (unregisterServiceType *)instData->argv;
    prim = (CsrBtCmSdsUnregisterCfm *)instData->recvMsg;

    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        CsrBtJsr82SrRemoveRecordCfm *msg;
        msg = CsrPmemAlloc(sizeof(CsrBtJsr82SrRemoveRecordCfm));
        msg->type = CSR_BT_JSR82_SR_REMOVE_RECORD_CFM;
        msg->serviceRecHandle = downstreamToUpstreamHandle(prim->serviceRecHandle, instData->serviceRecHandlePool);
        if(unregisteringService->prevState != UPDATING_RECORD_S)
        {
            removeHandle(msg->serviceRecHandle, &instData->serviceRecHandlePool);
        }
        msg->reqID = unregisteringService->reqID;
        CsrBtJsr82MessagePut(unregisteringService->appHandle, msg);
        CsrBtJsr82SetState(instData,unregisteringService->prevState);
        CsrPmemFree(instData->argv);
        instData->argv=NULL;
    }
    else
    {
        if(unregisteringService->retries < JSR82_SR_RETRIES)
        {
            unregisteringService->retries++;
            CsrSchedTimerSet(JSR82_SR_DELAY, csrBtJsr82SendCmSdsUnRegisterReq, 0, (void *)unregisteringService);
            CsrBtJsr82SetState(instData,unregisteringService->prevState);
        }
        else
        {
            CsrBtJsr82SrRemoveRecordCfm *msg;
            msg = CsrPmemAlloc(sizeof(CsrBtJsr82SrRemoveRecordCfm));
            msg->type = CSR_BT_JSR82_SR_REMOVE_RECORD_CFM;
            msg->serviceRecHandle = 0;
            msg->reqID = unregisteringService->reqID;
            CsrBtJsr82MessagePut(unregisteringService->appHandle, msg);
            CsrBtJsr82SetState(instData,unregisteringService->prevState);
            CsrPmemFree(instData->argv);
            instData->argv=NULL;
        }
    }
}

void CsrBtJsr82SrCleanupUnregisterCfm(Jsr82MainInstanceData *instData)
{
    CsrBtCmSdsUnregisterCfm *prim = (CsrBtCmSdsUnregisterCfm *)instData->recvMsg;

    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        removeHandle(downstreamToUpstreamHandle(prim->serviceRecHandle, instData->serviceRecHandlePool), &instData->serviceRecHandlePool);
    }
}


void CsrBtJsr82StartLdUpdateRecord(Jsr82MainInstanceData *instData)
{
    CsrBtJsr82SrRemoveRecordReq  *unreg;
    CsrBtJsr82SrRegisterRecordReq  *reg;
    CsrBtJsr82LdUpdateRecordReq  *prim;

    prim = (CsrBtJsr82LdUpdateRecordReq *)instData->recvMsg;
    instData->appHandle = prim->appHandle;
    instData->reqID = prim->reqID;

    /* Create the messages */
    unreg = CsrPmemAlloc(sizeof(CsrBtJsr82SrRemoveRecordReq));
    unreg->type = CSR_BT_JSR82_SR_REMOVE_RECORD_REQ;
    unreg->appHandle = CSR_BT_JSR82_IFACEQUEUE;

    unreg->reqID = prim->reqID;
    unreg->serviceRecHandle = prim->serviceRecHandle;

    reg = CsrPmemAlloc(sizeof(CsrBtJsr82SrRegisterRecordReq));
    reg->type = CSR_BT_JSR82_SR_REGISTER_RECORD_REQ;
    reg->appHandle = CSR_BT_JSR82_IFACEQUEUE;
    reg->reqID = prim->reqID;
    reg->serviceRecordLength = prim->serviceRecordSize;
    reg->serviceRecord = prim->serviceRecord;

    /* Put one message on the internal save queue - stick the other on the instance data */
    CsrMessageQueuePush(&instData->saveQueue, CSR_BT_JSR82_PRIM, unreg);
    instData->updateMessage = reg;

    CsrBtJsr82SetUpdatingRecord(instData);
}

void CsrBtJsr82MidLdUpdateRecord(Jsr82MainInstanceData *instData)
{
    instData->restoreFlag = TRUE;
    CsrMessageQueuePush(&instData->saveQueue, CSR_BT_JSR82_PRIM, instData->updateMessage);
    instData->updateMessage = NULL;
}

void CsrBtJsr82EndLdUpdateRecord(Jsr82MainInstanceData *instData)
{
    CsrBtJsr82LdUpdateRecordCfm  *msg;
    CsrBtJsr82SrRegisterRecordCfm  *prim;

    prim = (CsrBtJsr82SrRegisterRecordCfm *)instData->recvMsg;
    msg = CsrPmemAlloc(sizeof(CsrBtJsr82LdUpdateRecordCfm));
    msg->type = CSR_BT_JSR82_LD_UPDATE_RECORD_CFM;
    msg->serviceRecHandle = prim->serviceRecHandle;
    msg->resultCode = CSR_BT_RESULT_CODE_JSR82_SUCCESS;
    msg->reqID = instData->reqID;
    CsrBtJsr82MessagePut(instData->appHandle, msg);
    instData->argv = NULL;
    CsrBtJsr82SetIdle(instData);
}

void CsrBtJsr82SrUpdateRegisterReq(Jsr82MainInstanceData *instData)
{
    CsrBtJsr82SrRegisterRecordReq  *prim;

    prim = (CsrBtJsr82SrRegisterRecordReq *)instData->recvMsg;
    if (prim->appHandle == CSR_BT_JSR82_IFACEQUEUE)
    {
        CsrBtJsr82SrRegisterReq(instData);
    }
    else
    {
        CsrBtJsr82SaveMessage(instData);
    }
}

void CsrBtJsr82SrUpdateUnRegisterReq(Jsr82MainInstanceData *instData)
{
    CsrBtJsr82SrRemoveRecordReq  *prim;

    prim = (CsrBtJsr82SrRemoveRecordReq *)instData->recvMsg;
    if (prim->appHandle == CSR_BT_JSR82_IFACEQUEUE)
    {
        CsrBtJsr82SrUnRegisterReq(instData);
    }
    else
    {
        CsrBtJsr82SaveMessage(instData);
    }
}

