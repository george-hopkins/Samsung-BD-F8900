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
#include "csr_bt_bppc_converter_init.h"
#ifndef EXCLUDE_CSR_BT_BPPC_MODULE
#include "csr_bt_bppc_serialize.h"
#include "csr_bt_bppc_prim.h"

static CsrMsgConvMsgEntry csr_bt_bppc_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_BPPC_ABORT_CFM
    { CSR_BT_BPPC_ABORT_CFM, CsrBtBppcAbortCfmSizeof, CsrBtBppcAbortCfmSer, CsrBtBppcAbortCfmDes, CsrBtBppcAbortCfmSerFree },
#endif /* EXCLUDE_CSR_BT_BPPC_ABORT_CFM */
#ifndef EXCLUDE_CSR_BT_BPPC_ABORT_REQ
    { CSR_BT_BPPC_ABORT_REQ, CsrBtBppcAbortReqSizeof, CsrBtBppcAbortReqSer, CsrBtBppcAbortReqDes, CsrBtBppcAbortReqSerFree },
#endif /* EXCLUDE_CSR_BT_BPPC_ABORT_REQ */
#ifndef EXCLUDE_CSR_BT_BPPC_ACTIVATE_REQ
    { CSR_BT_BPPC_ACTIVATE_REQ, CsrBtBppcActivateReqSizeof, CsrBtBppcActivateReqSer, CsrBtBppcActivateReqDes, CsrBtBppcActivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_BPPC_ACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_BPPC_AUTHENTICATE_IND
    { CSR_BT_BPPC_AUTHENTICATE_IND, CsrBtBppcAuthenticateIndSizeof, CsrBtBppcAuthenticateIndSer, CsrBtBppcAuthenticateIndDes, CsrBtBppcAuthenticateIndSerFree },
#endif /* EXCLUDE_CSR_BT_BPPC_AUTHENTICATE_IND */
#ifndef EXCLUDE_CSR_BT_BPPC_AUTHENTICATE_RES
    { CSR_BT_BPPC_AUTHENTICATE_RES, CsrBtBppcAuthenticateResSizeof, CsrBtBppcAuthenticateResSer, CsrBtBppcAuthenticateResDes, CsrBtBppcAuthenticateResSerFree },
#endif /* EXCLUDE_CSR_BT_BPPC_AUTHENTICATE_RES */
#ifndef EXCLUDE_CSR_BT_BPPC_CANCEL_CONNECT_REQ
    { CSR_BT_BPPC_CANCEL_CONNECT_REQ, CsrBtBppcCancelConnectReqSizeof, CsrBtBppcCancelConnectReqSer, CsrBtBppcCancelConnectReqDes, CsrBtBppcCancelConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_BPPC_CANCEL_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_BPPC_CANCEL_JOB_CFM
    { CSR_BT_BPPC_CANCEL_JOB_CFM, CsrBtBppcCancelJobCfmSizeof, CsrBtBppcCancelJobCfmSer, CsrBtBppcCancelJobCfmDes, CsrBtBppcCancelJobCfmSerFree },
#endif /* EXCLUDE_CSR_BT_BPPC_CANCEL_JOB_CFM */
#ifndef EXCLUDE_CSR_BT_BPPC_CANCEL_JOB_REQ
    { CSR_BT_BPPC_CANCEL_JOB_REQ, CsrBtBppcCancelJobReqSizeof, CsrBtBppcCancelJobReqSer, CsrBtBppcCancelJobReqDes, CsrBtBppcCancelJobReqSerFree },
#endif /* EXCLUDE_CSR_BT_BPPC_CANCEL_JOB_REQ */
#ifndef EXCLUDE_CSR_BT_BPPC_CONNECT_CFM
    { CSR_BT_BPPC_CONNECT_CFM, CsrBtBppcConnectCfmSizeof, CsrBtBppcConnectCfmSer, CsrBtBppcConnectCfmDes, CsrBtBppcConnectCfmSerFree },
#endif /* EXCLUDE_CSR_BT_BPPC_CONNECT_CFM */
#ifndef EXCLUDE_CSR_BT_BPPC_CONNECT_IND
    { CSR_BT_BPPC_CONNECT_IND, CsrBtBppcConnectIndSizeof, CsrBtBppcConnectIndSer, CsrBtBppcConnectIndDes, CsrBtBppcConnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_BPPC_CONNECT_IND */
#ifndef EXCLUDE_CSR_BT_BPPC_CONNECT_REQ
    { CSR_BT_BPPC_CONNECT_REQ, CsrBtBppcConnectReqSizeof, CsrBtBppcConnectReqSer, CsrBtBppcConnectReqDes, CsrBtBppcConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_BPPC_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_BPPC_CONNECT_RES
    { CSR_BT_BPPC_CONNECT_RES, CsrBtBppcConnectResSizeof, CsrBtBppcConnectResSer, CsrBtBppcConnectResDes, CsrBtBppcConnectResSerFree },
#endif /* EXCLUDE_CSR_BT_BPPC_CONNECT_RES */
#ifndef EXCLUDE_CSR_BT_BPPC_CREATE_JOB_CFM
    { CSR_BT_BPPC_CREATE_JOB_CFM, CsrBtBppcCreateJobCfmSizeof, CsrBtBppcCreateJobCfmSer, CsrBtBppcCreateJobCfmDes, CsrBtBppcCreateJobCfmSerFree },
#endif /* EXCLUDE_CSR_BT_BPPC_CREATE_JOB_CFM */
#ifndef EXCLUDE_CSR_BT_BPPC_CREATE_JOB_REQ
    { CSR_BT_BPPC_CREATE_JOB_REQ, CsrBtBppcCreateJobReqSizeof, CsrBtBppcCreateJobReqSer, CsrBtBppcCreateJobReqDes, CsrBtBppcCreateJobReqSerFree },
#endif /* EXCLUDE_CSR_BT_BPPC_CREATE_JOB_REQ */
#ifndef EXCLUDE_CSR_BT_BPPC_DEACTIVATE_CFM
    { CSR_BT_BPPC_DEACTIVATE_CFM, CsrBtBppcDeactivateCfmSizeof, CsrBtBppcDeactivateCfmSer, CsrBtBppcDeactivateCfmDes, CsrBtBppcDeactivateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_BPPC_DEACTIVATE_CFM */
#ifndef EXCLUDE_CSR_BT_BPPC_DEACTIVATE_REQ
    { CSR_BT_BPPC_DEACTIVATE_REQ, CsrBtBppcDeactivateReqSizeof, CsrBtBppcDeactivateReqSer, CsrBtBppcDeactivateReqDes, CsrBtBppcDeactivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_BPPC_DEACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_BPPC_DISCONNECT_IND
    { CSR_BT_BPPC_DISCONNECT_IND, CsrBtBppcDisconnectIndSizeof, CsrBtBppcDisconnectIndSer, CsrBtBppcDisconnectIndDes, CsrBtBppcDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_BPPC_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_BT_BPPC_DISCONNECT_REQ
    { CSR_BT_BPPC_DISCONNECT_REQ, CsrBtBppcDisconnectReqSizeof, CsrBtBppcDisconnectReqSer, CsrBtBppcDisconnectReqDes, CsrBtBppcDisconnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_BPPC_DISCONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_BPPC_GET_EVENT_IND
    { CSR_BT_BPPC_GET_EVENT_IND, CsrBtBppcGetEventIndSizeof, CsrBtBppcGetEventIndSer, CsrBtBppcGetEventIndDes, CsrBtBppcGetEventIndSerFree },
#endif /* EXCLUDE_CSR_BT_BPPC_GET_EVENT_IND */
#ifndef EXCLUDE_CSR_BT_BPPC_GET_EVENT_REQ
    { CSR_BT_BPPC_GET_EVENT_REQ, CsrBtBppcGetEventReqSizeof, CsrBtBppcGetEventReqSer, CsrBtBppcGetEventReqDes, CsrBtBppcGetEventReqSerFree },
#endif /* EXCLUDE_CSR_BT_BPPC_GET_EVENT_REQ */
#ifndef EXCLUDE_CSR_BT_BPPC_GET_EVENT_RES
    { CSR_BT_BPPC_GET_EVENT_RES, CsrBtBppcGetEventResSizeof, CsrBtBppcGetEventResSer, CsrBtBppcGetEventResDes, CsrBtBppcGetEventResSerFree },
#endif /* EXCLUDE_CSR_BT_BPPC_GET_EVENT_RES */
#ifndef EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_CFM
    { CSR_BT_BPPC_GET_JOB_ATTRIBUTES_CFM, CsrBtBppcGetJobAttributesCfmSizeof, CsrBtBppcGetJobAttributesCfmSer, CsrBtBppcGetJobAttributesCfmDes, CsrBtBppcGetJobAttributesCfmSerFree },
#endif /* EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_CFM */
#ifndef EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_IND
    { CSR_BT_BPPC_GET_JOB_ATTRIBUTES_IND, CsrBtBppcGetJobAttributesIndSizeof, CsrBtBppcGetJobAttributesIndSer, CsrBtBppcGetJobAttributesIndDes, CsrBtBppcGetJobAttributesIndSerFree },
#endif /* EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_IND */
#ifndef EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_REQ
    { CSR_BT_BPPC_GET_JOB_ATTRIBUTES_REQ, CsrBtBppcGetJobAttributesReqSizeof, CsrBtBppcGetJobAttributesReqSer, CsrBtBppcGetJobAttributesReqDes, CsrBtBppcGetJobAttributesReqSerFree },
#endif /* EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_REQ */
#ifndef EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_RES
    { CSR_BT_BPPC_GET_JOB_ATTRIBUTES_RES, CsrBtBppcGetJobAttributesResSizeof, CsrBtBppcGetJobAttributesResSer, CsrBtBppcGetJobAttributesResDes, CsrBtBppcGetJobAttributesResSerFree },
#endif /* EXCLUDE_CSR_BT_BPPC_GET_JOB_ATTRIBUTES_RES */
#ifndef EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_CFM
    { CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_CFM, CsrBtBppcGetPrinterAttributesCfmSizeof, CsrBtBppcGetPrinterAttributesCfmSer, CsrBtBppcGetPrinterAttributesCfmDes, CsrBtBppcGetPrinterAttributesCfmSerFree },
#endif /* EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_CFM */
#ifndef EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_IND
    { CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_IND, CsrBtBppcGetPrinterAttributesIndSizeof, CsrBtBppcGetPrinterAttributesIndSer, CsrBtBppcGetPrinterAttributesIndDes, CsrBtBppcGetPrinterAttributesIndSerFree },
#endif /* EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_IND */
#ifndef EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_REQ
    { CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_REQ, CsrBtBppcGetPrinterAttributesReqSizeof, CsrBtBppcGetPrinterAttributesReqSer, CsrBtBppcGetPrinterAttributesReqDes, CsrBtBppcGetPrinterAttributesReqSerFree },
#endif /* EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_REQ */
#ifndef EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_RES
    { CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_RES, CsrBtBppcGetPrinterAttributesResSizeof, CsrBtBppcGetPrinterAttributesResSer, CsrBtBppcGetPrinterAttributesResDes, CsrBtBppcGetPrinterAttributesResSerFree },
#endif /* EXCLUDE_CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_RES */
#ifndef EXCLUDE_CSR_BT_BPPC_GET_REFERENCE_OBJECT_IND
    { CSR_BT_BPPC_GET_REFERENCE_OBJECT_IND, CsrBtBppcGetReferenceObjectIndSizeof, CsrBtBppcGetReferenceObjectIndSer, CsrBtBppcGetReferenceObjectIndDes, CsrBtBppcGetReferenceObjectIndSerFree },
#endif /* EXCLUDE_CSR_BT_BPPC_GET_REFERENCE_OBJECT_IND */
#ifndef EXCLUDE_CSR_BT_BPPC_GET_REFERENCE_OBJECT_RES
    { CSR_BT_BPPC_GET_REFERENCE_OBJECT_RES, CsrBtBppcGetReferenceObjectResSizeof, CsrBtBppcGetReferenceObjectResSer, CsrBtBppcGetReferenceObjectResDes, CsrBtBppcGetReferenceObjectResSerFree },
#endif /* EXCLUDE_CSR_BT_BPPC_GET_REFERENCE_OBJECT_RES */
#ifndef EXCLUDE_CSR_BT_BPPC_SECURITY_OUT_CFM
    { CSR_BT_BPPC_SECURITY_OUT_CFM, CsrBtBppcSecurityOutCfmSizeof, CsrBtBppcSecurityOutCfmSer, CsrBtBppcSecurityOutCfmDes, CsrBtBppcSecurityOutCfmSerFree },
#endif /* EXCLUDE_CSR_BT_BPPC_SECURITY_OUT_CFM */
#ifndef EXCLUDE_CSR_BT_BPPC_SECURITY_OUT_REQ
    { CSR_BT_BPPC_SECURITY_OUT_REQ, CsrBtBppcSecurityOutReqSizeof, CsrBtBppcSecurityOutReqSer, CsrBtBppcSecurityOutReqDes, CsrBtBppcSecurityOutReqSerFree },
#endif /* EXCLUDE_CSR_BT_BPPC_SECURITY_OUT_REQ */
#ifndef EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_CFM
    { CSR_BT_BPPC_SEND_DOCUMENT_CFM, CsrBtBppcSendDocumentCfmSizeof, CsrBtBppcSendDocumentCfmSer, CsrBtBppcSendDocumentCfmDes, CsrBtBppcSendDocumentCfmSerFree },
#endif /* EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_CFM */
#ifndef EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_IND
    { CSR_BT_BPPC_SEND_DOCUMENT_IND, CsrBtBppcSendDocumentIndSizeof, CsrBtBppcSendDocumentIndSer, CsrBtBppcSendDocumentIndDes, CsrBtBppcSendDocumentIndSerFree },
#endif /* EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_IND */
#ifndef EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_REQ
    { CSR_BT_BPPC_SEND_DOCUMENT_REQ, CsrBtBppcSendDocumentReqSizeof, CsrBtBppcSendDocumentReqSer, CsrBtBppcSendDocumentReqDes, CsrBtBppcSendDocumentReqSerFree },
#endif /* EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_REQ */
#ifndef EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_RES
    { CSR_BT_BPPC_SEND_DOCUMENT_RES, CsrBtBppcSendDocumentResSizeof, CsrBtBppcSendDocumentResSer, CsrBtBppcSendDocumentResDes, CsrBtBppcSendDocumentResSerFree },
#endif /* EXCLUDE_CSR_BT_BPPC_SEND_DOCUMENT_RES */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtBppcConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_BPPC_PRIM, csr_bt_bppc_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_bppc_conv_info = {
    CSR_BT_BPPC_PRIM,
    "CSR_BT_BPPC_PRIM",
    csr_bt_bppc_conv_lut
};

CsrLogPrimitiveInformation* CsrBtBppcTechInfoGet(void)
{
    return &csr_bt_bppc_conv_info;
}
#endif
#endif

