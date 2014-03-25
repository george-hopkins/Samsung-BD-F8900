#ifndef CSR_BT_BPPC_SERIALIZE_H__
#define CSR_BT_BPPC_SERIALIZE_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_types.h"

#include "csr_synergy.h"
#ifdef __cplusplus
extern "C" {
#endif

void CsrBtBppcPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_BPPC_DEACTIVATE_CFM
CsrSize CsrBtBppcDeactivateCfmSizeof(void *msg);
CsrUint8 *CsrBtBppcDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppcDeactivateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBppcDeactivateCfmSerFree CsrBtBppcPfree
#endif /* EXCLUDE_CSR_BT_BPPC_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_BPPC_CONNECT_RES
CsrSize CsrBtBppcConnectResSizeof(void *msg);
CsrUint8 *CsrBtBppcConnectResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppcConnectResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBppcConnectResSerFree CsrBtBppcPfree
#endif /* EXCLUDE_CSR_BT_BPPC_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_BPPC_CONNECT_IND
CsrSize CsrBtBppcConnectIndSizeof(void *msg);
CsrUint8 *CsrBtBppcConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppcConnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBppcConnectIndSerFree CsrBtBppcPfree
#endif /* EXCLUDE_CSR_BT_BPPC_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_EVENT_RES
CsrSize CsrBtBppcGetEventResSizeof(void *msg);
CsrUint8 *CsrBtBppcGetEventResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppcGetEventResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBppcGetEventResSerFree CsrBtBppcPfree
#endif /* EXCLUDE_CSR_BT_BPPC_GET_EVENT_RES */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_CFM
CsrSize CsrBtBppcGetJobAttributesCfmSizeof(void *msg);
CsrUint8 *CsrBtBppcGetJobAttributesCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppcGetJobAttributesCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBppcGetJobAttributesCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_CFM */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_EVENT_IND
CsrSize CsrBtBppcGetEventIndSizeof(void *msg);
CsrUint8 *CsrBtBppcGetEventIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppcGetEventIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBppcGetEventIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BPPC_GET_EVENT_IND */

#ifndef EXCLUDE_CSR_BT_BPPC_CANCEL_JOB_CFM
CsrSize CsrBtBppcCancelJobCfmSizeof(void *msg);
CsrUint8 *CsrBtBppcCancelJobCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppcCancelJobCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBppcCancelJobCfmSerFree CsrBtBppcPfree
#endif /* EXCLUDE_CSR_BT_BPPC_CANCEL_JOB_CFM */

#ifndef EXCLUDE_CSR_BT_BPPC_CONNECT_REQ
CsrSize CsrBtBppcConnectReqSizeof(void *msg);
CsrUint8 *CsrBtBppcConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppcConnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBppcConnectReqSerFree CsrBtBppcPfree
#endif /* EXCLUDE_CSR_BT_BPPC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_REQ
CsrSize CsrBtBppcGetPrinterAttributesReqSizeof(void *msg);
CsrUint8 *CsrBtBppcGetPrinterAttributesReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppcGetPrinterAttributesReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBppcGetPrinterAttributesReqSerFree CsrBtBppcPfree
#endif /* EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_RES
CsrSize CsrBtBppcGetJobAttributesResSizeof(void *msg);
CsrUint8 *CsrBtBppcGetJobAttributesResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppcGetJobAttributesResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBppcGetJobAttributesResSerFree CsrBtBppcPfree
#endif /* EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_RES */

#ifndef EXCLUDE_CSR_BT_BPPC_ABORT_CFM
CsrSize CsrBtBppcAbortCfmSizeof(void *msg);
CsrUint8 *CsrBtBppcAbortCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppcAbortCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBppcAbortCfmSerFree CsrBtBppcPfree
#endif /* EXCLUDE_CSR_BT_BPPC_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_BPPC_SECURITY_OUT_REQ
CsrSize CsrBtBppcSecurityOutReqSizeof(void *msg);
CsrUint8 *CsrBtBppcSecurityOutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppcSecurityOutReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBppcSecurityOutReqSerFree CsrBtBppcPfree
#endif /* EXCLUDE_CSR_BT_BPPC_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_CFM
CsrSize CsrBtBppcGetPrinterAttributesCfmSizeof(void *msg);
CsrUint8 *CsrBtBppcGetPrinterAttributesCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppcGetPrinterAttributesCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBppcGetPrinterAttributesCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_CFM */

#ifndef EXCLUDE_CSR_BT_BPPC_SECURITY_OUT_CFM
CsrSize CsrBtBppcSecurityOutCfmSizeof(void *msg);
CsrUint8 *CsrBtBppcSecurityOutCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppcSecurityOutCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBppcSecurityOutCfmSerFree CsrBtBppcPfree
#endif /* EXCLUDE_CSR_BT_BPPC_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_IND
CsrSize CsrBtBppcGetJobAttributesIndSizeof(void *msg);
CsrUint8 *CsrBtBppcGetJobAttributesIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppcGetJobAttributesIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBppcGetJobAttributesIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_IND */

#ifndef EXCLUDE_CSR_BT_BPPC_ABORT_REQ
CsrSize CsrBtBppcAbortReqSizeof(void *msg);
CsrUint8 *CsrBtBppcAbortReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppcAbortReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBppcAbortReqSerFree CsrBtBppcPfree
#endif /* EXCLUDE_CSR_BT_BPPC_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_REFERENCE_OBJECT_IND
CsrSize CsrBtBppcGetReferenceObjectIndSizeof(void *msg);
CsrUint8 *CsrBtBppcGetReferenceObjectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppcGetReferenceObjectIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBppcGetReferenceObjectIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BPPC_GET_REFERENCE_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_RES
CsrSize CsrBtBppcSendDocumentResSizeof(void *msg);
CsrUint8 *CsrBtBppcSendDocumentResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppcSendDocumentResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBppcSendDocumentResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_RES */

#ifndef EXCLUDE_CSR_BT_BPPC_AUTHENTICATE_IND
CsrSize CsrBtBppcAuthenticateIndSizeof(void *msg);
CsrUint8 *CsrBtBppcAuthenticateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppcAuthenticateIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBppcAuthenticateIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BPPC_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_RES
CsrSize CsrBtBppcGetPrinterAttributesResSizeof(void *msg);
CsrUint8 *CsrBtBppcGetPrinterAttributesResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppcGetPrinterAttributesResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBppcGetPrinterAttributesResSerFree CsrBtBppcPfree
#endif /* EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_RES */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_IND
CsrSize CsrBtBppcGetPrinterAttributesIndSizeof(void *msg);
CsrUint8 *CsrBtBppcGetPrinterAttributesIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppcGetPrinterAttributesIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBppcGetPrinterAttributesIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_IND */

#ifndef EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_REQ
CsrSize CsrBtBppcSendDocumentReqSizeof(void *msg);
CsrUint8 *CsrBtBppcSendDocumentReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppcSendDocumentReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBppcSendDocumentReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_EVENT_REQ
CsrSize CsrBtBppcGetEventReqSizeof(void *msg);
CsrUint8 *CsrBtBppcGetEventReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppcGetEventReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBppcGetEventReqSerFree CsrBtBppcPfree
#endif /* EXCLUDE_CSR_BT_BPPC_GET_EVENT_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_CANCEL_JOB_REQ
CsrSize CsrBtBppcCancelJobReqSizeof(void *msg);
CsrUint8 *CsrBtBppcCancelJobReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppcCancelJobReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBppcCancelJobReqSerFree CsrBtBppcPfree
#endif /* EXCLUDE_CSR_BT_BPPC_CANCEL_JOB_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_CREATE_JOB_REQ
CsrSize CsrBtBppcCreateJobReqSizeof(void *msg);
CsrUint8 *CsrBtBppcCreateJobReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppcCreateJobReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBppcCreateJobReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BPPC_CREATE_JOB_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_REQ
CsrSize CsrBtBppcGetJobAttributesReqSizeof(void *msg);
CsrUint8 *CsrBtBppcGetJobAttributesReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppcGetJobAttributesReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBppcGetJobAttributesReqSerFree CsrBtBppcPfree
#endif /* EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_CREATE_JOB_CFM
CsrSize CsrBtBppcCreateJobCfmSizeof(void *msg);
CsrUint8 *CsrBtBppcCreateJobCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppcCreateJobCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBppcCreateJobCfmSerFree CsrBtBppcPfree
#endif /* EXCLUDE_CSR_BT_BPPC_CREATE_JOB_CFM */

#ifndef EXCLUDE_CSR_BT_BPPC_CONNECT_CFM
CsrSize CsrBtBppcConnectCfmSizeof(void *msg);
CsrUint8 *CsrBtBppcConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppcConnectCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBppcConnectCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BPPC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_BPPC_DISCONNECT_IND
CsrSize CsrBtBppcDisconnectIndSizeof(void *msg);
CsrUint8 *CsrBtBppcDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppcDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBppcDisconnectIndSerFree CsrBtBppcPfree
#endif /* EXCLUDE_CSR_BT_BPPC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_CFM
CsrSize CsrBtBppcSendDocumentCfmSizeof(void *msg);
CsrUint8 *CsrBtBppcSendDocumentCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppcSendDocumentCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBppcSendDocumentCfmSerFree CsrBtBppcPfree
#endif /* EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_CFM */

#ifndef EXCLUDE_CSR_BT_BPPC_CANCEL_CONNECT_REQ
CsrSize CsrBtBppcCancelConnectReqSizeof(void *msg);
CsrUint8 *CsrBtBppcCancelConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppcCancelConnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBppcCancelConnectReqSerFree CsrBtBppcPfree
#endif /* EXCLUDE_CSR_BT_BPPC_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_IND
CsrSize CsrBtBppcSendDocumentIndSizeof(void *msg);
CsrUint8 *CsrBtBppcSendDocumentIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppcSendDocumentIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBppcSendDocumentIndSerFree CsrBtBppcPfree
#endif /* EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_IND */

#ifndef EXCLUDE_CSR_BT_BPPC_DISCONNECT_REQ
CsrSize CsrBtBppcDisconnectReqSizeof(void *msg);
CsrUint8 *CsrBtBppcDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppcDisconnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBppcDisconnectReqSerFree CsrBtBppcPfree
#endif /* EXCLUDE_CSR_BT_BPPC_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_AUTHENTICATE_RES
CsrSize CsrBtBppcAuthenticateResSizeof(void *msg);
CsrUint8 *CsrBtBppcAuthenticateResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppcAuthenticateResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBppcAuthenticateResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BPPC_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_BPPC_GET_REFERENCE_OBJECT_RES
CsrSize CsrBtBppcGetReferenceObjectResSizeof(void *msg);
CsrUint8 *CsrBtBppcGetReferenceObjectResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppcGetReferenceObjectResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBppcGetReferenceObjectResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BPPC_GET_REFERENCE_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BPPC_DEACTIVATE_REQ
CsrSize CsrBtBppcDeactivateReqSizeof(void *msg);
CsrUint8 *CsrBtBppcDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppcDeactivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBppcDeactivateReqSerFree CsrBtBppcPfree
#endif /* EXCLUDE_CSR_BT_BPPC_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_BPPC_ACTIVATE_REQ
CsrSize CsrBtBppcActivateReqSizeof(void *msg);
CsrUint8 *CsrBtBppcActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppcActivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBppcActivateReqSerFree CsrBtBppcPfree
#endif /* EXCLUDE_CSR_BT_BPPC_ACTIVATE_REQ */

#ifdef __cplusplus
}
#endif

#endif
