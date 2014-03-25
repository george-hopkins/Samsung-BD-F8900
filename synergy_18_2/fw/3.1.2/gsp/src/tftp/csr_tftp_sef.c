/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_tftp_lib.h"
#include "csr_tftp_prim.h"
#include "csr_tftp_handler.h"
#include "csr_tftp_sef.h"
#include "csr_tftp_task.h"

#include "csr_ip_socket_lib.h"

#include "csr_log_text_2.h"

static void csrTftpUdpDataSend(CsrTftpConnection *conn, CsrUint16 socketHandle, CsrUint8 *ipAddress, CsrUint16 port,
    CsrUint16 dataLength, CsrUint8 *data, CsrUint8 state, CsrBool retransmit, CsrTftpConnectionClient *client, CsrSchedQid qid);

static CsrBool csrTftpCancelTimer(CsrTftpConnectionClient *client)
{
    CsrBool result = FALSE;

    if (client->timer != CSR_SCHED_TID_INVALID)
    {
        result = CsrSchedTimerCancel(client->timer, NULL, NULL);
        client->timer = CSR_SCHED_TID_INVALID;

        client->numberOfRetransmissions = 0;
    }

    return result;
}

static void csrTftpCloseTimeout(CsrUint16 mi, void *mv)
{
    CsrTftpInstanceData *instanceData = (CsrTftpInstanceData *) mv;

    if (instanceData->client.conn.state != CSR_TFTP_STATE_CONNECTION_IDLE)
    {
        instanceData->client.timer = CSR_SCHED_TID_INVALID;
        CsrIpSocketUdpCloseReqSend(instanceData->client.conn.socketHandle);
        CsrTftpConnectionClientInit(&instanceData->client);
    }
}

static void csrTftpDataRetransmissionTimeout(CsrUint16 qid, void *mv)
{
    CsrTftpConnectionClient *client = (CsrTftpConnectionClient *) mv;

    client->timer = CSR_SCHED_TID_INVALID;

    if ((client->conn.state == CSR_TFTP_STATE_CONNECTION_READ_DATA_SENDING) ||
        (client->conn.state == CSR_TFTP_STATE_CONNECTION_READ) ||
        (client->conn.state == CSR_TFTP_STATE_CONNECTION_READ_CLOSE) ||
        (client->conn.state == CSR_TFTP_STATE_CONNECTION_WRITE_DATA_SENDING) ||
        (client->conn.state == CSR_TFTP_STATE_CONNECTION_WRITE))
    {
        if (client->numberOfRetransmissions < CSR_TFTP_MAX_NUMBER_OF_RETRANSMISSIONS)
        {
            CsrUint8 *data = CsrPmemAlloc(client->dataLength);
            CsrMemCpy(data, client->data, client->dataLength);

            csrTftpUdpDataSend(&client->conn, client->conn.socketHandle, client->conn.ipAddress, client->remotePort,
                client->dataLength, data, client->conn.state, TRUE, client, (CsrSchedQid) qid);
            client->numberOfRetransmissions++;
        }
        else
        {
            CsrTftpDisconnectIndSend((CsrSchedQid) qid);
            CsrIpSocketUdpCloseReqSend(client->conn.socketHandle);
            CsrTftpConnectionClientInit(client);
        }
    }
}

static void csrTftpUdpDataSend(CsrTftpConnection *conn, CsrUint16 socketHandle, CsrUint8 *ipAddress, CsrUint16 port,
    CsrUint16 dataLength, CsrUint8 *data, CsrUint8 state, CsrBool retransmit, CsrTftpConnectionClient *client, CsrSchedQid qid)
{
    if ((conn->state == CSR_TFTP_STATE_CONNECTION_READ_DATA_SENDING) ||
        (conn->state == CSR_TFTP_STATE_CONNECTION_WRITE_DATA_SENDING) ||
        (conn->state == CSR_TFTP_STATE_CONNECTION_ERROR_DATA_SENDING))
    {
        CsrTftpUdpDataReq *elem = (CsrTftpUdpDataReq *) CsrPmemAlloc(sizeof(CsrTftpUdpDataReq));
        elem->state = state;
        elem->socketHandle = socketHandle;
        CsrMemCpy(elem->ipAddress, ipAddress, 4);
        elem->port = port;
        elem->dataLength = dataLength;
        elem->data = data;
        elem->retransmit = retransmit;

        CSR_TFTP_UDP_DATA_QUEUE(&conn->udpDataReqList, elem);
    }
    else
    {
        if (retransmit)
        {
            csrTftpCancelTimer(client);

            CsrPmemFree(client->data);
            client->timer = CsrSchedTimerSet(CSR_TFTP_DATA_RETRANSMISSION_TIMEOUT, csrTftpDataRetransmissionTimeout, qid, client);
            client->data = CsrPmemAlloc(dataLength);
            CsrMemCpy(client->data, data, dataLength);
            client->dataLength = dataLength;
        }

        CsrIpSocketUdpDataReqSend(socketHandle, ipAddress, port, dataLength, data);
        conn->state = state;
    }
}

static void csrTftpErrorSend(CsrTftpConnection *conn, CsrUint16 socketHandle, CsrUint8 *ipAddress, CsrUint16 port, CsrUint16 errorCode, CsrUint8 state)
{
    CsrUint16 opcode = CSR_TFTP_OPCODE_ERROR;
    CsrUint16 dataLength = errorCode == CSR_TFTP_ERROR_CODE_NOT_DEFINED ? CSR_TFTP_ERROR_CODE_REJECTED_NAME_LEN + CSR_TFTP_MIN_DATA_PACKET_LENGTH + 1 : CSR_TFTP_MIN_DATA_PACKET_LENGTH + 1;
    CsrUint8 *data = CsrPmemAlloc(dataLength);

    CSR_COPY_UINT16_TO_BIG_ENDIAN(opcode, data);
    CSR_COPY_UINT16_TO_BIG_ENDIAN(errorCode, &data[2]);

    if (errorCode == CSR_TFTP_ERROR_CODE_NOT_DEFINED)
    {
        CsrMemCpy(&data[4], CSR_TFTP_ERROR_CODE_REJECTED_NAME, CSR_TFTP_ERROR_CODE_REJECTED_NAME_LEN);
        data[12] = '\0';
    }
    else
    {
        data[4] = '\0';
    }

    csrTftpUdpDataSend(conn, socketHandle, ipAddress, port, dataLength, data, state, FALSE, NULL, 0);
}

static void csrTftpAckSend(CsrTftpConnectionClient *client, CsrUint16 socketHandle, CsrUint8 *ipAddress, CsrUint16 port, CsrUint16 blockNumber, CsrUint8 state, CsrSchedQid qid)
{
    CsrUint16 opcode = CSR_TFTP_OPCODE_ACK;
    CsrUint16 dataLength = 4;
    CsrUint8 *data = CsrPmemAlloc(dataLength);

    CSR_COPY_UINT16_TO_BIG_ENDIAN(opcode, data);
    CSR_COPY_UINT16_TO_BIG_ENDIAN(blockNumber, &data[2]);

    csrTftpUdpDataSend(&client->conn, socketHandle, ipAddress, port, dataLength, data, state, TRUE, client, qid);
}

static void csrTftpDataSend(CsrTftpConnectionClient *client, CsrUint16 socketHandle, CsrUint8 *ipAddress, CsrUint16 port, CsrUint16 blockNumber,
    CsrUint16 dataLength, CsrUint8 *data, CsrUint8 state, CsrSchedQid qid)
{
    CsrUint16 opcode = CSR_TFTP_OPCODE_DATA;
    CsrUint16 udpDataLength = dataLength + 4;
    CsrUint8 *udpData = CsrPmemAlloc(udpDataLength);

    CSR_COPY_UINT16_TO_BIG_ENDIAN(opcode, udpData);
    CSR_COPY_UINT16_TO_BIG_ENDIAN(blockNumber, &udpData[2]);
    CsrMemCpy(&udpData[4], data, dataLength);

    CsrPmemFree(data);

    csrTftpUdpDataSend(&client->conn, socketHandle, ipAddress, port, udpDataLength, udpData, state, TRUE, client, qid);
}

static void csrTftpSendDeactivateCfm(CsrTftpInstanceData *instanceData)
{
    CsrTftpDeactivateReq *prim = (CsrTftpDeactivateReq *) instanceData->msg;

    if (instanceData->conn.state == CSR_TFTP_STATE_CONNECTION_IDLE)
    {
        CsrTftpConnectionInit(&instanceData->conn, CSR_TFTP_PORT);
    }

    if (instanceData->client.conn.state == CSR_TFTP_STATE_CONNECTION_IDLE)
    {
        CsrTftpConnectionClientInit(&instanceData->client);
    }

    if ((instanceData->client.conn.state == CSR_TFTP_STATE_CONNECTION_IDLE) &&
        (instanceData->conn.state == CSR_TFTP_STATE_CONNECTION_IDLE))
    {
        CsrTftpDeactivateCfmSend(prim->qid);
        instanceData->state = CSR_TFTP_STATE_IDLE;
        instanceData->qid = CSR_SCHED_QID_INVALID;
    }
}

void CsrTftpActivateHandler(CsrTftpInstanceData *instanceData)
{
    CsrTftpActivateReq *prim = (CsrTftpActivateReq *) instanceData->msg;

    if ((instanceData->state == CSR_TFTP_STATE_IDLE) && (prim->family == CSR_TFTP_FAMILY_IP4))
    {
        CsrMemCpy(instanceData->conn.ipAddress, prim->ipAddress, 16);
        instanceData->conn.state = CSR_TFTP_STATE_CONNECTION_CREATING;

        instanceData->state = CSR_TFTP_STATE_ACTIVATING;
        instanceData->qid = prim->qid;

        CsrIpSocketUdpNewReqSend(CSR_TFTP_IFACEQUEUE);
    }
    else
    {
        CsrTftpActivateCfmSend(prim->qid, CSR_RESULT_FAILURE);
    }
}

static void csrTftpHandleDeactivate(CsrTftpConnection *conn)
{
    switch (conn->state)
    {
        case CSR_TFTP_STATE_CONNECTION_CREATING:
        case CSR_TFTP_STATE_CONNECTION_BINDING:
        case CSR_TFTP_STATE_CONNECTION_READ_DATA_SENDING:
        case CSR_TFTP_STATE_CONNECTION_READ_CLOSE:
        case CSR_TFTP_STATE_CONNECTION_WRITE_DATA_SENDING:
        case CSR_TFTP_STATE_CONNECTION_WRITE_WAIT_CLOSE:
        case CSR_TFTP_STATE_CONNECTION_ERROR_DATA_SENDING:
        case CSR_TFTP_STATE_CONNECTION_CLOSE:
        { /* Wait for a signal */
            break;
        }
        case CSR_TFTP_STATE_CONNECTION_ACTIVE:
        case CSR_TFTP_STATE_CONNECTION_READ:
        case CSR_TFTP_STATE_CONNECTION_WRITE:
        {
            CsrIpSocketUdpCloseReqSend(conn->socketHandle);
        }
        /* FALLTHROUGH */
        case CSR_TFTP_STATE_CONNECTION_IDLE:
        case CSR_TFTP_STATE_CONNECTION_CONNECTING:
        {
            conn->state = CSR_TFTP_STATE_CONNECTION_IDLE;
            break;
        }
        default:
        {
            CSR_LOG_TEXT_WARNING((CsrTftpLto, 0, "CSR_TFTP_DEACTIVATE_REQ received in wrong state 0x%X for tftp handle 0x%X",
                                  conn->state, conn));
            break;
        }
    }
}

void CsrTftpDeactivateHandler(CsrTftpInstanceData *instanceData)
{
    CsrTftpDeactivateReq *prim = (CsrTftpDeactivateReq *) instanceData->msg;
    instanceData->qid = prim->qid;

    csrTftpHandleDeactivate(&instanceData->conn);
    csrTftpHandleDeactivate(&instanceData->client.conn);

    instanceData->state = CSR_TFTP_STATE_DEACTIVATING;
    csrTftpSendDeactivateCfm(instanceData);
}

void CsrTftpConnectHandler(CsrTftpInstanceData *instanceData)
{
    CsrTftpConnectRes *prim = (CsrTftpConnectRes *) instanceData->msg;

    if (prim->result == CSR_RESULT_SUCCESS)
    {
        instanceData->client.conn.state = CSR_TFTP_STATE_CONNECTION_CREATING;

        CsrIpSocketUdpNewReqSend(CSR_TFTP_IFACEQUEUE);
    }
    else
    {
        CsrTftpConnection *connMain = &instanceData->conn;
        CsrTftpConnectionClient *client = &instanceData->client;
        CsrTftpConnection *conn = &client->conn;

        csrTftpErrorSend(connMain, connMain->socketHandle, conn->ipAddress, client->remotePort, CSR_TFTP_ERROR_CODE_NOT_DEFINED, CSR_TFTP_STATE_CONNECTION_ERROR_DATA_SENDING);
        CsrTftpConnectionClientInit(client);
    }
}

void CsrTftpReadHandler(CsrTftpInstanceData *instanceData)
{
    CsrTftpReadRes *prim = (CsrTftpReadRes *) instanceData->msg;
    CsrTftpConnectionClient *client = &instanceData->client;
    CsrTftpConnection *conn = &client->conn;

    if ((conn->state != CSR_TFTP_STATE_CONNECTION_CLOSE) && (conn->state != CSR_TFTP_STATE_CONNECTION_IDLE))
    {
        if (prim->result == CSR_RESULT_SUCCESS)
        {
            CsrUint8 state = prim->dataLength == CSR_TFTP_DEFAULT_BLKSIZE ? CSR_TFTP_STATE_CONNECTION_READ_DATA_SENDING : CSR_TFTP_STATE_CONNECTION_READ_CLOSE;

            csrTftpDataSend(client, conn->socketHandle, conn->ipAddress, client->remotePort, client->blockNumber,
                prim->dataLength, prim->data, state, instanceData->qid);
        }
        else
        {
            csrTftpErrorSend(conn, conn->socketHandle, conn->ipAddress, client->remotePort, CSR_TFTP_ERROR_CODE_NOT_DEFINED, CSR_TFTP_STATE_CONNECTION_CLOSE);
            CsrPmemFree(prim->data);
            prim->data = NULL;
        }
    }
    else /* The connection has been closed */
    {
        CsrPmemFree(prim->data);
        prim->data = NULL;
    }
}

void CsrTftpWriteHandler(CsrTftpInstanceData *instanceData)
{
    CsrTftpWriteRes *prim = (CsrTftpWriteRes *) instanceData->msg;
    CsrTftpConnectionClient *client = &instanceData->client;
    CsrTftpConnection *conn = &client->conn;

    if ((conn->state != CSR_TFTP_STATE_CONNECTION_CLOSE) && (conn->state != CSR_TFTP_STATE_CONNECTION_IDLE))
    {
        if (prim->result == CSR_RESULT_SUCCESS)
        {
            CsrUint8 state = client->lastDataSize == CSR_TFTP_DEFAULT_BLKSIZE ? CSR_TFTP_STATE_CONNECTION_WRITE_DATA_SENDING : CSR_TFTP_STATE_CONNECTION_WRITE_WAIT_CLOSE;
            csrTftpAckSend(client, conn->socketHandle, conn->ipAddress, client->remotePort, client->blockNumber, state, instanceData->qid);
        }
        else
        {
            csrTftpErrorSend(conn, conn->socketHandle, conn->ipAddress, client->remotePort, CSR_TFTP_ERROR_CODE_NOT_DEFINED, CSR_TFTP_STATE_CONNECTION_CLOSE);
        }
    }
}

void CsrTftpIpSocketUdpNewHandler(CsrTftpInstanceData *instanceData)
{
    CsrIpSocketUdpNewCfm *prim = (CsrIpSocketUdpNewCfm *) instanceData->msg;
    CsrTftpConnection *conn = instanceData->conn.state == CSR_TFTP_STATE_CONNECTION_CREATING ? &instanceData->conn : &instanceData->client.conn;
    CsrTftpConnection *connMain = &instanceData->conn;

    if (prim->result == CSR_RESULT_SUCCESS)
    {
        conn->state = CSR_TFTP_STATE_CONNECTION_BINDING;
        conn->socketHandle = prim->socketHandle;
        CsrIpSocketUdpBindReqSend(prim->socketHandle, connMain->ipAddress, conn->localPort);
    }
    else
    {
        CSR_LOG_TEXT_WARNING((CsrTftpLto, 0, "Not able to create new UDP socket - result: 9x%X",
                              prim->result));

        if (conn->localPort == CSR_TFTP_PORT)
        {
            instanceData->state = CSR_TFTP_STATE_IDLE;
            CsrTftpActivateCfmSend(instanceData->qid, CSR_RESULT_FAILURE);
            CsrTftpConnectionInit(&instanceData->conn, CSR_TFTP_PORT);
        }
        else
        {
            CsrTftpConnectionClient *client = &instanceData->client;

            csrTftpErrorSend(connMain, connMain->socketHandle, conn->ipAddress, client->remotePort, CSR_TFTP_ERROR_CODE_NOT_DEFINED, CSR_TFTP_STATE_CONNECTION_ERROR_DATA_SENDING);
            CsrTftpDisconnectIndSend(instanceData->qid);
            CsrTftpConnectionClientInit(&instanceData->client);
        }
    }
}

void CsrTftpIpSocketUdpBindHandler(CsrTftpInstanceData *instanceData)
{
    CsrIpSocketUdpBindCfm *prim = (CsrIpSocketUdpBindCfm *) instanceData->msg;
    CsrTftpConnection *conn = instanceData->conn.state == CSR_TFTP_STATE_CONNECTION_BINDING ? &instanceData->conn : &instanceData->client.conn;
    CsrTftpConnectionClient *client = &instanceData->client;

    if (prim->result == CSR_RESULT_SUCCESS)
    {
        conn->state = CSR_TFTP_STATE_CONNECTION_ACTIVE;
        conn->localPort = prim->port;

        if (conn->localPort == CSR_TFTP_PORT)
        {
            instanceData->state = CSR_TFTP_STATE_ACTIVATED;
            CsrTftpActivateCfmSend(instanceData->qid, CSR_RESULT_SUCCESS);
        }
        else
        {
            if (client->readRequest)
            {
                CsrTftpReadIndSend(instanceData->qid);
                client->blockNumber++;
            }
            else
            {
                csrTftpAckSend(client, conn->socketHandle, conn->ipAddress, client->remotePort, 0, CSR_TFTP_STATE_CONNECTION_WRITE_DATA_SENDING, instanceData->qid);
            }
        }
    }
    else
    {
        CSR_LOG_TEXT_WARNING((CsrTftpLto, 0, "Not able to bind to port 0x%X", prim->port));

        if (conn->localPort == CSR_TFTP_PORT)
        {
            instanceData->state = CSR_TFTP_STATE_IDLE;
            CsrTftpActivateCfmSend(instanceData->qid, CSR_RESULT_FAILURE);
            CsrIpSocketUdpCloseReqSend(instanceData->conn.socketHandle);
            CsrTftpConnectionInit(&instanceData->conn, CSR_TFTP_PORT);
        }
        else
        {
            CsrTftpConnection *connMain = &instanceData->conn;

            csrTftpErrorSend(connMain, connMain->socketHandle, conn->ipAddress, client->remotePort, CSR_TFTP_ERROR_CODE_NOT_DEFINED, CSR_TFTP_STATE_CONNECTION_ERROR_DATA_SENDING);
            CsrTftpDisconnectIndSend(instanceData->qid);
            CsrIpSocketUdpCloseReqSend(instanceData->client.conn.socketHandle);
            CsrTftpConnectionClientInit(&instanceData->client);
        }
    }
}

static void csrTftpHandleUdpDataInd(CsrTftpInstanceData *instanceData, CsrIpSocketUdpDataInd *prim)
{
    CsrTftpOpcode opcode;
    CsrTftpConnection *conn = &instanceData->conn;
    CsrTftpConnectionClient *client = &instanceData->client;

    if ((client->conn.state != CSR_TFTP_STATE_CONNECTION_IDLE) && (CsrMemCmp(client->conn.ipAddress, prim->ipAddress, 4) == 0) && (prim->port == client->remotePort)) /* Could indicate that the response for a read or write never got to the remote device, will let the retransmission handle this */
    {
        CsrPmemFree(prim->data);
        prim->data = NULL;
        return;
    }

    opcode = CSR_GET_UINT16_FROM_BIG_ENDIAN(prim->data);

    switch (opcode)
    {
        case CSR_TFTP_OPCODE_READ:
        case CSR_TFTP_OPCODE_WRITE:
        {
            CsrUint16 strLen;
            CsrCharString *fileName = NULL;
            CsrUint16 offset = 2;

            if ((client->conn.state != CSR_TFTP_STATE_CONNECTION_IDLE)) /* A connection already exists */
            {
                csrTftpErrorSend(conn, conn->socketHandle, prim->ipAddress, prim->port, CSR_TFTP_ERROR_CODE_NOT_DEFINED, CSR_TFTP_STATE_CONNECTION_ERROR_DATA_SENDING);
                CsrPmemFree(prim->data);
                prim->data = NULL;
                break;
            }
            else if (prim->data[prim->dataLength - 1] != '\0')
            {
                CSR_LOG_TEXT_WARNING((CsrTftpLto, 0, "Data received from socket handle 0x%X does not end with a '\0'",
                                      prim->socketHandle));
                csrTftpErrorSend(conn, conn->socketHandle, prim->ipAddress, prim->port, CSR_TFTP_ERROR_CODE_ILLEGAL_TFTP_OPERATION, CSR_TFTP_STATE_CONNECTION_ERROR_DATA_SENDING);
                CsrPmemFree(prim->data);
                prim->data = NULL;
                break;
            }

            CsrMemCpy(client->conn.ipAddress, prim->ipAddress, 4);
            client->remotePort = prim->port;

            strLen = (CsrUint16) CsrStrLen((const CsrCharString *) (&prim->data[offset])) + 1;
            fileName = (CsrCharString *) CsrPmemAlloc(strLen);
            CsrMemCpy(fileName, &prim->data[offset], strLen);
            offset += strLen;

            if (prim->dataLength > offset)
            {
                strLen = (CsrUint16) CsrStrLen((const CsrCharString *) (&prim->data[offset]));

                if ((strLen != CSR_TFTP_MODE_OCTET_NAME_LEN) && CsrStrNICmp(CSR_TFTP_MODE_OCTET_NAME, (const CsrCharString *) &prim->data[offset], CSR_TFTP_MODE_OCTET_NAME_LEN))
                {
                    CSR_LOG_TEXT_WARNING((CsrTftpLto, 0, "Data received from socket handle 0x%X does not contain the 'mode' == 'octet'",
                                          prim->socketHandle));
                    csrTftpErrorSend(conn, conn->socketHandle, prim->ipAddress, prim->port, CSR_TFTP_ERROR_CODE_NOT_DEFINED, CSR_TFTP_STATE_CONNECTION_ERROR_DATA_SENDING);
                    CsrPmemFree(prim->data);
                    prim->data = NULL;
                    CsrPmemFree(fileName);
                    break;
                }
            }
            else
            {
                CSR_LOG_TEXT_WARNING((CsrTftpLto, 0, "Data received from socket handle 0x%X does not contain the 'mode'",
                                      prim->socketHandle));
                csrTftpErrorSend(conn, conn->socketHandle, prim->ipAddress, prim->port, CSR_TFTP_ERROR_CODE_ILLEGAL_TFTP_OPERATION, CSR_TFTP_STATE_CONNECTION_ERROR_DATA_SENDING);
                CsrPmemFree(prim->data);
                prim->data = NULL;
                CsrPmemFree(fileName);
                break;
            }

            client->readRequest = opcode == CSR_TFTP_OPCODE_WRITE ? FALSE : TRUE;
            client->conn.state = CSR_TFTP_STATE_CONNECTION_CONNECTING;

            CsrTftpConnectIndSend(instanceData->qid, CSR_TFTP_FAMILY_IP4, prim->ipAddress, prim->port, opcode, fileName);
            CsrPmemFree(prim->data);
            prim->data = NULL;

            break;
        }
        case CSR_TFTP_OPCODE_ERROR:
        { /* Ignore */
            break;
        }
        case CSR_TFTP_OPCODE_DATA:
        case CSR_TFTP_OPCODE_ACK:
        default:
        {
            CSR_LOG_TEXT_WARNING((CsrTftpLto, 0, "Unknown opcode received %d from socket handle 0x%X", opcode, prim->socketHandle));
            csrTftpErrorSend(conn, conn->socketHandle, prim->ipAddress, prim->port, CSR_TFTP_ERROR_CODE_ILLEGAL_TFTP_OPERATION, CSR_TFTP_STATE_CONNECTION_ERROR_DATA_SENDING);
            CsrPmemFree(prim->data);
            prim->data = NULL;
            break;
        }
    }
}

static void csrTftpHandleUdpDataIndClient(CsrTftpInstanceData *instanceData, CsrIpSocketUdpDataInd *prim)
{
    CsrTftpOpcode opcode;
    CsrTftpConnectionClient *client = &instanceData->client;
    CsrTftpConnection *conn = &client->conn;

    if (conn->state == CSR_TFTP_STATE_CONNECTION_IDLE) /* Crossing signal, ignore */
    {
        CsrPmemFree(prim->data);
        prim->data = NULL;
        return;
    }
    else if ((CsrMemCmp(conn->ipAddress, prim->ipAddress, 4) != 0) || (prim->port != client->remotePort))
    {
        csrTftpErrorSend(conn, conn->socketHandle, prim->ipAddress, prim->port, CSR_TFTP_ERROR_CODE_NOT_DEFINED, CSR_TFTP_STATE_CONNECTION_ERROR_DATA_SENDING);
        CsrPmemFree(prim->data);
        prim->data = NULL;
        return;
    }

    opcode = CSR_GET_UINT16_FROM_BIG_ENDIAN(prim->data);

    switch (opcode)
    {
        case CSR_TFTP_OPCODE_DATA:
        {
            if (client->readRequest)
            {
                CSR_LOG_TEXT_WARNING((CsrTftpLto, 0, "Received data from socket handle but are in the middle of reading data",
                                      opcode, prim->socketHandle));

                csrTftpErrorSend(conn, conn->socketHandle, conn->ipAddress, client->remotePort, CSR_TFTP_ERROR_CODE_ILLEGAL_TFTP_OPERATION, CSR_TFTP_STATE_CONNECTION_CLOSE);
            }
            else
            {
                CsrUint16 blockNumber = CSR_GET_UINT16_FROM_BIG_ENDIAN(&prim->data[2]);
                CsrBool cancelled = csrTftpCancelTimer(client);

                if (blockNumber == client->blockNumber + 1)
                {
                    CsrUint8 *data = NULL;

                    client->lastDataSize = (CsrUint16) prim->dataLength - CSR_TFTP_MIN_DATA_PACKET_LENGTH;

                    if (prim->dataLength > CSR_TFTP_MIN_DATA_PACKET_LENGTH)
                    {
                        data = CsrPmemAlloc(client->lastDataSize);
                        CsrMemCpy(data, &prim->data[CSR_TFTP_MIN_DATA_PACKET_LENGTH], client->lastDataSize);
                    }

                    CsrTftpWriteIndSend(instanceData->qid, client->lastDataSize, data);
                    client->blockNumber++;
                }
                else if ((blockNumber == client->blockNumber) && (client->conn.state == CSR_TFTP_STATE_CONNECTION_WRITE_WAIT_CLOSE)) /* Retransmission of a already received blocknumber and it is the last packet means the ACK has been lost, retransmit the ACK */
                {
                    if (cancelled)
                    {
                        csrTftpDataRetransmissionTimeout(instanceData->qid, client);
                    }
                }
            }

            CsrPmemFree(prim->data);
            prim->data = NULL;

            break;
        }
        case CSR_TFTP_OPCODE_ACK:
        {
            if (conn->state == CSR_TFTP_STATE_CONNECTION_READ_CLOSE) /* Last packet, disconnect */
            {
                CsrIpSocketUdpCloseReqSend(conn->socketHandle);
                CsrTftpConnectionClientInit(client);
            }
            else if ((conn->state == CSR_TFTP_STATE_CONNECTION_READ) || (conn->state == CSR_TFTP_STATE_CONNECTION_READ_DATA_SENDING))
            {
                CsrUint16 blockNumber = CSR_GET_UINT16_FROM_BIG_ENDIAN(&prim->data[2]);

                if (blockNumber == client->blockNumber)
                {
                    CsrTftpReadIndSend(instanceData->qid);
                    client->blockNumber++;
                    csrTftpCancelTimer(client);
                }
            }
            else
            {
                CSR_LOG_TEXT_WARNING((CsrTftpLto, 0, "Received ACK from socket handle 0x%X but are not in reading state but in state 0x%X",
                                      conn->socketHandle, conn->state));

                csrTftpErrorSend(conn, conn->socketHandle, conn->ipAddress, client->remotePort, CSR_TFTP_ERROR_CODE_ILLEGAL_TFTP_OPERATION, CSR_TFTP_STATE_CONNECTION_CLOSE);
            }

            CsrPmemFree(prim->data);
            prim->data = NULL;

            break;
        }
        case CSR_TFTP_OPCODE_ERROR:
        {
            csrTftpCancelTimer(client);

            if ((conn->state == CSR_TFTP_STATE_CONNECTION_READ_DATA_SENDING) ||
                (conn->state == CSR_TFTP_STATE_CONNECTION_WRITE_DATA_SENDING) ||
                (conn->state == CSR_TFTP_STATE_CONNECTION_ERROR_DATA_SENDING))
            { /* Wait for a data cfm */
                conn->state = CSR_TFTP_STATE_CONNECTION_CLOSE;
            }
            else if (conn->state == CSR_TFTP_STATE_CONNECTION_READ_CLOSE)
            {
                CsrIpSocketUdpCloseReqSend(conn->socketHandle);
                CsrTftpConnectionClientInit(client);
            }
            else
            {
                CsrIpSocketUdpCloseReqSend(conn->socketHandle);
                CsrTftpDisconnectIndSend(instanceData->qid);
                CsrTftpConnectionClientInit(client);
            }

            CsrPmemFree(prim->data);
            prim->data = NULL;
            break;
        }
        case CSR_TFTP_OPCODE_READ:
        case CSR_TFTP_OPCODE_WRITE:
        default:
        {
            CSR_LOG_TEXT_WARNING((CsrTftpLto, 0, "Unknown opcode received %d from socket handle 0x%X", opcode, prim->socketHandle));
            csrTftpErrorSend(conn, conn->socketHandle, prim->ipAddress, prim->port, CSR_TFTP_ERROR_CODE_ILLEGAL_TFTP_OPERATION, CSR_TFTP_STATE_CONNECTION_CLOSE);
            CsrPmemFree(prim->data);
            prim->data = NULL;
            break;
        }
    }
}

void CsrTftpIpSocketUdpDataIndHandler(CsrTftpInstanceData *instanceData)
{
    CsrIpSocketUdpDataInd *prim = (CsrIpSocketUdpDataInd *) instanceData->msg;
    CsrTftpConnection *conn = instanceData->conn.socketHandle == prim->socketHandle ? &instanceData->conn : &instanceData->client.conn;

    if (prim->dataLength < CSR_TFTP_MIN_DATA_PACKET_LENGTH)
    {
        CsrTftpConnectionClient *client = &instanceData->client;

        CSR_LOG_TEXT_WARNING((CsrTftpLto, 0, "Not enough data received (%d bytes) from socket handle 0x%X",
                              prim->dataLength, prim->socketHandle));

        if (conn->localPort == CSR_TFTP_PORT)
        {
            conn->state = CSR_TFTP_STATE_CONNECTION_ERROR_DATA_SENDING;
        }
        else
        {
            if ((CsrMemCmp(conn->ipAddress, prim->ipAddress, 4) != 0) || (prim->port != client->remotePort))
            {
                conn->state = CSR_TFTP_STATE_CONNECTION_ERROR_DATA_SENDING;
            }
            else
            {
                conn->state = CSR_TFTP_STATE_CONNECTION_CLOSE;
            }
        }

        csrTftpErrorSend(conn, conn->socketHandle, conn->ipAddress, client->remotePort, CSR_TFTP_ERROR_CODE_ILLEGAL_TFTP_OPERATION, conn->state);
        CsrPmemFree(prim->data);
        prim->data = NULL;
        return;
    }

    if (conn->localPort == CSR_TFTP_PORT)
    {
        csrTftpHandleUdpDataInd(instanceData, prim);
    }
    else
    {
        csrTftpHandleUdpDataIndClient(instanceData, prim);
    }
}

static CsrBool csrTftpHandleWaitingSignals(CsrTftpInstanceData *instanceData, CsrTftpConnection *conn, CsrInt16 state)
{
    CsrBool result = FALSE;
    CsrTftpUdpDataReq *elem = CSR_TFTP_UDP_DATA_DEQUEUE(&conn->udpDataReqList);

    if (elem)
    {
        CsrTftpConnectionClient *client = &instanceData->client;
        CsrUint8 s = state == -1 ? elem->state : state;

        csrTftpUdpDataSend(conn, elem->socketHandle, elem->ipAddress, elem->port, elem->dataLength, elem->data, s, elem->retransmit, client, instanceData->qid);
        CsrPmemFree(elem);
        result = TRUE;
    }

    return result;
}

void CsrTftpIpSocketUdpDataCfmHandler(CsrTftpInstanceData *instanceData)
{
    CsrIpSocketUdpDataCfm *prim = (CsrIpSocketUdpDataCfm *) instanceData->msg;
    CsrTftpConnection *conn = instanceData->conn.socketHandle == prim->socketHandle ? &instanceData->conn : &instanceData->client.conn;
    CsrTftpConnectionClient *client = &instanceData->client;

    switch (conn->state)
    {
        case CSR_TFTP_STATE_CONNECTION_CLOSE:
        {
            if (csrTftpHandleWaitingSignals(instanceData, conn, CSR_TFTP_STATE_CONNECTION_CLOSE) == FALSE)
            {
                CsrIpSocketUdpCloseReqSend(conn->socketHandle);
                CsrTftpDisconnectIndSend(instanceData->qid);
                CsrTftpConnectionClientInit(&instanceData->client);
            }
            break;
        }
        case CSR_TFTP_STATE_CONNECTION_WRITE_WAIT_CLOSE:
        {
            if (csrTftpHandleWaitingSignals(instanceData, conn, CSR_TFTP_STATE_CONNECTION_WRITE_WAIT_CLOSE) == FALSE)
            {
                conn->state = CSR_TFTP_STATE_CONNECTION_ACTIVE;
                csrTftpCancelTimer(client);
                client->timer = CsrSchedTimerSet(CSR_TFTP_DATA_RETRANSMISSION_TIMEOUT, csrTftpCloseTimeout, 0, instanceData);
            }
            break;
        }
        case CSR_TFTP_STATE_CONNECTION_READ_CLOSE:
        {
            csrTftpHandleWaitingSignals(instanceData, conn, CSR_TFTP_STATE_CONNECTION_READ_CLOSE);
            break;
        }
        case CSR_TFTP_STATE_CONNECTION_IDLE:
        { /* Ignore, already closed */
            break;
        }
        default:
        {
            if (csrTftpHandleWaitingSignals(instanceData, conn, -1) == FALSE)
            {
                if (conn->localPort == CSR_TFTP_PORT)
                {
                    conn->state = CSR_TFTP_STATE_CONNECTION_ACTIVE;
                }
                else
                {
                    conn->state = client->readRequest ? CSR_TFTP_STATE_CONNECTION_READ : CSR_TFTP_STATE_CONNECTION_WRITE;
                }
            }
            break;
        }
    }
}

void CsrTftpIpSocketUdpNewHandlerDeactivating(CsrTftpInstanceData *instanceData)
{
    CsrIpSocketUdpNewCfm *prim = (CsrIpSocketUdpNewCfm *) instanceData->msg;
    CsrTftpConnection *conn = instanceData->conn.state == CSR_TFTP_STATE_CONNECTION_CREATING ? &instanceData->conn : &instanceData->client.conn;

    if (prim->result == CSR_RESULT_SUCCESS)
    {
        CsrIpSocketUdpCloseReqSend(prim->socketHandle);
    }

    conn->state = CSR_TFTP_STATE_CONNECTION_IDLE;
    csrTftpSendDeactivateCfm(instanceData);
}

void CsrTftpIpSocketUdpBindHandlerDeactivating(CsrTftpInstanceData *instanceData)
{
    CsrIpSocketUdpBindCfm *prim = (CsrIpSocketUdpBindCfm *) instanceData->msg;
    CsrTftpConnection *conn = instanceData->conn.state == CSR_TFTP_STATE_CONNECTION_BINDING ? &instanceData->conn : &instanceData->client.conn;

    CsrIpSocketUdpCloseReqSend(prim->socketHandle);

    conn->state = CSR_TFTP_STATE_CONNECTION_IDLE;
    csrTftpSendDeactivateCfm(instanceData);
}

void CsrTftpIpSocketUdpDataIndHandlerDeactivating(CsrTftpInstanceData *instanceData)
{
    CsrIpSocketUdpDataInd *prim = (CsrIpSocketUdpDataInd *) instanceData->msg;
    CsrPmemFree(prim->data);
    prim->data = NULL;
}

void CsrTftpIpSocketUdpDataCfmHandlerDeactivating(CsrTftpInstanceData *instanceData)
{
    CsrIpSocketUdpDataCfm *prim = (CsrIpSocketUdpDataCfm *) instanceData->msg;
    CsrTftpConnection *conn = instanceData->conn.socketHandle == prim->socketHandle ? &instanceData->conn : &instanceData->client.conn;

    CsrIpSocketUdpCloseReqSend(prim->socketHandle);

    conn->state = CSR_TFTP_STATE_CONNECTION_IDLE;
    csrTftpSendDeactivateCfm(instanceData);
}

void CsrTftpConnectionInit(CsrTftpConnection *conn, CsrUint16 port)
{
    CsrTftpUdpDataReq *msg;

    while ((msg = CSR_TFTP_UDP_DATA_DEQUEUE(&conn->udpDataReqList)) != NULL)
    {
        CsrPmemFree(msg->data);
        CsrPmemFree(msg);
    }

    conn->state = CSR_TFTP_STATE_CONNECTION_IDLE;
    conn->udpDataReqList.front = NULL;
    conn->udpDataReqList.back = NULL;
    conn->socketHandle = CSR_TFTP_INVALID_SOCKET_HANDLE;
    CsrMemSet(conn->ipAddress, 0, 16);
    conn->localPort = port;
}

void CsrTftpConnectionClientInit(CsrTftpConnectionClient *client)
{
    CsrTftpConnectionInit(&client->conn, 0);
    client->remotePort = 0;
    client->readRequest = FALSE;
    client->lastDataSize = 0;
    client->blockNumber = 0;
    csrTftpCancelTimer(client);
    client->numberOfRetransmissions = 0;
    client->dataLength = 0;
    CsrPmemFree(client->data);
    client->data = NULL;
}

void CsrTftpFifoQueue(CsrTftpFifo *fifo, void *data)
{
    CsrTftpFifoElement *elem = (CsrTftpFifoElement *) CsrPmemAlloc(sizeof(CsrTftpFifoElement));

    elem->next = NULL;
    elem->data = data;

    if (fifo->back == NULL) /* The FIFO is empty */
    {
        fifo->back = elem;
        fifo->front = elem;
    }
    else
    {
        fifo->back->next = elem;
        fifo->back = elem;
    }
}

void *CsrTftpFifoDequeue(CsrTftpFifo *fifo)
{
    void *result = NULL;

    if (fifo->front)
    {
        CsrTftpFifoElement *tmp;

        tmp = fifo->front->next;
        result = fifo->front->data;

        CsrPmemFree(fifo->front);

        if (tmp)
        {
            fifo->front = tmp;
        }
        else /* FIFO is now empty */
        {
            fifo->front = NULL;
            fifo->back = NULL;
        }
    }

    return result;
}
