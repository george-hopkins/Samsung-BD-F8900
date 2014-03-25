/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_synergy.h"
#include "csr_msgconv.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_bt_hand_coded.h"
#include "csr_bt_bpps_converter_init.h"
#ifndef EXCLUDE_CSR_BT_BPPS_MODULE
#include "csr_bt_bpps_serialize.h"
#include "csr_bt_bpps_prim.h"

static CsrMsgConvMsgEntry csr_bt_bpps_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_BPPS_ABORT_IND
    { CSR_BT_BPPS_ABORT_IND, CsrBtBppsAbortIndSizeof, CsrBtBppsAbortIndSer, CsrBtBppsAbortIndDes, CsrBtBppsAbortIndSerFree },
#endif /* EXCLUDE_CSR_BT_BPPS_ABORT_IND */
#ifndef EXCLUDE_CSR_BT_BPPS_ACTIVATE_CFM
    { CSR_BT_BPPS_ACTIVATE_CFM, CsrBtBppsActivateCfmSizeof, CsrBtBppsActivateCfmSer, CsrBtBppsActivateCfmDes, CsrBtBppsActivateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_BPPS_ACTIVATE_CFM */
#ifndef EXCLUDE_CSR_BT_BPPS_ACTIVATE_REQ
    { CSR_BT_BPPS_ACTIVATE_REQ, CsrBtBppsActivateReqSizeof, CsrBtBppsActivateReqSer, CsrBtBppsActivateReqDes, CsrBtBppsActivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_BPPS_ACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_CFM
    { CSR_BT_BPPS_AUTHENTICATE_CFM, CsrBtBppsAuthenticateCfmSizeof, CsrBtBppsAuthenticateCfmSer, CsrBtBppsAuthenticateCfmDes, CsrBtBppsAuthenticateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_CFM */
#ifndef EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_IND
    { CSR_BT_BPPS_AUTHENTICATE_IND, CsrBtBppsAuthenticateIndSizeof, CsrBtBppsAuthenticateIndSer, CsrBtBppsAuthenticateIndDes, CsrBtBppsAuthenticateIndSerFree },
#endif /* EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_IND */
#ifndef EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_REQ
    { CSR_BT_BPPS_AUTHENTICATE_REQ, CsrBtBppsAuthenticateReqSizeof, CsrBtBppsAuthenticateReqSer, CsrBtBppsAuthenticateReqDes, CsrBtBppsAuthenticateReqSerFree },
#endif /* EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_REQ */
#ifndef EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_RES
    { CSR_BT_BPPS_AUTHENTICATE_RES, CsrBtBppsAuthenticateResSizeof, CsrBtBppsAuthenticateResSer, CsrBtBppsAuthenticateResDes, CsrBtBppsAuthenticateResSerFree },
#endif /* EXCLUDE_CSR_BT_BPPS_AUTHENTICATE_RES */
#ifndef EXCLUDE_CSR_BT_BPPS_CANCEL_JOB_IND
    { CSR_BT_BPPS_CANCEL_JOB_IND, CsrBtBppsCancelJobIndSizeof, CsrBtBppsCancelJobIndSer, CsrBtBppsCancelJobIndDes, CsrBtBppsCancelJobIndSerFree },
#endif /* EXCLUDE_CSR_BT_BPPS_CANCEL_JOB_IND */
#ifndef EXCLUDE_CSR_BT_BPPS_CANCEL_JOB_RES
    { CSR_BT_BPPS_CANCEL_JOB_RES, CsrBtBppsCancelJobResSizeof, CsrBtBppsCancelJobResSer, CsrBtBppsCancelJobResDes, CsrBtBppsCancelJobResSerFree },
#endif /* EXCLUDE_CSR_BT_BPPS_CANCEL_JOB_RES */
#ifndef EXCLUDE_CSR_BT_BPPS_CONNECT_IND
    { CSR_BT_BPPS_CONNECT_IND, CsrBtBppsConnectIndSizeof, CsrBtBppsConnectIndSer, CsrBtBppsConnectIndDes, CsrBtBppsConnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_BPPS_CONNECT_IND */
#ifndef EXCLUDE_CSR_BT_BPPS_CONNECT_RES
    { CSR_BT_BPPS_CONNECT_RES, CsrBtBppsConnectResSizeof, CsrBtBppsConnectResSer, CsrBtBppsConnectResDes, CsrBtBppsConnectResSerFree },
#endif /* EXCLUDE_CSR_BT_BPPS_CONNECT_RES */
#ifndef EXCLUDE_CSR_BT_BPPS_CREATE_JOB_IND
    { CSR_BT_BPPS_CREATE_JOB_IND, CsrBtBppsCreateJobIndSizeof, CsrBtBppsCreateJobIndSer, CsrBtBppsCreateJobIndDes, CsrBtBppsCreateJobIndSerFree },
#endif /* EXCLUDE_CSR_BT_BPPS_CREATE_JOB_IND */
#ifndef EXCLUDE_CSR_BT_BPPS_CREATE_JOB_RES
    { CSR_BT_BPPS_CREATE_JOB_RES, CsrBtBppsCreateJobResSizeof, CsrBtBppsCreateJobResSer, CsrBtBppsCreateJobResDes, CsrBtBppsCreateJobResSerFree },
#endif /* EXCLUDE_CSR_BT_BPPS_CREATE_JOB_RES */
#ifndef EXCLUDE_CSR_BT_BPPS_DEACTIVATE_CFM
    { CSR_BT_BPPS_DEACTIVATE_CFM, CsrBtBppsDeactivateCfmSizeof, CsrBtBppsDeactivateCfmSer, CsrBtBppsDeactivateCfmDes, CsrBtBppsDeactivateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_BPPS_DEACTIVATE_CFM */
#ifndef EXCLUDE_CSR_BT_BPPS_DEACTIVATE_REQ
    { CSR_BT_BPPS_DEACTIVATE_REQ, CsrBtBppsDeactivateReqSizeof, CsrBtBppsDeactivateReqSer, CsrBtBppsDeactivateReqDes, CsrBtBppsDeactivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_BPPS_DEACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_BPPS_DISCONNECT_IND
    { CSR_BT_BPPS_DISCONNECT_IND, CsrBtBppsDisconnectIndSizeof, CsrBtBppsDisconnectIndSer, CsrBtBppsDisconnectIndDes, CsrBtBppsDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_BPPS_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_BT_BPPS_GET_EVENT_IND
    { CSR_BT_BPPS_GET_EVENT_IND, CsrBtBppsGetEventIndSizeof, CsrBtBppsGetEventIndSer, CsrBtBppsGetEventIndDes, CsrBtBppsGetEventIndSerFree },
#endif /* EXCLUDE_CSR_BT_BPPS_GET_EVENT_IND */
#ifndef EXCLUDE_CSR_BT_BPPS_GET_EVENT_RES
    { CSR_BT_BPPS_GET_EVENT_RES, CsrBtBppsGetEventResSizeof, CsrBtBppsGetEventResSer, CsrBtBppsGetEventResDes, CsrBtBppsGetEventResSerFree },
#endif /* EXCLUDE_CSR_BT_BPPS_GET_EVENT_RES */
#ifndef EXCLUDE_CSR_BT_BPPS_GET_JOB_ATTRIBS_IND
    { CSR_BT_BPPS_GET_JOB_ATTRIBS_IND, CsrBtBppsGetJobAttribsIndSizeof, CsrBtBppsGetJobAttribsIndSer, CsrBtBppsGetJobAttribsIndDes, CsrBtBppsGetJobAttribsIndSerFree },
#endif /* EXCLUDE_CSR_BT_BPPS_GET_JOB_ATTRIBS_IND */
#ifndef EXCLUDE_CSR_BT_BPPS_GET_JOB_ATTRIBS_RES
    { CSR_BT_BPPS_GET_JOB_ATTRIBS_RES, CsrBtBppsGetJobAttribsResSizeof, CsrBtBppsGetJobAttribsResSer, CsrBtBppsGetJobAttribsResDes, CsrBtBppsGetJobAttribsResSerFree },
#endif /* EXCLUDE_CSR_BT_BPPS_GET_JOB_ATTRIBS_RES */
#ifndef EXCLUDE_CSR_BT_BPPS_GET_PRINTER_ATTRIBS_IND
    { CSR_BT_BPPS_GET_PRINTER_ATTRIBS_IND, CsrBtBppsGetPrinterAttribsIndSizeof, CsrBtBppsGetPrinterAttribsIndSer, CsrBtBppsGetPrinterAttribsIndDes, CsrBtBppsGetPrinterAttribsIndSerFree },
#endif /* EXCLUDE_CSR_BT_BPPS_GET_PRINTER_ATTRIBS_IND */
#ifndef EXCLUDE_CSR_BT_BPPS_GET_PRINTER_ATTRIBS_RES
    { CSR_BT_BPPS_GET_PRINTER_ATTRIBS_RES, CsrBtBppsGetPrinterAttribsResSizeof, CsrBtBppsGetPrinterAttribsResSer, CsrBtBppsGetPrinterAttribsResDes, CsrBtBppsGetPrinterAttribsResSerFree },
#endif /* EXCLUDE_CSR_BT_BPPS_GET_PRINTER_ATTRIBS_RES */
#ifndef EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_CFM
    { CSR_BT_BPPS_GET_REFERENCED_OBJ_CFM, CsrBtBppsGetReferencedObjCfmSizeof, CsrBtBppsGetReferencedObjCfmSer, CsrBtBppsGetReferencedObjCfmDes, CsrBtBppsGetReferencedObjCfmSerFree },
#endif /* EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_CFM */
#ifndef EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_IND
    { CSR_BT_BPPS_GET_REFERENCED_OBJ_IND, CsrBtBppsGetReferencedObjIndSizeof, CsrBtBppsGetReferencedObjIndSer, CsrBtBppsGetReferencedObjIndDes, CsrBtBppsGetReferencedObjIndSerFree },
#endif /* EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_IND */
#ifndef EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_REQ
    { CSR_BT_BPPS_GET_REFERENCED_OBJ_REQ, CsrBtBppsGetReferencedObjReqSizeof, CsrBtBppsGetReferencedObjReqSer, CsrBtBppsGetReferencedObjReqDes, CsrBtBppsGetReferencedObjReqSerFree },
#endif /* EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_REQ */
#ifndef EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_RES
    { CSR_BT_BPPS_GET_REFERENCED_OBJ_RES, CsrBtBppsGetReferencedObjResSizeof, CsrBtBppsGetReferencedObjResSer, CsrBtBppsGetReferencedObjResDes, CsrBtBppsGetReferencedObjResSerFree },
#endif /* EXCLUDE_CSR_BT_BPPS_GET_REFERENCED_OBJ_RES */
#ifndef EXCLUDE_CSR_BT_BPPS_JOB_COMPLETE_CFM
    { CSR_BT_BPPS_JOB_COMPLETE_CFM, CsrBtBppsJobCompleteCfmSizeof, CsrBtBppsJobCompleteCfmSer, CsrBtBppsJobCompleteCfmDes, CsrBtBppsJobCompleteCfmSerFree },
#endif /* EXCLUDE_CSR_BT_BPPS_JOB_COMPLETE_CFM */
#ifndef EXCLUDE_CSR_BT_BPPS_JOB_COMPLETE_REQ
    { CSR_BT_BPPS_JOB_COMPLETE_REQ, CsrBtBppsJobCompleteReqSizeof, CsrBtBppsJobCompleteReqSer, CsrBtBppsJobCompleteReqDes, CsrBtBppsJobCompleteReqSerFree },
#endif /* EXCLUDE_CSR_BT_BPPS_JOB_COMPLETE_REQ */
#ifndef EXCLUDE_CSR_BT_BPPS_NEXT_IND
    { CSR_BT_BPPS_NEXT_IND, CsrBtBppsNextIndSizeof, CsrBtBppsNextIndSer, CsrBtBppsNextIndDes, CsrBtBppsNextIndSerFree },
#endif /* EXCLUDE_CSR_BT_BPPS_NEXT_IND */
#ifndef EXCLUDE_CSR_BT_BPPS_NEXT_RES
    { CSR_BT_BPPS_NEXT_RES, CsrBtBppsNextResSizeof, CsrBtBppsNextResSer, CsrBtBppsNextResDes, CsrBtBppsNextResSerFree },
#endif /* EXCLUDE_CSR_BT_BPPS_NEXT_RES */
#ifndef EXCLUDE_CSR_BT_BPPS_SECURITY_IN_CFM
    { CSR_BT_BPPS_SECURITY_IN_CFM, CsrBtBppsSecurityInCfmSizeof, CsrBtBppsSecurityInCfmSer, CsrBtBppsSecurityInCfmDes, CsrBtBppsSecurityInCfmSerFree },
#endif /* EXCLUDE_CSR_BT_BPPS_SECURITY_IN_CFM */
#ifndef EXCLUDE_CSR_BT_BPPS_SECURITY_IN_REQ
    { CSR_BT_BPPS_SECURITY_IN_REQ, CsrBtBppsSecurityInReqSizeof, CsrBtBppsSecurityInReqSer, CsrBtBppsSecurityInReqDes, CsrBtBppsSecurityInReqSerFree },
#endif /* EXCLUDE_CSR_BT_BPPS_SECURITY_IN_REQ */
#ifndef EXCLUDE_CSR_BT_BPPS_SEND_DOCUMENT_IND
    { CSR_BT_BPPS_SEND_DOCUMENT_IND, CsrBtBppsSendDocumentIndSizeof, CsrBtBppsSendDocumentIndSer, CsrBtBppsSendDocumentIndDes, CsrBtBppsSendDocumentIndSerFree },
#endif /* EXCLUDE_CSR_BT_BPPS_SEND_DOCUMENT_IND */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtBppsConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_BPPS_PRIM, csr_bt_bpps_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_bpps_conv_info = {
    CSR_BT_BPPS_PRIM,
    "CSR_BT_BPPS_PRIM",
    csr_bt_bpps_conv_lut
};

CsrLogPrimitiveInformation* CsrBtBppsTechInfoGet(void)
{
    return &csr_bt_bpps_conv_info;
}
#endif
#endif

