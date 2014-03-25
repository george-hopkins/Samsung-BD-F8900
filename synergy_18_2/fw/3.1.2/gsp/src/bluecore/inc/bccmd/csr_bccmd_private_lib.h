#ifndef CSR_BCCMD_PRIVATE_LIB_H__
#define CSR_BCCMD_PRIVATE_LIB_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/
#include "csr_bccmd_private_prim.h"
#include "csr_bccmd_task.h"
#include "csr_msg_transport.h"

#ifdef __cplusplus
extern "C" {
#endif

CsrBccmdBootstrapReq *CsrBccmdBootstrapReq_struct(CsrSchedQid phandle, CsrUint16 payloadLength, CsrUint8 *payload);
#define CsrBccmdBootstrapReqSend(_ph, _pl, _p){ \
        CsrBccmdBootstrapReq *msg__; \
        msg__ = CsrBccmdBootstrapReq_struct(_ph, _pl, _p); \
        CsrMsgTransport(CSR_BCCMD_IFACEQUEUE, CSR_BCCMD_PRIM, msg__);}

CsrBccmdResetRes *CsrBccmdResetRes_struct(CsrResult status);
#define CsrBccmdResetResSend(_st){ \
        CsrBccmdResetRes *msg__; \
        msg__ = CsrBccmdResetRes_struct(_st); \
        CsrMsgTransport(CSR_BCCMD_IFACEQUEUE, CSR_BCCMD_PRIM, msg__);}

CsrBccmdGetBuildIdReq *CsrBccmdGetBuildIdReq_struct(CsrSchedQid phandle);
#define CsrBccmdGetBuildIdReqSend(_ph){ \
        CsrBccmdGetBuildIdReq *msg__; \
        msg__ = CsrBccmdGetBuildIdReq_struct(_ph); \
        CsrMsgTransport(CSR_BCCMD_IFACEQUEUE, CSR_BCCMD_PRIM, msg__);}

CsrBccmdQueryReq *CsrBccmdQueryReq_struct(void);
#define CsrBccmdQueryReqSend(){ \
        CsrBccmdQueryReq *msg__; \
        msg__ = CsrBccmdQueryReq_struct(); \
        CsrMsgTransport(CSR_BCCMD_IFACEQUEUE, CSR_BCCMD_PRIM, msg__);}

CsrBccmdPanicArgsReq *CsrBccmdPanicArgsReq_struct(CsrSchedQid phandle);
#define CsrBccmdPanicArgsReqSend(_ph){ \
        CsrBccmdPanicArgsReq *msg__; \
        msg__ = CsrBccmdPanicArgsReq_struct(_ph); \
        CsrMsgTransport(CSR_BCCMD_IFACEQUEUE, CSR_BCCMD_PRIM, msg__);}

#ifdef __cplusplus
}
#endif

#endif /* CSR_BCCMD_PRIVATE_LIB_H__ */
