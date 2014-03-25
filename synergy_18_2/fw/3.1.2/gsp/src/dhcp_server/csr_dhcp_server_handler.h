#ifndef CSR_DHCP_SERVER_HANDLER_H__
#define CSR_DHCP_SERVER_HANDLER_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_list.h"
#include "csr_log_text_2.h"
#include "csr_dhcp_server_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DECLARE(CsrDhcpServerLto);

#define CSR_DHCP_SERVER_STATE_CREATING              (0)
#define CSR_DHCP_SERVER_STATE_STOPPED               (1)
#define CSR_DHCP_SERVER_STATE_STARTED               (2)
#define CSR_DHCP_SERVER_STATE_DESTROYING            (3)

#define CSR_DHCP_SERVER_PORT_SERVER                 (67)
#define CSR_DHCP_SERVER_PORT_CLIENT                 (68)
#define CSR_DHCP_SERVER_BROADCAST_IP_ADDRESS        {255, 255, 255, 255}

#define CSR_DHCP_SERVER_INVALID_SOCKET_HANDLE       (-1)

#define CSR_DHCP_SERVER_MAX_MSG_SIZE                (576)

#define CSR_DHCP_SERVER_IP_ADDRESS_STATE_IDLE       (0)
#define CSR_DHCP_SERVER_IP_ADDRESS_STATE_ACTIVE     (1)
#define CSR_DHCP_SERVER_IP_ADDRESS_STATE_INACTIVE   (2)
#define CSR_DHCP_SERVER_IP_ADDRESS_STATE_OFFERED    (3)

#define CSR_DHCP_SERVER_TIMER_DELAY                 (5 * CSR_SCHED_SECOND)

typedef struct CsrDhcpServerFifoElement
{
    struct CsrDhcpServerFifoElement *next;
    void                            *data;
} CsrDhcpServerFifoElement;

typedef struct CsrDhcpServerFifo
{
    CsrDhcpServerFifoElement *front;
    CsrDhcpServerFifoElement *back;
} CsrDhcpServerFifo;

typedef struct
{
    CsrUint8 ipAddressState;

    CsrUint32 requestedLeaseTime;              /* Received from client            */
    CsrUint8  clientIdLen;                     /* Received from client            */
    CsrUint8 *clientId;                        /* Received from client            */
    CsrUint8  requestedIpAddress[4];           /* Received from client            */
    CsrUint8  serverId[4];                     /* Received from client            */

    CsrDhcpServerLease lease;                  /* The lease given to the client   */
    CsrUint32          leaseSec;               /* The lease given in seconds      */
    CsrUint32          leaseSet;               /* The lease set in lease set req  */
} CsrDhcpServerClientInfo;

typedef struct CsrDhcpServerClient
{
    struct CsrDhcpServerClient *next;              /* Do not move - required by linked-list library */
    struct CsrDhcpServerClient *prev;              /* Do not move - required by linked-list library */

    CsrDhcpServerClientInfo *info;
} CsrDhcpServerClient;

typedef struct CsrDhcpServerList
{
    struct CsrDhcpServerList *next;
    void                     *data;
} CsrDhcpServerList;

typedef struct CsrDhcpServerTimer
{
    struct CsrDhcpServerTimer *next;
    struct CsrDhcpServerTimer *prev;

    CsrDhcpServerClient *client;
    CsrUint32            sec;
    CsrBool              removeClient;
} CsrDhcpServerTimer;

typedef struct CsrDhcpServerData
{
    struct CsrDhcpServerData *next;              /* Do not move - required by linked-list library */
    struct CsrDhcpServerData *prev;              /* Do not move - required by linked-list library */

    CsrUint8            state;
    CsrInt32            socketHandle;
    CsrBool             sendingData;
    CsrDhcpServerFifo   pendingMessages;
    CsrCmnList_t        clients;
    CsrDhcpServerList  *invalidIpList;
    CsrSchedTid         tid;
    CsrDhcpServerTimer *timerList;

    /* Data received from app */
    CsrSchedQid qid;
    CsrUint8    ipAddress[4];
    CsrUint8    network[4];
    CsrUint8    networkMask[4];
    CsrUint8    gateway[4];
    CsrUint8    dns1[4];
    CsrUint8    dns2[4];
    CsrUint8    leaseTime;
} CsrDhcpServerData;

#ifdef CSR_DHCP_SERVER_USE_IFCONFIG_ARP
typedef struct CsrDhcpServerResponse
{
    CsrDhcpServerData            *server;
    CsrUint8                     *data;
    CsrUint16                     dataLength;
    CsrUint8                      ip[4];
    CsrUint16                     port;
    struct CsrDhcpServerResponse *next;
} CsrDhcpServerResponse;
#endif

typedef struct
{
    void        *msg;
    CsrCmnList_t servers;
#ifdef CSR_DHCP_SERVER_USE_IFCONFIG_ARP
    CsrDhcpServerResponse *responseQueue;
#endif
    CsrInt32          broadcastSocketHandle;
    CsrDhcpServerFifo broadcastMessages;
} CsrDhcpServerInstanceData;

extern void CsrDhcpServerCreateCfmSend(CsrSchedQid queue, CsrDhcpServerHandle serverHandle, CsrResult result);
extern void CsrDhcpServerDestroyCfmSend(CsrSchedQid queue, CsrDhcpServerHandle serverHandle, CsrResult result);
extern void CsrDhcpServerStartCfmSend(CsrSchedQid queue, CsrDhcpServerHandle serverHandle, CsrResult result);
extern void CsrDhcpServerStopCfmSend(CsrSchedQid queue, CsrDhcpServerHandle serverHandle, CsrResult result);
extern void CsrDhcpServerLeasesGetCfmSend(CsrSchedQid queue, CsrDhcpServerHandle serverHandle, CsrUint16 leasesCount, CsrDhcpServerLease *leases);
extern void CsrDhcpServerLeasesSetCfmSend(CsrSchedQid queue, CsrDhcpServerHandle serverHandle, CsrResult result);
extern void CsrDhcpServerRogueIpAddressIndSend(CsrSchedQid queue, CsrDhcpServerHandle serverHandle, CsrUint8 *ip);
extern void CsrDhcpServerFreeDownstreamMessageContents(CsrUint16 eventClass, void *message);

extern void CsrDhcpServerFifoInit(CsrDhcpServerFifo *fifo);
extern void CsrDhcpServerFifoQueue(CsrDhcpServerFifo *fifo, void *data);
extern void *CsrDhcpServerFifoDequeue(CsrDhcpServerFifo *fifo);

extern void CsrDhcpServerListAdd(CsrDhcpServerList **list, void *data);

extern void CsrDhcpServerTimerAdd(CsrDhcpServerData *server, CsrDhcpServerClient *client, CsrUint32 seconds, CsrBool removeClient);
extern void CsrDhcpServerTimerRemove(CsrDhcpServerData *server, CsrDhcpServerTimer *timer);
extern CsrDhcpServerTimer *CsrDhcpServerTimerFindFromClient(CsrDhcpServerData *server, CsrDhcpServerClient *client);

#define CSR_DHCP_SERVER_ADD(listPtr)                            ((CsrDhcpServerData *) CsrCmnListElementAddFirst((listPtr), sizeof(CsrDhcpServerData)))
#define CSR_DHCP_SERVER_REMOVE(listPtr, elemPtr)                (CsrCmnListElementRemove((listPtr), (CsrCmnListElm_t *) (elemPtr)))
#define CSR_DHCP_SERVER_EXISTS(listPtr)                         (CsrCmnListElementGetFirst((listPtr)) == NULL ? FALSE : TRUE)
#define CSR_DHCP_SERVER_GET_FROM_STATE(listPtr, sta)            ((CsrDhcpServerData *) CsrCmnListSearchOffsetUint8((listPtr), CsrOffsetOf(CsrDhcpServerData, state), sta))
#define CSR_DHCP_SERVER_GET_FROM_SOCKET_HANDLE(listPtr, handle) ((CsrDhcpServerData *) CsrCmnListSearchOffsetUint16((listPtr), CsrOffsetOf(CsrDhcpServerData, socketHandle), handle))

#define CSR_DHCP_SERVER_CLIENT_ADD(listPtr)                     ((CsrDhcpServerClient *) CsrCmnListElementAddFirst((listPtr), sizeof(CsrDhcpServerClient)))
#define CSR_DHCP_SERVER_CLIENT_REMOVE(listPtr, elemPtr)         (CsrCmnListElementRemove((listPtr), (CsrCmnListElm_t *) (elemPtr)))

#define CSR_DHCP_SERVER_MESSAGE_QUEUE(fifo, data)               (CsrDhcpServerFifoQueue(fifo, data))
#define CSR_DHCP_SERVER_MESSAGE_DEQUEUE(fifo)                   ((CsrIpSocketUdpDataInd *) CsrDhcpServerFifoDequeue(fifo))
#define CSR_DHCP_SERVER_MESSAGE_DEQUEUE_ALL(fifo)                       \
    do {                                                                \
        CsrIpSocketUdpDataInd *msg_;                                    \
        while ((msg_ = CSR_DHCP_SERVER_MESSAGE_DEQUEUE(fifo)) != NULL)  \
                {                                                               \
                    CsrPmemFree(msg_->data);                                    \
                    CsrPmemFree(msg_);                                          \
                }                                                               \
                } while (0)

#define CSR_DHCP_SERVER_INVALID_IP_ADD(listPtr, ip)                     \
    do {                                                                \
        CsrUint8 *ip_ = CsrPmemAlloc(sizeof(CsrUint8) * 4);             \
        CsrMemCpy(ip_, ip, 4);                                          \
        CsrDhcpServerListAdd(&listPtr, ip_);                            \
    } while (0)

#define CSR_DHCP_SERVER_INVALID_IP_REMOVE_ALL(list)                     \
    do {                                                                \
        CsrDhcpServerList *next;                                        \
        while (list)                                                    \
        {                                                               \
            next = list->next;                                          \
            CsrPmemFree(list->data);                                    \
            CsrPmemFree(list);                                          \
            list = next;                                                \
        }                                                               \
    } while (0)

#define CSR_DHCP_SERVER_TIMER_REMOVE_ALL(server)                        \
    do {                                                                \
        CsrDhcpServerTimer *timer_ = server->timerList;                 \
        while (timer_)                                                  \
        {                                                               \
            CsrDhcpServerTimer *next_ = timer_->next;                   \
            CsrPmemFree(timer_);                                        \
            timer_ = next_;                                             \
        }                                                               \
    } while (0)

#ifdef __cplusplus
}
#endif

#endif
