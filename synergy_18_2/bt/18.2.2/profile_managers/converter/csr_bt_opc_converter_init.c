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
#include "csr_bt_opc_converter_init.h"
#ifndef EXCLUDE_CSR_BT_OPC_MODULE
#include "csr_bt_opc_serialize.h"
#include "csr_bt_opc_prim.h"

static CsrMsgConvMsgEntry csr_bt_opc_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_OPC_ABORT_CFM
    { CSR_BT_OPC_ABORT_CFM, CsrBtOpcAbortCfmSizeof, CsrBtOpcAbortCfmSer, CsrBtOpcAbortCfmDes, CsrBtOpcAbortCfmSerFree },
#endif /* EXCLUDE_CSR_BT_OPC_ABORT_CFM */
#ifndef EXCLUDE_CSR_BT_OPC_ABORT_REQ
    { CSR_BT_OPC_ABORT_REQ, CsrBtOpcAbortReqSizeof, CsrBtOpcAbortReqSer, CsrBtOpcAbortReqDes, CsrBtOpcAbortReqSerFree },
#endif /* EXCLUDE_CSR_BT_OPC_ABORT_REQ */
#ifndef EXCLUDE_CSR_BT_OPC_AUTHENTICATE_IND
    { CSR_BT_OPC_AUTHENTICATE_IND, CsrBtOpcAuthenticateIndSizeof, CsrBtOpcAuthenticateIndSer, CsrBtOpcAuthenticateIndDes, CsrBtOpcAuthenticateIndSerFree },
#endif /* EXCLUDE_CSR_BT_OPC_AUTHENTICATE_IND */
#ifndef EXCLUDE_CSR_BT_OPC_AUTHENTICATE_RES
    { CSR_BT_OPC_AUTHENTICATE_RES, CsrBtOpcAuthenticateResSizeof, CsrBtOpcAuthenticateResSer, CsrBtOpcAuthenticateResDes, CsrBtOpcAuthenticateResSerFree },
#endif /* EXCLUDE_CSR_BT_OPC_AUTHENTICATE_RES */
#ifndef EXCLUDE_CSR_BT_OPC_CANCEL_CONNECT_REQ
    { CSR_BT_OPC_CANCEL_CONNECT_REQ, CsrBtOpcCancelConnectReqSizeof, CsrBtOpcCancelConnectReqSer, CsrBtOpcCancelConnectReqDes, CsrBtOpcCancelConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_OPC_CANCEL_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_OPC_CONNECT_CFM
    { CSR_BT_OPC_CONNECT_CFM, CsrBtOpcConnectCfmSizeof, CsrBtOpcConnectCfmSer, CsrBtOpcConnectCfmDes, CsrBtOpcConnectCfmSerFree },
#endif /* EXCLUDE_CSR_BT_OPC_CONNECT_CFM */
#ifndef EXCLUDE_CSR_BT_OPC_CONNECT_REQ
    { CSR_BT_OPC_CONNECT_REQ, CsrBtOpcConnectReqSizeof, CsrBtOpcConnectReqSer, CsrBtOpcConnectReqDes, CsrBtOpcConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_OPC_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_OPC_DISCONNECT_IND
    { CSR_BT_OPC_DISCONNECT_IND, CsrBtOpcDisconnectIndSizeof, CsrBtOpcDisconnectIndSer, CsrBtOpcDisconnectIndDes, CsrBtOpcDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_OPC_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_BT_OPC_DISCONNECT_REQ
    { CSR_BT_OPC_DISCONNECT_REQ, CsrBtOpcDisconnectReqSizeof, CsrBtOpcDisconnectReqSer, CsrBtOpcDisconnectReqDes, CsrBtOpcDisconnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_OPC_DISCONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_OPC_GET_HEADER_CFM
    { CSR_BT_OPC_GET_HEADER_CFM, CsrBtOpcGetHeaderCfmSizeof, CsrBtOpcGetHeaderCfmSer, CsrBtOpcGetHeaderCfmDes, CsrBtOpcGetHeaderCfmSerFree },
#endif /* EXCLUDE_CSR_BT_OPC_GET_HEADER_CFM */
#ifndef EXCLUDE_CSR_BT_OPC_GET_HEADER_REQ
    { CSR_BT_OPC_GET_HEADER_REQ, CsrBtOpcGetHeaderReqSizeof, CsrBtOpcGetHeaderReqSer, CsrBtOpcGetHeaderReqDes, CsrBtOpcGetHeaderReqSerFree },
#endif /* EXCLUDE_CSR_BT_OPC_GET_HEADER_REQ */
#ifndef EXCLUDE_CSR_BT_OPC_GET_OBJECT_CFM
    { CSR_BT_OPC_GET_OBJECT_CFM, CsrBtOpcGetObjectCfmSizeof, CsrBtOpcGetObjectCfmSer, CsrBtOpcGetObjectCfmDes, CsrBtOpcGetObjectCfmSerFree },
#endif /* EXCLUDE_CSR_BT_OPC_GET_OBJECT_CFM */
#ifndef EXCLUDE_CSR_BT_OPC_GET_OBJECT_REQ
    { CSR_BT_OPC_GET_OBJECT_REQ, CsrBtOpcGetObjectReqSizeof, CsrBtOpcGetObjectReqSer, CsrBtOpcGetObjectReqDes, CsrBtOpcGetObjectReqSerFree },
#endif /* EXCLUDE_CSR_BT_OPC_GET_OBJECT_REQ */
#ifndef EXCLUDE_CSR_BT_OPC_PUT_CFM
    { CSR_BT_OPC_PUT_CFM, CsrBtOpcPutCfmSizeof, CsrBtOpcPutCfmSer, CsrBtOpcPutCfmDes, CsrBtOpcPutCfmSerFree },
#endif /* EXCLUDE_CSR_BT_OPC_PUT_CFM */
#ifndef EXCLUDE_CSR_BT_OPC_PUT_OBJECT_IND
    { CSR_BT_OPC_PUT_OBJECT_IND, CsrBtOpcPutObjectIndSizeof, CsrBtOpcPutObjectIndSer, CsrBtOpcPutObjectIndDes, CsrBtOpcPutObjectIndSerFree },
#endif /* EXCLUDE_CSR_BT_OPC_PUT_OBJECT_IND */
#ifndef EXCLUDE_CSR_BT_OPC_PUT_OBJECT_RES
    { CSR_BT_OPC_PUT_OBJECT_RES, CsrBtOpcPutObjectResSizeof, CsrBtOpcPutObjectResSer, CsrBtOpcPutObjectResDes, CsrBtOpcPutObjectResSerFree },
#endif /* EXCLUDE_CSR_BT_OPC_PUT_OBJECT_RES */
#ifndef EXCLUDE_CSR_BT_OPC_PUT_REQ
    { CSR_BT_OPC_PUT_REQ, CsrBtOpcPutReqSizeof, CsrBtOpcPutReqSer, CsrBtOpcPutReqDes, CsrBtOpcPutReqSerFree },
#endif /* EXCLUDE_CSR_BT_OPC_PUT_REQ */
#ifndef EXCLUDE_CSR_BT_OPC_SECURITY_OUT_CFM
    { CSR_BT_OPC_SECURITY_OUT_CFM, CsrBtOpcSecurityOutCfmSizeof, CsrBtOpcSecurityOutCfmSer, CsrBtOpcSecurityOutCfmDes, CsrBtOpcSecurityOutCfmSerFree },
#endif /* EXCLUDE_CSR_BT_OPC_SECURITY_OUT_CFM */
#ifndef EXCLUDE_CSR_BT_OPC_SECURITY_OUT_REQ
    { CSR_BT_OPC_SECURITY_OUT_REQ, CsrBtOpcSecurityOutReqSizeof, CsrBtOpcSecurityOutReqSer, CsrBtOpcSecurityOutReqDes, CsrBtOpcSecurityOutReqSerFree },
#endif /* EXCLUDE_CSR_BT_OPC_SECURITY_OUT_REQ */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtOpcConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_OPC_PRIM, csr_bt_opc_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_opc_conv_info = {
    CSR_BT_OPC_PRIM,
    "CSR_BT_OPC_PRIM",
    csr_bt_opc_conv_lut
};

CsrLogPrimitiveInformation* CsrBtOpcTechInfoGet(void)
{
    return &csr_bt_opc_conv_info;
}
#endif
#endif

