/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_types.h"
#include "csr_prim_defs.h"
#include "csr_log_text_2.h"

#include "csr_usr_config.h"

#include "csr_dhcp_server_prim.h"
#include "csr_dhcp_server_lib.h"
#include "csr_dhcp_server_sef.h"
#include "csr_dhcp_server_util.h"
#include "csr_dhcp_server_handler.h"
#include "csr_dhcp_server_task.h"

#include "csr_ip_socket_prim.h"
#include "csr_ip_socket_lib.h"

#ifdef CSR_DHCP_SERVER_USE_IFCONFIG_ARP
#include "csr_ip_ifconfig_prim.h"
#endif

#include "csr_sched.h"

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DEFINE(CsrDhcpServerLto);

typedef void (*CsrDhcpServerStateHandlerType)(CsrDhcpServerInstanceData *instanceData);
static const CsrDhcpServerStateHandlerType appStateHandlers[CSR_DHCP_SERVER_PRIM_DOWNSTREAM_COUNT] =
{
    CsrDhcpServerCreateHandler,      /* CSR_DHCP_SERVER_CREATE_REQ     */
    CsrDhcpServerDestroyHandler,     /* CSR_DHCP_SERVER_DESTROY_REQ    */
    CsrDhcpServerStartHandler,       /* CSR_DHCP_SERVER_START_REQ      */
    CsrDhcpServerStopHandler,        /* CSR_DHCP_SERVER_STOP_REQ       */
    CsrDhcpServerLeasesGetHandler,   /* CSR_DHCP_SERVER_LEASES_GET_REQ */
    CsrDhcpServerLeasesSetHandler,   /* CSR_DHCP_SERVER_LEASES_SET_REQ */
};

static void csrDhcpServerAddClient(CsrCmnListElm_t *elem)
{
    CsrDhcpServerClient *client = (CsrDhcpServerClient *) elem;
    client->info = (CsrDhcpServerClientInfo *) CsrPmemZalloc(sizeof(CsrDhcpServerClientInfo));

    /* Everything set to 0 through CsrPmemZalloc */
    client->info->ipAddressState = CSR_DHCP_SERVER_IP_ADDRESS_STATE_IDLE;
    client->info->clientId = NULL;
}

static void csrDhcpServerRemoveClient(CsrCmnListElm_t *elem)
{
    CsrDhcpServerClient *client = (CsrDhcpServerClient *) elem;

    CsrPmemFree(client->info->clientId);
    CsrPmemFree(client->info);
}

static void csrDhcpServerAdd(CsrCmnListElm_t *elem)
{
    CsrDhcpServerData *server = (CsrDhcpServerData *) elem;
    server->state = CSR_DHCP_SERVER_STATE_CREATING;
    server->socketHandle = CSR_DHCP_SERVER_INVALID_SOCKET_HANDLE;
    server->sendingData = FALSE;

    CsrDhcpServerFifoInit(&server->pendingMessages);
    CsrCmnListInit(&server->clients, 0, csrDhcpServerAddClient, csrDhcpServerRemoveClient);
    server->invalidIpList = NULL;
    server->tid = CsrSchedTimerSet(CSR_DHCP_SERVER_TIMER_DELAY, CsrDhcpServerTimerExpired, 0, server);
    server->timerList = NULL;
}

static void csrDhcpServerRemove(CsrCmnListElm_t *elem)
{
    CsrDhcpServerData *server = (CsrDhcpServerData *) elem;

    CsrCmnListDeinit(&server->clients);
    CSR_DHCP_SERVER_MESSAGE_DEQUEUE_ALL(&server->pendingMessages);
    CSR_DHCP_SERVER_INVALID_IP_REMOVE_ALL(server->invalidIpList);
    CSR_DHCP_SERVER_TIMER_REMOVE_ALL(server);

    CsrSchedTimerCancel(server->tid, NULL, NULL);
}

void CsrDhcpServerInit(void **gash)
{
    CsrDhcpServerInstanceData *instanceData;

    instanceData = (CsrDhcpServerInstanceData *) CsrPmemAlloc(sizeof(CsrDhcpServerInstanceData));
    instanceData->msg = NULL;
#ifdef CSR_DHCP_SERVER_USE_IFCONFIG_ARP
    instanceData->responseQueue = NULL;
#endif

    CsrCmnListInit(&instanceData->servers, 0, csrDhcpServerAdd, csrDhcpServerRemove);
    instanceData->broadcastSocketHandle = CSR_DHCP_SERVER_INVALID_SOCKET_HANDLE;
    CsrDhcpServerFifoInit(&instanceData->broadcastMessages);

    *gash = (void *) instanceData;

    CSR_LOG_TEXT_REGISTER(&CsrDhcpServerLto, "DHCP_SERVER", 0, NULL);
}

#ifdef CSR_DHCP_SERVER_USE_IFCONFIG_ARP
static CsrBool serverFind(CsrCmnListElm_t *elem, void *arg)
{
    CsrCmnListElm_t *server;

    server = arg;

    if (elem == server)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

#endif

void CsrDhcpServerHandler(void **gash)
{
    CsrDhcpServerInstanceData *instanceData;
    CsrUint16 event;
    void *message;

    instanceData = (CsrDhcpServerInstanceData *) *gash;

    if (CsrSchedMessageGet(&event, &message))
    {
        instanceData->msg = message;

        switch (event)
        {
            case CSR_DHCP_SERVER_PRIM:
            {
                CsrPrim *type = message;

                if ((*type < CSR_DHCP_SERVER_PRIM_DOWNSTREAM_COUNT) && ((appStateHandlers[*type]) != NULL))
                {
                    appStateHandlers[*type](instanceData);
                }
                else
                {
                    CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrDhcpServerLto, 0, event, *type);
                }

                break;
            }
            case CSR_IP_SOCKET_PRIM:
            {
                CsrPrim *type = message;

                switch (*type)
                {
                    case CSR_IP_SOCKET_UDP_NEW_CFM:
                    {
                        CsrDhcpServerIpSocketUdpNewHandler(instanceData);
                        break;
                    }
                    case CSR_IP_SOCKET_UDP_BIND_CFM:
                    {
                        CsrDhcpServerIpSocketUdpBindHandler(instanceData);
                        break;
                    }
                    case CSR_IP_SOCKET_UDP_DATA_IND:
                    {
                        CsrDhcpServerIpSocketUdpDataIndHandler(instanceData);
                        break;
                    }
                    case CSR_IP_SOCKET_UDP_DATA_CFM:
                    {
                        CsrDhcpServerIpSocketUdpDataHandler(instanceData);
                        break;
                    }
                    case CSR_IP_SOCKET_OPTIONS_CFM:
                    {
                        CsrDhcpServerIpSocketOptionsHandler(instanceData);
                        break;
                    }
                    default:
                    {
                        CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrDhcpServerLto, 0, event, *type);
                        break;
                    }
                }

                break;
            }
#ifdef CSR_DHCP_SERVER_USE_IFCONFIG_ARP
            case CSR_IP_IFCONFIG_PRIM:
            {
                CsrPrim *type = message;

                if (*type == CSR_IP_IFCONFIG_ARP_ENTRY_ADD_CFM)
                {
                    CsrDhcpServerResponse *msg;

                    msg = instanceData->responseQueue;

                    if ((msg != NULL) &&
                        (CsrCmnListSearch(&instanceData->servers,
                             serverFind, msg->server) != NULL))
                    {
                        CsrIpIfconfigArpEntryAddCfm *confirm = message;
                        CsrDhcpServerData *server;

                        server = msg->server;

                        if (confirm->result != CSR_RESULT_SUCCESS)
                        {
                            /*
                             * If the ARP trick failed, we'll
                             * just broadcast it.
                             */

                            CsrDhcpServerNetworkBroadcastAddressRetrieve(
                                server, msg->ip);
                        }

                        if (server->sendingData == FALSE)
                        {
                            CsrIpSocketUdpDataReqSend((CsrUint16) msg->server->socketHandle,
                                msg->ip, msg->port, msg->dataLength, msg->data);

                            instanceData->responseQueue =
                                instanceData->responseQueue->next;
                            CsrPmemFree(msg);
                        }
                        /* Otherwise the DHCP offer will be sent later. */
                    }
                }
                else
                {
                    CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrDhcpServerLto, 0,
                        CSR_IP_IFCONFIG_PRIM, *type);
                }

                break;
            }
#endif
            default:
            {
                CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrDhcpServerLto, 0, event, 0);
                break;
            }
        }

        /* Free the message itself */
        if (instanceData->msg)
        {
            CsrPmemFree(instanceData->msg);
            instanceData->msg = NULL;
        }
    }
    else
    {
        /* Failed to retrieve message from queue */
    }
}

#ifdef ENABLE_SHUTDOWN
void CsrDhcpServerDeinit(void **gash)
{
    CsrUint16 event;
    void *message;
    CsrDhcpServerInstanceData *instanceData;
#ifdef CSR_DHCP_SERVER_USE_IFCONFIG_ARP
    CsrDhcpServerResponse *resp, *respNext;
#endif

    instanceData = (CsrDhcpServerInstanceData *) *gash;

    /* Clean queue */
    while (CsrSchedMessageGet(&event, &message))
    {
        switch (event)
        {
            case CSR_DHCP_SERVER_PRIM:
            {
                CsrDhcpServerFreeDownstreamMessageContents(event, message);
                break;
            }

            case CSR_IP_SOCKET_PRIM:
            {
                CsrIpSocketFreeUpstreamMessageContents(event, message);
                break;
            }

            default:
            {
                break;
            }
        }

        CsrPmemFree(message);
    }

#ifdef CSR_DHCP_SERVER_USE_IFCONFIG_ARP
    for (resp = instanceData->responseQueue;
         resp != NULL;
         resp = respNext)
    {
        respNext = resp->next;

        CsrPmemFree(resp->data);
        CsrPmemFree(resp);
    }
#endif

    CSR_DHCP_SERVER_MESSAGE_DEQUEUE_ALL(&instanceData->broadcastMessages);
    CsrCmnListDeinit(&instanceData->servers);

    /* Deallocate instance data */
    CsrPmemFree(instanceData);
}

#endif /* ENABLE_SHUTDOWN */

/* Util functions */
void CsrDhcpServerFifoInit(CsrDhcpServerFifo *fifo)
{
    fifo->front = NULL;
    fifo->back = NULL;
}

void CsrDhcpServerFifoQueue(CsrDhcpServerFifo *fifo, void *data)
{
    CsrDhcpServerFifoElement *elem = (CsrDhcpServerFifoElement *) CsrPmemAlloc(sizeof(CsrDhcpServerFifoElement));

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

void *CsrDhcpServerFifoDequeue(CsrDhcpServerFifo *fifo)
{
    void *result = NULL;

    if (fifo->front)
    {
        CsrDhcpServerFifoElement *tmp;

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

void CsrDhcpServerListAdd(CsrDhcpServerList **list, void *data)
{
    CsrDhcpServerList *elem = (CsrDhcpServerList *) CsrPmemAlloc(sizeof(CsrDhcpServerList));

    elem->next = NULL;
    elem->data = data;

    if (*list == NULL) /* The list is empty */
    {
        *list = elem;
    }
    else
    {
        CsrDhcpServerList *last = *list;

        while (last->next)
        {
            last = last->next;
        }

        last->next = elem;
    }
}

void CsrDhcpServerTimerAdd(CsrDhcpServerData *server, CsrDhcpServerClient *client, CsrUint32 seconds, CsrBool removeClient)
{
    CsrDhcpServerTimer *elem = (CsrDhcpServerTimer *) CsrPmemAlloc(sizeof(CsrDhcpServerTimer));
    CsrTimeUtc time;
    CsrUint32 sec;

    CsrTimeUtcGet(&time, NULL, NULL);

    sec = time.sec + seconds + 1;

    elem->next = NULL;
    elem->prev = NULL;
    elem->client = client;
    elem->sec = sec;
    elem->removeClient = removeClient;

    if (server->timerList == NULL) /* The list is empty */
    {
        server->timerList = elem;
    }
    else
    {
        CsrDhcpServerTimer *el = server->timerList;

        while (el->next)
        {
            if (sec < el->sec)
            {
                break;
            }

            el = el->next;
        }

        if (sec < el->sec) /* Put elem in before el */
        {
            if (el->prev)
            {
                el->prev->next = elem;
                elem->prev = el->prev;
            }
            else /* First element in the list */
            {
                server->timerList = elem;
            }

            elem->next = el;
            el->prev = elem;
        }
        else /* Put elem in after el */
        {
            if (el->next)
            {
                el->next->prev = elem;
                elem->next = el->next;
            }

            el->next = elem;
            elem->prev = el;
        }
    }
}

void CsrDhcpServerTimerRemove(CsrDhcpServerData *server, CsrDhcpServerTimer *timer)
{
    if (timer->prev && timer->next) /* In the middle of the list */
    {
        timer->prev->next = timer->next;
        timer->next->prev = timer->prev;
    }
    else if (timer->prev) /* The last element in the list */
    {
        timer->prev->next = NULL;
    }
    else if (timer->next) /* The first element in the list */
    {
        server->timerList = timer->next;
        timer->next->prev = NULL;
    }
    else /* The only element in the list */
    {
        server->timerList = NULL;
    }

    CsrPmemFree(timer);
}

CsrDhcpServerTimer *CsrDhcpServerTimerFindFromClient(CsrDhcpServerData *server, CsrDhcpServerClient *client)
{
    CsrDhcpServerTimer *timer = server->timerList;

    while (timer)
    {
        if (timer->client == client)
        {
            return timer;
        }

        timer = timer->next;
    }

    return NULL;
}
