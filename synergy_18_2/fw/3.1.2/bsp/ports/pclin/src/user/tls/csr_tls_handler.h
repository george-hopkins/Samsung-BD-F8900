#ifndef CSR_TLS_HANDLER_H__
#define CSR_TLS_HANDLER_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
#include <sys/select.h>

#include <openssl/ssl.h>
#include <openssl/err.h>

#include "csr_types.h"
#include "csr_framework_ext.h"
#include "csr_sched.h"

#include "csr_tls_prim.h"
#include "csr_ip_socket_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

struct csrTlsSession;
struct csrTlsConnection;
struct CsrTlsInstanceData;
struct CsrTlsThreadInstanceData;

typedef struct csrTlsSession csrTlsSession;
typedef struct csrTlsConnection csrTlsConnection;
typedef struct CsrTlsInstanceData CsrTlsInstanceData;
typedef struct CsrTlsThreadInstanceData CsrTlsThreadInstanceData;

#ifndef CSR_TLS_PSK_CLIENT_IDENTITY
#define CSR_TLS_PSK_CLIENT_IDENTITY "Client_identity"
#endif

#define CSR_TLS_STRINGIFY_REAL(a) #a
#define CSR_TLS_STRINGIFY(a) CSR_TLS_STRINGIFY_REAL(a)

#define CSR_TLS_ASSERT(cond, str)              \
    do {                                                \
        if (!(cond))                                    \
        {                                               \
            char *panic_arg = "[" __FILE__ ":" CSR_TLS_STRINGIFY(__LINE__) "] - " CSR_TLS_STRINGIFY(cond); \
            char *panic_str = "[" __FILE__ ":" CSR_TLS_STRINGIFY(__LINE__) "] - " str; \
            CsrSize panic_len = CsrStrLen(panic_arg) + CsrStrLen(panic_str) + 3;        \
            char *panic = CsrPmemZalloc(panic_len);                                       \
            CsrStrCat(panic, panic_arg);                                                \
            CsrStrCat(panic, "\n");                                                     \
            CsrStrCat(panic, panic_str);                                                \
            CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_ASSERTION_FAIL, panic);                  \
            CsrPmemFree(panic);                                                            \
        }                                                                               \
    } while (0)

#define CSR_TLS_ASSERT_NOT_REACHED() CSR_TLS_ASSERT(FALSE, "Unreachable")

#define MAX_SOCKETS 100

#define RX_BUFFER_SIZE 65535

typedef struct csrTlsRxElement
{
    struct csrTlsRxElement *next;
    CsrUint8               *buf;
    CsrUint16               bufLen;
} csrTlsRxElement;

typedef struct
{
    CsrUint16          handle;
    CsrIpSocketFamily  family;
    int                socket;
    CsrUint16          type;
    CsrBool            dead;
    CsrBool            reap;
    CsrUint8          *data;
    CsrUint32          dataOffset;
    CsrUint32          dataLen;
    csrTlsRxElement   *rxQueue;
    struct sockaddr_in sockAddr;
    socklen_t          sockLen;

    CsrTlsThreadInstanceData *threadInstanceData;
    csrTlsConnection         *conn;
    SSL                      *ssl;
    CsrBool                   readBlockedOnWrite;
    CsrBool                   writeBlockedOnRead;

    CsrEventHandle callbackEvent;
    CsrBool        acceptCert;
    CsrUint8      *pwd;
    CsrSize        pwdLen;
} CsrTlsSocketInst;

struct CsrTlsThreadInstanceData
{
    CsrTlsSocketInst *socketInstance[MAX_SOCKETS];
    CsrInt32          maxSocketHandle;
    pthread_mutex_t   instLock;
    CsrUint8         *rxBuffer;
    CsrUint16         rxBufferSize;
    int               maxfd;
    fd_set            rsetAll;
    fd_set            wsetAll;
    int               closeThreadPipe[2];
    int               addSocketPipe[2];
    pthread_t         tid;

    CsrTlsInstanceData *taskInstanceData;
};

struct csrTlsSession
{
    CsrSchedQid qid;

    struct
    {
        CsrUint16 keybitsMin;
        CsrUint16 keybitsMax;
    } cipher[3];

    CsrUint8          connections;
    csrTlsConnection *connList;
    CsrUint8         *pskKey;
    CsrSize           pskKeyLength;

    /* OpenSSL CTX configuration*/
    SSL_CTX *ctx;

    csrTlsSession *next;
};

struct csrTlsConnection
{
    csrTlsSession    *session;
    CsrUint16         socketHandle;
    csrTlsConnection *next;
};

struct CsrTlsInstanceData
{
    void       *msg;
    CsrSchedQid qid;

    CsrUint8       sessions;
    csrTlsSession *sessionList;

    CsrTlsThreadInstanceData threadInstanceData;
};

typedef void (*CsrTlsEventHandler)(CsrTlsInstanceData *);

extern CsrTlsInstanceData *globalCsrTlsInstanceData;

#ifdef __cplusplus
}
#endif

#endif
