#ifndef CSR_TLS_LIB_H__
#define CSR_TLS_LIB_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_sched.h"

#include "csr_tls_prim.h"
#include "csr_msg_transport.h"

#ifdef CSR_IP_SUPPORT_TLS
#include "csr_ip_task.h"
#define CSR_TLS_IFACEQUEUE CSR_IP_IFACEQUEUE
#else
#include "csr_tls_task.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------------------------------------------
   Name
       CSR_TLS_SESSION_CREATE_REQ

   Description
        Creates a new TLS session

 * -------------------------------------------------------------------- */
CsrTlsSessionCreateReq *CsrTlsSessionCreateReq_struct(
    CsrSchedQid qid);
#define CsrTlsSessionCreateReqSend2(_queueId) { \
        CsrTlsSessionCreateReq *__msg; \
        __msg = CsrTlsSessionCreateReq_struct(_queueId); \
        CsrMsgTransport(CSR_TLS_IFACEQUEUE, CSR_TLS_PRIM, __msg); \
}
#define CsrTlsSessionCreateReqSend CsrTlsSessionCreateReqSend2

CsrTlsSessionDestroyReq *CsrTlsSessionDestroyReq_struct(
    CsrTlsSession session);
#define CsrTlsSessionDestroyReqSend2(_session) { \
        CsrTlsSessionDestroyReq *__msg; \
        __msg = CsrTlsSessionDestroyReq_struct(_session); \
        CsrMsgTransport(CSR_TLS_IFACEQUEUE, CSR_TLS_PRIM, __msg); \
}
#define CsrTlsSessionDestroyReqSend CsrTlsSessionDestroyReqSend2

CsrTlsConfigCipherReq *CsrTlsConfigCipherReq_struct(
    CsrTlsSession session,
    CsrUint16 *cipherSuite,
    CsrUint16 cipherSuiteLength,
    CsrUint16 keybitsMin,
    CsrUint16 keybitsMax);
#define CsrTlsConfigCipherReqSend2(_session, _cipherSuite, _cipherSuiteLength, \
                                   _keybitMin, _keybitsMax) { \
        CsrTlsConfigCipherReq *__msg; \
        __msg = CsrTlsConfigCipherReq_struct(_session, _cipherSuite, _cipherSuiteLength, _keybitMin, _keybitsMax); \
        CsrMsgTransport(CSR_TLS_IFACEQUEUE, CSR_TLS_PRIM, __msg); \
}
#define CsrTlsConfigCipherReqSend CsrTlsConfigCipherReqSend2

CsrTlsConfigCertClientReq *CsrTlsConfigCertClientReq_struct(
    CsrTlsSession session,
    CsrUint8 certificateType,
    CsrUint8 *certificate,
    CsrSize certificateLength,
    CsrUint8 *password,
    CsrSize passwordLength);
#define CsrTlsConfigCertClientReqSend2(_session, _certificateType, \
                                       _certificate, _certificateLength, _password, _passwordLength) { \
        CsrTlsConfigCertClientReq *__msg; \
        __msg = CsrTlsConfigCertClientReq_struct(_session, _certificateType, _certificate, _certificateLength, _password, _passwordLength); \
        CsrMsgTransport(CSR_TLS_IFACEQUEUE, CSR_TLS_PRIM, __msg); \
}
#define CsrTlsConfigCertClientReqSend CsrTlsConfigCertClientReqSend2

CsrTlsConfigCertVerifyReq *CsrTlsConfigCertVerifyReq_struct(
    CsrTlsSession session,
    CsrUint8 *trustedCAcertificate,
    CsrSize trustedCAcertificateLength);
#define CsrTlsConfigCertVerifyReqSend2(_session, _trustedCAcertificate, \
                                       _trustedCAcertificateLength) { \
        CsrTlsConfigCertVerifyReq *__msg; \
        __msg = CsrTlsConfigCertVerifyReq_struct(_session, _trustedCAcertificate, _trustedCAcertificateLength); \
        CsrMsgTransport(CSR_TLS_IFACEQUEUE, CSR_TLS_PRIM, __msg); \
}
#define CsrTlsConfigCertVerifyReqSend CsrTlsConfigCertVerifyReqSend2

CsrTlsConfigCompressionReq *CsrTlsConfigCompressionReq_struct(
    CsrTlsSession session,
    CsrUint8 compression);
#define CsrTlsConfigCompressionReqSend2(_session, _compression) { \
        CsrTlsConfigCompressionReq *__msg; \
        __msg = CsrTlsConfigCompressionReq_struct(_session, _compression); \
        CsrMsgTransport(CSR_TLS_IFACEQUEUE, CSR_TLS_PRIM, __msg); \
}
#define CsrTlsConfigCompressionReqSend CsrTlsConfigCompressionReqSend2

CsrTlsConnectionCreateReq *CsrTlsConnectionCreateReq_struct(
    CsrTlsSession session,
    CsrIpSocketFamily socketFamily);
#define CsrTlsConnectionCreateReqSend2(_session, _family) { \
        CsrTlsConnectionCreateReq *__msg; \
        __msg = CsrTlsConnectionCreateReq_struct(_session, socketFamily); \
        CsrMsgTransport(CSR_TLS_IFACEQUEUE, CSR_TLS_PRIM, __msg); \
}
#define CsrTlsConnectionCreateReqSend(_session) \
    CsrTlsConnectionCreateReqSend2(_session, CSR_IP_SOCKET_FAMILY_IP4)

CsrTlsConnectionDestroyReq *CsrTlsConnectionDestroyReq_struct(
    CsrTlsConnection conn);
#define CsrTlsConnectionDestroyReqSend2(_conn){ \
        CsrTlsConnectionDestroyReq *__msg; \
        __msg = CsrTlsConnectionDestroyReq_struct(_conn); \
        CsrMsgTransport(CSR_TLS_IFACEQUEUE, CSR_TLS_PRIM, __msg); \
}
#define CsrTlsConnectionDestroyReqSend CsrTlsConnectionDestroyReqSend2

CsrTlsConnectionBindReq *CsrTlsConnectionBindReq_struct(
    CsrTlsConnection conn,
    CsrUint8 ip[16],
    CsrUint16 port);
#define CsrTlsConnectionBindReqSend2(_conn, _ip, _port) { \
        CsrTlsConnectionBindReq *__msg; \
        __msg = CsrTlsConnectionBindReq_struct(_conn, _ip, _port); \
        CsrMsgTransport(CSR_TLS_IFACEQUEUE, CSR_TLS_PRIM, __msg); \
}
#define CsrTlsConnectionBindReqSend(_conn, _ip, _port){ \
        CsrTlsConnectionBindReq *__msg; \
        CsrUint8 ipAddress__[16]; \
        CsrMemCpy(ipAddress__, _ip, 4); \
        CsrMemSet(&ipAddress__[4], 0, 12); \
        CsrTlsConnectionBindReqSend2(_conn, ipAddress__, _port); \
}

CsrTlsConnectionConnectReq *CsrTlsConnectionConnectReq_struct(
    CsrTlsConnection conn,
    CsrUint8 ip[16],
    CsrUint16 port);
#define CsrTlsConnectionConnectReqSend2(_conn, _ip, _port) { \
        CsrTlsConnectionConnectReq *__msg; \
        __msg = CsrTlsConnectionConnectReq_struct(_conn, _ip, _port); \
        CsrMsgTransport(CSR_TLS_IFACEQUEUE, CSR_TLS_PRIM, __msg); \
}
#define CsrTlsConnectionConnectReqSend(_conn, _ip, _port) { \
        CsrUint8 ipAddress__[16]; \
        CsrMemCpy(ipAddress__, _ip, 4); \
        CsrMemSet(&ipAddress__[4], 0, 12); \
        CsrTlsConnectionConnectReqSend2(_conn, ipAddress__, _port); \
}

CsrTlsConnectionDisconnectReq *CsrTlsConnectionDisconnectReq_struct(
    CsrTlsConnection conn);
#define CsrTlsConnectionDisconnectReqSend2(_conn) { \
        CsrTlsConnectionDisconnectReq *__msg; \
        __msg = CsrTlsConnectionDisconnectReq_struct(_conn); \
        CsrMsgTransport(CSR_TLS_IFACEQUEUE, CSR_TLS_PRIM, __msg); \
}
#define CsrTlsConnectionDisconnectReqSend CsrTlsConnectionDisconnectReqSend2

CsrTlsConnectionDisconnectRes *CsrTlsConnectionDisconnectRes_struct(
    CsrTlsConnection conn);
#define CsrTlsConnectionDisconnectResSend2(_conn) { \
        CsrTlsConnectionDisconnectRes *__msg; \
        __msg = CsrTlsConnectionDisconnectRes_struct(_conn); \
        CsrMsgTransport(CSR_TLS_IFACEQUEUE, CSR_TLS_PRIM, __msg); \
}
#define CsrTlsConnectionDisconnectResSend CsrTlsConnectionDisconnectResSend2

CsrTlsConnectionCertRes *CsrTlsConnectionCertRes_struct(
    CsrTlsConnection conn,
    CsrBool accepted);
#define CsrTlsConnectionCertResSend2(_conn, _accepted) { \
        CsrTlsConnectionCertRes *__msg; \
        __msg = CsrTlsConnectionCertRes_struct(_conn, _accepted); \
        CsrMsgTransport(CSR_TLS_IFACEQUEUE, CSR_TLS_PRIM, __msg); \
}
#define CsrTlsConnectionCertResSend CsrTlsConnectionCertResSend2

CsrTlsConnectionDataReq *CsrTlsConnectionDataReq_struct(
    CsrTlsConnection conn,
    CsrUint8 *buf,
    CsrSize buflen);
#define CsrTlsConnectionDataReqSend2(_conn, _buf, _buflen) { \
        CsrTlsConnectionDataReq *__msg; \
        __msg = CsrTlsConnectionDataReq_struct(_conn, _buf, _buflen); \
        CsrMsgTransport(CSR_TLS_IFACEQUEUE, CSR_TLS_PRIM, __msg); \
}
#define CsrTlsConnectionDataReqSend CsrTlsConnectionDataReqSend2

CsrTlsConnectionDataRes *CsrTlsConnectionDataRes_struct(
    CsrTlsConnection conn);
#define CsrTlsConnectionDataResSend2(_conn) { \
        CsrTlsConnectionDataRes *__msg; \
        __msg = CsrTlsConnectionDataRes_struct(_conn); \
        CsrMsgTransport(CSR_TLS_IFACEQUEUE, CSR_TLS_PRIM, __msg); \
}
#define CsrTlsConnectionDataResSend CsrTlsConnectionDataResSend2

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrTlsFreeUpstreamMessageContents
 *
 *  DESCRIPTION
 *      Deallocates the payload in the CSR_TLS upstream messages
 *
 *    PARAMETERS
 *      eventClass:   Must be CSR_TLS_PRIM
 *      message:      The message received from CSR TLS
 *----------------------------------------------------------------------------*/
void CsrTlsFreeUpstreamMessageContents(CsrUint16 eventClass, void *message);

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrTlsFreeDownstreamMessageContents
 *
 *  DESCRIPTION
 *      Deallocates the payload in the CSR_TLS downstream messages
 *
 *    PARAMETERS
 *      eventClass:   Must be CSR_TLS_PRIM
 *      message:      The message received from an external task
 *----------------------------------------------------------------------------*/
void CsrTlsFreeDownstreamMessageContents(CsrUint16 eventClass, void *message);

#ifdef __cplusplus
}
#endif

#endif
