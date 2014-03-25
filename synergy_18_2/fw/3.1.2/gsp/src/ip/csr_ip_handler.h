#ifndef CSR_IP_HANDLER_H__
#define CSR_IP_HANDLER_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_ip_ifconfig_prim.h"
#include "csr_ip_ether_prim.h"
#include "csr_ip_socket_prim.h"
#include "csr_log_text_2.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CSR_IP_MAX_SOCKETS 15
#define CSR_IP_MAX_INTERFACES 4

#define CSR_IP_SOCKET_TYPE_RAW       0x0000
#define CSR_IP_SOCKET_TYPE_TCP       0x0001
#define CSR_IP_SOCKET_TYPE_TCPLISTEN 0x0002
#define CSR_IP_SOCKET_TYPE_UDP       0x0003

#define CSR_IP_SOCKET_STATE_OPEN    0
#define CSR_IP_SOCKET_STATE_CLOSING 1

struct CsrIpInstanceData;
typedef struct CsrIpInstanceData CsrIpInstanceData;

typedef struct
{
    CsrSchedQid            queue;
    CsrUint16              socketHandle;
    CsrSchedTid            timer;
    CsrUint16              state;
    CsrIpSocketTcpDataReq *currentMsg;
    CsrUint16              currentMsgIndex;
    CsrIpInstanceData     *instanceData;
    union
    {
        struct ip_pcb  *ip;
        struct tcp_pcb *tcp;
        struct udp_pcb *udp;
        struct raw_pcb *raw;
    } pcb;
    CsrUint16    socketType;
    CsrBool      connectReq;
    CsrUint16    pendingDataResSize;
    struct pbuf *rxQueue;
} CsrIpSocketInstanceData;

typedef struct CsrIpEtherInstanceData
{
    CsrSchedQid               queue;
    CsrIpEtherIfContext       ifContext;
    CsrIpEtherFrameTxFunction frameTxFunction;
    CsrIpInstanceData        *instanceData;
    CsrUint32                 ifHandle;
    CsrIpIfconfigIfType       ifType;
    CsrIpIfconfigIfState      ifState;
} CsrIpEtherInstanceData;

typedef struct CsrIpIfconfigListener
{
    CsrUint16                     queue;
    struct CsrIpIfconfigListener *next;
} CsrIpIfconfigListener;

struct CsrIpInstanceData
{
    CsrIpSocketInstanceData *socketInstance[CSR_IP_MAX_SOCKETS];
    struct netif            *interfaces[CSR_IP_MAX_INTERFACES];
    struct netif            *defaultInterface;
    CsrSchedTid              ethArpTimerId;
    CsrSchedTid              coarseTimerId;
    CsrSchedTid              ipTickTimerId;
    CsrIpIfconfigListener   *ifconfigListeners;
    void                    *msg;
};

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DECLARE(CsrIpLto);

/* Log Text Sub Origins */
#define CSR_IP_LTSO_ETHER       1
#define CSR_IP_LTSO_IFCONFIG    2
#define CSR_IP_LTSO_SOCKET      3

#ifdef __cplusplus
}
#endif

#endif
