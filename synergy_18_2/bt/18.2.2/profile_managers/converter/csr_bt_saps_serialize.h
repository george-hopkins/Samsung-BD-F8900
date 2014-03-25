#ifndef CSR_BT_SAPS_SERIALIZE_H__
#define CSR_BT_SAPS_SERIALIZE_H__
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

void CsrBtSapsPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_SAPS_POWER_SIM_ON_RES
CsrSize CsrBtSapsPowerSimOnResSizeof(void *msg);
CsrUint8 *CsrBtSapsPowerSimOnResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapsPowerSimOnResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSapsPowerSimOnResSerFree CsrBtSapsPfree
#endif /* EXCLUDE_CSR_BT_SAPS_POWER_SIM_ON_RES */

#ifndef EXCLUDE_CSR_BT_SAPS_POWER_SIM_OFF_IND
CsrSize CsrBtSapsPowerSimOffIndSizeof(void *msg);
CsrUint8 *CsrBtSapsPowerSimOffIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapsPowerSimOffIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSapsPowerSimOffIndSerFree CsrBtSapsPfree
#endif /* EXCLUDE_CSR_BT_SAPS_POWER_SIM_OFF_IND */

#ifndef EXCLUDE_CSR_BT_SAPS_SECURITY_IN_CFM
CsrSize CsrBtSapsSecurityInCfmSizeof(void *msg);
CsrUint8 *CsrBtSapsSecurityInCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapsSecurityInCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSapsSecurityInCfmSerFree CsrBtSapsPfree
#endif /* EXCLUDE_CSR_BT_SAPS_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_SAPS_TRANSFER_CARD_READER_STATUS_IND
CsrSize CsrBtSapsTransferCardReaderStatusIndSizeof(void *msg);
CsrUint8 *CsrBtSapsTransferCardReaderStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapsTransferCardReaderStatusIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSapsTransferCardReaderStatusIndSerFree CsrBtSapsPfree
#endif /* EXCLUDE_CSR_BT_SAPS_TRANSFER_CARD_READER_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_SAPS_SEND_STATUS_REQ
CsrSize CsrBtSapsSendStatusReqSizeof(void *msg);
CsrUint8 *CsrBtSapsSendStatusReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapsSendStatusReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSapsSendStatusReqSerFree CsrBtSapsPfree
#endif /* EXCLUDE_CSR_BT_SAPS_SEND_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_SAPS_ACTIVATE_CFM
CsrSize CsrBtSapsActivateCfmSizeof(void *msg);
CsrUint8 *CsrBtSapsActivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapsActivateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSapsActivateCfmSerFree CsrBtSapsPfree
#endif /* EXCLUDE_CSR_BT_SAPS_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_SAPS_RESET_SIM_RES
CsrSize CsrBtSapsResetSimResSizeof(void *msg);
CsrUint8 *CsrBtSapsResetSimResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapsResetSimResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSapsResetSimResSerFree CsrBtSapsPfree
#endif /* EXCLUDE_CSR_BT_SAPS_RESET_SIM_RES */

#ifndef EXCLUDE_CSR_BT_SAPS_CONNECT_RES
CsrSize CsrBtSapsConnectResSizeof(void *msg);
CsrUint8 *CsrBtSapsConnectResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapsConnectResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSapsConnectResSerFree CsrBtSapsPfree
#endif /* EXCLUDE_CSR_BT_SAPS_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_SAPS_TRANSFER_CARD_READER_STATUS_RES
CsrSize CsrBtSapsTransferCardReaderStatusResSizeof(void *msg);
CsrUint8 *CsrBtSapsTransferCardReaderStatusResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapsTransferCardReaderStatusResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSapsTransferCardReaderStatusResSerFree CsrBtSapsPfree
#endif /* EXCLUDE_CSR_BT_SAPS_TRANSFER_CARD_READER_STATUS_RES */

#ifndef EXCLUDE_CSR_BT_SAPS_POWER_SIM_ON_IND
CsrSize CsrBtSapsPowerSimOnIndSizeof(void *msg);
CsrUint8 *CsrBtSapsPowerSimOnIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapsPowerSimOnIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSapsPowerSimOnIndSerFree CsrBtSapsPfree
#endif /* EXCLUDE_CSR_BT_SAPS_POWER_SIM_ON_IND */

#ifndef EXCLUDE_CSR_BT_SAPS_TRANSFER_ATR_RES
CsrSize CsrBtSapsTransferAtrResSizeof(void *msg);
CsrUint8 *CsrBtSapsTransferAtrResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapsTransferAtrResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSapsTransferAtrResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SAPS_TRANSFER_ATR_RES */

#ifndef EXCLUDE_CSR_BT_SAPS_TRANSFER_ATR_IND
CsrSize CsrBtSapsTransferAtrIndSizeof(void *msg);
CsrUint8 *CsrBtSapsTransferAtrIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapsTransferAtrIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSapsTransferAtrIndSerFree CsrBtSapsPfree
#endif /* EXCLUDE_CSR_BT_SAPS_TRANSFER_ATR_IND */

#ifndef EXCLUDE_CSR_BT_SAPS_RESET_SIM_IND
CsrSize CsrBtSapsResetSimIndSizeof(void *msg);
CsrUint8 *CsrBtSapsResetSimIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapsResetSimIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSapsResetSimIndSerFree CsrBtSapsPfree
#endif /* EXCLUDE_CSR_BT_SAPS_RESET_SIM_IND */

#ifndef EXCLUDE_CSR_BT_SAPS_SET_TRANSFER_PROTOCOL_RES
CsrSize CsrBtSapsSetTransferProtocolResSizeof(void *msg);
CsrUint8 *CsrBtSapsSetTransferProtocolResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapsSetTransferProtocolResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSapsSetTransferProtocolResSerFree CsrBtSapsPfree
#endif /* EXCLUDE_CSR_BT_SAPS_SET_TRANSFER_PROTOCOL_RES */

#ifndef EXCLUDE_CSR_BT_SAPS_CONNECT_IND
CsrSize CsrBtSapsConnectIndSizeof(void *msg);
CsrUint8 *CsrBtSapsConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapsConnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSapsConnectIndSerFree CsrBtSapsPfree
#endif /* EXCLUDE_CSR_BT_SAPS_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SAPS_ACTIVATE_REQ
CsrSize CsrBtSapsActivateReqSizeof(void *msg);
CsrUint8 *CsrBtSapsActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapsActivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSapsActivateReqSerFree CsrBtSapsPfree
#endif /* EXCLUDE_CSR_BT_SAPS_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SAPS_SECURITY_IN_REQ
CsrSize CsrBtSapsSecurityInReqSizeof(void *msg);
CsrUint8 *CsrBtSapsSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapsSecurityInReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSapsSecurityInReqSerFree CsrBtSapsPfree
#endif /* EXCLUDE_CSR_BT_SAPS_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_SAPS_DISCONNECT_IND
CsrSize CsrBtSapsDisconnectIndSizeof(void *msg);
CsrUint8 *CsrBtSapsDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapsDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSapsDisconnectIndSerFree CsrBtSapsPfree
#endif /* EXCLUDE_CSR_BT_SAPS_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SAPS_TRANSFER_APDU_IND
CsrSize CsrBtSapsTransferApduIndSizeof(void *msg);
CsrUint8 *CsrBtSapsTransferApduIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapsTransferApduIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSapsTransferApduIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SAPS_TRANSFER_APDU_IND */

#ifndef EXCLUDE_CSR_BT_SAPS_DEACTIVATE_CFM
CsrSize CsrBtSapsDeactivateCfmSizeof(void *msg);
CsrUint8 *CsrBtSapsDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapsDeactivateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSapsDeactivateCfmSerFree CsrBtSapsPfree
#endif /* EXCLUDE_CSR_BT_SAPS_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_SAPS_DISCONNECT_REQ
CsrSize CsrBtSapsDisconnectReqSizeof(void *msg);
CsrUint8 *CsrBtSapsDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapsDisconnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSapsDisconnectReqSerFree CsrBtSapsPfree
#endif /* EXCLUDE_CSR_BT_SAPS_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SAPS_POWER_SIM_OFF_RES
CsrSize CsrBtSapsPowerSimOffResSizeof(void *msg);
CsrUint8 *CsrBtSapsPowerSimOffResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapsPowerSimOffResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSapsPowerSimOffResSerFree CsrBtSapsPfree
#endif /* EXCLUDE_CSR_BT_SAPS_POWER_SIM_OFF_RES */

#ifndef EXCLUDE_CSR_BT_SAPS_DEACTIVATE_REQ
CsrSize CsrBtSapsDeactivateReqSizeof(void *msg);
CsrUint8 *CsrBtSapsDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapsDeactivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSapsDeactivateReqSerFree CsrBtSapsPfree
#endif /* EXCLUDE_CSR_BT_SAPS_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SAPS_SET_TRANSFER_PROTOCOL_IND
CsrSize CsrBtSapsSetTransferProtocolIndSizeof(void *msg);
CsrUint8 *CsrBtSapsSetTransferProtocolIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapsSetTransferProtocolIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSapsSetTransferProtocolIndSerFree CsrBtSapsPfree
#endif /* EXCLUDE_CSR_BT_SAPS_SET_TRANSFER_PROTOCOL_IND */

#ifndef EXCLUDE_CSR_BT_SAPS_TRANSFER_APDU_RES
CsrSize CsrBtSapsTransferApduResSizeof(void *msg);
CsrUint8 *CsrBtSapsTransferApduResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapsTransferApduResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSapsTransferApduResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SAPS_TRANSFER_APDU_RES */

#ifndef EXCLUDE_CSR_BT_SAPS_HOUSE_CLEANING
CsrSize CsrBtSapsHouseCleaningSizeof(void *msg);
CsrUint8 *CsrBtSapsHouseCleaningSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapsHouseCleaningDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSapsHouseCleaningSerFree CsrBtSapsPfree
#endif /* EXCLUDE_CSR_BT_SAPS_HOUSE_CLEANING */

#ifdef __cplusplus
}
#endif

#endif
