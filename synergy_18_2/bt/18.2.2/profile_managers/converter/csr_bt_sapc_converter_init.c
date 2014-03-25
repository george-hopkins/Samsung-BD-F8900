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
#include "csr_bt_sapc_converter_init.h"
#ifndef EXCLUDE_CSR_BT_SAPC_MODULE
#include "csr_bt_sapc_serialize.h"
#include "csr_bt_sapc_prim.h"

static CsrMsgConvMsgEntry csr_bt_sapc_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_SAPC_CONNECT_CFM
    { CSR_BT_SAPC_CONNECT_CFM, CsrBtSapcConnectCfmSizeof, CsrBtSapcConnectCfmSer, CsrBtSapcConnectCfmDes, CsrBtSapcConnectCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SAPC_CONNECT_CFM */
#ifndef EXCLUDE_CSR_BT_SAPC_CONNECT_REQ
    { CSR_BT_SAPC_CONNECT_REQ, CsrBtSapcConnectReqSizeof, CsrBtSapcConnectReqSer, CsrBtSapcConnectReqDes, CsrBtSapcConnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_SAPC_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_SAPC_DISCONNECT_IND
    { CSR_BT_SAPC_DISCONNECT_IND, CsrBtSapcDisconnectIndSizeof, CsrBtSapcDisconnectIndSer, CsrBtSapcDisconnectIndDes, CsrBtSapcDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_SAPC_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_BT_SAPC_DISCONNECT_REQ
    { CSR_BT_SAPC_DISCONNECT_REQ, CsrBtSapcDisconnectReqSizeof, CsrBtSapcDisconnectReqSer, CsrBtSapcDisconnectReqDes, CsrBtSapcDisconnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_SAPC_DISCONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_SAPC_HOUSE_CLEANING
    { CSR_BT_SAPC_HOUSE_CLEANING, CsrBtSapcHouseCleaningSizeof, CsrBtSapcHouseCleaningSer, CsrBtSapcHouseCleaningDes, CsrBtSapcHouseCleaningSerFree },
#endif /* EXCLUDE_CSR_BT_SAPC_HOUSE_CLEANING */
#ifndef EXCLUDE_CSR_BT_SAPC_POWER_SIM_OFF_CFM
    { CSR_BT_SAPC_POWER_SIM_OFF_CFM, CsrBtSapcPowerSimOffCfmSizeof, CsrBtSapcPowerSimOffCfmSer, CsrBtSapcPowerSimOffCfmDes, CsrBtSapcPowerSimOffCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SAPC_POWER_SIM_OFF_CFM */
#ifndef EXCLUDE_CSR_BT_SAPC_POWER_SIM_OFF_REQ
    { CSR_BT_SAPC_POWER_SIM_OFF_REQ, CsrBtSapcPowerSimOffReqSizeof, CsrBtSapcPowerSimOffReqSer, CsrBtSapcPowerSimOffReqDes, CsrBtSapcPowerSimOffReqSerFree },
#endif /* EXCLUDE_CSR_BT_SAPC_POWER_SIM_OFF_REQ */
#ifndef EXCLUDE_CSR_BT_SAPC_POWER_SIM_ON_CFM
    { CSR_BT_SAPC_POWER_SIM_ON_CFM, CsrBtSapcPowerSimOnCfmSizeof, CsrBtSapcPowerSimOnCfmSer, CsrBtSapcPowerSimOnCfmDes, CsrBtSapcPowerSimOnCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SAPC_POWER_SIM_ON_CFM */
#ifndef EXCLUDE_CSR_BT_SAPC_POWER_SIM_ON_REQ
    { CSR_BT_SAPC_POWER_SIM_ON_REQ, CsrBtSapcPowerSimOnReqSizeof, CsrBtSapcPowerSimOnReqSer, CsrBtSapcPowerSimOnReqDes, CsrBtSapcPowerSimOnReqSerFree },
#endif /* EXCLUDE_CSR_BT_SAPC_POWER_SIM_ON_REQ */
#ifndef EXCLUDE_CSR_BT_SAPC_RESET_SIM_CFM
    { CSR_BT_SAPC_RESET_SIM_CFM, CsrBtSapcResetSimCfmSizeof, CsrBtSapcResetSimCfmSer, CsrBtSapcResetSimCfmDes, CsrBtSapcResetSimCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SAPC_RESET_SIM_CFM */
#ifndef EXCLUDE_CSR_BT_SAPC_RESET_SIM_REQ
    { CSR_BT_SAPC_RESET_SIM_REQ, CsrBtSapcResetSimReqSizeof, CsrBtSapcResetSimReqSer, CsrBtSapcResetSimReqDes, CsrBtSapcResetSimReqSerFree },
#endif /* EXCLUDE_CSR_BT_SAPC_RESET_SIM_REQ */
#ifndef EXCLUDE_CSR_BT_SAPC_SECURITY_OUT_CFM
    { CSR_BT_SAPC_SECURITY_OUT_CFM, CsrBtSapcSecurityOutCfmSizeof, CsrBtSapcSecurityOutCfmSer, CsrBtSapcSecurityOutCfmDes, CsrBtSapcSecurityOutCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SAPC_SECURITY_OUT_CFM */
#ifndef EXCLUDE_CSR_BT_SAPC_SECURITY_OUT_REQ
    { CSR_BT_SAPC_SECURITY_OUT_REQ, CsrBtSapcSecurityOutReqSizeof, CsrBtSapcSecurityOutReqSer, CsrBtSapcSecurityOutReqDes, CsrBtSapcSecurityOutReqSerFree },
#endif /* EXCLUDE_CSR_BT_SAPC_SECURITY_OUT_REQ */
#ifndef EXCLUDE_CSR_BT_SAPC_SET_TRANSFER_PROTOCOL_CFM
    { CSR_BT_SAPC_SET_TRANSFER_PROTOCOL_CFM, CsrBtSapcSetTransferProtocolCfmSizeof, CsrBtSapcSetTransferProtocolCfmSer, CsrBtSapcSetTransferProtocolCfmDes, CsrBtSapcSetTransferProtocolCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SAPC_SET_TRANSFER_PROTOCOL_CFM */
#ifndef EXCLUDE_CSR_BT_SAPC_SET_TRANSFER_PROTOCOL_REQ
    { CSR_BT_SAPC_SET_TRANSFER_PROTOCOL_REQ, CsrBtSapcSetTransferProtocolReqSizeof, CsrBtSapcSetTransferProtocolReqSer, CsrBtSapcSetTransferProtocolReqDes, CsrBtSapcSetTransferProtocolReqSerFree },
#endif /* EXCLUDE_CSR_BT_SAPC_SET_TRANSFER_PROTOCOL_REQ */
#ifndef EXCLUDE_CSR_BT_SAPC_STATUS_IND
    { CSR_BT_SAPC_STATUS_IND, CsrBtSapcStatusIndSizeof, CsrBtSapcStatusIndSer, CsrBtSapcStatusIndDes, CsrBtSapcStatusIndSerFree },
#endif /* EXCLUDE_CSR_BT_SAPC_STATUS_IND */
#ifndef EXCLUDE_CSR_BT_SAPC_TRANSFER_APDU_CFM
    { CSR_BT_SAPC_TRANSFER_APDU_CFM, CsrBtSapcTransferApduCfmSizeof, CsrBtSapcTransferApduCfmSer, CsrBtSapcTransferApduCfmDes, CsrBtSapcTransferApduCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SAPC_TRANSFER_APDU_CFM */
#ifndef EXCLUDE_CSR_BT_SAPC_TRANSFER_APDU_REQ
    { CSR_BT_SAPC_TRANSFER_APDU_REQ, CsrBtSapcTransferApduReqSizeof, CsrBtSapcTransferApduReqSer, CsrBtSapcTransferApduReqDes, CsrBtSapcTransferApduReqSerFree },
#endif /* EXCLUDE_CSR_BT_SAPC_TRANSFER_APDU_REQ */
#ifndef EXCLUDE_CSR_BT_SAPC_TRANSFER_ATR_CFM
    { CSR_BT_SAPC_TRANSFER_ATR_CFM, CsrBtSapcTransferAtrCfmSizeof, CsrBtSapcTransferAtrCfmSer, CsrBtSapcTransferAtrCfmDes, CsrBtSapcTransferAtrCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SAPC_TRANSFER_ATR_CFM */
#ifndef EXCLUDE_CSR_BT_SAPC_TRANSFER_ATR_REQ
    { CSR_BT_SAPC_TRANSFER_ATR_REQ, CsrBtSapcTransferAtrReqSizeof, CsrBtSapcTransferAtrReqSer, CsrBtSapcTransferAtrReqDes, CsrBtSapcTransferAtrReqSerFree },
#endif /* EXCLUDE_CSR_BT_SAPC_TRANSFER_ATR_REQ */
#ifndef EXCLUDE_CSR_BT_SAPC_TRANSFER_CARD_READER_STATUS_CFM
    { CSR_BT_SAPC_TRANSFER_CARD_READER_STATUS_CFM, CsrBtSapcTransferCardReaderStatusCfmSizeof, CsrBtSapcTransferCardReaderStatusCfmSer, CsrBtSapcTransferCardReaderStatusCfmDes, CsrBtSapcTransferCardReaderStatusCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SAPC_TRANSFER_CARD_READER_STATUS_CFM */
#ifndef EXCLUDE_CSR_BT_SAPC_TRANSFER_CARD_READER_STATUS_REQ
    { CSR_BT_SAPC_TRANSFER_CARD_READER_STATUS_REQ, CsrBtSapcTransferCardReaderStatusReqSizeof, CsrBtSapcTransferCardReaderStatusReqSer, CsrBtSapcTransferCardReaderStatusReqDes, CsrBtSapcTransferCardReaderStatusReqSerFree },
#endif /* EXCLUDE_CSR_BT_SAPC_TRANSFER_CARD_READER_STATUS_REQ */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtSapcConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_SAPC_PRIM, csr_bt_sapc_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_sapc_conv_info = {
    CSR_BT_SAPC_PRIM,
    "CSR_BT_SAPC_PRIM",
    csr_bt_sapc_conv_lut
};

CsrLogPrimitiveInformation* CsrBtSapcTechInfoGet(void)
{
    return &csr_bt_sapc_conv_info;
}
#endif
#endif

