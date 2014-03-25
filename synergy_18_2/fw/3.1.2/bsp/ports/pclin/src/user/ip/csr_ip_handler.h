#ifndef CSR_IP_HANDLER_H__
#define CSR_IP_HANDLER_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <sys/select.h>

#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "csr_types.h"
#include "csr_message_queue.h"
#include "csr_log_text_2.h"

#include "csr_ip_ether_prim.h"
#include "csr_ip_ifconfig_prim.h"
#include "csr_ip_socket_prim.h"

#ifdef CSR_IP_SUPPORT_TLS
#include "csr_ip_tls_handler.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DECLARE(CsrIpLto);

#define CSR_IP_SOCKET_STRINGIFY_REAL(a) #a
#define CSR_IP_SOCKET_STRINGIFY(a) CSR_IP_SOCKET_STRINGIFY_REAL(a)

#define CSR_IP_SOCKET_ASSERT(cond, str)              \
    do {                                                \
        if (!(cond))                                    \
        {                                               \
            char *panic_arg = "[" __FILE__ ":" CSR_IP_SOCKET_STRINGIFY(__LINE__) "] - " CSR_IP_SOCKET_STRINGIFY(cond); \
            char *panic_str = "[" __FILE__ ":" CSR_IP_SOCKET_STRINGIFY(__LINE__) "] - " str; \
            CsrSize panic_len = CsrStrLen(panic_arg) + CsrStrLen(panic_str) + 3;        \
            char *panic = CsrPmemZalloc(panic_len);                                       \
            CsrStrCat(panic, panic_arg);                                                \
            CsrStrCat(panic, "\n");                                                     \
            CsrStrCat(panic, panic_str);                                                \
            CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_ASSERTION_FAIL, panic);                  \
            CsrPmemFree(panic);                                                            \
        }                                                                               \
    } while (0)

#define CSR_IP_SOCKET_ASSERT_NOT_REACHED() CSR_IP_SOCKET_ASSERT(FALSE, "Unreachable")

struct CsrIpInstanceData;
typedef struct CsrIpInstanceData CsrIpInstanceData;

#define MAX_ETHERS  8
typedef struct
{
    CsrSchedQid        qid;
    CsrIpInstanceData *inst;
    int                fd;
    int                reap;

    CsrUint8                  mac[6];
    CsrIpEtherIfType          ifType;
    CsrIpEtherIfContext       ifContext;
    CsrUint16                 maxTxUnit;
    CsrIpEtherEncapsulation   encapsulation;
    CsrIpEtherFrameTxFunction frameTxFunction;
} CsrIpEtherInst;


#define MAX_SOCKETS 100

#define CSR_IP_SOCKET_TYPE_TCP_LISTEN  0x0000
#define CSR_IP_SOCKET_TYPE_TCP_CONNECT 0x0001
#define CSR_IP_SOCKET_TYPE_TCP_DATA    0x0002
#define CSR_IP_SOCKET_TYPE_UDP         0x0003
#define CSR_IP_SOCKET_TYPE_RAW         0x0004
#define CSR_IP_SOCKET_TYPE_MAX         CSR_IP_SOCKET_TYPE_RAW
#define CSR_IP_SOCKET_TYPE_COUNT       (CSR_IP_SOCKET_TYPE_MAX + 1)

#define RX_BUFFER_SIZE 65535

typedef struct csrIpRxElement
{
    struct csrIpRxElement *next;
    CsrUint8              *buf;
    CsrUint16              bufLen;
} csrIpRxElement;

typedef struct
{
    CsrSchedQid             queue;
    CsrUint16               handle;
    int                     socket;
    CsrIpSocketFamily       family;
    CsrUint16               type;
    CsrBool                 dead;
    CsrBool                 reap;
    CsrUint8               *data;
    CsrUint16               dataOffset;
    CsrUint16               dataLen;
    csrIpRxElement         *rxQueue;
    struct sockaddr_storage sockAddr;
} CsrIpSocketInst;

struct netdev_t
{
    struct netdev_t       *next;
    char                  *ifName;
    pid_t                  dhcpPid;
    CsrBool                natEnabled;
    CsrIpIfconfigIfDetails ifd;
};

typedef struct subqueue_t
{
    struct subqueue_t *next;
    CsrSchedQid        qid;
} CsrIpIfconfigSubscriber;

struct CsrIpInstanceData
{
    CsrIpSocketInst *socketInstance[MAX_SOCKETS];
    CsrIpEtherInst  *etherInstance[MAX_ETHERS];
#ifdef CSR_IP_SUPPORT_TLS
    CsrIpTlsInstanceData tlsInstance;
#endif
    CsrInt32                 maxSocketHandle;
    pthread_mutex_t          instLock;
    pthread_cond_t           dnsEvent;
    CsrUint8                *rxBuffer;
    void                    *msg;
    int                      maxfd;
    fd_set                   rsetAll;
    fd_set                   wsetAll;
    int                      controlPipe[2];
    int                      rtSocketListen;
    int                      ipSocketQuery;
    pthread_t                tid;
    CsrIpIfconfigSubscriber *subscribers;
    struct netdev_t         *netdevs;
    int                      netdevsCount;
    CsrUint32                dnsThreadCount;
    CsrMessageQueueType     *ifconfigRequests;
};

typedef struct
{
    CsrIpInstanceData            *inst;
    CsrIpSocketDnsResolveNameReq *msg;
} csrIpSocketDnsLookupData;

#ifdef __cplusplus
}
#endif

#endif
