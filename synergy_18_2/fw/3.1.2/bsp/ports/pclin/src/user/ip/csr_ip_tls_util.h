#ifndef CSR_IP_TLS_UTIL_H__
#define CSR_IP_TLS_UTIL_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_sched.h"
#include "csr_prim_defs.h"

#include "csr_ip_tls_thread.h"

#ifdef __cplusplus
extern "C" {
#endif

CsrTlsSocketInst *csrIpTlsInstGet(CsrIpTlsInstanceData *instanceData,
    CsrInt32 sockHandle);

void csrTlsSocketInstFree(CsrIpTlsInstanceData *instanceData,
    CsrTlsSocketInst *sockInst);

void csrIpTlsSessionDestroy(CsrIpTlsInstanceData *instanceData,
    csrTlsSession *sess);
void csrIpTlsConnectionDestroy(csrTlsConnection *conn);

int CsrIpTlsMaxFdGet(CsrIpTlsInstanceData *instanceData);

void csrIpTlsCreateSsl(CsrIpTlsInstanceData *instanceData,
    CsrTlsSocketInst *sockInst);
void csrIpTlsConnectSsl(CsrIpTlsInstanceData *instanceData,
    CsrTlsSocketInst *sockInst);

void CsrTlsSessionCreateCfmSend(CsrSchedQid qid,
    CsrTlsSession, CsrResult);
void CsrTlsSessionDestroyCfmSend(CsrSchedQid qid,
    CsrTlsSession, CsrResult);
void CsrTlsConnectionCreateCfmSend(CsrSchedQid qid,
    CsrTlsSession, CsrResult, CsrTlsConnection);
void CsrTlsConnectionDestroyCfmSend(CsrSchedQid qid,
    CsrTlsConnection);
void CsrTlsConnectionBindCfmSend(CsrSchedQid qid,
    CsrTlsConnection, CsrResult);
void CsrTlsConnectionConnectCfmSend(CsrSchedQid qid,
    CsrTlsConnection, CsrResult);
void CsrTlsConnectionDisconnectIndSend(CsrSchedQid qid,
    CsrTlsConnection);
void CsrTlsConnectionDisconnectCfmSend(CsrSchedQid qid,
    CsrTlsConnection, CsrResult);
void CsrTlsConnectionDataCfmSend(CsrSchedQid qid,
    CsrTlsConnection, CsrResult);
void CsrTlsConnectionDataIndSend(CsrSchedQid qid,
    CsrTlsConnection, CsrSize, CsrUint8 *);
void CsrTlsConnectionCertIndSend(CsrSchedQid qid,
    CsrTlsConnection conn, CsrSize certLen, CsrUint8 *cert, CsrResult results);

#ifdef __cplusplus
}
#endif

#endif
