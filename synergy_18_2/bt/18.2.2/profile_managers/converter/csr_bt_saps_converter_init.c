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
#include "csr_bt_saps_converter_init.h"
#ifndef EXCLUDE_CSR_BT_SAPS_MODULE
#include "csr_bt_saps_serialize.h"
#include "csr_bt_saps_prim.h"

static CsrMsgConvMsgEntry csr_bt_saps_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_SAPS_ACTIVATE_CFM
    { CSR_BT_SAPS_ACTIVATE_CFM, CsrBtSapsActivateCfmSizeof, CsrBtSapsActivateCfmSer, CsrBtSapsActivateCfmDes, CsrBtSapsActivateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SAPS_ACTIVATE_CFM */
#ifndef EXCLUDE_CSR_BT_SAPS_ACTIVATE_REQ
    { CSR_BT_SAPS_ACTIVATE_REQ, CsrBtSapsActivateReqSizeof, CsrBtSapsActivateReqSer, CsrBtSapsActivateReqDes, CsrBtSapsActivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_SAPS_ACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_SAPS_CONNECT_IND
    { CSR_BT_SAPS_CONNECT_IND, CsrBtSapsConnectIndSizeof, CsrBtSapsConnectIndSer, CsrBtSapsConnectIndDes, CsrBtSapsConnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_SAPS_CONNECT_IND */
#ifndef EXCLUDE_CSR_BT_SAPS_CONNECT_RES
    { CSR_BT_SAPS_CONNECT_RES, CsrBtSapsConnectResSizeof, CsrBtSapsConnectResSer, CsrBtSapsConnectResDes, CsrBtSapsConnectResSerFree },
#endif /* EXCLUDE_CSR_BT_SAPS_CONNECT_RES */
#ifndef EXCLUDE_CSR_BT_SAPS_DEACTIVATE_CFM
    { CSR_BT_SAPS_DEACTIVATE_CFM, CsrBtSapsDeactivateCfmSizeof, CsrBtSapsDeactivateCfmSer, CsrBtSapsDeactivateCfmDes, CsrBtSapsDeactivateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SAPS_DEACTIVATE_CFM */
#ifndef EXCLUDE_CSR_BT_SAPS_DEACTIVATE_REQ
    { CSR_BT_SAPS_DEACTIVATE_REQ, CsrBtSapsDeactivateReqSizeof, CsrBtSapsDeactivateReqSer, CsrBtSapsDeactivateReqDes, CsrBtSapsDeactivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_SAPS_DEACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_SAPS_DISCONNECT_IND
    { CSR_BT_SAPS_DISCONNECT_IND, CsrBtSapsDisconnectIndSizeof, CsrBtSapsDisconnectIndSer, CsrBtSapsDisconnectIndDes, CsrBtSapsDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_SAPS_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_BT_SAPS_DISCONNECT_REQ
    { CSR_BT_SAPS_DISCONNECT_REQ, CsrBtSapsDisconnectReqSizeof, CsrBtSapsDisconnectReqSer, CsrBtSapsDisconnectReqDes, CsrBtSapsDisconnectReqSerFree },
#endif /* EXCLUDE_CSR_BT_SAPS_DISCONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_SAPS_HOUSE_CLEANING
    { CSR_BT_SAPS_HOUSE_CLEANING, CsrBtSapsHouseCleaningSizeof, CsrBtSapsHouseCleaningSer, CsrBtSapsHouseCleaningDes, CsrBtSapsHouseCleaningSerFree },
#endif /* EXCLUDE_CSR_BT_SAPS_HOUSE_CLEANING */
#ifndef EXCLUDE_CSR_BT_SAPS_POWER_SIM_OFF_IND
    { CSR_BT_SAPS_POWER_SIM_OFF_IND, CsrBtSapsPowerSimOffIndSizeof, CsrBtSapsPowerSimOffIndSer, CsrBtSapsPowerSimOffIndDes, CsrBtSapsPowerSimOffIndSerFree },
#endif /* EXCLUDE_CSR_BT_SAPS_POWER_SIM_OFF_IND */
#ifndef EXCLUDE_CSR_BT_SAPS_POWER_SIM_OFF_RES
    { CSR_BT_SAPS_POWER_SIM_OFF_RES, CsrBtSapsPowerSimOffResSizeof, CsrBtSapsPowerSimOffResSer, CsrBtSapsPowerSimOffResDes, CsrBtSapsPowerSimOffResSerFree },
#endif /* EXCLUDE_CSR_BT_SAPS_POWER_SIM_OFF_RES */
#ifndef EXCLUDE_CSR_BT_SAPS_POWER_SIM_ON_IND
    { CSR_BT_SAPS_POWER_SIM_ON_IND, CsrBtSapsPowerSimOnIndSizeof, CsrBtSapsPowerSimOnIndSer, CsrBtSapsPowerSimOnIndDes, CsrBtSapsPowerSimOnIndSerFree },
#endif /* EXCLUDE_CSR_BT_SAPS_POWER_SIM_ON_IND */
#ifndef EXCLUDE_CSR_BT_SAPS_POWER_SIM_ON_RES
    { CSR_BT_SAPS_POWER_SIM_ON_RES, CsrBtSapsPowerSimOnResSizeof, CsrBtSapsPowerSimOnResSer, CsrBtSapsPowerSimOnResDes, CsrBtSapsPowerSimOnResSerFree },
#endif /* EXCLUDE_CSR_BT_SAPS_POWER_SIM_ON_RES */
#ifndef EXCLUDE_CSR_BT_SAPS_RESET_SIM_IND
    { CSR_BT_SAPS_RESET_SIM_IND, CsrBtSapsResetSimIndSizeof, CsrBtSapsResetSimIndSer, CsrBtSapsResetSimIndDes, CsrBtSapsResetSimIndSerFree },
#endif /* EXCLUDE_CSR_BT_SAPS_RESET_SIM_IND */
#ifndef EXCLUDE_CSR_BT_SAPS_RESET_SIM_RES
    { CSR_BT_SAPS_RESET_SIM_RES, CsrBtSapsResetSimResSizeof, CsrBtSapsResetSimResSer, CsrBtSapsResetSimResDes, CsrBtSapsResetSimResSerFree },
#endif /* EXCLUDE_CSR_BT_SAPS_RESET_SIM_RES */
#ifndef EXCLUDE_CSR_BT_SAPS_SECURITY_IN_CFM
    { CSR_BT_SAPS_SECURITY_IN_CFM, CsrBtSapsSecurityInCfmSizeof, CsrBtSapsSecurityInCfmSer, CsrBtSapsSecurityInCfmDes, CsrBtSapsSecurityInCfmSerFree },
#endif /* EXCLUDE_CSR_BT_SAPS_SECURITY_IN_CFM */
#ifndef EXCLUDE_CSR_BT_SAPS_SECURITY_IN_REQ
    { CSR_BT_SAPS_SECURITY_IN_REQ, CsrBtSapsSecurityInReqSizeof, CsrBtSapsSecurityInReqSer, CsrBtSapsSecurityInReqDes, CsrBtSapsSecurityInReqSerFree },
#endif /* EXCLUDE_CSR_BT_SAPS_SECURITY_IN_REQ */
#ifndef EXCLUDE_CSR_BT_SAPS_SEND_STATUS_REQ
    { CSR_BT_SAPS_SEND_STATUS_REQ, CsrBtSapsSendStatusReqSizeof, CsrBtSapsSendStatusReqSer, CsrBtSapsSendStatusReqDes, CsrBtSapsSendStatusReqSerFree },
#endif /* EXCLUDE_CSR_BT_SAPS_SEND_STATUS_REQ */
#ifndef EXCLUDE_CSR_BT_SAPS_SET_TRANSFER_PROTOCOL_IND
    { CSR_BT_SAPS_SET_TRANSFER_PROTOCOL_IND, CsrBtSapsSetTransferProtocolIndSizeof, CsrBtSapsSetTransferProtocolIndSer, CsrBtSapsSetTransferProtocolIndDes, CsrBtSapsSetTransferProtocolIndSerFree },
#endif /* EXCLUDE_CSR_BT_SAPS_SET_TRANSFER_PROTOCOL_IND */
#ifndef EXCLUDE_CSR_BT_SAPS_SET_TRANSFER_PROTOCOL_RES
    { CSR_BT_SAPS_SET_TRANSFER_PROTOCOL_RES, CsrBtSapsSetTransferProtocolResSizeof, CsrBtSapsSetTransferProtocolResSer, CsrBtSapsSetTransferProtocolResDes, CsrBtSapsSetTransferProtocolResSerFree },
#endif /* EXCLUDE_CSR_BT_SAPS_SET_TRANSFER_PROTOCOL_RES */
#ifndef EXCLUDE_CSR_BT_SAPS_TRANSFER_APDU_IND
    { CSR_BT_SAPS_TRANSFER_APDU_IND, CsrBtSapsTransferApduIndSizeof, CsrBtSapsTransferApduIndSer, CsrBtSapsTransferApduIndDes, CsrBtSapsTransferApduIndSerFree },
#endif /* EXCLUDE_CSR_BT_SAPS_TRANSFER_APDU_IND */
#ifndef EXCLUDE_CSR_BT_SAPS_TRANSFER_APDU_RES
    { CSR_BT_SAPS_TRANSFER_APDU_RES, CsrBtSapsTransferApduResSizeof, CsrBtSapsTransferApduResSer, CsrBtSapsTransferApduResDes, CsrBtSapsTransferApduResSerFree },
#endif /* EXCLUDE_CSR_BT_SAPS_TRANSFER_APDU_RES */
#ifndef EXCLUDE_CSR_BT_SAPS_TRANSFER_ATR_IND
    { CSR_BT_SAPS_TRANSFER_ATR_IND, CsrBtSapsTransferAtrIndSizeof, CsrBtSapsTransferAtrIndSer, CsrBtSapsTransferAtrIndDes, CsrBtSapsTransferAtrIndSerFree },
#endif /* EXCLUDE_CSR_BT_SAPS_TRANSFER_ATR_IND */
#ifndef EXCLUDE_CSR_BT_SAPS_TRANSFER_ATR_RES
    { CSR_BT_SAPS_TRANSFER_ATR_RES, CsrBtSapsTransferAtrResSizeof, CsrBtSapsTransferAtrResSer, CsrBtSapsTransferAtrResDes, CsrBtSapsTransferAtrResSerFree },
#endif /* EXCLUDE_CSR_BT_SAPS_TRANSFER_ATR_RES */
#ifndef EXCLUDE_CSR_BT_SAPS_TRANSFER_CARD_READER_STATUS_IND
    { CSR_BT_SAPS_TRANSFER_CARD_READER_STATUS_IND, CsrBtSapsTransferCardReaderStatusIndSizeof, CsrBtSapsTransferCardReaderStatusIndSer, CsrBtSapsTransferCardReaderStatusIndDes, CsrBtSapsTransferCardReaderStatusIndSerFree },
#endif /* EXCLUDE_CSR_BT_SAPS_TRANSFER_CARD_READER_STATUS_IND */
#ifndef EXCLUDE_CSR_BT_SAPS_TRANSFER_CARD_READER_STATUS_RES
    { CSR_BT_SAPS_TRANSFER_CARD_READER_STATUS_RES, CsrBtSapsTransferCardReaderStatusResSizeof, CsrBtSapsTransferCardReaderStatusResSer, CsrBtSapsTransferCardReaderStatusResDes, CsrBtSapsTransferCardReaderStatusResSerFree },
#endif /* EXCLUDE_CSR_BT_SAPS_TRANSFER_CARD_READER_STATUS_RES */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtSapsConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_SAPS_PRIM, csr_bt_saps_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_saps_conv_info = {
    CSR_BT_SAPS_PRIM,
    "CSR_BT_SAPS_PRIM",
    csr_bt_saps_conv_lut
};

CsrLogPrimitiveInformation* CsrBtSapsTechInfoGet(void)
{
    return &csr_bt_saps_conv_info;
}
#endif
#endif

