/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_usr_config.h"

#include "csr_dhcp_server_lib.h"
#include "csr_dhcp_server_prim.h"
#include "csr_dhcp_server_handler.h"
#include "csr_dhcp_server_util.h"
#include "csr_dhcp_server_sef.h"
#include "csr_dhcp_server_task.h"

#include "csr_ip_socket_prim.h"
#include "csr_ip_socket_lib.h"

#include "csr_log_text_2.h"

static void csrDhcpServerCloseBroadcastSocket(CsrDhcpServerInstanceData *instanceData)
{
    if (instanceData->broadcastSocketHandle != CSR_DHCP_SERVER_INVALID_SOCKET_HANDLE)
    {
        if (CSR_DHCP_SERVER_EXISTS(&instanceData->servers) == FALSE)
        {
            CsrIpSocketUdpCloseReqSend((CsrUint16) instanceData->broadcastSocketHandle);
            instanceData->broadcastSocketHandle = CSR_DHCP_SERVER_INVALID_SOCKET_HANDLE;
        }
    }
}

void CsrDhcpServerCreateHandler(CsrDhcpServerInstanceData *instanceData)
{
    CsrDhcpServerCreateReq *prim = (CsrDhcpServerCreateReq *) instanceData->msg;

    CsrDhcpServerData *server = CSR_DHCP_SERVER_ADD(&instanceData->servers);

    server->qid = prim->qid;
    server->leaseTime = prim->leaseTime;

    CsrMemCpy(server->networkMask, prim->networkMask, 4);
    CsrMemCpy(server->ipAddress, prim->ipAddress, 4);
    CsrMemCpy(server->gateway, prim->gateway, 4);
    CsrMemCpy(server->dns1, prim->dns1, 4);
    CsrMemCpy(server->dns2, prim->dns2, 4);

    server->network[0] = prim->network[0] & server->networkMask[0];
    server->network[1] = prim->network[1] & server->networkMask[1];
    server->network[2] = prim->network[2] & server->networkMask[2];
    server->network[3] = prim->network[3] & server->networkMask[3];

    CsrIpSocketUdpNewReqSend(CSR_DHCP_SERVER_IFACEQUEUE);
}

void CsrDhcpServerDestroyHandler(CsrDhcpServerInstanceData *instanceData)
{
    CsrCmnListElm_t *element, *nextElement;
#ifdef CSR_DHCP_SERVER_USE_IFCONFIG_ARP
    CsrDhcpServerResponse *resp, *respPrev, *respNext;
#endif
    CsrDhcpServerDestroyReq *prim = (CsrDhcpServerDestroyReq *) instanceData->msg;
    CsrDhcpServerData *server = (CsrDhcpServerData *) prim->serverHandle;

#ifdef CSR_DHCP_SERVER_USE_IFCONFIG_ARP
    /* Get rid of any queued responses. */
    for (resp = instanceData->responseQueue, respPrev = NULL;
         resp != NULL;
         resp = respNext)
    {
        respNext = resp->next;

        if (resp->server == server)
        {
            if (respPrev == NULL)
            {
                instanceData->responseQueue = respNext;
            }
            else
            {
                respPrev->next = respNext;
            }

            CsrPmemFree(resp->data);
            CsrPmemFree(resp);
        }
        else
        {
            respPrev = resp;
        }
    }
#endif

    CsrIpSocketUdpCloseReqSend((CsrUint16) server->socketHandle);
    CsrDhcpServerDestroyCfmSend(server->qid, server, CSR_RESULT_SUCCESS);

    for (element = server->clients.first; element; element = nextElement)
    {
        CsrDhcpServerClient *client = (CsrDhcpServerClient *) element;
        CsrDhcpServerTimer *timer = CsrDhcpServerTimerFindFromClient(server, client);

        if (timer) /* Cancel the timer */
        {
            CsrDhcpServerTimerRemove(server, timer);
        }

        nextElement = element->next;
        CsrCmnListElementRemove(&server->clients, element);
    }

    CSR_DHCP_SERVER_REMOVE(&instanceData->servers, server);
    csrDhcpServerCloseBroadcastSocket(instanceData);
}

void CsrDhcpServerStartHandler(CsrDhcpServerInstanceData *instanceData)
{
    CsrDhcpServerStartReq *prim = (CsrDhcpServerStartReq *) instanceData->msg;
    CsrDhcpServerData *server = (CsrDhcpServerData *) prim->serverHandle;

    if (server->state == CSR_DHCP_SERVER_STATE_STOPPED)
    {
        server->state = CSR_DHCP_SERVER_STATE_STARTED;

        CsrDhcpServerStartCfmSend(server->qid, server, CSR_RESULT_SUCCESS);
        CSR_DHCP_SERVER_INVALID_IP_REMOVE_ALL(server->invalidIpList);
        server->invalidIpList = NULL;
    }
    else
    {
        CsrDhcpServerStartCfmSend(server->qid, server, CSR_RESULT_FAILURE);
    }
}

void CsrDhcpServerStopHandler(CsrDhcpServerInstanceData *instanceData)
{
    CsrDhcpServerStopReq *prim = (CsrDhcpServerStopReq *) instanceData->msg;
    CsrDhcpServerData *server = (CsrDhcpServerData *) prim->serverHandle;

    if (server->state == CSR_DHCP_SERVER_STATE_STARTED)
    {
        server->state = CSR_DHCP_SERVER_STATE_STOPPED;
        server->sendingData = FALSE;

        CSR_DHCP_SERVER_MESSAGE_DEQUEUE_ALL(&server->pendingMessages);

        CsrDhcpServerStopCfmSend(server->qid, server, CSR_RESULT_SUCCESS);
    }
    else
    {
        CsrDhcpServerStopCfmSend(server->qid, server, CSR_RESULT_FAILURE);
    }
}

typedef struct
{
    CsrDhcpServerLease *leases;
    CsrUint16          *count;
} CsrDhcpServerLeaseIterateData;

static void csrDhcpServerLeaseIterateFunc(CsrCmnListElm_t *elem, void *data)
{
    CsrDhcpServerClient *clientElem = (CsrDhcpServerClient *) elem;
    CsrDhcpServerLeaseIterateData *leaseIter = (CsrDhcpServerLeaseIterateData *) data;

    if ((clientElem->info->ipAddressState == CSR_DHCP_SERVER_IP_ADDRESS_STATE_ACTIVE) ||
        (clientElem->info->ipAddressState == CSR_DHCP_SERVER_IP_ADDRESS_STATE_INACTIVE))
    {
        leaseIter->leases[*leaseIter->count] = clientElem->info->lease;
        (*leaseIter->count)++;
    }
}

void CsrDhcpServerLeasesGetHandler(CsrDhcpServerInstanceData *instanceData)
{
    CsrDhcpServerClient *client;
    CsrCmnListElm_t *clientElem;
    CsrDhcpServerLeasesGetReq *prim = (CsrDhcpServerLeasesGetReq *) instanceData->msg;
    CsrDhcpServerData *server = (CsrDhcpServerData *) prim->serverHandle;
    CsrDhcpServerLease *leases = NULL;
    CsrUint16 numOfClients = 0;

    for (clientElem = server->clients.first; clientElem; clientElem = clientElem->next) /* Get the number of active or inactive clients */
    {
        client = (CsrDhcpServerClient *) clientElem;

        if ((client->info->ipAddressState == CSR_DHCP_SERVER_IP_ADDRESS_STATE_ACTIVE) ||
            (client->info->ipAddressState == CSR_DHCP_SERVER_IP_ADDRESS_STATE_INACTIVE))
        {
            numOfClients++;
        }
    }

    if (numOfClients > 0)
    {
        CsrDhcpServerLeaseIterateData data;
        CsrUint16 i = 0;

        leases = CsrPmemAlloc(sizeof(CsrDhcpServerLease) * numOfClients);
        data.leases = leases;
        data.count = &i;

        CsrCmnListIterate(&server->clients, csrDhcpServerLeaseIterateFunc, &data);
    }

    CsrDhcpServerLeasesGetCfmSend(server->qid, server, numOfClients, leases);
}

void CsrDhcpServerLeasesSetHandler(CsrDhcpServerInstanceData *instanceData)
{
    CsrUint16 i;
    CsrCmnListElm_t *element, *nextElement;

    CsrDhcpServerLeasesSetReq *prim = (CsrDhcpServerLeasesSetReq *) instanceData->msg;
    CsrDhcpServerData *server = (CsrDhcpServerData *) prim->serverHandle;

    if (server->state != CSR_DHCP_SERVER_STATE_STOPPED)
    {
        CSR_LOG_TEXT_WARNING((CsrDhcpServerLto, 0, "Server is NOT stopped! (State: 0x%X)", server->state));
    }

    for (element = server->clients.first; element; element = nextElement)
    {
        CsrDhcpServerClient *client = (CsrDhcpServerClient *) element;
        CsrDhcpServerTimer *timer = CsrDhcpServerTimerFindFromClient(server, client);

        if (timer) /* Cancel the timer for the old message */
        {
            CsrDhcpServerTimerRemove(server, timer);
        }

        nextElement = element->next;
        CsrCmnListElementRemove(&server->clients, element);
    }

    for (i = 0; i < prim->leasesCount; i++)
    {
        CsrTimeUtc time;
        CsrDhcpServerClient *client = CSR_DHCP_SERVER_CLIENT_ADD(&server->clients);
        client->info->lease = prim->leases[i];

        CsrTimeUtcGet(&time, NULL, NULL);

        if (time.sec < client->info->lease.expiryTime.sec)
        {
            CsrUint32 sec = client->info->lease.expiryTime.sec - time.sec;

            CsrDhcpServerTimerAdd(server, client, sec, FALSE);
            client->info->ipAddressState = CSR_DHCP_SERVER_IP_ADDRESS_STATE_ACTIVE;
            client->info->leaseSet = sec + 1;
        }
        else
        {
            client->info->ipAddressState = CSR_DHCP_SERVER_IP_ADDRESS_STATE_INACTIVE;
            client->info->leaseSet = 0;
        }

        client->info->leaseSec = 0;
    }

    CsrPmemFree(prim->leases);
    CsrDhcpServerLeasesSetCfmSend(server->qid, server, CSR_RESULT_SUCCESS);
}

void CsrDhcpServerIpSocketUdpNewHandler(CsrDhcpServerInstanceData *instanceData)
{
    CsrIpSocketUdpNewCfm *prim = (CsrIpSocketUdpNewCfm *) instanceData->msg;
    CsrDhcpServerData *server = CSR_DHCP_SERVER_GET_FROM_STATE(&instanceData->servers, CSR_DHCP_SERVER_STATE_CREATING);

    if (server)
    {
        if (prim->result == CSR_RESULT_SUCCESS)
        {
            if (instanceData->broadcastSocketHandle != CSR_DHCP_SERVER_INVALID_SOCKET_HANDLE) /* Broadcast server already running */
            {
                server->socketHandle = prim->socketHandle;
            }
            else
            {
                instanceData->broadcastSocketHandle = prim->socketHandle;
            }

            CsrIpSocketOptionsReqSend(prim->socketHandle,
                0, 0, FALSE, FALSE, TRUE,
                CSR_IP_SOCKET_PRIORITY_DEFAULT,
                CSR_IP_SOCKET_OPTION_BROADCAST);
        }
        else
        {
            CSR_LOG_TEXT_ERROR((CsrDhcpServerLto, 0, "Create UDP socket failed (0x%04X)", prim->result));
            CsrDhcpServerCreateCfmSend(server->qid, NULL, CSR_RESULT_FAILURE);

            CSR_DHCP_SERVER_REMOVE(&instanceData->servers, server);
            csrDhcpServerCloseBroadcastSocket(instanceData);
        }
    }
    else
    {
        CSR_LOG_TEXT_ERROR((CsrDhcpServerLto, 0, "No servers in CSR_DHCP_SERVER_STATE_CREATING state"));

        if (prim->result == CSR_RESULT_SUCCESS)
        {
            CsrIpSocketUdpCloseReqSend(prim->socketHandle);

            if (prim->socketHandle == instanceData->broadcastSocketHandle)
            {
                instanceData->broadcastSocketHandle = CSR_DHCP_SERVER_INVALID_SOCKET_HANDLE;
            }
        }

        csrDhcpServerCloseBroadcastSocket(instanceData);
    }
}

void CsrDhcpServerIpSocketUdpBindHandler(CsrDhcpServerInstanceData *instanceData)
{
    CsrIpSocketUdpBindCfm *prim = (CsrIpSocketUdpBindCfm *) instanceData->msg;
    CsrDhcpServerData *server;

    if (prim->socketHandle == instanceData->broadcastSocketHandle)
    {
        if (prim->result == CSR_RESULT_SUCCESS)
        {
            CsrIpSocketUdpNewReqSend(CSR_DHCP_SERVER_IFACEQUEUE);
        }
        else
        {
            server = CSR_DHCP_SERVER_GET_FROM_STATE(&instanceData->servers, CSR_DHCP_SERVER_STATE_CREATING);

            if (server)
            {
                CSR_LOG_TEXT_ERROR((CsrDhcpServerLto, 0, "Bind broadcast UDP socket failed (0x%04X)", prim->result));
                CsrDhcpServerCreateCfmSend(server->qid, NULL, CSR_RESULT_FAILURE);
                CSR_DHCP_SERVER_REMOVE(&instanceData->servers, server);
            }
            else
            {
                CSR_LOG_TEXT_ERROR((CsrDhcpServerLto, 0, "No servers found from CSR_DHCP_SERVER_STATE_CREATING state"));
            }

            csrDhcpServerCloseBroadcastSocket(instanceData);
        }
    }
    else
    {
        server = CSR_DHCP_SERVER_GET_FROM_SOCKET_HANDLE(&instanceData->servers, prim->socketHandle);

        if (server)
        {
            if (prim->result == CSR_RESULT_SUCCESS)
            {
                server->state = CSR_DHCP_SERVER_STATE_STOPPED;

                CsrDhcpServerCreateCfmSend(server->qid, server, CSR_RESULT_SUCCESS);
            }
            else
            {
                CSR_LOG_TEXT_ERROR((CsrDhcpServerLto, 0, "Bind UDP socket failed (0x%04X)", prim->result));
                CsrIpSocketUdpCloseReqSend(prim->socketHandle);
                CsrDhcpServerCreateCfmSend(server->qid, NULL, CSR_RESULT_FAILURE);

                CSR_DHCP_SERVER_REMOVE(&instanceData->servers, server);
                csrDhcpServerCloseBroadcastSocket(instanceData);
            }
        }
        else
        {
            CSR_LOG_TEXT_ERROR((CsrDhcpServerLto, 0, "No servers found from socketHandle 0x%X", prim->socketHandle));

            CsrIpSocketUdpCloseReqSend(prim->socketHandle);
            csrDhcpServerCloseBroadcastSocket(instanceData);
        }
    }
}

static void csrDhcpServerMessageHandler(CsrDhcpServerInstanceData *instanceData,
    CsrDhcpServerData *server, CsrIpSocketUdpDataInd *prim)
{
    if (server->state == CSR_DHCP_SERVER_STATE_STARTED)
    {
        if (server->sendingData) /* Waiting for a CsrIpSocketUdpDataCfm */
        {
            CsrIpSocketUdpDataInd *msg = CsrPmemAlloc(sizeof(CsrIpSocketUdpDataInd) + prim->dataLength);

            CsrMemCpy(msg, prim, sizeof(CsrIpSocketUdpDataInd));

            msg->data = CsrPmemAlloc(prim->dataLength);
            CsrMemCpy(msg->data, prim->data, prim->dataLength);

            CSR_DHCP_SERVER_MESSAGE_QUEUE(&server->pendingMessages, msg);
        }
        else
        {
            CsrDhcpServerHandleMessage(instanceData, server, prim);
        }
    }
    else
    {
        /* Ignore */
    }
}

static void csrDhcpServerIterateServers(CsrCmnListElm_t *elem, void *data)
{
    CsrDhcpServerInstanceData *instanceData = (CsrDhcpServerInstanceData *) data;
    CsrDhcpServerData *server = (CsrDhcpServerData *) elem;
    CsrIpSocketUdpDataInd *prim = (CsrIpSocketUdpDataInd *) instanceData->msg;

    csrDhcpServerMessageHandler(instanceData, server, prim);
}

void CsrDhcpServerIpSocketUdpDataIndHandler(CsrDhcpServerInstanceData *instanceData)
{
    CsrIpSocketUdpDataInd *prim = (CsrIpSocketUdpDataInd *) instanceData->msg;

    if (prim->socketHandle == instanceData->broadcastSocketHandle)
    {
        CsrCmnListIterate(&instanceData->servers, csrDhcpServerIterateServers, instanceData);
    }
    else
    {
        CsrDhcpServerData *server = CSR_DHCP_SERVER_GET_FROM_SOCKET_HANDLE(&instanceData->servers, prim->socketHandle);

        if (server)
        {
            csrDhcpServerMessageHandler(instanceData, server, prim);
        }
        else
        {
            CSR_LOG_TEXT_ERROR((CsrDhcpServerLto, 0, "No servers found from socketHandle 0x%X", prim->socketHandle));
        }
    }

    CsrPmemFree(prim->data);
}

void CsrDhcpServerIpSocketOptionsHandler(CsrDhcpServerInstanceData *instanceData)
{
    CsrIpSocketOptionsCfm *prim = (CsrIpSocketOptionsCfm *) instanceData->msg;
    CsrDhcpServerData *server = CSR_DHCP_SERVER_GET_FROM_STATE(&instanceData->servers, CSR_DHCP_SERVER_STATE_CREATING);

    if (server)
    {
        CsrUint8 address[4];

        if (prim->socketHandle == instanceData->broadcastSocketHandle)
        {
            CsrUint8 addr[] = CSR_DHCP_SERVER_BROADCAST_IP_ADDRESS;
            CsrMemCpy(address, addr, 4);
        }
        else
        {
            CsrMemCpy(address, server->ipAddress, 4);
        }

        if ((prim->validOptions & CSR_IP_SOCKET_OPTION_BROADCAST) == CSR_IP_SOCKET_OPTION_BROADCAST)
        {
            if (prim->broadcast)
            {
                CsrIpSocketUdpBindReqSend(prim->socketHandle, address, CSR_DHCP_SERVER_PORT_SERVER);
            }
            else
            {
                CsrDhcpServerCreateCfmSend(server->qid, NULL, CSR_RESULT_FAILURE);

                CSR_DHCP_SERVER_REMOVE(&instanceData->servers, server);
                csrDhcpServerCloseBroadcastSocket(instanceData);
            }
        }
        else
        {
            CSR_LOG_TEXT_WARNING((CsrDhcpServerLto, 0,
                                  "Not able to set options for broadcast server, could be that IP stack does not support this"));
            CsrIpSocketUdpBindReqSend(prim->socketHandle, address, CSR_DHCP_SERVER_PORT_SERVER);
        }
    }
    else
    {
        CSR_LOG_TEXT_ERROR((CsrDhcpServerLto, 0, "No servers in CSR_DHCP_SERVER_STATE_CREATING state"));
        CsrIpSocketUdpCloseReqSend(prim->socketHandle);

        if (prim->socketHandle == instanceData->broadcastSocketHandle)
        {
            instanceData->broadcastSocketHandle = CSR_DHCP_SERVER_INVALID_SOCKET_HANDLE;
        }

        csrDhcpServerCloseBroadcastSocket(instanceData);
    }
}

void CsrDhcpServerIpSocketUdpDataHandler(CsrDhcpServerInstanceData *instanceData)
{
    CsrIpSocketUdpDataCfm *prim = (CsrIpSocketUdpDataCfm *) instanceData->msg;
    CsrDhcpServerData *server = CSR_DHCP_SERVER_GET_FROM_SOCKET_HANDLE(&instanceData->servers, prim->socketHandle);

    if (server)
    {
        server->sendingData = FALSE;

        if (server->state == CSR_DHCP_SERVER_STATE_STARTED)
        {
#ifdef CSR_DHCP_SERVER_USE_IFCONFIG_ARP
            if (instanceData->responseQueue != NULL)
            {
                CsrDhcpServerResponse *resp, *respPrev, *respNext;

                /* Get rid of any queued responses. */
                for (resp = instanceData->responseQueue, respPrev = NULL;
                     resp != NULL;
                     resp = respNext)
                {
                    respNext = resp->next;

                    if (resp->server == server)
                    {
                        if (respPrev == NULL)
                        {
                            instanceData->responseQueue = respNext;
                        }
                        else
                        {
                            respPrev->next = respNext;
                        }

                        CsrDhcpServerMessageSend(server,
                            resp->ip, resp->port,
                            resp->dataLength, resp->data);
                    }
                    else
                    {
                        respPrev = resp;
                    }
                }
            }

            /* If the server still isn't busy, handle incoming data. */
            if (server->sendingData)
#endif
            {
                CsrIpSocketUdpDataInd *msg = CSR_DHCP_SERVER_MESSAGE_DEQUEUE(&server->pendingMessages);

                if (msg)
                {
                    CsrDhcpServerHandleMessage(instanceData, server, msg);

                    CsrPmemFree(msg->data);
                    CsrPmemFree(msg);
                }
            }
        }
    }
    else
    {
        CSR_LOG_TEXT_ERROR((CsrDhcpServerLto, 0, "No servers found from socketHandle 0x%X", prim->socketHandle));
    }
}
