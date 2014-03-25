#ifndef CSR_BT_SAPC_SERIALIZE_H__
#define CSR_BT_SAPC_SERIALIZE_H__
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

void CsrBtSapcPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_SAPC_POWER_SIM_OFF_REQ
CsrSize CsrBtSapcPowerSimOffReqSizeof(void *msg);
CsrUint8 *CsrBtSapcPowerSimOffReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapcPowerSimOffReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSapcPowerSimOffReqSerFree CsrBtSapcPfree
#endif /* EXCLUDE_CSR_BT_SAPC_POWER_SIM_OFF_REQ */

#ifndef EXCLUDE_CSR_BT_SAPC_HOUSE_CLEANING
CsrSize CsrBtSapcHouseCleaningSizeof(void *msg);
CsrUint8 *CsrBtSapcHouseCleaningSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapcHouseCleaningDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSapcHouseCleaningSerFree CsrBtSapcPfree
#endif /* EXCLUDE_CSR_BT_SAPC_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_SAPC_TRANSFER_CARD_READER_STATUS_CFM
CsrSize CsrBtSapcTransferCardReaderStatusCfmSizeof(void *msg);
CsrUint8 *CsrBtSapcTransferCardReaderStatusCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapcTransferCardReaderStatusCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSapcTransferCardReaderStatusCfmSerFree CsrBtSapcPfree
#endif /* EXCLUDE_CSR_BT_SAPC_TRANSFER_CARD_READER_STATUS_CFM */

#ifndef EXCLUDE_CSR_BT_SAPC_TRANSFER_APDU_REQ
CsrSize CsrBtSapcTransferApduReqSizeof(void *msg);
CsrUint8 *CsrBtSapcTransferApduReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapcTransferApduReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSapcTransferApduReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SAPC_TRANSFER_APDU_REQ */

#ifndef EXCLUDE_CSR_BT_SAPC_DISCONNECT_IND
CsrSize CsrBtSapcDisconnectIndSizeof(void *msg);
CsrUint8 *CsrBtSapcDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapcDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSapcDisconnectIndSerFree CsrBtSapcPfree
#endif /* EXCLUDE_CSR_BT_SAPC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SAPC_CONNECT_REQ
CsrSize CsrBtSapcConnectReqSizeof(void *msg);
CsrUint8 *CsrBtSapcConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapcConnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSapcConnectReqSerFree CsrBtSapcPfree
#endif /* EXCLUDE_CSR_BT_SAPC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SAPC_TRANSFER_APDU_CFM
CsrSize CsrBtSapcTransferApduCfmSizeof(void *msg);
CsrUint8 *CsrBtSapcTransferApduCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapcTransferApduCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSapcTransferApduCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SAPC_TRANSFER_APDU_CFM */

#ifndef EXCLUDE_CSR_BT_SAPC_SET_TRANSFER_PROTOCOL_CFM
CsrSize CsrBtSapcSetTransferProtocolCfmSizeof(void *msg);
CsrUint8 *CsrBtSapcSetTransferProtocolCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapcSetTransferProtocolCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSapcSetTransferProtocolCfmSerFree CsrBtSapcPfree
#endif /* EXCLUDE_CSR_BT_SAPC_SET_TRANSFER_PROTOCOL_CFM */

#ifndef EXCLUDE_CSR_BT_SAPC_STATUS_IND
CsrSize CsrBtSapcStatusIndSizeof(void *msg);
CsrUint8 *CsrBtSapcStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapcStatusIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSapcStatusIndSerFree CsrBtSapcPfree
#endif /* EXCLUDE_CSR_BT_SAPC_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_SAPC_POWER_SIM_ON_REQ
CsrSize CsrBtSapcPowerSimOnReqSizeof(void *msg);
CsrUint8 *CsrBtSapcPowerSimOnReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapcPowerSimOnReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSapcPowerSimOnReqSerFree CsrBtSapcPfree
#endif /* EXCLUDE_CSR_BT_SAPC_POWER_SIM_ON_REQ */

#ifndef EXCLUDE_CSR_BT_SAPC_TRANSFER_ATR_CFM
CsrSize CsrBtSapcTransferAtrCfmSizeof(void *msg);
CsrUint8 *CsrBtSapcTransferAtrCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapcTransferAtrCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtSapcTransferAtrCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_SAPC_TRANSFER_ATR_CFM */

#ifndef EXCLUDE_CSR_BT_SAPC_RESET_SIM_REQ
CsrSize CsrBtSapcResetSimReqSizeof(void *msg);
CsrUint8 *CsrBtSapcResetSimReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapcResetSimReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSapcResetSimReqSerFree CsrBtSapcPfree
#endif /* EXCLUDE_CSR_BT_SAPC_RESET_SIM_REQ */

#ifndef EXCLUDE_CSR_BT_SAPC_SECURITY_OUT_REQ
CsrSize CsrBtSapcSecurityOutReqSizeof(void *msg);
CsrUint8 *CsrBtSapcSecurityOutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapcSecurityOutReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSapcSecurityOutReqSerFree CsrBtSapcPfree
#endif /* EXCLUDE_CSR_BT_SAPC_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_SAPC_SET_TRANSFER_PROTOCOL_REQ
CsrSize CsrBtSapcSetTransferProtocolReqSizeof(void *msg);
CsrUint8 *CsrBtSapcSetTransferProtocolReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapcSetTransferProtocolReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSapcSetTransferProtocolReqSerFree CsrBtSapcPfree
#endif /* EXCLUDE_CSR_BT_SAPC_SET_TRANSFER_PROTOCOL_REQ */

#ifndef EXCLUDE_CSR_BT_SAPC_TRANSFER_CARD_READER_STATUS_REQ
CsrSize CsrBtSapcTransferCardReaderStatusReqSizeof(void *msg);
CsrUint8 *CsrBtSapcTransferCardReaderStatusReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapcTransferCardReaderStatusReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSapcTransferCardReaderStatusReqSerFree CsrBtSapcPfree
#endif /* EXCLUDE_CSR_BT_SAPC_TRANSFER_CARD_READER_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_SAPC_CONNECT_CFM
CsrSize CsrBtSapcConnectCfmSizeof(void *msg);
CsrUint8 *CsrBtSapcConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapcConnectCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSapcConnectCfmSerFree CsrBtSapcPfree
#endif /* EXCLUDE_CSR_BT_SAPC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_SAPC_TRANSFER_ATR_REQ
CsrSize CsrBtSapcTransferAtrReqSizeof(void *msg);
CsrUint8 *CsrBtSapcTransferAtrReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapcTransferAtrReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSapcTransferAtrReqSerFree CsrBtSapcPfree
#endif /* EXCLUDE_CSR_BT_SAPC_TRANSFER_ATR_REQ */

#ifndef EXCLUDE_CSR_BT_SAPC_POWER_SIM_ON_CFM
CsrSize CsrBtSapcPowerSimOnCfmSizeof(void *msg);
CsrUint8 *CsrBtSapcPowerSimOnCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapcPowerSimOnCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSapcPowerSimOnCfmSerFree CsrBtSapcPfree
#endif /* EXCLUDE_CSR_BT_SAPC_POWER_SIM_ON_CFM */

#ifndef EXCLUDE_CSR_BT_SAPC_POWER_SIM_OFF_CFM
CsrSize CsrBtSapcPowerSimOffCfmSizeof(void *msg);
CsrUint8 *CsrBtSapcPowerSimOffCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapcPowerSimOffCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSapcPowerSimOffCfmSerFree CsrBtSapcPfree
#endif /* EXCLUDE_CSR_BT_SAPC_POWER_SIM_OFF_CFM */

#ifndef EXCLUDE_CSR_BT_SAPC_RESET_SIM_CFM
CsrSize CsrBtSapcResetSimCfmSizeof(void *msg);
CsrUint8 *CsrBtSapcResetSimCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapcResetSimCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSapcResetSimCfmSerFree CsrBtSapcPfree
#endif /* EXCLUDE_CSR_BT_SAPC_RESET_SIM_CFM */

#ifndef EXCLUDE_CSR_BT_SAPC_SECURITY_OUT_CFM
CsrSize CsrBtSapcSecurityOutCfmSizeof(void *msg);
CsrUint8 *CsrBtSapcSecurityOutCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapcSecurityOutCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSapcSecurityOutCfmSerFree CsrBtSapcPfree
#endif /* EXCLUDE_CSR_BT_SAPC_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_SAPC_DISCONNECT_REQ
CsrSize CsrBtSapcDisconnectReqSizeof(void *msg);
CsrUint8 *CsrBtSapcDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtSapcDisconnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtSapcDisconnectReqSerFree CsrBtSapcPfree
#endif /* EXCLUDE_CSR_BT_SAPC_DISCONNECT_REQ */

#ifdef __cplusplus
}
#endif

#endif
