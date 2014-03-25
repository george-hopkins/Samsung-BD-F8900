#ifndef CSR_BT_OPC_SEF_H__
#define CSR_BT_OPC_SEF_H__

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_OPC_MODULE
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

CsrUint8 CsrBtOpcConnectReqHandler(OpcInstanceDataType *pInst, void *msg);
CsrUint8 CsrBtOpcDisconnectReqHandler(OpcInstanceDataType *pInst, void *msg);
CsrUint8 CsrBtOpcCancelConnectReqHandler(OpcInstanceDataType *pInst, void *msg);
CsrUint8 CsrBtOpcAbortReqHandler(OpcInstanceDataType *pInst, void *msg);
CsrUint8 CsrBtOpcPutHeaderReqHandler(OpcInstanceDataType *pInst, void *msg);
CsrUint8 CsrBtOpcPutBodyReqHandler(OpcInstanceDataType *pInst, void *msg);
#ifdef CSR_BT_INSTALL_OPC_GET
CsrUint8 CsrBtOpcGetHeaderReqHandler(OpcInstanceDataType *pInst, void *msg);
CsrUint8 CsrBtOpcGetObjReqHandler(OpcInstanceDataType *pInst, void *msg);
#endif
CsrUint8 CsrBtOpcSecurityOutReqHandler(OpcInstanceDataType * pInst, void *msg);
#ifdef CSR_BT_OBEX_AUTH_OPC_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE
CsrUint8 CsrBtOpcAuthenticateResHandler(OpcInstanceDataType *pInst, void *msg);
#endif /* CSR_BT_OBEX_AUTH_OPC_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE */

/* Prototypes from opc_free_down.c */
void CsrBtOpcFreeDownstreamMessageContents(CsrUint16 eventClass, void * message);


#ifdef __cplusplus
}
#endif
#endif
#endif
