#ifndef CSR_BT_BPPS_SERIALIZE_H__
#define CSR_BT_BPPS_SERIALIZE_H__
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

void CsrBtBppsPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_BPPS_DEACTIVATE_CFM
CsrSize CsrBtBppsDeactivateCfmSizeof(void *msg);
CsrUint8 *CsrBtBppsDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppsDeactivateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBppsDeactivateCfmSerFree CsrBtBppsPfree
#endif /* EXCLUDE_CSR_BT_BPPS_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_BPPS_JOB_COMPLETE_CFM
CsrSize CsrBtBppsJobCompleteCfmSizeof(void *msg);
CsrUint8 *CsrBtBppsJobCompleteCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppsJobCompleteCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBppsJobCompleteCfmSerFree CsrBtBppsPfree
#endif /* EXCLUDE_CSR_BT_BPPS_JOB_COMPLETE_CFM */

#ifndef EXCLUDE_CSR_BT_BPPS_NEXT_RES
CsrSize CsrBtBppsNextResSizeof(void *msg);
CsrUint8 *CsrBtBppsNextResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppsNextResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBppsNextResSerFree CsrBtBppsPfree
#endif /* EXCLUDE_CSR_BT_BPPS_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_REQ
CsrSize CsrBtBppsGetReferencedObjReqSizeof(void *msg);
CsrUint8 *CsrBtBppsGetReferencedObjReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppsGetReferencedObjReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBppsGetReferencedObjReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_REQ */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_PRINTER_ATTRIBS_IND
CsrSize CsrBtBppsGetPrinterAttribsIndSizeof(void *msg);
CsrUint8 *CsrBtBppsGetPrinterAttribsIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppsGetPrinterAttribsIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBppsGetPrinterAttribsIndSerFree CsrBtBppsPfree
#endif /* EXCLUDE_CSR_BT_BPPS_GET_PRINTER_ATTRIBS_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_JOB_ATTRIBS_IND
CsrSize CsrBtBppsGetJobAttribsIndSizeof(void *msg);
CsrUint8 *CsrBtBppsGetJobAttribsIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppsGetJobAttribsIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBppsGetJobAttribsIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BPPS_GET_JOB_ATTRIBS_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_CONNECT_RES
CsrSize CsrBtBppsConnectResSizeof(void *msg);
CsrUint8 *CsrBtBppsConnectResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppsConnectResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBppsConnectResSerFree CsrBtBppsPfree
#endif /* EXCLUDE_CSR_BT_BPPS_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_BPPS_DISCONNECT_IND
CsrSize CsrBtBppsDisconnectIndSizeof(void *msg);
CsrUint8 *CsrBtBppsDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppsDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBppsDisconnectIndSerFree CsrBtBppsPfree
#endif /* EXCLUDE_CSR_BT_BPPS_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_SEND_DOCUMENT_IND
CsrSize CsrBtBppsSendDocumentIndSizeof(void *msg);
CsrUint8 *CsrBtBppsSendDocumentIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppsSendDocumentIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBppsSendDocumentIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BPPS_SEND_DOCUMENT_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_NEXT_IND
CsrSize CsrBtBppsNextIndSizeof(void *msg);
CsrUint8 *CsrBtBppsNextIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppsNextIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBppsNextIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BPPS_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_SECURITY_IN_REQ
CsrSize CsrBtBppsSecurityInReqSizeof(void *msg);
CsrUint8 *CsrBtBppsSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppsSecurityInReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBppsSecurityInReqSerFree CsrBtBppsPfree
#endif /* EXCLUDE_CSR_BT_BPPS_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_BPPS_DEACTIVATE_REQ
CsrSize CsrBtBppsDeactivateReqSizeof(void *msg);
CsrUint8 *CsrBtBppsDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppsDeactivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBppsDeactivateReqSerFree CsrBtBppsPfree
#endif /* EXCLUDE_CSR_BT_BPPS_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_BPPS_SECURITY_IN_CFM
CsrSize CsrBtBppsSecurityInCfmSizeof(void *msg);
CsrUint8 *CsrBtBppsSecurityInCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppsSecurityInCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBppsSecurityInCfmSerFree CsrBtBppsPfree
#endif /* EXCLUDE_CSR_BT_BPPS_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_BPPS_CANCEL_JOB_IND
CsrSize CsrBtBppsCancelJobIndSizeof(void *msg);
CsrUint8 *CsrBtBppsCancelJobIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppsCancelJobIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBppsCancelJobIndSerFree CsrBtBppsPfree
#endif /* EXCLUDE_CSR_BT_BPPS_CANCEL_JOB_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_REQ
CsrSize CsrBtBppsAuthenticateReqSizeof(void *msg);
CsrUint8 *CsrBtBppsAuthenticateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppsAuthenticateReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBppsAuthenticateReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_REQ */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_CFM
CsrSize CsrBtBppsGetReferencedObjCfmSizeof(void *msg);
CsrUint8 *CsrBtBppsGetReferencedObjCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppsGetReferencedObjCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBppsGetReferencedObjCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_CFM */

#ifndef EXCLUDE_CSR_BT_BPPS_CONNECT_IND
CsrSize CsrBtBppsConnectIndSizeof(void *msg);
CsrUint8 *CsrBtBppsConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppsConnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBppsConnectIndSerFree CsrBtBppsPfree
#endif /* EXCLUDE_CSR_BT_BPPS_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_CFM
CsrSize CsrBtBppsAuthenticateCfmSizeof(void *msg);
CsrUint8 *CsrBtBppsAuthenticateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppsAuthenticateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBppsAuthenticateCfmSerFree CsrBtBppsPfree
#endif /* EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_CFM */

#ifndef EXCLUDE_CSR_BT_BPPS_ABORT_IND
CsrSize CsrBtBppsAbortIndSizeof(void *msg);
CsrUint8 *CsrBtBppsAbortIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppsAbortIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBppsAbortIndSerFree CsrBtBppsPfree
#endif /* EXCLUDE_CSR_BT_BPPS_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_PRINTER_ATTRIBS_RES
CsrSize CsrBtBppsGetPrinterAttribsResSizeof(void *msg);
CsrUint8 *CsrBtBppsGetPrinterAttribsResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppsGetPrinterAttribsResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBppsGetPrinterAttribsResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BPPS_GET_PRINTER_ATTRIBS_RES */

#ifndef EXCLUDE_CSR_BT_BPPS_ACTIVATE_REQ
CsrSize CsrBtBppsActivateReqSizeof(void *msg);
CsrUint8 *CsrBtBppsActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppsActivateReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBppsActivateReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BPPS_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_JOB_ATTRIBS_RES
CsrSize CsrBtBppsGetJobAttribsResSizeof(void *msg);
CsrUint8 *CsrBtBppsGetJobAttribsResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppsGetJobAttribsResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBppsGetJobAttribsResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BPPS_GET_JOB_ATTRIBS_RES */

#ifndef EXCLUDE_CSR_BT_BPPS_CREATE_JOB_IND
CsrSize CsrBtBppsCreateJobIndSizeof(void *msg);
CsrUint8 *CsrBtBppsCreateJobIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppsCreateJobIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBppsCreateJobIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BPPS_CREATE_JOB_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_IND
CsrSize CsrBtBppsGetReferencedObjIndSizeof(void *msg);
CsrUint8 *CsrBtBppsGetReferencedObjIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppsGetReferencedObjIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBppsGetReferencedObjIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_EVENT_RES
CsrSize CsrBtBppsGetEventResSizeof(void *msg);
CsrUint8 *CsrBtBppsGetEventResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppsGetEventResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBppsGetEventResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BPPS_GET_EVENT_RES */

#ifndef EXCLUDE_CSR_BT_BPPS_JOB_COMPLETE_REQ
CsrSize CsrBtBppsJobCompleteReqSizeof(void *msg);
CsrUint8 *CsrBtBppsJobCompleteReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppsJobCompleteReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBppsJobCompleteReqSerFree CsrBtBppsPfree
#endif /* EXCLUDE_CSR_BT_BPPS_JOB_COMPLETE_REQ */

#ifndef EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_IND
CsrSize CsrBtBppsAuthenticateIndSizeof(void *msg);
CsrUint8 *CsrBtBppsAuthenticateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppsAuthenticateIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBppsAuthenticateIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_RES
CsrSize CsrBtBppsAuthenticateResSizeof(void *msg);
CsrUint8 *CsrBtBppsAuthenticateResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppsAuthenticateResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtBppsAuthenticateResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_BPPS_CREATE_JOB_RES
CsrSize CsrBtBppsCreateJobResSizeof(void *msg);
CsrUint8 *CsrBtBppsCreateJobResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppsCreateJobResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBppsCreateJobResSerFree CsrBtBppsPfree
#endif /* EXCLUDE_CSR_BT_BPPS_CREATE_JOB_RES */

#ifndef EXCLUDE_CSR_BT_BPPS_CANCEL_JOB_RES
CsrSize CsrBtBppsCancelJobResSizeof(void *msg);
CsrUint8 *CsrBtBppsCancelJobResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppsCancelJobResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBppsCancelJobResSerFree CsrBtBppsPfree
#endif /* EXCLUDE_CSR_BT_BPPS_CANCEL_JOB_RES */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_RES
CsrSize CsrBtBppsGetReferencedObjResSizeof(void *msg);
CsrUint8 *CsrBtBppsGetReferencedObjResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppsGetReferencedObjResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBppsGetReferencedObjResSerFree CsrBtBppsPfree
#endif /* EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_RES */

#ifndef EXCLUDE_CSR_BT_BPPS_GET_EVENT_IND
CsrSize CsrBtBppsGetEventIndSizeof(void *msg);
CsrUint8 *CsrBtBppsGetEventIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppsGetEventIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBppsGetEventIndSerFree CsrBtBppsPfree
#endif /* EXCLUDE_CSR_BT_BPPS_GET_EVENT_IND */

#ifndef EXCLUDE_CSR_BT_BPPS_ACTIVATE_CFM
CsrSize CsrBtBppsActivateCfmSizeof(void *msg);
CsrUint8 *CsrBtBppsActivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtBppsActivateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtBppsActivateCfmSerFree CsrBtBppsPfree
#endif /* EXCLUDE_CSR_BT_BPPS_ACTIVATE_CFM */

#ifdef __cplusplus
}
#endif

#endif
