#ifndef CSR_IP_HANDLER_H__
#define CSR_IP_HANDLER_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <winsock2.h>
#include <windows.h>

#include "csr_types.h"
#include "csr_list.h"
#include "csr_ip_socket_prim.h"
#include "csr_log_text_2.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Log Text */
CSR_LOG_TEXT_HANDLE_DECLARE(CsrIpLto);

/* --------------------------------- */
/*          Asserts                 */
/* --------------------------------- */
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

#define CSR_IP_SOCKET_INST_STATE_FREE       0
#define CSR_IP_SOCKET_INST_STATE_NEW        1
#define CSR_IP_SOCKET_INST_STATE_BIND       2
#define CSR_IP_SOCKET_INST_STATE_LISTEN     3
#define CSR_IP_SOCKET_INST_STATE_ACCEPT     4
#define CSR_IP_SOCKET_INST_STATE_ACCEPTED   5
#define CSR_IP_SOCKET_INST_STATE_ACTIVE     6
#define CSR_IP_SOCKET_INST_STATE_ABORT      7
#define CSR_IP_SOCKET_INST_STATE_CLOSE      8
#define CSR_IP_SOCKET_INST_STATE_CONNECT    9
#define CSR_IP_SOCKET_INST_STATE_DEAD       10

#define CSR_IP_SOCKET_INST_TYPE_RAW         0
#define CSR_IP_SOCKET_INST_TYPE_UDP         1
#define CSR_IP_SOCKET_INST_TYPE_TCP         2

#define CSR_IP_SOCKET_EVENT_TYPE_SOCKET      0
#define CSR_IP_SOCKET_EVENT_TYPE_RESCHEDULE  1
#define CSR_IP_SOCKET_NUMBER_OF_EVENT_TYPES  2

#define CSR_IP_SOCKET_MAX_SOCKETS (WSA_MAXIMUM_WAIT_EVENTS / CSR_IP_SOCKET_NUMBER_OF_EVENT_TYPES)

#define CSR_IP_IFCONFIG_MAX_SUBSCRIBERS 100

typedef struct csrIpRxElement
{
    struct csrIpRxElement *next;
    CsrUint8              *buf;
    CsrUint16              bufLen;
} csrIpRxElement;

typedef struct
{
    CsrUint32         state;
    CsrSchedQid       appHandle;
    CsrIpSocketFamily socketFamily;
    CsrUint8          socketType;
    CsrUint16         appSocketHandle;
    SOCKET            platformSocketHandle;
    /* Maximum UDP packet size */
    CsrUint16 maxUdpMsgSize;

    CsrUint16       dataLength;
    CsrUint16       dataOffset;
    CsrUint8       *data;
    csrIpRxElement *rxQueue;
    /* Specify destination adress/port */
    CsrUint8  ipAddress[16];
    CsrUint16 port;
} CsrIpSocketInstanceData;

typedef struct
{
    void     *msg;
    int       bufferLength;
    CsrUint8 *buffer;

    HANDLE threadHandle;
    HANDLE instMutex;

    CsrIpSocketInstanceData socketInsts[CSR_IP_SOCKET_MAX_SOCKETS];

    WSAEVENT events[CSR_IP_SOCKET_MAX_SOCKETS * CSR_IP_SOCKET_NUMBER_OF_EVENT_TYPES];
} CsrIpInstanceData;

#ifdef __cplusplus
}
#endif

#endif
