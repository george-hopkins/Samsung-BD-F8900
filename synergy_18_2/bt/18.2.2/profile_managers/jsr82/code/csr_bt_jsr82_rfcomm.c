/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_jsr82_main.h"
#include "csr_bt_jsr82_rfcomm.h"
#include "csr_pmem.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_util.h"
#include "csr_bt_jsr82_maintenance_handler.h"
#include "csr_bt_usr_config.h"
#include "csr_bt_jsr82_remote_device.h"
#include "csr_bt_jsr82_l2cap.h"

static void sendReceiveDataCfm(CsrSchedQid appHandle, CsrUint32 reqID, CsrUint8 serverChannel, CsrUint32 payloadLength, CsrUint8 *payload);

static void closeSendOperation(rfcSendStruct *sendInfo, CsrUint8 serverChannel, Jsr82MainInstanceData *instData)
{
    CsrBtJsr82RfcSendDataCfm *msg;
    if (sendInfo)
    {
        msg = (CsrBtJsr82RfcSendDataCfm *)CsrPmemAlloc(sizeof(CsrBtJsr82RfcSendDataCfm));
        msg->type = CSR_BT_JSR82_RFC_SEND_DATA_CFM;
        msg->bytesWritten = sendInfo->position;
        msg->localServerChannel = serverChannel;
        msg->reqID = sendInfo->reqID;
        CsrBtJsr82MessagePut(sendInfo->appHandle, msg);
        CsrPmemFree(sendInfo->buffer);
        sendInfo->buffer = NULL;
        if (sendInfo->sendingPacket)
        {
            CsrBtJsr82SetIdle(instData);
        }
    }
}

static void closeReceiveOperation(rfcConnectionStruct *rfcConnection)
{
    CsrBtJsr82RfcReceiveDataReq *dataReq;
    void *popMsg;

    CsrUint16 event;
    while(rfcConnection && CsrMessageQueuePop(&rfcConnection->receiveInfo.dataReqQueue, &event, &popMsg))
    {
        dataReq=(CsrBtJsr82RfcReceiveDataReq *)popMsg;

        sendReceiveDataCfm(dataReq->appHandle, dataReq->reqID, rfcConnection->serverChannel, 0, NULL);
        CsrPmemFree(dataReq);
    }
}

rfcConnectionStruct *getRfcConnectingInstace(rfcConnectionPoolStruct *pool)
{
    rfcConnectionPoolStruct *poolPtr;

    poolPtr = pool;
    while (poolPtr)
    {
        CsrUint16 index;
        for (index = 0; index < RFC_CONNECTION_POOL_SIZE; index++)
        {
            if (poolPtr->connectionList[index].connecting)
            {
                return &(poolPtr->connectionList[index]);
            }
        }
        poolPtr = poolPtr->next;
    }
    return NULL;
}

rfcConnectionStruct *getRfcConnectionFromConnId(CsrUint32 conn_id, rfcConnectionPoolStruct *pool)
{
    rfcConnectionPoolStruct *poolPtr;

    poolPtr = pool;
    while (poolPtr)
    {
        CsrUint16 index;
        for (index = 0; index < RFC_CONNECTION_POOL_SIZE; index++)
        {
            if (poolPtr->connectionList[index].jsr82ConnId == conn_id)
            {
                return &(poolPtr->connectionList[index]);
            }
        }
        poolPtr = poolPtr->next;
    }
    return NULL;
}

rfcConnectionStruct *getRfcConnection(CsrUint8 serverChannel, rfcConnectionPoolStruct *pool)
{
    rfcConnectionPoolStruct *poolPtr;

    poolPtr = pool;
    while (poolPtr)
    {
        CsrUint16 index;
        for (index = 0; index < RFC_CONNECTION_POOL_SIZE; index++)
        {
            if (poolPtr->connectionList[index].serverChannel == serverChannel)
            {
                return &(poolPtr->connectionList[index]);
            }
        }
        poolPtr = poolPtr->next;
    }
    return NULL;
}

static rfcConnectionStruct *findFreeRfcConnectionSlot(rfcConnectionPoolStruct *pool)
{
    rfcConnectionPoolStruct   *poolPtr;

    poolPtr = pool;
    while (poolPtr)
    {
        CsrUint16 index;
        for (index = 0; index < RFC_CONNECTION_POOL_SIZE; index++)
        {
            if (!(poolPtr->connectionList[index].serverChannel))
            {
                return &(poolPtr->connectionList[index]);
            }
        }
        poolPtr = poolPtr->next;
    }
    return NULL;

}

static void createRfcConnectionPoolElement(rfcConnectionPoolStruct **pool)
{
    rfcConnectionPoolStruct   *poolPtr;
    rfcConnectionPoolStruct   *newElement;

    poolPtr = *pool;
    while (poolPtr && poolPtr->next)
    {
        poolPtr = poolPtr->next;
    }

    newElement = CsrPmemZalloc(sizeof(rfcConnectionPoolStruct));

    if (poolPtr)
    {
        poolPtr->next = newElement;
    }
    else
    {
        *pool = newElement;
    }
}

static void createRfcConnection(CsrUint8 serverChannel, rfcConnectionPoolStruct **pool)
{
    rfcConnectionStruct *rfcConnection;
    rfcConnection = findFreeRfcConnectionSlot(*pool);
    while (!rfcConnection)
    {
        createRfcConnectionPoolElement(pool);
        rfcConnection = findFreeRfcConnectionSlot(*pool);
    }

    rfcConnection->serverChannel = serverChannel;
}

static void cleanupConnectionPool(rfcConnectionPoolStruct **pool)
{
    rfcConnectionPoolStruct   *poolPtr;
    rfcConnectionPoolStruct   *prevPoolPtr = NULL;

    poolPtr = *pool;
    while (poolPtr)
    {
        CsrUint16 index;
        CsrBool empty = TRUE;
        for (index = 0; index < RFC_CONNECTION_POOL_SIZE; index++)
        {
            if (poolPtr->connectionList[index].serverChannel)
            {
                empty = FALSE;
            }
        }
        if (empty)
        {
            if (prevPoolPtr)
            {
                prevPoolPtr->next = poolPtr->next;
                CsrPmemFree(poolPtr);
                poolPtr = prevPoolPtr;
            }
            else
            {
                *pool = poolPtr->next;
                CsrPmemFree(poolPtr);
                poolPtr = *pool;
            }
        }
        prevPoolPtr = poolPtr;
        if (poolPtr)
        {
            poolPtr = poolPtr->next;
        }
    }
}

static void closeRfcConnection(rfcConnectionStruct *rfcConnection, Jsr82MainInstanceData *instData)
{
    if (rfcConnection)
    {
        if (rfcConnection->sendInfo)
        {
            closeSendOperation(rfcConnection->sendInfo, rfcConnection->serverChannel, instData);
            CsrPmemFree(rfcConnection->sendInfo);
            rfcConnection->sendInfo = NULL;
        }
        if (rfcConnection->receiveInfo.receiveBuffer)
        {
            closeReceiveOperation(rfcConnection);
            CsrPmemFree(rfcConnection->receiveInfo.receiveBuffer);
            rfcConnection->receiveInfo.receiveBuffer = NULL;
        }
        CsrMemSet(&rfcConnection->receiveInfo, 0, sizeof(rfcReceiveStruct));
        rfcConnection->jsr82ConnId = CSR_BT_CONN_ID_INVALID;
        rfcConnection->attrs = 0;
        CsrMemSet(&rfcConnection->deviceAddr, 0, sizeof(CsrBtDeviceAddr));
        rfcConnection->connected = FALSE;
        rfcConnection->connecting = FALSE;
        rfcConnection->destroy = FALSE;
    }
}

static CsrBool removeRfcConnection(CsrUint8 serverChannel, rfcConnectionPoolStruct **pool, Jsr82MainInstanceData *instData)
{
    rfcConnectionStruct *rfcConnection;

    rfcConnection = getRfcConnection(serverChannel, *pool);
    if (rfcConnection)
    {
        closeRfcConnection(rfcConnection, instData);
        CsrBtCmUnRegisterReqSend(serverChannel);
        CsrMemSet(rfcConnection, 0, sizeof(rfcConnectionStruct));
        cleanupConnectionPool(pool);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void CsrBtJsr82StartRfcSendData(Jsr82MainInstanceData *instData)
{
    CsrBtJsr82RfcSendDataReq *prim;
    rfcConnectionStruct *connection;

    prim = (CsrBtJsr82RfcSendDataReq *)instData->recvMsg;

    connection = getRfcConnection(prim->localServerChannel, instData->rfcConnectionPool);


    if(connection && connection->connected && !connection->sendInfo)
    {
        CsrUint8 *thisFrame;
        CsrUint16 frameLength;

        connection->sendInfo = (rfcSendStruct *)CsrPmemZalloc(sizeof(rfcSendStruct));

        instData->appHandle= prim->appHandle;
        instData->reqID = prim->reqID;

        connection->sendInfo->appHandle = instData->appHandle;
        connection->sendInfo->reqID = instData->reqID;

        connection->sendInfo->bufferLength = prim->payloadLength;
        connection->sendInfo->buffer = prim->payload;

        if(prim->payloadLength > connection->maxFrameSize)
        {
            frameLength = connection->maxFrameSize;

        }
        else
        {
            frameLength = (CsrUint16)connection->sendInfo->bufferLength;
        }

        thisFrame = (CsrUint8 *)CsrPmemAlloc(frameLength*sizeof(CsrUint8));
        CsrMemCpy(thisFrame, connection->sendInfo->buffer, frameLength);

        if(prim->payloadLength <= connection->maxFrameSize)
        {
            CsrPmemFree(connection->sendInfo->buffer);
            connection->sendInfo->buffer = NULL; /* indicates only one frame to be sent */
        }
        CsrBtCmDataReqSend(connection->jsr82ConnId, frameLength, thisFrame );
        connection->sendInfo->sendingPacket = TRUE;
        CsrBtJsr82SetBusy(instData);
    }
    else
    {
        /* connection doesn't exist or another transmission is underway - return cfm with bytesWritten = 0 */
        CsrBtJsr82RfcSendDataCfm *msg;
        msg = (CsrBtJsr82RfcSendDataCfm *)CsrPmemAlloc(sizeof(CsrBtJsr82RfcSendDataCfm));
        msg->type = CSR_BT_JSR82_RFC_SEND_DATA_CFM;
        msg->reqID = prim->reqID;
        msg->bytesWritten = 0;
        msg->localServerChannel = prim->localServerChannel;
        CsrBtJsr82MessagePut(prim->appHandle, msg);

    }
}

void CsrBtJsr82ContinueRfcSendDataHandler(Jsr82MainInstanceData *instData)
{
    rfcConnectionStruct *connection;
    rfcSendStruct *sendInfo;
    CsrBtJsr82ContinueRfcSendData *prim;
    prim = (CsrBtJsr82ContinueRfcSendData *)instData->recvMsg;


    connection = getRfcConnection(prim->localServerChannel, instData->rfcConnectionPool);

    if(connection && connection->connected && connection->sendInfo)
    {

        CsrUint8 *thisFrame;
        CsrUint16 frameLength;

        sendInfo = connection->sendInfo;
        sendInfo->position += connection->maxFrameSize;

        if((sendInfo->bufferLength - sendInfo->position) <= connection->maxFrameSize)
        {
            frameLength = (CsrUint16)(sendInfo->bufferLength - sendInfo->position);
        }
        else
        {
            frameLength = connection->maxFrameSize;
        }
        thisFrame = (CsrUint8 *)CsrPmemAlloc(frameLength*sizeof(CsrUint8));
        CsrMemCpy(thisFrame, sendInfo->buffer + sendInfo->position, frameLength);

        if((sendInfo->bufferLength - sendInfo->position) <= connection->maxFrameSize)
        {
            CsrPmemFree(sendInfo->buffer);
            sendInfo->buffer = NULL;
        }

        CsrBtCmDataReqSend(connection->jsr82ConnId, frameLength, thisFrame );
        connection->sendInfo->sendingPacket = TRUE;
        CsrBtJsr82SetBusy(instData);
    }
    else
    {
    }
}

void CsrBtJsr82RfcHandleDataCfm(Jsr82MainInstanceData *instData)
{
    CsrBtCmDataCfm *prim;
    rfcConnectionStruct *connection;

    prim = (CsrBtCmDataCfm *)instData->recvMsg;

    connection = getRfcConnectionFromConnId(prim->btConnId, instData->rfcConnectionPool);

    if(connection && connection->connected)
    {
        connection->sendInfo->sendingPacket = FALSE;
        CsrBtJsr82SetIdle(instData);
        if(connection->sendInfo->buffer)
        {
            CsrBtJsr82ContinueRfcSendData *msg;
            msg = (CsrBtJsr82ContinueRfcSendData *)CsrPmemAlloc(sizeof(CsrBtJsr82ContinueRfcSendData));
            msg->type = CSR_BT_JSR82_CONTINUE_RFC_SEND_DATA;
            msg->localServerChannel = connection->serverChannel;
            CsrBtJsr82MessagePut(CSR_BT_JSR82_IFACEQUEUE, msg);
        }
        else
        { /* last frame has been sent */
            rfcSendStruct *sendInfo;
            CsrBtJsr82RfcSendDataCfm *msg;
            sendInfo = connection->sendInfo;
            msg = (CsrBtJsr82RfcSendDataCfm *)CsrPmemAlloc(sizeof(CsrBtJsr82RfcSendDataCfm));
            msg->type = CSR_BT_JSR82_RFC_SEND_DATA_CFM;
            msg->reqID = sendInfo->reqID;
            msg->bytesWritten = sendInfo->bufferLength;
            msg->localServerChannel = connection->serverChannel;

            CsrBtJsr82MessagePut(sendInfo->appHandle, msg);
            CsrPmemFree(sendInfo);
            connection->sendInfo = NULL;
        }
    }
}


void CsrBtJsr82RfcHandleDataInd(Jsr82MainInstanceData *instData)
{
    CsrBtCmDataInd *prim;
    rfcConnectionStruct *connection;
    CsrUint32 writePointer, readPointer;
    CsrUint32 i;

    prim = (CsrBtCmDataInd *)instData->recvMsg;
    connection = getRfcConnectionFromConnId(prim->btConnId, instData->rfcConnectionPool);

    if(connection && connection->connected)
    {
        CsrBtJsr82RfcReceiveDataReq *dataReq;
        void *popMsg;
        CsrUint16 event;
        readPointer = connection->receiveInfo.readPointer;
        writePointer = connection->receiveInfo.writePointer;

        /* Buffer the payload */
        for (i=0; i<prim->payloadLength; i++)
        {
            connection->receiveInfo.receiveBuffer[writePointer] = prim->payload[i];
            writePointer = (writePointer < ((connection->maxFrameSize * CSR_BT_JSR82_RFC_BUFFER_SIZE)-1)) ? writePointer+1 : 0;
        }
        /* update the write pointer and valid data counter */
        connection->receiveInfo.writePointer = writePointer;
        connection->receiveInfo.validData += prim->payloadLength;

        /* check if app is waiting for data - send as much as it wants (and we've got); buffer the rest */
        while(CsrMessageQueuePop(&connection->receiveInfo.dataReqQueue, &event, &popMsg) && connection->receiveInfo.validData)
        {
            CsrUint32 payloadLength = 0;
            CsrUint8 *payload = NULL;

            dataReq=(CsrBtJsr82RfcReceiveDataReq *)popMsg;

            payloadLength = (CsrUint32)((connection->receiveInfo.validData < dataReq->bytesToRead) ? connection->receiveInfo.validData : dataReq->bytesToRead);
            payload = CsrPmemAlloc(payloadLength*sizeof(CsrUint8));

            for (i=0; i<payloadLength; i++)
            {
                payload[i] = connection->receiveInfo.receiveBuffer[readPointer];
                readPointer = (readPointer < ((connection->maxFrameSize * CSR_BT_JSR82_RFC_BUFFER_SIZE)-1)) ? readPointer+1 : 0;
            }
            /* update the read pointer and valid data counter */
            connection->receiveInfo.readPointer = readPointer;
            connection->receiveInfo.validData -= payloadLength;
            sendReceiveDataCfm(dataReq->appHandle, dataReq->reqID, dataReq->localServerChannel, payloadLength, payload);
            CsrPmemFree(dataReq);
        }


        if ((((connection->maxFrameSize * CSR_BT_JSR82_RFC_BUFFER_SIZE)-1) - connection->receiveInfo.validData) >= connection->maxFrameSize)
        {
            CsrBtCmDataResSend(connection->jsr82ConnId);
            connection->receiveInfo.cmHasCredit = TRUE;
        }
        else
        {
            connection->receiveInfo.cmHasCredit = FALSE;
        }
    }
    CsrPmemFree(prim->payload);
}


static void sendReceiveDataCfm(CsrSchedQid appHandle, CsrUint32 reqID, CsrUint8 serverChannel, CsrUint32 payloadLength, CsrUint8 *payload)
{
    CsrBtJsr82RfcReceiveDataCfm *msg;
    msg = CsrPmemAlloc(sizeof(CsrBtJsr82RfcReceiveDataCfm));
    msg->type = CSR_BT_JSR82_RFC_RECEIVE_DATA_CFM;
    msg->reqID = reqID;
    msg->localServerChannel = serverChannel;
    msg->payloadLength = payloadLength;
    msg->payload = payload;
    CsrBtJsr82MessagePut(appHandle, msg);
}

void CsrBtJsr82RfcReceiveData(Jsr82MainInstanceData *instData)
{
    CsrBtJsr82RfcReceiveDataReq *prim;
    rfcConnectionStruct *connection;
    CsrUint32 readPointer;
    CsrUint32 i;
    CsrUint32 payloadLength;
    CsrUint8 *payload;

    prim = (CsrBtJsr82RfcReceiveDataReq *)instData->recvMsg;

    connection = getRfcConnection(prim->localServerChannel, instData->rfcConnectionPool);


    if(connection && prim->bytesToRead)
    {
        if(connection->receiveInfo.validData)
        {
            readPointer = connection->receiveInfo.readPointer;
            payloadLength = (prim->bytesToRead < connection->receiveInfo.validData) ? prim->bytesToRead : connection->receiveInfo.validData;
            payload = (CsrUint8 *)CsrPmemAlloc(payloadLength*sizeof(CsrUint8));

            for (i=0; i<payloadLength; i++)
            {
                payload[i] = connection->receiveInfo.receiveBuffer[readPointer];
                readPointer = (readPointer < ((connection->maxFrameSize * CSR_BT_JSR82_RFC_BUFFER_SIZE)-1)) ? readPointer+1 : 0;
            }
            connection->receiveInfo.readPointer = readPointer;
            connection->receiveInfo.validData -= payloadLength;
            sendReceiveDataCfm(prim->appHandle, prim->reqID, prim->localServerChannel, payloadLength, payload);

            if( ((((connection->maxFrameSize * CSR_BT_JSR82_RFC_BUFFER_SIZE)-1) - connection->receiveInfo.validData) >= connection->maxFrameSize) && !connection->receiveInfo.cmHasCredit)
            {
                if (connection->connected)
                {
                    CsrBtCmDataResSend(connection->jsr82ConnId);
                    connection->receiveInfo.cmHasCredit = TRUE;
                }
            }

        }
        else
        {
            if (connection->connected)
            { /* no data ready - set bytesRequested and wait for a CSR_BT_CM_DATA_IND (java app is now blocked!) */
                CsrMessageQueuePush(&connection->receiveInfo.dataReqQueue, instData->eventClass, instData->recvMsg);
                instData->recvMsg = NULL;
            }
            else
            { /* connection to remote device has been lost and the receive buffer is empty */
                CsrPmemFree(connection->receiveInfo.receiveBuffer);
                connection->receiveInfo.receiveBuffer = NULL;
                sendReceiveDataCfm(prim->appHandle, prim->reqID, prim->localServerChannel, 0, NULL);
            }
        }
    }
    else /* no connection, or application asked for 0 bytes */
    {
        sendReceiveDataCfm(prim->appHandle, prim->reqID, prim->localServerChannel, 0, NULL);
    }
}

void CsrBtJsr82RfcGetAvailable(Jsr82MainInstanceData *instData)
{
    CsrBtJsr82RfcGetAvailableReq *prim;
    CsrBtJsr82RfcGetAvailableCfm *msg;
    rfcConnectionStruct *connection;

    prim = (CsrBtJsr82RfcGetAvailableReq *)instData->recvMsg;

    msg = (CsrBtJsr82RfcGetAvailableCfm *)CsrPmemAlloc(sizeof(CsrBtJsr82RfcGetAvailableCfm));
    msg->type = CSR_BT_JSR82_RFC_GET_AVAILABLE_CFM;
    msg->reqID = prim->reqID;
    msg->localServerChannel = prim->localServerChannel;

    connection = getRfcConnection(prim->localServerChannel, instData->rfcConnectionPool);

    if(connection && connection->connected)
    {
        msg->bytesAvailable = connection->receiveInfo.validData;
        msg->resultCode = CSR_BT_RESULT_CODE_JSR82_SUCCESS;
    }
    else
    {
        msg->bytesAvailable = 0;
        msg->resultCode = CSR_BT_RESULT_CODE_JSR82_DEVICE_NOT_CONNECTED;
    }

    CsrBtJsr82MessagePut(prim->appHandle, msg);
}

void CsrBtJsr82RfcGetServerChannelReqHandler(Jsr82MainInstanceData *instData)
{
    const jsr82DynSignalHandler cfmHandler[] = {CsrBtJsr82CmRegisterCfmHandler};
    CsrBtJsr82SendCmMsgDown(instData, (CsrBtJsr82Prim **)&instData->recvMsg, CsrBtCmRegisterReq_struct(CSR_BT_JSR82_IFACEQUEUE, CSR_BT_CM_CONTEXT_UNUSED, CSR_BT_CM_SERVER_CHANNEL_DONT_CARE), cfmHandler);
}

CsrBool CsrBtJsr82CmRegisterCfmHandler(Jsr82MainInstanceData *instData, CsrBtJsr82Prim **orgMsg)
{
    CsrBtCmRegisterCfm   *prim;
    CsrUint8       localServerChannel;
    rfcConnectionStruct         *rfcConnection;
    CsrBtJsr82RfcGetServerChannelCfm *msg;
    CsrBtJsr82RfcGetServerChannelReq *orgPrim;

    prim = (CsrBtCmRegisterCfm *)instData->recvMsg;
    orgPrim = ((CsrBtJsr82RfcGetServerChannelReq *)*orgMsg);
    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        localServerChannel = prim->serverChannel;
        createRfcConnection(localServerChannel, &(instData->rfcConnectionPool));
    }
    else
    {
        localServerChannel = 0;
    }

    rfcConnection = getRfcConnection(localServerChannel, instData->rfcConnectionPool);
    if (rfcConnection)
    {
        rfcConnection->server = TRUE;
    }
    msg = CsrPmemAlloc(sizeof(CsrBtJsr82RfcGetServerChannelCfm));
    msg->type = CSR_BT_JSR82_RFC_GET_SERVER_CHANNEL_CFM;
    msg->reqID = orgPrim->reqID;
    msg->localServerChannel = localServerChannel;
    CsrBtJsr82MessagePut(orgPrim->appHandle, msg);
    return TRUE;
}

static void sendCsrBtJsr82RfcAcceptAndOpenCfm(CsrSchedQid appHandle,
    CsrUint32 reqID, CsrBtDeviceAddr deviceAddr,
    CsrUint8 localServerChannel, CsrBtResultCode result)
{
    CsrBtJsr82RfcAcceptAndOpenCfm *msg;

    msg                     = CsrPmemAlloc(sizeof(CsrBtJsr82RfcAcceptAndOpenCfm));
    msg->type               = CSR_BT_JSR82_RFC_ACCEPT_AND_OPEN_CFM;
    msg->reqID              = reqID;
    msg->deviceAddr         = deviceAddr;
    msg->localServerChannel = localServerChannel;
    msg->resultCode         = result;
    CsrBtJsr82MessagePut(appHandle, (void *)msg);
}

void CsrBtJsr82RfcAcceptAndOpenReqHandler(Jsr82MainInstanceData *instData)
{
    CsrBtJsr82RfcAcceptAndOpenReq    *prim;
    rfcConnectionStruct         *rfcConnection;
    dm_security_mode_t          curSecMode = CSR_BT_DEFAULT_SEC_MODE;

    prim = (CsrBtJsr82RfcAcceptAndOpenReq *)instData->recvMsg;

    rfcConnection = getRfcConnection(prim->localServerChannel, instData->rfcConnectionPool);
    if ((curSecMode < SEC_MODE2_SERVICE) && (prim->attrs & (CSR_BT_JSR82_AUTHENTICATE | CSR_BT_JSR82_ENCRYPT)))
    {
        CsrBtDeviceAddr dummyAddr = {0,0,0};
        sendCsrBtJsr82RfcAcceptAndOpenCfm(prim->appHandle, prim->reqID,
            dummyAddr, 0, CSR_BT_RESULT_CODE_JSR82_SECURITY_FAIL);
    }
    else if (rfcConnection && !(rfcConnection->connected || rfcConnection->connecting || rfcConnection->connectable))
    {
        dm_security_level_t secLevel;
        CsrUint8 modemStatus = CSR_BT_MODEM_SEND_CTRL_DCE_DEFAULT;

        rfcConnection->appHandle    = prim->appHandle;
        rfcConnection->reqID        = prim->reqID;
        rfcConnection->connectable  = TRUE;
        rfcConnection->attrs        = prim->attrs;
        secLevel                    = CsrBtJsr82SetSecurity(rfcConnection->attrs, TRUE);
        CsrBtCmConnectAcceptReqSend(CSR_BT_JSR82_IFACEQUEUE, (CsrUint24)0x000000, CSR_BT_INFINITE_TIME, 
                                    CSR_BT_JSR82_RFCOMM_PROFILE_DEFAULT_MTU_SIZE, prim->localServerChannel, 
                                    secLevel, JSR82_RFC_UUID, modemStatus,CSR_BT_DEFAULT_BREAK_SIGNAL ,CSR_BT_DEFAULT_MSC_TIMEOUT);
    }
    else
    {
        CsrBtDeviceAddr    dummyAddr = {0,0,0};
        sendCsrBtJsr82RfcAcceptAndOpenCfm(prim->appHandle, prim->reqID,
            dummyAddr, 0, CSR_BT_RESULT_CODE_JSR82_COMMAND_DISALLOWED);
    }
}

void CsrBtJsr82CmConnectAcceptCfmHandler(Jsr82MainInstanceData *instData)
{
    CsrBtCmConnectAcceptCfm *prim;
    rfcConnectionStruct     *rfcConnection;

    prim = (CsrBtCmConnectAcceptCfm *)instData->recvMsg;

    rfcConnection = getRfcConnection(prim->serverChannel, instData->rfcConnectionPool);
    if (rfcConnection)
    {
        if (prim->resultSupplier == CSR_BT_SUPPLIER_CM &&
            prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS)
        {
            CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_DATA_CHANNEL,
                    prim->deviceAddr,prim->btConnId);

            rfcConnection->maxFrameSize = prim->profileMaxFrameSize;
            rfcConnection->jsr82ConnId = prim->btConnId;
            rfcConnection->deviceAddr = prim->deviceAddr;
            rfcConnection->connectable = FALSE;
            rfcConnection->connected = TRUE;
            rfcConnection->receiveInfo.receiveBuffer = CsrPmemAlloc(rfcConnection->maxFrameSize * CSR_BT_JSR82_RFC_BUFFER_SIZE);
            rfcConnection->receiveInfo.dataReqQueue = NULL;

            sendCsrBtJsr82RfcAcceptAndOpenCfm(rfcConnection->appHandle,
                rfcConnection->reqID, rfcConnection->deviceAddr,
                rfcConnection->serverChannel, CSR_BT_RESULT_CODE_JSR82_SUCCESS);

            CsrBtJsr82AddConnection(instData, prim->deviceAddr);

        }
        else
        {
            CsrBtDeviceAddr    dummyAddr = {0,0,0};
            sendCsrBtJsr82RfcAcceptAndOpenCfm(rfcConnection->appHandle,
                rfcConnection->reqID, dummyAddr, prim->serverChannel,
                CSR_BT_RESULT_CODE_JSR82_UNSPECIFIED_ERROR);
        }
    }
    else
    {
        CsrBtCmDisconnectReqSend(prim->btConnId);
    }
}

static RFC_PORTNEG_VALUES_T createPortPar(void)
{
    RFC_PORTNEG_VALUES_T              portPar;

    portPar.baud_rate       = CSR_BT_DEFAULT_PORT_SPEED;
    portPar.data_bits       = CSR_BT_DEFAULT_DATA_BITS;
    portPar.stop_bits       = CSR_BT_DEFAULT_STOP_BITS;
    portPar.parity          = CSR_BT_DEFAULT_PARITY;
    portPar.parity_type     = CSR_BT_DEFAULT_PARITY_TYPE;
    portPar.flow_ctrl_mask  = CSR_BT_DEFAULT_FLOW_CTRL_MASK;
    portPar.xon             = CSR_BT_DEFAULT_XON;
    portPar.xoff            = CSR_BT_DEFAULT_XOFF;
    portPar.parameter_mask  = CSR_BT_DEFAULT_PARAMETER_MASK;

    return portPar;
}

static void sendCsrBtJsr82RfcConnectCfm(CsrSchedQid appHandle, CsrUint32 reqID,
    CsrUint8 localServerChannel, CsrBtDeviceAddr deviceAddr,
    CsrBtResultCode result)
{
    CsrBtJsr82RfcConnectCfm *msg;

    msg                     = CsrPmemAlloc(sizeof(CsrBtJsr82RfcConnectCfm));
    msg->type               = CSR_BT_JSR82_RFC_CONNECT_CFM;
    msg->reqID              = reqID;
    msg->localServerChannel = localServerChannel;
    msg->deviceAddr         = deviceAddr;
    msg->resultCode         = result;

    CsrBtJsr82MessagePut(appHandle, msg);
}

CsrBool CsrBtJsr82ConnectGetServerChannelCallBack(Jsr82MainInstanceData *instData, CsrBtJsr82Prim **orgMsg)
{
    rfcConnectionStruct     *rfcConnection;
    RFC_PORTNEG_VALUES_T    portPar;
    CsrBtJsr82RfcConnectReq *orgPrim;
    CsrBtCmRegisterCfm *prim;
    CsrUint8       localServerChannel;

    portPar = createPortPar();
    prim = (CsrBtCmRegisterCfm *)instData->recvMsg;
    orgPrim = ((CsrBtJsr82RfcConnectReq *)*orgMsg);
    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS &&
        prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        localServerChannel = prim->serverChannel;
        createRfcConnection(localServerChannel, &(instData->rfcConnectionPool));
    }
    else
    {
        localServerChannel = 0;
    }


    if (localServerChannel)
    {
        rfcConnection = getRfcConnection(localServerChannel, instData->rfcConnectionPool);
        if (rfcConnection)
        {
            dm_security_level_t secLevel;
            CsrUint8 modemStatus = CSR_BT_MODEM_SEND_CTRL_DCE_DEFAULT;
            
            rfcConnection->connecting = TRUE;
            rfcConnection->deviceAddr = orgPrim->deviceAddr;
            rfcConnection->attrs = orgPrim->attrs;
            rfcConnection->server = FALSE;
            secLevel                  = CsrBtJsr82SetSecurity(rfcConnection->attrs, FALSE);

            instData->appHandle = orgPrim->appHandle;
            instData->reqID = orgPrim->reqID;
            CsrBtCmConnectExtReqSend(CSR_BT_JSR82_IFACEQUEUE, localServerChannel, orgPrim->remoteServerChannel,
                                CSR_BT_JSR82_RFCOMM_PROFILE_DEFAULT_MTU_SIZE, FALSE, FALSE, portPar, secLevel, orgPrim->deviceAddr,
                                modemStatus, CSR_BT_DEFAULT_BREAK_SIGNAL ,CSR_BT_DEFAULT_MSC_TIMEOUT);
            CsrBtJsr82SetBusy(instData);
        }
        else
        {
            sendCsrBtJsr82RfcConnectCfm(orgPrim->appHandle, orgPrim->reqID,
                localServerChannel, orgPrim->deviceAddr,
                CSR_BT_RESULT_CODE_JSR82_MEMORY_CAPACITY_ERROR);
        }
    }
    else
    {
        sendCsrBtJsr82RfcConnectCfm(orgPrim->appHandle, orgPrim->reqID,
            localServerChannel, orgPrim->deviceAddr,
            CSR_BT_RESULT_CODE_JSR82_MEMORY_CAPACITY_ERROR);
    }
    return TRUE;
}

void CsrBtJsr82RfcConnectReqHandler(Jsr82MainInstanceData *instData)
{
    CsrBtJsr82RfcConnectReq  *prim;
    dm_security_mode_t  curSecMode = CSR_BT_DEFAULT_SEC_MODE;

    prim = (CsrBtJsr82RfcConnectReq *)instData->recvMsg;
    instData->appHandle = prim->appHandle;
    instData->reqID = prim->reqID;

    if ((curSecMode < SEC_MODE2_SERVICE) && (prim->attrs & (CSR_BT_JSR82_AUTHENTICATE | CSR_BT_JSR82_ENCRYPT)))
    {
        CsrBtDeviceAddr dummyAddr = {0,0,0};
        sendCsrBtJsr82RfcConnectCfm(prim->appHandle, prim->reqID, 0, dummyAddr,
            CSR_BT_RESULT_CODE_JSR82_SECURITY_FAIL);
    }
    else
    {
        const jsr82DynSignalHandler cfmHandler[] = {CsrBtJsr82ConnectGetServerChannelCallBack};
        CsrBtJsr82SendCmMsgDown(instData, (CsrBtJsr82Prim **)&instData->recvMsg, CsrBtCmRegisterReq_struct(CSR_BT_JSR82_IFACEQUEUE, CSR_BT_CM_CONTEXT_UNUSED, CSR_BT_CM_SERVER_CHANNEL_DONT_CARE), cfmHandler);
    }
}

void CsrBtJsr82CmConnectCfmHandler(Jsr82MainInstanceData *instData)
{
    CsrBtCmConnectCfm    *prim;
    rfcConnectionStruct *rfcConnection;

    prim = (CsrBtCmConnectCfm *)instData->recvMsg;

    rfcConnection = getRfcConnectingInstace(instData->rfcConnectionPool);
    if (rfcConnection && rfcConnection->connecting)
    {
        if(prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
        {
            CsrUint8 modemStatus = 0;

            CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_DATA_CHANNEL,
                        prim->deviceAddr,prim->btConnId);

            rfcConnection->maxFrameSize = prim->profileMaxFrameSize;
            rfcConnection->jsr82ConnId = prim->btConnId;
            rfcConnection->connecting = FALSE;
            rfcConnection->connected = TRUE;
            rfcConnection->receiveInfo.receiveBuffer = CsrPmemAlloc(rfcConnection->maxFrameSize * CSR_BT_JSR82_RFC_BUFFER_SIZE);
            rfcConnection->receiveInfo.dataReqQueue = NULL;

            modemStatus = CSR_BT_MODEM_SEND_CTRL_DTE_DEFAULT;
            CsrBtCmControlReqSend(rfcConnection->jsr82ConnId, modemStatus, 0);
            sendCsrBtJsr82RfcConnectCfm(instData->appHandle, instData->reqID,
                rfcConnection->serverChannel, rfcConnection->deviceAddr,
                CSR_BT_RESULT_CODE_JSR82_SUCCESS);

            CsrBtJsr82AddConnection(instData, prim->deviceAddr);

        }
        else
        {
            sendCsrBtJsr82RfcConnectCfm(instData->appHandle, instData->reqID,
                rfcConnection->serverChannel, rfcConnection->deviceAddr,
                CSR_BT_RESULT_CODE_JSR82_UNSPECIFIED_ERROR);
            removeRfcConnection(rfcConnection->serverChannel, &(instData->rfcConnectionPool), instData);
        }
    }
    else
    {
        if(prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
        {
            CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_DATA_CHANNEL,
                        prim->deviceAddr,prim->btConnId);
        }
        CsrBtCmDisconnectReqSend(prim->btConnId);
    }
    CsrBtJsr82SetIdle(instData);
}

/*
  Ignore CSR_BT_CM_CONTROL_IND
*/
void CsrBtJsr82CmControlIndHandler(Jsr82MainInstanceData *instData)
{
    CSR_UNUSED(instData);
}

void CsrBtJsr82CmPortNegIndHandler(Jsr82MainInstanceData *instData)
{
    CsrBtCmPortnegInd                 *prim;

    prim = (CsrBtCmPortnegInd *)instData->recvMsg;
    if (prim->request == TRUE)
    {
        RFC_PORTNEG_VALUES_T portPar;

        portPar = createPortPar();
        CsrBtCmPortnegResSend(prim->btConnId, &portPar);
    }
    else
    {
        CsrBtCmPortnegResSend(prim->btConnId, &(prim->portPar) );
    }
}

static void sendCsrBtJsr82RfcCloseCfm(CsrSchedQid appHandle, CsrUint32 reqID,
    CsrUint8 localServerChannel, CsrBtResultCode result,
    CsrBool localTerminated)
{
    CsrBtJsr82RfcCloseCfm   *msg;

    msg = CsrPmemAlloc(sizeof(CsrBtJsr82RfcCloseCfm));
    msg->type = CSR_BT_JSR82_RFC_CLOSE_CFM;
    msg->reqID = reqID;
    msg->localServerChannel = localServerChannel;
    msg->resultCode = result;
    msg->localTerminated = localTerminated;

    CsrBtJsr82MessagePut(appHandle, msg);
}

void CsrBtJsr82RfcCloseReqHandler(Jsr82MainInstanceData *instData)
{
    CsrBtJsr82RfcCloseReq        *prim;
    rfcConnectionStruct     *rfcConnection;

    prim = (CsrBtJsr82RfcCloseReq *)instData->recvMsg;
    instData->appHandle = prim->appHandle;
    instData->reqID = prim->reqID;

    rfcConnection = getRfcConnection(prim->localServerChannel, instData->rfcConnectionPool);
    if (rfcConnection)
    {
        if (rfcConnection->connected)
        {
            rfcConnection->connected = FALSE;
            rfcConnection->destroy = FALSE;
            CsrBtCmDisconnectReqSend(rfcConnection->jsr82ConnId);
            CsrBtJsr82RemoveConnection(instData, rfcConnection->deviceAddr);

            closeSendOperation(rfcConnection->sendInfo, rfcConnection->serverChannel, instData);
            CsrPmemFree(rfcConnection->sendInfo);
            rfcConnection->sendInfo = NULL;

            closeReceiveOperation(rfcConnection);
            CsrPmemFree(rfcConnection->receiveInfo.receiveBuffer);
            rfcConnection->receiveInfo.receiveBuffer = NULL;

            CsrBtJsr82SetBusy(instData);
        }
        else if (rfcConnection->connectable)
        {
            sendCsrBtJsr82RfcCloseCfm(instData->appHandle, instData->reqID,
                prim->localServerChannel, CSR_BT_RESULT_CODE_JSR82_LINK_LOST,
                FALSE);
        }
        else
        {
            sendCsrBtJsr82RfcCloseCfm(instData->appHandle, instData->reqID,
                prim->localServerChannel, CSR_BT_RESULT_CODE_JSR82_SUCCESS,
                FALSE);
            if (rfcConnection->server)
            {
                closeRfcConnection(rfcConnection, instData);
            }
            else
            {
                removeRfcConnection(prim->localServerChannel, &(instData->rfcConnectionPool), instData);
            }
        }
    }
    else
    {
        sendCsrBtJsr82RfcCloseCfm(instData->appHandle, instData->reqID,
            prim->localServerChannel,
            CSR_BT_RESULT_CODE_JSR82_DEVICE_NOT_CONNECTED, FALSE);
    }
}

static void sendCsrBtJsr82RfcDisconnectCfm(CsrSchedQid appHandle, CsrUint32 reqID,
    CsrUint8 localServerChannel, CsrBtResultCode resultCode,
    CsrBool localTerminated)
{
    CsrBtJsr82RfcDisconnectCfm   *msg;

    msg = CsrPmemAlloc(sizeof(CsrBtJsr82RfcDisconnectCfm));
    msg->type = CSR_BT_JSR82_RFC_DISCONNECT_CFM;
    msg->reqID = reqID;
    msg->localServerChannel = localServerChannel;
    msg->resultCode = resultCode;
    msg->localTerminated = localTerminated;

    CsrBtJsr82MessagePut(appHandle, msg);
}

void CsrBtJsr82RfcDisconnectReqHandler(Jsr82MainInstanceData *instData)
{
    CsrBtJsr82RfcDisconnectReq   *prim;
    rfcConnectionStruct     *rfcConnection;

    prim = (CsrBtJsr82RfcDisconnectReq *)instData->recvMsg;
    instData->appHandle = prim->appHandle;
    instData->reqID = prim->reqID;

    rfcConnection = getRfcConnection(prim->localServerChannel, instData->rfcConnectionPool);
    if (rfcConnection)
    {
        if (rfcConnection->connected)
        {
            rfcConnection->connected = FALSE;
            rfcConnection->destroy = TRUE;
            CsrBtCmDisconnectReqSend(rfcConnection->jsr82ConnId);
            CsrBtJsr82RemoveConnection(instData, rfcConnection->deviceAddr);

            closeSendOperation(rfcConnection->sendInfo, rfcConnection->serverChannel, instData);
            CsrPmemFree(rfcConnection->sendInfo);
            rfcConnection->sendInfo = NULL;

            closeReceiveOperation(rfcConnection);
            CsrPmemFree(rfcConnection->receiveInfo.receiveBuffer);
            rfcConnection->receiveInfo.receiveBuffer = NULL;

            CsrBtJsr82SetBusy(instData);
        }
        else if (rfcConnection->connectable)
        {
            rfcConnection->connectable = FALSE;
            CsrBtCmCancelAcceptConnectReqSend(CSR_BT_JSR82_IFACEQUEUE, prim->localServerChannel);
            CsrBtJsr82SetBusy(instData);
        }
        else
        {
            sendCsrBtJsr82RfcDisconnectCfm(instData->appHandle, instData->reqID, prim->localServerChannel,
                CSR_BT_RESULT_CODE_JSR82_SUCCESS, TRUE);
            removeRfcConnection(prim->localServerChannel, &(instData->rfcConnectionPool), instData);
        }
    }
    else
    {
        sendCsrBtJsr82RfcDisconnectCfm(instData->appHandle, instData->reqID, prim->localServerChannel,
            CSR_BT_RESULT_CODE_JSR82_DEVICE_NOT_CONNECTED, FALSE);
    }
}

void CsrBtJsr82CmDisconnectIndHandler(Jsr82MainInstanceData *instData)
{
    CsrBtCmDisconnectInd *prim;
    rfcConnectionStruct *rfcConnection;

    prim = (CsrBtCmDisconnectInd *)instData->recvMsg;

    if (prim->status == TRUE)
    {
        rfcConnection = getRfcConnectionFromConnId(prim->btConnId, instData->rfcConnectionPool);

        if (rfcConnection)
        {
            if (prim->localTerminated == TRUE)
            {
                CsrBtJsr82PropgateRfcDisconnectIndEvent(instData, rfcConnection->serverChannel,
                    CSR_BT_RESULT_CODE_JSR82_SUCCESS, TRUE);
            }
            else
            {
                CsrBtJsr82PropgateRfcDisconnectIndEvent(instData, rfcConnection->serverChannel,
                    CSR_BT_RESULT_CODE_JSR82_SUCCESS, FALSE);
            }


            CsrBtCmLogicalChannelTypeReqSend(CSR_BT_NO_ACTIVE_LOGICAL_CHANNEL,
                    rfcConnection->deviceAddr,rfcConnection->jsr82ConnId);
            if (rfcConnection->connected)
            {

                rfcConnection->connected = FALSE;
                CsrBtJsr82RemoveConnection(instData, rfcConnection->deviceAddr);

                closeSendOperation(rfcConnection->sendInfo, rfcConnection->serverChannel, instData);
                CsrPmemFree(rfcConnection->sendInfo);
                rfcConnection->sendInfo = NULL;
                if (!rfcConnection->receiveInfo.validData)
                { /* no data left in receive buffer. Inform application */
                    closeReceiveOperation(rfcConnection);
                }
            }
            else if (rfcConnection->connectable)
            {
                rfcConnection->connectable = FALSE;
            }
            else if (rfcConnection->connecting)
            {
                rfcConnection->connecting = FALSE;
            }
            else
            {
                if (rfcConnection->destroy)
                {
                    sendCsrBtJsr82RfcDisconnectCfm(instData->appHandle,
                        instData->reqID, rfcConnection->serverChannel,
                        CSR_BT_RESULT_CODE_JSR82_SUCCESS, TRUE);
                    removeRfcConnection(rfcConnection->serverChannel, &(instData->rfcConnectionPool), instData);
                }
                else
                {
                    sendCsrBtJsr82RfcCloseCfm(instData->appHandle,
                        instData->reqID, rfcConnection->serverChannel,
                        CSR_BT_RESULT_CODE_JSR82_SUCCESS, TRUE);
                    if (rfcConnection->server)
                    {
                        closeRfcConnection(rfcConnection, instData);
                    }
                    else
                    {
                        removeRfcConnection(rfcConnection->serverChannel, &(instData->rfcConnectionPool), instData);
                    }
                }
                CsrBtJsr82SetIdle(instData);
            }
        }
    }
    else
    {
        CsrBtCmDisconnectReqSend(prim->btConnId);
    }
}

void CsrBtJsr82CmCancelAcceptConnectCfmHandler(Jsr82MainInstanceData *instData)
{
    CsrBtCmCancelAcceptConnectCfm    *prim;
    rfcConnectionStruct             *rfcConnection;

    prim = (CsrBtCmCancelAcceptConnectCfm *)instData->recvMsg;
    rfcConnection = getRfcConnection(prim->serverChannel, instData->rfcConnectionPool);
    if (rfcConnection)
    {
        if (rfcConnection->connectable)
        {
            rfcConnection->connectable = FALSE;

            closeSendOperation(rfcConnection->sendInfo, rfcConnection->serverChannel, instData);
            if (rfcConnection->sendInfo)
            {
                CsrPmemFree(rfcConnection->sendInfo);
                rfcConnection->sendInfo = NULL;
            }

            closeReceiveOperation(rfcConnection);
            CsrPmemFree(rfcConnection->receiveInfo.receiveBuffer);
            rfcConnection->receiveInfo.receiveBuffer = NULL;
        }
        else
        {
            sendCsrBtJsr82RfcDisconnectCfm(instData->appHandle, instData->reqID,
                prim->serverChannel,
                CSR_BT_RESULT_CODE_JSR82_CONNECT_FAILED, FALSE);
            removeRfcConnection(prim->serverChannel, &(instData->rfcConnectionPool), instData);
            CsrBtJsr82SetIdle(instData);
        }
    }
}

void CsrBtJsr82RfcCleanupConnection(Jsr82MainInstanceData *instData)
{
    CsrBtCmDisconnectInd *prim = (CsrBtCmDisconnectInd *)instData->recvMsg;
    rfcConnectionStruct *rfcConnection = getRfcConnectionFromConnId(prim->btConnId, instData->rfcConnectionPool);

    if (rfcConnection)
    {
        CsrBtCmLogicalChannelTypeReqSend(CSR_BT_NO_ACTIVE_LOGICAL_CHANNEL,
                    rfcConnection->deviceAddr,rfcConnection->jsr82ConnId);
        removeRfcConnection(rfcConnection->serverChannel, &(instData->rfcConnectionPool), instData);
    }
}

#ifdef CSR_BT_INSTALL_CM_PARK_MODE
void CsrBtJsr82CmModeChangeIndHandler(Jsr82MainInstanceData *instData)
{
    CsrBtCmRfcModeChangeInd * prim;
    prim = (CsrBtCmRfcModeChangeInd *) instData->recvMsg;


    /* If we're in park mode - get us into active */
    if(prim->mode == CSR_BT_PARK_MODE)
    {
        CsrBtCmRfcModeChangeReqSend(prim->btConnId, CSR_BT_ACTIVE_MODE);
    }
}
#endif

